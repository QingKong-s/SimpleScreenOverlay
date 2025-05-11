#include "pch.h"
#include "CWndMain.h"

constexpr char Shader[] = R"VSPS(
Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

cbuffer cb: register(b0)
{
	float2 toUV;
	float2 offsetUV;
};

float4 PS(float4 pos : SV_POSITION) : SV_Target
{
	//return float4(1.f, 1.f,0.f, 0.2f);
	float4 texClr = gTexture.Sample(gSampler, (pos.xy - offsetUV) * toUV);
	return texClr;
	float r = (texClr.r * 0.2126f + texClr.g * 0.7152f + texClr.b * 0.0722f);
	return float4(r, r, r, texClr.a);
}
)VSPS";


constexpr char VSBitBlt[]{ R"VSPS(
float4 VS(float4 pos : POSITION) : SV_Position
{
	return pos;
}
)VSPS" };

constexpr char PSLight[]{ R"VSPS(
Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

cbuffer cb: register(b0)
{
	float2 toUV;
	float2 offsetUV;
	float threshold;
};
float4 PS(float4 pos : SV_POSITION) : SV_Target
{
	float4 texClr = gTexture.Sample(gSampler, (pos.xy - offsetUV) * toUV);
	float luminance = dot(texClr.rgb, float3(0.2126, 0.7152, 0.0722));
	return (luminance > threshold) ? texClr *1.f : float4(0, 0, 0, 1);
	return float4(
		saturate(((texClr.rgb) - threshold) / (1 - threshold)) * 1000.f,
		texClr.a);
}
)VSPS" };

constexpr char PSBlur[]{ R"VSPS(
Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

cbuffer cb: register(b0)
{
	float2 toUV;
	float2 offsetUV;
	float2 blurDir;
	float blurRadius;
	float sigma;
};
struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};
float Gaussian(float x, float sigma)
{
	return exp(-0.5 * (x * x) / (sigma * sigma));
}
float4 PS(float4 pos : SV_POSITION) : SV_Target
{
	float weightSum = 0;

	float4 color = float4(0, 0, 0, 0);
	for (int i = -blurRadius; i <= blurRadius; ++i)
	{
		float offset = float(i);
		float2 offsetNew = offsetUV + offset * blurDir;
		float weight = Gaussian(offset, sigma);
		float4 sample = gTexture.Sample(gSampler, (pos.xy - offsetNew) * toUV);
		color += (sample * weight);
		weightSum += weight;
	}
	//color.a = 0;
	color /= weightSum;
	return color;
}
)VSPS" };

constexpr char PSFinal[]{ R"VSPS(
Texture2D gTextureBase : register(t0);
Texture2D gTextureBloom : register(t1);
SamplerState gSampler : register(s0);

cbuffer cb: register(b0)
{
	float2 toUV;
	float2 offsetUV;
	float BaseIntensity;
	float BaseSaturation;
	float BloomIntensity;
	float BloomSaturation;
};

float4 AdjustSaturation(float4 color, float saturation)
{
	float grey = dot(color.rgb, float3(0.3, 0.59, 0.11));
	return lerp(grey, color, saturation);
}

float4 PS(float4 pos : SV_POSITION) : SV_Target
{
	float4 bloom = gTextureBloom.Sample(gSampler, (pos.xy - offsetUV) * toUV);
	float4 base = gTextureBase.Sample(gSampler, (pos.xy - offsetUV) * toUV);
	float a1 = base.a, a2 = bloom.a;
	//return bloom;

	bloom = AdjustSaturation(bloom, BloomSaturation) * BloomIntensity;
	base = AdjustSaturation(base, BaseSaturation) * BaseIntensity;

	base *= (1 - saturate(bloom));
	float4 outClr = base + bloom;
	outClr.a = min(a1, a2);
	// outClr.a = 1;
	return outClr;
}
)VSPS" };


BOOL CWndMain::InitRawInput()
{
	(void)GetKeyboardState((BYTE*)&m_bKeyDown);
	for (auto& e : m_bKeyDown)
		if (e & 0x80)
			e = TRUE;
		else
			e = FALSE;
	const RAWINPUTDEVICE rid[]
	{
		{ 0x0001,0x0002,RIDEV_INPUTSINK,HWnd },
		{ 0x0001,0x0006,RIDEV_INPUTSINK,HWnd },
	};
	return RegisterRawInputDevices(rid, ARRAYSIZE(rid), sizeof(RAWINPUTDEVICE));
}

