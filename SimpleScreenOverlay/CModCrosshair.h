#pragma once
#include "CModBase.h"

class CModCrosshair final : public CModBase
{
public:
    void ModOnAppEvent(MOD_APPEVT_PARAM& Opt) override;
    void ModOnRender(const MOD_RENDER_PARAM& Opt) override;
    BOOL ModIsValid() const override { return TRUE; }
    BOOL ModIsTimeLineValid() const override
    {
        return App->GetOpt().bRainbowColor && App->GetOpt().bCrosshair;
    }
};