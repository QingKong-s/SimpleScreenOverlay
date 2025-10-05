#pragma once
#include "CApp.h"

// ModOnAppEvent
struct MOD_APPEVT_PARAM
{
    Notify eNotify;
    /*[InOut]*/ BOOLEAN bWakeRenderThread;
    /*[InOut]*/ BOOLEAN bUpdate;
    SSONOTIFY n;
    // 相对容器元素坐标的脏区域
    // 函数退出前模块负责更新该区域，注意绝对不能缩减区域
    /*[InOut]*/ D2D1_RECT_F rcUpdate;
    D2D1_POINT_2F ptNewCursor;
    D2D1_POINT_2F ptOldCursor;

};

struct MOD_RENDER_PARAM
{
    ID2D1DeviceContext1* pDC;
    ID2D1SolidColorBrush* pBrush;
    ID2D1SolidColorBrush* pBrRainbow;
    Dui::ELEMPAINTSTRU ps;
};

struct MOD_TICK_PARAM
{
    float ms;
    // 相对容器元素坐标的脏区域
    // 函数退出前模块负责更新该区域，注意绝对不能缩减区域
    /*[InOut]*/ D2D1_RECT_F rcUpdate;
};

class CVeVisualContainer;
class CModBase
{
protected:
    CVeVisualContainer* m_pContainer{};
public:
    virtual ~CModBase() = default;
    virtual HRESULT ModInit(CVeVisualContainer* pContainer)
    {
        EckAssert(!m_pContainer);
        m_pContainer = pContainer;
        return S_OK;
    }
    virtual void ModUnInit() { m_pContainer = nullptr; }
    virtual void ModOnAppEvent(MOD_APPEVT_PARAM& Opt) {}
    virtual void ModOnRender(const MOD_RENDER_PARAM& Opt) = 0;
    virtual BOOL ModIsValid() const = 0;
    virtual BOOL ModIsTimeLineValid() const = 0;
    virtual void ModOnTick(MOD_TICK_PARAM& Opt) {}
};