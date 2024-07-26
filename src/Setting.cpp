//#include "pch.h"

#include "Setting.h"


using Tools_Tool::WindowsSystem::WindowHosting;


//Settings Tools;

int Windows程序启动项()
{
    lgcr(L"Windows程序启动项()");

    //设置DPI
    WindowHosting::WindowDPI();

    //单实例程序
    if (WindowHosting::单实例运行(Tools.程序_窗口类名, Tools.程序_标题栏名) == 0) {
        return 0;
    }

    Tools.工具箱配置文件.初始化(Tools.程序_名);
    UpdateConfig();

    if (Tools.设置_开机自启动 == L"否") {
        if (SetAutoRun(L"典型一号的工具箱", Tools.工具箱配置文件.Get程序路径().c_str())) {
            auto temp = Tools.工具箱配置文件.Get基本设置内容();
            if (temp.find(L"注册表开机自启动") != temp.cend()) {
                temp[L"注册表开机自启动"] = L"是";
                lgc(temp.find(L"注册表开机自启动")->first + L"=" + temp.find(L"注册表开机自启动")->second, lgm::wr);
            }
            std::vector<Ustr> tempSetting;
            tempSetting.push_back(L"[基本设置]\n");
            for (auto temp内容 = temp.cbegin(); temp内容 != temp.cend(); temp内容++) {
                auto tempStr = temp内容->first + L"=" + temp内容->second + L"\n";
                tempSetting.push_back(tempStr);
                lgc(tempStr, lgm::wr);
            }

            lgc(L"注册表修改后的配置文件");
            CfgFile::写入文件(Tools.工具箱配置文件.Get工具箱配置文件路径(), tempSetting);
            lgr(L"注册开机自启动: 典型一号的工具箱");
        }
    }

        /*if (!IsUserAdmin()) {
            lgr(L"稍后会弹出: \n  获取管理员权限的窗口\n  选择确定获取后自动注册开机自启动\n  选择取消不会注册");
            if (WindowHosting::获取管理员权限(true)) {
                return 0;
            }
        }
        else {
            if (SetAutoRun(L"典型一号的工具箱", Tools.工具箱配置文件.Get程序路径().c_str())) {
                auto temp = Tools.工具箱配置文件.Get基本设置内容();
                if (temp.find(L"注册表开机自启动") != temp.cend()) {
                    temp[L"注册表开机自启动"] = L"是";
                    lgc(temp.find(L"注册表开机自启动")->first + L"=" + temp.find(L"注册表开机自启动")->second, lgm::wr);
                }
                std::vector<Ustr> tempSetting;
                tempSetting.push_back(L"[基本设置]\n");
                for (auto temp内容 = temp.cbegin(); temp内容 != temp.cend(); temp内容++) {
                    auto tempStr = temp内容->first + L"=" + temp内容->second + L"\n";
                    tempSetting.push_back(tempStr);
                    lgc(tempStr, lgm::wr);
                }
                
                lgc(L"注册表修改后的配置文件");
                CfgFile::写入文件(Tools.工具箱配置文件.Get工具箱配置文件路径(), tempSetting);
                lgr(L"注册开机自启动: 典型一号的工具箱");
            }*/

    Tools.Icon = (LPWSTR)IDI_ICON256X;

    Tools.hIns = GetModuleHandle(NULL);

    HFONT Font = CreateFont(
        -16, -7, 0, 0,
        400, //粗度 一般这个值设为400
        FALSE, //斜体
        FALSE, //下划线
        FALSE, //删除线
        DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
        FF_DONTCARE,
        TEXT("微软雅黑")
    );
    Tools.WinHost.设置字体(Font);
    
    return 1;
}


