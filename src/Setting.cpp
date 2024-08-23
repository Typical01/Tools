//#include "pch.h"

#include "Setting.h"


using Tools_Tool::WindowsSystem::WindowHosting;


//Settings Tools;

int Windows程序启动项()
{
    lgcr(L"Windows程序启动项()");

    WindowHosting::WindowDPI();
    if (WindowHosting::单实例运行(Tools.程序_窗口类名, Tools.程序_标题栏名) == 0) {
        return 0;
    }

    Tools.工具箱配置文件.初始化(Tools.程序_名);
    打开配置文件();
    UpdateConfig();

    if (Tools.设置_开机自启动 == L"否") {
        //开机自启动 设置修改为开关, 而不是状态(根据当前状态: 否为没有注册, 则注册并改为是

        //if (SetAutoRun(L"典型一号的工具箱", Tools.工具箱配置文件.Get程序路径().c_str())) {

        //    //通过 配置文件对象进行修改
        //    if (Tools.基本设置内容.find(L"注册表开机自启动") != Tools.基本设置内容.cend()) {
        //        //注册开机自启后, 修改配置文件中的设置项
        //        Tools.配置文件.修改区域(L"基本设置", L"注册表开机自启动", L"是");
        //        UpdateConfig();
        //        lgc(Tools.基本设置内容.find(L"注册表开机自启动")->first + L"=" + Tools.基本设置内容.find(L"注册表开机自启动")->second, lgm::wr);
        //    }

        //    lgc(L"注册表修改后的配置文件");
        //    Tools.配置文件.写入文件();
        //    lgr(L"注册开机自启动: 典型一号的工具箱");
        //}
    }
    else if (Tools.设置_开机自启动 == L"是") { //
        if (SetAutoRun(L"典型一号的工具箱", Tools.工具箱配置文件.Get程序路径().c_str())) {
            lgc(L"典型一号的工具箱注册开机自启动 成功!", lgm::wr);
        }
        else {
            lgc(L"典型一号的工具箱注册开机自启动 失败!", lgm::er);
        }
    }
    初始化();

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
    Tools.wh.设置字体(Font);
    
    return 1;
}


void Windows窗口类注册()
{
    lgcr(L"Windows窗口类注册()");

    WNDCLASS wndclass = { 0 };
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

    WNDCLASS wndclass_标签 = { 0 };
    wndclass_标签.style = CS_HREDRAW | CS_VREDRAW;
    wndclass_标签.lpfnWndProc = TipsWndProc;
    wndclass_标签.cbClsExtra = 0;
    wndclass_标签.cbWndExtra = 0;
    wndclass_标签.hInstance = Tools.hIns;
    wndclass_标签.hIcon = LoadIcon(Tools.hIns, Tools.Icon);
    wndclass_标签.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass_标签.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass_标签.lpszMenuName = NULL;
    wndclass_标签.lpszClassName = Tools.程序_标签窗口类名;
    Tools.wh.注册窗口类(wndclass_标签);

    WinHost::注册进度条窗口类();

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
    Tools.wh.添加窗口托管(L"hWnd_托盘", Tools.hWnd_托盘);

    Tools.we.创建标签窗口(Tools.程序_标签窗口类名, Tools.wh);

    // 不要修改TaskbarCreated，这是系统任务栏自定义的消息  
    Tools.WM_TASKBARCREATED = RegisterWindowMessage(L"TaskbarCreated");
}

