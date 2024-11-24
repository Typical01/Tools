//#include "pch.h"

#include "Setting.h"


void 配置初始化()
{
    lgc(_T("配置初始化()"));
    Tools.配置文件.SetShowManageLog(true);
    //先创建文件夹(否则后面的文件不能创建): \\Tools\\config
    if (WindowsSystem::CreateFolder(Tools.工具箱配置文件.Get程序父文件夹路径() + _T("\\config"))) {
        //文件不存在时，创建
        if (!Tools.配置文件.Init(Tools.工具箱配置文件.Get工具箱配置文件路径()))
        {
            std::vector<Tstr> 区域内容;
            区域内容.push_back(_T("原本屏幕分辨率宽=1920"));
            区域内容.push_back(_T("原本屏幕分辨率高=1080"));
            区域内容.push_back(_T("修改的屏幕分辨率宽=1280"));
            区域内容.push_back(_T("修改的屏幕分辨率高=1024"));
            区域内容.push_back(_T("Repos=C:\\Users\\22793\\source\\repos"));
            区域内容.push_back(_T("Lib=C:\\Typical\\ProgramProject\\C++\\Libs"));
            区域内容.push_back(_T("注册表开机自启动=否"));
            区域内容.push_back(_T("文件夹快捷键=否"));

            std::vector<Tstr> ping_baidu;
            ping_baidu.push_back(_T("菜单按键=是"));
            ping_baidu.push_back(_T("模式=打开文件"));
            ping_baidu.push_back(_T("文件=cmd"));
            ping_baidu.push_back(_T("参数=/k ping -t www.baidu.com"));
            ping_baidu.push_back(_T("显示窗口=是"));
            std::vector<Tstr> 笔记本键盘开;
            笔记本键盘开.push_back(_T("菜单按键=是"));
            笔记本键盘开.push_back(_T("模式=管理员运行"));
            笔记本键盘开.push_back(_T("文件=cmd"));
            笔记本键盘开.push_back(_T("参数=sc config i8042prt start= auto"));
            笔记本键盘开.push_back(_T("显示窗口=是"));
            std::vector<Tstr> 笔记本键盘关;
            笔记本键盘关.push_back(_T("菜单按键=是"));
            笔记本键盘关.push_back(_T("模式=管理员运行"));
            笔记本键盘关.push_back(_T("文件=cmd"));
            笔记本键盘关.push_back(_T("参数=sc config i8042prt start= disabled"));
            笔记本键盘关.push_back(_T("显示窗口=是"));

            Tools.配置文件.AddConfig_FormatSampleText();
            Tools.配置文件.AddConfig(_T("基本设置"), 区域内容);
            //Tools.配置文件.AddConfig(_T("nvidia-smi"), nv_smi);
            Tools.配置文件.AddConfig(_T("ping-baidu"), ping_baidu);
            Tools.配置文件.AddConfig(_T("笔记本键盘开"), 笔记本键盘开);
            Tools.配置文件.AddConfig(_T("笔记本键盘关"), 笔记本键盘关);
            Tools.配置文件.WriteConfigFile();

            //Tools.基本设置内容 = Tools.配置文件.GetConfig(_T("基本设置"));
            //Tools.配置文件全内容 = Tools.配置文件.GetConfigMap();
        }
        else //文件存在
        {
            //Tools.基本设置内容 = Tools.配置文件.GetConfig(_T("基本设置"));
            //Tools.配置文件全内容 = Tools.配置文件.GetConfigMap();
        }

        //Tools.配置文件.OutConfigFile_All();
        //lgc("  Tools.配置文件全内容: [" + Tto_string(Tools.配置文件.GetConfigMap().size()) + "]");
    }
}

