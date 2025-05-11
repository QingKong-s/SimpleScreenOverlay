#include "pch.h"

CApp* App{};

void CApp::Init()
{
	m_pTfCommon = eck::CreateDefTextFormatWithSize(14);
	OptLoad();
}

IDWriteTextFormat* CApp::CreateTextFormat(float fSize)
{
	return eck::CreateDefTextFormatWithSize(fSize);
}

void CApp::OptLoad()
{
	eck::CIniExtMut Ini{};
	const auto rbIni = eck::ReadInFile((eck::GetRunningPath() + L"\\Options.ini").Data());
	Ini.Load((PCWSTR)rbIni.Data(), rbIni.Size() / sizeof WCHAR);
	auto Sec = Ini.GetSection(L"SimpleScreenOverlay");

	m_Opt.bAutoRun = Ini.GetKeyValue(Sec, L"AutoRun"sv).GetBool(FALSE);
	m_Opt.bRainbowColor = Ini.GetKeyValue(Sec, L"RainbowColor"sv).GetBool(TRUE);
	m_Opt.bCrosshair = Ini.GetKeyValue(Sec, L"Crosshair"sv).GetBool(FALSE);
	m_Opt.bKeyStroke = Ini.GetKeyValue(Sec, L"KeyStroke"sv).GetBool(FALSE);
	m_Opt.bKeyStroke2 = Ini.GetKeyValue(Sec, L"KeyStroke2"sv).GetBool(TRUE);
	m_Opt.bSpotLight = Ini.GetKeyValue(Sec, L"SpotLight"sv).GetBool(TRUE);
	m_Opt.bWndHilight = Ini.GetKeyValue(Sec, L"WndHilight"sv).GetBool(FALSE);
	m_Opt.bWndHilightDetectChild = Ini.GetKeyValue(Sec, L"WndHilightDetectChild"sv).GetBool(TRUE);
	m_Opt.bRuler = Ini.GetKeyValue(Sec, L"Ruler"sv).GetBool(FALSE);
	m_Opt.bWatermark = Ini.GetKeyValue(Sec, L"Watermark"sv).GetBool(FALSE);
	m_Opt.bTime = Ini.GetKeyValue(Sec, L"Time"sv).GetBool(TRUE);
	m_Opt.bWndTip = Ini.GetKeyValue(Sec, L"WndTip"sv).GetBool(FALSE);
	m_Opt.bBlurBkg = Ini.GetKeyValue(Sec, L"BlurBkg"sv).GetBool(FALSE);
	m_Opt.cxCrosshairLine = (float)Ini.GetKeyValue(Sec,
		L"CrosshairLineWidth"sv).GetDouble(1.f);
	m_Opt.dCrosshairCursorGap = (float)Ini.GetKeyValue(Sec,
		L"CrosshairCursorGap"sv).GetDouble(20.f);
	m_Opt.fSpotLightRadius = (float)Ini.GetKeyValue(Sec,
		L"SpotLightRadius"sv).GetDouble(40.f);
	Ini.GetKeyValue(Sec, L"WatermarkText"sv).GetString(m_Opt.rsWatermark, L"QKSOFT"sv);
}

void CApp::OptSave()
{
	eck::CIniExtMut Ini{};
	auto Sec = Ini.CreateSection(L"SimpleScreenOverlay");
	eck::CRefStrW rsTmp{};
	rsTmp.Format(L"%d", m_Opt.bAutoRun);
	Ini.SetKeyValue(Sec, L"AutoRun"sv, rsTmp.ToStringView());
	rsTmp.Format(L"%d", m_Opt.bRainbowColor);
	Ini.SetKeyValue(Sec, L"RainbowColor"sv, rsTmp.ToStringView());
	rsTmp.Format(L"%d", m_Opt.bCrosshair);
	Ini.SetKeyValue(Sec, L"Crosshair"sv, rsTmp.ToStringView());
	rsTmp.Format(L"%d", m_Opt.bKeyStroke);
	Ini.SetKeyValue(Sec, L"KeyStroke"sv, rsTmp.ToStringView());
	rsTmp.Format(L"%d", m_Opt.bKeyStroke2);
	Ini.SetKeyValue(Sec, L"KeyStroke2"sv, rsTmp.ToStringView());
	rsTmp.Format(L"%d", m_Opt.bSpotLight);
	Ini.SetKeyValue(Sec, L"SpotLight"sv, rsTmp.ToStringView());
	rsTmp.Format(L"%d", m_Opt.bWndHilight);
	Ini.SetKeyValue(Sec, L"WndHilight"sv, rsTmp.ToStringView());
	rsTmp.Format(L"%d", m_Opt.bWndHilightDetectChild);
	Ini.SetKeyValue(Sec, L"WndHilightDetectChild"sv, rsTmp.ToStringView());
	rsTmp.Format(L"%d", m_Opt.bRuler);
	Ini.SetKeyValue(Sec, L"Ruler"sv, rsTmp.ToStringView());
	rsTmp.Format(L"%d", m_Opt.bWatermark);
	Ini.SetKeyValue(Sec, L"Watermark"sv, rsTmp.ToStringView());
	rsTmp.Format(L"%d", m_Opt.bTime);
	Ini.SetKeyValue(Sec, L"Time"sv, rsTmp.ToStringView());
	rsTmp.Format(L"%d", m_Opt.bWndTip);
	Ini.SetKeyValue(Sec, L"WndTip"sv, rsTmp.ToStringView());
	rsTmp.Format(L"%d", m_Opt.bBlurBkg);
	Ini.SetKeyValue(Sec, L"BlurBkg"sv, rsTmp.ToStringView());
	rsTmp.Format(L"%f", m_Opt.cxCrosshairLine);
	Ini.SetKeyValue(Sec, L"CrosshairLineWidth"sv, rsTmp.ToStringView());
	rsTmp.Format(L"%f", m_Opt.dCrosshairCursorGap);
	Ini.SetKeyValue(Sec, L"CrosshairCursorGap"sv, rsTmp.ToStringView());
	rsTmp.Format(L"%f", m_Opt.fSpotLightRadius);
	Ini.SetKeyValue(Sec, L"SpotLightRadius"sv, rsTmp.ToStringView());
	Ini.SetKeyValue(Sec, L"WatermarkText"sv, m_Opt.rsWatermark.ToStringView());
	rsTmp.Clear();
	Ini.Save(rsTmp);
	eck::WriteToFile((eck::GetRunningPath() + L"\\Options.ini").Data(),
		rsTmp.Data(), (DWORD)rsTmp.ByteSizePure());
}