void Windows窗口类注册()
{
    lgcr(L"Windows窗口类注册()");

    WNDCLASS wndclass = { 0 };
    WNDCLASS wndclass_设置窗口 = { 0 };
    WNDCLASS wndclass_标签窗口 = { 0 };

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = Tools.hIns;
    wndclass.hIcon = LoadIcon(Tools.hIns, Tools.Icon);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = Tools.程序_窗口类名;

    /*wndclass_设置窗口.style = CS_HREDRAW | CS_VREDRAW;
    wndclass_设置窗口.lpfnWndProc = SettingWndProc;
    wndclass_设置窗口.cbClsExtra = 0;
    wndclass_设置窗口.cbWndExtra = 0;
    wndclass_设置窗口.hInstance = Tools.hIns;
    wndclass_设置窗口.hIcon = LoadIcon(Tools.hIns, Tools.Icon);
    wndclass_设置窗口.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass_设置窗口.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass_设置窗口.lpszMenuName = NULL;
    wndclass_设置窗口.lpszClassName = Tools.程序_设置窗口类名;

    wndclass_标签窗口.style = CS_HREDRAW | CS_VREDRAW;
    wndclass_标签窗口.lpfnWndProc = TipsWndProc;
    wndclass_标签窗口.cbClsExtra = 0;
    wndclass_标签窗口.cbWndExtra = 0;
    wndclass_标签窗口.hInstance = Tools.hIns;
    wndclass_标签窗口.hIcon = LoadIcon(Tools.hIns, Tools.Icon);
    wndclass_标签窗口.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass_标签窗口.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass_标签窗口.lpszMenuName = NULL;
    wndclass_标签窗口.lpszClassName = Tools.程序_标签窗口类名;*/

    Tools.WinHost.注册窗口类(wndclass);/*
    Tools.WinHost.注册窗口类(wndclass_设置窗口);
    Tools.WinHost.注册窗口类(wndclass_标签窗口);*/
}

void Windows窗口创建()
{
    lgcr(L"Windows窗口创建()");

    //窗口创建时会产生消息: WM_CREATE
    //要在 WM_CREATE 之前设置好菜单选项的 ID
    //按键
    Tools.ID_帮助 = WindowHosting::GetHMENU();
    Tools.ID_退出 = WindowHosting::GetHMENU();
    Tools.ID_工具箱设置 = WindowHosting::GetHMENU();
    Tools.ID_标签 = WindowHosting::GetHMENU();
    Tools.ID_笔记本键盘开关 = WindowHosting::GetHMENU();
    Tools.ID_修改屏幕分辨率 = WindowHosting::GetHMENU();
    Tools.ID_Ping = WindowHosting::GetHMENU();
    /*lgc(L"ID_帮助: " + Uto_string(Tools.ID_帮助), lgm::wr);
    lgc(L"ID_退出: " + Uto_string(Tools.ID_退出), lgm::wr);
    lgc(L"ID_工具箱设置: " + Uto_string(Tools.ID_工具箱设置), lgm::wr);
    lgc(L"ID_标签: " + Uto_string(Tools.ID_标签), lgm::wr);
    lgc(L"ID_笔记本键盘开关: " + Uto_string(Tools.ID_笔记本键盘开关), lgm::wr);
    lgc(L"ID_修改屏幕分辨率: " + Uto_string(Tools.ID_修改屏幕分辨率), lgm::wr);
    lgc(L"ID_Ping: " + Uto_string(Tools.ID_Ping), lgm::wr);*/

    Tools.hWnd_托盘 = CreateWindowEx(
    // 此处使用WS_EX_TOOLWINDOW 属性来隐藏显示在任务栏上的窗口程序按钮  
        WS_EX_TOOLWINDOW,
        //0,
        Tools.程序_窗口类名, Tools.程序_标题栏名,
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL, NULL, Tools.hIns, NULL);
    Tools.WinHost.添加窗口托管(L"hWnd_托盘", Tools.hWnd_托盘);

    // 不要修改TaskbarCreated，这是系统任务栏自定义的消息  
    Tools.WM_TASKBARCREATED = RegisterWindowMessage(L"TaskbarCreated");
}

