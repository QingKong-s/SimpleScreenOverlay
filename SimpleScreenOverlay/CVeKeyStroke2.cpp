#include "pch.h"
#include "CWndMain.h"


constexpr std::wstring_view KeyName[]
{
    {},
    LR"(LMB)"sv,
    LR"(RMB)"sv,
    {},
    LR"(MMB)"sv,
    LR"(X1)"sv,
    LR"(X2)"sv,
    {},
    LR"(Back)"sv,
    LR"(Tab)"sv,
    {},
    {},
    LR"(Clear)"sv,
    LR"(Enter)"sv,
    {},
    {},
    LR"(Shift)"sv,
    LR"(Ctrl)"sv,
    LR"(Alt)"sv,
    LR"(Pause)"sv,
    LR"(Caps Lk)"sv,
    {},
    {},
    {},
    {},
    {},
    {},
    LR"(Esc)"sv,
    {},
    {},
    {},
    {},
    LR"(--)"sv,
    LR"(Pg Up)"sv,
    LR"(Pg Dn)"sv,
    LR"(End)"sv,
    LR"(Home)"sv,
    LR"(←)"sv,
    LR"(↑)"sv,
    LR"(→)"sv,
    LR"(↓)"sv,
    LR"(Select)"sv,
    LR"(Print)"sv,
    LR"(Execute)"sv,
    LR"(Prt Src)"sv,
    LR"(Ins)"sv,
    LR"(Del)"sv,
    LR"(Help)"sv,
    LR"(0)"sv,
    LR"(1)"sv,
    LR"(2)"sv,
    LR"(3)"sv,
    LR"(4)"sv,
    LR"(5)"sv,
    LR"(6)"sv,
    LR"(7)"sv,
    LR"(8)"sv,
    LR"(9)"sv,
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    LR"(A)"sv,
    LR"(B)"sv,
    LR"(C)"sv,
    LR"(D)"sv,
    LR"(E)"sv,
    LR"(F)"sv,
    LR"(G)"sv,
    LR"(H)"sv,
    LR"(I)"sv,
    LR"(J)"sv,
    LR"(K)"sv,
    LR"(L)"sv,
    LR"(M)"sv,
    LR"(N)"sv,
    LR"(O)"sv,
    LR"(P)"sv,
    LR"(Q)"sv,
    LR"(R)"sv,
    LR"(S)"sv,
    LR"(T)"sv,
    LR"(U)"sv,
    LR"(V)"sv,
    LR"(W)"sv,
    LR"(X)"sv,
    LR"(Y)"sv,
    LR"(Z)"sv,
    LR"(L Win)"sv,
    LR"(R Win)"sv,
    LR"(Apps)"sv,
    {},
    LR"(Sleep)"sv,
    LR"(N0)"sv,
    LR"(N1)"sv,
    LR"(N2)"sv,
    LR"(N3)"sv,
    LR"(N4)"sv,
    LR"(N5)"sv,
    LR"(N6)"sv,
    LR"(N7)"sv,
    LR"(N8)"sv,
    LR"(N9)"sv,
    LR"(*)"sv,
    LR"(+)"sv,
    LR"(Separator)"sv,
    LR"(-)"sv,
    LR"(.)"sv,
    LR"(/)"sv,
    LR"(F1)"sv,
    LR"(F2)"sv,
    LR"(F3)"sv,
    LR"(F4)"sv,
    LR"(F5)"sv,
    LR"(F6)"sv,
    LR"(F7)"sv,
    LR"(F8)"sv,
    LR"(F9)"sv,
    LR"(F10)"sv,
    LR"(F11)"sv,
    LR"(F12)"sv,
    LR"(F13)"sv,
    LR"(F14)"sv,
    LR"(F15)"sv,
    LR"(F16)"sv,
    LR"(F17)"sv,
    LR"(F18)"sv,
    LR"(F19)"sv,
    LR"(F20)"sv,
    LR"(F21)"sv,
    LR"(F22)"sv,
    LR"(F23)"sv,
    LR"(F24)"sv,
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    LR"(Num Lk)"sv,
    LR"(Scr Lk)"sv,
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    LR"(LShift)"sv,
    LR"(RShift)"sv,
    LR"(LCtrl)"sv,
    LR"(RCtrl)"sv,
    LR"(LAlt)"sv,
    LR"(RAlt)"sv,
    LR"(BrsBack)"sv,
    LR"(BrsFwd)"sv,
    LR"(BrsRe)"sv,
    LR"(BrsStop)"sv,
    LR"(BrsSch)"sv,
    LR"(BrsFav)"sv,
    LR"(BrsHome)"sv,
    LR"(Med Slt)"sv,
    LR"(Vol-)"sv,
    LR"(Vol+)"sv,
    LR"(Med Nxt)"sv,
    LR"(Med Prv)"sv,
    LR"(Med Stop)"sv,
    LR"(Med PP)"sv,
    LR"(EMail)"sv,
    LR"(Sel Med)"sv,
    LR"(Run1)"sv,
    LR"(Run2)"sv,
    {},
    {},
    LR"(;)"sv,// :
    LR"(=)"sv,// +
    LR"(,)"sv,// <
    LR"(-)"sv,// _
    LR"(.)"sv,// >
    LR"(/)"sv,// ?
    LR"(`)"sv,// ~
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    LR"([)"sv,// {
    LR"(\)"sv,// |
    LR"(])"sv,// }
    LR"(')"sv,// "
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    LR"(Attn)"sv,
    LR"(CrSel)"sv,
    LR"(ExSel)"sv,
    LR"(ErEOF)"sv,
    LR"(Play)"sv,
    LR"(Scale)"sv,
    {},
    LR"(PA1)"sv,
    LR"(Clear)"sv,
};