void UpdateConfig()
{
    lgc(_T("UpdateConfig()"));

    //获取更新后的配置文件
    Tools.基本设置内容 = Tools.配置文件.GetConfig(_T("基本设置"));
    Tools.配置文件全内容 = Tools.配置文件.GetConfigMap();

    lgc(_T("find: 原本屏幕分辨率宽: ") + Tools.基本设置内容.find("原本屏幕分辨率宽")->second, lm::ts);
    lgc(_T("find: 原本屏幕分辨率高: ") + Tools.基本设置内容.find("原本屏幕分辨率高")->second, lm::ts);
    lgc(_T("find: 修改的屏幕分辨率宽: ") + Tools.基本设置内容.find("修改的屏幕分辨率宽")->second, lm::ts);
    lgc(_T("find: 修改的屏幕分辨率高: ") + Tools.基本设置内容.find("修改的屏幕分辨率高")->second, lm::ts);
    //lgc(_T("find: 笔记本键盘开关状态: ") + Tools.基本设置内容.find("笔记本键盘开关状态")->second, lm::ts);
    lgc(_T("find: 设置_Repos: ") + Tools.基本设置内容.find("Repos")->second, lm::ts);
    lgc(_T("find: 设置_Lib: ") + Tools.基本设置内容.find("Lib")->second, lm::ts);
    lgc(_T("find: 设置_开机自启动: ") + Tools.基本设置内容.find("注册表开机自启动")->second, lm::ts);
    lgc(_T("find: 设置_文件夹快捷键: ") + Tools.基本设置内容.find("文件夹快捷键")->second, lm::ts);

    //lgc("  Tools.配置文件全内容: [" + Tto_string(Tools.配置文件.GetConfigMap().size()) + "]");
}

void 初始化()
{
    lgc(_T("初始化()"));

    for (auto tempConfig = Tools.配置文件全内容.begin(); tempConfig != Tools.配置文件全内容.end(); tempConfig++) {
        const Tstr Config = tempConfig->first;
        
        //执行Shell配置
        if (Config != "基本设置") {
            auto tempConfigItem_Invalid = tempConfig->second.end(); //无效配置项
            std::map<Tstr, Tstr>::iterator tempConfigItem_Iter;

            lgc("ShellConfig: [" + Config + "]");

            Tstr 模式;
            Tstr 文件;
            Tstr 参数;
            Tstr 显示窗口;
            Tstr 菜单按键;

            //读取配置信息
            tempConfigItem_Iter = tempConfig->second.find(_T("模式"));
            if (tempConfigItem_Invalid != tempConfigItem_Iter) {
                模式 = tempConfigItem_Iter->second;
                lgc("  模式: " + 模式);
            }
            else {
                lgcr("  ShellConfig: 没有 模式" + 模式, lm::wr);
            }
            tempConfigItem_Iter = tempConfig->second.find(_T("文件"));
            if (tempConfigItem_Invalid != tempConfigItem_Iter) {
                文件 = tempConfigItem_Iter->second;
                lgc("  文件: " + 文件);
            }
            else {
                lgcr("  ShellConfig: 没有 文件" + 模式, lm::wr);
            }
            tempConfigItem_Iter = tempConfig->second.find(_T("参数"));
            if (tempConfigItem_Invalid != tempConfigItem_Iter) {
                参数 = tempConfigItem_Iter->second;
                lgc("  参数: " + 参数);
            }
            else {
                lgcr("  ShellConfig: 没有 参数" + 模式, lm::wr);
            }
            tempConfigItem_Iter = tempConfig->second.find(_T("显示窗口"));
            if (tempConfigItem_Invalid != tempConfigItem_Iter) {
                显示窗口 = tempConfigItem_Iter->second;
                lgc("  显示窗口: " + 显示窗口);
            }
            else {
                lgcr("  ShellConfig: 没有 显示窗口" + 模式, lm::wr);
            }
            tempConfigItem_Iter = tempConfig->second.find(_T("菜单按键"));
            if (tempConfigItem_Invalid != tempConfigItem_Iter) {
                菜单按键 = tempConfigItem_Iter->second;
                lgc("  菜单按键: " + 菜单按键);
            }
            else {
                lgcr("  ShellConfig: 没有 菜单按键" + 模式, lm::wr);
            }

            Tools.ShellConfig.push_back(ShellConfig(Config, 模式, 文件, 参数, 显示窗口, 菜单按键));
        }
        else {
            lgc("OtherConfig: [" + Config + "]");
        }
    }
}

