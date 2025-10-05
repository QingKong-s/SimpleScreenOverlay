#pragma once
#include "CModBase.h"

class CModClickShow final : public CModBase
{
private:
    constexpr static float ClickGeoRadius = 40.f;

    enum class ClickState : BYTE
    {
        None,
        L,
        M,
        R,
    };
    struct CLICK
    {
        ULONGLONG tAdd;
        D2D1_POINT_2F ptCenter;
        ClickState eState;
        BOOLEAN bUp;
        float ms;
        float fRadius;
        float fAlpha;
    };
    std::vector<CLICK> m_vClick{};
    ComPtr<ID2D1StrokeStyle1> m_pStrokeStyleClick{};
    ComPtr<ID2D1EllipseGeometry> m_pEllipseClick{};
public:
    HRESULT ModInit(CVeVisualContainer* pContainer) override;
    void ModUnInit() override;
    void ModOnAppEvent(MOD_APPEVT_PARAM& Opt) override;
    void ModOnRender(const MOD_RENDER_PARAM& Opt) override;
    BOOL ModIsValid() const override { return TRUE; }
    BOOL ModIsTimeLineValid() const override { return !m_vClick.empty(); }
    void ModOnTick(MOD_TICK_PARAM& Opt) override;
};