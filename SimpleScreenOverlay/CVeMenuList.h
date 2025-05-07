#pragma once
struct ML_DISPINFO : Dui::DUINMHDR
{
	int idx;
	int cchText;
	PCWSTR pszText;
};

class CVeMenuList final : public Dui::CListTemplate
{
	void LVPaintItem(int idx, int idxGroup, const D2D1_RECT_F& rcPaint) override;
public:
	CVeMenuList()
	{
		SetToggleSel(TRUE);
		SetDeSelInSpace(FALSE);
		SetAllowDragSel(FALSE);
		SetItemNotify(TRUE);
	}
};