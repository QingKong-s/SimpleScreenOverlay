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

		D2D1_ROUNDED_RECT rrc;
		rrc.rect = GetRectInClientF();
		ClientToElem(rrc.rect);
		rrc.radiusX = rrc.radiusY = 16;
		m_pBrush->SetColor(App->GetColor(CApp::CrDefFuncMenuTitle));
		m_pDC->FillRoundedRectangle(rrc, m_pBrush);

		rrc.rect.left += (float)VeCxFuncMenuBorder;
		rrc.rect.top += (float)VeCyFuncMenuTitle;
		rrc.rect.right -= (float)VeCxFuncMenuBorder;
		rrc.rect.bottom -= float(VeCyFuncMenuFooter / 2);
		rrc.radiusX = rrc.radiusY = 10;
		rrc.rect.top += rrc.radiusY;
		m_pDC->PushAxisAlignedClip(rrc.rect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		rrc.rect.top -= rrc.radiusY;
		m_pBrush->SetColor(App->GetColor(CApp::CrDefFuncMenuBkg));
		m_pDC->FillRoundedRectangle(rrc, m_pBrush);
		m_pDC->PopAxisAlignedClip();

		rrc.rect.bottom = rrc.rect.top + rrc.radiusY;
		m_pDC->FillRectangle(rrc.rect, m_pBrush);

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

void CVeFunctionMenu::CommitItemChange()
{
	auto cyList = int(m_LV.GetItems().size()) *
		(VeCyFuncMenuItem + VeCyFuncMenuItemPadding);
	cyList += (VeCyFuncMenuItemPadding * 2);
	m_LV.SetSize(GetWidth(), cyList);
	SetSize(GetWidth(), VeCyFuncMenuTitle + cyList + VeCyFuncMenuFooter);
	m_LV.SetItemCount(int(m_LV.GetItems().size()));
}
