#include "framework.h"
#include "Resource.h"

#include "Setting.h"


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
    Log::SetDebug(true);
    lgc.SetAllLogFileWrite(true, Tx("Tools"), 0);
#else
    lgc.SetAllLogFileWrite(true, Tx("Tools"));
    Log::SetShowConsole(false);
#endif
    if (WindowsSystem::GetExeParentDirectoryPathName(Tools.ExeCurrentPath)) {
        lgcr(War, Format(Tx("当前程序路径: [%]"), Tools.ExeCurrentPath));
        lgcr(3);
    }
    else {
        lgcr(Err, Format(Tx("当前程序路径: 获取失败!")));
        lgcr(3);
        return 0;
    }

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