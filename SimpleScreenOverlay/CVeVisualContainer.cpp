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
			UpdateCursorPos();// 先更新一次，防止脏坐标

			ECKBOOLNOT(m_bShowSpotLight);
			m_bSpotLightReverse = !!!m_bShowSpotLight;
			m_bSpotLightAnimating = TRUE;
			GetWnd()->WakeRenderThread();
		}
	}
	break;
	case Notify::SingleCtrl:
	{
		if (App->GetOpt().bLocateCursorWithCtrl)
		{
			ECK_DUILOCK;
			UpdateCursorPos();// 先更新一次，防止脏坐标

			m_bLocatingCursor = TRUE;
			m_msCursorLocateAni = 0.f;
			GetWnd()->WakeRenderThread();
		}
	}
	break;
	case Notify::GlobalMouseDown:
	{
		BOOL bWake{};
		// 聚光灯
		if (App->GetOpt().bSpotLight && m_bShowSpotLight)
		{
			ECK_DUILOCK;
			m_bShowSpotLight = FALSE;
			m_bSpotLightReverse = TRUE;
			m_bSpotLightAnimating = TRUE;
			bWake = TRUE;
		}
		// 点击显示
		if (App->GetOpt().bShowClick)
		{
			ClickState eState;
			switch (n.Vk)
			{
			case VK_LBUTTON: eState = ClickState::L; break;
			case VK_RBUTTON: eState = ClickState::R; break;
			case VK_MBUTTON: eState = ClickState::M; break;
			default: goto NoClick;
			}
			ECK_DUILOCK;
			UpdateCursorPos();// 先更新一次，防止脏坐标
			bWake = m_vClick.empty();
			m_vClick.emplace_back(NtGetTickCount64(), m_ptCursor, eState);
		}
	NoClick:;
		if (bWake)
			GetWnd()->WakeRenderThread();
	}
	break;
	case Notify::GlobalMouseUp:
	{
		// 点击显示
		if (App->GetOpt().bShowClick)
		{
			ClickState eState;
			switch (n.Vk)
			{
			case VK_LBUTTON: eState = ClickState::L; break;
			case VK_RBUTTON: eState = ClickState::R; break;
			case VK_MBUTTON: eState = ClickState::M; break;
			default: goto NoClick1;
			}
			ECK_DUILOCK;
			UpdateCursorPos();// 先更新一次，防止脏坐标
			const auto bWake = m_vClick.empty();
			constexpr ULONGLONG MinInterval = 300;
			const auto tCurr = NtGetTickCount64();
			const auto dx = eck::DaGetSystemMetrics(SM_CXDRAG,
				GetWnd()->GetUserDpiValue());
			const auto dy = eck::DaGetSystemMetrics(SM_CYDRAG,
				GetWnd()->GetUserDpiValue());

			for (const auto& e : m_vClick)
				if (e.eState == eState && !e.bUp)
				{
					// 仅当满足以下条件之一时才列队当前放开事件
					// 1. 按下与放开事件发生间隔大于MinInterval
					// 2. 按下与放开事件发生在不同位置
					const auto bTooFrequent = (tCurr - e.tAdd < MinInterval);
					const auto bTooNear = (fabs(e.ptCenter.x - m_ptCursor.x) <= dx) ||
						(fabs(e.ptCenter.y - m_ptCursor.y) <= dy);
					if (bTooFrequent && bTooNear)
						goto NoClick1;
				}
			m_vClick.emplace_back(NtGetTickCount64(), m_ptCursor, eState, TRUE);
			if (bWake)
				GetWnd()->WakeRenderThread();
		}
	NoClick1:;
	}
	break;
	case Notify::GlobalMouseMove:
	{
		ECK_DUILOCK;
		m_bCursorPosDirty = FALSE;
		auto pt{ n.pt };
		ScreenToClient(GetWnd()->HWnd, &pt);
		GetWnd()->Phy2Log(pt);
		ClientToElem(pt);
		const D2D1_POINT_2F ptNew{ (float)pt.x, (float)pt.y };

		BOOL bUpdate{};
		D2D1_RECT_F rcUpdate{};
		//===聚光灯
		if (m_bShowSpotLight && App->GetOpt().bSpotLight)
		{
			const auto r = m_fSpotLightMaxRadius * m_kSpotLight;
			eck::UnionRect(rcUpdate, rcUpdate, {
					m_ptCursor.x - r,m_ptCursor.y - r,
					m_ptCursor.x + r,m_ptCursor.y + r
				});
			eck::UnionRect(rcUpdate, rcUpdate, {
					ptNew.x - r,ptNew.y - r,
					ptNew.x + r,ptNew.y + r
				});
			bUpdate = TRUE;
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
			if (!App->GetOpt().bRainbowColor && App->GetOpt().bRuler)
			{
				RECT rc;
				eck::CeilRect(rcWndHili, rc);
				ExtendDirtyRect(rc);
				InvalidateRect(rc);
			}
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
				if (!IsValid())
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
		//===光标定位
		if (m_bLocatingCursor && App->GetOpt().bLocateCursorWithCtrl)
		{
			eck::UnionRect(rcUpdate, rcUpdate, {
					m_ptCursor.x - m_fCursorLocateRadius,m_ptCursor.y - m_fCursorLocateRadius,
					m_ptCursor.x + m_fCursorLocateRadius,m_ptCursor.y + m_fCursorLocateRadius
				});
			eck::UnionRect(rcUpdate, rcUpdate, {
					ptNew.x - m_fCursorLocateRadius,ptNew.y - m_fCursorLocateRadius,
					ptNew.x + m_fCursorLocateRadius,ptNew.y + m_fCursorLocateRadius
				});
			bUpdate = TRUE;
		}
		//===光标位置
		if (App->GetOpt().bShowCursorPos)
		{
			const auto r = App->GetOpt().fCursorPosRadius;
			eck::UnionRect(rcUpdate, rcUpdate, {
					m_ptCursor.x - r,m_ptCursor.y - r,
					m_ptCursor.x + r,m_ptCursor.y + r
				});
			eck::UnionRect(rcUpdate, rcUpdate, {
					ptNew.x - r,ptNew.y - r,
					ptNew.x + r,ptNew.y + r
				});
			bUpdate = TRUE;
		}
		m_ptCursor = ptNew;
		//===滚轮显示
		if (m_eWheel != Wheel::None && App->GetOpt().bShowWheel)
		{
			const auto r = (float)VeCxWheelIndicator;
			eck::UnionRect(rcUpdate, rcUpdate, {
					m_ptWheelIndicator.x,m_ptWheelIndicator.y,
					m_ptWheelIndicator.x + r,m_ptWheelIndicator.y + r
				});
			UpdateWheelIndicatorPos();
			eck::UnionRect(rcUpdate, rcUpdate, {
					m_ptWheelIndicator.x,m_ptWheelIndicator.y,
					m_ptWheelIndicator.x + r,m_ptWheelIndicator.y + r
				});
			bUpdate = TRUE;
		}
		if (bUpdate)
		{
			RECT rc;
			eck::CeilRect(rcUpdate, rc);
			ElemToClient(rc);
			ExtendDirtyRect(rc);
			InvalidateRect(rcUpdate);
		}
	}
	break;
	case Notify::GlobalMouseWheel:
	case Notify::GlobalMouseHWheel:
	{
		if (!App->GetOpt().bShowWheel)
			break;
		ECK_DUILOCK;
		UpdateCursorPos();
		Wheel eWheel;
		if (eNotify == Notify::GlobalMouseWheel)
			eWheel = (n.dWheel > 0 ? Wheel::Up : Wheel::Down);
		else
			eWheel = (n.dWheel > 0 ? Wheel::Left : Wheel::Right);
		if (!m_bWheelTimer)
		{
			SetTimer(IDT_WHEEL, TE_WHEEL);
			m_bWheelTimer = TRUE;
		}
		m_cWheelShowCount = 3;
		if (m_eWheel != eWheel)
		{
			m_eWheel = eWheel;
			UpdateWheelIndicatorPos();
			InvalidateWheelIndicator();
		}
	}
	break;
	case Notify::OptionsChanged:
	{
		ECK_DUILOCK;
		const auto Opt = App->GetOpt();
		const auto b = !!Opt.bCrosshair || !!Opt.bRuler ||
			!!Opt.bWndHilight;
		if ((m_bTimeLineActive && !b) ||
			m_bWatermarkEnabled != !!Opt.bWatermark)
			InvalidateRect();
		m_bTimeLineActive = b;
		m_bWatermarkEnabled = !!Opt.bWatermark;
	}
	break;
	case Notify::CursorSettingChanged:
	{
		ECK_DUILOCK;
		if (m_eWheel != Wheel::None)
		{
			UpdateWheelIndicatorPos();
			InvalidateWheelIndicator();
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

void CVeVisualContainer::UpdateCursorPos()
{
	if (!m_bCursorPosDirty)
		return;
	m_bCursorPosDirty = FALSE;
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(GetWnd()->HWnd, &pt);
	GetWnd()->Phy2Log(pt);
	ClientToElem(pt);
	m_ptCursor = { (float)pt.x, (float)pt.y };
}

void CVeVisualContainer::UpdateWheelIndicatorPos()
{
	const auto cxCursor = eck::DpiScaleF(
		(float)((CWndMain*)GetWnd())->GetCursorSize().cx,
		96,
		GetWnd()->GetUserDpiValue());
	const auto cyCursor = eck::DpiScaleF(
		(float)((CWndMain*)GetWnd())->GetCursorSize().cy,
		96,
		GetWnd()->GetUserDpiValue());
	if (m_ptCursor.y - cyCursor / 2.f < 0.f)
		m_ptWheelIndicator.y = m_ptCursor.y + cyCursor;
	else
		m_ptWheelIndicator.y = m_ptCursor.y - cyCursor / 2.f;
	if (m_ptCursor.x + cxCursor / 2.f > GetWnd()->GetClientWidthLog())
		m_ptWheelIndicator.x = m_ptCursor.x - cxCursor;
	else
		m_ptWheelIndicator.x = m_ptCursor.x + cxCursor / 2.f;
}

void CVeVisualContainer::InvalidateWheelIndicator()
{
	RECT rc
	{
		(int)m_ptWheelIndicator.x,
		(int)m_ptWheelIndicator.y,
		(int)m_ptWheelIndicator.x + VeCxWheelIndicator,
		(int)m_ptWheelIndicator.y + VeCxWheelIndicator
	};
	ElemToClient(rc);
	ExtendDirtyRect(rc);
	InvalidateRect(rc);
}

void CVeVisualContainer::InitSpotLight()
{
	ComPtr<ID2D1EllipseGeometry> pGeoCircle;
	constexpr D2D1_ELLIPSE Circle
	{
		{ SpotLightGeoRadius,SpotLightGeoRadius },
		SpotLightGeoRadius,SpotLightGeoRadius
	};
	eck::g_pD2dFactory->CreateEllipseGeometry(Circle, &pGeoCircle);
	float xDpi, yDpi;
	m_pDC->GetDpi(&xDpi, &yDpi);
	SafeRelease(m_pGrSpotLight);
	m_pDC1->CreateFilledGeometryRealization(pGeoCircle.Get(),
		D2D1::ComputeFlatteningTolerance(
			D2D1::Matrix3x2F::Identity(), xDpi, yDpi, 1.f),
		&m_pGrSpotLight);
}

void CVeVisualContainer::InitClick()
{
	constexpr D2D1_ELLIPSE Circle
	{
		{ ClickGeoRadius,ClickGeoRadius },
		ClickGeoRadius,ClickGeoRadius
	};
	eck::g_pD2dFactory->CreateEllipseGeometry(Circle, &m_pEllipseClick);

	ComPtr<ID2D1StrokeStyle1> pStrokeStyle;
	D2D1_STROKE_STYLE_PROPERTIES1 StrokeStyle{
		.transformType = D2D1_STROKE_TRANSFORM_TYPE_FIXED };
	eck::g_pD2dFactory->CreateStrokeStyle(StrokeStyle, nullptr, 0, &m_pStrokeStyleClick);
}

void CVeVisualContainer::InitCursorLocateGeoReal()
{
	ComPtr<ID2D1EllipseGeometry> pGeoCircle;
	constexpr D2D1_ELLIPSE Circle
	{
		{ CursorLocateGeoRadius,CursorLocateGeoRadius },
		CursorLocateGeoRadius,CursorLocateGeoRadius
	};
	eck::g_pD2dFactory->CreateEllipseGeometry(Circle, &pGeoCircle);
	float xDpi, yDpi;
	m_pDC->GetDpi(&xDpi, &yDpi);
	SafeRelease(m_pGrCursorLocate);
	m_pDC1->CreateFilledGeometryRealization(pGeoCircle.Get(),
		D2D1::ComputeFlatteningTolerance(
			D2D1::Matrix3x2F::Identity(), xDpi, yDpi, 1.f),
		&m_pGrCursorLocate);
}

void CVeVisualContainer::InitCursorLocate()
{
	InitCursorLocateGeoReal();
	ComPtr<ID2D1GradientStopCollection> pStop;
	constexpr D2D1_GRADIENT_STOP Stop[]
	{
		{ 0.f,{} },
		{ 0.8f,{} },
		{ 0.94f,{1.f,1.f,1.f,0.5f} }
	};
	m_pDC1->CreateGradientStopCollection(Stop, ARRAYSIZE(Stop),
		D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &pStop);
	constexpr D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES BrushProp
	{
		{ CursorLocateGeoRadius,CursorLocateGeoRadius },{},
		CursorLocateGeoRadius,CursorLocateGeoRadius
	};
	m_pDC->CreateRadialGradientBrush(BrushProp,
		pStop.Get(), &m_pBrCursorLocate);
}

void CVeVisualContainer::InitCursorPos()
{
	ComPtr<ID2D1EllipseGeometry> pGeoCircle;
	constexpr D2D1_ELLIPSE Circle
	{
		{ CursorGeoRadius,CursorGeoRadius },
		CursorGeoRadius,CursorGeoRadius
	};
	eck::g_pD2dFactory->CreateEllipseGeometry(Circle, &pGeoCircle);
	float xDpi, yDpi;
	m_pDC->GetDpi(&xDpi, &yDpi);
	SafeRelease(m_pGrCursorPos);
	m_pDC1->CreateFilledGeometryRealization(pGeoCircle.Get(),
		D2D1::ComputeFlatteningTolerance(
			D2D1::Matrix3x2F::Identity(), xDpi, yDpi, 1.f),
		&m_pGrCursorPos);
}

void CVeVisualContainer::InitWheel()
{
	ComPtr<ID2D1PathGeometry1> pPath;
	eck::g_pD2dFactory->CreatePathGeometry(&pPath);
	ComPtr<ID2D1GeometrySink> pSink;
	pPath->Open(&pSink);
	constexpr auto y = WheelGeoSide * 1.73205f / 2.f;
	pSink->BeginFigure({ WheelGeoSide / 2.f,0.f }, D2D1_FIGURE_BEGIN_FILLED);
	pSink->AddLine({ 0.f,y });
	pSink->AddLine({ WheelGeoSide,y });
	pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	pSink->Close();
	SafeRelease(m_pGrWheel);
	m_pDC1->CreateFilledGeometryRealization(pPath.Get(),
		D2D1::ComputeFlatteningTolerance(
			D2D1::Matrix3x2F::Identity(), 96.f, 96.f, 1.f),
		&m_pGrWheel);
	SafeRelease(m_pGrWheelBorder);
	float xDpi, yDpi;
	m_pDC->GetDpi(&xDpi, &yDpi);
	m_pDC1->CreateStrokedGeometryRealization(pPath.Get(),
		D2D1::ComputeFlatteningTolerance(
			D2D1::Matrix3x2F::Identity(), xDpi, yDpi, 1.f),
		WheelGeoSide / (float)VeCxWheelIndicator, nullptr, &m_pGrWheelBorder);
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
			auto cr{ App->GetColor(CApp::CrSpotLightBkg) };
			cr.a *= m_kSpotLight;
			m_pBrush->SetColor(cr);
			m_pDC->FillRectangle(ps.rcfClipInElem, m_pBrush);

			const auto eOldBlend = m_pDC->GetPrimitiveBlend();
			m_pDC->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
			m_pBrush->SetColor(D2D1_COLOR_F{});
			D2D1::Matrix3x2F MatOld;
			m_pDC->GetTransform(&MatOld);
			const auto fScale = std::max(
				App->GetOpt().fSpotLightRadius,
				m_fSpotLightMaxRadius * (1.f - m_kSpotLight)) / SpotLightGeoRadius;
			m_pDC->SetTransform(
				D2D1::Matrix3x2F::Translation(
					m_ptCursor.x - SpotLightGeoRadius,
					m_ptCursor.y - SpotLightGeoRadius) *
				D2D1::Matrix3x2F::Scale(
					fScale, fScale,
					{ m_ptCursor.x,m_ptCursor.y }) *
				MatOld);
			m_pDC1->DrawGeometryRealization(m_pGrSpotLight, m_pBrush);
			m_pDC->SetTransform(MatOld);
			m_pDC->SetPrimitiveBlend(eOldBlend);
		}
		//===准星线
		m_pBrush->SetColor(CalcRainbowColor(NtGetTickCount64()));
		if (App->GetOpt().bCrosshair)
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
		if (App->GetOpt().bWndHilight && m_hWndCursorAt)
		{
			m_pDC->DrawRectangle(m_rcWndHili, m_pBrush, (float)VeCxWndHiliBorder);
			if (App->GetOpt().bWndTip && m_TcWndTip.IsValid())
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
			m_pBrush->SetColor(App->GetColor(CApp::CrWatermark));
			m_pDC1->DrawGeometryRealization(m_TcWatermark.GetGeometryRealization(),
				m_pBrush);
		}
		//===光标位置
		if (App->GetOpt().bShowCursorPos)
		{
			D2D1::Matrix3x2F MatOld;
			m_pDC->GetTransform(&MatOld);
			const auto fScale =
				App->GetOpt().fCursorPosRadius / CursorGeoRadius;
			m_pDC->SetTransform(
				D2D1::Matrix3x2F::Translation(
					m_ptCursor.x - CursorGeoRadius,
					m_ptCursor.y - CursorGeoRadius) *
				D2D1::Matrix3x2F::Scale(
					fScale, fScale,
					{ m_ptCursor.x,m_ptCursor.y }) *
				MatOld);
			m_pBrush->SetColor(D2D1_COLOR_F{ 1.f,1.f,0.f,0.2f });
			m_pDC1->DrawGeometryRealization(m_pGrCursorPos, m_pBrush);
			m_pDC->SetTransform(MatOld);
		}
		//===点击显示
		if (App->GetOpt().bShowClick)
		{
			D2D1::Matrix3x2F MatOld;
			m_pDC->GetTransform(&MatOld);
			for (const auto& e : m_vClick)
			{
				switch (e.eState)
				{
				case ClickState::L:
					m_pBrush->SetColor(D2D1_COLOR_F{ 1.f,0.f,1.f,e.fAlpha });
					break;
				case ClickState::M:
					m_pBrush->SetColor(D2D1_COLOR_F{ 0.f,1.f,0.f,e.fAlpha });
					break;
				case ClickState::R:
					m_pBrush->SetColor(D2D1_COLOR_F{ 0.f,1.f,1.f,e.fAlpha });
					break;
				default: ECK_UNREACHABLE;
				}
				const auto fScale = e.fRadius / ClickGeoRadius;
				m_pDC->SetTransform(
					D2D1::Matrix3x2F::Translation(
						e.ptCenter.x - ClickGeoRadius,
						e.ptCenter.y - ClickGeoRadius) *
					D2D1::Matrix3x2F::Scale(
						fScale, fScale,
						{ e.ptCenter.x,e.ptCenter.y }) *
					MatOld);
				const D2D1_ELLIPSE Ellipse{
					{ e.ptCenter.x,e.ptCenter.y },
					e.fRadius,e.fRadius };
				m_pDC->DrawGeometry(m_pEllipseClick, m_pBrush,
					(float)VeCxClickStroke, m_pStrokeStyleClick);
			}
			m_pDC->SetTransform(MatOld);
		}
		//===光标定位
		if (m_bLocatingCursor && App->GetOpt().bLocateCursorWithCtrl)
		{
			D2D1::Matrix3x2F MatOld;
			m_pDC->GetTransform(&MatOld);
			const auto fScale = m_fCursorLocateRadius / CursorLocateGeoRadius;
			m_pDC->SetTransform(
				D2D1::Matrix3x2F::Translation(
					m_ptCursor.x - SpotLightGeoRadius,
					m_ptCursor.y - SpotLightGeoRadius) *
				D2D1::Matrix3x2F::Scale(
					fScale, fScale,
					{ m_ptCursor.x,m_ptCursor.y }) *
				MatOld);
			m_pBrCursorLocate->SetOpacity(m_fCursorLocateAlpha);
			m_pDC1->DrawGeometryRealization(m_pGrCursorLocate, m_pBrCursorLocate);
			m_pDC->SetTransform(MatOld);
		}
		//===滚轮显示
		if (m_eWheel != Wheel::None && App->GetOpt().bShowWheel)
		{
			D2D1::Matrix3x2F MatOld;
			m_pDC->GetTransform(&MatOld);
			const auto fAngle = (m_eWheel == Wheel::Up ? 0.f :
				m_eWheel == Wheel::Down ? 180.f :
				m_eWheel == Wheel::Left ? 90.f : -90.f);
			constexpr auto fScale = (float)VeCxWheelIndicator / WheelGeoSide;
			m_pDC->SetTransform(
				D2D1::Matrix3x2F::Rotation(
					fAngle, WheelGeoCenter) *
				D2D1::Matrix3x2F::Scale(
					fScale, fScale) *
				D2D1::Matrix3x2F::Translation(
					m_ptWheelIndicator.x,
					m_ptWheelIndicator.y) *
				MatOld);
			m_pBrush->SetColor(App->GetColor(CApp::CrWheelIndicator));
			m_pDC1->DrawGeometryRealization(m_pGrWheel, m_pBrush);
			m_pBrush->SetColor(App->GetColor(CApp::CrWheelIndicatorBorder));
			m_pDC1->DrawGeometryRealization(m_pGrWheelBorder, m_pBrush);
			m_pDC->SetTransform(MatOld);
		}
		EndPaint(ps);
	}
	return 0;

	case WM_TIMER:
	{
		if (wParam == IDT_WHEEL)
		{
			ECK_DUILOCK;
			if (!m_cWheelShowCount || m_eWheel == Wheel::None)
			{
				KillTimer(IDT_WHEEL);
				m_bWheelTimer = FALSE;
				return 0;
			}
			if (!--m_cWheelShowCount)
			{
				KillTimer(IDT_WHEEL);
				m_bWheelTimer = FALSE;
				m_eWheel = Wheel::None;
				InvalidateWheelIndicator();
			}
		}
	}
	return 0;

	case WM_SIZE:
	{
		m_fSpotLightMaxRadius = std::max(GetWidthF(), GetHeightF()) / 2.f;
		RECT rcKs2;
		rcKs2.left = 0;
		rcKs2.right = GetWidth();
		rcKs2.top = GetHeight() - VeCyKeyStroke2 - 10;
		rcKs2.bottom = rcKs2.top + VeCyKeyStroke2;

		m_KeyStroke2.SetRect(rcKs2);
	}
	break;

	case WM_DPICHANGED:
	{
		InitSpotLight();
		InitCursorLocateGeoReal();
		InitCursorPos();
		InitWheel();
	}
	break;

	case WM_CREATE:
	{
		m_pDC->QueryInterface(&m_pDC1);
		m_pDC->CreateSolidColorBrush({}, &m_pBrush);

		InitSpotLight();
		InitClick();
		InitCursorLocate();
		InitCursorPos();
		InitWheel();

		m_hSlot = App->GetSignal().Connect(this, &CVeVisualContainer::OnAppEvent);
		const auto pTfKeyStroke = App->CreateTextFormat(16);

		const auto pTfWatermark = App->CreateTextFormat(42);
		const auto cchWatermark = (UINT32)App->GetOpt().rsWatermark.Size();
		m_TcWatermark.CreateTextLayout(App->GetOpt().rsWatermark.Data(),
			cchWatermark, pTfWatermark, GetWidthF(), GetHeightF());
		const auto pTlWatermark = m_TcWatermark.GetTextLayout();
		IDWriteTextLayout1* pTlWatermark1;
		pTlWatermark->QueryInterface(&pTlWatermark1);
		pTlWatermark1->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTlWatermark1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pTlWatermark1->Release();
		m_TcWatermark.CreateGeometryRealization(m_pDC1);
		pTfWatermark->Release();

		m_KeyStroke.Create(nullptr, Dui::DES_VISIBLE, 0,
			VeCxKeyStrokeMargin, VeCyKeyStrokeMargin,
			VeCxKeyStroke, VeCyKeyStroke, this);
		m_KeyStroke.SetTextFormat(pTfKeyStroke);

		m_KeyStroke2.Create(nullptr, Dui::DES_VISIBLE, 0,
			0, 0, 0, 0, this);
		m_KeyStroke2.SetTextFormat(pTfKeyStroke);

		pTfKeyStroke->Release();
		GetWnd()->RegisterTimeLine(this);
	}
	break;
	case WM_DESTROY:
	{
		SafeRelease(m_pBrush);
		SafeRelease(m_pGrSpotLight);
		SafeRelease(m_pGrCursorLocate);
		SafeRelease(m_pGrCursorPos);
		SafeRelease(m_pStrokeStyleClick);
		SafeRelease(m_pEllipseClick);
		SafeRelease(m_pBrCursorLocate);
		SafeRelease(m_pGrWheel);
		SafeRelease(m_pGrWheelBorder);
		SafeRelease(m_pDC1);
		m_TcWndTip.Invalidate();
		m_TcRulerCursorTip.Invalidate();
		m_TcWatermark.Invalidate();
		App->GetSignal().Disconnect(m_hSlot);
		GetWnd()->UnregisterTimeLine(this);
	}
	break;
	}
	return __super::OnEvent(uMsg, wParam, lParam);
}

