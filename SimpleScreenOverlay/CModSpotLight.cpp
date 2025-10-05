#include "pch.h"
#include "CModSpotLight.h"
#include "CVeVisualContainer.h"

void CModSpotLight::ReCreateGeometry()
{
    ComPtr<ID2D1EllipseGeometry> pGeoCircle;
    constexpr D2D1_ELLIPSE Circle
    {
        { SpotLightGeoRadius,SpotLightGeoRadius },
        SpotLightGeoRadius,SpotLightGeoRadius
    };
    eck::g_pD2dFactory->CreateEllipseGeometry(Circle, &pGeoCircle);
    const auto pDC = m_pContainer->GetDC1();
    float xDpi, yDpi;
    pDC->GetDpi(&xDpi, &yDpi);
    pDC->CreateFilledGeometryRealization(pGeoCircle.Get(),
        D2D1::ComputeFlatteningTolerance(
            D2D1::Matrix3x2F::Identity(), xDpi, yDpi, 1.f),
        m_pGrSpotLight.AddrOfClear());
}

HRESULT CModSpotLight::ModInit(CVeVisualContainer* pContainer)
{
    __super::ModInit(pContainer);
    m_hSlot = pContainer->GetSignal().Connect(
        [&](UINT uMsg, WPARAM wParam, LPARAM lParam, eck::SlotCtx& Ctx) -> LRESULT
        {
            switch (uMsg)
            {
            case WM_SIZE:
                m_fSpotLightMaxRadius = std::max(
                    m_pContainer->GetWidthF(), m_pContainer->GetHeightF()) / 2.f;
                break;
            case WM_DPICHANGED:
                ReCreateGeometry();
                break;
            }
            return 0;
        });
    ReCreateGeometry();
    return S_OK;
}

void CModSpotLight::ModUnInit()
{
    m_pContainer->GetSignal().Disconnect(m_hSlot);
    m_hSlot = nullptr;
    m_pGrSpotLight.Clear();
    __super::ModUnInit();
}

void CModSpotLight::ModOnAppEvent(MOD_APPEVT_PARAM& Opt)
{
    switch (Opt.eNotify)
    {
    case Notify::DoubleCtrl:
    {
        if (App->GetOpt().bSpotLight)
        {
            m_pContainer->UpdateCursorPos();// 先更新一次，防止脏坐标

            ECKBOOLNOT(m_bShowSpotLight);
            m_bSpotLightReverse = !!!m_bShowSpotLight;
            m_bSpotLightAnimating = TRUE;
            Opt.bWakeRenderThread = TRUE;
        }
    }
    break;
    case Notify::GlobalKeyDown:
    {
        if (App->GetOpt().bSpotLight && m_bShowSpotLight)
        {
            m_bShowSpotLight = FALSE;
            m_bSpotLightReverse = TRUE;
            m_bSpotLightAnimating = TRUE;
            Opt.bWakeRenderThread = TRUE;
        }
    }
    break;
    case Notify::GlobalMouseDown:
    {
        if (App->GetOpt().bSpotLight && m_bShowSpotLight)
        {
            m_bShowSpotLight = FALSE;
            m_bSpotLightReverse = TRUE;
            m_bSpotLightAnimating = TRUE;
            Opt.bWakeRenderThread = TRUE;
        }
    }
    break;
    case Notify::GlobalMouseMove:
    {
        if (m_bShowSpotLight && App->GetOpt().bSpotLight)
        {
            const auto r = m_fSpotLightMaxRadius * m_kSpotLight;
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

void CModSpotLight::ModOnRender(const MOD_RENDER_PARAM& Opt)
{
    if (!(App->GetOpt().bSpotLight &&
        (m_bShowSpotLight || m_bSpotLightAnimating)))
        return;
    auto cr{ App->GetColor(CApp::CrSpotLightBkg) };
    cr.a *= m_kSpotLight;
    Opt.pBrush->SetColor(cr);
    Opt.pDC->FillRectangle(Opt.ps.rcfClipInElem, Opt.pBrush);

    const auto eOldBlend = Opt.pDC->GetPrimitiveBlend();
    Opt.pDC->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
    Opt.pBrush->SetColor(D2D1_COLOR_F{});
    D2D1::Matrix3x2F MatOld;
    Opt.pDC->GetTransform(&MatOld);
    const auto fScale = std::max(
        App->GetOpt().fSpotLightRadius,
        m_fSpotLightMaxRadius * (1.f - m_kSpotLight)) / SpotLightGeoRadius;
    const auto ptCursor = m_pContainer->GetCurrentCursorPos();
    Opt.pDC->SetTransform(
        D2D1::Matrix3x2F::Translation(
            ptCursor.x - SpotLightGeoRadius,
            ptCursor.y - SpotLightGeoRadius) *
        D2D1::Matrix3x2F::Scale(
            fScale, fScale,
            { ptCursor.x,ptCursor.y }) *
        MatOld);
    Opt.pDC->DrawGeometryRealization(m_pGrSpotLight.Get(), Opt.pBrush);
    Opt.pDC->SetTransform(MatOld);
    Opt.pDC->SetPrimitiveBlend(eOldBlend);
}

void CModSpotLight::ModOnTick(MOD_TICK_PARAM& Opt)
{
    if (m_bSpotLightAnimating)
    {
        if (m_bSpotLightReverse)
            m_msSpotLightAni -= (float)Opt.ms;
        else
            m_msSpotLightAni += (float)Opt.ms;
        m_kSpotLight = eck::Easing::OutCubic(m_msSpotLightAni, 0.f, 1.f, 500.f);
        if (m_kSpotLight >= 1.f || m_kSpotLight <= 0.f)
        {
            m_kSpotLight = m_bSpotLightReverse ? 0.f : 1.f;
            m_msSpotLightAni = m_bSpotLightReverse ? 0.f : 500.f;
            m_bSpotLightAnimating = FALSE;
        }
        eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate, m_pContainer->GetViewRectF());
    }
}