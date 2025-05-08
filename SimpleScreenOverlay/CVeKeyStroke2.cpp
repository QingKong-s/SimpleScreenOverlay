#include "pch.h"
#include "CWndMain.h"


constexpr std::wstring_view KeyName[]
{
	LR"()"sv,
	LR"(LMB)"sv,
	LR"(RMB)"sv,
	LR"()"sv,
	LR"(MMB)"sv,
	LR"(X1)"sv,
	LR"(X2)"sv,
	LR"()"sv,
	LR"(Back)"sv,
	LR"(Tab)"sv,
	LR"()"sv,
	LR"()"sv,
	LR"(Clear)"sv,
	LR"(Enter)"sv,
	LR"()"sv,
	LR"()"sv,
	LR"(Shift)"sv,
	LR"(Ctrl)"sv,
	LR"(Alt)"sv,
	LR"(Pause)"sv,
	LR"(CapsLk)"sv,
	LR"(IME 假名/谚文)"sv,
	LR"(IME 打开)"sv,
	LR"(IME Junja)"sv,
	LR"(IME 完成)"sv,
	LR"(IME 汉字)"sv,
	LR"(IME 关闭)"sv,
	LR"(Esc)"sv,
	LR"(IME 转换)"sv,
	LR"(IME 不转换)"sv,
	LR"(IME 接受)"sv,
	LR"(IME 模式更改)"sv,
	LR"(--)"sv,
	LR"(Page Up)"sv,
	LR"(Page Down)"sv,
	LR"(End)"sv,
	LR"(Home)"sv,
	LR"(←)"sv,
	LR"(↑)"sv,
	LR"(→)"sv,
	LR"(↓)"sv,
	LR"(Select)"sv,
	LR"(Print)"sv,
	LR"(Execute)"sv,
	LR"(PrtSrc)"sv,
	LR"(Ins)"sv,
	LR"(Del)"sv,
	LR"(Help)"sv,
	LR"(0)"sv,
	LR"(1)"sv,
	LR"(2)"sv,
	LR"(3)"sv,
	LR"(4)"sv,
	LR"(5)"sv,
	LR"(6)"sv,
	LR"(7)"sv,
	LR"(8)"sv,
	LR"(9)"sv,
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	LR"(A)"sv,
	LR"(B)"sv,
	LR"(C)"sv,
	LR"(D)"sv,
	LR"(E)"sv,
	LR"(F)"sv,
	LR"(G)"sv,
	LR"(H)"sv,
	LR"(I)"sv,
	LR"(J)"sv,
	LR"(K)"sv,
	LR"(L)"sv,
	LR"(M)"sv,
	LR"(N)"sv,
	LR"(O)"sv,
	LR"(P)"sv,
	LR"(Q)"sv,
	LR"(R)"sv,
	LR"(S)"sv,
	LR"(T)"sv,
	LR"(U)"sv,
	LR"(V)"sv,
	LR"(W)"sv,
	LR"(X)"sv,
	LR"(Y)"sv,
	LR"(Z)"sv,
	LR"(LWin)"sv,
	LR"(RWin)"sv,
	LR"(Apps)"sv,
	{},
	LR"(Sleep)"sv,
	LR"(N0)"sv,
	LR"(N1)"sv,
	LR"(N2)"sv,
	LR"(N3)"sv,
	LR"(N4)"sv,
	LR"(N5)"sv,
	LR"(N6)"sv,
	LR"(N7)"sv,
	LR"(N8)"sv,
	LR"(N9)"sv,
	LR"(*)"sv,
	LR"(+)"sv,
	LR"(Separator)"sv,
	LR"(-)"sv,
	LR"(.)"sv,
	LR"(/)"sv,
	LR"(F1)"sv,
	LR"(F2)"sv,
	LR"(F3)"sv,
	LR"(F4)"sv,
	LR"(F5)"sv,
	LR"(F6)"sv,
	LR"(F7)"sv,
	LR"(F8)"sv,
	LR"(F9)"sv,
	LR"(F10)"sv,
	LR"(F11)"sv,
	LR"(F12)"sv,
	LR"(F13)"sv,
	LR"(F14)"sv,
	LR"(F15)"sv,
	LR"(F16)"sv,
	LR"(F17)"sv,
	LR"(F18)"sv,
	LR"(F19)"sv,
	LR"(F20)"sv,
	LR"(F21)"sv,
	LR"(F22)"sv,
	LR"(F23)"sv,
	LR"(F24)"sv,
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	LR"(NumLk)"sv,
	LR"(ScrLk)"sv,
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	LR"(LShift)"sv,
	LR"(RShift)"sv,
	LR"(LCtrl)"sv,
	LR"(RCtrl)"sv,
	LR"(LAlt)"sv,
	LR"(RAlt)"sv,
	LR"(BrsBack)"sv,
	LR"(BrsFwd)"sv,
	LR"(BrsRe)"sv,
	LR"(BrsStop)"sv,
	LR"(BrsSch)"sv,
	LR"(BrsFav)"sv,
	LR"(BrsHome)"sv,
	LR"(MdSlt)"sv,
	LR"(Vol-)"sv,
	LR"(Vol+)"sv,
	LR"(MdNxt)"sv,
	LR"(MdPrv)"sv,
	LR"(MdStop)"sv,
	LR"(MdPP)"sv,
	LR"(EMail)"sv,
	LR"(SelMd)"sv,
	LR"(Run1)"sv,
	LR"(Run2)"sv,
	{},
	{},
	LR"(;)"sv,// :
	LR"(=)"sv,// +
	LR"(,)"sv,// <
	LR"(-)"sv,// _
	LR"(.)"sv,// >
	LR"(/)"sv,// ?
	LR"(`)"sv,// ~
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	LR"([)"sv,// {
	LR"(\)"sv,// |
	LR"(])"sv,// }
	LR"(')"sv,// "
	LR"(其他键盘字符)"sv,
	{},
	{},
	LR"(<> /|)"sv,
	{},
	{},
	LR"(IME Process)"sv,
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	LR"(Attn)"sv,
	LR"(CrSel)"sv,
	LR"(ExSel)"sv,
	LR"(ErEOF)"sv,
	LR"(Play)"sv,
	LR"(Scale)"sv,
	{},
	LR"(PA1)"sv,
	LR"(Clear)"sv,
};

