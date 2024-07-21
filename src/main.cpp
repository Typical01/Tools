#include "framework.h"
#include "Resource.h"

#include "Setting.h"

//#pragma comment(lib, "libTools_Tool.lib")


int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    Setting.Windows程序启动项(hInstance);

    Setting.Windows窗口类注册((LPCWSTR)IDI_ICON256x);

    Setting.Windows窗口创建();

    Setting.Windows窗口初始化((LPCWSTR)IDI_ICON256x);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}