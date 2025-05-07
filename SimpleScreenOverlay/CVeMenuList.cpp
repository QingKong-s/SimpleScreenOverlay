#include "pch.h"
#include "CVeMenuList.h"
#include "CWndMain.h"

void CVeMenuList::LVPaintItem(int idx, int idxGroup, const D2D1_RECT_F& rcPaint)
{
	const float Padding = GetTheme()->GetMetrics(Dui::Metrics::SmallPadding);
	const float Padding2 = GetTheme()->GetMetrics(Dui::Metrics::LargePadding);

	D2D1_RECT_F rcFill, rcItem;
	GetItemRect(idx, rcItem);
	eck::IntersectRect(rcFill, rcItem, rcPaint);
	if (GetItemState(idx) & Dui::LEIF_SELECTED)
		if (App->GetOpt().bRainbowColor)
			m_pBrush->SetColor(CalcRainbowColor(NtGetTickCount64()));
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

	ML_DISPINFO di{ UIE_MENU_GETDISPINFO };
	di.idx = idx;
	GenElemNotify(&di);

	if (di.cchText && !e.pLayout.Get())
	{
		const auto cx = rcItem.right - rcItem.left;
		eck::g_pDwFactory->CreateTextLayout(di.pszText, di.cchText,
			GetTextFormat(), cx, (float)m_cyItem, &e.pLayout);
		e.pLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		e.pLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		e.pLayout->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	}

	if (e.pLayout.Get())
	{
		if (App->GetOpt().bRainbowColor)
		{
			auto cr{ CalcRainbowColor(NtGetTickCount64()) };
			cr.r = 1.f - cr.r;
			cr.g = 1.f - cr.g;
			cr.b = 1.f - cr.b;
			m_pBrush->SetColor(cr);
		}
		else
			m_pBrush->SetColor(App->GetColor(CApp::CrText));
		m_pDC->DrawTextLayout({ 0,rcItem.top }, e.pLayout.Get(),
			m_pBrush, Dui::DrawTextLayoutFlags);
	}
}