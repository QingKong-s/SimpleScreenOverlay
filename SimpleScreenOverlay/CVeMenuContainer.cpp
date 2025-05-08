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
};
enum class MRender
{
	RainbowColor,
	Crosshair,
	KeyStroke,
	KeyStroke2,
	Time,
	SpotLight,
	WindowHighlight,
	WindowTip,
	Ruler,
	Watermark,
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
	{ L"退出程序"sv,{} },
};

constexpr static ITEM_DESC ItemRender[]
{
	{ L"彩虹色"sv, {} },
	{ L"准星线"sv, L"在屏幕上显示准星线"sv },
	{ L"按键显示"sv, L"显示游戏按键输入"sv },
	{ L"按键显示2"sv, L"显示所有按键输入"sv },
	{ L"时间"sv, L"显示时间"sv },
	{ L"聚光灯"sv, L"双击Ctrl在光标位置显示一个光斑"sv },
	{ L"窗口高亮"sv, L"在光标所在窗口的周围显示一个方框"sv },
	{ L"窗口提示"sv, L"显示在光标所在窗口的详细信息"sv },
	{ L"标尺"sv, L"显示标尺"sv },
	{ L"水印"sv, L"在屏幕中央显示一个水印"sv }
};
constexpr static ITEM_DESC ItemTools[]
{
	{ L"重启资源管理器"sv },
	{ L"计算器"sv },
	{ L"命令提示符"sv },
	{ L"注册表编辑器"sv },
	{ L"组策略编辑器"sv },
	{ L"本地用户和组"sv },
	{ L"桌面"sv },
};

LRESULT CVeMenuContainer::OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		Dui::ELEMPAINTSTRU ps;
		BeginPaint(ps, wParam, lParam);

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
			if ((WPARAM)&m_MenuBox[MenuIdxProgram] == wParam)
			{
				p->pszText = ItemProgram[p->idx].svText.data();
				p->cchText = (int)ItemProgram[p->idx].svText.size();
			}
			else if ((WPARAM)&m_MenuBox[MenuIdxRender] == wParam)
			{
				p->pszText = ItemRender[p->idx].svText.data();
				p->cchText = (int)ItemRender[p->idx].svText.size();
			}
			else if ((WPARAM)&m_MenuBox[MenuIdxTools] == wParam)
			{
				p->pszText = ItemTools[p->idx].svText.data();
				p->cchText = (int)ItemTools[p->idx].svText.size();
			}
		}
		return 0;
		case Dui::LTE_ITEMCHANED:
		{
			const auto p = (Dui::LTN_ITEMCHANGE*)lParam;
			auto& Opt = App->GetOpt();
			const auto bSel = !!(p->uFlagsNew & Dui::LEIF_SELECTED);
			if ((WPARAM)&m_MenuBox[MenuIdxProgram] == wParam)
			{
				switch ((MProgram)p->idx)
				{
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
				case MRender::RainbowColor:	Opt.bRainbowColor = bSel;	break;
				case MRender::Crosshair:	Opt.bCrosshair = bSel;		break;
				case MRender::KeyStroke:	Opt.bKeyStroke = bSel;		break;
				case MRender::KeyStroke2:	Opt.bKeyStroke2 = bSel;		break;
				case MRender::Time:			Opt.bTime = bSel;			break;
				case MRender::SpotLight:	Opt.bSpotLight = bSel;		break;
				case MRender::WindowHighlight:	Opt.bWndHilight = bSel;	break;
				case MRender::WindowTip:	Opt.bWndTip = bSel;			break;
				case MRender::Ruler:		Opt.bRuler = bSel;			break;
				case MRender::Watermark:	Opt.bWatermark = bSel;		break;
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
		int x = (GetWidth() -
			((VeCxFuncMenu + 20) * ARRAYSIZE(m_MenuBox))) / 2;
		constexpr auto y = 400;
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
			MenuRender.SetItemState((int)MRender::WindowHighlight, Dui::LEIF_SELECTED);
		if (Opt.bWndTip)
			MenuRender.SetItemState((int)MRender::WindowTip, Dui::LEIF_SELECTED);
		if (Opt.bRuler)
			MenuRender.SetItemState((int)MRender::Ruler, Dui::LEIF_SELECTED);
		if (Opt.bWatermark)
			MenuRender.SetItemState((int)MRender::Watermark, Dui::LEIF_SELECTED);
		GetWnd()->RegisterTimeLine(this);
	}
	break;

	case WM_DESTROY:
		GetWnd()->UnregisterTimeLine(this);
		break;
	}
	return __super::OnEvent(uMsg, wParam, lParam);
}

void STDMETHODCALLTYPE CVeMenuContainer::Tick(int iMs)
{
	for (auto& e : m_MenuBox)
		e.InvalidateRect();
}