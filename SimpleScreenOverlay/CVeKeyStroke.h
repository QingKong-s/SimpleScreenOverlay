#pragma once
class CVeKeyStroke : public Dui::CElem
{
private:
	enum class Key
	{
		W,
		A,
		S,
		D,

		SingleCharEnd,

		Space = SingleCharEnd,
		Shift,
		MLeft,
		MRight,

		Max,
	};

	ID2D1SolidColorBrush* m_pBrush{};
	ID2D1SolidColorBrush* m_pBrushForegnd{};
	float m_cxyBlock{};
	float m_cySpaceBtn{};
	float m_cyMouseBtn{};

	BOOLEAN m_bKeyDown[(size_t)Key::Max]{};
	IDWriteTextLayout* m_pTl[(size_t)Key::Max]{};

	Key VkToKey(UINT vk);
	UINT KeyToVk(Key eKey);

	void OnAppEvent(Notify eNotify, SSONOTIFY& n);

	void PaintUnit(const D2D1_RECT_F& rc, float cxLine, Key eKey);
public:
	LRESULT OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};