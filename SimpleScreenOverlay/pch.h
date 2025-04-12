#pragma once
#define ECK_OPT_NO_YYJSON 1
#define ECK_OPT_NO_PUGIXML 1

#define _CRTDBG_MAP_ALLOC 1

#define SSO_WINRT 0

#include "eck\PchInc.h"
#include "eck\CCommDlg.h"
#include "eck\SystemHelper.h"
#include "eck\MathHelper.h"
#include "eck\CDuiListTemplate.h"

#if SSO_WINRT
#include "eck\WinRtDCompInterop.h"
#include "eck\BackdropEffectChain.h"

#include <DispatcherQueue.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.Graphics.Effects.h>
#include <winrt/Windows.UI.Composition.Effects.h>
#endif

#include <d2d1effectauthor.h>
#include <d3dcompiler.h>

#include "CApp.h"
#include "CFixTimeLine.h"

using eck::PCVOID;
using eck::PCBYTE;
using eck::SafeRelease;
using eck::ComPtr;

namespace Dui = eck::Dui;

using namespace std::literals;
#if SSO_WINRT
using namespace winrt;
using namespace Windows::UI;
using namespace Windows::UI::Composition;
using namespace Windows::UI::Composition::Desktop;
#endif