void CWndMain::OnInput(WPARAM wParam, LPARAM lParam)
{
	RAWINPUT ri;
	UINT cbBuf{ sizeof(ri) };
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
		&ri, &cbBuf, sizeof(RAWINPUTHEADER));
	Notify eNotify[11];
	SSONOTIFY n[11]{};
	size_t cEvt{};
	switch (ri.header.dwType)
	{
	case RIM_TYPEMOUSE:
	{
		if ((ri.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) ||
			(ri.data.mouse.lLastX || ri.data.mouse.lLastY))
		{
			GetCursorPos(&n[cEvt].pt);
			eNotify[cEvt] = Notify::GlobalMouseMove;
			++cEvt;
		}

		if (ri.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
		{
			eNotify[cEvt] = Notify::GlobalMouseDown;
			n[cEvt].Vk = VK_LBUTTON;
			++cEvt;
		}
		if (ri.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
		{
			eNotify[cEvt] = Notify::GlobalMouseUp;
			n[cEvt].Vk = VK_LBUTTON;
			++cEvt;
		}
		if (ri.data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
		{
			eNotify[cEvt] = Notify::GlobalMouseDown;
			n[cEvt].Vk = VK_RBUTTON;
			++cEvt;
		}
		if (ri.data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
		{
			eNotify[cEvt] = Notify::GlobalMouseUp;
			n[cEvt].Vk = VK_RBUTTON;
			++cEvt;
		}
		if (ri.data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
		{
			eNotify[cEvt] = Notify::GlobalMouseDown;
			n[cEvt].Vk = VK_MBUTTON;
			++cEvt;
		}
		if (ri.data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)
		{
			eNotify[cEvt] = Notify::GlobalMouseUp;
			n[cEvt].Vk = VK_MBUTTON;
			++cEvt;
		}
		if (ri.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN)
		{
			eNotify[cEvt] = Notify::GlobalMouseDown;
			n[cEvt].Vk = VK_XBUTTON1;
			++cEvt;
		}
		if (ri.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP)
		{
			eNotify[cEvt] = Notify::GlobalMouseDown;
			n[cEvt].Vk = VK_XBUTTON1;
			++cEvt;
		}
		if (ri.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN)
		{
			eNotify[cEvt] = Notify::GlobalMouseDown;
			n[cEvt].Vk = VK_XBUTTON2;
			++cEvt;
		}
		if (ri.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP)
		{
			eNotify[cEvt] = Notify::GlobalMouseDown;
			n[cEvt].Vk = VK_XBUTTON2;
			++cEvt;
		}
		for (size_t i = 0; i < cEvt; ++i)
			App->GetSignal().Emit(eNotify[i], n[i]);
	}
	break;
	case RIM_TYPEKEYBOARD:
	{
		if (ri.data.keyboard.Message == WM_KEYDOWN ||
			ri.data.keyboard.Message == WM_SYSKEYDOWN)
		{
			if (m_bKeyDown[ri.data.keyboard.VKey])
				n[cEvt].bRepeat = TRUE;
			m_bKeyDown[ri.data.keyboard.VKey] = TRUE;
			eNotify[cEvt] = Notify::GlobalKeyDown;
			n[cEvt].Vk = ri.data.keyboard.VKey;
			const auto bRepeat = n[cEvt].bRepeat;
			++cEvt;
			if (!bRepeat && (
				ri.data.keyboard.VKey == VK_CONTROL ||
				ri.data.keyboard.VKey == VK_LCONTROL ||
				ri.data.keyboard.VKey == VK_RCONTROL))
			{
				const auto t = NtGetTickCount64();
				if (UINT(t - m_dwLastCtrlTick) <= GetDoubleClickTime())
				{
					eNotify[cEvt] = Notify::DoubleCtrl;
					n[cEvt].bRepeat = TRUE;
					++cEvt;
				}
				m_dwLastCtrlTick = NtGetTickCount64();
			}
			if (ri.data.keyboard.VKey == VK_F7)
			{// HACK
				SwitchMenuShowing(!m_bShowMenu);
			}
		}
		if (ri.data.keyboard.Message == WM_KEYUP ||
			ri.data.keyboard.Message == WM_SYSKEYUP)
		{
			m_bKeyDown[ri.data.keyboard.VKey] = FALSE;
			eNotify[cEvt] = Notify::GlobalKeyUp;
			n[cEvt].Vk = ri.data.keyboard.VKey;
			++cEvt;
		}
		for (size_t i = 0; i < cEvt; ++i)
			App->GetSignal().Emit(eNotify[i], n[i]);
	}
	break;
	}
}

void CWndMain::OnAppEvent(Notify eNotify, SSONOTIFY& n)
{
	switch (eNotify)
	{
	case Notify::OptionsChanged:
#if SSO_WINRT
		if (m_bBlurBkgEnabled != App->GetOpt().bBlurBkg)
		{
			if (!App->GetOpt().bBlurBkg)
				m_RootVisual.Brush(nullptr);
			else if (m_bShowMenu)
				m_RootVisual.Brush(m_BkDropBrush);
		}
		m_bBlurBkgEnabled = App->GetOpt().bBlurBkg;
#endif// SSO_WINRT
		break;
	}
}

void CWndMain::SdEnsureTextureDimension(int cx, int cy)
{
	//if (!m_pD3DTxRenderResult || m_cxD3DTx < cx || m_cyD3DTx < cy)
	//{
	//	SafeRelease(m_pD3DTxRenderResult);
	//	m_cxD3DTx = cx;
	//	m_cyD3DTx = cy;
	//	D3D11_TEXTURE2D_DESC Desc{};
	//	Desc.Width = cx;
	//	Desc.Height = cy;
	//	Desc.MipLevels = 1;
	//	Desc.ArraySize = 1;
	//	Desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	//	Desc.SampleDesc.Count = 1;
	//	Desc.Usage = D3D11_USAGE_DEFAULT;
	//	Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//	Desc.CPUAccessFlags = 0;
	//	Desc.MiscFlags = 0;
	//	eck::g_pD3d11Device->CreateTexture2D(&Desc, nullptr, &m_pD3DTxRenderResult);
	//	eck::g_pD3d11Device->CreateTexture2D(&Desc, nullptr, &m_pD3DTxtMid1);
	//	eck::g_pD3d11Device->CreateTexture2D(&Desc, nullptr, &m_pD3DTxtMid2);
	//	m_pD3DTxRenderResult->QueryInterface(&m_pDxgiSfcRenderResult);

	//	SafeRelease(m_pD3DSrvRenderResult);
	//	D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc{};
	//	SrvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	//	SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//	SrvDesc.Texture2D.MipLevels = 1;
	//	eck::g_pD3d11Device->CreateShaderResourceView(
	//		m_pD3DTxRenderResult, &SrvDesc, &m_pD3DSrvRenderResult);
	//	//m_pD3DC->GenerateMips(m_pD3DSrvRenderResult);
	//}
}

void CWndMain::SdInitD3D()
{
	eck::g_pD3d11Device->GetImmediateContext(&m_pD3DC);
	const auto cxClient = GetClientWidth();
	const auto cyClient = GetClientHeight();
	m_TexMid1.Create(cxClient / 2, cyClient / 2);
	m_RtvMid1.Create(m_TexMid1.Get());
	m_SrvMid1.Create(m_TexMid1.Get());
	m_TexMid2.Create(cxClient / 2, cyClient / 2);
	//m_TexMid2.Create(cxClient, cyClient, DXGI_FORMAT_R16G16B16A16_FLOAT);
	m_RtvMid2.Create(m_TexMid2.Get());
	m_SrvMid2.Create(m_TexMid2.Get());
	m_cxD3DTx = cxClient;
	m_cyD3DTx = cyClient;
	m_TexRenderResult.Create(cxClient, cyClient);
	m_TexRenderResult->QueryInterface(&m_pDxgiSfcRenderResult);
	m_RtvRenderResult.Create(m_TexRenderResult.Get());
	m_SrvRenderResult.Create(m_TexRenderResult.Get());

	m_Sampler.Create();

	constexpr D3D11_INPUT_ELEMENT_DESC Layout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT },
	};
	m_VSBitBlt.Create(EckStrAndLen(VSBitBlt), "VS", Layout, ARRAYSIZE(Layout));
	m_PSLight.Create(EckStrAndLen(PSLight), "PS");
	m_PSBlur.Create(EckStrAndLen(PSBlur), "PS");
	m_PSFinal.Create(EckStrAndLen(PSFinal), "PS");
	m_PSBitBlt.Create(EckStrAndLen(Shader), "PS");

	m_BufConstBitBlt.Create(sizeof(m_CbBitBlt), D3D11_BIND_CONSTANT_BUFFER,
		D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	m_BufConstLight.Create(sizeof(m_CbLight), D3D11_BIND_CONSTANT_BUFFER,
		D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	m_BufConstBlur.Create(sizeof(m_CbBlur), D3D11_BIND_CONSTANT_BUFFER,
		D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	m_BufConstFinal.Create(sizeof(m_CbFinal), D3D11_BIND_CONSTANT_BUFFER,
		D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	DirectX::XMFLOAT4 Vet[6]
	{
		{ -1.f, 1.f, 0.f, 1.f },
		{ 1.f, 1.f, 0.f, 1.f },
		{ -1.f, -1.f, 0.f, 1.f },
		{ -1.f, -1.f, 0.f, 1.f },
		{ 1.f, 1.f, 0.f, 1.f },
		{ 1.f, -1.f, 0.f, 1.f }
	};
	m_BufVertex.Create(sizeof(Vet), D3D11_BIND_VERTEX_BUFFER,
		D3D11_USAGE_IMMUTABLE, 0, 0, Vet);

	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11BlendState* blendState = nullptr;
	eck::g_pD3d11Device->CreateBlendState(&blendDesc, &m_pD3DBS);
}

LRESULT CWndMain::OnMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INPUT:
		OnInput(wParam, lParam);
		break;

	case WM_SETTINGCHANGE:
	{
		eck::MsgOnSettingChangeFixDpiAwareV2(hWnd, wParam, lParam);
		if (eck::MsgOnSettingChangeMainWnd(hWnd, wParam, lParam, TRUE))
		{
			const auto bDark = ShouldAppsUseDarkMode();
			App->SetDarkMode(bDark);
			StSwitchStdThemeMode(bDark);
		}
	}
	break;

	case WM_CREATE:
	{
		SetDrawDirtyRect(TRUE);
		m_ptcUiThread = eck::GetThreadCtx();
		InitRawInput();
		BOOL bExcludeFromPeek{ TRUE };
		DwmSetWindowAttribute(hWnd, DWMWA_EXCLUDED_FROM_PEEK,
			&bExcludeFromPeek, sizeof(BOOL));

#if SSO_WINRT
		// 初始化互操作混合器
		eck::DciCreateInteropCompositorFactory(eck::g_pD2dDevice,
			m_pInteropFactory, m_Compositor, m_pDcDevice);
		m_RootVisual = m_Compositor.CreateSpriteVisual();
		m_RootVisual.RelativeSizeAdjustment({ 1.f, 1.f });
		HwndTarget* Dummy;
		eck::DciCreateWindowTarget(hWnd, TRUE, m_Compositor, m_pDcDevice,
			m_RootVisual, *m_pDcTarget.put(), m_Target, Dummy);

		com_ptr<IDCompositionVisual2> pContentVisual;
		m_pDcDevice->CreateVisual(pContentVisual.put());
		m_ContentVisual = pContentVisual.as<Visual>();
		m_ContentVisual.RelativeSizeAdjustment({ 1.f, 1.f });
		m_RootVisual.Children().InsertAtTop(m_ContentVisual);

		eck::CBecBlur Bec{};
		Bec.FxBlur->NameString().DupString(EckStrAndLen(L"Blur"));
		D2D1_COLOR_F crTint{ 1.f,1.f,1.f }, crLuma;
		eck::BecGetEffectiveColor(crTint, crLuma, 1.f, 0.85f);
		Bec.Connect(crTint);
		auto Factory{ m_Compositor.CreateEffectFactory(
			Bec.GetOutputEffect().as<winrt::Windows::Graphics::Effects::IGraphicsEffect>(),
			{ L"Blur.Deviation" }) };
		m_BkDropBrush = Factory.CreateBrush();
		m_BkDropBrush.SetSourceParameter(Bec.NBackdrop, m_Compositor.CreateBackdropBrush());

		const auto pDcVisual = m_ContentVisual.as<IDCompositionVisual2>();
		IDCompositionDevice* pDcDevice;
		m_pDcDevice->QueryInterface(&pDcDevice);
		DcvInit(pDcVisual.get(), pDcDevice);
#endif// SSO_WINRT
		const auto lResult = __super::OnMsg(hWnd, uMsg, wParam, lParam);
		SdInitD3D();

		m_pCompMenuSwitch = new Dui::CCompositorPageAn{};
		m_pCompMenuSwitch->InitAsTranslationOpacity();
		POINT pt{ GetClientWidth(),GetClientHeight() };
		Phy2Log(pt);
		// 保证VisualContainer的Z序低于MenuContainer
		m_VisualContainer.Create(nullptr, 0, 0,
			0, 0, pt.x, pt.y, nullptr, this);
		m_MenuContainer.Create(nullptr, 0, 0,
			0, 0, pt.x, pt.y, nullptr, this);
		m_VisualContainer.SetTextFormat(App->GetTextFormatCommon());
		RegisterTimeLine(this);

		const auto bDark = ShouldAppsUseDarkMode();
		App->SetDarkMode(bDark);
		StSwitchStdThemeMode(bDark);

		m_hSlot = App->GetSignal().Connect(this, &CWndMain::OnAppEvent);
		SSONOTIFY n{};
		App->GetSignal().Emit(Notify::OptionsChanged, n);

		m_VisualContainer.SetVisible(TRUE);
		return lResult;
	}
	return 0;

	case WM_DESTROY:
	{
		App->GetSignal().Disconnect(m_hSlot);
		UnregisterTimeLine(this);
		const auto lResult = __super::OnMsg(hWnd, uMsg, wParam, lParam);
#if SSO_WINRT
		SafeRelease(m_pInteropFactory);
		m_pDcTarget = nullptr;
		m_Target.Close();
		m_Target = nullptr;
		m_ContentVisual = nullptr;
		m_RootVisual = nullptr;
		m_BkDropBrush.Close();
		m_BkDropBrush = nullptr;
		m_Compositor = nullptr;
		m_pDcDevice->Commit();
		m_pDcDevice->WaitForCommitCompletion();
		SafeRelease(m_pDcDevice);
#endif// SSO_WINRT
		SafeRelease(m_pCompMenuSwitch);
		PostQuitMessage(0);
		return lResult;
	}
	break;
	}
	return __super::OnMsg(hWnd, uMsg, wParam, lParam);
}

LRESULT CWndMain::OnRenderEvent(UINT uMsg, Dui::RENDER_EVENT& e)
{
	switch (uMsg)
	{
	case Dui::RE_PRERENDER:
		//return 0;
	{
		int cx, cy;
		if (e.PreRender.prcDirtyPhy)
		{
			cx = e.PreRender.prcDirtyPhy->right - e.PreRender.prcDirtyPhy->left;
			cy = e.PreRender.prcDirtyPhy->bottom - e.PreRender.prcDirtyPhy->top;
		}
		else
		{
			cx = GetClientWidth();
			cy = GetClientHeight();
		}
		SdEnsureTextureDimension(cx, cy);
		e.PreRender.pSfcNewDst = m_pDxgiSfcRenderResult;
		auto& rcNew = *e.PreRender.prcNewDirtyPhy;
		rcNew.left = 0;
		rcNew.top = 0;
		rcNew.right = cx;
		rcNew.bottom = cy;
	}
	return Dui::RER_REDIRECTION;
	case Dui::RE_POSTRENDER:
	{
		constexpr float ClearColor[4]{ 0.f,0.f,0.f,0.f };
		const auto& rcNew = *e.PreRender.prcNewDirtyPhy;
		const auto cx = rcNew.right - rcNew.left;
		const auto cy = rcNew.bottom - rcNew.top;
		// 通用状态
		m_pD3DC->IASetInputLayout(m_VSBitBlt.GetInputLayout());
		constexpr UINT Stride = sizeof(DirectX::XMFLOAT4);
		constexpr UINT Offset = 0;
		m_pD3DC->IASetVertexBuffers(0, 1, &m_BufVertex.pBuffer, &Stride, &Offset);
		m_pD3DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pD3DC->VSSetShader(m_VSBitBlt.GetShader(), nullptr, 0);
		m_pD3DC->OMSetBlendState(m_pD3DBS, nullptr, 0xffffffff);
		m_pD3DC->PSSetSamplers(0, 1, &m_Sampler.pSampler);
		// 最终图面渲染目标视图
		ComPtr<ID3D11RenderTargetView> pRtv;
		ComPtr<ID3D11Resource> pSfcRes;
		e.PreRender.pSfcFinalDst->QueryInterface(&pSfcRes);
		eck::g_pD3d11Device->CreateRenderTargetView(pSfcRes.Get(), nullptr, &pRtv);
		D3D11_VIEWPORT Viewport{};
		// 提取亮色
		m_pD3DC->ClearRenderTargetView(m_RtvMid1.Get(), ClearColor);
		m_pD3DC->OMSetRenderTargets(1, &m_RtvMid1.pRtv, nullptr);
		/*Viewport.TopLeftX = e.PreRender.ptOffsetPhy.x;
		Viewport.TopLeftY = e.PreRender.ptOffsetPhy.y;*/
		Viewport.Width = cx;
		Viewport.Height = cy;
		Viewport.MinDepth = 0.f;
		Viewport.MaxDepth = 1.f;
		m_pD3DC->RSSetViewports(1, &Viewport);

		m_CbLight.ToUV = { 1.f / m_cxD3DTx, 1.f / m_cyD3DTx };
		m_CbLight.Threshold = 0.1f;
		D3D11_MAPPED_SUBRESOURCE Mapped{};
		auto hr = m_pD3DC->Map(m_BufConstLight.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
		memcpy(Mapped.pData, &m_CbLight, sizeof(m_CbLight));
		m_pD3DC->Unmap(m_BufConstLight.Get(), 0);

		m_pD3DC->PSSetShader(m_PSLight.Get(), nullptr, 0);
		m_pD3DC->PSSetConstantBuffers(0, 1, &m_BufConstLight.pBuffer);
		m_pD3DC->PSSetShaderResources(0, 1, &m_SrvRenderResult.pSrv);

		m_pD3DC->Draw(6, 0);
		// 横向模糊
		m_pD3DC->ClearRenderTargetView(m_RtvMid2.Get(), ClearColor);
		m_pD3DC->OMSetRenderTargets(1, &m_RtvMid2.pRtv, nullptr);
		m_CbBlur.ToUV = { 1.f / m_cxD3DTx, 1.f / m_cyD3DTx };
		m_CbBlur.OffsetUV = { 0.f, 0.f };
		m_CbBlur.Sigma = 10.f;
		m_CbBlur.Radius = 30.f;
		m_CbBlur.Direction = { 1.f,0.f };
		hr = m_pD3DC->Map(m_BufConstBlur.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
		memcpy(Mapped.pData, &m_CbBlur, sizeof(m_CbBlur));
		m_pD3DC->Unmap(m_BufConstBlur.Get(), 0);

		m_pD3DC->PSSetShader(m_PSBlur.Get(), nullptr, 0);
		m_pD3DC->PSSetConstantBuffers(0, 1, &m_BufConstBlur.pBuffer);
		m_pD3DC->PSSetShaderResources(0, 1, &m_SrvMid1.pSrv);

		m_pD3DC->Draw(6, 0);
		// 纵向模糊
		m_pD3DC->ClearRenderTargetView(m_RtvMid1.Get(), ClearColor);
		m_pD3DC->OMSetRenderTargets(1, &m_RtvMid1.pRtv, nullptr);
		m_CbBlur.Direction = { 0.f,1.f };
		hr = m_pD3DC->Map(m_BufConstBlur.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
		memcpy(Mapped.pData, &m_CbBlur, sizeof(m_CbBlur));
		m_pD3DC->Unmap(m_BufConstBlur.Get(), 0);

		m_pD3DC->PSSetShaderResources(0, 1, &m_SrvMid2.pSrv);

		m_pD3DC->Draw(6, 0);
		// 叠加
		Viewport.TopLeftX = e.PreRender.ptOffsetPhy.x;
		Viewport.TopLeftY = e.PreRender.ptOffsetPhy.y;
		m_pD3DC->RSSetViewports(1, &Viewport);

		m_pD3DC->ClearRenderTargetView(pRtv.Get(), ClearColor);
		m_pD3DC->OMSetRenderTargets(1, pRtv.AddrOf(), nullptr);

		m_CbFinal.ToUV = { 1.f / m_cxD3DTx, 1.f / m_cyD3DTx };
		m_CbFinal.OffsetUV = { Viewport.TopLeftX, Viewport.TopLeftY };
		m_CbFinal.BloomIntensity = 1.25f;
		m_CbFinal.BaseIntensity = 1.f;
		m_CbFinal.BloomSaturation = 1.f;
		m_CbFinal.BaseSaturation = 1.f;
		hr = m_pD3DC->Map(m_BufConstFinal.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
		memcpy(Mapped.pData, &m_CbFinal, sizeof(m_CbFinal));
		m_pD3DC->Unmap(m_BufConstFinal.Get(), 0);

		m_pD3DC->PSSetShader(m_PSFinal.Get(), nullptr, 0);
		m_pD3DC->PSSetConstantBuffers(0, 1, &m_BufConstFinal.pBuffer);
		ID3D11ShaderResourceView* const pSrv[]
		{
			m_SrvRenderResult.Get(),
			m_SrvMid1.Get(),
		};
		m_pD3DC->PSSetShaderResources(0, 2, pSrv);

		m_pD3DC->Draw(6, 0);

		//m_CbLight.ToUV = { 1.f / m_cxD3DTx, 1.f / m_cyD3DTx };
		//m_CbLight.OffsetUV = { Viewport.TopLeftX, Viewport.TopLeftY };
		//m_pD3DC->Map(m_BufConstLight.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
		//memcpy(Mapped.pData, &m_CbLight, sizeof(m_CbLight));
		//m_pD3DC->Unmap(m_BufConstLight.Get(), 0);

		//m_pD3DC->Draw(6, 0);
	}
	return 0;
#if SSO_WINRT
	case Dui::RE_COMMIT:
		m_pDcDevice->Commit();
		return 0;
#endif// SSO_WINRT
	}
	return 0;
}

void CWndMain::Tick(int iMs)
{
	if (m_bMenuAn)
	{
		if (!m_MenuContainer.GetCompositor())
			m_MenuContainer.SetCompositor(m_pCompMenuSwitch);
		constexpr float MenuAnDuration = 400.f;
		if (m_bShowMenu)
			m_msMenuAn += iMs;
		else
			m_msMenuAn -= iMs;
		const auto k = eck::Easing::OutCubic(m_msMenuAn, 0.f, 1.f, MenuAnDuration);
		if (k > 1.f || k < 0.f)
		{
			if (m_bShowMenu)
			{
				m_msMenuAn = MenuAnDuration;
				m_ptcUiThread->Callback.EnQueueCallback([this]
					{
						SetExStyle(GetExStyle() & ~(WS_EX_TRANSPARENT));
					}, 1);
			}
			else
			{
				m_msMenuAn = 0.f;
				m_MenuContainer.SetVisible(FALSE);
				m_ptcUiThread->Callback.EnQueueCallback([this]
					{
						SetExStyle(GetExStyle() | WS_EX_TRANSPARENT);
					}, 1);
			}
			m_MenuContainer.SetCompositor(nullptr);
			m_bMenuAn = FALSE;
			Redraw();
			return;
		}
		m_pCompMenuSwitch->Opacity = k;
		const auto cyScr = GetClientHeightLog();
		m_pCompMenuSwitch->Dy = int(-(cyScr / 6.f) * (1.f - k));
		m_MenuContainer.CompReCalcCompositedRect();
		Redraw();
	}
}

void CWndMain::SwitchMenuShowing(BOOL bShow)
{
	if (bShow == m_bShowMenu)
		return;
	m_MenuContainer.SetVisible(TRUE);
	m_bMenuAn = TRUE;
	m_bShowMenu = bShow;
#if SSO_WINRT
	if (App->GetOpt().bBlurBkg)
	{
		m_BkDropBrush.StopAnimation(L"Blur.Deviation");
		m_RootVisual.Brush(m_BkDropBrush);
		auto Batch{ m_Compositor.CreateScopedBatch(CompositionBatchTypes::Animation) };
		auto An{ m_Compositor.CreateScalarKeyFrameAnimation() };
		An.Duration(800ms);
		if (bShow)
		{
			An.InsertKeyFrame(0.f, 0.f);
			An.InsertKeyFrame(1.f, 20.f);
		}
		else
		{
			An.InsertKeyFrame(0.f, 20.f);
			An.InsertKeyFrame(1.f, 0.f);
		}
		m_BkDropBrush.StartAnimation(L"Blur.Deviation", An);
		Batch.End();
		Batch.Completed([this](const auto&, const auto&)
			{
				m_ptcUiThread->Callback.EnQueueCallback([this]
					{
						if (!m_bShowMenu && !m_bMenuAn)
							m_RootVisual.Brush(nullptr);
					}, 0);
			});
	}
#endif// SSO_WINRT
	WakeRenderThread();
}