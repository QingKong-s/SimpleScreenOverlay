#pragma once
#include "CVeMenuContainer.h"
#include "CVeVisualContainer.h"

class CWndMain final : public Dui::CDuiWnd, public eck::CFixedTimeLine
{
private:
	eck::THREADCTX* m_ptcUiThread{};

	CVeMenuContainer m_MenuContainer{};
	CVeVisualContainer m_VisualContainer{};

	float m_msMenuAn{};
	Dui::CCompositorPageAn* m_pCompMenuSwitch{};

	ULONGLONG m_dwLastCtrlTick{};
	BOOLEAN m_bKeyDown[256]{};

	BITBOOL m_bShowMenu : 1{};
	BITBOOL m_bMenuAn : 1{};
	BOOLEAN m_bBlurBkgEnabled{ 2 };

	CApp::HSlot m_hSlot{};



	ID3D11DeviceContext* m_pD3DC{};
	int m_cxD3DTx{}, m_cyD3DTx{};
	ID3D11BlendState* m_pD3DBS{};

	EzDx::CTexture m_TexMid1{};
	EzDx::CRenderTargetView m_RtvMid1{};
	EzDx::CShaderResourceView m_SrvMid1{};

	EzDx::CTexture m_TexMid2{};
	EzDx::CRenderTargetView m_RtvMid2{};
	EzDx::CShaderResourceView m_SrvMid2{};

	EzDx::CTexture m_TexRenderResult{};
	EzDx::CRenderTargetView m_RtvRenderResult{};
	EzDx::CShaderResourceView m_SrvRenderResult{};
	IDXGISurface* m_pDxgiSfcRenderResult{};

	EzDx::CSampler m_Sampler{};
	EzDx::CShader<EzDx::PS_T> m_PSLight{}, m_PSBlur{},
		m_PSFinal{}, m_PSBitBlt{};
	EzDx::CVSAndInputLayout m_VSBitBlt{};

	struct alignas(16) CB_BITBLT
	{
		DirectX::XMFLOAT2 ToUV;
		DirectX::XMFLOAT2 OffsetUV;
	};
	struct alignas(16) CB_LIGHT
	{
		DirectX::XMFLOAT2 ToUV;
		DirectX::XMFLOAT2 OffsetUV;
		float Threshold;
	};
	struct alignas(16) CB_BLUR
	{
		DirectX::XMFLOAT2 ToUV;
		DirectX::XMFLOAT2 OffsetUV;
		DirectX::XMFLOAT2 Direction;
		float Radius;
		float Sigma;
	};
	struct alignas(16) CB_FINAL
	{
		DirectX::XMFLOAT2 ToUV;
		DirectX::XMFLOAT2 OffsetUV;
		float BaseIntensity;
		float BaseSaturation;
		float BloomIntensity;
		float BloomSaturation;
	};
	CB_BITBLT m_CbBitBlt{};
	CB_LIGHT m_CbLight{};
	CB_BLUR m_CbBlur{};
	CB_FINAL m_CbFinal{};
	EzDx::CBuffer m_BufConstBitBlt{}, m_BufConstLight{},
		m_BufConstBlur{}, m_BufConstFinal{};
	EzDx::CBuffer m_BufVertex{};

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

	void SdEnsureTextureDimension(int cx, int cy);

	void SdInitD3D();
public:
	LRESULT OnMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	LRESULT OnRenderEvent(UINT uMsg, Dui::RENDER_EVENT& e);

	void STDMETHODCALLTYPE Tick(int iMs);

	BOOL STDMETHODCALLTYPE IsValid() { return m_bMenuAn; }

	void SwitchMenuShowing(BOOL bShow);
};