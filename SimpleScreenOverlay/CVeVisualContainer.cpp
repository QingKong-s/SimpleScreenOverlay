#include "pch.h"
#include "CVeVisualContainer.h"
#include "CWndMain.h"

#define CALL_ALL_MOD(Func)      \
    m_ModCrosshair.Func;        \
    m_ModCursorPos.Func;        \
    m_ModCursorLocation.Func;   \
    m_ModSpotLight.Func;        \
    m_ModClickShow.Func;        \
    m_ModWndMarker.Func;

void CVeVisualContainer::OnAppEvent(Notify eNotify, const SSONOTIFY& n)
{
    ECK_DUILOCK;
    MOD_APPEVT_PARAM Opt{ .eNotify = eNotify, .n = n };
    switch (eNotify)
    {
    case Notify::GlobalMouseMove:
    {
        m_bCursorPosDirty = FALSE;
        auto pt{ n.pt };
        ScreenToClient(GetWnd()->HWnd, &pt);
        GetWnd()->Phy2Log(pt);
        ClientToElem(pt);
        Opt.ptNewCursor = { (float)pt.x,(float)pt.y };
        Opt.ptOldCursor = m_ptCursor;
        m_ptCursor = Opt.ptNewCursor;
    }
    break;
    }
    CALL_ALL_MOD(ModOnAppEvent(Opt));
    if (Opt.bUpdate)
        InvalidateRect(Opt.rcUpdate);
    else if (Opt.bWakeRenderThread)
        GetWnd()->WakeRenderThread();
}

void CVeVisualContainer::UpdateCursorPos()
{
    if (!m_bCursorPosDirty)
        return;
    m_bCursorPosDirty = FALSE;
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(GetWnd()->HWnd, &pt);
    GetWnd()->Phy2Log(pt);
    ClientToElem(pt);
    m_ptCursor = { (float)pt.x, (float)pt.y };
}

LRESULT CVeVisualContainer::OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        MOD_RENDER_PARAM Opt
        {
            .pDC = m_pDC1,
            .pBrush = m_pBrTemp,
            .pBrRainbow = m_pBrRainbow,
        };
        BeginPaint(Opt.ps, wParam, lParam);
        if (App->GetOpt().bRainbowColor)
            m_pBrRainbow->SetColor(CalcRainbowColor(NtGetTickCount64()));

        CALL_ALL_MOD(ModOnRender(Opt));
        ////===水印
        //if (App->GetOpt().bWatermark)
        //{
        //    m_pBrTemp->SetColor(App->GetColor(CApp::CrWatermark));
        //    m_pDC1->DrawGeometryRealization(m_TcWatermark.GetGeometryRealization(),
        //        m_pBrTemp);
        //}

        EndPaint(Opt.ps);
    }
    return 0;

    case WM_SIZE:
    {
        D2D1_RECT_F rcKs2;
        rcKs2.left = 0;
        rcKs2.right = GetWidthF();
        rcKs2.top = GetHeightF() - VeCyKeyStroke2 - 10;
        rcKs2.bottom = rcKs2.top + VeCyKeyStroke2;
        m_KeyStroke2.SetRect(rcKs2);
    }
    break;

    case WM_CREATE:
    {
        m_pDC->QueryInterface(&m_pDC1);
        m_pDC->CreateSolidColorBrush({}, &m_pBrTemp);
        m_pDC->CreateSolidColorBrush({}, &m_pBrRainbow);

        m_hSlot = App->GetSignal().Connect(this, &CVeVisualContainer::OnAppEvent);
        const auto pTfKeyStroke = App->CreateTextFormat(16);

        const auto pTfWatermark = App->CreateTextFormat(42);
        const auto cchWatermark = (UINT32)App->GetOpt().rsWatermark.Size();
        m_TcWatermark.CreateTextLayout(App->GetOpt().rsWatermark.Data(),
            cchWatermark, pTfWatermark, GetWidthF(), GetHeightF());
        const auto pTlWatermark = m_TcWatermark.GetTextLayout();
        IDWriteTextLayout1* pTlWatermark1;
        pTlWatermark->QueryInterface(&pTlWatermark1);
        pTlWatermark1->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        pTlWatermark1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        pTlWatermark1->Release();
        m_TcWatermark.CreateGeometryRealization(m_pDC1);
        pTfWatermark->Release();

        m_KeyStroke.Create(nullptr, Dui::DES_VISIBLE, 0,
            VeCxKeyStrokeMargin, VeCyKeyStrokeMargin,
            VeCxKeyStroke, VeCyKeyStroke, this);
        m_KeyStroke.SetTextFormat(pTfKeyStroke);

        m_KeyStroke2.Create(nullptr, Dui::DES_VISIBLE, 0,
            0, 0, 0, 0, this);
        m_KeyStroke2.SetTextFormat(pTfKeyStroke);

        pTfKeyStroke->Release();
        GetWnd()->RegisterTimeLine(this);

        CALL_ALL_MOD(ModInit(this));
    }
    break;
    case WM_DESTROY:
    {
        SafeRelease(m_pBrTemp);
        SafeRelease(m_pDC1);
        m_TcWatermark.Invalidate();
        App->GetSignal().Disconnect(m_hSlot);
        GetWnd()->UnregisterTimeLine(this);
        CALL_ALL_MOD(ModUnInit());
    }
    break;
    }
    return __super::OnEvent(uMsg, wParam, lParam);
}

void CVeVisualContainer::TlTick(int iMs)
{
    MOD_TICK_PARAM Opt{ .ms = (float)iMs };
    CALL_ALL_MOD(ModOnTick(Opt));
    InvalidateRect(Opt.rcUpdate);
}

BOOL CVeVisualContainer::TlIsValid()
{
    const CModBase* pMods[]
    {
        &m_ModCrosshair,
        &m_ModCursorPos,
        &m_ModCursorLocation,
        &m_ModSpotLight,
        &m_ModClickShow,
        &m_ModWndMarker,
    };
    for (const auto pMod : pMods)
    {
        if (pMod->ModIsValid() && pMod->ModIsTimeLineValid())
            return TRUE;
    }
    return FALSE;
}