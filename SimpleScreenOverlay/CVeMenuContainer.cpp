#include "pch.h"
#include "CVeMenuContainer.h"

constexpr static PCWSTR MenuTitle[]
{
	L"渲染"
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

using namespace std::literals;

struct ITEM_DESC
{
	std::wstring_view svText;
	std::wstring_view svTip;
};

ITEM_DESC ItemRender[]
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

void CVeMenuContainer::InitMenu()
{
	auto& Render = m_MenuBox[0];
	auto& vItemRender = Render.GetMenuList().GetItems();

	for (const auto& e : ItemRender)
		vItemRender.emplace_back(e.svText);
	Render.CommitItemChange();

}

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

	case WM_CREATE:
	{
		for (size_t i{}; auto & e : m_MenuBox)
		{
			e.Create(MenuTitle[i], Dui::DES_VISIBLE, 0,
				100, 100, VeCxFuncMenu, 400, this);
			++i;
		}
		InitMenu();
	}
	break;
	}
	return __super::OnEvent(uMsg, wParam, lParam);
}