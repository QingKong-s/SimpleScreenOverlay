#include "pch.h"

CApp* App{};

void CApp::Init()
{
	m_pTfCommon = eck::CreateDefTextFormatWithSize(14);

	m_Opt.bRainbowColor = 1;
	m_Opt.bCrosshair = 0;
	m_Opt.cxCrosshairLine = 1.f;
	m_Opt.dCrosshairCursorGap = 20.f;
	m_Opt.bKeyStroke = 0;
	m_Opt.bKeyStroke2 = 1;
	m_Opt.bSpotLight = 1;
	m_Opt.fSpotLightRadius = 40.f;
	m_Opt.bWndHilight = 0;
	m_Opt.bWndHilightDetectChild = 1;
	m_Opt.bRuler = 0;
	m_Opt.rsWatermark = L"WATERMARK";
	m_Opt.bWatermark = 0;
	m_Opt.bKSGiveAWayToCursor = 1;
	m_Opt.bWndTip = 0;
}

IDWriteTextFormat* CApp::CreateTextFormat(float fSize)
{
	return eck::CreateDefTextFormatWithSize(fSize);
}
