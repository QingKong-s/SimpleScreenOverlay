#pragma once
class CVeKeyStroke2 final : public Dui::CElem, public CFixTimeLine
{
private:
	constexpr static int StayMillSecPreHit = 8000;// 每次击键后，指定键显示的毫秒数
	enum class ItemState : BYTE
	{
		None,
		FadeIn,	// 该键新加入列表，正在播放进入动画
		FadeOut,// 该键显示超时，正在播放退出动画
		RePos,	// 有键进入或退出，其他键位置需要重新计算
		Deleted,// 应删除
	};
	enum : BYTE
	{
		KIF_NONE = 0,
		KIF_KEYDOWN = 1,
	};

	struct ITEM
	{
		UINT Vk;
		int msRemain;
		ItemState eState;
		BYTE uFlags;
		float fOpacity;		// 透明度
		float msTime;		// 动画当前时间
		float x, y;			// 当前位置
		float xSrc, ySrc;	// 动画开始位置
		IDWriteTextLayout* pTextLayout;

		std::strong_ordering operator<=>(const ITEM& x) const
		{
			return Vk <=> x.Vk;
		}
	};

	ID2D1SolidColorBrush* m_pBrush{};
	ID2D1SolidColorBrush* m_pBrushForegnd{};
	std::vector<ITEM> m_vItem{};// 虚拟键代码从小到大排序
	float m_cxyBlock{};
	BOOL m_bIdle{ 0 };

	void OnAppEvent(Notify eNotify, SSONOTIFY& n);

	std::vector<ITEM>::iterator IkpInsert(UINT Vk, _Out_ BOOL& bNewItem);

	void IkOnKeyDown(UINT Vk);

	void IkOnKeyUp(UINT Vk);

	void IkBeginRePos();

	void ChangeIdleState(BOOL bIdle, BOOL bWakeRenderThread = TRUE)
	{
		m_bIdle = bIdle;
		if (m_bIdle && bWakeRenderThread)
			GetWnd()->WakeRenderThread();
	}

	void CalcKeyItemNormalPos(size_t idx, _Out_ float& x, _Out_ float& y);

	void PaintUnit(const D2D1_RECT_F& rc, float cxLine, ITEM& e);

	void CalcCenterBottomPos(_Out_ float& x, _Out_ float& y);
public:
	LRESULT OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	void STDMETHODCALLTYPE Tick(int iMs);
	BOOL STDMETHODCALLTYPE IsValid() { return !m_bIdle; }
};