int Windows程序启动项()
{
    lgc(_T("Windows程序启动项()"));

    WinSys::WindowDPI();
    if (WinSys::单实例运行(wtos(Tools.程序_窗口类名), Tools.程序_标题栏名) == 0) {
        return 0;
    }

    Tools.工具箱配置文件.初始化(Tools.程序_名);
    配置初始化();
    UpdateConfig();

    if (Tools.基本设置内容.find("注册表开机自启动")->second == _T("否")) {
        lgc(_T("工具箱注册开机自启动: 不需要"), lm::wr);
    }
    else {
        if (SetAutoRun(_T("典型一号的工具箱"), Tools.工具箱配置文件.Get程序路径().c_str())) {
            lgc(_T("典型一号的工具箱注册开机自启动 成功!"), lm::wr);
        }
        else {
            lgc(_T("典型一号的工具箱注册开机自启动 失败!"), lm::wr);
        }
    }
    初始化();

    Tools.Icon = (LPTSTR)IDI_ICON256X;
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
    Tools.wh.设置字体(Font);
    
    return 1;
}


void Windows窗口类注册()
{
    lgc(_T("Windows窗口类注册()"));

    WNDCLASSW wndclass = { 0 };
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
    Tools.wh.注册窗口类(wndclass);

    WinHost::注册进度条窗口类();
}

void Windows窗口创建()
{
    lgc(_T("Windows窗口创建()"));

    //窗口创建时会产生消息: WM_CREATE
    //要在 WM_CREATE 之前设置好菜单选项的 ID
    //按键
    Tools.ID_修改屏幕分辨率 = WindowHosting::GetHMENU();

    Tools.hWnd_托盘 = CreateWindowExW(
    // 此处使用WS_EX_TOOLWINDOW 属性来隐藏显示在任务栏上的窗口程序按钮  
        WS_EX_TOOLWINDOW,
        //0,
        Tools.程序_窗口类名, stow(Tools.程序_标题栏名).c_str(),
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL, NULL, Tools.hIns, NULL);
    Tools.wh.添加窗口托管("hWnd_托盘", Tools.hWnd_托盘);

    // 不要修改TaskbarCreated，这是系统任务栏自定义的消息  
    Tools.WM_TASKBARCREATED = RegisterWindowMessageW(L"TaskbarCreated");
}

void 菜单生成(HMENU 菜单)
{
    //为菜单添加选项  
    AppendMenuW(菜单, MF_STRING, Tools.ID_修改屏幕分辨率, _L("修改屏幕分辨率"));
    AppendMenuW(菜单, MF_SEPARATOR, NULL, _L("分割线"));

    //Shell插入位置: 菜单项总数 - 4
    Tools.ws.Shell处理(菜单, Tools.ShellConfig);
    Tools.ws.执行程序启动项Shell();

    Tools.ID_工具箱设置 = WindowHosting::GetHMENU();
    Tools.ID_帮助 = WindowHosting::GetHMENU();
    Tools.ID_退出 = WindowHosting::GetHMENU();

    AppendMenuW(菜单, MF_SEPARATOR, NULL, _L("分割线"));
    AppendMenuW(菜单, MF_STRING, Tools.ID_工具箱设置, _L("配置"));
    AppendMenuW(菜单, MF_STRING, Tools.ID_帮助, _L("帮助"));
    AppendMenuW(菜单, MF_STRING, Tools.ID_退出, _L("退出"));

    Tools.菜单_修改分辨率 = WindowHotkey::GetHotkey();
    Tools.菜单_打开Repos = WindowHotkey::GetHotkey();
    Tools.菜单_打开Lib = WindowHotkey::GetHotkey();

    热键注册消息 热键注册_修改分辨率("Ctrl + Alt + F9", RegisterHotKey(Tools.hWnd_托盘, Tools.菜单_修改分辨率, MOD_CONTROL | MOD_ALT, VK_F9));
    热键注册消息 热键注册_打开Repos("Ctrl + Alt + F10", RegisterHotKey(Tools.hWnd_托盘, Tools.菜单_打开Repos, MOD_CONTROL | MOD_ALT, VK_F10));
    热键注册消息 热键注册_打开Lib("Ctrl + Alt + F11", RegisterHotKey(Tools.hWnd_托盘, Tools.菜单_打开Lib, MOD_CONTROL | MOD_ALT, VK_F11));
}

