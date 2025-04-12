#include "pch.h"
#include "CVeVisualContainer.h"
#include "CWndMain.h"


void CVeVisualContainer::OnAppEvent(Notify eNotify, SSONOTIFY& n)
{
	switch (eNotify)
	{
	case Notify::DoubleCtrl:
	{
		//===聚光灯
		if (App->GetOpt().bSpotLight)
		{
			ECK_DUILOCK;
			// 先更新一次，防止脏坐标
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(GetWnd()->HWnd, &pt);
			GetWnd()->Phy2Log(pt);
			ClientToElem(pt);
			m_ptSpotLightCenter = { (float)pt.x, (float)pt.y };

			ECKBOOLNOT(m_bShowSpotLight);
			m_bSpotLightReverse = !!!m_bShowSpotLight;
			m_bSpotLightAnimating = TRUE;
			GetWnd()->WakeRenderThread();
		}
	}
	break;
	case Notify::GlobalMouseDown:
	{
		// 聚光灯
		if (App->GetOpt().bSpotLight && m_bShowSpotLight)
		{
			ECK_DUILOCK;
			m_bShowSpotLight = FALSE;
			m_bSpotLightReverse = TRUE;
			m_bSpotLightAnimating = TRUE;
			GetWnd()->WakeRenderThread();
		}
	}
	break;
	case Notify::GlobalMouseMove:
	{
		ECK_DUILOCK;
		auto pt{ n.pt };
		//===窗口高亮
		if (App->GetOpt().bWndHilight)
		{
			const auto hWndCursorAt = App->GetOpt().bWndHilightDetectChild ?
				eck::WndFromPoint(pt) : WindowFromPoint(pt);
			RECT rcWnd;
			GetWindowRect(hWndCursorAt, &rcWnd);
			if (m_hWndCursorAt == hWndCursorAt &&
				eck::EquRect(rcWnd, m_rcLastWndInScreen))
				goto SkipUpdateWndHilight;
			m_rcLastWndInScreen = rcWnd;

			SafeRelease(m_pTlWndTip);
			m_rsWndTip.Clear();
			auto cchText = GetWindowTextLengthW(hWndCursorAt);
			m_rsWndTip.PushBack(EckStrAndLen(L"Handle: "))
				.AppendFormat(L"%p", hWndCursorAt);
			m_rsWndTip.PushBack(EckStrAndLen(L"\nCaption: "));
			auto psBuf = m_rsWndTip.PushBack(cchText);
			GetWindowTextW(hWndCursorAt, psBuf, cchText + 1);
			cchText = GetClassNameW(hWndCursorAt, nullptr, 0);
			m_rsWndTip.PushBack(EckStrAndLen(L"\nClass: "));
			const auto cchOld = m_rsWndTip.Size();
			m_rsWndTip.Reserve(m_rsWndTip.Size() + MAX_PATH);
			cchText = GetClassNameW(hWndCursorAt, 
				m_rsWndTip.PushBack(MAX_PATH),
				MAX_PATH);
			m_rsWndTip.ReSize(cchOld + cchText);

			eck::ScreenToClient(GetWnd()->HWnd, &rcWnd);
			D2D1_RECT_F rcWndHili{ eck::MakeD2DRcF(rcWnd) };
			GetWnd()->Phy2Log(rcWndHili);
			ClientToElem(rcWndHili);
			if (m_hWndCursorAt)
			{
				m_hWndCursorAt = hWndCursorAt;
				m_rcWndHiliSrc = m_rcWndHili;
				m_rcWndHiliDst = rcWndHili;
				m_msWndHiliAni = 0.f;
				m_bWndHiliAnimating = TRUE;

				m_ptWndTipSrc = m_ptWndTip;
				CalcWindowTipPos(m_rcWndHiliDst, m_ptWndTipDst);
				m_msWndTipAni = 0.f;
				m_bWndTipAnimating = TRUE;
				GetWnd()->WakeRenderThread();
			}
			else
			{
				m_hWndCursorAt = hWndCursorAt;
				m_rcWndHili = rcWndHili;
				CalcWindowTipPos(m_rcWndHili, m_ptWndTip);
			}
		}
	SkipUpdateWndHilight:;
		//===聚光灯
		if (App->GetOpt().bSpotLight)
		{
			ScreenToClient(GetWnd()->HWnd, &pt);
			GetWnd()->Phy2Log(pt);
			ClientToElem(pt);
			m_ptSpotLightCenter = { (float)pt.x, (float)pt.y };
			if ((App->GetOpt().bCrosshair && !App->GetOpt().bImmdiateMode)
				&& m_bShowSpotLight)
				InvalidateRect();
		}
	}
	break;
	}
}

