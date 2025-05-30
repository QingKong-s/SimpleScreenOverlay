﻿#include "pch.h"
#include "CVeKeyStroke.h"
#include "CWndMain.h"


CVeKeyStroke::Key CVeKeyStroke::VkToKey(UINT vk)
{
	switch (vk)
	{
	case 'W': return Key::W;
	case 'A': return Key::A;
	case 'S': return Key::S;
	case 'D': return Key::D;
	case VK_SPACE: return Key::Space;
	case VK_SHIFT: return Key::Shift;
	case VK_LBUTTON: return Key::MLeft;
	case VK_RBUTTON: return Key::MRight;
	}
	return Key::Max;
}

UINT CVeKeyStroke::KeyToVk(Key eKey)
{
	switch (eKey)
	{
	case Key::W: return 'W';
	case Key::A: return 'A';
	case Key::S: return 'S';
	case Key::D: return 'D';
	case Key::Space: return VK_SPACE;
	case Key::Shift: return VK_SHIFT;
	case Key::MLeft: return VK_LBUTTON;
	case Key::MRight: return VK_RBUTTON;
	}
	return 0;
}

void CVeKeyStroke::OnAppEvent(Notify eNotify, SSONOTIFY& n)
{
	Key idx;
	switch (eNotify)
	{
	case Notify::GlobalKeyDown:
	{
		if (!App->GetOpt().bKeyStroke)
			break;
		idx = VkToKey(n.Vk);
		if (idx == Key::Max)
			return;
		ECK_DUILOCK;
		m_bKeyDown[(size_t)idx] = TRUE;
		InvalidateRect();
	}
	break;
	case Notify::GlobalKeyUp:
	{
		if (!App->GetOpt().bKeyStroke)
			break;
		idx = VkToKey(n.Vk);
		if (idx == Key::Max)
			return;
		ECK_DUILOCK;
		m_bKeyDown[(size_t)idx] = FALSE;
		InvalidateRect();
	}
	break;
	case Notify::GlobalMouseDown:
	{
		if (!App->GetOpt().bKeyStroke)
			break;
		if (n.Vk == VK_LBUTTON)
			idx = Key::MLeft;
		else if (n.Vk == VK_RBUTTON)
			idx = Key::MRight;
		else
			break;
		ECK_DUILOCK;
		m_bKeyDown[(size_t)idx] = TRUE;
		InvalidateRect();
	}
	break;
	case Notify::GlobalMouseUp:
	{
		if (!App->GetOpt().bKeyStroke)
			break;
		if (n.Vk == VK_LBUTTON)
			idx = Key::MLeft;
		else if (n.Vk == VK_RBUTTON)
			idx = Key::MRight;
		else
			break;
		ECK_DUILOCK;
		m_bKeyDown[(size_t)idx] = FALSE;
		InvalidateRect();
	}
	break;
	case Notify::GlobalMouseMove:
	{
		if (!App->GetOpt().bKeyStroke)
			break;
		POINT pt{ n.pt };
		ScreenToClient(GetWnd()->HWnd, &pt);
		GetWnd()->Phy2Log(pt);
		if (eck::PtInRect(GetRectInClient(), pt))
			if (m_bLeft)
			{
				m_bLeft = FALSE;
				SetPos(
					GetParentElem()->GetWidth() - GetWidth() - VeCxKeyStrokeMargin,
					GetRect().top);
			}
			else
				if (!m_bLeft)
				{
					m_bLeft = TRUE;
					SetPos(VeCxKeyStrokeMargin, GetRect().top);
				}
	}
	break;
	case Notify::OptionsChanged:
	{
		ECK_DUILOCK;
		SetVisible(App->GetOpt().bKeyStroke);
		if (!App->GetOpt().bKeyStroke)
			memset(m_bKeyDown, 0, sizeof(m_bKeyDown));
		if (App->GetOpt().bRainbowColor)
			GetWnd()->WakeRenderThread();
	}
	break;
	}
}

void CVeKeyStroke::PaintUnit(const D2D1_RECT_F& rc, float cxLine, Key eKey)
{
	if (m_bKeyDown[(size_t)eKey])
		m_pBrush->SetColor(App->GetColor(CApp::CrKeyStrokeBkgPressed));
	else
		m_pBrush->SetColor(App->GetColor(CApp::CrKeyStrokeBkg));
	m_pDC->FillRectangle(rc, m_pBrush);
	m_pDC->DrawTextLayout({ rc.left,rc.top }, m_pTl[(size_t)eKey],
		m_pBrushForegnd, Dui::DrawTextLayoutFlags);
	m_pDC->DrawRectangle(rc, m_pBrushForegnd, cxLine);
}

