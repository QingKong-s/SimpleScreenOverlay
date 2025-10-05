#pragma once
#include "CModBase.h"

class CModWheelIndicator final : public CModBase
{
private:
    constexpr static float WheelGeoSide = 1.f;
    constexpr static D2D1_POINT_2F WheelGeoCenter{ 0.5f,1.73205f / 4.f };
    constexpr static UINT TE_WHEEL = 200;

    enum class Wheel : BYTE
    {
        None,
        Up,
        Down,
        Left,
        Right,
    };
    ComPtr<ID2D1GeometryRealization> m_pGrWheel{};
    ComPtr<ID2D1GeometryRealization> m_pGrWheelBorder{};
    D2D1_POINT_2F m_ptWheelIndicator{};
    Wheel m_eWheel{ Wheel::None };
    BOOLEAN m_bWheelTimer{};
    BYTE m_cWheelShowCount{};// 计数器，归零时取消显示
    Dui::CElem::HSlot m_hSlot{};

    void ReCreateGeometry();
    void UpdateWheelIndicatorPos();
    D2D1_RECT_F GetWheelIndicatorRect();
public:
    HRESULT ModInit(CVeVisualContainer* pContainer) override;
    void ModUnInit() override;
    void ModOnAppEvent(MOD_APPEVT_PARAM& Opt) override;
    void ModOnRender(const MOD_RENDER_PARAM& Opt) override;
    BOOL ModIsValid() const override { return TRUE; }
    BOOL ModIsTimeLineValid() const override { return FALSE; }
};