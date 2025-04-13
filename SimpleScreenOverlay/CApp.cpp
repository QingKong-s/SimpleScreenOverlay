#include "pch.h"

CApp* App{};

#if SSO_USE_LOWLEVEL_HOOK
LRESULT __stdcall CApp::LowLevelKeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		const auto* const p = (KBDLLHOOKSTRUCT*)lParam;
		SSONOTIFY n;
		n.Vk = p->vkCode;
		App->GetSignal().Emit(
			(wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ?
			Notify::GlobalKeyDown : Notify::GlobalKeyUp, n);
		if (wParam == WM_KEYDOWN
			&& (p->vkCode == VK_CONTROL
				|| p->vkCode == VK_LCONTROL
				|| p->vkCode == VK_RCONTROL))
		{
			const auto d = GetDoubleClickTime();
			if (p->time - App->m_dwLastCtrlTime < d)
				App->GetSignal().Emit(Notify::DoubleCtrl, n);
			App->m_dwLastCtrlTime = p->time;
		}
	}
	return CallNextHookEx(App->m_hhkKeyboard, nCode, wParam, lParam);
}

LRESULT __stdcall CApp::LowLevelMouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		Notify eNotify;
		SSONOTIFY n{};
		if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN)
		{
			n.bLBtn = (wParam == WM_LBUTTONDOWN || wParam == WM_LBUTTONUP);
			eNotify = Notify::GlobalMouseDown;
		}
		else if (wParam == WM_LBUTTONUP || wParam == WM_RBUTTONUP)
		{
			n.bLBtn = (wParam == WM_LBUTTONDOWN || wParam == WM_LBUTTONUP);
			eNotify = Notify::GlobalMouseUp;
		}
		else if (wParam == WM_MOUSEMOVE)
		{
			n.pt = ((MSLLHOOKSTRUCT*)lParam)->pt;
			eNotify = Notify::GlobalMouseMove;
		}
		else
			goto Skip;
		App->GetSignal().Emit(eNotify, n);
	}
Skip:
	return CallNextHookEx(App->m_hhkKeyboard, nCode, wParam, lParam);
}
#else
LRESULT __stdcall CApp::KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	SSONOTIFY n;
	n.Vk = (UINT)wParam;
	const auto eNotify = (lParam & (1u << 31)) ?
		Notify::GlobalKeyDown : Notify::GlobalKeyUp;
	App->GetSignal().Emit(eNotify, n);
	if (eNotify == Notify::GlobalKeyDown && wParam == VK_CONTROL)
	{
		const auto d = GetDoubleClickTime();
		const auto uTick = NtGetTickCount();
		if (uTick - App->m_dwLastCtrlTime < d)
			App->GetSignal().Emit(Notify::DoubleCtrl, n);
		App->m_dwLastCtrlTime = uTick;
	}
	return CallNextHookEx(App->m_hhkKeyboard, nCode, wParam, lParam);
}

LRESULT __stdcall CApp::MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	Notify eNotify;
	SSONOTIFY n{};
	if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN)
	{
		n.bLBtn = (wParam == WM_LBUTTONDOWN || wParam == WM_LBUTTONUP);
		eNotify = Notify::GlobalMouseDown;
	}
	else if (wParam == WM_LBUTTONUP || wParam == WM_RBUTTONUP)
	{
		n.bLBtn = (wParam == WM_LBUTTONDOWN || wParam == WM_LBUTTONUP);
		eNotify = Notify::GlobalMouseUp;
	}
	else if (wParam == WM_MOUSEMOVE)
	{
		n.pt = ((MOUSEHOOKSTRUCT*)lParam)->pt;
		eNotify = Notify::GlobalMouseMove;
	}
	else
		goto Skip;
	App->GetSignal().Emit(eNotify, n);
Skip:
	return CallNextHookEx(App->m_hhkMouse, nCode, wParam, lParam);
}
#endif// SSO_USE_LOWLEVEL_HOOK


void CApp::Init()
{
	m_pTfCommon = eck::CreateDefTextFormatWithSize(14);
#if SSO_USE_LOWLEVEL_HOOK
	m_hhkKeyboard = SetWindowsHookExW(WH_KEYBOARD_LL,
		LowLevelKeyboardHookProc, nullptr, 0);
	m_hhkMouse = SetWindowsHookExW(WH_MOUSE_LL,
		LowLevelMouseHookProc, nullptr, 0);
#else
	m_hhkKeyboard = SetWindowsHookExW(WH_KEYBOARD,
		KeyboardHookProc, nullptr, NtCurrentThreadId32());
	m_hhkMouse = SetWindowsHookExW(WH_MOUSE,
		MouseHookProc, nullptr, NtCurrentThreadId32());
#endif// SSO_USE_LOWLEVEL_HOOK

	m_Opt.bImmdiateMode = 1;
	m_Opt.bCrosshair = 0;
	m_Opt.cxCrosshairLine = 1.f;
	m_Opt.dCrosshairCursorGap = 20.f;
	m_Opt.bKeyStroke = 1;
	m_Opt.bKeyStroke2 = 1;
	m_Opt.bSpotLight = 1;
	m_Opt.fSpotLightRadius = 40.f;
	m_Opt.bWndHilight = 1;
	m_Opt.bWndHilightDetectChild = 1;
	m_Opt.bRuler = 0;
}

IDWriteTextFormat* CApp::CreateTextFormat(float fSize)
{
	return eck::CreateDefTextFormatWithSize(fSize);
}
