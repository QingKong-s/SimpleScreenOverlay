#include "pch.h"
#include "CModWheelIndicator.h"
#include "CWndMain.h"
#include "VeDef.h"


void CModWheelIndicator::ReCreateGeometry()
{
    ComPtr<ID2D1PathGeometry1> pPath;
    eck::g_pD2dFactory->CreatePathGeometry(&pPath);
    ComPtr<ID2D1GeometrySink> pSink;
    pPath->Open(&pSink);
    constexpr auto y = WheelGeoSide * 1.73205f / 2.f;
    pSink->BeginFigure({ WheelGeoSide / 2.f,0.f }, D2D1_FIGURE_BEGIN_FILLED);
    pSink->AddLine({ 0.f,y });
    pSink->AddLine({ WheelGeoSide,y });
    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
    pSink->Close();

    const auto pDC = m_pContainer->GetDC1();
    float xDpi, yDpi;
    pDC->GetDpi(&xDpi, &yDpi);

    pDC->CreateFilledGeometryRealization(pPath.Get(),
        D2D1::ComputeFlatteningTolerance(
            D2D1::Matrix3x2F::Identity(), xDpi, yDpi, 1.f),
        m_pGrWheel.AddrOfClear());
    pDC->CreateStrokedGeometryRealization(pPath.Get(),
        D2D1::ComputeFlatteningTolerance(
            D2D1::Matrix3x2F::Identity(), xDpi, yDpi, 1.f),
        WheelGeoSide / (float)VeCxWheelIndicator, nullptr,
        m_pGrWheelBorder.AddrOfClear());
}

void CModWheelIndicator::UpdateWheelIndicatorPos()
{
    const auto pWnd = (CWndMain*)m_pContainer->GetWnd();
    const auto cxCursor = eck::DpiScaleF((float)pWnd->GetCursorSize().cx,
        96, pWnd->GetUserDpi());
    const auto cyCursor = eck::DpiScaleF((float)pWnd->GetCursorSize().cy,
        96, pWnd->GetUserDpi());
    const auto& ptCursor = m_pContainer->GetCurrentCursorPos();
    if (ptCursor.y - cyCursor / 2.f < 0.f)
        m_ptWheelIndicator.y = ptCursor.y + cyCursor;
    else
        m_ptWheelIndicator.y = ptCursor.y - cyCursor / 2.f;
    if (ptCursor.x + cxCursor / 2.f > pWnd->GetClientWidthLog())
        m_ptWheelIndicator.x = ptCursor.x - cxCursor;
    else
        m_ptWheelIndicator.x = ptCursor.x + cxCursor / 2.f;
}

D2D1_RECT_F CModWheelIndicator::GetWheelIndicatorRect()
{
    return
    {
        m_ptWheelIndicator.x,
        m_ptWheelIndicator.y,
        m_ptWheelIndicator.x + VeCxWheelIndicator,
        m_ptWheelIndicator.y + VeCxWheelIndicator
    };
}

HRESULT CModWheelIndicator::ModInit(CVeVisualContainer* pContainer)
{
    __super::ModInit(pContainer);
    m_hSlot = pContainer->GetSignal().Connect(
        [this](UINT uMsg, WPARAM wParam, LPARAM lParam, eck::SlotCtx& Ctx) -> LRESULT
        {
            switch (uMsg)
            {
            case WM_DPICHANGED:
                ReCreateGeometry();
                break;
            case WM_TIMER:
            {
                if (wParam == IDT_WHEEL)
                {
                    eck::CCsGuard _{ m_pContainer->GetCriticalSection() };
                    if (!m_cWheelShowCount || m_eWheel == Wheel::None)
                    {
                        m_pContainer->KillTimer(IDT_WHEEL);
                        m_bWheelTimer = FALSE;
                        return 0;
                    }
                    if (!--m_cWheelShowCount)
                    {
                        m_pContainer->KillTimer(IDT_WHEEL);
                        m_bWheelTimer = FALSE;
                        m_eWheel = Wheel::None;
                        m_pContainer->InvalidateRect(GetWheelIndicatorRect());
                    }
                }
            }
            break;
            }
            return 0;
        });
    ReCreateGeometry();
    return S_OK;
}

