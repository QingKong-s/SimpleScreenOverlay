#pragma once
enum
{
	VeCxFuncMenuBorder = 2,
	VeCxFuncMenu = 130,
	VeCyFuncMenuTitle = 26,
	VeCyFuncMenuFooter = 16,
	VeCyFuncMenuItem = 30,
	VeCyFuncMenuItemPadding = 1,

	VeCxyKeyStrokePadding = 8,
	VeCxyKeyStroke2Padding = 6,
	VeCyKeyStrokeMouseBtnPadding = VeCxyKeyStrokePadding * 5,
	VeCyKeyStrokeSpaceBtn = 24,
	VeCyKeyStrokeMouseBtn = 24,
	VeCxKeyStrokeCachePadding = 2,
	VeCxKeyStrokeBorder = 1,

	VeCxWndHiliBorder = 2,
	VeCxWndTipMargin = 12,
	VeCyWndTipMargin = 12,

	VeCxRulerTipMargin = 12,
	VeCyRulerTipMargin = 12,
};

enum
{
	MenuIdxRender,
	MenuIdxTools,

	MenuIdxMax
};

enum
{
	UIE_BEGIN = Dui::EE_PRIVATE_BEGIN,

	UIE_MENU_GETDISPINFO,
};