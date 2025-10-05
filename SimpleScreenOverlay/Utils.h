#pragma once
void HSV2RGB(float h, float s, float v,
    _Out_ float& r, _Out_ float& g, _Out_ float& b);

D2D1_COLOR_F CalcRainbowColor(ULONGLONG ullTick);

D2D1_COLOR_F CalcRainbowColorWithStep(ULONGLONG ullTick, int nStep);