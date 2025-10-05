#include "pch.h"
#include "CModWndMarker.h"
#include "CVeVisualContainer.h"
#include "VeDef.h"

void CModWndMarker::CalcWindowTipPos(const D2D1_RECT_F& rcWnd,
    _Out_ D2D1_POINT_2F& ptTip)
{
    ptTip.x = rcWnd.left + (float)VeCxWndTipMargin;
    ptTip.y = rcWnd.top + (float)VeCyWndTipMargin;
}

void CModWndMarker::ModUnInit()
{
    m_TcWndTip.Invalidate();
    m_TcRulerCursorTip.Invalidate();
    __super::ModUnInit();
}

void CModWndMarker::ModOnAppEvent(MOD_APPEVT_PARAM& Opt)
{
    switch (Opt.eNotify)
    {
    case Notify::GlobalMouseMove:
    {
        if (App->GetOpt().bWndHilight)
        {
            const auto hWndCursorAt = App->GetOpt().bWndHilightDetectChild ?
                eck::WndFromPoint(Opt.n.pt) : WindowFromPoint(Opt.n.pt);
            RECT rcWnd;
            GetWindowRect(hWndCursorAt, &rcWnd);
            //---更新标尺文本
            if (App->GetOpt().bRuler)
            {
                eck::CRefStrW rsRulerTip{};
                rsRulerTip.Format(L"Left = %d, Top = %d\nRight = %d, Bottom = %d",
                    Opt.n.pt.x - rcWnd.left,
                    Opt.n.pt.y - rcWnd.top,
                    rcWnd.right - Opt.n.pt.x,
                    rcWnd.bottom - Opt.n.pt.y);
                m_TcRulerCursorTip.Create(rsRulerTip.Data(), rsRulerTip.Size(),
                    m_pContainer->GetTextFormat(), m_pContainer->GetWidthF(),
                    m_pContainer->GetHeightF(), m_pContainer->GetDC1());
            }
            if (m_hWndCursorAt == hWndCursorAt &&
                eck::EquRect(rcWnd, m_rcLastWndInScreen))
                goto SkipUpdateWndHilight;
            m_rcLastWndInScreen = rcWnd;
            //---更新标签文本
            if (m_hWndCursorAt != hWndCursorAt)
            {
                eck::CRefStrW rsWndTip{};
                rsWndTip.Clear();
                auto cchText = GetWindowTextLengthW(hWndCursorAt);
                rsWndTip.PushBack(EckStrAndLen(L"Handle: 0x"))
                    .AppendFormat(L"%08X", (UINT)(UINT_PTR)hWndCursorAt);
                rsWndTip.PushBack(EckStrAndLen(L"\nCaption: "));
                auto psBuf = rsWndTip.PushBack(cchText);
                GetWindowTextW(hWndCursorAt, psBuf, cchText + 1);
                cchText = GetClassNameW(hWndCursorAt, nullptr, 0);
                rsWndTip.PushBack(EckStrAndLen(L"\nClass: "));
                const auto cchOld = rsWndTip.Size();
                rsWndTip.Reserve(rsWndTip.Size() + MAX_PATH);
                cchText = GetClassNameW(hWndCursorAt,
                    rsWndTip.PushBack(MAX_PATH),
                    MAX_PATH);
                rsWndTip.ReSize(cchOld + cchText);
                rsWndTip.AppendFormat(L"\nLeft = %d, Top = %d, Width = %d, Height = %d",
                    m_rcLastWndInScreen.left,
                    m_rcLastWndInScreen.top,
                    m_rcLastWndInScreen.right - m_rcLastWndInScreen.left,
                    m_rcLastWndInScreen.bottom - m_rcLastWndInScreen.top);
                m_TcWndTip.Create(rsWndTip.Data(), rsWndTip.Size(),
                    m_pContainer->GetTextFormat(), m_pContainer->GetWidthF(),
                    m_pContainer->GetHeightF(), m_pContainer->GetDC1());
            }
            //---
            eck::ScreenToClient(m_pContainer->GetWnd()->HWnd, &rcWnd);
            D2D1_RECT_F rcWndHili{ eck::MakeD2DRectF(rcWnd) };
            m_pContainer->GetWnd()->Phy2Log(rcWndHili);
            m_pContainer->ClientToElem(rcWndHili);
            if (!App->GetOpt().bRainbowColor && App->GetOpt().bRuler)
            {
                auto rc{ rcWndHili };
                eck::InflateRect(rc, VeCxWndHiliBorder / 2.f, VeCxWndHiliBorder / 2.f);
                eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate, rc);
            }
            if (m_hWndCursorAt)
            {
                m_hWndCursorAt = hWndCursorAt;
                m_rcWndHiliSrc = m_rcWndHili;
                m_rcWndHiliDst = rcWndHili;
                m_msWndHiliAni = 0.f;
                m_bWndHiliAnimating = TRUE;

                m_ptWndTipSrc = m_ptWndTip;
                CalcWindowTipPos(m_rcWndHiliDst, m_ptWndTipDst);
                m_msWndTipAni = 0.f;
                m_bWndTipAnimating = TRUE;
                Opt.bWakeRenderThread = TRUE;
            }
            else
            {
                m_hWndCursorAt = hWndCursorAt;
                m_rcWndHili = rcWndHili;
                CalcWindowTipPos(m_rcWndHili, m_ptWndTip);
            }
        }
    SkipUpdateWndHilight:;
    }
    break;
    }
}

