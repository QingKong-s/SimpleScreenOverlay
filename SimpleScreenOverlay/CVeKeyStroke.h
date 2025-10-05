#pragma once
#include "CApp.h"

class CVeKeyStroke final : public Dui::CElem, public eck::CFixedTimeLine
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
    CApp::HSlot m_hSlot{};
    ID2D1SolidColorBrush* m_pBrush{};
    ID2D1SolidColorBrush* m_pBrushForegnd{};
    float m_cxyBlock{};
    float m_cySpaceBtn{};
    float m_cyMouseBtn{};

    BOOLEAN m_bKeyDown[(size_t)Key::Max]{};
    IDWriteTextLayout* m_pTl[(size_t)Key::Max]{};

    BOOLEAN m_bLeft{ TRUE };

    Key VkToKey(UINT vk);
    UINT KeyToVk(Key eKey);

    void OnAppEvent(Notify eNotify, const SSONOTIFY& n);

    void PaintUnit(const D2D1_RECT_F& rc, float cxLine, Key eKey);
public:
    LRESULT OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    void TlTick(int iMs) override { InvalidateRect(); }

    BOOL TlIsValid() override
    {
        return App->GetOpt().bRainbowColor && App->GetOpt().bKeyStroke;
    }
};