#pragma once
constexpr inline float
VeCxFuncMenuBorder = 2,
VeCxFuncMenu = 130,
VeCyFuncMenuTitle = 26,
VeCyFuncMenuFooter = 16,
VeCyFuncMenuItem = 24,
VeCyFuncMenuItemPadding = 0,

VeCxKeyStrokeMargin = 16,
VeCyKeyStrokeMargin = 16,
VeCxKeyStroke = 150,
VeCyKeyStroke = 320,
VeCyKeyStroke2 = 240,
VeCxyKeyStrokePadding = 8,
VeCxyKeyStroke2Padding = 6,
VeCyKeyStrokeMouseBtnPadding = VeCxyKeyStrokePadding * 5,
VeCyKeyStrokeSpaceBtn = 24,
VeCyKeyStrokeMouseBtn = 24,
VeCxKeyStrokeBorder = 1,

VeCxWndHiliBorder = 2,
VeCxWndTipMargin = 12,
VeCyWndTipMargin = 12,

VeCxRulerTipMargin = 12,
VeCyRulerTipMargin = 12,

VeCyMenuTipBottomMarginPct = 15,
VeCxyMenuTipMargin = 5,

VeCxClickStroke = 4,

VeCxWheelIndicator = 15,

VeCxSizeBorder = 2,
VeCxSizeBorderArrow = 5
;

enum
{
	MenuIdxProgram,
	MenuIdxRender,
	MenuIdxTools,

	MenuIdxMax
};

enum
{
	UIE_BEGIN = Dui::EE_PRIVATE_BEGIN,
	UIE_MENU_GETDISPINFO,

	EWM_PRIVBEGIN0 = Dui::EWM_PRIVBEGIN,
	EWM_SHOW_MENU,// (bShow, 0)
};

enum
{
	IDT_BEGIN = 0x514B,
	IDT_WHEEL,
};