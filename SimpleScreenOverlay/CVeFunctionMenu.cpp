#include "pch.h"
#include "CVeFunctionMenu.h"

LRESULT CVeFunctionMenu::OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		Dui::ELEMPAINTSTRU ps;
		BeginPaint(ps, wParam, lParam);

		D2D1_RECT_F rc{ GetViewRectF() };
		m_pBrush->SetColor(App->GetColor(CApp::CrDefFuncMenuTitle));
		m_pDC->FillRectangle(rc, m_pBrush);

		rc.left += (float)VeCxFuncMenuBorder;
		rc.top += (float)VeCyFuncMenuTitle;
		rc.right -= (float)VeCxFuncMenuBorder;
		rc.bottom -= float(VeCyFuncMenuFooter / 2);

		m_pBrush->SetColor(App->GetColor(CApp::CrDefFuncMenuBkg));
		const auto eOldBlend = m_pDC->GetPrimitiveBlend();
		m_pDC->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
		m_pDC->FillRectangle(rc, m_pBrush);
		m_pDC->SetPrimitiveBlend(eOldBlend);

		m_pBrush->SetColor(App->GetColor(CApp::CrText));
		if (!m_pTextLayout && !GetText().IsEmpty())
		{
			eck::g_pDwFactory->CreateTextLayout(GetText().Data(), GetText().Size(),
				GetTextFormat(), GetWidthF(), (float)VeCyFuncMenuTitle, &m_pTextLayout);
			m_pTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			m_pTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}
		if (m_pTextLayout)
		m_pDC->DrawTextLayout({ 0,0 }, m_pTextLayout, m_pBrush,
			Dui::DrawTextLayoutFlags);

		EndPaint(ps);
	}
	return 0;

	case WM_SIZE:
	{
		RECT rc;
		rc.left = VeCxFuncMenuBorder;
		rc.top = VeCyFuncMenuTitle;
		rc.right = GetWidth() - VeCxFuncMenuBorder;
		rc.bottom = GetHeight() - VeCyFuncMenuFooter;
		m_LV.SetRect(rc);
	}
	break;

	case WM_NOTIFY:
	{
		const auto pnm = (Dui::DUINMHDR*)lParam;
		if (pnm->uCode == UIE_MENU_GETDISPINFO &&
			(WPARAM)&m_LV == wParam)
			return GenElemNotify(pnm);
	}
	break;

	case WM_MOUSEMOVE:
	{
		if (m_bLBtnDown)
		{
			const POINT pt ECK_GET_PT_LPARAM(lParam);
			SetPos(
				m_ptDragOffset.x + pt.x - m_ptDragStart.x,
				m_ptDragOffset.y + pt.y - m_ptDragStart.y
			);
		}
	}
	return 0;
	case WM_LBUTTONDOWN:
	{
		m_ptDragStart = ECK_GET_PT_LPARAM(lParam);
		m_ptDragOffset = { GetRect().left, GetRect().top };
		m_bLBtnDown = TRUE;
		SetCapture();
	}
	return 0;
	case WM_LBUTTONUP:
	{
		if (m_bLBtnDown)
		{
			ReleaseCapture();
			m_bLBtnDown = FALSE;
		}
	}
	return 0;

	case WM_SETTEXT:
	{
		ECK_DUILOCK;
		SafeRelease(m_pTextLayout);
	}
	break;

	case WM_SETFONT:
	{
		ECK_DUILOCK;
		SafeRelease(m_pTextLayout);
	}
	break;
	case WM_CREATE:
	{
		m_pDC->CreateSolidColorBrush({}, &m_pBrush);
		m_LV.Create(nullptr, Dui::DES_VISIBLE, 0,
			0, 0, 10,10, this);
		m_LV.SetItemHeight(VeCyFuncMenuItem);
		m_LV.SetItemPadding(VeCyFuncMenuItemPadding);

		m_LV.SetTextFormat(App->GetTextFormatCommon());
		SetTextFormat(App->GetTextFormatCommon());
	}
	break;
	case WM_DESTROY:
	{
		SafeRelease(m_pBrush);
	}
	break;
	}
	return __super::OnEvent(uMsg, wParam, lParam);
}

void CVeFunctionMenu::ReCalcIdealSize()
{
	auto cyList = m_LV.GetItemCount() *
		(VeCyFuncMenuItem + VeCyFuncMenuItemPadding);
	cyList += (VeCyFuncMenuItemPadding);
	m_LV.SetSize(GetWidth(), cyList);
	SetSize(GetWidth(), VeCyFuncMenuTitle + cyList + VeCyFuncMenuFooter);
}
