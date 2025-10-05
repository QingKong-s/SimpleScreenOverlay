#pragma once
#include "CModBase.h"

class CModCursorPos final : public CModBase
{
private:
    constexpr static float CursorGeoRadius = 40.f;

    ComPtr<ID2D1GeometryRealization> m_pGrCursorPos{};
    Dui::CElem::HSlot m_hSlot{};

    void ReCreateGeometry();
public:
    HRESULT ModInit(CVeVisualContainer* pContainer) override;
    void ModUnInit() override;
    void ModOnAppEvent(MOD_APPEVT_PARAM& Opt) override;
    void ModOnRender(const MOD_RENDER_PARAM& Opt) override;
    BOOL ModIsValid() const override { return TRUE; }
    BOOL ModIsTimeLineValid() const override { return FALSE; }
};