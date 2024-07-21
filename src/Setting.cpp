//#include "pch.h"

#include "Setting.h"


void Tools::Windows程序启动项(HINSTANCE& hInst)
{
    //单实例程序

    //设置DPI

    hInstance = hInst;
}


void Tools::Windows窗口类注册(LPCWSTR icon)
{
    WNDCLASS wndclass;
    WNDCLASS wndclass2;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, icon);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = 程序_窗口类名;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WindowProcedure;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, icon);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = 程序_窗口类名2;
}

void Tools::Windows窗口创建()
{
    // 此处使用WS_EX_TOOLWINDOW 属性来隐藏显示在任务栏上的窗口程序按钮  
    hWnd = CreateWindowEx(WS_EX_TOOLWINDOW,
        程序_窗口类名, 程序_标题栏名,
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);

    hWnd_子窗口 = CreateWindowEx(0,
        程序_窗口类名, 程序_标题栏名_子窗口,
        WS_CHILD,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        300,
        100,
        hWnd, (HMENU)1001, hInstance, NULL);

    hWnd_进度条 = CreateWindowEx(0,
        程序_窗口类名2, 程序_标题栏名_子窗口,
        WS_CHILD,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        300,
        100,
        hWnd, (HMENU)1002, hInstance, NULL);

    //SetWindowLongPtr(hWnd_进度条, GWLP_WNDPROC, (LONG_PTR)WindowProcedure);

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    ShowWindow(hWnd_进度条, SW_SHOW);
    UpdateWindow(hWnd_进度条);
}

void Tools::Windows窗口初始化(LPCWSTR icon)
{
    工具箱配置文件.初始化(程序_名);

    // NOTIFYICONDATA
    nid.cbSize = sizeof(nid);
    nid.hWnd = hWnd;
    nid.uID = 0;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_USER;
    nid.hIcon = LoadIcon(hInstance, icon);
    lstrcpy(nid.szTip, 程序_窗口类名);

    Shell_NotifyIcon(NIM_ADD, &nid);

    nid = nid;
    hMenu = CreatePopupMenu(); //生成菜单

    //为菜单添加选项  
    AppendMenu(hMenu, MF_STRING, ID_工具箱配置, TEXT("工具箱配置"));
    AppendMenu(hMenu, MF_STRING, ID_清单, TEXT("清单"));
    AppendMenu(hMenu, MF_STRING, ID_帮助, TEXT("帮助"));
    AppendMenu(hMenu, MF_SEPARATOR, NULL, __TEXT("分割线"));
    AppendMenu(hMenu, MF_STRING, ID_笔记本键盘开关, TEXT("笔记本键盘开关"));
    AppendMenu(hMenu, MF_STRING, ID_修改屏幕分辨率, TEXT("修改屏幕分辨率"));
    AppendMenu(hMenu, MF_STRING, ID_Ping, TEXT("Ping"));
    AppendMenu(hMenu, MF_STRING, ID_子窗口test, TEXT("子窗口test"));
    AppendMenu(hMenu, MF_SEPARATOR, NULL, __TEXT("分割线"));
    AppendMenu(hMenu, MF_STRING, ID_退出, TEXT("退出"));

    //注册热键: Ctrl + Alt + F9
    //热键注册消息 temp(L"Ctrl + Alt + F9");
    //temp = RegisterHotKey(hWnd, Ctrl_Alt_F9, MOD_CONTROL | MOD_ALT, VK_F9);

    ////注册热键: Ctrl + Alt + F1
    //热键注册消息 temp1(L"Ctrl + Alt + F1");
    //temp1 = RegisterHotKey(hWnd, Ctrl_Alt_F1, MOD_CONTROL | MOD_ALT, VK_F1);

    ////注册热键: Ctrl + Alt + F10
    //热键注册消息 temp2(L"Ctrl + Alt + F10");
    //temp2 = RegisterHotKey(hWnd, Ctrl_Alt_F10, MOD_CONTROL | MOD_ALT, VK_F10);

    ////注册热键: Ctrl + Alt + F11
    //热键注册消息 temp3(L"Ctrl + Alt + F11");
    //temp3 = RegisterHotKey(hWnd, Ctrl_Alt_F11, MOD_CONTROL | MOD_ALT, VK_F11);

    //命令符 修改 nv显卡频率为 1080Mhz
    if (配置文件_nvidia == L"开") // second 没有去掉换行符
    {
        Shell消息 temp(L"nvidia-smi");
        temp = (int)ShellExecuteW(NULL, L"runas", L"cmd", L"nvidia-smi -lgc 1080", NULL, SW_SHOWNORMAL);
    }
}