void 初始化()
{
    lgcr(L"初始化()");

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
    Tools.标签_打开 = WindowHotkey::GetHotkey();
    //Tools.标签_切换状态 = WindowHotkey::GetHotkey();

    热键注册消息 热键注册_修改分辨率(L"Ctrl + Alt + F9", RegisterHotKey(Tools.hWnd_托盘, Tools.菜单_修改分辨率, MOD_CONTROL | MOD_ALT, VK_F9));
    热键注册消息 热键注册_打开Repos(L"Ctrl + Alt + F10", RegisterHotKey(Tools.hWnd_托盘, Tools.菜单_打开Repos, MOD_CONTROL | MOD_ALT, VK_F10));
    热键注册消息 热键注册_打开Lib(L"Ctrl + Alt + F11", RegisterHotKey(Tools.hWnd_托盘, Tools.菜单_打开Lib, MOD_CONTROL | MOD_ALT, VK_F11));
    热键注册消息 热键注册_打开标签(L"Ctrl + ~", RegisterHotKey(Tools.hWnd_托盘, Tools.标签_打开, MOD_CONTROL, VK_OEM_3));
    //热键注册消息 热键注册_标签(L"Ctrl + `", RegisterHotKey(Tools.hWnd_托盘, Tools.标签_切换状态, MOD_CONTROL, VK_OEM_3));

    std::vector<ShellConfig> shellConfig;
    auto temp配置全文件内容 = Tools.配置文件.Get配置文件全内容();
    for (auto temp区域设置 = temp配置全文件内容.cbegin(); temp区域设置 != temp配置全文件内容.cend(); temp区域设置++) {
        const Ustr 区域名称 = temp区域设置->first;
        auto temp无效配置 = temp区域设置->second.cend();
        std::map<Ustr, Ustr>::const_iterator temp迭代器;

        //执行Shell配置
        if (区域名称 != L"基本设置" && 区域名称 != L"标签") {
            Ustr temp程序启动时运行;
            Ustr 模式;
            Ustr 文件;
            Ustr 参数;
            Ustr temp显示窗口;

            temp迭代器 = temp区域设置->second.find(L"程序启动时运行");
            if (temp无效配置 != temp迭代器) {
                temp程序启动时运行 = temp迭代器->second;
                lgc();
            }
            temp迭代器 = temp区域设置->second.find(L"模式");
            if (temp无效配置 != temp迭代器) {
                模式 = temp迭代器->second;
            }
            temp迭代器 = temp区域设置->second.find(L"文件");
            if (temp无效配置 != temp迭代器) {
                文件 = temp迭代器->second;
            }
            temp迭代器 = temp区域设置->second.find(L"参数");
            if (temp无效配置 != temp迭代器) {
                参数 = temp迭代器->second;
            }
            temp迭代器 = temp区域设置->second.find(L"显示窗口");
            if (temp无效配置 != temp迭代器) {
                temp显示窗口 = temp迭代器->second;
            }

            bool 程序启动时运行 = false;
            if (temp程序启动时运行 == L"是") {
                程序启动时运行 = true;
            }
            bool 显示窗口 = false;
            if (temp显示窗口 == L"是") {
                显示窗口 = true;
            }

            ShellConfig tempShellConfig(区域名称, 程序启动时运行, 模式, 文件, 参数, 显示窗口);
            shellConfig.push_back(tempShellConfig);
        }
    }
    Tools.ws.Shell处理(Tools.hMenu, shellConfig);

    //标签
    Tools.we.Init(Tools.配置文件);
}

void 打开配置文件()
{
    //先创建文件夹(否则后面的文件不能创建): \\Tools\\config
    if (WindowsSystem::CreateFolder(Tools.工具箱配置文件.Get程序父文件夹路径() + L"\\config")) {
        //文件不存在时，创建
        if (!Tools.配置文件.Init(Tools.工具箱配置文件.Get工具箱配置文件路径()))
        {
            std::vector<Ustr> 区域内容;
            区域内容.push_back(L"原本屏幕分辨率宽=1920");
            区域内容.push_back(L"原本屏幕分辨率高=1080");
            区域内容.push_back(L"修改的屏幕分辨率宽=1280");
            区域内容.push_back(L"修改的屏幕分辨率高=1024");
            区域内容.push_back(L"Repos=C:\\Users\\22793\\source\\repos");
            区域内容.push_back(L"Lib=C:\\Typical\\ProgramProject\\C++\\Libs");
            区域内容.push_back(L"笔记本键盘开关状态=开");
            区域内容.push_back(L"注册表开机自启动=否");

            /*std::vector<Ustr> 标签区域内容;
            标签区域内容.push_back(L"标签1=" + Tools.工具箱配置文件.Get程序父文件夹路径() + L"\\config" + L"\\标签1.txt");*/

            std::vector<Ustr> nv_smi;
            nv_smi.push_back(L"程序启动时运行=否");
            nv_smi.push_back(L"模式=管理员运行");
            nv_smi.push_back(L"文件=cmd");
            nv_smi.push_back(L"参数=nvidia-smi -lgc 1080");
            nv_smi.push_back(L"显示窗口=否");
            std::vector<Ustr> ping_baidu;
            ping_baidu.push_back(L"程序启动时运行=否");
            ping_baidu.push_back(L"模式=打开文件");
            ping_baidu.push_back(L"文件=cmd");
            ping_baidu.push_back(L"参数=/k ping -t www.baidu.com");
            ping_baidu.push_back(L"显示窗口=是");

            Tools.配置文件.添加区域(L"基本设置", 区域内容);
            /*Tools.配置文件.添加区域(L"标签", 标签区域内容);*/
            Tools.配置文件.添加区域(L"nvidia-smi 1080", nv_smi);
            Tools.配置文件.添加区域(L"ping baidu", ping_baidu);
            Tools.配置文件.写入文件();

            Tools.基本设置内容 = Tools.配置文件.Get指定区域内容(L"基本设置");
            Tools.配置文件全内容 = Tools.配置文件.Get配置文件全内容();
        }
        else //文件存在
        {
            Tools.基本设置内容 = Tools.配置文件.Get指定区域内容(L"基本设置");
            Tools.配置文件全内容 = Tools.配置文件.Get配置文件全内容();
        }
    }
}

