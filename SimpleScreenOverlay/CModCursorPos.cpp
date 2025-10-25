#include "pch.h"
#include "CModCursorPos.h"
#include "CVeVisualContainer.h"

void CModCursorPos::ReCreateGeometry()
{
    ComPtr<ID2D1EllipseGeometry> pGeoCircle;
    constexpr D2D1_ELLIPSE Circle
    {
        { CursorGeoRadius,CursorGeoRadius },
        CursorGeoRadius,CursorGeoRadius
    };
    eck::g_pD2dFactory->CreateEllipseGeometry(Circle, &pGeoCircle);
    const auto pDC = m_pContainer->GetDC1();
    float xDpi, yDpi;
    pDC->GetDpi(&xDpi, &yDpi);
    pDC->CreateFilledGeometryRealization(pGeoCircle.Get(),
        D2D1::ComputeFlatteningTolerance(
            D2D1::Matrix3x2F::Identity(), xDpi, yDpi, 1.f),
        m_pGrCursorPos.AddrOfClear());
}

HRESULT CModCursorPos::ModInit(CVeVisualContainer* pContainer)
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
    ReCreateGeometry();
    return S_OK;
}

void CModCursorPos::ModUnInit()
{
    m_pContainer->GetSignal().Disconnect(m_hSlot);
    m_hSlot = nullptr;
    m_pGrCursorPos.Clear();
    __super::ModUnInit();
}

void CModCursorPos::ModOnAppEvent(MOD_APPEVT_PARAM& Opt)
{
    switch (Opt.eNotify)
    {
    case Notify::GlobalMouseMove:
    {
        if (App->GetOpt().bShowCursorPos)
        {
            const auto r = App->GetOpt().fCursorPosRadius;
            eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate, {
                    std::min(Opt.ptOldCursor.x, Opt.ptNewCursor.x) - r,
                    std::min(Opt.ptOldCursor.y, Opt.ptNewCursor.y) - r,
                    std::max(Opt.ptOldCursor.x, Opt.ptNewCursor.x) + r,
                    std::max(Opt.ptOldCursor.y, Opt.ptNewCursor.y) + r
                });
            Opt.bUpdate = TRUE;
        }
    }
    break;
    }
}

void CModCursorPos::ModOnRender(const MOD_RENDER_PARAM& Opt)
{
    if (!App->GetOpt().bShowCursorPos)
        return;
    const auto& ptCursor = m_pContainer->GetCurrentCursorPos();
    D2D1::Matrix3x2F MatOld;
    Opt.pDC->GetTransform(&MatOld);
    const auto fScale =
        App->GetOpt().fCursorPosRadius / CursorGeoRadius;
    Opt.pDC->SetTransform(
        D2D1::Matrix3x2F::Translation(
            ptCursor.x - CursorGeoRadius,
            ptCursor.y - CursorGeoRadius) *
        D2D1::Matrix3x2F::Scale(
            fScale, fScale,
            { ptCursor.x,ptCursor.y }) *
        MatOld);
    Opt.pBrush->SetColor(D2D1_COLOR_F{ 1.f,1.f,0.f,0.2f });
    Opt.pDC->DrawGeometryRealization(m_pGrCursorPos.Get(), Opt.pBrush);
    Opt.pDC->SetTransform(MatOld);
}