void CVeVisualContainer::CalcWindowTipPos(const D2D1_RECT_F& rcWnd,
	_Out_ D2D1_POINT_2F& ptTip)
{
	ptTip.x = rcWnd.left + (float)VeCxWndTipMargin;
	ptTip.y = rcWnd.top + (float)VeCyWndTipMargin;

}

LRESULT CVeVisualContainer::OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		Dui::ELEMPAINTSTRU ps;
		BeginPaint(ps, wParam, lParam);
		//===聚光灯
		if (App->GetOpt().bSpotLight &&
			(m_bShowSpotLight || m_bSpotLightAnimating))
		{
			m_pBrush->SetColor({ 0.f,0.f,0.f,0.5f * m_kSpotLight });
			m_pDC->FillRectangle(ps.rcfClipInElem, m_pBrush);

			const auto eOldBlend = m_pDC->GetPrimitiveBlend();
			m_pDC->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
			m_pBrush->SetColor(D2D1_COLOR_F{});
			D2D1_ELLIPSE Ellipse;
			Ellipse.point = D2D1::Point2F(m_ptSpotLightCenter.x, m_ptSpotLightCenter.y);
			Ellipse.radiusX = Ellipse.radiusY = std::max(
				App->GetOpt().fSpotLightRadius,
				m_fSpotLightMaxRadius * (1.f - m_kSpotLight));
			m_pDC->FillEllipse(Ellipse, m_pBrush);
			m_pDC->SetPrimitiveBlend(eOldBlend);
		}
		//===准星线
		if (App->GetOpt().bCrosshair && App->GetOpt().bImmdiateMode)
		{
			m_pBrush->SetColor(((CWndMain*)GetWnd())->GetCurrAnColor());

			const auto d = App->GetOpt().dCrosshairCursorGap;
			const auto cxLine = App->GetOpt().cxCrosshairLine;
			D2D1_POINT_2F pt1, pt2;

			POINT ptCursor;
			GetCursorPos(&ptCursor);
			ScreenToClient(GetWnd()->HWnd, &ptCursor);
			GetWnd()->Phy2Log(ptCursor);
			ClientToElem(ptCursor);

			if (ptCursor.y >= ps.rcfClipInElem.top - cxLine &&
				ptCursor.y <= ps.rcfClipInElem.bottom + cxLine)
			{
				pt1.y = (float)ptCursor.y;
				pt2.y = (float)ptCursor.y;
				pt1.x = ps.rcfClipInElem.left;
				pt2.x = (float)ptCursor.x - d;
				m_pDC->DrawLine(pt1, pt2, m_pBrush, cxLine);
				pt1.x = (float)ptCursor.x + d;
				pt2.x = ps.rcfClipInElem.right;
				m_pDC->DrawLine(pt1, pt2, m_pBrush, cxLine);
			}
			if (ptCursor.x >= ps.rcfClipInElem.left - cxLine &&
				ptCursor.x <= ps.rcfClipInElem.right + cxLine)
			{
				pt1.x = (float)ptCursor.x;
				pt2.x = (float)ptCursor.x;
				pt1.y = ps.rcfClipInElem.top;
				pt2.y = (float)ptCursor.y - d;
				m_pDC->DrawLine(pt1, pt2, m_pBrush, cxLine);
				pt1.y = (float)ptCursor.y + d;
				pt2.y = ps.rcfClipInElem.bottom;
				m_pDC->DrawLine(pt1, pt2, m_pBrush, cxLine);
			}
		}
		//===窗口高亮
		if (App->GetOpt().bWndHilight && App->GetOpt().bImmdiateMode &&
			m_hWndCursorAt)
		{
			m_pDC->DrawRectangle(m_rcWndHili, m_pBrush, (float)VeCxWndHiliBorder);
			if (!m_pTlWndTip)
			{
				eck::g_pDwFactory->CreateTextLayout(m_rsWndTip.Data(),
					(UINT32)m_rsWndTip.Size(), GetTextFormat(),
					GetWidthF(), GetHeightF(), &m_pTlWndTip);
			}
			if (m_pTlWndTip)
			{
				m_pDC->DrawTextLayout(m_ptWndTip, m_pTlWndTip, m_pBrush,
					Dui::DrawTextLayoutFlags);
			}
		}
		EndPaint(ps);
	}
	return 0;

	case WM_SIZE:
	{
		m_fSpotLightMaxRadius = std::max(GetWidthF(), GetHeightF()) / 2.f;
	}
	break;

	case WM_CREATE:
	{
		m_pDC->CreateSolidColorBrush({}, &m_pBrush);
		App->GetSignal().Connect(this, &CVeVisualContainer::OnAppEvent);
		const auto pTfKeyStroke = App->CreateTextFormat(16);

		m_KeyStroke.Create(nullptr, Dui::DES_VISIBLE, 0,
			10, 10, 150, 400, this);
		m_KeyStroke.SetTextFormat(pTfKeyStroke);

		eck::RCWH rcKs2;
		rcKs2.cx = GetWidth() / 2;
		rcKs2.x = (GetWidth() - rcKs2.cx) / 2;
		rcKs2.cy = 210;
		rcKs2.y = GetHeight() - rcKs2.cy - 10;
		m_KeyStroke2.Create(nullptr, Dui::DES_VISIBLE, 0,
			rcKs2.x, rcKs2.y, rcKs2.cx, rcKs2.cy, this);
		m_KeyStroke2.SetTextFormat(pTfKeyStroke);

		pTfKeyStroke->Release();
		GetWnd()->RegisterTimeLine(this);
	}
	break;
	case WM_DESTROY:
	{
		SafeRelease(m_pBrush);
		GetWnd()->UnregisterTimeLine(this);
	}
	break;
	}
	return __super::OnEvent(uMsg, wParam, lParam);
}