void 菜单生成(HMENU 菜单)
{
    //为菜单添加选项  
    AppendMenu(菜单, MF_STRING, Tools.ID_标签, TEXT("标签"));
    AppendMenu(菜单, MF_SEPARATOR, NULL, TEXT("分割线"));
    AppendMenu(菜单, MF_STRING, Tools.ID_笔记本键盘开关, TEXT("笔记本键盘开关"));
    AppendMenu(菜单, MF_STRING, Tools.ID_修改屏幕分辨率, TEXT("修改屏幕分辨率"));
    //AppendMenu(菜单, MF_STRING, Tools.ID_Ping, TEXT("Ping"));
    AppendMenu(菜单, MF_SEPARATOR, NULL, TEXT("分割线"));
    AppendMenu(菜单, MF_STRING, Tools.ID_工具箱设置, TEXT("配置"));
    AppendMenu(菜单, MF_STRING, Tools.ID_帮助, TEXT("帮助"));
    AppendMenu(菜单, MF_STRING, Tools.ID_退出, TEXT("退出"));
}

void 菜单选择(int 菜单选项ID)
{
    if (菜单选项ID == Tools.ID_工具箱设置)
    {
        lgc(L"ID_工具箱设置", L"菜单选项ID");

        Tools.工具箱配置文件.打开配置文件();
        lgr(L"修改后需要重启程序!");
    }
    if (菜单选项ID == Tools.ID_标签)
    {
        lgc(L"ID_标签", L"菜单选项ID");

        Tools.we.显示标签窗口(true);
    }
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
\tCtrl + Alt + F11: 打开 Lib 文件夹\n\
\tCtrl + ~: 打开/关闭标签", L"快捷键帮助");
    }
    /*if (菜单选项ID == Tools.ID_Ping)
    {
        lgc(L"ID_Ping", L"菜单选项ID");
        WindowShell::执行(L"Ping", L"open", L"cmd", L"/k ping -t www.baidu.com");
    }*/

    if (菜单选项ID == Tools.ID_退出)
    {
        lgc(L"ID_退出", L"菜单选项ID");
        PostQuitMessage(0);
    }
}

