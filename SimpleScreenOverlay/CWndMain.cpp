#include "pch.h"
#include "CWndMain.h"

void HSV2RGB(float h, float s, float v, float& r, float& g, float& b)
{
	const int i = int(h * 6);
	const float f = h * 6 - i;
	const float p = v * (1 - s);
	const float q = v * (1 - f * s);
	const float t = v * (1 - (1 - f) * s);
	float rf, gf, bf;

	switch (i % 6)
	{
	case 0: rf = v; gf = t; bf = p; break;
	case 1: rf = q; gf = v; bf = p; break;
	case 2: rf = p; gf = v; bf = t; break;
	case 3: rf = p; gf = q; bf = v; break;
	case 4: rf = t; gf = p; bf = v; break;
	case 5: rf = v; gf = p; bf = q; break;
	default: ECK_UNREACHABLE;
	}
	r = rf;
	g = gf;
	b = bf;
}

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
			if (!n[cEvt].bRepeat && (
				ri.data.keyboard.VKey == VK_CONTROL ||
				ri.data.keyboard.VKey == VK_LCONTROL ||
				ri.data.keyboard.VKey == VK_RCONTROL))
			{
				const auto t = NtGetTickCount64();
				if (UINT(t - m_dwLastCtrlTick) <= GetDoubleClickTime())
				{
					eNotify[cEvt] = Notify::DoubleCtrl;
					++cEvt;
				}
				m_dwLastCtrlTick = NtGetTickCount64();
			}
			if (ri.data.keyboard.VKey == VK_F7)
			{
				SwitchMenuShowing(!m_bShowMenu);
			}
			++cEvt;
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

LRESULT CWndMain::OnMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INPUT:
		OnInput(wParam, lParam);
		break;

	case WM_SETTINGCHANGE:
		eck::MsgOnSettingChangeFixDpiAwareV2(hWnd, wParam, lParam);
		eck::MsgOnSettingChangeMainWnd(hWnd, wParam, lParam);
		break;

	case WM_CREATE:
	{
		m_ptcUiThread = eck::GetThreadCtx();
		InitRawInput();
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
		m_pCompMenuSwitch = new Dui::CCompositorPageAn{};
		m_pCompMenuSwitch->InitAsTranslationOpacity();
		POINT pt{ GetClientWidth(),GetClientHeight() };
		Phy2Log(pt);
		// 保证VisualContainer的Z序低于MenuContainer
		m_VisualContainer.Create(nullptr, Dui::DES_VISIBLE, 0,
			0, 0, pt.x, pt.y, nullptr, this);
		m_MenuContainer.Create(nullptr, 0, 0,
			0, 0, pt.x, pt.y, nullptr, this);
		m_VisualContainer.SetTextFormat(App->GetTextFormatCommon());
		RegisterTimeLine(this);
		return lResult;
	}
	return 0;

	case WM_DESTROY:
	{
		const auto lResult = __super::OnMsg(hWnd, uMsg, wParam, lParam);

		return lResult;
	}
	break;
	}
	return __super::OnMsg(hWnd, uMsg, wParam, lParam);
}

#if SSO_WINRT
LRESULT CWndMain::OnRenderEvent(UINT uMsg, Dui::RENDER_EVENT& e)
{
	switch (uMsg)
	{
	case Dui::RE_POSTRENDER:
	case Dui::RE_COMMIT:
	{
		m_pDcDevice->Commit();
	}
	break;
	}
	return 0;
}
#endif// SSO_WINRT

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
			return;
		}
		m_pCompMenuSwitch->Opacity = k;
		const auto cyScr = GetClientHeight();
		m_pCompMenuSwitch->Dy = int(-(cyScr / 3.f) * (1.f - k));
		m_MenuContainer.CompReCalcCompositedRect();
		//m_MenuContainer.InvalidateRect();
	}

	m_fHue += 0.002f;
	if (m_fHue >= 1.f)
		m_fHue -= 1.f;
	HSV2RGB(m_fHue, 1.f, 1.f, m_crAnimation.r, m_crAnimation.g, m_crAnimation.b);
	Redraw();
}

void CWndMain::SwitchMenuShowing(BOOL bShow)
{
	if (bShow == m_bShowMenu)
		return;
	m_MenuContainer.SetVisible(TRUE);
	m_bMenuAn = TRUE;
	m_bShowMenu = bShow;
#if SSO_WINRT
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
#endif// SSO_WINRT
	WakeRenderThread();
}