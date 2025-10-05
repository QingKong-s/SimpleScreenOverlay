#include "pch.h"
#include "CVeMenuContainer.h"
#include "CWndMain.h"

constexpr static PCWSTR MenuTitle[]
{
    L"程序",
    L"渲染",
    L"系统工具"
};

enum class MProgram
{
    Exit,
    SetAutoRun,
};
enum class MRender
{
    BlurBkg,
    RainbowColor,
    Crosshair,
    KeyStroke,
    KeyStroke2,
    Time,
    SpotLight,
    WindowHiLight,
    WindowTip,
    Ruler,
    Watermark,
    Click,
    LocateCursor,
    CursorPos,
    WheelIndicator,
};
enum class MTools
{
    RestartExplorer,
    Calculator,
    CmdPrompt,
    RegEdit,
    GPEdit,
    LocalUser,
    Desktop,
};

struct ITEM_DESC
{
    std::wstring_view svText;
    std::wstring_view svTip;
};

constexpr static ITEM_DESC ItemProgram[]
{
    { L"退出程序"sv,L"关闭程序并保存设置"sv },
    { L"开机自启动"sv,L"设置一个计划任务，使程序开机时自动启动"sv },
};

constexpr static ITEM_DESC ItemRender[]
{
    { L"模糊背景"sv, L"是否在显示菜单时模糊背景"sv },
    { L"彩虹色"sv, L"某些前景色显示为彩虹色"sv },
    { L"准星线"sv, L"在屏幕上显示准星线"sv },
    { L"按键显示"sv, L"显示游戏按键输入"sv },
    { L"按键显示2"sv, L"显示所有按键输入"sv },
    { L"时间"sv, L"TODO"sv },
    { L"聚光灯"sv, L"双击Ctrl在光标位置显示一个光斑"sv },
    { L"窗口高亮"sv, L"在光标所在窗口的周围显示一个方框"sv },
    { L"窗口提示"sv, L"显示在光标所在窗口的详细信息"sv },
    { L"标尺"sv, L"显示光标所在窗口坐标内的标尺"sv },
    { L"水印"sv, L"在屏幕中央显示水印"sv },
    { L"点击提示"sv, L"显示光标点击状态"sv },
    { L"定位光标"sv, L"按下Ctrl时显示光标位置"sv },
    { L"光标位置"sv, L"在光标处绘制一个圆"sv },
    { L"滚轮指示器"sv, L"在光标旁边绘制当前滚轮的状态"sv },
};
constexpr static ITEM_DESC ItemTools[]
{
    { L"重启资源管理器"sv,L"NtTerminateProcess(hExplorer, 2)" },
    { L"计算器"sv },
    { L"命令提示符"sv },
    { L"注册表编辑器"sv },
    { L"组策略编辑器"sv },
    { L"本地用户和组"sv },
    { L"桌面"sv,L"打开桌面文件夹"sv },
};

constexpr const ITEM_DESC& GetItemDesc(int idxMenu, int idx)
{
    EckAssert(idxMenu >= 0 && idxMenu < ARRAYSIZE(MenuTitle));
    switch (idxMenu)
    {
    case MenuIdxProgram: return ItemProgram[idx];
    case MenuIdxRender: return ItemRender[idx];
    case MenuIdxTools: return ItemTools[idx];
    default: ECK_UNREACHABLE;
    }
}