constexpr float FadeInOutAnDuration = 2000.f;
constexpr float RePosAnDuration = 1000.f;
constexpr float JumpAnDuration = 300.f;

void CVeKeyStroke2::OnAppEvent(Notify eNotify, const SSONOTIFY& n)
{
    switch (eNotify)
    {
    case Notify::GlobalKeyDown:
    case Notify::GlobalMouseDown:
    {
        if (!App->GetOpt().bKeyStroke2)
            break;
        if (n.Vk < ARRAYSIZE(KeyName) &&
            !KeyName[n.Vk].empty())
        {
            ECK_DUILOCK;
            IkOnKeyDown(n.Vk);
        }
    }
    break;
    case Notify::GlobalKeyUp:
    case Notify::GlobalMouseUp:
    {
        if (!App->GetOpt().bKeyStroke2)
            break;
        if (n.Vk < ARRAYSIZE(KeyName) &&
            !KeyName[n.Vk].empty())
        {
            ECK_DUILOCK;
            IkOnKeyUp(n.Vk);
        }
    }
    break;
    case Notify::GlobalMouseMove:
    {
        if (!App->GetOpt().bKeyStroke2)
            break;
        ECK_DUILOCK;
        auto pt{ n.pt };
        ScreenToClient(GetWnd()->HWnd, &pt);
        GetWnd()->Phy2Log(pt);
        ClientToElem(pt);
        IkOnMouseMove(pt);
    }
    break;
    case Notify::OptionsChanged:
    {
        ECK_DUILOCK;
        SetVisible(App->GetOpt().bKeyStroke2);
        if (!App->GetOpt().bKeyStroke2)
            m_vItem.clear();
        if (App->GetOpt().bRainbowColor)
            GetWnd()->WakeRenderThread();
    }
    break;
    }
}

std::vector<CVeKeyStroke2::ITEM>::iterator CVeKeyStroke2::IkpInsert(
    UINT Vk, _Out_ BOOL& bNewItem)
{
    ITEM e{ Vk };
    const auto it = std::lower_bound(m_vItem.begin(), m_vItem.end(), e);
    if (it == m_vItem.end())// 新元素是最大的
    {
        m_vItem.emplace_back(Vk, StayMillSecInit, ItemState::FadeIn);
        bNewItem = TRUE;
        return m_vItem.end() - 1;
    }
    else if (it->Vk != Vk)// 新元素插入到中间
    {
        const auto itNew = m_vItem.emplace(it, Vk, StayMillSecInit, ItemState::FadeIn);
        bNewItem = TRUE;
        return itNew;
    }
    else// 新元素和已有元素相同
    {
        it->msRemain += StayMillSecPreHit;
        bNewItem = FALSE;
        return it;
    }
}