void CModWndMarker::ModOnRender(const MOD_RENDER_PARAM& Opt)
{
    if (!(App->GetOpt().bWndHilight && m_hWndCursorAt))
        return;
    ID2D1SolidColorBrush* pBr;
    if (App->GetOpt().bRainbowColor)
        pBr = Opt.pBrRainbow;
    else
    {
        Opt.pBrush->SetColor(App->GetColor(CApp::CrLine));
        pBr = Opt.pBrush;
    }
    Opt.pDC->DrawRectangle(m_rcWndHili, pBr, (float)VeCxWndHiliBorder);
    if (App->GetOpt().bWndTip && m_TcWndTip.IsValid())
    {
        D2D1::Matrix3x2F Mat;
        Opt.pDC->GetTransform(&Mat);
        const auto MatNew = Mat * D2D1::Matrix3x2F::Translation(
            m_ptWndTip.x, m_ptWndTip.y);
        Opt.pDC->SetTransform(MatNew);
        Opt.pDC->DrawGeometryRealization(m_TcWndTip.GetGeometryRealization(), pBr);
        Opt.pBrush->SetColor(D2D1_COLOR_F{ .a = 0.2f });
        Opt.pDC->DrawGeometryRealization(m_TcWndTip.GetGeometryRealizationBk(),
            Opt.pBrush);
        Opt.pDC->SetTransform(Mat);
    }
    //===标尺
    if (App->GetOpt().bRuler)
    {
        if (m_TcRulerCursorTip.IsValid())
        {
            const auto& ptCursor = m_pContainer->GetCurrentCursorPos();
            D2D1::Matrix3x2F Mat;
            Opt.pDC->GetTransform(&Mat);
            const auto MatNew = Mat * D2D1::Matrix3x2F::Translation(
                ptCursor.x + (float)VeCxRulerTipMargin,
                ptCursor.y + (float)VeCyRulerTipMargin);
            Opt.pDC->SetTransform(MatNew);
            Opt.pDC->DrawGeometryRealization(m_TcRulerCursorTip.GetGeometryRealization(),
                pBr);
            Opt.pDC->SetTransform(Mat);
            D2D1_POINT_2F pt1, pt2;
            pt1.x = ptCursor.x;
            pt1.y = std::max(Opt.ps.rcfClipInElem.top, m_rcWndHili.top);
            pt2.x = ptCursor.x;
            pt2.y = std::min(Opt.ps.rcfClipInElem.bottom, m_rcWndHili.bottom);
            Opt.pDC->DrawLine(pt1, pt2, pBr);
            pt2.x = std::max(Opt.ps.rcfClipInElem.left, m_rcWndHili.left);
            pt1.y = ptCursor.y;
            pt1.x = std::min(Opt.ps.rcfClipInElem.right, m_rcWndHili.right);
            pt2.y = ptCursor.y;
            Opt.pDC->DrawLine(pt1, pt2, pBr);
        }
    }
}

void CModWndMarker::ModOnTick(MOD_TICK_PARAM& Opt)
{
    DWRITE_TEXT_METRICS tm;
    if (m_bWndHiliAnimating)
    {
        m_msWndHiliAni += Opt.ms;
        auto k = eck::Easing::OutCubic(m_msWndHiliAni, 0.f, 1.f, 150.f);
        if (k >= 1.f)
        {
            m_bWndHiliAnimating = FALSE;
            k = 1.f;
        }
        eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate, m_rcWndHili);
        m_rcWndHili.left = (m_rcWndHiliSrc.left * (1.f - k)) + (m_rcWndHiliDst.left * k);
        m_rcWndHili.top = (m_rcWndHiliSrc.top * (1.f - k)) + (m_rcWndHiliDst.top * k);
        m_rcWndHili.right = (m_rcWndHiliSrc.right * (1.f - k)) + (m_rcWndHiliDst.right * k);
        m_rcWndHili.bottom = (m_rcWndHiliSrc.bottom * (1.f - k)) + (m_rcWndHiliDst.bottom * k);
        eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate, m_rcWndHili);
        eck::InflateRect(Opt.rcUpdate, VeCxWndHiliBorder / 2.f, VeCxWndHiliBorder / 2.f);
        if (m_TcWndTip.GetTextLayout())
        {
            m_TcWndTip.GetTextLayout()->GetMetrics(&tm);
            eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate, {
                    m_ptWndTip.x,m_ptWndTip.y,
                    m_ptWndTip.x + tm.width,
                    m_ptWndTip.y + tm.height });
        }
        else
            tm.width = tm.height = 0.f;
        m_ptWndTip.x = (m_ptWndTipSrc.x * (1.f - k)) + (m_ptWndTipDst.x * k);
        m_ptWndTip.y = (m_ptWndTipSrc.y * (1.f - k)) + (m_ptWndTipDst.y * k);
        if (m_TcWndTip.GetTextLayout())
            eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate, {
                m_ptWndTip.x,m_ptWndTip.y,
                m_ptWndTip.x + tm.width,
                m_ptWndTip.y + tm.height });
    }
    else if (App->GetOpt().bWndHilight)
    {
        eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate, m_rcWndHili);
        if (m_TcWndTip.GetTextLayout())
        {
            m_TcWndTip.GetTextLayout()->GetMetrics(&tm);
            eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate, {
                    m_ptWndTip.x,m_ptWndTip.y,
                    m_ptWndTip.x + tm.width,
                    m_ptWndTip.y + tm.height });
        }
    }
}