#pragma once
#include "CVeKeyStroke.h"
#include "CVeKeyStroke2.h"

class CVeVisualContainer : public Dui::CElem
{
private:
	ID2D1SolidColorBrush* m_pBrush{};
	eck::CEasingCurve* m_pecSpotLight{};
	CVeKeyStroke m_KeyStroke{};
	CVeKeyStroke2 m_KeyStroke2{};

	float m_fMaxRadius{};
	D2D1_POINT_2F m_ptCenter{};

	BOOL m_bShowSpotLight{};

	void OnAppEvent(Notify eNotify, SSONOTIFY& n);
public:
	LRESULT OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};