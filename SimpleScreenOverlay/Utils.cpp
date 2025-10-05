#include "pch.h"

void HSV2RGB(float h, float s, float v,
    _Out_ float& r, _Out_ float& g, _Out_ float& b)
{
    const int i = int(h * 6);
    const float f = h * 6 - i;
    const float p = v * (1 - s);
    const float q = v * (1 - f * s);
    const float t = v * (1 - (1 - f) * s);
    float rf, gf, bf;

    switch (i % 6)
    {
    case 0: rf = v; gf = t; bf = p; break;
    case 1: rf = q; gf = v; bf = p; break;
    case 2: rf = p; gf = v; bf = t; break;
    case 3: rf = p; gf = q; bf = v; break;
    case 4: rf = t; gf = p; bf = v; break;
    case 5: rf = v; gf = p; bf = q; break;
    default: ECK_UNREACHABLE;
    }
    r = rf;
    g = gf;
    b = bf;
}

constexpr ULONGLONG TickStep = 40ull;
constexpr float HsvStep = 1.0f / 360.0f;

D2D1_COLOR_F CalcRainbowColor(ULONGLONG ullTick)
{
    const auto ullBase = (ullTick / (360 * TickStep)) * (360 * TickStep);
    D2D1_COLOR_F cr;
    HSV2RGB((ullTick - ullBase) / TickStep * HsvStep, 1.f, 1.f, cr.r, cr.g, cr.b);
    cr.a = 1.f;
    return cr;
}

D2D1_COLOR_F CalcRainbowColorWithStep(ULONGLONG ullTick, int nStep)
{
    return CalcRainbowColor(ullTick + nStep * TickStep);
}