#include "pch.h"
#include "CVeMenuList.h"
#include "CWndMain.h"

void CVeMenuList::PaintItem(int idx, const D2D1_RECT_F& rcItem, const D2D1_RECT_F& rcPaint)
{
	//__super::PaintItem(idx, rcItem, rcPaint);
	//return;
	const float Padding = GetTheme()->GetMetrics(Dui::Metrics::SmallPadding);
	const float Padding2 = GetTheme()->GetMetrics(Dui::Metrics::LargePadding);

	D2D1_RECT_F rcFill;
	eck::IntersectRect(rcFill, rcItem, rcPaint);
	if (IsItemSel(idx))
		if (App->GetOpt().bImmdiateMode)
			m_pBrush->SetColor(((CWndMain*)GetWnd())->GetCurrAnColor());
		else
			m_pBrush->SetColor(App->GetColor(CApp::CrDefFuncMenuHotSelected));
	else
		if (m_idxHot == idx)
			m_pBrush->SetColor(App->GetColor(CApp::CrDefFuncMenuHot));
		else
			goto SkipFill;
	m_pDC->FillRectangle(rcFill, m_pBrush);
SkipFill:
	auto& e = m_vItem[idx];

	if (!e.rsText.IsEmpty() && !e.pTextLayout)
	{
		const auto cx = rcItem.right - rcItem.left;
		eck::g_pDwFactory->CreateTextLayout(e.rsText.Data(), e.rsText.Size(),
			GetTextFormat(), cx, (float)m_cyItem, &e.pTextLayout);
		e.pTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		e.pTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		e.pTextLayout->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	}

	if (e.pTextLayout)
	{
		if (App->GetOpt().bImmdiateMode)
		{
			auto cr{ ((CWndMain*)GetWnd())->GetCurrAnColor() };
			cr.r = 1.f - cr.r;
			cr.g = 1.f - cr.g;
			cr.b = 1.f - cr.b;
			m_pBrush->SetColor(cr);
		}
		else
			m_pBrush->SetColor(App->GetColor(CApp::CrText));
		m_pDC->DrawTextLayout({ 0,rcItem.top }, e.pTextLayout,
			m_pBrush, Dui::DrawTextLayoutFlags);
	}
}

LRESULT CVeMenuList::OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NOTIFY:
		if (wParam == (WPARAM)&m_SB)
			if (((Dui::DUINMHDR*)lParam)->uCode == Dui::EE_VSCROLL)
			{
				InvalidateRect();
				return TRUE;
			}
		break;
	case WM_CREATE:
	{
		m_pDC->CreateSolidColorBrush({}, &m_pBrush);
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

LRESULT CVeMenuList::OnNotify(Dui::DUINMHDR* pnm, BOOL& bProcessed)
{
	return 0;
}