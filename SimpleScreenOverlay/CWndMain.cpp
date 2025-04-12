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

LRESULT CWndMain::OnMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
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

		const auto pDcVisual = m_ContentVisual.as<IDCompositionVisual2>();
		IDCompositionDevice* pDcDevice;
		m_pDcDevice->QueryInterface(&pDcDevice);
		DcvInit(pDcVisual.get(), pDcDevice);
#endif// SSO_WINRT
		const auto lResult = __super::OnMsg(hWnd, uMsg, wParam, lParam);
		POINT pt{ GetClientWidth(),GetClientHeight() };
		Phy2Log(pt);
		// 保证VisualContainter的Z序低于MenuContainer
		m_VisualContainer.Create(nullptr, Dui::DES_VISIBLE, 0,
			0, 0, pt.x, pt.y, nullptr, this);
		m_MenuContainer.Create(nullptr, Dui::DES_VISIBLE, 0,
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
	m_fHue += 0.002f;
	if (m_fHue >= 1.f)
		m_fHue -= 1.f;
	HSV2RGB(m_fHue, 1.f, 1.f, m_crAnimation.r, m_crAnimation.g, m_crAnimation.b);
	Redraw();
}