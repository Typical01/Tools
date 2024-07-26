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
    Log::SetConsoleShow(false);
    Log::SetConsoleTimeShow(false);

    if (!Windows程序启动项()) {
        return 0;
    }

    Windows窗口类注册();

    Windows窗口创建();

    初始化();

    /*WinHost::注册进度条窗口类(hIns);
    WH::设置进度条步进(1);
    WH::创建进度条(L"进度条");*/

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        /*Timers::sleep_s(50);
        WH::增加进度条进度();*/
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}