void CVeKeyStroke2::IkOnKeyDown(UINT Vk)
{
    BOOL bNewItem;
    const auto it = IkpInsert(Vk, bNewItem);
    if (bNewItem)// 准备动画
    {
        it->uFlags |= KIF_KEYDOWN;
        CalcCenterBottomPos(it->xSrc, it->ySrc);
        it->x = it->xSrc;
        it->y = it->ySrc;
        IkpBeginRePos();
        GetWnd()->WakeRenderThread();
    }
    else
    {
        if (!(it->uFlags & KIF_KEYDOWN))
            it->uFlags |= KIF_KEYDOWN;
        IkpInvalidateRect(*it);
    }
}

void CVeKeyStroke2::IkOnKeyUp(UINT Vk)
{
    ITEM e{ Vk };
    const auto it = std::lower_bound(m_vItem.begin(), m_vItem.end(), e);
    if (it == m_vItem.end())
        return;
    if (it->uFlags & KIF_KEYDOWN)
        it->uFlags &= ~KIF_KEYDOWN;
    IkpInvalidateRect(*it);
}

void CVeKeyStroke2::IkpBeginRePos()
{
    for (auto& e : m_vItem)
    {
        if (e.eState == ItemState::None ||
            e.eState == ItemState::FadeIn ||
            e.eState == ItemState::RePos ||
            e.eState == ItemState::Jump ||
            e.eState == ItemState::Restore)
        {
            if (e.eState == ItemState::None)
                e.eState = ItemState::RePos;
            e.xSrc = e.x;
            e.ySrc = e.y;
            e.msTime = 0;
        }
    }
}

void CVeKeyStroke2::IkOnMouseMove(POINT pt_)
{
    D2D1_POINT_2F pt{ eck::MakeD2DPointF(pt_) };
    D2D1_RECT_F rc;
    BOOL bAn{};
    for (size_t i{}; auto& e : m_vItem)
    {
        if (e.eState == ItemState::None ||
            e.eState == ItemState::Restore)
        {
            rc.left = e.x;
            rc.top = e.y;
            rc.right = rc.left + m_cxyBlock;
            rc.bottom = rc.top + m_cxyBlock;
            if (eck::PtInRect(rc, pt))
            {
                bAn = TRUE;
                IkpBeginJump(e);
            }
        }
        else if (e.eState == ItemState::Jump ||
            e.eState == ItemState::Jumped)
        {
            CalcKeyItemNormalPos(i, rc.left, rc.top);
            rc.right = rc.left + m_cxyBlock;
            rc.bottom = rc.top + m_cxyBlock;
            if (!eck::PtInRect(rc, pt))
            {
                bAn = TRUE;
                IkpCancelJump(e);
            }
        }
        ++i;
    }
    if (bAn)
        GetWnd()->WakeRenderThread();
}

void CVeKeyStroke2::IkpBeginJump(ITEM& e)
{
    e.eState = ItemState::Jump;
    e.xSrc = e.x;
    e.ySrc = e.y;
    e.msTime = 0;
}

void CVeKeyStroke2::IkpCancelJump(ITEM& e)
{
    e.eState = ItemState::Restore;
    e.xSrc = e.x;
    e.ySrc = e.y;
    e.msTime = 0;
}

void CVeKeyStroke2::IkpInvalidateRect(ITEM& e)
{
    D2D1_RECT_F rcF;
    rcF.left = e.x;
    rcF.top = e.y;
    rcF.right = rcF.left + m_cxyBlock;
    rcF.bottom = rcF.top + m_cxyBlock;
    InvalidateRect(rcF);
}