void 菜单选择(int 菜单选项ID)
{
    if (菜单选项ID == Tools.ID_工具箱设置)
    {
        lgc(_T("菜单选项ID: ID_工具箱设置 Path: " + Tools.工具箱配置文件.Get工具箱配置文件路径()));

        Tools.ws.ExecuteAnalyze("打开配置文件", "打开文件", Tools.工具箱配置文件.Get工具箱配置文件路径());
        lgr(_T("修改后需要重启程序!"));
    }
    else if (菜单选项ID == Tools.ID_修改屏幕分辨率)
    {
        lgc(_T("菜单选项ID: ID_修改屏幕分辨率"));
        if (Tools.修改屏幕分辨率)
        {
            //修改屏幕分辨率
            SetDisplaySize(
                std::stoi(Tools.基本设置内容.find("原本屏幕分辨率宽")->second),
                std::stoi(Tools.基本设置内容.find("原本屏幕分辨率高")->second)
            );
            Tools.修改屏幕分辨率 = false;

            lgc((Tools.基本设置内容.find("原本屏幕分辨率宽")->second + _T(" x ") + Tools.基本设置内容.find("原本屏幕分辨率高")->second), _T("屏幕分辨率修改成功"));
        }
        else
        {
            //修改屏幕分辨率
            SetDisplaySize(
                std::stoi(Tools.基本设置内容.find("修改的屏幕分辨率宽")->second),
                std::stoi(Tools.基本设置内容.find("修改的屏幕分辨率高")->second)
            );
            Tools.修改屏幕分辨率 = true;

            lgc((Tools.基本设置内容.find("修改的屏幕分辨率宽")->second + _T(" x ") + Tools.基本设置内容.find("修改的屏幕分辨率高")->second), _T("屏幕分辨率修改成功"));
        }
    }
    else if (菜单选项ID == Tools.ID_帮助)
    {
        lgc(_T("菜单选项ID: ID_帮助"));
        MessageBoxW(NULL, _L("分辨率:\t分辨率宽高最好是成比例(避免无效)\n\
\t例如:\n\
\t16:9 1920x1080\n\
\t4:3  1280x960\n\
\n快捷键:\tCtrl + Alt + F9: 修改分辨率\n\
\t需要在配置文件中修改: 文件夹快捷键=是\n\
\t  Ctrl + Alt + F10: 打开 Repos 文件夹\n\
\t  Ctrl + Alt + F11: 打开 Lib 文件夹\n\
"), _L("快捷键帮助"), NULL);
    }

    else if (菜单选项ID == Tools.ID_退出)
    {
        lgc(_T("菜单选项ID: ID_退出"));
        PostQuitMessage(0);
    }
    else {
        Tools.ws.执行程序菜单项Shell(菜单选项ID);
    }
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
        lgc(_T("WndProc::WM_CREATE"));
        
        // NOTIFYICONDATA: 用于与任务栏通知区域（也称为系统托盘）中的图标进行交互
        Tools.nid.cbSize = sizeof(NOTIFYICONDATA);
        // hWnd: 传入的 hWnd, 否则 Shell_NotifyIcon添加托盘图标时会报错: 2147500037
        Tools.nid.hWnd = hWnd;
        Tools.nid.uID = 1;
        Tools.nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_SHOWTIP;
        Tools.nid.uCallbackMessage = WM_TRAY;
        Tools.nid.hIcon = LoadIcon(Tools.hIns, MAKEINTRESOURCE(Tools.Icon));
        if (Tools.nid.hIcon == NULL) {
            lg(_T("菜单图标资源无效!"), lm::er);
        }
        lstrcpy(Tools.nid.szTip, Tools.程序_托盘名.c_str());

        Tools.hMenu = CreatePopupMenu(); //生成菜单

        菜单生成(Tools.hMenu);

        //lgc(_T("Shell_NotifyIcon之前 ErrorCode:") + Tto_string(GetLastError()), lm::er);
        if (!Shell_NotifyIcon(NIM_ADD, &Tools.nid)) {
            lgc("Shell_NotifyIcon ErrorCode:" + Tto_string(GetLastError()), lm::er);
        }

        /*Tchar tempMenuStr[MAX_PATH] = _T("");
        for (int i = 0; i < GetMenuItemCount(Tools.hMenu); i++) {
            if (GetMenuString(Tools.hMenu, i, tempMenuStr, sizeof(tempMenuStr) / sizeof (Tchar*), MF_BYPOSITION)) {
                lgc(_T("菜单选项ID: ") + Tto_string(i), lm::wr);
                lgc(_T("菜单选项字符串: ") + (Tstr)tempMenuStr, lm::wr);
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
            //lgc(_T("按下鼠标右键"));
            GetCursorPos(&pt); //取鼠标坐标  
            ::SetForegroundWindow(hWnd); //解决在菜单外单击左键菜单不消失的问题  
            //EnableMenuItem(hmenu, IDR_PAUSE, MF_GRAYED); //让菜单中的某一项变灰

            菜单选项ID = TrackPopupMenu(Tools.hMenu, TPM_RIGHTBUTTON | TPM_RETURNCMD,
                pt.x, pt.y, 0, hWnd, NULL); //显示菜单并获取选项ID 
            //lgc(_T("菜单选项ID: ") + Tto_string(菜单选项ID), lm::wr);
            if (菜单选项ID == 0) {
                lgc(_T("TrackPopupMenu: ") + Tto_string(GetLastError()), lm::er);
            }

            菜单选择(菜单选项ID);
            
            break;
        }
        break;
        }
    }
    case WM_HOTKEY:
    {        
        //按键消息为: Ctrl+Alt+F9 时: 修改屏幕分辨率
        if (Tools.菜单_修改分辨率 == wParam)
        {
            lgc(_T("快捷键: 修改分辨率"));
            if (Tools.修改屏幕分辨率)
            {
                //修改屏幕分辨率
                SetDisplaySize(
                    std::stoi(Tools.基本设置内容.find("原本屏幕分辨率宽")->second),
                    std::stoi(Tools.基本设置内容.find("原本屏幕分辨率高")->second)
                );
                Tools.修改屏幕分辨率 = false;

                lgc((Tools.基本设置内容.find("原本屏幕分辨率宽")->second + _T(" x ") + Tools.基本设置内容.find("原本屏幕分辨率高")->second), _T("屏幕分辨率修改成功"));
            }
            else
            {
                //修改屏幕分辨率
                SetDisplaySize(
                    std::stoi(Tools.基本设置内容.find("修改的屏幕分辨率宽")->second),
                    std::stoi(Tools.基本设置内容.find("修改的屏幕分辨率高")->second)
                );
                Tools.修改屏幕分辨率 = true;

                lgc((Tools.基本设置内容.find("修改的屏幕分辨率宽")->second + _T(" x ") + Tools.基本设置内容.find("修改的屏幕分辨率高")->second), _T("屏幕分辨率修改成功"));
            }
        }
        //按键消息为: Ctrl+Alt+F10 时: 打开 repos 文件夹
        if (Tools.菜单_打开Repos == wParam)
        {
            lgc(_T("快捷键: 打开Repos"));
            if (Tools.基本设置内容.find("文件夹快捷键")->second == "是") {
                Shell消息 temp(_T("Repos"), (int)ShellExecute(NULL, _T("explore"), Tools.基本设置内容.find("Repos")->second.c_str(), NULL, NULL, SW_SHOWNORMAL));
            }
        }
        //按键消息为: Ctrl+Alt+F11 时: 打开 Lib 文件夹
        if (Tools.菜单_打开Lib == wParam)
        {
            lgc(_T("快捷键: 打开Lib"));
            if (Tools.基本设置内容.find("文件夹快捷键")->second == "是") {
                Shell消息 temp(_T("Lib"), (int)ShellExecute(NULL, _T("explore"), Tools.基本设置内容.find("Lib")->second.c_str(), NULL, NULL, SW_SHOWNORMAL));
            }
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
