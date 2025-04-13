#include "pch.h"
#include "CWndMain.h"


constexpr std::wstring_view KeyName[]
{
	LR"()"sv,
	LR"(鼠标左键)"sv,
	LR"(鼠标右键)"sv,
	LR"(控制中断)"sv,
	LR"(鼠标中键)"sv,
	LR"(鼠标X1)"sv,
	LR"(鼠标X2)"sv,
	LR"(未定义)"sv,
	LR"(Backspace)"sv,
	LR"(Tab)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(Clear)"sv,
	LR"(Enter)"sv,
	LR"(未定义)"sv,
	LR"(未定义)"sv,
	LR"(Shift)"sv,
	LR"(Ctrl)"sv,
	LR"(Alt)"sv,
	LR"(Pause)"sv,
	LR"(Caps Lock)"sv,
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
	LR"(空格)"sv,
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
	LR"(未定义)"sv,
	LR"(未定义)"sv,
	LR"(未定义)"sv,
	LR"(未定义)"sv,
	LR"(未定义)"sv,
	LR"(未定义)"sv,
	LR"(未定义)"sv,
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
	LR"(Win(左))"sv,
	LR"(Win(右))"sv,
	LR"(Apps)"sv,
	LR"(保留)"sv,
	LR"(Sleep)"sv,
	LR"(NUM 0)"sv,
	LR"(NUM 1)"sv,
	LR"(NUM 2)"sv,
	LR"(NUM 3)"sv,
	LR"(NUM 4)"sv,
	LR"(NUM 5)"sv,
	LR"(NUM 6)"sv,
	LR"(NUM 7)"sv,
	LR"(NUM 8)"sv,
	LR"(NUM 9)"sv,
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
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(Num Lock)"sv,
	LR"(Scroll Lock)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(Shift(左))"sv,
	LR"(Shift(右))"sv,
	LR"(Ctrl(左))"sv,
	LR"(Ctrl(右))"sv,
	LR"(Alt(左))"sv,
	LR"(Alt(右))"sv,
	LR"(浏览器后退)"sv,
	LR"(浏览器前进)"sv,
	LR"(浏览器刷新)"sv,
	LR"(浏览器停止)"sv,
	LR"(浏览器搜索)"sv,
	LR"(浏览器收藏)"sv,
	LR"(浏览器开始/主页)"sv,
	LR"(静音)"sv,
	LR"(音量减)"sv,
	LR"(音量加)"sv,
	LR"(下一曲)"sv,
	LR"(上一曲)"sv,
	LR"(停止播放)"sv,
	LR"(播放/暂停)"sv,
	LR"(启动邮件)"sv,
	LR"(选择媒体)"sv,
	LR"(启动程序1)"sv,
	LR"(启动程序2)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(; :)"sv,
	LR"(= +)"sv,
	LR"(, <)"sv,
	LR"(- _)"sv,
	LR"(. >)"sv,
	LR"(/ ?)"sv,
	LR"(` ~)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(保留)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"(未分配)"sv,
	LR"([ {)"sv,
	LR"(\ |)"sv,
	LR"(] })"sv,
	LR"(' ")"sv,
	LR"(其他键盘字符)"sv,
	LR"(保留)"sv,
	LR"(OEM特定)"sv,
	LR"(<> /|)"sv,
	LR"(OEM特定)"sv,
	LR"(OEM特定)"sv,
	LR"(IME Process)"sv,
	LR"(OEM 特定)"sv,
	LR"(内部)"sv,
	LR"(未分配)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(OEM 特定)"sv,
	LR"(Attn)"sv,
	LR"(CrSel)"sv,
	LR"(ExSel)"sv,
	LR"(ErEOF)"sv,
	LR"(播放)"sv,
	LR"(缩放)"sv,
	LR"(保留)"sv,
	LR"(PA1)"sv,
	LR"(清除)"sv,
};

constexpr float FadeInOutAnDuration = 6000.f;

