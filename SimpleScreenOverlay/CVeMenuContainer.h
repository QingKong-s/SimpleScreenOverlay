#pragma once
#include "CVeFunctionMenu.h"

class CVeMenuContainer final : public Dui::CElem, public eck::CFixedTimeLine
{
private:
    CVeFunctionMenu m_MenuBox[MenuIdxMax]{};
    ComPtr<IDWriteTextLayout> m_pTlTip{};
    ComPtr<IDWriteTextFormat> m_pTfTip{};
    ID2D1SolidColorBrush* m_pBrushTip{};
    D2D1_RECT_F m_rcTip{};// 相对元素
    BOOL m_bTimeLineActive{ FALSE };
public:
    LRESULT OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    void TlTick(int iMs) override;

    BOOL TlIsValid() override
    {
        return App->GetOpt().bRainbowColor && m_bTimeLineActive;
    }
};