#pragma once
#include "CVeMenuContainer.h"
#include "CVeVisualContainer.h"

class CWndMain final : public Dui::CDuiWnd, public eck::CFixedTimeLine
{
private:
	constexpr static float BlurStdDev = 20.0f;

	eck::THREADCTX* m_ptcUiThread{};

	CVeMenuContainer m_MenuContainer{};
	CVeVisualContainer m_VisualContainer{};

	float m_msMenuAn{};
	Dui::CCompositorPageAn* m_pCompMenuSwitch{};

	ULONGLONG m_dwLastCtrlTick{};
	BOOLEAN m_bKeyDown[256]{};

	BOOLEAN m_bShowMenu{};
	BOOLEAN m_bMenuAn{};
	BOOLEAN m_bBlurBkgEnabled{ 2 };

	UINT m_uMsgTaskbarCreated{};

	CApp::HSlot m_hSlot{};

#if SSO_WINRT
	IInteropCompositorFactoryPartner* m_pInteropFactory{};
	IDCompositionDesktopDevice* m_pDcDevice{};
	com_ptr<IDCompositionTarget> m_pDcTarget{};

	Compositor m_Compositor{};
	DesktopWindowTarget m_Target{ nullptr };
	SpriteVisual m_RootVisual{ nullptr };
	Visual m_ContentVisual{ nullptr };

	CompositionEffectBrush m_BkDropBrush{ nullptr };
#endif

	BOOL InitRawInput();

	void OnInput(WPARAM wParam, LPARAM lParam);

	void OnAppEvent(Notify eNotify, SSONOTIFY& n);

	void RePosWindow();
public:
	LRESULT OnMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

#if SSO_WINRT
	LRESULT OnRenderEvent(UINT uMsg, Dui::RENDER_EVENT& e);
#endif

	void STDMETHODCALLTYPE Tick(int iMs);

	BOOL STDMETHODCALLTYPE IsValid() { return m_bMenuAn; }

	void SwitchMenuShowing(BOOL bShow);
};