#pragma once

enum class Notify
{
	GlobalKeyDown,		// Vk
	GlobalKeyUp,		// Vk
	GlobalMouseDown,	// Vk
	GlobalMouseUp,		// Vk
	DoubleCtrl,			// 
	SingleCtrl,			//
	GlobalMouseMove,	// pt
	GlobalMouseWheel,	// dWheel
	GlobalMouseHWheel,	// dWheel
	OptionsChanged,		//
	CursorSettingChanged,//
};

struct SSONOTIFY
{
	union
	{
		struct
		{
			UINT Vk;
			BOOL bRepeat;
		};
		POINT pt;
		int dWheel;
	};
};

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

		CrKeyStroke,
		CrKeyStrokeBkg,
		CrKeyStrokeBkgPressed,

		CrWatermark,

		CrMenuTip,
		CrMenuTipBkg,

		CrSpotLightBkg,

		CrWheelIndicator,
		CrWheelIndicatorBorder,

		CrLine,

		CrMax
	};

	constexpr static D2D1_COLOR_F ColorLight[CrMax]
	{
		eck::ColorrefToD2dColorF(eck::Colorref::White),

		eck::ColorrefToD2dColorF(RGB(3, 201, 255), 0.7f),
		eck::ColorrefToD2dColorF(RGB(18, 19, 20), 0.6f),
		eck::ColorrefToD2dColorF(RGB(67, 68, 69), 0.65f),
		eck::ColorrefToD2dColorF(RGB(20, 21, 22), 0.6f),

		{ 1.f,1.f,1.f,0.6f },
		eck::ColorrefToD2dColorF(RGB(20, 20, 20), 0.3f),
		eck::ColorrefToD2dColorF(RGB(20, 150, 150), 0.3f),

		{ 0.f,0.f,0.f,0.1f },

		{ 1.f,1.f,1.f,0.6f },
		eck::ColorrefToD2dColorF(RGB(20, 20, 20), 0.6f),

		{ 0.f,0.f,0.f,0.5f },

		{ 1.f,1.f,1.f,0.6f },
		{ 0.f,0.f,0.f,0.6f },

		{ 0.f,0.f,0.f,0.6f },
	};

	constexpr static D2D1_COLOR_F ColorDark[CrMax]
	{
		eck::ColorrefToD2dColorF(eck::Colorref::White),

		eck::ColorrefToD2dColorF(RGB(4, 170, 211), 0.7f),
		eck::ColorrefToD2dColorF(RGB(18, 19, 20), 0.6f),
		eck::ColorrefToD2dColorF(RGB(70, 71, 72), 0.6f),
		eck::ColorrefToD2dColorF(RGB(110, 111, 112), 0.65f),

		{ 1.f,1.f,1.f,0.6f },
		eck::ColorrefToD2dColorF(RGB(80, 80, 80), 0.3f),
		eck::ColorrefToD2dColorF(RGB(20, 150, 150), 0.3f),

		{ 1.f,1.f,1.f,0.1f },

		{ 1.f,1.f,1.f,0.6f },
		eck::ColorrefToD2dColorF(RGB(20, 20, 20), 0.6f),

		{ 0.f,0.f,0.f,0.5f },

		{ 0.f,0.f,0.f,0.6f },
		{ 1.f,1.f,1.f,0.6f },

		{ 1.f,1.f,1.f,0.6f },
	};
private:
	struct OPT
	{
		BOOLEAN bRainbowColor;
		BOOLEAN bCrosshair;
		BOOLEAN bKeyStroke;
		BOOLEAN bKeyStroke2;
		BOOLEAN bSpotLight;
		BOOLEAN bWndHilight;
		BOOLEAN bWndHilightDetectChild;
		BOOLEAN bRuler;
		BOOLEAN bWatermark;
		BOOLEAN bKSGiveAWayToCursor;
		BOOLEAN bTime;
		BOOLEAN bWndTip;
		BOOLEAN bBlurBkg;
		BOOLEAN bAutoRun;
		BOOLEAN bShowClick;
		BOOLEAN bShowCursorPos;
		BOOLEAN bLocateCursorWithCtrl;
		BOOLEAN bShowWheel;

		float cxCrosshairLine;
		float dCrosshairCursorGap;
		float fSpotLightRadius;
		float fClickRadius;
		float fClickRadiusUp;
		float fCursorPosRadius;
		float fLocateCursorRadius;

		eck::CRefStrW rsWatermark;
	};

	BOOL m_bDarkMode{};
	BOOL m_bAdmin{};
	IDWriteTextFormat* m_pTfCommon{};
	OPT m_Opt{};

	eck::CSignal<eck::NoIntercept_T, void, Notify, SSONOTIFY&> m_Sig{};
public:
	using HSlot = decltype(m_Sig)::HSlot;

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

	EckInlineCe void SetDarkMode(BOOL bDark) { m_bDarkMode = bDark; }

	void OptLoad();
	void OptSave();

	EckInlineNdCe BOOL IsAdmin() const { return m_bAdmin; }
	// 仅供初始化时使用
	void SetAdmin(BOOL bAdmin) { m_bAdmin = bAdmin; }
};

extern CApp* App;