void UpdateConfig()
{
    lgcr(L"UpdateConfig()");

    Tools.基本设置内容 = Tools.配置文件.Get指定区域内容(L"基本设置");
    Tools.配置文件全内容 = Tools.配置文件.Get配置文件全内容();

    Tools.设置_原屏幕分辨率宽 = Tools.基本设置内容.find(L"原本屏幕分辨率宽")->second;
    lgc(L"find: 原本屏幕分辨率宽: " + Tools.设置_原屏幕分辨率宽, lgm::wr);
    Tools.设置_原屏幕分辨率高 = Tools.基本设置内容.find(L"原本屏幕分辨率高")->second;
    lgc(L"find: 原本屏幕分辨率高: " + Tools.设置_原屏幕分辨率高, lgm::wr);
    Tools.设置_修改屏幕分辨率宽 = Tools.基本设置内容.find(L"修改的屏幕分辨率宽")->second;
    lgc(L"find: 修改的屏幕分辨率宽: " + Tools.设置_修改屏幕分辨率宽, lgm::wr);
    Tools.设置_修改屏幕分辨率高 = Tools.基本设置内容.find(L"修改的屏幕分辨率高")->second;
    lgc(L"find: 修改的屏幕分辨率高: " + Tools.设置_修改屏幕分辨率高, lgm::wr);
    Tools.设置_笔记本键盘开关状态 = Tools.基本设置内容.find(L"笔记本键盘开关状态")->second;
    lgc(L"find: 笔记本键盘开关状态: " + Tools.设置_笔记本键盘开关状态, lgm::wr);
    Tools.设置_Repos = Tools.基本设置内容.find(L"Repos")->second;
    lgc(L"find: 设置_Repos: " + Tools.设置_Repos, lgm::wr);
    Tools.设置_Lib = Tools.基本设置内容.find(L"Lib")->second;
    lgc(L"find: 设置_Lib: " + Tools.设置_Lib, lgm::wr);
    Tools.设置_开机自启动 = Tools.基本设置内容.find(L"注册表开机自启动")->second;
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

        菜单生成(Tools.hMenu);

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
            Tools.ws.程序菜单项Shell(菜单选项ID);
            
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
        if (Tools.标签_打开 == wParam) {
            Tools.we.显示标签窗口(true);
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

        //标签菜单
        if (wmId == Tools.we.标签菜单_Add)
        {
            lgc(L"增加", L"点击了");
            Tools.标签_是否修改 = false;
            Tools.we.显示命名窗口(true);

            break;
        }
        if (wmId == Tools.we.标签菜单_Set)
        {
            lgc(L"修改", L"点击了");
            Tools.标签_是否修改 = true;
            Tools.we.显示命名窗口(true);

            break;
        }
        if (wmId == Tools.we.标签菜单_Del)
        {
            lgc(L"删除", L"点击了");
            Tools.we.删除标签();

            break;
        }
        if (wmId == Tools.we.标签菜单_Help) {
            lg(L"呼出标签快捷键: Ctrl + `/~\n\
透明标签快捷键: Shift + `/~", L"帮助");

            break;
        }
        if (wmId == Tools.we.标签菜单_Save) {
            Tools.we.标签内容保存(Tools.工具箱配置文件, Tools.配置文件);

            break;
        }

        if (wmId == Tools.we.标签命名_确认按键) {
            if (Tools.标签_是否修改) { //需要修改
                Tools.we.修改标签();
            }
            else { //不需要, 添加
                Tools.we.添加标签();
            }
            Tools.we.显示命名窗口(false);
            Tools.we.标签窗口刷新();

            break;
        }

        break;
    }
    case WM_NOTIFY: //WM_NOTIFY消息中，lParam是一个NMHDR结构，code字段是具体通知消息，hwndFrom是发出通知的窗口Handle
    {
        switch (((LPNMHDR)lParam)->code)
        {
        //case TCN_SELCHANGING: //选项焦点即将改变
        //{
        //    break;
        //}
        case TCN_SELCHANGE: //选项焦点已改变
        {
            lgc(L"选项已改变");
            //Timers::sleep(2);

            Tools.we.显示标签内容(Tools.we.Get标签名(Tools.we.Get当前标签选项()));
            Tools.we.标签窗口刷新();

            break;
        }
        }
        break;
    }
    //case WM_ACTIVATE:
    //{
    //    if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) {
    //        Tools.标签_全选 = WindowHotkey::GetHotkey();
    //        Tools.标签_保存 = WindowHotkey::GetHotkey();
    //        // 窗口被激活，注册快捷键  
    //        热键注册消息 热键注册_全选(L"Ctrl + A", RegisterHotKey(Tools.hWnd_标签, Tools.标签_全选, MOD_CONTROL, 'A'));
    //        热键注册消息 热键注册_保存(L"Ctrl + S", RegisterHotKey(Tools.hWnd_标签, Tools.标签_保存, MOD_CONTROL, 'S'));
    //    }
    //    else {
    //        // 窗口失活，注销快捷键  
    //        UnregisterHotKey(Tools.hWnd_标签, Tools.标签_全选);
    //        lgc(L"Ctrl + A", L"注销");
    //        UnregisterHotKey(Tools.hWnd_标签, Tools.标签_保存);
    //        lgc(L"Ctrl + S", L"注销");
    //    }
    //    break;
    //}
    //case WM_HOTKEY:
    //{
    //    if (wParam == Tools.标签_全选) {
    //        // 处理Ctrl+A的快捷键  
    //        lgc(L"Ctrl + A", L"按下");
    //        SendMessage(Tools.hWnd_标签, EM_SETSEL, 0, -1);
    //    }
    //    else if (wParam == Tools.标签_保存) {
    //        // 处理Ctrl+S的快捷键  
    //        lgc(L"Ctrl + S", L"按下");
    //        Tools.we.标签内容保存(Tools.工具箱配置文件, Tools.配置文件);
    //    }

    //    if (wParam == Tools.标签_切换状态) {

    //        EnableWindow(hWnd_透明标签, FALSE); // 禁用窗口
    //    }

    //    break;
    //}
    case WM_SIZE:
    {
        Tools.we.标签窗口刷新();

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