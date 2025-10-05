#include "pch.h"

#include "CWndMain.h"

#include "eck\Env.h"

#if SSO_WINRT
#pragma comment(lib, "CoreMessaging.lib")
#pragma comment(lib, "RuntimeObject.lib")
#endif

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ PWSTR pszCmdLine, _In_ int nCmdShow)
{
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
    HRESULT hr;

    if (FAILED(hr = CoInitialize(nullptr)))
    {
        EckDbgPrintFormatMessage(hr);
        eck::MsgBox(eck::Format(L"CoInitialize failed!\r\nhr = %08X", hr),
            L"Error", MB_ICONERROR);
        return 0;
    }

    DWORD dwErr;
    if (const auto iRetInit = eck::Init(hInstance, nullptr, &dwErr);
        iRetInit != eck::InitStatus::Ok)
    {
        EckDbgPrintFormatMessage(dwErr);
        eck::MsgBox(eck::Format(L"Init failed!\r\nInitStatus = %d\r\nError code = %08X",
            (int)iRetInit, dwErr), L"Error", MB_ICONERROR);
        return 0;
    }
    const auto bAdmin = IsNTAdmin(0, nullptr);
#ifndef _DEBUG
    if (bAdmin && !eck::UiaIsAcquired())
    {
        eck::UIA uia;
        if (NT_SUCCESS(eck::UiaTryAcquire(uia)))
            eck::UiaRestart(uia);
    }
#endif // _DEBUG

#if SSO_WINRT
    DispatcherQueueOptions options
    {
        sizeof(DispatcherQueueOptions),
        DQTYPE_THREAD_CURRENT,
        DQTAT_COM_STA
    };

    Windows::System::DispatcherQueueController Controller{ nullptr };
    hr = CreateDispatcherQueueController(options,
        (ABI::Windows::System::IDispatcherQueueController**)(put_abi(Controller)));
    if (FAILED(hr))
    {
        EckDbgPrintFormatMessage(hr);
        eck::MsgBox(eck::Format(L"CreateDispatcherQueueController failed!\r\nhr = %08X", hr),
            L"Error", MB_ICONERROR);
        return 0;
    }
#endif // SSO_WINRT

    App = new CApp{};
    App->Init();
    App->SetAdmin(bAdmin);

    const auto pWnd = new CWndMain{};
    const auto hMon = eck::GetOwnerMonitor(nullptr);
    const auto iDpi = eck::GetMonitorDpi(hMon);
    MONITORINFO mi;
    mi.cbSize = sizeof(MONITORINFO);
    GetMonitorInfoW(hMon, &mi);

    pWnd->SetUserDpi(iDpi);
    pWnd->SetTransparent(TRUE);
#if SSO_WINRT
    pWnd->SetPresentMode(Dui::PresentMode::DCompositionVisual);
#else
    pWnd->SetPresentMode(Dui::PresentMode::DCompositionSurface);
#endif
    pWnd->Create(nullptr, WS_POPUP,
        WS_EX_LAYERED | WS_EX_TRANSPARENT |
        WS_EX_TOPMOST |
        WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
        mi.rcWork.left, mi.rcWork.top,
        mi.rcWork.right - mi.rcWork.left,
        mi.rcWork.bottom - mi.rcWork.top,
        nullptr, 0);
    pWnd->Visible = TRUE;

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        if (!eck::PreTranslateMessage(msg))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    App->OptSave();
    delete pWnd;
    delete App;
    eck::ThreadUnInit();
    eck::UnInit();
#ifdef _DEBUG
    if (eck::g_pDxgiDebug)
        eck::g_pDxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
#endif // _DEBUG
    CoUninitialize();
    return (int)msg.wParam;
}