void CVeKeyStroke2::OnAppEvent(Notify eNotify, SSONOTIFY& n)
{
	switch (eNotify)
	{
	case Notify::GlobalKeyDown:
	{
		ECK_DUILOCK;
		IkOnKeyDown(n.Vk);
	}
	break;
	case Notify::GlobalKeyUp:
	{
		ECK_DUILOCK;
		IkOnKeyUp(n.Vk);
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
		m_vItem.emplace_back(Vk, StayMillSecPreHit, ItemState::FadeIn);
		bNewItem = TRUE;
		return m_vItem.end() - 1;
	}
	else if (it->Vk != Vk)// 新元素插入到中间
	{
		const auto idx = std::distance(m_vItem.begin(), it);
		m_vItem.emplace(it, Vk, StayMillSecPreHit, ItemState::FadeIn);
		bNewItem = TRUE;
		return m_vItem.begin() + idx;
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
		IkBeginRePos();
	}
	else if (it->eState == ItemState::FadeOut)
	{
		it->eState = ItemState::FadeIn;
		it->xSrc = it->x;
		it->ySrc = it->y;
		it->msTime = 0;
		IkBeginRePos();
	}
}

void CVeKeyStroke2::IkOnKeyUp(UINT Vk)
{
	ITEM e{ Vk };
	const auto it = std::lower_bound(m_vItem.begin(), m_vItem.end(), e);
	if (it == m_vItem.end())
		return;
	it->uFlags &= ~KIF_KEYDOWN;
}

void CVeKeyStroke2::IkBeginRePos()
{
	for (auto& e : m_vItem)
	{
		if (e.eState == ItemState::None ||
			e.eState == ItemState::RePos)
		{
			e.eState = ItemState::RePos;
			e.xSrc = e.x;
			e.ySrc = e.y;
			e.msTime = 0;
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

		m_pBrushForegnd->SetColor(((CWndMain*)GetWnd())->GetCurrAnColor());
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
		App->GetSignal().Connect(this, &CVeKeyStroke2::OnAppEvent);
		GetWnd()->RegisterTimeLine(this);
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
	BOOL bDelayDelete{};
	for (auto& e : m_vItem)
	{
		if (e.eState == ItemState::FadeIn || e.eState == ItemState::RePos)
		{
			e.msTime += iMs;
			const auto k = eck::Easing::OutCubic(e.msTime, 0.f, 1.f, FadeInOutAnDuration);
			if (e.eState == ItemState::FadeIn)
				e.fOpacity = k;
			if (k >= 1.f)
			{
				e.eState = ItemState::None;
				e.x = xFinal;
				e.y = yFinal;
			}
			else
			{
				e.x = e.xSrc + (xFinal - e.xSrc) * k;
				e.y = e.ySrc + (yFinal - e.ySrc) * k;
			}
		}
		else if (e.eState == ItemState::FadeOut)
		{
			float xDst, yDst;
			CalcCenterBottomPos(xDst, yDst);
			e.msTime += iMs;
			const auto k = eck::Easing::OutCubic(e.msTime, 0.f, 1.f, FadeInOutAnDuration);
			e.fOpacity = 1.f - k;
			if (k >= 1.f)
			{
				e.eState = ItemState::Deleted;
				bDelayDelete = TRUE;
			}
			else
			{
				e.x = e.xSrc + (xDst - e.xSrc) * k;
				e.y = e.ySrc + (yDst - e.ySrc) * k;
			}
		}
		if (e.eState != ItemState::FadeIn &&
			e.eState != ItemState::FadeOut &&
			e.eState != ItemState::Deleted)
		{
			if ((e.msRemain -= iMs) <= 0)
			{
				e.xSrc = e.x;
				e.ySrc = e.y;
				e.eState = ItemState::FadeOut;
				e.msTime = 0;
			}
		}
		xFinal += (m_cxyBlock + (float)VeCxyKeyStroke2Padding);
	}
	if (bDelayDelete)
	{
		for (size_t i = m_vItem.size(); i; --i)
			if (m_vItem[i - 1].eState == ItemState::Deleted)
				m_vItem.erase(m_vItem.begin() + i - 1);
		IkBeginRePos();
	}
}

void CVeKeyStroke2::CalcKeyItemNormalPos(size_t idx, _Out_ float& x, _Out_ float& y)
{
	EckAssert(idx < m_vItem.size());
	x = (GetWidthF() - m_cxyBlock * m_vItem.size() -
		(float)VeCxyKeyStroke2Padding * (m_vItem.size() - 1)) / 2.f +
		(m_cxyBlock + (float)VeCxyKeyStroke2Padding) * idx;
	y = (float)VeCxKeyStrokeBorder;
}

void CVeKeyStroke2::PaintUnit(const D2D1_RECT_F& rc, float cxLine, ITEM& e)
{
	BOOL bDown;
	if (App->GetOpt().bImmdiateMode)
		bDown = (GetAsyncKeyState(e.Vk) & 0x8000);
	else
		bDown = (e.uFlags & KIF_KEYDOWN);

	D2D1_COLOR_F crBkg, crForegnd;
	if (bDown)
		crBkg = App->GetColor(CApp::CrKeyStrokeBkgPressed);
	else
		crBkg = App->GetColor(CApp::CrKeyStrokeBkg);
	if (e.eState == ItemState::FadeIn || e.eState == ItemState::FadeOut)
	{
		crBkg.a *= e.fOpacity;
		crForegnd = m_pBrushForegnd->GetColor();
		const auto OldA = crForegnd.a;
		crForegnd.a *= e.fOpacity;
		m_pBrushForegnd->SetColor(crForegnd);
		crForegnd.a = OldA;
	}
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
		m_pDC->DrawTextLayout({ rc.left,rc.top }, e.pTextLayout,
			m_pBrushForegnd, Dui::DrawTextLayoutFlags);
	m_pDC->DrawRectangle(rc, m_pBrushForegnd, cxLine);

	if (e.eState == ItemState::FadeIn || e.eState == ItemState::FadeOut)
		m_pBrushForegnd->SetColor(crForegnd);
}

void CVeKeyStroke2::CalcCenterBottomPos(_Out_ float& x, _Out_ float& y)
{
	x = (GetWidthF() - m_cxyBlock) / 2.f;
	y = GetHeightF() - m_cxyBlock - (float)VeCxKeyStrokeBorder;
}