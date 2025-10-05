#include "pch.h"
#include "CModCrosshair.h"
#include "CVeVisualContainer.h"

void CModCrosshair::ModOnAppEvent(MOD_APPEVT_PARAM& Opt)
{
    switch (Opt.eNotify)
    {
    case Notify::GlobalMouseMove:
    {
        if (App->GetOpt().bCrosshair)
        {
            eck::UnionRect(Opt.rcUpdate, Opt.rcUpdate, m_pContainer->GetViewRectF());
            Opt.bUpdate = TRUE;
        }
    }
    break;
    }
}

void CModCrosshair::ModOnRender(const MOD_RENDER_PARAM& Opt)
{
    if (!App->GetOpt().bCrosshair)
        return;
    ID2D1SolidColorBrush* pBr;
    if (App->GetOpt().bRainbowColor)
        pBr = Opt.pBrRainbow;
    else
    {
        Opt.pBrush->SetColor(App->GetColor(CApp::CrLine));
        pBr = Opt.pBrush;
    }
    const auto d = App->GetOpt().dCrosshairCursorGap;
    const auto cxLine = App->GetOpt().cxCrosshairLine;
    D2D1_POINT_2F pt1, pt2;

    POINT ptCursor;
    GetCursorPos(&ptCursor);
    ScreenToClient(m_pContainer->GetWnd()->HWnd, &ptCursor);
    m_pContainer->GetWnd()->Phy2Log(ptCursor);
    m_pContainer->ClientToElem(ptCursor);

    if (ptCursor.y >= Opt.ps.rcfClipInElem.top - cxLine &&
        ptCursor.y <= Opt.ps.rcfClipInElem.bottom + cxLine)
    {
        pt1.y = (float)ptCursor.y;
        pt2.y = (float)ptCursor.y;
        pt1.x = Opt.ps.rcfClipInElem.left;
        pt2.x = (float)ptCursor.x - d;
        Opt.pDC->DrawLine(pt1, pt2, pBr, cxLine);
        pt1.x = (float)ptCursor.x + d;
        pt2.x = Opt.ps.rcfClipInElem.right;
        Opt.pDC->DrawLine(pt1, pt2, pBr, cxLine);
    }
    if (ptCursor.x >= Opt.ps.rcfClipInElem.left - cxLine &&
        ptCursor.x <= Opt.ps.rcfClipInElem.right + cxLine)
    {
        pt1.x = (float)ptCursor.x;
        pt2.x = (float)ptCursor.x;
        pt1.y = Opt.ps.rcfClipInElem.top;
        pt2.y = (float)ptCursor.y - d;
        Opt.pDC->DrawLine(pt1, pt2, pBr, cxLine);
        pt1.y = (float)ptCursor.y + d;
        pt2.y = Opt.ps.rcfClipInElem.bottom;
        Opt.pDC->DrawLine(pt1, pt2, pBr, cxLine);
    }
}