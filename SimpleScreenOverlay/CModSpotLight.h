#pragma once
#include "CModBase.h"

class CModSpotLight final : public CModBase
{
private:
    constexpr static float SpotLightGeoRadius = 40.f;
   
    float m_fSpotLightMaxRadius{};
    float m_msSpotLightAni{};
    float m_kSpotLight{};
    BOOLEAN m_bShowSpotLight{};
    BOOLEAN m_bSpotLightReverse{};
    BOOLEAN m_bSpotLightAnimating{};
    ComPtr<ID2D1GeometryRealization> m_pGrSpotLight{};
    Dui::CElem::HSlot m_hSlot{};

    void ReCreateGeometry();
public:
    HRESULT ModInit(CVeVisualContainer* pContainer) override;
    void ModUnInit() override;
    void ModOnAppEvent(MOD_APPEVT_PARAM& Opt) override;
    void ModOnRender(const MOD_RENDER_PARAM& Opt) override;
    BOOL ModIsValid() const override { return TRUE; }
    BOOL ModIsTimeLineValid() const override { return m_bSpotLightAnimating; }
    void ModOnTick(MOD_TICK_PARAM& Opt) override;
};