void 初始化()
{
    lgcr(L"窗口创建后: 初始化()");

    UpdateConfig();

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
    Tools.菜单_修改分辨率 = WindowHotkey::GetHotkey();
    Tools.菜单_打开Repos = WindowHotkey::GetHotkey();
    Tools.菜单_打开Lib = WindowHotkey::GetHotkey();

    热键注册消息 热键注册_F9(L"Ctrl + Alt + F9", RegisterHotKey(Tools.hWnd_托盘, Tools.菜单_修改分辨率, MOD_CONTROL | MOD_ALT, VK_F9));
    热键注册消息 热键注册_F10(L"Ctrl + Alt + F10", RegisterHotKey(Tools.hWnd_托盘, Tools.菜单_打开Repos, MOD_CONTROL | MOD_ALT, VK_F10));
    热键注册消息 热键注册_F11(L"Ctrl + Alt + F11", RegisterHotKey(Tools.hWnd_托盘, Tools.菜单_打开Lib, MOD_CONTROL | MOD_ALT, VK_F11));

    //命令符 修改 nv显卡频率为 1080Mhz
    if (Tools.设置_Nvidia == L"开") {
        WindowShell::执行(L"nvidia-smi", L"runas", L"cmd", L"nvidia-smi -lgc 1080", 0);
    }
    }       

void 菜单选择(int 菜单选项ID)
{
    if (菜单选项ID == Tools.ID_工具箱设置)
    {
        lgc(L"ID_工具箱设置", L"菜单选项ID");

#if 0
        //设置窗口
        Tools.hWnd_设置 = CreateWindowEx(0,
            Tools.程序_窗口类名, Tools.程序_标题栏名_设置窗口,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            600,
            400,
            NULL, NULL, Tools.hIns, NULL);
        SetWindowLongPtr(Tools.hWnd_设置, GWLP_WNDPROC, (LONG_PTR)SettingWndProc);
        Tools.WinHost.添加窗口托管(L"hWnd_设置", Tools.hWnd_设置);
#endif
        Tools.工具箱配置文件.打开配置文件();
        lgr(L"修改后需要重启程序!");
    }
#if 0
    if (菜单选项ID == Tools.ID_标签)
    {
        lgc(L"ID_标签", L"菜单选项ID");

        Tools.hWnd_标签 = CreateWindowEx(NULL,
            Tools.程序_窗口类名, Tools.程序_标题栏名_标签窗口,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            550,
            300,
            NULL, NULL, Tools.hIns, NULL);
        SetWindowLongPtr(Tools.hWnd_标签, GWLP_WNDPROC, (LONG_PTR)TipsWndProc);
        Tools.WinHost.添加窗口托管(L"hWnd_标签", Tools.hWnd_标签);

        Tools.标签菜单_Add = WinHost::GetHMENU();
        Tools.标签菜单_Set = WinHost::GetHMENU();
        Tools.标签菜单_Del = WinHost::GetHMENU();
        Tools.标签菜单_Setting = WinHost::GetHMENU();
        Tools.标签菜单_Help = WinHost::GetHMENU();

        HMENU hMain = CreateMenu();
        AppendMenu(hMain, MF_POPUP, Tools.标签菜单_Add, __TEXT("增加"));
        AppendMenu(hMain, MF_POPUP, Tools.标签菜单_Set, __TEXT("修改"));
        AppendMenu(hMain, MF_POPUP, Tools.标签菜单_Del, __TEXT("删除"));
        AppendMenu(hMain, MF_POPUP, Tools.标签菜单_Setting, __TEXT("设置"));
        AppendMenu(hMain, MF_POPUP, Tools.标签菜单_Help, __TEXT("帮助"));
        SetMenu(Tools.hWnd_标签, hMain);
    }
#endif
    if (菜单选项ID == Tools.ID_笔记本键盘开关)
    {
        Shell消息 temp;

        lgc(L"ID_笔记本键盘开关", L"菜单选项ID");
        if (Tools.设置_笔记本键盘开关状态 == L"关")
        {
            temp = WindowShell::执行(L"笔记本键盘关", L"runas", L"cmd", L"sc config i8042prt start= disabled", 0);
            if (temp.IsSucceed()) {
                lgr(L"笔记本键盘关闭");
            }
        }
        else if (Tools.设置_笔记本键盘开关状态 == L"开")
        {
            temp = WindowShell::执行(L"笔记本键盘开", L"runas", L"cmd", L"sc config i8042prt start= auto", 0);
            if (temp.IsSucceed()) {
                lgr(L"笔记本键盘打开");
            }
        }
        else
        {
            lg(L"笔记本键盘开关状态", lgm::er);
        }
    }
    if (菜单选项ID == Tools.ID_修改屏幕分辨率)
    {
        lgc(L"ID_修改屏幕分辨率", L"菜单选项ID");
        if (Tools.修改屏幕分辨率)
        {
            //修改屏幕分辨率
            SetDisplaySize(
                std::stoi(WstringToString(Tools.设置_原屏幕分辨率宽)),
                std::stoi(WstringToString(Tools.设置_原屏幕分辨率高))
            );
            Tools.修改屏幕分辨率 = false;

            lgc((Tools.设置_原屏幕分辨率宽 + L" x " + Tools.设置_原屏幕分辨率高), L"屏幕分辨率修改成功");
        }
        else
        {
            //修改屏幕分辨率
            SetDisplaySize(
                std::stoi(WstringToString(Tools.设置_修改屏幕分辨率宽)),
                std::stoi(WstringToString(Tools.设置_修改屏幕分辨率高))
            );
            Tools.修改屏幕分辨率 = true;

            lgc((Tools.设置_修改屏幕分辨率宽 + L" x " + Tools.设置_修改屏幕分辨率高), L"屏幕分辨率修改成功");
        }
    }
    if (菜单选项ID == Tools.ID_帮助)
    {
        lgc(L"ID_帮助", L"菜单选项ID");
        lgr(L"分辨率:\t分辨率宽高最好是成比例(避免无效)\n\
\t例如: 16:9 1920x1080\n\
\t      4:3  1280x960\n\n快捷键:\
\tCtrl + Alt + F9: 修改分辨率\n\
\tCtrl + Alt + F10: 打开 Repos 文件夹\n\
\tCtrl + Alt + F11: 打开 Lib 文件夹", L"快捷键帮助");
    }
    if (菜单选项ID == Tools.ID_Ping)
    {
        lgc(L"ID_Ping", L"菜单选项ID");
        WindowShell::执行(L"Ping", L"open", L"cmd", L"/k ping -t www.baidu.com");
    }

    if (菜单选项ID == Tools.ID_退出)
    {
        lgc(L"ID_退出", L"菜单选项ID");
        PostQuitMessage(0);
    }
}