LRESULT CVeMenuContainer::OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        Dui::ELEMPAINTSTRU ps;
        BeginPaint(ps, wParam, lParam);

        if (m_pTlTip.Get())
        {
            auto rcF{ m_rcTip };
            m_pBrushTip->SetColor(App->GetColor(CApp::CrMenuTipBkg));
            m_pDC->FillRectangle(rcF, m_pBrushTip);

            if (App->GetOpt().bRainbowColor)
                m_pBrushTip->SetColor(CalcRainbowColor(NtGetTickCount64()));
            else
                m_pBrushTip->SetColor(App->GetColor(CApp::CrMenuTip));
            m_pDC->DrawTextLayout({ 0.f,rcF.top + (float)VeCxyMenuTipMargin },
                m_pTlTip.Get(), m_pBrushTip, Dui::DrawTextLayoutFlags);

            constexpr float LineWidth = 1.f;
            eck::InflateRect(rcF, -LineWidth / 2.f, -LineWidth / 2.f);
            m_pDC->DrawRectangle(rcF, m_pBrushTip, LineWidth);
        }

        ECK_DUI_DBG_DRAW_FRAME;
        EndPaint(ps);
    }
    return 0;

    case WM_NOTIFY:
    {
        switch (((Dui::DUINMHDR*)lParam)->uCode)
        {
        case UIE_MENU_GETDISPINFO:
        {
            const auto p = (ML_DISPINFO*)lParam;
            const auto idxMenu = int((CVeFunctionMenu*)wParam - m_MenuBox);
            const auto& e = GetItemDesc(idxMenu, p->idx);
            p->pszText = e.svText.data();
            p->cchText = (int)e.svText.size();
        }
        return 0;

        case Dui::LTE_HOTITEMCHANED:
        {
            const auto p = (Dui::NMLTHOTITEMCHANGE*)lParam;
            if (p->idx < 0)
            {
                m_pTlTip.Clear();
            Exit:;
                InvalidateRect(m_rcTip);
                return 0;
            }
            const auto idxMenu = int((CVeFunctionMenu*)wParam - m_MenuBox);
            const auto& e = GetItemDesc(idxMenu, p->idx);
            eck::g_pDwFactory->CreateTextLayout(
                e.svTip.data(), (UINT32)e.svTip.size(), m_pTfTip.Get(),
                GetWidthF(), GetHeightF(), m_pTlTip.AddrOfClear());
            if (!m_pTlTip.Get())
                goto Exit;
            const auto rcOld{ m_rcTip };
            DWRITE_TEXT_METRICS tm;
            m_pTlTip->GetMetrics(&tm);
            const auto cyBottom = GetHeightF() *
                (float)VeCyMenuTipBottomMarginPct / 100.f;
            m_rcTip.left = (GetWidthF() - tm.width) / 2.f;
            m_rcTip.top = GetHeightF() - tm.height - cyBottom;
            m_rcTip.right = m_rcTip.left + tm.width;
            m_rcTip.bottom = m_rcTip.top + tm.height;
            eck::InflateRect(m_rcTip, VeCxyMenuTipMargin, VeCxyMenuTipMargin);
            auto rcNew{ m_rcTip };
            eck::UnionRect(rcNew, rcNew, rcOld);
            InvalidateRect(rcNew);
        }
        return 0;

        case Dui::LTE_ITEMCHANED:
        {
            const auto p = (Dui::NMLTITEMCHANGE*)lParam;
            auto& Opt = App->GetOpt();
            const auto bSel = !!(p->uFlagsNew & Dui::LEIF_SELECTED);
            if ((WPARAM)&m_MenuBox[MenuIdxProgram] == wParam)
            {
                switch ((MProgram)p->idx)
                {
                case MProgram::SetAutoRun:
                {
                    Opt.bAutoRun = bSel;
                    auto uFlags = eck::AutoRunType::TaskScheduler;
                    if (App->IsAdmin())
                        uFlags |= eck::AutoRunType::RunAdmin;
                    eck::SetAutoRun(L"qk_s_simplescreenoverlay",
                        bSel, uFlags);
                }
                return FALSE;
                case MProgram::Exit:// 此时在临界区中，另起一下操作防止死锁
                    eck::GetThreadCtx()->Callback.EnQueueCallback([this]
                        {
                            GetWnd()->Destroy();
                        });
                    break;
                }
                return TRUE;
            }
            else if ((WPARAM)&m_MenuBox[MenuIdxRender] == wParam)
            {
                switch ((MRender)p->idx)
                {
                case MRender::BlurBkg:		Opt.bBlurBkg = bSel;		break;
                case MRender::RainbowColor:	Opt.bRainbowColor = bSel;	break;
                case MRender::Crosshair:	Opt.bCrosshair = bSel;		break;
                case MRender::KeyStroke:	Opt.bKeyStroke = bSel;		break;
                case MRender::KeyStroke2:	Opt.bKeyStroke2 = bSel;		break;
                case MRender::Time:			Opt.bTime = bSel;			break;
                case MRender::SpotLight:	Opt.bSpotLight = bSel;		break;
                case MRender::WindowHiLight:Opt.bWndHilight = bSel;		break;
                case MRender::WindowTip:	Opt.bWndTip = bSel;			break;
                case MRender::Ruler:		Opt.bRuler = bSel;			break;
                case MRender::Watermark:	Opt.bWatermark = bSel;		break;
                case MRender::Click:		Opt.bShowClick = bSel;		break;
                case MRender::LocateCursor:	Opt.bLocateCursorWithCtrl = bSel;	break;
                case MRender::CursorPos:	Opt.bShowCursorPos = bSel;	break;
                case MRender::WheelIndicator: Opt.bShowWheel = bSel;	break;
                }
                SSONOTIFY n{};
                App->GetSignal().Emit(Notify::OptionsChanged, n);
            }
            else if ((WPARAM)&m_MenuBox[MenuIdxTools] == wParam &&
                ((p->uFlagsNew ^ p->uFlagsOld) & Dui::LEIF_SELECTED) &&
                bSel)
            {
                PCWSTR pszCmd = nullptr;
                switch ((MTools)p->idx)
                {
                case MTools::RestartExplorer: eck::RestartExplorer(); break;
                case MTools::Calculator:pszCmd = L"calc.exe";		break;
                case MTools::CmdPrompt:	pszCmd = L"cmd.exe";		break;
                case MTools::RegEdit:	pszCmd = L"regedit.exe";	break;
                case MTools::GPEdit:	pszCmd = L"gpedit.msc";		break;
                case MTools::LocalUser:	pszCmd = L"compmgmt.msc";	break;
                case MTools::Desktop:
                {
                    PWSTR pszDesktop = nullptr;
                    SHGetKnownFolderPath(FOLDERID_Desktop, 0, nullptr,
                        &pszDesktop);
                    eck::OpenInExplorer(pszDesktop, TRUE);
                    CoTaskMemFree(pszDesktop);
                }
                break;
                }
                if (pszCmd)
                    ShellExecuteW(nullptr, L"open", pszCmd,
                        nullptr, nullptr, SW_SHOWNORMAL);
                ((CWndMain*)GetWnd())->SwitchMenuShowing(FALSE);
                return TRUE;
            }
        }
        return 0;
        }
    }
    return 0;

    case WM_STYLECHANGED:
    {
        if ((wParam ^ lParam) & Dui::DES_VISIBLE)
        {
            m_pTlTip.Clear();
            if (lParam & Dui::DES_VISIBLE)
            {
                m_bTimeLineActive = TRUE;
                GetWnd()->WakeRenderThread();
            }
            else
                m_bTimeLineActive = FALSE;
        }
    }
    break;

    case WM_CREATE:
    {
        m_pTfTip = App->CreateTextFormat(20);
        m_pTfTip->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        m_pTfTip->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        m_pTfTip->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
        m_pTfTip->Release();

        m_pDC->CreateSolidColorBrush({}, &m_pBrushTip);

        float x = (GetWidthF() -
            ((VeCxFuncMenu + 20) * ARRAYSIZE(m_MenuBox))) / 2;
        constexpr auto y = 400.f;
        for (size_t i{}; auto& e : m_MenuBox)
        {
            e.Create(MenuTitle[i], Dui::DES_VISIBLE, 0,
                x, y, VeCxFuncMenu, 400, this);
            x += (VeCxFuncMenu + 10);
            ++i;
        }
        auto& MenuProgram = m_MenuBox[MenuIdxProgram].GetMenuList();
        auto& MenuRender = m_MenuBox[MenuIdxRender].GetMenuList();
        auto& MenuTools = m_MenuBox[MenuIdxTools].GetMenuList();
        MenuProgram.SetItemCount(ARRAYSIZE(ItemProgram));
        m_MenuBox[MenuIdxProgram].ReCalcIdealSize();
        MenuRender.SetItemCount(ARRAYSIZE(ItemRender));
        m_MenuBox[MenuIdxRender].ReCalcIdealSize();
        MenuTools.SetItemCount(ARRAYSIZE(ItemTools));
        m_MenuBox[MenuIdxTools].ReCalcIdealSize();
        const auto& Opt = App->GetOpt();
        if (Opt.bBlurBkg)
            MenuRender.SetItemState((int)MRender::BlurBkg, Dui::LEIF_SELECTED);
        if (Opt.bRainbowColor)
            MenuRender.SetItemState((int)MRender::RainbowColor, Dui::LEIF_SELECTED);
        if (Opt.bCrosshair)
            MenuRender.SetItemState((int)MRender::Crosshair, Dui::LEIF_SELECTED);
        if (Opt.bKeyStroke)
            MenuRender.SetItemState((int)MRender::KeyStroke, Dui::LEIF_SELECTED);
        if (Opt.bKeyStroke2)
            MenuRender.SetItemState((int)MRender::KeyStroke2, Dui::LEIF_SELECTED);
        if (Opt.bTime)
            MenuRender.SetItemState((int)MRender::Time, Dui::LEIF_SELECTED);
        if (Opt.bSpotLight)
            MenuRender.SetItemState((int)MRender::SpotLight, Dui::LEIF_SELECTED);
        if (Opt.bWndHilight)
            MenuRender.SetItemState((int)MRender::WindowHiLight, Dui::LEIF_SELECTED);
        if (Opt.bWndTip)
            MenuRender.SetItemState((int)MRender::WindowTip, Dui::LEIF_SELECTED);
        if (Opt.bRuler)
            MenuRender.SetItemState((int)MRender::Ruler, Dui::LEIF_SELECTED);
        if (Opt.bWatermark)
            MenuRender.SetItemState((int)MRender::Watermark, Dui::LEIF_SELECTED);
        if (Opt.bAutoRun)
            MenuProgram.SetItemState((int)MProgram::SetAutoRun, Dui::LEIF_SELECTED);
        if (Opt.bShowClick)
            MenuRender.SetItemState((int)MRender::Click, Dui::LEIF_SELECTED);
        if (Opt.bLocateCursorWithCtrl)
            MenuRender.SetItemState((int)MRender::LocateCursor, Dui::LEIF_SELECTED);
        if (Opt.bShowCursorPos)
            MenuRender.SetItemState((int)MRender::CursorPos, Dui::LEIF_SELECTED);
        if (Opt.bShowWheel)
            MenuRender.SetItemState((int)MRender::WheelIndicator, Dui::LEIF_SELECTED);
        GetWnd()->RegisterTimeLine(this);
    }
    break;

    case WM_DESTROY:
        GetWnd()->UnregisterTimeLine(this);
        SafeRelease(m_pBrushTip);
        break;
    }
    return __super::OnEvent(uMsg, wParam, lParam);
}

void CVeMenuContainer::TlTick(int iMs)
{
    for (auto& e : m_MenuBox)
        e.InvalidateRect();
    if (m_pTlTip.Get())
        InvalidateRect(m_rcTip);
}