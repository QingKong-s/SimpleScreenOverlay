#pragma once
#include "CVeKeyStroke.h"
#include "CVeKeyStroke2.h"
#include "CModClickShow.h"
#include "CModCrosshair.h"
#include "CModCursorLocation.h"
#include "CModCursorPos.h"
#include "CModSpotLight.h"
#include "CModWheelIndicator.h"
#include "CModWndMarker.h"

class CVeVisualContainer final : public Dui::CElem, public eck::CFixedTimeLine
{
private:
    CApp::HSlot m_hSlot{};
    ID2D1DeviceContext1* m_pDC1{};
    ID2D1SolidColorBrush* m_pBrTemp{}, * m_pBrRainbow{};
    CVeKeyStroke m_KeyStroke{};
    CVeKeyStroke2 m_KeyStroke2{};

    D2D1_POINT_2F m_ptCursor{};
    BOOLEAN m_bCursorPosDirty{ TRUE };

    // 模块
    CModCrosshair m_ModCrosshair{};
    CModCursorPos m_ModCursorPos{};
    CModCursorLocation m_ModCursorLocation{};
    CModSpotLight m_ModSpotLight{};
    CModClickShow m_ModClickShow{};
    CModWndMarker m_ModWndMarker{};


    // 水印

    CGrTextCache m_TcWatermark{};
    BOOLEAN m_bWatermarkEnabled{ 2 };

    BOOLEAN m_bTimeLineActive{ TRUE };

    void OnAppEvent(Notify eNotify, const SSONOTIFY& n);
public:
    LRESULT OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    void TlTick(int iMs) override;
    BOOL TlIsValid() override;

    void UpdateCursorPos();
    EckInlineNdCe auto& GetCurrentCursorPos() { return m_ptCursor; }
    EckInlineNdCe auto GetDC1() const { return m_pDC1; }
};