void STDMETHODCALLTYPE CVeVisualContainer::Tick(int iMs)
{
	if (m_bSpotLightAnimating)
	{
		if (m_bSpotLightReverse)
			m_msSpotLightAni -= (float)iMs;
		else
			m_msSpotLightAni += (float)iMs;
		m_kSpotLight = eck::Easing::OutCubic(m_msSpotLightAni, 0.f, 1.f, 500.f);
		if (m_kSpotLight >= 1.f || m_kSpotLight <= 0.f)
		{
			m_kSpotLight = m_bSpotLightReverse ? 0.f : 1.f;
			m_msSpotLightAni = m_bSpotLightReverse ? 0.f : 500.f;
			m_bSpotLightAnimating = FALSE;
		}
	}
	if (m_bWndHiliAnimating)
	{
		m_msWndHiliAni += (float)iMs;
		auto k = eck::Easing::OutCubic(m_msWndHiliAni, 0.f, 1.f, 150.f);
		if (k >= 1.f)
		{
			m_bWndHiliAnimating = FALSE;
			k = 1.f;
		}
		m_rcWndHili.left = (m_rcWndHiliSrc.left * (1.f - k)) + (m_rcWndHiliDst.left * k);
		m_rcWndHili.top = (m_rcWndHiliSrc.top * (1.f - k)) + (m_rcWndHiliDst.top * k);
		m_rcWndHili.right = (m_rcWndHiliSrc.right * (1.f - k)) + (m_rcWndHiliDst.right * k);
		m_rcWndHili.bottom = (m_rcWndHiliSrc.bottom * (1.f - k)) + (m_rcWndHiliDst.bottom * k);

		m_ptWndTip.x = (m_ptWndTipSrc.x * (1.f - k)) + (m_ptWndTipDst.x * k);
		m_ptWndTip.y = (m_ptWndTipSrc.y * (1.f - k)) + (m_ptWndTipDst.y * k);
	}
}