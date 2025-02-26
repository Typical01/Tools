#include "framework.h"
#include "Resource.h"

#include "Setting.h"


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
    Log::SetDebug(true);
#endif
    lgc.SetAllLogFileWrite(true, Tx("Tools"));
#ifndef _DEBUG
    Log::SetShowConsole(false);
#endif

    if (!WindowsExeAutoRunItem()) {
        return 0;
    }

    WindowsWindowClassRegister();

    WindowsWindowCreate();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}