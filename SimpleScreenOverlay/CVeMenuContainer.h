#pragma once
#include "CVeFunctionMenu.h"

class CVeMenuContainer : public Dui::CElem
{
private:
	CVeFunctionMenu m_MenuBox[MenuIdxMax]{};

	void InitMenu();
public:
	LRESULT OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

};