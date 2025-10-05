#include "pch.h"
#include "CModClickShow.h"
#include "CVeVisualContainer.h"
#include "VeDef.h"

HRESULT CModClickShow::ModInit(CVeVisualContainer* pContainer)
{
    constexpr D2D1_ELLIPSE Circle
    {
        { ClickGeoRadius,ClickGeoRadius },
        ClickGeoRadius,ClickGeoRadius
    };
    eck::g_pD2dFactory->CreateEllipseGeometry(Circle, &m_pEllipseClick);

    ComPtr<ID2D1StrokeStyle1> pStrokeStyle;
    D2D1_STROKE_STYLE_PROPERTIES1 StrokeStyle{
        .transformType = D2D1_STROKE_TRANSFORM_TYPE_FIXED };
    eck::g_pD2dFactory->CreateStrokeStyle(StrokeStyle, nullptr, 0, &m_pStrokeStyleClick);
    return __super::ModInit(pContainer);
}

void CModClickShow::ModUnInit()
{
    m_pEllipseClick.Clear();
    m_pStrokeStyleClick.Clear();
    m_vClick.clear();
    __super::ModUnInit();
}

void CModClickShow::ModOnAppEvent(MOD_APPEVT_PARAM& Opt)
{
    switch (Opt.eNotify)
    {
    case Notify::GlobalMouseDown:
    {
        if (App->GetOpt().bShowClick)
        {
            ClickState eState;
            switch (Opt.n.Vk)
            {
            case VK_LBUTTON: eState = ClickState::L; break;
            case VK_RBUTTON: eState = ClickState::R; break;
            case VK_MBUTTON: eState = ClickState::M; break;
            default: goto NoClick;
            }
            m_pContainer->UpdateCursorPos();// 先更新一次，防止脏坐标
            if (m_vClick.empty())
                Opt.bWakeRenderThread = TRUE;
            m_vClick.emplace_back(NtGetTickCount64(),
                m_pContainer->GetCurrentCursorPos(), eState);
        }
    NoClick:;
    }
    break;
    case Notify::GlobalMouseUp:
    {
        // 点击显示
        if (App->GetOpt().bShowClick)
        {
            ClickState eState;
            switch (Opt.n.Vk)
            {
            case VK_LBUTTON: eState = ClickState::L; break;
            case VK_RBUTTON: eState = ClickState::R; break;
            case VK_MBUTTON: eState = ClickState::M; break;
            default: goto NoClick1;
            }
            m_pContainer->UpdateCursorPos();// 先更新一次，防止脏坐标
            constexpr ULONGLONG MinInterval = 300;
            const auto tCurr = NtGetTickCount64();
            const auto dx = eck::DaGetSystemMetrics(SM_CXDRAG,
                m_pContainer->GetWnd()->GetUserDpi());
            const auto dy = eck::DaGetSystemMetrics(SM_CYDRAG,
                m_pContainer->GetWnd()->GetUserDpi());
            const auto& ptCursor = m_pContainer->GetCurrentCursorPos();
            for (const auto& e : m_vClick)
                if (e.eState == eState && !e.bUp)
                {
                    // 仅当满足以下条件之一时才列队当前放开事件
                    // 1. 按下与放开事件发生间隔大于MinInterval
                    // 2. 按下与放开事件发生在不同位置
                    const auto bTooFrequent = (tCurr - e.tAdd < MinInterval);
                    const auto bTooNear = (fabs(e.ptCenter.x - ptCursor.x) <= dx) ||
                        (fabs(e.ptCenter.y - ptCursor.y) <= dy);
                    if (bTooFrequent && bTooNear)
                        goto NoClick1;
                }
            if (m_vClick.empty())
                Opt.bWakeRenderThread = TRUE;
            m_vClick.emplace_back(NtGetTickCount64(), ptCursor, eState, TRUE);
        }
    NoClick1:;
    }
    break;
    }
}

void CModClickShow::ModOnRender(const MOD_RENDER_PARAM& Opt)
{
    if (!App->GetOpt().bShowClick)
        return;
    D2D1::Matrix3x2F MatOld;
    Opt.pDC->GetTransform(&MatOld);
    for (const auto& e : m_vClick)
    {
        switch (e.eState)
        {
        case ClickState::L:
            Opt.pBrush->SetColor(D2D1_COLOR_F{ 1.f,0.f,1.f,e.fAlpha });
            break;
        case ClickState::M:
            Opt.pBrush->SetColor(D2D1_COLOR_F{ 0.f,1.f,0.f,e.fAlpha });
            break;
        case ClickState::R:
            Opt.pBrush->SetColor(D2D1_COLOR_F{ 0.f,1.f,1.f,e.fAlpha });
            break;
        default: ECK_UNREACHABLE;
        }
        const auto fScale = e.fRadius / ClickGeoRadius;
        Opt.pDC->SetTransform(
            D2D1::Matrix3x2F::Translation(
                e.ptCenter.x - ClickGeoRadius,
                e.ptCenter.y - ClickGeoRadius) *
            D2D1::Matrix3x2F::Scale(
                fScale, fScale,
                { e.ptCenter.x,e.ptCenter.y }) *
            MatOld);
        const D2D1_ELLIPSE Ellipse{
            { e.ptCenter.x,e.ptCenter.y },
            e.fRadius,e.fRadius };
        Opt.pDC->DrawGeometry(m_pEllipseClick.Get(), Opt.pBrush,
            (float)VeCxClickStroke, m_pStrokeStyleClick.Get());
    }
    Opt.pDC->SetTransform(MatOld);
}

void CModClickShow::ModOnTick(MOD_TICK_PARAM& Opt)
{
    if (m_vClick.empty())
        return;
    for (size_t i{ m_vClick.size() }; i; --i)
    {
        auto& e = m_vClick[i - 1];
        e.ms += Opt.ms;
        const auto k = eck::Easing::OutCubic(e.ms, 0.f, 1.f, 500.f);
        if (k >= 1.f)
        {
            const auto r = e.fRadius + (float)VeCxClickStroke;
            eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate, {
                e.ptCenter.x - r, e.ptCenter.y - r,
                e.ptCenter.x + r, e.ptCenter.y + r });
            m_vClick.erase(m_vClick.begin() + i - 1);
            continue;
        }
        e.fRadius = ((e.bUp ? App->GetOpt().fClickRadiusUp :
            App->GetOpt().fClickRadius) * k);
        e.fAlpha = 1.f - k;
        const auto r = e.fRadius + (float)VeCxClickStroke;
        eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate, {
            e.ptCenter.x - r, e.ptCenter.y - r,
            e.ptCenter.x + r, e.ptCenter.y + r });
    }
}