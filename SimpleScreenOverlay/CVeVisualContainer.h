#pragma once
#include "CVeKeyStroke.h"
#include "CVeKeyStroke2.h"

class CVeVisualContainer final : public Dui::CElem, public eck::CFixedTimeLine
{
private:
    constexpr static float SpotLightGeoRadius = 40.f;
    constexpr static float ClickGeoRadius = 40.f;
    constexpr static float CursorLocateGeoRadius = 40.f;
    constexpr static float CursorGeoRadius = 40.f;
    constexpr static float WheelGeoSide = 1.f;
    constexpr static D2D1_POINT_2F WheelGeoCenter{ 0.5f,1.73205f / 4.f };
    constexpr static UINT TE_WHEEL = 200;
    //
    CApp::HSlot m_hSlot{};
    ID2D1DeviceContext1* m_pDC1{};
    ID2D1SolidColorBrush* m_pBrush{};
    CVeKeyStroke m_KeyStroke{};
    CVeKeyStroke2 m_KeyStroke2{};
    // 公用光标位置
    D2D1_POINT_2F m_ptCursor{};
    BOOLEAN m_bCursorPosDirty{ TRUE };
    // 聚光灯

    float m_fSpotLightMaxRadius{};
    float m_msSpotLightAni{};
    float m_kSpotLight{};
    BOOLEAN m_bShowSpotLight{};
    BOOLEAN m_bSpotLightReverse{};
    BOOLEAN m_bSpotLightAnimating{};
    ID2D1GeometryRealization* m_pGrSpotLight{};
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
    // 水印

    CGrTextCache m_TcWatermark{};
    BOOLEAN m_bWatermarkEnabled{ 2 };
    // 点击

    enum class ClickState : BYTE
    {
        None,
        L,
        M,
        R,
    };
    struct CLICK
    {
        ULONGLONG tAdd;
        D2D1_POINT_2F ptCenter;
        ClickState eState;
        BOOLEAN bUp;
        float ms;
        float fRadius;
        float fAlpha;
    };
    std::vector<CLICK> m_vClick{};
    ID2D1StrokeStyle1* m_pStrokeStyleClick{};
    ID2D1EllipseGeometry* m_pEllipseClick{};
    // 光标定位

    ID2D1GeometryRealization* m_pGrCursorLocate{};
    ID2D1RadialGradientBrush* m_pBrCursorLocate{};
    float m_msCursorLocateAni{};
    float m_fCursorLocateRadius{};
    float m_fCursorLocateAlpha{};
    BOOL m_bLocatingCursor{};
    // 光标位置

    ID2D1GeometryRealization* m_pGrCursorPos{};
    // 滚轮显示

    enum class Wheel : BYTE
    {
        None,
        Up,
        Down,
        Left,
        Right,
    };
    ID2D1GeometryRealization* m_pGrWheel{};
    ID2D1GeometryRealization* m_pGrWheelBorder{};
    D2D1_POINT_2F m_ptWheelIndicator{};
    Wheel m_eWheel{ Wheel::None };
    BOOLEAN m_bWheelTimer{};
    BYTE m_cWheelShowCount{};// 计数器，归零时取消显示

    //
    BOOLEAN m_bTimeLineActive{ TRUE };

    void OnAppEvent(Notify eNotify, SSONOTIFY& n);

    void CalcWindowTipPos(const D2D1_RECT_F& rcWnd, _Out_ D2D1_POINT_2F& ptTip);

    void UpdateCursorPos();

    void UpdateWheelIndicatorPos();
    void InvalidateWheelIndicator();

    void InitSpotLight();
    void InitClick();
    void InitCursorLocateGeoReal();
    void InitCursorLocate();
    void InitCursorPos();
    void InitWheel();
public:
    LRESULT OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    void TlTick(int iMs) override;

    BOOL TlIsValid() override
    {
        const auto bRainbow = App->GetOpt().bRainbowColor;
        return m_bTimeLineActive || m_bSpotLightAnimating ||
            !m_vClick.empty() || m_bLocatingCursor ||
            (bRainbow && App->GetOpt().bWndHilight) ||
            (bRainbow && App->GetOpt().bCrosshair);
    }
};