void CModWheelIndicator::ModUnInit()
{
    m_pContainer->GetSignal().Disconnect(m_hSlot);
    m_hSlot = nullptr;
    if (m_bWheelTimer)
    {
        m_pContainer->KillTimer(IDT_WHEEL);
        m_bWheelTimer = FALSE;
    }
    m_pGrWheel.Clear();
    m_pGrWheelBorder.Clear();
    __super::ModUnInit();
}

void CModWheelIndicator::ModOnAppEvent(MOD_APPEVT_PARAM& Opt)
{
    switch (Opt.eNotify)
    {
    case Notify::GlobalMouseMove:
    {
        if (m_eWheel != Wheel::None && App->GetOpt().bShowWheel)
        {
            constexpr auto cxy = (float)VeCxWheelIndicator;
            const auto xOld = m_ptWheelIndicator.x;
            const auto yOld = m_ptWheelIndicator.y;
            UpdateWheelIndicatorPos();
            eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate, {
                    std::min(xOld, m_ptWheelIndicator.x),
                    std::min(yOld, m_ptWheelIndicator.y),
                    std::max(xOld, m_ptWheelIndicator.x) + cxy,
                    std::max(yOld, m_ptWheelIndicator.y) + cxy
                });
            Opt.bUpdate = TRUE;
        }
    }
    break;
    case Notify::GlobalMouseWheel:
    case Notify::GlobalMouseHWheel:
    {
        if (!App->GetOpt().bShowWheel)
            break;
        m_pContainer->UpdateCursorPos();
        Wheel eWheel;
        if (Opt.eNotify == Notify::GlobalMouseWheel)
            eWheel = (Opt.n.dWheel > 0 ? Wheel::Up : Wheel::Down);
        else
            eWheel = (Opt.n.dWheel > 0 ? Wheel::Left : Wheel::Right);
        if (!m_bWheelTimer)
        {
            m_pContainer->SetTimer(IDT_WHEEL, TE_WHEEL);
            m_bWheelTimer = TRUE;
        }
        m_cWheelShowCount = 3;
        if (m_eWheel != eWheel)
        {
            m_eWheel = eWheel;
            UpdateWheelIndicatorPos();
            eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate,
                GetWheelIndicatorRect());
            Opt.bUpdate = TRUE;
        }
    }
    break;
    case Notify::CursorSettingChanged:
    {
        if (m_eWheel != Wheel::None)
        {
            UpdateWheelIndicatorPos();
            eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate,
                GetWheelIndicatorRect());
            Opt.bUpdate = TRUE;
        }
    }
    break;
    }
}

void CModWheelIndicator::ModOnRender(const MOD_RENDER_PARAM& Opt)
{
    if (m_eWheel != Wheel::None && App->GetOpt().bShowWheel)
    {
        D2D1::Matrix3x2F MatOld;
        Opt.pDC->GetTransform(&MatOld);
        const auto fAngle = (m_eWheel == Wheel::Up ? 0.f :
            m_eWheel == Wheel::Down ? 180.f :
            m_eWheel == Wheel::Left ? 90.f : -90.f);
        constexpr auto fScale = (float)VeCxWheelIndicator / WheelGeoSide;
        Opt.pDC->SetTransform(
            D2D1::Matrix3x2F::Rotation(
                fAngle, WheelGeoCenter) *
            D2D1::Matrix3x2F::Scale(
                fScale, fScale) *
            D2D1::Matrix3x2F::Translation(
                m_ptWheelIndicator.x,
                m_ptWheelIndicator.y) *
            MatOld);
        Opt.pBrush->SetColor(App->GetColor(CApp::CrWheelIndicator));
        Opt.pDC->DrawGeometryRealization(m_pGrWheel.Get(), Opt.pBrush);
        Opt.pBrush->SetColor(App->GetColor(CApp::CrWheelIndicatorBorder));
        Opt.pDC->DrawGeometryRealization(m_pGrWheelBorder.Get(), Opt.pBrush);
        Opt.pDC->SetTransform(MatOld);
    }
}
