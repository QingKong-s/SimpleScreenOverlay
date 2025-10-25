#include "pch.h"
#include "CModCursorLocation.h"
#include "CVeVisualContainer.h"

void CModCursorLocation::ReCreateGeometry()
{
    ComPtr<ID2D1EllipseGeometry> pGeoCircle;
    constexpr D2D1_ELLIPSE Circle
    {
        { CursorLocateGeoRadius,CursorLocateGeoRadius },
        CursorLocateGeoRadius,CursorLocateGeoRadius
    };
    eck::g_pD2dFactory->CreateEllipseGeometry(Circle, &pGeoCircle);
    const auto pDC = m_pContainer->GetDC1();
    float xDpi, yDpi;
    pDC->GetDpi(&xDpi, &yDpi);
    pDC->CreateFilledGeometryRealization(pGeoCircle.Get(),
        D2D1::ComputeFlatteningTolerance(
            D2D1::Matrix3x2F::Identity(), xDpi, yDpi, 1.f),
        m_pGrCursorLocate.AddrOfClear());
}

void CModCursorLocation::ReCreateBrush()
{
    const auto pDC = m_pContainer->GetDC1();
    ComPtr<ID2D1GradientStopCollection> pStop;
    constexpr D2D1_GRADIENT_STOP Stop[]
    {
        { 0.f,{} },
        { 0.8f,{} },
        { 0.94f,{ 1.f,1.f,1.f,0.5f } }
    };
    pDC->CreateGradientStopCollection(Stop, ARRAYSIZE(Stop),
        D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &pStop);
    constexpr D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES BrushProp
    {
        { CursorLocateGeoRadius,CursorLocateGeoRadius },{},
        CursorLocateGeoRadius,CursorLocateGeoRadius
    };
    pDC->CreateRadialGradientBrush(BrushProp,
        pStop.Get(), m_pBrCursorLocate.AddrOfClear());
}

HRESULT CModCursorLocation::ModInit(CVeVisualContainer* pContainer)
{
    __super::ModInit(pContainer);
    m_hSlot = pContainer->GetSignal().Connect(
        [&](UINT uMsg, WPARAM wParam, LPARAM lParam, eck::SlotCtx& Ctx) -> LRESULT
        {
            switch (uMsg)
            {
            case WM_DPICHANGED:
                ReCreateGeometry();
                break;
            }
            return 0;
        });
    ReCreateBrush();
    ReCreateGeometry();
    return S_OK;
}

void CModCursorLocation::ModUnInit()
{
    m_pContainer->GetSignal().Disconnect(m_hSlot);
    m_hSlot = nullptr;
    m_pGrCursorLocate.Clear();
    m_pBrCursorLocate.Clear();
    __super::ModUnInit();
}

void CModCursorLocation::ModOnAppEvent(MOD_APPEVT_PARAM& Opt)
{
    switch (Opt.eNotify)
    {
    case Notify::SingleCtrl:
    {
        if (App->GetOpt().bLocateCursorWithCtrl)
        {
            m_pContainer->UpdateCursorPos();// 先更新一次，防止脏坐标

            m_bLocatingCursor = TRUE;
            m_msCursorLocateAni = 0.f;
            Opt.bWakeRenderThread = TRUE;
        }
    }
    break;
    case Notify::GlobalMouseMove:
    {
        if (m_bLocatingCursor && App->GetOpt().bLocateCursorWithCtrl)
        {
            eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate, {
                    std::min(Opt.ptOldCursor.x, Opt.ptNewCursor.x) - m_fCursorLocateRadius,
                    std::min(Opt.ptOldCursor.y, Opt.ptNewCursor.y) - m_fCursorLocateRadius,
                    std::max(Opt.ptOldCursor.x, Opt.ptNewCursor.x) + m_fCursorLocateRadius,
                    std::max(Opt.ptOldCursor.y, Opt.ptNewCursor.y) + m_fCursorLocateRadius
                });
            Opt.bUpdate = TRUE;
        }
    }
    break;
    }
}

void CModCursorLocation::ModOnRender(const MOD_RENDER_PARAM& Opt)
{
    if (!(m_bLocatingCursor && App->GetOpt().bLocateCursorWithCtrl))
        return;
    D2D1::Matrix3x2F MatOld;
    Opt.pDC->GetTransform(&MatOld);
    const auto fScale = m_fCursorLocateRadius / CursorLocateGeoRadius;
    const auto& ptCursor = m_pContainer->GetCurrentCursorPos();
    Opt.pDC->SetTransform(
        D2D1::Matrix3x2F::Translation(
            ptCursor.x - CursorLocateGeoRadius,
            ptCursor.y - CursorLocateGeoRadius) *
        D2D1::Matrix3x2F::Scale(
            fScale, fScale,
            { ptCursor.x,ptCursor.y }) *
        MatOld);
    m_pBrCursorLocate->SetOpacity(m_fCursorLocateAlpha);
    Opt.pDC->DrawGeometryRealization(m_pGrCursorLocate.Get(), m_pBrCursorLocate.Get());
    Opt.pDC->SetTransform(MatOld);
}

void CModCursorLocation::ModOnTick(MOD_TICK_PARAM& Opt)
{
    if (m_bLocatingCursor)
    {
        m_msCursorLocateAni += Opt.ms;
        auto k = eck::Easing::OutCubic(m_msCursorLocateAni, 0.f, 1.f, 700.f);
        if (k >= 1.f)
        {
            m_bLocatingCursor = FALSE;
            k = 1.f;
        }
        m_fCursorLocateAlpha = 1.f - k;
        const auto& ptCursor = m_pContainer->GetCurrentCursorPos();
        const auto rOld = m_fCursorLocateRadius;
        m_fCursorLocateRadius = App->GetOpt().fLocateCursorRadius * (1.f - k);
        const auto rMin = std::min(m_fCursorLocateRadius, rOld);
        const auto rMax = std::max(m_fCursorLocateRadius, rOld);

        eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate, {
            ptCursor.x - rMax,ptCursor.y - rMax,
            ptCursor.x + rMin,ptCursor.y + rMin });
    }
}