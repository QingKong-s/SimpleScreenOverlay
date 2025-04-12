#pragma once
#include "CVeMenuContainer.h"
#include "CVeVisualContainer.h"

class CWndMain final : public Dui::CDuiWnd, public eck::ITimeLine
{
private:
	CVeMenuContainer m_MenuContainer{};
	CVeVisualContainer m_VisualContainer{};
	D2D1_COLOR_F m_crAnimation{ .a = 1.f };
	float m_fHue{};
#if SSO_WINRT
	IInteropCompositorFactoryPartner* m_pInteropFactory{};
	IDCompositionDesktopDevice* m_pDcDevice{};
	com_ptr<IDCompositionTarget> m_pDcTarget{};

	Compositor m_Compositor{};
	DesktopWindowTarget m_Target{ nullptr };
	SpriteVisual m_RootVisual{ nullptr };
	Visual m_ContentVisual{ nullptr };
#endif
public:
	LRESULT OnMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

#if SSO_WINRT
	LRESULT OnRenderEvent(UINT uMsg, Dui::RENDER_EVENT& e);
#endif

	ULONG STDMETHODCALLTYPE AddRef() { return 1; }
	ULONG STDMETHODCALLTYPE Release() { return 1; }
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) { return E_NOINTERFACE; }

	void STDMETHODCALLTYPE Tick(int iMs);
	BOOL STDMETHODCALLTYPE IsValid() { return TRUE; }
	int STDMETHODCALLTYPE GetCurrTickInterval() { return 0; }

	EckInlineNdCe const auto& GetCurrAnColor() const { return m_crAnimation; }
};