constexpr float FadeInOutAnDuration = 2000.f;
constexpr float RePosAnDuration = 1000.f;
constexpr float JumpAnDuration = 300.f;

void CVeKeyStroke2::OnAppEvent(Notify eNotify, SSONOTIFY& n)
{
	switch (eNotify)
	{
	case Notify::GlobalKeyDown:
	case Notify::GlobalMouseDown:
	{
		if (!App->GetOpt().bKeyStroke2)
			break;
		ECK_DUILOCK;
		IkOnKeyDown(n.Vk);
	}
	break;
	case Notify::GlobalKeyUp:
	case Notify::GlobalMouseUp:
	{
		if (!App->GetOpt().bKeyStroke2)
			break;
		ECK_DUILOCK;
		IkOnKeyUp(n.Vk);
	}
	break;
	case Notify::GlobalMouseMove:
	{
		if (!App->GetOpt().bKeyStroke2)
			break;
		ECK_DUILOCK;
		auto pt{ n.pt };
		ScreenToClient(GetWnd()->HWnd, &pt);
		GetWnd()->Phy2Log(pt);
		ClientToElem(pt);
		IkOnMouseMove(pt);
	}
	break;
	case Notify::OptionsChanged:
	{
		ECK_DUILOCK;
		m_bRainbow = App->GetOpt().bRainbowColor;
		SetVisible(App->GetOpt().bKeyStroke2);
		if (App->GetOpt().bKeyStroke2)
			SetTimer(IDT_KEYSTROKE2, TE_KEYSTROKE2);
		else
		{
			m_vItem.clear();
			KillTimer(IDT_KEYSTROKE2);
		}
		if (m_bRainbow)
			GetWnd()->WakeRenderThread();
	}
	break;
	}
}

