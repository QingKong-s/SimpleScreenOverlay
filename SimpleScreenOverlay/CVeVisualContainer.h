#pragma once
#include "CVeKeyStroke.h"
#include "CVeKeyStroke2.h"

class CVeVisualContainer final : public Dui::CElem, public CFixTimeLine
{
private:
	ID2D1SolidColorBrush* m_pBrush{};
	CVeKeyStroke m_KeyStroke{};
	CVeKeyStroke2 m_KeyStroke2{};

	D2D1_POINT_2F m_ptSpotLightCenter{};
	float m_fSpotLightMaxRadius{};
	float m_msSpotLightAni{};
	float m_kSpotLight{};
	BOOLEAN m_bShowSpotLight{};
	BOOLEAN m_bSpotLightReverse{};
	BOOLEAN m_bSpotLightAnimating{};

	RECT m_rcLastWndInScreen{ -32000, -32000, 32000, 32000 };
	BOOLEAN m_bWndHiliAnimating{};
	D2D1_RECT_F m_rcWndHiliSrc{},
		m_rcWndHiliDst{},
		m_rcWndHili{};
	float m_msWndHiliAni{};
	HWND m_hWndCursorAt{};

	eck::CRefStrW m_rsWndTip{};
	IDWriteTextLayout* m_pTlWndTip{};
	D2D1_POINT_2F m_ptWndTipSrc{},
		m_ptWndTipDst{},
		m_ptWndTip{};
	float m_msWndTipAni{};
	BOOLEAN m_bWndTipAnimating{};

	void OnAppEvent(Notify eNotify, SSONOTIFY& n);

	void CalcWindowTipPos(const D2D1_RECT_F& rcWnd, _Out_ D2D1_POINT_2F& ptTip);
public:
	LRESULT OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	void STDMETHODCALLTYPE Tick(int iMs);
};