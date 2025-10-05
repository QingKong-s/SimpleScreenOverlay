#pragma once
#include "CModBase.h"

class CModWndMarker final : public CModBase
{
private:
    // 窗口高亮

    RECT m_rcLastWndInScreen{ -32000, -32000, 32000, 32000 };
    BOOLEAN m_bWndHiliAnimating{};
    D2D1_RECT_F m_rcWndHiliSrc{},
        m_rcWndHiliDst{},
        m_rcWndHili{};
    float m_msWndHiliAni{};
    HWND m_hWndCursorAt{};
    // 窗口提示

    CGrTextCacheWithBk m_TcWndTip{};
    D2D1_POINT_2F m_ptWndTipSrc{},
        m_ptWndTipDst{},
        m_ptWndTip{};
    float m_msWndTipAni{};
    BOOLEAN m_bWndTipAnimating{};
    // 标尺

    CGrTextCache m_TcRulerCursorTip{};

    void CalcWindowTipPos(const D2D1_RECT_F& rcWnd, _Out_ D2D1_POINT_2F& ptTip);
public:
    void ModUnInit() override;
    void ModOnAppEvent(MOD_APPEVT_PARAM& Opt) override;
    void ModOnRender(const MOD_RENDER_PARAM& Opt) override;
    BOOL ModIsValid() const override { return TRUE; }
    BOOL ModIsTimeLineValid() const override
    {
        return (App->GetOpt().bRainbowColor || m_bWndHiliAnimating) &&
            App->GetOpt().bWndHilight;
    }
    void ModOnTick(MOD_TICK_PARAM& Opt) override;
};