LRESULT CVeKeyStroke::OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		if (!m_pTl[0])
			return 0;

		Dui::ELEMPAINTSTRU ps;
		BeginPaint(ps, wParam, lParam);

		if (App->GetOpt().bRainbowColor)
			m_pBrushForegnd->SetColor(CalcRainbowColor(NtGetTickCount64()));
		else
			m_pBrushForegnd->SetColor(App->GetColor(CApp::CrKeyStroke));

		constexpr auto Padding = (float)VeCxyKeyStrokePadding;
		constexpr auto Padding2 = (float)VeCyKeyStrokeMouseBtnPadding;
		const auto cxElem = GetWidthF();
		constexpr auto d = (float)VeCxKeyStrokeBorder;

		D2D1_RECT_F rc;
		rc.left = m_cxyBlock + Padding;
		rc.top = d;
		rc.right = rc.left + m_cxyBlock;
		rc.bottom = rc.top + m_cxyBlock;
		PaintUnit(rc, d, Key::W);

		rc.top = rc.bottom + Padding;
		rc.bottom = rc.top + m_cxyBlock;
		PaintUnit(rc, d, Key::S);

		rc.left = d;
		rc.right = rc.left + m_cxyBlock;
		PaintUnit(rc, d, Key::A);

		rc.left = d + (m_cxyBlock + Padding) * 2.f;
		rc.right = rc.left + m_cxyBlock;
		PaintUnit(rc, d, Key::D);

		rc.left = d;
		rc.right = cxElem - d;
		rc.top = rc.bottom + Padding;
		rc.bottom = rc.top + m_cySpaceBtn;
		PaintUnit(rc, d, Key::Space);

		rc.top = rc.bottom + Padding;
		rc.bottom = rc.top + m_cySpaceBtn;
		PaintUnit(rc, d, Key::Shift);

		rc.left = d;
		rc.right = rc.left + cxElem / 2.f - Padding;
		rc.top = rc.bottom + Padding2;
		rc.bottom = rc.top + m_cyMouseBtn;
		PaintUnit(rc, d, Key::MLeft);

		rc.left = rc.right + Padding;
		rc.right = cxElem - d;
		PaintUnit(rc, d, Key::MRight);

		ECK_DUI_DBG_DRAW_FRAME;
		EndPaint(ps);
	}
	return 0;
	case WM_SIZE:
	{
		m_cxyBlock = (GetWidthF() -
			float(VeCxyKeyStrokePadding + VeCxKeyStrokeBorder) * 2.f) / 3.f;
		m_cySpaceBtn = m_cxyBlock * 3.f / 4.f;
		m_cyMouseBtn = m_cxyBlock * 5.f / 6.f;
	}
	break;
	case WM_SETFONT:
	{
		IDWriteTextLayout* pTlTemp{};
		DWRITE_TEXT_METRICS tm;
		constexpr static std::wstring_view Text[]
		{
			L"W"sv,
			L"A"sv,
			L"S"sv,
			L"D"sv,
			L"Space"sv,
			L"Shift"sv,
			L"LMB"sv,
			L"RMB"sv,
		};
		const auto pTf = GetTextFormat();

		eck::g_pDwFactory->CreateTextLayout(EckStrAndLen(L"bq啊"), pTf,
			m_cxyBlock, m_cxyBlock, &pTlTemp);
		pTlTemp->GetMetrics(&tm);

		float cxMax{ m_cxyBlock }, cyMax{ m_cxyBlock };
		EckCounter((size_t)Key::Max, i)
		{
			if (i == (size_t)Key::Space)
			{
				cxMax = GetWidthF();
				cyMax = m_cySpaceBtn;
			}
			else if (i == (size_t)Key::MLeft)
			{
				cxMax = GetWidthF() / 2.f - (float)VeCxyKeyStrokePadding;
				cyMax = m_cyMouseBtn;
			}

			eck::g_pDwFactory->CreateTextLayout(Text[i].data(),
				(UINT32)Text[i].size(), pTf, cxMax, cyMax, m_pTl + i);
			m_pTl[i]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			m_pTl[i]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}
	}
	break;
	case WM_CREATE:
	{
		m_pDC->CreateSolidColorBrush(App->GetColor(CApp::CrKeyStrokeBkg), &m_pBrush);
		m_pDC->CreateSolidColorBrush({}, &m_pBrushForegnd);
		m_hSlot = App->GetSignal().Connect(this, &CVeKeyStroke::OnAppEvent);
		GetWnd()->RegisterTimeLine(this);
	}
	break;
	case WM_DESTROY:
	{
		App->GetSignal().Disconnect(m_hSlot);
		SafeRelease(m_pBrush);
		SafeRelease(m_pBrushForegnd);
		GetWnd()->UnregisterTimeLine(this);
	}
	break;
	}
	return __super::OnEvent(uMsg, wParam, lParam);
}