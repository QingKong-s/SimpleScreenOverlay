#pragma once

class CVeMenuList final : public Dui::CListTemplate
{
public:
	struct ITEM
	{
		eck::CRefStrW rsText{};
		IDWriteTextLayout* pTextLayout{};
		BOOLEAN bSelected{};
	};

	std::vector<ITEM> m_vItem{};
	ID2D1SolidColorBrush* m_pBrush{};
private:
	void PaintItem(int idx, const D2D1_RECT_F& rcItem, const D2D1_RECT_F& rcPaint) override;
public:
	LRESULT OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	LRESULT OnNotify(Dui::DUINMHDR* pnm, BOOL& bProcessed) override;

	// 【！！注意！！】修改数组必须进入临界区
	EckInlineNdCe auto& GetItems() { return m_vItem; }
};