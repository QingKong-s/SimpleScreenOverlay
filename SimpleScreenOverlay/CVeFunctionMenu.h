#pragma once
#include "CApp.h"
#include "VeDef.h"
#include "CVeMenuList.h"

class CVeFunctionMenu final : public Dui::CElem
{
private:
	CVeMenuList m_LV{};

	D2D1_COLOR_F m_crTitleBkg{ CApp::ColorLight[CApp::CrDefFuncMenuTitle] };
	D2D1_COLOR_F m_crBkg{ CApp::ColorLight[CApp::CrDefFuncMenuBkg] };
	ID2D1SolidColorBrush* m_pBrush{};
	IDWriteTextLayout* m_pTextLayout{};

	BOOLEAN m_bLBtnDown{};
	POINT m_ptDragStart{};
	POINT m_ptDragOffset{};
public:
	LRESULT OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	EckInlineNdCe auto& GetMenuList() { return m_LV; }

	void ReCalcIdealSize();
};