LRESULT CVeKeyStroke2::OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        Dui::ELEMPAINTSTRU ps;
        BeginPaint(ps, wParam, lParam);

        D2D1_RECT_F rcBlock;
        for (auto& e : m_vItem)
        {
            rcBlock.left = e.x;
            rcBlock.top = e.y;
            rcBlock.right = rcBlock.left + m_cxyBlock;
            rcBlock.bottom = rcBlock.top + m_cxyBlock;
            PaintUnit(rcBlock, VeCxKeyStrokeBorder, e);
        }
        ECK_DUI_DBG_DRAW_FRAME;
        EndPaint(ps);
    }
    break;

    case WM_SIZE:
    {
        ECK_DUILOCK;
        m_cxyBlock = GetHeightF() - float(VeCxKeyStrokeBorder * 2);
        m_cxyBlock = 40;
        IkpBeginRePos();
        GetWnd()->WakeRenderThread();
    }
    break;

    case WM_CREATE:
    {
        m_pDC->CreateSolidColorBrush({}, &m_pBrush);
        m_pDC->CreateSolidColorBrush({}, &m_pBrushForegnd);
        m_hSlot = App->GetSignal().Connect(this, &CVeKeyStroke2::OnAppEvent);
        GetWnd()->RegisterTimeLine(this);
    }
    break;

    case WM_DESTROY:
    {
        App->GetSignal().Disconnect(m_hSlot);
        SafeRelease(m_pBrush);
        SafeRelease(m_pBrushForegnd);
        GetWnd()->UnregisterTimeLine(this);
    }
    break;
    }
    return __super::OnEvent(uMsg, wParam, lParam);
}

void CVeKeyStroke2::TlTick(int iMs)
{
    if (m_vItem.empty())
        return;
    float xFinal, yFinal;
    CalcKeyItemNormalPos(0, xFinal, yFinal);
    BOOL bActive{}, bDelete{};
    D2D1_RECT_F rcUpdate{};
    EckCounter(m_vItem.size(), i)
    {
        auto& e = m_vItem[i];
        if (e.eState == ItemState::FadeIn || e.eState == ItemState::RePos ||
            e.eState == ItemState::Restore)
        {
            e.msTime += iMs;
            const auto k = eck::Easing::OutCubic(e.msTime, 0.f, 1.f,
                (e.eState == ItemState::FadeIn) ? FadeInOutAnDuration :
                (e.eState == ItemState::RePos) ? RePosAnDuration : JumpAnDuration);
            if (e.eState == ItemState::FadeIn)
            {
                const auto k1 = std::clamp(k, 0.f, 1.f);
                if (k1 > e.fOpacity)
                    e.fOpacity = k1;
            }
            const auto xOld = e.x, yOld = e.y;
            if (k >= 1.f)
            {
                e.msTime = 0;
                e.eState = ItemState::None;
                e.x = xFinal;
                e.y = yFinal;
            }
            else
            {
                e.x = e.xSrc + (xFinal - e.xSrc) * k;
                e.y = e.ySrc + (yFinal - e.ySrc) * k;
                bActive = TRUE;
            }
            eck::UnionRect(rcUpdate, rcUpdate, {
                std::min(xOld, e.x), std::min(yOld, e.y),
                std::max(xOld, e.x) + m_cxyBlock,
                std::max(yOld, e.y) + m_cxyBlock });
        }
        else
        {
            if (!(e.uFlags & KIF_KEYDOWN) && (e.msRemain -= iMs) <= 0)
            {
                bDelete = TRUE;
                e.eState = ItemState::Deleted;
                eck::UnionRect(rcUpdate, rcUpdate, {
                    e.x, e.y,
                    e.x + m_cxyBlock, e.y + m_cxyBlock });
                goto Next;
            }
            if (e.eState == ItemState::Jump)
            {
                const float xDst = xFinal, yDst = 0.f;
                e.msTime += iMs;
                const auto k = eck::Easing::OutCubic(e.msTime, 0.f, 1.f, JumpAnDuration);
                const auto xOld = e.x, yOld = e.y;
                if (k >= 1.f)
                {
                    e.eState = ItemState::Jumped;
                    e.x = xDst;
                    e.y = yDst;
                }
                else
                {
                    e.x = e.xSrc + (xDst - e.xSrc) * k;
                    e.y = e.ySrc + (yDst - e.ySrc) * k;
                    bActive = TRUE;
                }
                eck::UnionRect(rcUpdate, rcUpdate, {
                    std::min(xOld, e.x), std::min(yOld, e.y),
                    std::max(xOld, e.x) + m_cxyBlock,
                    std::max(yOld, e.y) + m_cxyBlock });
            }
            else if (App->GetOpt().bRainbowColor)// 常规项目，更新彩虹色
            {
                eck::UnionRect(rcUpdate, rcUpdate, {
                    e.x, e.y,
                    e.x + m_cxyBlock, e.y + m_cxyBlock });
            }
        }
    Next:;
        xFinal += (m_cxyBlock + (float)VeCxyKeyStroke2Padding);
    }
    if (bDelete && !m_vItem.empty())
    {
        for (auto i = m_vItem.size(); i; --i)
        {
            auto& e = m_vItem[i - 1];
            if (e.eState == ItemState::Deleted)
                m_vItem.erase(m_vItem.begin() + i - 1);
        }
        if (!m_vItem.empty())
            IkpBeginRePos();
    }
    eck::InflateRect(rcUpdate, VeCxKeyStrokeBorder / 2.f, VeCxKeyStrokeBorder / 2.f);
    InvalidateRect(rcUpdate);
}

