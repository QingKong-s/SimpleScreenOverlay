#pragma once
#include "CModBase.h"

class CModCursorLocation final : public CModBase
{
private:
    constexpr static float CursorLocateGeoRadius = 40.f;

    ComPtr<ID2D1GeometryRealization> m_pGrCursorLocate{};
    ComPtr<ID2D1RadialGradientBrush> m_pBrCursorLocate{};
    float m_msCursorLocateAni{};
    float m_fCursorLocateRadius{};
    float m_fCursorLocateAlpha{};
    BOOL m_bLocatingCursor{};
    Dui::CElem::HSlot m_hSlot{};

    void ReCreateGeometry();
    void ReCreateBrush();
public:
    HRESULT ModInit(CVeVisualContainer* pContainer) override;
    void ModUnInit() override;
    void ModOnAppEvent(MOD_APPEVT_PARAM& Opt) override;
    void ModOnRender(const MOD_RENDER_PARAM& Opt) override;
    BOOL ModIsValid() const override { return TRUE; }
    BOOL ModIsTimeLineValid() const override { return m_bLocatingCursor; }
    void ModOnTick(MOD_TICK_PARAM& Opt) override;
};