std::vector<CVeKeyStroke2::ITEM>::iterator CVeKeyStroke2::IkpInsert(
	UINT Vk, _Out_ BOOL& bNewItem)
{
	ITEM e{ Vk };
	const auto it = std::lower_bound(m_vItem.begin(), m_vItem.end(), e);
	if (it == m_vItem.end())// 新元素是最大的
	{
		m_vItem.emplace_back(Vk, StayMillSecInit, ItemState::FadeIn);
		bNewItem = TRUE;
		return m_vItem.end() - 1;
	}
	else if (it->Vk != Vk)// 新元素插入到中间
	{
		const auto itNew = m_vItem.emplace(it, Vk, StayMillSecInit, ItemState::FadeIn);
		bNewItem = TRUE;
		return itNew;
	}
	else// 新元素和已有元素相同
	{
		it->msRemain += StayMillSecPreHit;
		bNewItem = FALSE;
		return it;
	}
}

void CVeKeyStroke2::IkOnKeyDown(UINT Vk)
{
	BOOL bNewItem;
	const auto it = IkpInsert(Vk, bNewItem);
	it->uFlags |= KIF_KEYDOWN;
	if (bNewItem)// 准备动画
	{
		CalcCenterBottomPos(it->xSrc, it->ySrc);
		it->x = it->xSrc;
		it->y = it->ySrc;
		IkpBeginRePos();
		ActivateTimeLine();
	}
	else
	{
		InvalidateRect();
	}
}

void CVeKeyStroke2::IkOnKeyUp(UINT Vk)
{
	ITEM e{ Vk };
	const auto it = std::lower_bound(m_vItem.begin(), m_vItem.end(), e);
	if (it == m_vItem.end())
		return;
	it->uFlags &= ~KIF_KEYDOWN;
	InvalidateRect();
}

void CVeKeyStroke2::IkpBeginRePos()
{
	for (auto& e : m_vItem)
	{
		if (e.eState == ItemState::None ||
			e.eState == ItemState::FadeIn ||
			e.eState == ItemState::RePos ||
			e.eState == ItemState::Jump ||
			e.eState == ItemState::Restore)
		{
			if (e.eState == ItemState::None)
				e.eState = ItemState::RePos;
			e.xSrc = e.x;
			e.ySrc = e.y;
			e.msTime = 0;
		}
	}
}

void CVeKeyStroke2::IkOnMouseMove(POINT pt_)
{
	D2D1_POINT_2F pt{ eck::MakeD2dPtF(pt_) };
	D2D1_RECT_F rc;
	BOOL bAn{};
	for (size_t i{}; auto& e : m_vItem)
	{
		if (e.eState == ItemState::None ||
			e.eState == ItemState::Restore)
		{
			rc.left = e.x;
			rc.top = e.y;
			rc.right = rc.left + m_cxyBlock;
			rc.bottom = rc.top + m_cxyBlock;
			if (eck::PtInRect(rc, pt))
			{
				bAn = TRUE;
				IkpBeginJump(e);
			}
		}
		else if (e.eState == ItemState::Jump ||
			e.eState == ItemState::Jumped)
		{
			CalcKeyItemNormalPos(i, rc.left, rc.top);
			rc.right = rc.left + m_cxyBlock;
			rc.bottom = rc.top + m_cxyBlock;
			if (!eck::PtInRect(rc, pt))
			{
				bAn = TRUE;
				IkpCancelJump(e);
			}
		}
		++i;
	}
	if (bAn)
		ActivateTimeLine();
}