void CVeKeyStroke2::CalcKeyItemNormalPos(size_t idx, _Out_ float& x, _Out_ float& y)
{
    EckAssert(idx < m_vItem.size());
    x = (GetWidthF() - m_cxyBlock * m_vItem.size() -
        (float)VeCxyKeyStroke2Padding * (m_vItem.size() - 1)) / 2.f +
        (m_cxyBlock + (float)VeCxyKeyStroke2Padding) * idx;
    y = (GetHeightF() - m_cxyBlock) * 2.f / 5.f - m_cxyBlock / 2.f;
}

void CVeKeyStroke2::PaintUnit(const D2D1_RECT_F& rc, float cxLine, ITEM& e)
{
    D2D1_COLOR_F crBkg, crForegnd;
    if (e.uFlags & KIF_KEYDOWN)
        crBkg = App->GetColor(CApp::CrKeyStrokeBkgPressed);
    else
        crBkg = App->GetColor(CApp::CrKeyStrokeBkg);
    if (App->GetOpt().bRainbowColor)
        crForegnd = CalcRainbowColorWithStep(
            NtGetTickCount64(), int(&e - &m_vItem[0]) * 8);
    else
        crForegnd = App->GetColor(CApp::CrKeyStroke);
    if (e.eState == ItemState::FadeIn)
    {
        crBkg.a *= e.fOpacity;
        crForegnd.a *= e.fOpacity;
    }
    m_pBrushForegnd->SetColor(crForegnd);
    m_pBrush->SetColor(crBkg);
    m_pDC->FillRectangle(rc, m_pBrush);

    if (!e.pTextLayout)
    {
        eck::g_pDwFactory->CreateTextLayout(KeyName[e.Vk].data(),
            (UINT32)KeyName[e.Vk].size(), GetTextFormat(),
            m_cxyBlock, m_cxyBlock, &e.pTextLayout);
        if (e.pTextLayout)
        {
            e.pTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            e.pTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        }
    }
    if (e.pTextLayout)
    {
        D2D1::Matrix3x2F MatOld;
        if (KeyName[e.Vk].size() == 1)
        {
            m_pDC->GetTransform(&MatOld);
            constexpr float fScale = 1.8f;
            const D2D1_POINT_2F pt
            {
                (rc.left + rc.right) / 2.f,
                (rc.top + rc.bottom) / 2.f
            };
            m_pDC->SetTransform(D2D1::Matrix3x2F::Scale(
                fScale, fScale, pt) * MatOld);
        }
        m_pDC->DrawTextLayout({ rc.left,rc.top }, e.pTextLayout,
            m_pBrushForegnd, Dui::DrawTextLayoutFlags);
        if (KeyName[e.Vk].size() == 1)
            m_pDC->SetTransform(MatOld);
    }
    m_pDC->DrawRectangle(rc, m_pBrushForegnd, cxLine);
}

void CVeKeyStroke2::CalcCenterBottomPos(_Out_ float& x, _Out_ float& y)
{
    x = (GetWidthF() - m_cxyBlock) / 2.f;
    y = GetHeightF() - m_cxyBlock - (float)VeCxKeyStrokeBorder;
}