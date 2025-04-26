#include "framework.h"
#include "Resource.h"

#include "Setting.h"



int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    if (!WindowsSystem::GetExePathName(Tools.ExeCurrentPath)) {
        std::wcout << Printf(TEXT("Main: 当前程序路径: 获取失败!")).str();
        return 0;
    }
    if (!WindowsSystem::ExtractExeDirectoryName(Tools.ExeCurrentPath)) {
        std::wcout << Printf(TEXT("Main: 当前程序路径: 提取失败!")).str();
        std::wcout << Printf(TEXT("Tools.ExeCurrentPath: [%s]\n\n"), Tools.ExeCurrentPath).str();
        return 0;
    }
    std::wcout << Printf(TEXT("Main: 当前程序路径: 获取成功[%s]\n\n"), Tools.ExeCurrentPath).str();

    if (!WindowsExeAutoRunItem()) {
        return 0;
    }

#ifdef _DEBUG
    Log::SetDebug(true);
    lgc.SetAllLogFileWrite(true, TEXT("Tools"), Tools.ExeCurrentPath, 0);
#else
    Log::SetDebug(false);
    lgc.SetAllLogFileWrite(true, TEXT("Tools"), Tools.ExeCurrentPath);
    Log::SetShowConsole(false);
#endif
    WindowsWindowClassRegister();
    WindowsWindowCreate();

    MSG msg;
    //while (true) {
    //    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //    }
    //    else {
    //        WaitMessage(); // 等待新消息到达
    //    }
    //}

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}