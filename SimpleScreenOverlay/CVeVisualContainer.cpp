#include "pch.h"
#include "CVeVisualContainer.h"
#include "CWndMain.h"


void CVeVisualContainer::OnAppEvent(Notify eNotify, SSONOTIFY& n)
{
	switch (eNotify)
	{
	case Notify::DoubleCtrl:
	{
		ECK_DUILOCK;

		// 先更新一次，防止脏坐标
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(GetWnd()->HWnd, &pt);
		GetWnd()->Phy2Log(pt);
		ClientToElem(pt);
		m_ptCenter = { (float)pt.x, (float)pt.y };

		ECKBOOLNOT(m_bShowSpotLight);
		m_pecSpotLight->SetReverse(!m_bShowSpotLight);
		m_pecSpotLight->Begin(eck::ECBF_CONTINUE);
		GetWnd()->WakeRenderThread();
	}
	break;
	case Notify::GlobalMouseDown:
	{
		if (!m_bShowSpotLight)
			break;
		ECK_DUILOCK;
		m_bShowSpotLight = FALSE;
		m_pecSpotLight->SetReverse(TRUE);
		m_pecSpotLight->Begin(eck::ECBF_CONTINUE);
		GetWnd()->WakeRenderThread();
	}
	break;
	case Notify::GlobalMouseMove:
	{
		auto pt{ n.pt };
		ScreenToClient(GetWnd()->HWnd, &pt);
		GetWnd()->Phy2Log(pt);
		ClientToElem(pt);
		m_ptCenter = { (float)pt.x, (float)pt.y };
		if ((App->GetOpt().bCrosshair && !App->GetOpt().bImmdiateMode) 
			&& m_bShowSpotLight)
			InvalidateRect();
	}
	break;
	}
}

LRESULT CVeVisualContainer::OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		Dui::ELEMPAINTSTRU ps;
		BeginPaint(ps, wParam, lParam);

		if (App->GetOpt().bSpotLight)
		{
			float f;
			if (m_pecSpotLight->IsActive())
				f = m_pecSpotLight->GetCurrValue();
			else if (m_bShowSpotLight)
				f = 1.f;
			else
				goto SkipDrawSpotLight;
			m_pBrush->SetColor({ 0.f,0.f,0.f,0.5f * f });
			m_pDC->FillRectangle(ps.rcfClipInElem, m_pBrush);

			const auto eOldBlend = m_pDC->GetPrimitiveBlend();
			m_pDC->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
			m_pBrush->SetColor(D2D1_COLOR_F{});
			D2D1_ELLIPSE Ellipse;
			Ellipse.point = D2D1::Point2F(m_ptCenter.x, m_ptCenter.y);
			Ellipse.radiusX = Ellipse.radiusY = std::max(
				App->GetOpt().fSpotLightRadius,
				m_fMaxRadius * (1.f - m_pecSpotLight->GetCurrValue()));
			m_pDC->FillEllipse(Ellipse, m_pBrush);
			m_pDC->SetPrimitiveBlend(eOldBlend);
		}
	SkipDrawSpotLight:;

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

		EndPaint(ps);
	}
	return 0;

	case WM_SIZE:
	{
		m_fMaxRadius = std::max(GetWidthF(), GetHeightF()) / 2.f;
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

		m_pecSpotLight = new eck::CEasingCurve{};
		InitEasingCurve(m_pecSpotLight);
		m_pecSpotLight->SetDuration(500);
		m_pecSpotLight->SetRange(0.f, 1.f);
		m_pecSpotLight->SetAnProc(eck::Easing::OutCubic);
		m_pecSpotLight->SetCallBack([](float fCurrValue, float fOldValue, LPARAM lParam)
			{
				//((CVeVisualContainer*)lParam)->InvalidateRect();
			});
	}
	break;
	case WM_DESTROY:
	{
		SafeRelease(m_pBrush);
		GetWnd()->UnregisterTimeLine(m_pecSpotLight);
		SafeRelease(m_pecSpotLight);
	}
	break;
	}
	return __super::OnEvent(uMsg, wParam, lParam);
}