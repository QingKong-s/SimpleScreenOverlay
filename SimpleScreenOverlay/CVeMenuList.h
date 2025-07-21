#pragma once
struct ML_DISPINFO : Dui::DUINMHDR
{
	int idx;
	int cchText;
	PCWSTR pszText;
};

class CVeMenuList final : public Dui::CListTemplate
{
	void LVPaintItem(const D2D1_RECT_F& rcPaint, Dui::NMLTCUSTOMDRAW& nm, LRESULT r) override;
public:
	LRESULT OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	
	CVeMenuList()
	{
		SetToggleSel(TRUE);
		SetDeSelInSpace(FALSE);
		SetAllowDragSel(FALSE);
		SetItemNotify(TRUE);
	}
};