void UpdateConfig()
{
    lgcr(L"UpdateConfig()");

    Tools.设置_原屏幕分辨率宽 = Tools.工具箱配置文件.Get基本设置内容().find(L"原本屏幕分辨率宽")->second;
    lgc(L"find: 原本屏幕分辨率宽: " + Tools.设置_原屏幕分辨率宽, lgm::wr);
    Tools.设置_原屏幕分辨率高 = Tools.工具箱配置文件.Get基本设置内容().find(L"原本屏幕分辨率高")->second;
    lgc(L"find: 原本屏幕分辨率高: " + Tools.设置_原屏幕分辨率高, lgm::wr);
    Tools.设置_修改屏幕分辨率宽 = Tools.工具箱配置文件.Get基本设置内容().find(L"修改的屏幕分辨率宽")->second;
    lgc(L"find: 修改的屏幕分辨率宽: " + Tools.设置_修改屏幕分辨率宽, lgm::wr);
    Tools.设置_修改屏幕分辨率高 = Tools.工具箱配置文件.Get基本设置内容().find(L"修改的屏幕分辨率高")->second;
    lgc(L"find: 修改的屏幕分辨率高: " + Tools.设置_修改屏幕分辨率高, lgm::wr);
    Tools.设置_笔记本键盘开关状态 = Tools.工具箱配置文件.Get基本设置内容().find(L"笔记本键盘开关状态")->second;
    lgc(L"find: 笔记本键盘开关状态: " + Tools.设置_笔记本键盘开关状态, lgm::wr);
    Tools.设置_Repos = Tools.工具箱配置文件.Get基本设置内容().find(L"Repos")->second;
    lgc(L"find: 设置_Repos: " + Tools.设置_Repos, lgm::wr);
    Tools.设置_Lib = Tools.工具箱配置文件.Get基本设置内容().find(L"Lib")->second;
    lgc(L"find: 设置_Lib: " + Tools.设置_Lib, lgm::wr);
    Tools.设置_开机自启动 = Tools.工具箱配置文件.Get基本设置内容().find(L"注册表开机自启动")->second;
    lgc(L"find: 设置_开机自启动: " + Tools.设置_开机自启动, lgm::wr);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;

    POINT pt; //用于接收鼠标坐标  
    int 菜单选项ID; //用于接收菜单选项返回值  

    switch (uMsg)
    {
    case WM_CREATE: //窗口创建时候的消息.  
    {
        lgcr(L"WndProc::WM_CREATE");
        
        // NOTIFYICONDATA: 用于与任务栏通知区域（也称为系统托盘）中的图标进行交互
        Tools.nid.cbSize = sizeof(NOTIFYICONDATA);
        // hWnd: 传入的 hWnd, 否则 Shell_NotifyIcon添加托盘图标时会报错: 2147500037
        Tools.nid.hWnd = hWnd;
        Tools.nid.uID = 1;
        Tools.nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_SHOWTIP;
        Tools.nid.uCallbackMessage = WM_TRAY;
        Tools.nid.hIcon = LoadIcon(Tools.hIns, MAKEINTRESOURCE(Tools.Icon));
        if (Tools.nid.hIcon == NULL) {
            lg(L"菜单图标资源无效!", lgm::er);
        }
        lstrcpy(Tools.nid.szTip, Tools.程序_托盘名);

        Tools.hMenu = CreatePopupMenu(); //生成菜单

        //为菜单添加选项  
        AppendMenu(Tools.hMenu, MF_STRING, Tools.ID_工具箱设置, TEXT("配置"));
        //AppendMenu(Tools.hMenu, MF_STRING, Tools.ID_标签, TEXT("标签"));
        AppendMenu(Tools.hMenu, MF_SEPARATOR, NULL, TEXT("分割线"));
        AppendMenu(Tools.hMenu, MF_STRING, Tools.ID_笔记本键盘开关, TEXT("笔记本键盘开关"));
        AppendMenu(Tools.hMenu, MF_STRING, Tools.ID_修改屏幕分辨率, TEXT("修改屏幕分辨率"));
        AppendMenu(Tools.hMenu, MF_STRING, Tools.ID_Ping, TEXT("Ping"));
        AppendMenu(Tools.hMenu, MF_SEPARATOR, NULL, TEXT("分割线"));
        AppendMenu(Tools.hMenu, MF_STRING, Tools.ID_帮助, TEXT("帮助"));
        AppendMenu(Tools.hMenu, MF_STRING, Tools.ID_退出, TEXT("退出"));

        //lgc(L"Shell_NotifyIcon之前 ErrorCode:" + std::to_wstring(GetLastError()), lgm::er);
        if (!Shell_NotifyIcon(NIM_ADD, &Tools.nid)) {
            lgc(L"Shell_NotifyIcon ErrorCode:" + std::to_wstring(GetLastError()), lgm::er);
        }

        /*Uchar tempMenuStr[MAX_PATH] = L"";
        for (int i = 0; i < GetMenuItemCount(Tools.hMenu); i++) {
            if (GetMenuString(Tools.hMenu, i, tempMenuStr, sizeof(tempMenuStr) / sizeof (Uchar*), MF_BYPOSITION)) {
                lgc(L"菜单选项ID: " + Uto_string(i), lgm::wr);
                lgc(L"菜单选项字符串: " + (Ustr)tempMenuStr, lgm::wr);
            }
        }*/

        break;
    }
    case WM_TRAY: //连续使用该程序时候的消息.
    {
        switch (lParam)
        {
        case WM_RBUTTONDOWN:
        {
            //lgc(L"按下鼠标右键");
            GetCursorPos(&pt); //取鼠标坐标  
            ::SetForegroundWindow(hWnd); //解决在菜单外单击左键菜单不消失的问题  
            //EnableMenuItem(hmenu, IDR_PAUSE, MF_GRAYED); //让菜单中的某一项变灰

            菜单选项ID = TrackPopupMenu(Tools.hMenu, TPM_RIGHTBUTTON | TPM_RETURNCMD,
                pt.x, pt.y, 0, hWnd, NULL); //显示菜单并获取选项ID 
            //lgc(L"菜单选项ID: " + Uto_string(菜单选项ID), lgm::wr);
            if (菜单选项ID == 0) {
                lgc(L"TrackPopupMenu: " + std::to_wstring(GetLastError()), lgm::er);
            }

            菜单选择(菜单选项ID);
            
            break;
        }
        break;
        }
    }
    case WM_HOTKEY:
    {        
        //按键消息为: Ctrl+Alt+F19 时: 修改屏幕分辨率
        if (Tools.菜单_修改分辨率 == wParam)
        {
            lgc(L"菜单_修改分辨率", L"快捷键");
            if (Tools.修改屏幕分辨率)
            {
                //修改屏幕分辨率
                SetDisplaySize(
                    std::stoi(WstringToString(Tools.设置_原屏幕分辨率宽)),
                    std::stoi(WstringToString(Tools.设置_原屏幕分辨率高))
                );
                Tools.修改屏幕分辨率 = false;

                lgc((Tools.设置_原屏幕分辨率宽 + L" x " + Tools.设置_原屏幕分辨率高), L"屏幕分辨率修改成功");
            }
            else
            {
                //修改屏幕分辨率
                SetDisplaySize(
                    std::stoi(WstringToString(Tools.设置_修改屏幕分辨率宽)),
                    std::stoi(WstringToString(Tools.设置_修改屏幕分辨率高))
                );
                Tools.修改屏幕分辨率 = true;

                lgc((Tools.设置_修改屏幕分辨率宽 + L" x " + Tools.设置_修改屏幕分辨率高), L"屏幕分辨率修改成功");
            }
        }
        //按键消息为: Ctrl+Alt+F10 时: 打开 repos 文件夹
        if (Tools.菜单_打开Repos == wParam)
        {
            Shell消息 temp(L"Repos", (int)ShellExecute(NULL, L"explore", Tools.设置_Repos.c_str(), NULL, NULL, SW_SHOWNORMAL));
        }
        //按键消息为: Ctrl+Alt+F11 时: 打开 Lib 文件夹
        if (Tools.菜单_打开Lib == wParam)
        {
            Shell消息 temp(L"Lib", (int)ShellExecute(NULL, L"explore", Tools.设置_Lib.c_str(), NULL, NULL, SW_SHOWNORMAL));
        }

        break;
    }
    case WM_DESTROY: //窗口销毁时候的消息.  
    {
        Shell_NotifyIcon(NIM_DELETE, &Tools.nid);
        PostQuitMessage(0);
        break;
    }
    default:
        /*
        * 防止当Explorer.exe 崩溃以后，程序在系统系统托盘中的图标就消失
        *
        * 原理：Explorer.exe 重新载入后会重建系统任务栏。当系统任务栏建立的时候会向系统内所有
        * 注册接收TaskbarCreated 消息的顶级窗口发送一条消息，我们只需要捕捉这个消息，并重建系
        * 统托盘的图标即可。
        */
        if (uMsg == Tools.WM_TASKBARCREATED) {
            SendMessage(hWnd, WM_CREATE, wParam, lParam);
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

LRESULT SettingWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        //确定按钮
        Tools.确认按钮 = WindowHosting::GetHMENU();
        Tools.hWnd_确认 = CreateWindowEx(0,
            L"BUTTON", L"确认",
            WS_VISIBLE | WS_CHILD,
            600 - 180,
            400 - 100,
            60,
            40,
            hWnd, (HMENU)Tools.确认按钮, Tools.hIns, NULL);
        Tools.WinHost.添加窗口托管(L"hWnd_确认", Tools.hWnd_确认);

        //取消按钮
        Tools.取消按钮 = WindowHosting::GetHMENU();
        Tools.hWnd_取消 = CreateWindowEx(0,
            L"BUTTON", L"取消",
            WS_VISIBLE | WS_CHILD,
            600 - 100,
            400 - 100,
            60,
            40,
            hWnd, (HMENU)Tools.取消按钮, Tools.hIns, NULL);
        Tools.WinHost.添加窗口托管(L"hWnd_取消", Tools.hWnd_取消);

        break;
    }
    default:
        /*
        * 防止当Explorer.exe 崩溃以后，程序在系统系统托盘中的图标就消失
        *
        * 原理：Explorer.exe 重新载入后会重建系统任务栏。当系统任务栏建立的时候会向系统内所有
        * 注册接收TaskbarCreated 消息的顶级窗口发送一条消息，我们只需要捕捉这个消息，并重建系
        * 统托盘的图标即可。
        */
        if (uMsg == Tools.WM_TASKBARCREATED) {
            SendMessage(hWnd, WM_CREATE, wParam, lParam);
        }

        // 对于未处理的消息，默认处理
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}
#if 0
LRESULT TipsWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;

    switch (uMsg)
    {
    case WM_COMMAND:
    {
        wmId = LOWORD(wParam); //编辑控件标识符
        wmEvent = HIWORD(wParam); //编辑控件通知代码 编辑控件的句柄
        //HWND temp = lParam; //编辑控件的句柄

        switch (wmEvent)
        {
            ////更改选择时
            //case LBN_SELCHANGE:
            //{
            //    lgc(std::to_wstring(SendMessage(hwnd3_ListBox, LB_GETCURSEL , 0, 0)).c_str(), L"ListBox");
            //    lgc(std::to_wstring(SendMessage(hwnd3_ListBox2, LB_GETSELCOUNT, 0, 0)).c_str(), L"ListBox2");

            //    break;
            //}
            //对列表项左键双击
        case LBN_DBLCLK:
        {
            lgc(std::to_wstring(SendMessage(hwnd3_ListBox, LB_GETCURSEL, 0, 0)), L"ListBox");
            lgc(std::to_wstring(SendMessage(hwnd3_ListBox2, LB_GETSELCOUNT, 0, 0)), L"ListBox2");

            break;
        }
        case DL_BEGINDRAG:
        {
            lgc(std::to_wstring(SendMessage(hwnd3_ListBox, LB_GETCURSEL, 0, 0)).c_str(), L"ListBox");
            lgc(std::to_wstring(SendMessage(hwnd3_ListBox2, LB_GETSELCOUNT, 0, 0)).c_str(), L"ListBox2");

            break;
        }
        }

        //标签菜单
        if (wmId == Tools.标签菜单_Add)
        {
            SetName = false;
            lgc(L"增加", L"点击了");
            CreateChildWindows();
            SetWindowsSize();

            break;
        }
        if (wmId == Tools.标签菜单_Set)
        {
            SetName = true;
            lgc(L"修改", L"点击了");
            CreateChildWindows();
            SetWindowsSize();

            for (auto temp2 = hwndEditCount.begin(); temp2 != hwndEditCount.end(); temp2++)
            {
                if (temp2->second != NowEdit)
                {
                    lgc(std::to_wstring((int)temp2->second).c_str(), L"隐藏窗口");
                    ShowWindow(temp2->second, SW_HIDE);
                }
            }
            ShowWindow(NowEdit, SW_SHOW);
            //SendMessage(hwndTabCtrl, TCM_SETCURFOCUS, (hwndEditCount.begin())->first, 0);
            //SendMessage(NowEdit, EM_SETSEL, 0, -1); // 设置编辑控件的选区，全选文本

            break;
        }
        if (wmId == Tools.标签菜单_Del)
        {
            lgc(L"删除", L"点击了");
            auto tempCount = SendMessage(hwndTabCtrl, TCM_GETITEMCOUNT, 0, 0);
            if (tempCount > 1)
            {
                lgc(std::to_wstring(tempCount).c_str(), L"删除前的标签计数");

                int tempIndex = SendMessage(hwndTabCtrl, TCM_GETCURSEL, 0, 0);

                //找到对应的文本编辑框, 并销毁
                auto temp = hwndEditCount.find(tempIndex);
                if (temp != hwndEditCount.end())
                {
                    lgc(std::to_wstring((int)temp->second).c_str(), L"销毁 Edit 窗口");
                    DestroyWindow(temp->second);
                    SendMessage(hwndTabCtrl, TCM_DELETEITEM, tempIndex, 0);

                    hwndEditCount.erase(tempIndex);
                    lgc(std::to_wstring(tempIndex).c_str(), L"删除标签与对应的 Edit 句柄");
                }


                for (auto temp2 = hwndEditCount.begin(); temp2 != hwndEditCount.end(); temp2++)
                {
                    if (temp2->second != NowEdit)
                    {
                        lgc(std::to_wstring((int)temp2->second).c_str(), L"隐藏窗口");
                        ShowWindow(temp2->second, SW_HIDE);
                    }
                }
                ShowWindow(NowEdit, SW_SHOW);
                //SendMessage(hwndTabCtrl, TCM_SETCURFOCUS, hwndEditCount.begin()->first, 0);
                //SendMessage(NowEdit, EM_SETSEL, 0, -1); // 设置编辑控件的选区，全选文本
            }

            SetWindowsSize();
            break;
        }
        if (wmId == Tools.标签菜单_Setting)
        {
            hWnd_Setting = CreateWindowEx(NULL,
                程序_子窗口类名_设置, L"设置",
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                550,
                300,
                NULL, NULL, hIns, NULL);
            break;
        }
        if (wmId == Tools.标签菜单_Help) {
            lg(L"呼出标签快捷键: Ctrl + `/~\n\
透明标签快捷键: Shift + `/~", L"帮助");

            break;
        }
    }
    case WM_NOTIFY: //WM_NOTIFY消息中，lParam是一个NMHDR结构，code字段是具体通知消息，hwndFrom是发出通知的窗口Handle
    {
        switch (((LPNMHDR)lParam)->code)
        {
            //case TCN_SELCHANGING: //选项焦点即将改变
            //{
            //    int count = TabCtrl_GetCurSel(hwndTabCtrl);
            //    lgc(std::to_wstring(count).c_str(), L"选项焦点即将改变");

            //    ShowWindow(hwndEditCount.find(count)->second, SW_SHOWNORMAL);

            //    //ShowWindow(Page[iPage], SW_HIDE);
            //    break;
            //}
        case TCN_SELCHANGE: //选项焦点已改变
        {
            int count = TabCtrl_GetCurSel(hwndTabCtrl);
            lgc(std::to_wstring(count).c_str(), L"选项焦点已改变");

            auto temp = hwndEditCount.find(count);
            if (temp != hwndEditCount.end())
            {
                NowEdit = temp->second;
                for (auto temp2 = hwndEditCount.begin(); temp2 != hwndEditCount.end(); temp2++)
                {
                    if (temp2->second != NowEdit)
                    {
                        lgc(std::to_wstring((int)temp2->second).c_str(), L"隐藏窗口");
                        ShowWindow(temp2->second, SW_HIDE);
                    }
                }
                ShowWindow(NowEdit, SW_SHOW);
                lgc(std::to_wstring((int)temp->second).c_str(), L"显示窗口");
            }
            else
            {
                lgc(L"TCN_SELCHANGE", lgm::er);
                lgc(std::to_wstring(hwndEditCount.size()).c_str(), L"TCN_SELCHANGE");
            }

            SetWindowsSize();

            break;
        }
        case TCN_SELCHANGING://Tab改变前
        {
            int iPage = TabCtrl_GetCurSel(hwndTabCtrl);
            auto temp = hwndEditCount.find(iPage)->second;
            if (temp != NULL)
            {
                ShowWindow(temp, SW_HIDE);
            }
            return FALSE;
        }
        }
        break;
    }
    case WM_ACTIVATE:
    {
        if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) {
            Tools.标签_全选 = WindowHotkey::GetHotkey();
            // 窗口被激活，注册快捷键  
            热键注册消息 热键注册(L"Ctrl + A", RegisterHotKey(Tools.hWnd_标签, Tools.标签_全选, MOD_CONTROL, 'A'));
        }
        else {
            // 窗口失活，注销快捷键  
            UnregisterHotKey(Tools.hWnd_标签, Tools.标签_全选);
            lgc(L"Ctrl + A", L"注销");
        }
        break;
    }
    case WM_HOTKEY:
    {
        if (wParam == Tools.标签_全选) {
            // 处理Ctrl+A的快捷键  
            lgc(L"Ctrl + A", L"按下");
            SendMessage(Tools.hWnd_标签, EM_SETSEL, 0, -1);
        }
        break;
    }
    //case WM_CREATE:
    //{
    //    //RegisterHotKey(hWnd, ID_SELECT_ALL, MOD_CONTROL, L'A');

    //    break;
    //}
    case WM_SIZE:
    {
        SetWindowsSize();

        break;
    }
    case WM_DESTROY: //窗口销毁时候的消息.  
    {
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }   
return 0;
}
#endif