void Tools::UpdateConfig()
{
    配置文件_原本屏幕分辨率宽 = 工具箱配置文件.Get基本设置内容().find(L"原本屏幕分辨率宽")->second;
    配置文件_原本屏幕分辨率高 = 工具箱配置文件.Get基本设置内容().find(L"原本屏幕分辨率高")->second;
    配置文件_修改的屏幕分辨率宽 = 工具箱配置文件.Get基本设置内容().find(L"修改的屏幕分辨率宽")->second;
    配置文件_修改的屏幕分辨率高 = 工具箱配置文件.Get基本设置内容().find(L"修改的屏幕分辨率高")->second;

    配置文件_笔记本键盘开关状态 = 工具箱配置文件.Get基本设置内容().find(L"笔记本键盘开关状态")->second;
    配置文件_Repos = 工具箱配置文件.Get基本设置内容().find(L"Repos")->second;
    配置文件_Lib = 工具箱配置文件.Get基本设置内容().find(L"Lib")->second;
    配置文件_nvidia = 工具箱配置文件.Get基本设置内容().find(L"nvidia-smi")->second;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;

    UINT WM_TASKBARCREATED;
    POINT pt; //用于接收鼠标坐标  
    int 菜单选项ID; //用于接收菜单选项返回值  

    // 不要修改TaskbarCreated，这是系统任务栏自定义的消息  
    WM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));
    switch (message)
    {
    case WM_CREATE: //窗口创建时候的消息.  
    {
        break;
    }
    case WM_USER: //连续使用该程序时候的消息.
    {
#if 0
        if (lParam == WM_LBUTTONDOWN)
            MessageBox(hwnd, TEXT("Win32 API 实现系统托盘程序,双击托盘可以退出!"), 程序_窗口类名, MB_OK);
        if (lParam == WM_LBUTTONDBLCLK)//双击托盘的消息,退出.  
            SendMessage(hwnd, WM_CLOSE, wParam, lParam);
#endif

        if (lParam == WM_RBUTTONDOWN)
        {
            GetCursorPos(&pt); //取鼠标坐标  
            ::SetForegroundWindow(hwnd); //解决在菜单外单击左键菜单不消失的问题  
            //EnableMenuItem(hmenu, IDR_PAUSE, MF_GRAYED); //让菜单中的某一项变灰  
            菜单选项ID = TrackPopupMenu(Setting.hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hwnd, NULL); //显示菜单并获取选项ID 

            if (菜单选项ID == ID_工具箱配置)
            {
                Setting.工具箱配置文件.打开配置文件();
                lg(L"修改配置后 需要重启本程序!");
                //MessageBoxW(0, L"修改配置后 需要重启本程序!", L"提示", 0);
            }
            if (菜单选项ID == ID_清单)
            {
                Setting.工具箱配置文件.清单开关();
            }
            if (菜单选项ID == ID_笔记本键盘开关)
            {
                if (Setting.配置文件_笔记本键盘开关状态 == L"关")
                {
                    Shell消息 temp(L"笔记本键盘关");
                    temp = (int)ShellExecuteW(NULL, L"runas", L"cmd", L"sc config i8042prt start= disabled", NULL, SW_SHOWNORMAL);
                }
                else if (Setting.配置文件_笔记本键盘开关状态 == L"开")
                {
                    Shell消息 temp(L"笔记本键盘开");
                    temp = (int)ShellExecuteW(NULL, L"runas", L"cmd", L"sc config i8042prt start= auto", NULL, SW_SHOWNORMAL);
                }
            }
            if (菜单选项ID == ID_修改屏幕分辨率)
            {
                if (Setting.修改屏幕分辨率)
                {
                    //修改屏幕分辨率
                    SetDisplaySize(
                        atoi(WstringToString(Setting.配置文件_原本屏幕分辨率宽).c_str()),
                        atoi(WstringToString(Setting.配置文件_原本屏幕分辨率高).c_str())
                    );
                    Setting.修改屏幕分辨率 = false;

                    lgc((Setting.配置文件_原本屏幕分辨率宽 + L" x " + Setting.配置文件_原本屏幕分辨率高).c_str(), L"屏幕分辨率修改成功");
                }
                else
                {
                    //修改屏幕分辨率
                    SetDisplaySize(
                        atoi(WstringToString(Setting.配置文件_修改的屏幕分辨率宽).c_str()),
                        atoi(WstringToString(Setting.配置文件_修改的屏幕分辨率高).c_str())
                    );
                    Setting.修改屏幕分辨率 = true;

                    lgc((Setting.配置文件_修改的屏幕分辨率宽 + L" x " + Setting.配置文件_修改的屏幕分辨率高).c_str(), L"屏幕分辨率修改成功");
                }
                break;
            }
            if (菜单选项ID == ID_帮助)
            {
                lg(L"Ctrl + Alt + F1: 清单\
\nCtrl + Alt + F9: 修改屏幕分辨率\
\nCtrl + Alt + F10: 打开 Repos 文件夹\
\nCtrl + Alt + F11: 打开 Lib 文件夹\
\n\n分辨率宽高最好是成比例(避免无效)\
\n   例如: 16:9 1920x1080\
\n       4:3 1280x960", L"快捷键帮助");

                //MessageBoxW(0, L"Ctrl + Alt + F1: 清单\nCtrl + Alt + F9: 修改屏幕分辨率\nCtrl + Alt + F10: 打开 Repos 文件夹\nCtrl + Alt + F11: 打开 Lib 文件夹\n\n分辨率宽高最好是成比例(避免无效)\n  例如: 16:9 1920x1080\n       4:3 1280x960", L"快捷键帮助", 0);
            }
            if (菜单选项ID == ID_Ping)
            {
                Shell消息 temp(L"Ping");
                temp = (int)ShellExecuteW(NULL, L"open", L"cmd", L"/k ping -t www.baidu.com", NULL, SW_SHOWNORMAL);
            }
            if (菜单选项ID == ID_子窗口test)
            {
                lgc(L"子窗口test: 测试开始");

                ShowWindow(Setting.hWnd_子窗口, SW_SHOW);
                UpdateWindow(Setting.hWnd_子窗口);
            }

            if (菜单选项ID == ID_退出)
            {
                PostQuitMessage(0);
            }
            //MessageBox(hwnd, TEXT("右键"), 程序_窗口类名, MB_OK);  
        }

        break;
    }
    case WM_HOTKEY:
    {        //按键消息为: Ctrl+Alt+F1 时: 打开清单
        //if (Ctrl_Alt_F1 == wParam)
        //{
        //    //打开清单
        //    工具箱配置文件.清单开关();
        //}
        ////按键消息为: Ctrl+Alt+F10 时: 打开 repos 文件夹
        //if (Ctrl_Alt_F10 == wParam)
        //{
        //    auto tempWstr = 配置文件_Repos;

        //    Shell消息 temp(L"Repos");
        //    temp = (int)ShellExecute(NULL, L"explore", tempWstr.c_str(), NULL, NULL, SW_SHOWNORMAL);
        //}
        ////按键消息为: Ctrl+Alt+F11 时: 打开 repos 文件夹
        //if (Ctrl_Alt_F11 == wParam)
        //{
        //    auto tempWstr = 配置文件_Lib;

        //    Shell消息 temp(L"Lib");
        //    temp = (int)ShellExecute(NULL, L"explore", tempWstr.c_str(), NULL, NULL, SW_SHOWNORMAL);
        //}
        ////按键消息为: Ctrl+Alt+F9 时: 修改屏幕分辨率
        //if (Ctrl_Alt_F9 == wParam)
        //{
        //    if (修改屏幕分辨率)
        //    {
        //        //修改屏幕分辨率
        //        SetDisplaySize(
        //            atoi(WstringToString(配置文件_原本屏幕分辨率宽).c_str()),
        //            atoi(WstringToString(配置文件_原本屏幕分辨率高).c_str())
        //        );
        //        修改屏幕分辨率 = false;

        //        lgc((配置文件_原本屏幕分辨率宽 + L" x " + 配置文件_原本屏幕分辨率高).c_str(), \
        //            L"屏幕分辨率修改成功");
        //    }
        //    else
        //    {
        //        //修改屏幕分辨率
        //        SetDisplaySize(
        //            atoi(WstringToString(配置文件_修改的屏幕分辨率宽).c_str()),
        //            atoi(WstringToString(配置文件_修改的屏幕分辨率高).c_str())
        //        );
        //        修改屏幕分辨率 = true;
        //        lgc((配置文件_修改的屏幕分辨率宽 + L" x " + 配置文件_修改的屏幕分辨率高).c_str(), \
        //            L"屏幕分辨率修改成功");
        //    }
        //}

        break;
    }
    case WM_DESTROY: //窗口销毁时候的消息.  
    {
        Shell_NotifyIcon(NIM_DELETE, &Setting.nid);
        PostQuitMessage(0);
        break;
    }
    default:
    {
        /*
        * 防止当Explorer.exe 崩溃以后，程序在系统系统托盘中的图标就消失
        *
        * 原理：Explorer.exe 重新载入后会重建系统任务栏。当系统任务栏建立的时候会向系统内所有
        * 注册接收TaskbarCreated 消息的顶级窗口发送一条消息，我们只需要捕捉这个消息，并重建系
        * 统托盘的图标即可。
        */
        if (message == WM_TASKBARCREATED)
            SendMessage(hwnd, WM_CREATE, wParam, lParam);
        break;
    }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK ChildWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
    {

    }
    case WM_DESTROY:
    {
        // 销毁窗口时清理资源
        PostQuitMessage(0);
        return 0;
    }
    case WM_PAINT:
    {
        // 绘制窗口内容
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // 例如：填充一个矩形
        RECT rect;
        GetClientRect(hwnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);

        return 0;
    }

    default:
    {
        // 对于未处理的消息，默认处理
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
}

LONG_PTR CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 填充背景
        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

        // 计算进度条的位置和大小
        int barWidth = (rect.right - rect.left) * Setting.进度条百分比值 / 100;
        RECT barRect = { rect.left + 1, rect.top + 1, rect.left + barWidth - 1, rect.bottom - 1 };
        RECT OldbarRect = { rect.left, rect.top, rect.right - rect.left, rect.bottom };

        // 绘制进度条的细线背景
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(150, 150, 150)); // 创建1像素宽的黑色实线画笔
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen); // 选择画笔到设备上下文
        Rectangle(hdc, OldbarRect.left, OldbarRect.top, OldbarRect.right, OldbarRect.bottom); // 绘制细线边框
        SelectObject(hdc, hOldPen); // 恢复旧画笔
        DeleteObject(hPen); // 释放画笔对象

        // 绘制进度条本身（内部填充）
        HBRUSH hBrush = CreateSolidBrush(RGB(0, 180, 0)); // 创建绿色画刷
        FillRect(hdc, &barRect, hBrush); // 填充进度条内部
        DeleteObject(hBrush); // 释放画刷对象

        EndPaint(hWnd, &ps);
        break;
    }

    default:
    {
        // 对于未处理的消息，默认处理
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    }
}
