#pragma once
#include "CApp.h"

class CVeKeyStroke2 final : public Dui::CElem, public eck::CFixedTimeLine
{
private:
    constexpr static int StayMillSecPreHit = 1000;// 每次击键后，指定键显示的毫秒数
    constexpr static int StayMillSecInit = 1600;// 第一次显示的毫秒数
    enum class ItemState : BYTE
    {
        None,	// 正常
        FadeIn,	// 新加入列表，正在播放进入动画
        RePos,	// 位置需要重新计算，因为有键进入或退出
        Jump,	// 避让光标，正在播放向上移动动画
        Jumped,	// 已避让光标
        Restore,// 取消避让光标，正在播放恢复动画
        Deleted,
    };
    enum : BYTE
    {
        KIF_NONE = 0,
        KIF_KEYDOWN = 1,
    };

    struct ITEM
    {
        UINT Vk;
        int msRemain;		// 剩余显示时间
        ItemState eState;
        BYTE uFlags;		// KIF_*
        float fOpacity;		// 透明度
        float msTime;		// 动画当前时间
        float x, y;			// 当前位置
        float xSrc, ySrc;	// 动画开始位置
        IDWriteTextLayout* pTextLayout;

        EckInlineNdCe std::strong_ordering operator<=>(const ITEM& x) const
        {
            return Vk <=> x.Vk;
        }
    };

    Dui::CSizeBox m_SizeBox{};
    CApp::HSlot m_hSlot{};
    ID2D1SolidColorBrush* m_pBrush{};
    ID2D1SolidColorBrush* m_pBrushForegnd{};
    std::vector<ITEM> m_vItem{};// 虚拟键代码从小到大排序
    float m_cxyBlock{};

    BOOLEAN m_bShowBorder{};

    void OnAppEvent(Notify eNotify, const SSONOTIFY& n);

    std::vector<ITEM>::iterator IkpInsert(UINT Vk, _Out_ BOOL& bNewItem);

    void IkOnKeyDown(UINT Vk);

    void IkOnKeyUp(UINT Vk);

    void IkpBeginRePos();

    void IkOnMouseMove(POINT pt_);

    void IkpBeginJump(ITEM& e);

    void IkpCancelJump(ITEM& e);

    void IkpInvalidateRect(ITEM& e);

    void CalcKeyItemNormalPos(size_t idx, _Out_ float& x, _Out_ float& y);

    void PaintUnit(const D2D1_RECT_F& rc, float cxLine, ITEM& e);

    void CalcCenterBottomPos(_Out_ float& x, _Out_ float& y);
public:
    LRESULT OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    void TlTick(int iMs) override;
    BOOL TlIsValid() override { return !m_vItem.empty(); }
};