#pragma once

enum class Notify
{
	GlobalKeyDown,		// Vk
	GlobalKeyUp,		// Vk
	GlobalMouseDown,	// bLBtn
	GlobalMouseUp,		// bLBtn
	DoubleCtrl,			// 
	GlobalMouseMove,	// pt
};

struct SSONOTIFY
{
	union
	{
		UINT Vk;
		BOOL bLBtn;
		POINT pt;
	};
};

#ifdef _DEBUG
#define SSO_USE_LOWLEVEL_HOOK 0
#else
#define SSO_USE_LOWLEVEL_HOOK 1
#endif

class CApp
{
public:
	enum
	{
		CrText,

		CrDefFuncMenuTitle,
		CrDefFuncMenuBkg,
		CrDefFuncMenuHot,
		CrDefFuncMenuSelected,
		CrDefFuncMenuHotSelected,

		CrCrosshair,

		CrKeyStrokeBkg,
		CrDefKeyStroke,
		CrKeyStrokeBkgPressed,

		CrMax
	};

	constexpr static D2D1_COLOR_F ColorLight[CrMax]
	{
		eck::ColorrefToD2dColorF(eck::Colorref::White),

		eck::ColorrefToD2dColorF(RGB(72, 88,104), 0.5f),
		eck::ColorrefToD2dColorF(RGB(18, 19, 20), 0.6f),
		eck::ColorrefToD2dColorF(RGB(38, 39, 40), 0.7f),
		eck::ColorrefToD2dColorF(RGB(10, 11, 12), 0.6f),
		eck::ColorrefToD2dColorF(RGB(25, 26, 27), 0.7f),

		eck::ColorrefToD2dColorF(RGB(30, 10, 20), 0.5f),

		{ 0.f,0.f,0.f,0.5f },
		eck::ColorrefToD2dColorF(RGB(18, 19, 20), 0.6f),
		eck::ColorrefToD2dColorF(RGB(120, 88,104), 0.5f),

	};

	constexpr static D2D1_COLOR_F ColorDark[CrMax]
	{
		eck::ColorrefToD2dColorF(eck::Colorref::Black),
		eck::ColorrefToD2dColorF(RGB(72, 88,104)),
		eck::ColorrefToD2dColorF(RGB(18, 19, 20)),
		eck::ColorrefToD2dColorF(RGB(100, 110, 130)),
	};
private:
	struct OPT
	{
		BOOLEAN bImmdiateMode;
		BOOLEAN bCrosshair;
		BOOLEAN bKeyStroke;
		BOOLEAN bKeyStroke2;
		BOOLEAN bSpotLight;
		BOOLEAN bWndHilight;
		BOOLEAN bWndHilightDetectChild;
		BOOLEAN bRuler;

		float cxCrosshairLine;
		float dCrosshairCursorGap;
		float fSpotLightRadius;
	};


	BOOL m_bDarkMode{};
	IDWriteTextFormat* m_pTfCommon{};
	OPT m_Opt{};

	eck::CSignal<eck::NoIntercept_T, void, Notify, SSONOTIFY&> m_Sig{};

	HHOOK m_hhkKeyboard{};
	HHOOK m_hhkMouse{};

	DWORD m_dwLastCtrlTime{};

#if SSO_USE_LOWLEVEL_HOOK
	static LRESULT __stdcall LowLevelKeyboardHookProc(
		int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT __stdcall LowLevelMouseHookProc(
		int nCode, WPARAM wParam, LPARAM lParam);
#else
	static LRESULT __stdcall KeyboardHookProc(
		int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT __stdcall MouseHookProc(
		int nCode, WPARAM wParam, LPARAM lParam);
#endif// SSO_USE_LOWLEVEL_HOOK
public:
	void Init();

	EckInlineNdCe const auto& GetColor(int idx)
	{
		if (m_bDarkMode)
			return ColorDark[idx];
		else
			return ColorLight[idx];
	}

	EckInlineNdCe auto GetTextFormatCommon() const { return m_pTfCommon; }

	EckInlineNdCe auto& GetOpt() { return m_Opt; }

	IDWriteTextFormat* CreateTextFormat(float fSize);

	EckInlineNdCe auto& GetSignal() { return m_Sig; }
};

extern CApp* App;