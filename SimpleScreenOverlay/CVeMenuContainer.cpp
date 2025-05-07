#include "pch.h"
#include "CVeMenuContainer.h"

constexpr static PCWSTR MenuTitle[]
{
	L"渲染",
	L"系统工具"
};

enum class MRender
{
	Crosshair,
	KeyStrokes,
	KeyStrokes2,
	Time,
	Spotlight,
	WindowHighlight,
	WindowTip,
	Ruler,
};

struct ITEM_DESC
{
	std::wstring_view svText;
	std::wstring_view svTip;
};

constexpr static ITEM_DESC ItemRender[]
{
	{ L"准星线"sv, L"在屏幕上显示准星线"sv },
	{ L"按键显示"sv, L"显示按键输入"sv },
	{ L"按键显示2"sv, L"显示按键输入2"sv },
	{ L"时间"sv, L"显示时间"sv },
	{ L"聚光灯"sv, L"双击Ctrl在光标位置显示一个光斑"sv },
	{ L"窗口高亮"sv, L"在光标所在窗口的周围显示一个方块"sv },
	{ L"窗口提示"sv, L"显示在光标所在窗口的详细信息"sv },
	{ L"标尺"sv, L"显示标尺"sv },
};
constexpr static ITEM_DESC ItemTools[]
{
	{ L"重启资源管理器"sv },
	{ L"计算器"sv },
	{ L"设置"sv },
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
		if (((Dui::DUINMHDR*)lParam)->uCode != UIE_MENU_GETDISPINFO)
			break;
		const auto p = (ML_DISPINFO*)lParam;
		if ((WPARAM)&m_MenuBox[MenuIdxRender] == wParam)
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

	case WM_CREATE:
	{
		int x = 120;
		for (size_t i{}; auto & e : m_MenuBox)
		{
			e.Create(MenuTitle[i], Dui::DES_VISIBLE, 0,
				x, 140, VeCxFuncMenu, 400, this);
			x += (VeCxFuncMenu + 10);
			++i;
		}
		m_MenuBox[MenuIdxRender].GetMenuList().SetItemCount(ARRAYSIZE(ItemRender));
		m_MenuBox[MenuIdxRender].ReCalcIdealSize();
		m_MenuBox[MenuIdxTools].GetMenuList().SetItemCount(ARRAYSIZE(ItemTools));
		m_MenuBox[MenuIdxTools].ReCalcIdealSize();
	}
	break;
	}
	return __super::OnEvent(uMsg, wParam, lParam);
}