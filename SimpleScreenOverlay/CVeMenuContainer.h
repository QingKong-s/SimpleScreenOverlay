#pragma once
#include "CVeFunctionMenu.h"

class CVeMenuContainer final : public Dui::CElem, public eck::CFixedTimeLine
{
private:
	CVeFunctionMenu m_MenuBox[MenuIdxMax]{};
	BOOL m_bTimeLineActive{ FALSE };
public:
	LRESULT OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	void STDMETHODCALLTYPE Tick(int iMs);

	BOOL STDMETHODCALLTYPE IsValid()
	{
		return App->GetOpt().bRainbowColor && m_bTimeLineActive;
	}
};