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
			m_ptCursor = { (float)pt.x, (float)pt.y };

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
		BOOL bUpdate{};
		D2D1_RECT_F rcUpdate{};
		//===聚光灯
		if (App->GetOpt().bSpotLight)
		{
			auto pt{ n.pt };
			ScreenToClient(GetWnd()->HWnd, &pt);
			GetWnd()->Phy2Log(pt);
			ClientToElem(pt);
			m_ptCursor = { (float)pt.x, (float)pt.y };
			/*if (!App->GetOpt().bImmdiateMode &&
				(App->GetOpt().bCrosshair || App->GetOpt().bRuler || m_bShowSpotLight))
				InvalidateRect();*/
		}
		//===窗口高亮
		if (App->GetOpt().bWndHilight)
		{
			const auto hWndCursorAt = App->GetOpt().bWndHilightDetectChild ?
				eck::WndFromPoint(n.pt) : WindowFromPoint(n.pt);
			RECT rcWnd;
			GetWindowRect(hWndCursorAt, &rcWnd);
			//---更新标尺文本
			if (App->GetOpt().bRuler)
			{
				eck::CRefStrW rsRulerTip{};
				rsRulerTip.Format(L"Left = %d, Top = %d\nRight = %d, Bottom = %d",
					n.pt.x - rcWnd.left,
					n.pt.y - rcWnd.top,
					rcWnd.right - n.pt.x,
					rcWnd.bottom - n.pt.y);
				m_TcRulerCursorTip.Create(rsRulerTip.Data(), rsRulerTip.Size(),
					GetTextFormat(), GetWidthF(), GetHeightF(), m_pDC1);
			}
			if (m_hWndCursorAt == hWndCursorAt &&
				eck::EquRect(rcWnd, m_rcLastWndInScreen))
				goto SkipUpdateWndHilight;
			m_rcLastWndInScreen = rcWnd;
			//---更新标签文本
			if (m_hWndCursorAt != hWndCursorAt)
			{
				eck::CRefStrW rsWndTip{};
				rsWndTip.Clear();
				auto cchText = GetWindowTextLengthW(hWndCursorAt);
				rsWndTip.PushBack(EckStrAndLen(L"Handle: "))
					.AppendFormat(L"%p", hWndCursorAt);
				rsWndTip.PushBack(EckStrAndLen(L"\nCaption: "));
				auto psBuf = rsWndTip.PushBack(cchText);
				GetWindowTextW(hWndCursorAt, psBuf, cchText + 1);
				cchText = GetClassNameW(hWndCursorAt, nullptr, 0);
				rsWndTip.PushBack(EckStrAndLen(L"\nClass: "));
				const auto cchOld = rsWndTip.Size();
				rsWndTip.Reserve(rsWndTip.Size() + MAX_PATH);
				cchText = GetClassNameW(hWndCursorAt,
					rsWndTip.PushBack(MAX_PATH),
					MAX_PATH);
				rsWndTip.ReSize(cchOld + cchText);
				rsWndTip.AppendFormat(L"Left = %d, Top = %d\nWidth = %d, Height = %d",
					m_rcLastWndInScreen.left,
					m_rcLastWndInScreen.top,
					m_rcLastWndInScreen.right - m_rcLastWndInScreen.left,
					m_rcLastWndInScreen.bottom - m_rcLastWndInScreen.top);
				m_TcWndTip.Create(rsWndTip.Data(), rsWndTip.Size(),
					GetTextFormat(), GetWidthF(), GetHeightF(), m_pDC1);
			}
			//---
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
			Ellipse.point = D2D1::Point2F(m_ptCursor.x, m_ptCursor.y);
			Ellipse.radiusX = Ellipse.radiusY = std::max(
				App->GetOpt().fSpotLightRadius,
				m_fSpotLightMaxRadius * (1.f - m_kSpotLight));
			m_pDC->FillEllipse(Ellipse, m_pBrush);
			m_pDC->SetPrimitiveBlend(eOldBlend);
		}
		//===准星线
		m_pBrush->SetColor(((CWndMain*)GetWnd())->GetCurrAnColor());
		if (App->GetOpt().bCrosshair && App->GetOpt().bImmdiateMode)
		{
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
			if (m_TcWndTip.IsValid())
			{
				D2D1::Matrix3x2F Mat;
				m_pDC->GetTransform(&Mat);
				const auto MatNew = Mat * D2D1::Matrix3x2F::Translation(
					m_ptWndTip.x, m_ptWndTip.y);
				m_pDC->SetTransform(MatNew);
				m_pDC1->DrawGeometryRealization(m_TcWndTip.GetGeometryRealization(),
					m_pBrush);
				const auto crOld = m_pBrush->GetColor();
				m_pBrush->SetColor(D2D1_COLOR_F{ .a = 0.2f });
				m_pDC1->DrawGeometryRealization(m_TcWndTip.GetGeometryRealizationBk(),
					m_pBrush);
				m_pBrush->SetColor(crOld);
				m_pDC->SetTransform(Mat);
			}
		}
		//===标尺
		if (App->GetOpt().bRuler)
		{
			if (m_TcRulerCursorTip.IsValid())
			{
				D2D1::Matrix3x2F Mat;
				m_pDC->GetTransform(&Mat);
				const auto MatNew = Mat * D2D1::Matrix3x2F::Translation(
					m_ptCursor.x + (float)VeCxRulerTipMargin,
					m_ptCursor.y + (float)VeCyRulerTipMargin);
				m_pDC->SetTransform(MatNew);
				m_pDC1->DrawGeometryRealization(m_TcRulerCursorTip.GetGeometryRealization(),
					m_pBrush);
				m_pDC->SetTransform(Mat);
				D2D1_POINT_2F pt1, pt2;
				pt1.x = m_ptCursor.x;
				pt1.y = std::max(ps.rcfClipInElem.top, m_rcWndHili.top);
				pt2.x = m_ptCursor.x;
				pt2.y = std::min(ps.rcfClipInElem.bottom, m_rcWndHili.bottom);
				m_pDC->DrawLine(pt1, pt2, m_pBrush);
				pt2.x = std::max(ps.rcfClipInElem.left, m_rcWndHili.left);
				pt1.y = m_ptCursor.y;
				pt1.x = std::min(ps.rcfClipInElem.right, m_rcWndHili.right);
				pt2.y = m_ptCursor.y;
				m_pDC->DrawLine(pt1, pt2, m_pBrush);
			}
		}
		//===水印
		if (App->GetOpt().bWatermark)
		{
			m_pBrush->SetColor(D2D1_COLOR_F{ 1.f,1.f,1.f,0.3f });
			m_pDC1->DrawGeometryRealization(m_TcWatermark.GetGeometryRealization(),
				m_pBrush);
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
		m_pDC->QueryInterface(&m_pDC1);
		m_pDC->CreateSolidColorBrush({}, &m_pBrush);
		App->GetSignal().Connect(this, &CVeVisualContainer::OnAppEvent);
		const auto pTfKeyStroke = App->CreateTextFormat(16);

		const auto pTfWatermark = App->CreateTextFormat(42);
		const auto cchWatermark = (UINT32)App->GetOpt().rsWatermark.Size();
		m_TcWatermark.CreateTextLayout(App->GetOpt().rsWatermark.Data(),
			cchWatermark, pTfWatermark, GetWidthF(), GetHeightF());
		const auto pTlWatermark = m_TcWatermark.GetTextLayout();
		IDWriteTextLayout1* pTlWatermark1;
		pTlWatermark->QueryInterface(&pTlWatermark1);
		pTlWatermark1->SetCharacterSpacing(6, 6, 0, { 0,cchWatermark });
		pTlWatermark1->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTlWatermark1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTlWatermark1->Release();
		m_TcWatermark.CreateGeometryRealization(m_pDC1);
		pTfWatermark->Release();

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