void CVeKeyStroke2::IkpBeginJump(ITEM& e)
{
	e.eState = ItemState::Jump;
	e.xSrc = e.x;
	e.ySrc = e.y;
	e.msTime = 0;
}

void CVeKeyStroke2::IkpCancelJump(ITEM& e)
{
	e.eState = ItemState::Restore;
	e.xSrc = e.x;
	e.ySrc = e.y;
	e.msTime = 0;
}

void CVeKeyStroke2::IkpTickKey()
{
	ECK_DUILOCK;
	BOOL bDel{};
	const auto ullTick = NtGetTickCount64();
	for (size_t i = m_vItem.size(); i; --i)
	{
		auto& e = m_vItem[i - 1];
		if (e.eState != ItemState::FadeIn &&
			!(e.uFlags & KIF_KEYDOWN))
		{
			e.msRemain -= TE_KEYSTROKE2;
			if (e.msRemain <= 0)
			{
				bDel = TRUE;
				m_vItem.erase(m_vItem.begin() + i - 1);
			}
		}
	}
	if (bDel)
	{
		if (m_vItem.empty())
			InvalidateRect();
		else
		{
			IkpBeginRePos();
			ActivateTimeLine();
		}
	}
}

LRESULT CVeKeyStroke2::OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		Dui::ELEMPAINTSTRU ps;
		BeginPaint(ps, wParam, lParam);

		D2D1_RECT_F rcBlock;
		for (auto& e : m_vItem)
		{
			rcBlock.left = e.x;
			rcBlock.top = e.y;
			rcBlock.right = rcBlock.left + m_cxyBlock;
			rcBlock.bottom = rcBlock.top + m_cxyBlock;
			PaintUnit(rcBlock, (float)VeCxKeyStrokeBorder, e);
		}
		ECK_DUI_DBG_DRAW_FRAME;
		EndPaint(ps);
	}
	break;

	case WM_TIMER:
	{
		if (wParam == IDT_KEYSTROKE2)
			IkpTickKey();
	}
	return 0;

	case WM_SIZE:
	{
		ECK_DUILOCK;
		m_cxyBlock = GetHeightF() - float(VeCxKeyStrokeBorder * 2);

		m_cxyBlock = 40;
	}
	break;

	case WM_CREATE:
	{
		m_pDC->CreateSolidColorBrush({}, &m_pBrush);
		m_pDC->CreateSolidColorBrush({}, &m_pBrushForegnd);
		m_hSlot = App->GetSignal().Connect(this, &CVeKeyStroke2::OnAppEvent);
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

void STDMETHODCALLTYPE CVeKeyStroke2::Tick(int iMs)
{
	if (m_vItem.empty())
		return;
	float xFinal, yFinal;
	CalcKeyItemNormalPos(0, xFinal, yFinal);
	BOOL bActive{};
	EckCounter(m_vItem.size(), i)
	{
		auto& e = m_vItem[i];
		if (e.eState == ItemState::FadeIn || e.eState == ItemState::RePos ||
			e.eState == ItemState::Restore)
		{
			if (e.eState == ItemState::RePos)
				EckDbgPrint("RePos");
			e.msTime += iMs;
			const auto k = eck::Easing::OutCubic(e.msTime, 0.f, 1.f,
				(e.eState == ItemState::FadeIn) ? FadeInOutAnDuration :
				(e.eState == ItemState::RePos) ? RePosAnDuration : JumpAnDuration);
			if (e.eState == ItemState::FadeIn)
			{
				const auto k1 = std::clamp(k, 0.f, 1.f);
				if (k1 > e.fOpacity)
					e.fOpacity = k1;
			}
			if (k >= 1.f)
			{
				e.msTime = 0;
				e.eState = ItemState::None;
				e.x = xFinal;
				e.y = yFinal;
			}
			else
			{
				e.x = e.xSrc + (xFinal - e.xSrc) * k;
				e.y = e.ySrc + (yFinal - e.ySrc) * k;
				bActive = TRUE;
			}
		}
		else if (e.eState == ItemState::Jump)
		{
			const float xDst = xFinal, yDst = 0.f;
			e.msTime += iMs;
			const auto k = eck::Easing::OutCubic(e.msTime, 0.f, 1.f, JumpAnDuration);
			if (k >= 1.f)
			{
				e.eState = ItemState::Jumped;
				e.x = xDst;
				e.y = yDst;
			}
			else
			{
				e.x = e.xSrc + (xDst - e.xSrc) * k;
				e.y = e.ySrc + (yDst - e.ySrc) * k;
				bActive = TRUE;
			}
		}
		xFinal += (m_cxyBlock + (float)VeCxyKeyStroke2Padding);
	}
	InvalidateRect();
	m_bAnimating = bActive;
}

void CVeKeyStroke2::CalcKeyItemNormalPos(size_t idx, _Out_ float& x, _Out_ float& y)
{
	EckAssert(idx < m_vItem.size());
	x = (GetWidthF() - m_cxyBlock * m_vItem.size() -
		(float)VeCxyKeyStroke2Padding * (m_vItem.size() - 1)) / 2.f +
		(m_cxyBlock + (float)VeCxyKeyStroke2Padding) * idx;
	y = (GetHeightF() - m_cxyBlock) * 2.f / 5.f - m_cxyBlock / 2.f;
}

void CVeKeyStroke2::PaintUnit(const D2D1_RECT_F& rc, float cxLine, ITEM& e)
{
	D2D1_COLOR_F crBkg, crForegnd;
	if (e.uFlags & KIF_KEYDOWN)
		crBkg = App->GetColor(CApp::CrKeyStrokeBkgPressed);
	else
		crBkg = App->GetColor(CApp::CrKeyStrokeBkg);
	if (m_bRainbow)
		crForegnd = CalcRainbowColorWithStep(
			NtGetTickCount64(), int(&e - &m_vItem[0]) * 8);
	else
		crForegnd = App->GetColor(CApp::CrDefKeyStroke);
	if (e.eState == ItemState::FadeIn)
	{
		crBkg.a *= e.fOpacity;
		crForegnd.a *= e.fOpacity;
	}
	m_pBrushForegnd->SetColor(crForegnd);
	m_pBrush->SetColor(crBkg);
	m_pDC->FillRectangle(rc, m_pBrush);

	if (!e.pTextLayout)
	{
		eck::g_pDwFactory->CreateTextLayout(KeyName[e.Vk].data(),
			(UINT32)KeyName[e.Vk].size(), GetTextFormat(),
			m_cxyBlock, m_cxyBlock, &e.pTextLayout);
		if (e.pTextLayout)
		{
			e.pTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			e.pTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}
	}
	if (e.pTextLayout)
	{
		D2D1::Matrix3x2F MatOld;
		if (KeyName[e.Vk].size() == 1)
		{
			m_pDC->GetTransform(&MatOld);
			constexpr float fScale = 1.8f;
			const D2D1_POINT_2F pt
			{
				(rc.left + rc.right) / 2.f,
				(rc.top + rc.bottom) / 2.f
			};
			m_pDC->SetTransform(D2D1::Matrix3x2F::Scale(
				fScale, fScale, pt) * MatOld);
		}
		m_pDC->DrawTextLayout({ rc.left,rc.top }, e.pTextLayout,
			m_pBrushForegnd, Dui::DrawTextLayoutFlags);
		if (KeyName[e.Vk].size() == 1)
			m_pDC->SetTransform(MatOld);
	}
	m_pDC->DrawRectangle(rc, m_pBrushForegnd, cxLine);
}

void CVeKeyStroke2::CalcCenterBottomPos(_Out_ float& x, _Out_ float& y)
{
	x = (GetWidthF() - m_cxyBlock) / 2.f;
	y = GetHeightF() - m_cxyBlock - (float)VeCxKeyStrokeBorder;
}