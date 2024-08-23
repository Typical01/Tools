#include "framework.h"
#include "Resource.h"

#include "Setting.h"

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    //Log::SetConsoleShow(false);
    Log::SetAllConsoleTimeShow(false);

    if (!Windows程序启动项()) {
        return 0;
    }

    Windows窗口类注册();

    Windows窗口创建();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}