void STDMETHODCALLTYPE CVeVisualContainer::Tick(int iMs)
{
	DWRITE_TEXT_METRICS tm;
	D2D1_RECT_F rcInvalid{};
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
		eck::UnionRect(rcInvalid, rcInvalid, GetViewRectF());
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
		eck::UnionRect(rcInvalid, rcInvalid, m_rcWndHili);
		m_rcWndHili.left = (m_rcWndHiliSrc.left * (1.f - k)) + (m_rcWndHiliDst.left * k);
		m_rcWndHili.top = (m_rcWndHiliSrc.top * (1.f - k)) + (m_rcWndHiliDst.top * k);
		m_rcWndHili.right = (m_rcWndHiliSrc.right * (1.f - k)) + (m_rcWndHiliDst.right * k);
		m_rcWndHili.bottom = (m_rcWndHiliSrc.bottom * (1.f - k)) + (m_rcWndHiliDst.bottom * k);
		eck::UnionRect(rcInvalid, rcInvalid, m_rcWndHili);
		if (m_TcWndTip.GetTextLayout())
		{
			m_TcWndTip.GetTextLayout()->GetMetrics(&tm);
			eck::UnionRect(rcInvalid, rcInvalid, {
					m_ptWndTip.x,m_ptWndTip.y,
					m_ptWndTip.x + tm.width,
					m_ptWndTip.y + tm.height });
		}
		m_ptWndTip.x = (m_ptWndTipSrc.x * (1.f - k)) + (m_ptWndTipDst.x * k);
		m_ptWndTip.y = (m_ptWndTipSrc.y * (1.f - k)) + (m_ptWndTipDst.y * k);
		if (m_TcWndTip.GetTextLayout())
			eck::UnionRect(rcInvalid, rcInvalid, {
				m_ptWndTip.x,m_ptWndTip.y,
				m_ptWndTip.x + tm.width,
				m_ptWndTip.y + tm.height });
	}
	else if (App->GetOpt().bWndHilight)
	{
		eck::UnionRect(rcInvalid, rcInvalid, m_rcWndHili);
		if (m_TcWndTip.GetTextLayout())
		{
			m_TcWndTip.GetTextLayout()->GetMetrics(&tm);
			eck::UnionRect(rcInvalid, rcInvalid, {
					m_ptWndTip.x,m_ptWndTip.y,
					m_ptWndTip.x + tm.width,
					m_ptWndTip.y + tm.height });
		}
	}
	if (!m_vClick.empty())
		for (size_t i{ m_vClick.size() }; i; --i)
		{
			auto& e = m_vClick[i - 1];
			e.ms += (float)iMs;
			const auto k = eck::Easing::OutCubic(e.ms, 0.f, 1.f, 500.f);
			if (k >= 1.f)
			{
				const auto r = e.fRadius + (float)VeCxClickStroke;
				eck::UnionRect(rcInvalid, rcInvalid, {
					e.ptCenter.x - r, e.ptCenter.y - r,
					e.ptCenter.x + r, e.ptCenter.y + r });
				m_vClick.erase(m_vClick.begin() + i - 1);
				continue;
			}
			e.fRadius = ((e.bUp ? App->GetOpt().fClickRadiusUp :
				App->GetOpt().fClickRadius) * k);
			e.fAlpha = 1.f - k;
			const auto r = e.fRadius + (float)VeCxClickStroke;
			eck::UnionRect(rcInvalid, rcInvalid, {
				e.ptCenter.x - r, e.ptCenter.y - r,
				e.ptCenter.x + r, e.ptCenter.y + r });
		}
	if (m_bLocatingCursor)
	{
		m_msCursorLocateAni += (float)iMs;
		auto k = eck::Easing::OutCubic(m_msCursorLocateAni, 0.f, 1.f, 700.f);
		if (k >= 1.f)
		{
			m_bLocatingCursor = FALSE;
			k = 1.f;
		}
		eck::UnionRect(rcInvalid, rcInvalid, {
			m_ptCursor.x - m_fCursorLocateRadius, m_ptCursor.y - m_fCursorLocateRadius,
			m_ptCursor.x + m_fCursorLocateRadius, m_ptCursor.y + m_fCursorLocateRadius });
		m_fCursorLocateRadius = App->GetOpt().fLocateCursorRadius * (1.f - k);
		eck::UnionRect(rcInvalid, rcInvalid, {
			m_ptCursor.x - m_fCursorLocateRadius, m_ptCursor.y - m_fCursorLocateRadius,
			m_ptCursor.x + m_fCursorLocateRadius, m_ptCursor.y + m_fCursorLocateRadius });
		m_fCursorLocateAlpha = 1.f - k;
	}
	RECT rc;
	eck::CeilRect(rcInvalid, rc);
	ElemToClient(rc);
	ExtendDirtyRect(rc);
	InvalidateRect(rc);
}