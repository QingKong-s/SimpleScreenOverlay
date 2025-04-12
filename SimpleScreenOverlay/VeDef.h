#pragma once
enum
{
	VeCxFuncMenuBorder = 2,
	VeCxFuncMenu = 120,
	VeCyFuncMenuTitle = 24,
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
};

enum
{
	MenuIdxRender,

	MenuIdxMax
};

enum
{
	UIE_BEGIN = Dui::EE_PRIVATE_BEGIN,

	UIE_FUNCMENU_GETDISPINFO,
};