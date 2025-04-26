//#include "pch.h"

#include "Setting.h"


void SelfStarting()
{
    Tstr ExePathName;
    if (Win::GetExePathName(ExePathName)) {
        if (Tools.BaseConfigItem[TEXT("注册表开机自启动")] != TEXT("是")) {
            lgc(TEXT("注册开机自启动: 否"), Tip);

            if (Win::SetSelfStarting((Tstr)TEXT("Typical_Tools"), Printf(TEXT("\"%s\""), ExePathName).str(), TEXT(""), false)) {
                lgc(TEXT("注册开机自启动: 删除成功!"), Tip);
            }
            else {
                lgcr(TEXT("注册开机自启动: 删除失败!"), Err);
            }
        }
        else {
            lgc(TEXT("注册开机自启动: 是"), Tip);
            if (Win::SetSelfStarting((Tstr)TEXT("Typical_Tools"), Printf(TEXT("\"%s\""), ExePathName).str(), TEXT(""), true)) {
                lgc(TEXT("注册开机自启动: 添加成功!"), Tip);
            }
            else {
                lgcr(TEXT("注册开机自启动: 添加失败!"), Err);
            }
        }
    }
}

int WindowsExeAutoRunItem()
{
    lgc(TEXT("WindowsExeAutoRunItem()"));

    Win::WindowDPI();
    if (Win::AloneRun(Tools.ToolsWindowClassName, Tools.ExeTitleName) == 0) {
        return 0;
    }

    LoadBaseConfig();
    LoadShellConfig();

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
    Tools.WindowHost.SetFont(Font);

    return 1;
}

void LoadBaseConfig(bool _bReLoad)
{
    if (_bReLoad) {
        lgc(Tip, TEXT("LoadBaseConfig() ReLoad"));
        Tools.ConfigFile.Clear();
    }
    else {
        lgc(TEXT("LoadBaseConfig()"));
    }

    //先创建文件夹(否则后面的文件不能创建): \\Tools\\Config
    Tstr ConfigDirectory = Printf(TEXT("%s\\Config"), Tools.ExeCurrentPath);
    Tools.ExeConfigFilePath = Printf(TEXT("%s%s"), ConfigDirectory, TEXT("\\Config.ini"));
    Tools.FileSystem.SetPath(ConfigDirectory);
    if (Tools.FileSystem.CreateDirectorys()) {
        //文件不存在时，创建
#ifdef _DEBUG
        Tools.ConfigFile.SetShowManageLog(true);
#endif
        Tools.ConfigFile.SetShowManageLog(true);
        if (!Tools.ConfigFile.Init(Printf(TEXT("%s"), Tools.ExeConfigFilePath))) {
            lg(Printf(TEXT("配置文件初始化: [%s]"), Tools.ConfigFile.GetConfigFilePath()));

            std::vector<Tstr> BaseConfig;
            BaseConfig.push_back(TEXT("默认的屏幕分辨率宽=1920"));
            BaseConfig.push_back(TEXT("默认的屏幕分辨率高=1080"));
            BaseConfig.push_back(TEXT("修改的屏幕分辨率宽=1280"));
            BaseConfig.push_back(TEXT("修改的屏幕分辨率高=1024"));
            BaseConfig.push_back(TEXT("注册表开机自启动=否"));
            //BaseConfig.push_back(TEXT("文件夹快捷键=否"));

            std::vector<Tstr> ping_baidu;
            ping_baidu.push_back(TEXT("菜单按键=是"));
            ping_baidu.push_back(TEXT("模式=打开文件"));
            ping_baidu.push_back(TEXT("文件=cmd"));
            ping_baidu.push_back(TEXT("参数=/k ping -t www.baidu.com"));
            ping_baidu.push_back(TEXT("显示窗口=是"));
            std::vector<Tstr> BookKeyOn;
            BookKeyOn.push_back(TEXT("菜单按键=是"));
            BookKeyOn.push_back(TEXT("模式=管理员运行"));
            BookKeyOn.push_back(TEXT("文件=cmd"));
            BookKeyOn.push_back(TEXT("参数=/k sc config i8042prt start= auto"));
            BookKeyOn.push_back(TEXT("显示窗口=是"));
            std::vector<Tstr> BookKeyOff;
            BookKeyOff.push_back(TEXT("菜单按键=是"));
            BookKeyOff.push_back(TEXT("模式=管理员运行"));
            BookKeyOff.push_back(TEXT("文件=cmd"));
            BookKeyOff.push_back(TEXT("参数=/k sc config i8042prt start= disabled"));
            BookKeyOff.push_back(TEXT("显示窗口=是"));

            //配置文件: 注释未完善
            //Tools.ConfigFile.AddConfig_PrintfSampleText();
            Tools.ConfigFile.AddConfig(TEXT("基本设置"), BaseConfig);
            //Tools.ConfigFile.AddConfig(TEXT("nvidia-smi"), nv_smi);
            Tools.ConfigFile.AddConfig(TEXT("ping-baidu"), ping_baidu);
            Tools.ConfigFile.AddConfig(TEXT("笔记本键盘开"), BookKeyOn);
            Tools.ConfigFile.AddConfig(TEXT("笔记本键盘关"), BookKeyOff);
            Tools.ConfigFile.WriteConfigFile();

        }
    }
    else {
#ifdef _DEBUG
        Tools.ConfigFile.SetShowManageLog(true);
#endif
        Tools.ConfigFile.SetShowManageLog(true);
        if (!Tools.ConfigFile.Init(Printf(TEXT("%s"), Tools.ExeConfigFilePath))) {
            lg(Printf(TEXT("配置文件初始化: [%s]"), Tools.ConfigFile.GetConfigFilePath()));

            std::vector<Tstr> BaseConfig;
            BaseConfig.push_back(TEXT("默认的屏幕分辨率宽=1920"));
            BaseConfig.push_back(TEXT("默认的屏幕分辨率高=1080"));
            BaseConfig.push_back(TEXT("修改的屏幕分辨率宽=1280"));
            BaseConfig.push_back(TEXT("修改的屏幕分辨率高=1024"));
            BaseConfig.push_back(TEXT("注册表开机自启动=否"));
            //BaseConfig.push_back(TEXT("文件夹快捷键=否"));

            std::vector<Tstr> ping_baidu;
            ping_baidu.push_back(TEXT("菜单按键=是"));
            ping_baidu.push_back(TEXT("模式=打开文件"));
            ping_baidu.push_back(TEXT("文件=cmd"));
            ping_baidu.push_back(TEXT("参数=/k ping -t www.baidu.com"));
            ping_baidu.push_back(TEXT("显示窗口=是"));
            std::vector<Tstr> BookKeyOn;
            BookKeyOn.push_back(TEXT("菜单按键=是"));
            BookKeyOn.push_back(TEXT("模式=管理员运行"));
            BookKeyOn.push_back(TEXT("文件=cmd"));
            BookKeyOn.push_back(TEXT("参数=/k sc config i8042prt start= auto"));
            BookKeyOn.push_back(TEXT("显示窗口=是"));
            std::vector<Tstr> BookKeyOff;
            BookKeyOff.push_back(TEXT("菜单按键=是"));
            BookKeyOff.push_back(TEXT("模式=管理员运行"));
            BookKeyOff.push_back(TEXT("文件=cmd"));
            BookKeyOff.push_back(TEXT("参数=/k sc config i8042prt start= disabled"));
            BookKeyOff.push_back(TEXT("显示窗口=是"));

            //Tools.ConfigFile.AddConfig_PrintfSampleText();
            Tools.ConfigFile.AddConfig(TEXT("基本设置"), BaseConfig);
            //Tools.ConfigFile.AddConfig(TEXT("nvidia-smi"), nv_smi);
            Tools.ConfigFile.AddConfig(TEXT("ping-baidu"), ping_baidu);
            Tools.ConfigFile.AddConfig(TEXT("笔记本键盘开"), BookKeyOn);
            Tools.ConfigFile.AddConfig(TEXT("笔记本键盘关"), BookKeyOff);
            Tools.ConfigFile.WriteConfigFile();
        }

#ifdef _DEBUG
        Tools.ConfigFile.SetShowManageLog(true);
#endif
        if (!Tools.ConfigFile.Init(Printf(TEXT("%s"), Tools.ExeConfigFilePath))) {
            lgcr(Err, Printf(TEXT("读取文件[%s]失败!"), Tools.ExeConfigFilePath));
        }
        else {
            lgc(Tip, Printf(TEXT("读取文件[%s]成功!"), Tools.ExeConfigFilePath));
        }
    }

    UpdateConfig();
}

void UpdateConfig()
{
    if (!Tools.ConfigFile_AllConfig.empty()) { //修改配置后, 重新加载
        lgc(Tip, TEXT("UpdateConfig() ReLoad"));
        Tools.ConfigFile_AllConfig.clear();
        Tools.BaseConfigItem.clear();
    }
    else {
        lgc(TEXT("UpdateConfig()"));
    }

    //获取更新后的ConfigFile
    Tools.ConfigFile_AllConfig = Tools.ConfigFile.GetConfigMap();
    Tools.BaseConfigItem = Tools.ConfigFile.GetConfigItem(TEXT("基本设置"));

    lgc(Tip, Printf(TEXT("  默认的屏幕分辨率宽: %s"), Tools.BaseConfigItem[TEXT("默认的屏幕分辨率宽")]));
    lgc(Tip, Printf(TEXT("  默认的屏幕分辨率高: %s"), Tools.BaseConfigItem[TEXT("默认的屏幕分辨率高")]));
    lgc(Tip, Printf(TEXT("  修改的屏幕分辨率宽: %s"), Tools.BaseConfigItem[TEXT("修改的屏幕分辨率宽")]));
    lgc(Tip, Printf(TEXT("  修改的屏幕分辨率高: %s"), Tools.BaseConfigItem[TEXT("修改的屏幕分辨率高")]));
    lgc(Tip, Printf(TEXT("  设置_开机自启动: %s"), Tools.BaseConfigItem[TEXT("注册表开机自启动")]));
    //lgc(Tip, Printf(TEXT("  设置_文件夹快捷键: %s"), Tools.BaseConfigItem[TEXT("文件夹快捷键")]));

    SelfStarting(); //更新 开机自启动

    lgc(Printf(TEXT("  Tools.ConfigFile 所有配置: [%s]"), ToStr(Tools.ConfigFile.GetConfigMap().size())));
}

void LoadShellConfig()
{
    if (!Tools.ShellConfig.empty()) { //修改配置后
        lgc(Tip, TEXT("ShellConfigInit() ReLoad"));
        Tools.ShellConfig.clear(); //清空Shell配置
        Tools.WindowShell.Clear(); //清空 程序启动项/菜单项
    }
    else {
        lgc(TEXT("ShellConfigInit()"));
    }

    for (auto tempConfig = Tools.ConfigFile_AllConfig.begin(); tempConfig != Tools.ConfigFile_AllConfig.end(); tempConfig++) {
        const Tstr Config = tempConfig->first;
        
        //执行Shell配置
        if (Config != TEXT("基本设置")) {
            auto tempConfigItem_Invalid = tempConfig->second.end(); //无效配置项
            std::map<Tstr, Tstr>::iterator tempConfigItem_Iter;

            lgc(Printf(TEXT("ShellConfig: [%s]"), Config));

            Tstr OperateMode;
            Tstr File;
            Tstr Args;
            Tstr WindowShow;
            Tstr MenuButton;

            //读取配置信息
            tempConfigItem_Iter = tempConfig->second.find(TEXT("模式"));
            if (tempConfigItem_Invalid != tempConfigItem_Iter) {
                OperateMode = tempConfigItem_Iter->second;
                lgc(Printf(TEXT("  操作模式: [%s]"), OperateMode));
            }
            else {
                lgcr(Printf(TEXT("  ShellConfig: [%s]没有 操作模式[%s]"), Config, OperateMode), War);
            }
            tempConfigItem_Iter = tempConfig->second.find(TEXT("文件"));
            if (tempConfigItem_Invalid != tempConfigItem_Iter) {
                File = tempConfigItem_Iter->second;
                lgc(Printf(TEXT("  文件: %s"), File));
            }
            else {
                lgcr(Printf(TEXT("  ShellConfig: [%s]没有 文件[%s]"), Config, OperateMode), War);
            }
            tempConfigItem_Iter = tempConfig->second.find(TEXT("参数"));
            if (tempConfigItem_Invalid != tempConfigItem_Iter) {
                Args = tempConfigItem_Iter->second;
                lgc(Printf(TEXT("  参数: %s"), Args));
            }
            else {
                lgcr(Printf(TEXT("  ShellConfig: [%s]没有 参数[%s]"), Config, OperateMode), War);
            }
            tempConfigItem_Iter = tempConfig->second.find(TEXT("显示窗口"));
            if (tempConfigItem_Invalid != tempConfigItem_Iter) {
                WindowShow = tempConfigItem_Iter->second;
                lgc(Printf(TEXT("  显示窗口: [%s]"), WindowShow));
            }
            else {
                lgcr(Printf(TEXT("  ShellConfig: [%s]没有 显示窗口[%s]"), Config, OperateMode), War);
            }
            tempConfigItem_Iter = tempConfig->second.find(TEXT("菜单按键"));
            if (tempConfigItem_Invalid != tempConfigItem_Iter) {
                MenuButton = tempConfigItem_Iter->second;
                lgc(Printf(TEXT("  菜单按键: [%s]"), MenuButton));
            }
            else {
                lgcr(Printf(TEXT("  ShellConfig: [%s]没有 菜单按键[%s]"), Config, OperateMode), War);
            }

            Tools.ShellConfig.push_back(ShellConfig(Config, OperateMode, File, Args, WindowShow, MenuButton));
        }
        else {
            lgc(Printf(TEXT("OtherConfig: [%s]"), Config));
        }
    }
}



void WindowsWindowClassRegister()
{
    lgc(TEXT("WindowsWindowClassRegister()"));

    // 注册托盘 窗口类
    WNDCLASS TrayWindowClass = { 0 };
    TrayWindowClass.style = CS_HREDRAW | CS_VREDRAW;
    TrayWindowClass.lpfnWndProc = WndProc;
    TrayWindowClass.cbClsExtra = 0;
    TrayWindowClass.cbWndExtra = 0;
    TrayWindowClass.hInstance = Tools.hIns;
    TrayWindowClass.hIcon = LoadIcon(Tools.hIns, Tools.Icon);
    TrayWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    TrayWindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    TrayWindowClass.lpszMenuName = NULL;
    TrayWindowClass.lpszClassName = Tools.ToolsWindowClassName.c_str();
    Tools.WindowHost.RegisterWindowClass(TrayWindowClass);
}

void WindowsWindowCreate()
{
    lgc(TEXT("WindowsWindowCreate()"));

    //窗口创建时会产生消息: WM_CREATE
    //要在 WM_CREATE 之前设置好Menu选项的 ID
    //按键
    Tools.ID_SetScreenResolution = WindowHost::GetHMENU();

    Tools.hWnd_Tray = CreateWindowEx(
    // 此处使用WS_EX_TOOLWINDOW 属性来隐藏显示在任务栏上的窗口程序按钮  
        WS_EX_TOOLWINDOW,
        //0,
        Tools.ToolsWindowClassName.c_str(), Tools.ExeTitleName.c_str(),
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL, NULL, Tools.hIns, NULL);
    Tools.WindowHost.AddWindowHost(TEXT("hWnd_Tray"), Tools.hWnd_Tray);

    // 不要修改TaskbarCreated，这是系统任务栏自定义的消息  
    Tools.WM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));
}

void LoadToolsMenu(HMENU Menu)
{
    int MenuItemCount = GetMenuItemCount(Menu);
    if (MenuItemCount != 0) { //已有选项时: 重新加载配置
        lgc(Tip, TEXT("LoadToolsMenu() ReLoad"));
        for (int i = MenuItemCount - 1; i >= 0; i--) { //向前遍历: 避免删除后继续使用改动后的数组而越界
            DeleteMenu(Menu, i, MF_BYPOSITION);
        }
    }
    else {
        lgc(TEXT("LoadToolsMenu()"));
    }

    //为Menu添加选项  
    AppendMenu(Menu, MF_STRING, Tools.ID_SetScreenResolution, TEXT("修改屏幕分辨率"));
    AppendMenu(Menu, MF_SEPARATOR, NULL, TEXT("分割线"));

    //Shell插入位置: Menu项总数 - 4
    Tools.WindowShell.ShellOperate(Menu, Tools.ShellConfig);
    Tools.WindowShell.ExeRunItemShell();

    Tools.ID_ToolsConfig = WindowHost::GetHMENU();
    lgc(Tip, Printf(TEXT("Tools.ID_ToolsConfig: [%s]"), Tools.ID_ToolsConfig));
    Tools.ID_Help = WindowHost::GetHMENU();
    lgc(Tip, Printf(TEXT("Tools.ID_Help: [%s]"), Tools.ID_Help));
    Tools.ID_Quit = WindowHost::GetHMENU();
    lgc(Tip, Printf(TEXT("Tools.ID_Quit: [%s]"), Tools.ID_Quit));

    AppendMenu(Menu, MF_SEPARATOR, NULL, TEXT("分割线"));
    AppendMenu(Menu, MF_STRING, Tools.ID_ToolsConfig, TEXT("配置"));
    AppendMenu(Menu, MF_STRING, Tools.ID_Help, TEXT("帮助"));
    AppendMenu(Menu, MF_STRING, Tools.ID_Quit, TEXT("退出"));

    //Win::RegisterHotKeyMessage RegisterHotKey_SetScreenResolution(TEXT("Ctrl + Alt + F9"), 
    // RegisterHotKey(Tools.hWnd_Tray, Tools.Menu_ScreenResolution, MOD_CONTROL | MOD_ALT, VK_F9));
}

void SelectMenu(int MenuItemID)
{
    if (MenuItemID == Tools.ID_ToolsConfig)
    {
        lgc(Printf(TEXT("菜单项ID: ID_ToolsConfig Path: [%s]"), Tools.ConfigFile.GetConfigFilePath()));

        Tools.WindowShell.ExecuteAnalyze(TEXT("打开配置文件"), TEXT("打开文件"), Tools.ConfigFile.GetConfigFilePath());
        lgr(Tip, TEXT("配置格式示例:\n\t[Shell命令名]\n\
\t参数=/k ping -t www.baidu.com\n\t文件=cmd\n\t显示窗口=是 (是/否)\n\
\t模式=打开文件 (打开文件/打开文件夹/管理员运行)\n\t菜单按键=是 (是/否: 托盘菜单中添加/程序启动时运行)"));
        int Result = MessageBox(NULL, TEXT("修改完成后, 点击[确定]\n即可更新托盘菜单的选项"), TEXT("提示"), MB_OKCANCEL | MB_ICONQUESTION);
        if (Result == IDOK) { //用户确定修改完成
            LoadBaseConfig(true);
            LoadShellConfig();
            LoadToolsMenu(Tools.hMenu);
        }
    }
    else if (MenuItemID == Tools.ID_SetScreenResolution)
    {
        lg(TEXT("菜单项ID: ID_SetScreenResolution"));
        if (Tools.SetScreenResolution)
        {
            //SetScreenResolution
            SetDisplaySize(
                std::stoi(Tools.BaseConfigItem.find(TEXT("默认的屏幕分辨率宽"))->second),
                std::stoi(Tools.BaseConfigItem.find(TEXT("默认的屏幕分辨率高"))->second)
            );
            Tools.SetScreenResolution = false;

            lgc(Tip, Printf(TEXT("屏幕分辨率修改成功:\n[%s] x [%s]"), 
                Tools.BaseConfigItem.find(TEXT("默认的屏幕分辨率宽"))->second, Tools.BaseConfigItem.find(TEXT("默认的屏幕分辨率高"))->second));
        }
        else
        {
            //SetScreenResolution
            SetDisplaySize(
                std::stoi(Tools.BaseConfigItem.find(TEXT("修改的屏幕分辨率宽"))->second),
                std::stoi(Tools.BaseConfigItem.find(TEXT("修改的屏幕分辨率高"))->second)
            );
            Tools.SetScreenResolution = true;

            lgc(Tip, Printf(TEXT("屏幕分辨率修改成功:\n[%s] x [%s]"), 
                Tools.BaseConfigItem.find(TEXT("修改的屏幕分辨率宽"))->second, Tools.BaseConfigItem.find(TEXT("修改的屏幕分辨率高"))->second));
        }
    }
    else if (MenuItemID == Tools.ID_Help)
    {
        lgc(TEXT("菜单项ID: ID_Help"));
        MessageBox(NULL, TEXT("分辨率: 宽高比例请在系统设置中查看!\n\t(设置->系统->屏幕->缩放和布局->显示器分辨率)\n\
"), TEXT("快捷键帮助"), NULL);
    }
    else if (MenuItemID == Tools.ID_Quit)
    {
        lgc(TEXT("菜单项ID: ID_Quit"));
        DestroyWindow(Tools.hWnd_Tray);
    }
    else {
        Tools.WindowShell.ExeMenuItemShell(MenuItemID);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //int wmId;
    //int wmEvent;

    POINT pt; //用于接收鼠标坐标  
    int MenuItemID; //用于接收Menu选项返回值  

    switch (uMsg)
    {
    case WM_CREATE: //窗口创建时候的消息.  
    {
        lgc(TEXT("WndProc::WM_CREATE"));

        // NOTIFYICONDATA: 用于与任务栏通知区域（也称为系统托盘）中的图标进行交互
        Tools.Nid.cbSize = sizeof(NOTIFYICONDATA);
        // hWnd: 传入的 hWnd, 否则 Shell_NotifyIcon添加托盘图标时会报错: 2147500037
        Tools.Nid.hWnd = hWnd;
        Tools.Nid.uID = 1;
        Tools.Nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_SHOWTIP;
        Tools.Nid.uCallbackMessage = WM_TRAY;
        Tools.Nid.hIcon = LoadIcon(Tools.hIns, MAKEINTRESOURCE(Tools.Icon));
        if (Tools.Nid.hIcon == NULL) {
            lg(TEXT("菜单图标资源无效!"), Err);
        }
        lstrcpy(Tools.Nid.szTip, Tools.ExeTrayName.c_str());

        Tools.hMenu = CreatePopupMenu(); //生成Menu

        LoadToolsMenu(Tools.hMenu);

        //lgc(TEXT("Shell_NotifyIcon之前 ErrorCode:") + ToStr(GetLastError()), Err);
        if (!Shell_NotifyIcon(NIM_ADD, &Tools.Nid)) {
            lgc(Printf(TEXT("Shell_NotifyIcon ErrorCode: [%s]"), ToStr(GetLastError())), Err);
        }

#ifdef _DEBUG
        //SelectMenu(Tools.ID_ToolsConfig);
#endif

        break;
    }
    case WM_TRAY: //连续使用该程序时候的消息.
    {
        switch (lParam)
        {
        case WM_RBUTTONDOWN:
        {
            //lgc(TEXT("按下鼠标右键"));
            GetCursorPos(&pt); //取鼠标坐标  
            ::SetForegroundWindow(hWnd); //解决在Menu外单击左键Menu不消失的问题  
            //EnableMenuItem(hmenu, IDR_PAUSE, MF_GRAYED); //让Menu中的某一项变灰

            MenuItemID = TrackPopupMenu(Tools.hMenu, TPM_RIGHTBUTTON | TPM_RETURNCMD,
                pt.x, pt.y, 0, hWnd, NULL); //显示Menu并获取选项ID 
            //lgc(TEXT("MenuItemID: ") + ToStr(MenuItemID), War);
            if (MenuItemID == 0) {
                lgc(TEXT("TrackPopupMenu: ") + ToStr(GetLastError()), Err);
            }

            SelectMenu(MenuItemID);

            break;
        }
        }
    case WM_HOTKEY:
    {
        //lgc(TEXT("快捷键: WM_HOTKEY"));

        //按键消息为: Ctrl+Alt+F9 时: 修改屏幕分辨率
        //if (Tools.Menu_ScreenResolution == wParam)
        //{
        //    lgc(TEXT("快捷键: 修改分辨率"));
        //    if (Tools.SetScreenResolution)
        //    {
        //        //SetScreenResolution
        //        SetDisplaySize(
        //            std::stoi(Tools.BaseConfigItem[TEXT("默认的屏幕分辨率宽")]),
        //            std::stoi(Tools.BaseConfigItem[TEXT("默认的屏幕分辨率高")])
        //        );
        //        Tools.SetScreenResolution = false;

        //        lgc(Printf(TEXT("屏幕分辨率修改成功:\n[%s] x [%s]"), Tools.BaseConfigItem[TEXT("默认的屏幕分辨率宽")], Tools.BaseConfigItem[TEXT("默认的屏幕分辨率高")]));
        //    }
        //    else
        //    {
        //        //SetScreenResolution
        //        SetDisplaySize(
        //            std::stoi(Tools.BaseConfigItem[TEXT("修改的屏幕分辨率宽")]),
        //            std::stoi(Tools.BaseConfigItem[TEXT("修改的屏幕分辨率高")])
        //        );
        //        Tools.SetScreenResolution = true;

        //        lgc(Printf(TEXT("屏幕分辨率修改成功:\n[%s] x [%s]"), Tools.BaseConfigItem[TEXT("修改的屏幕分辨率宽")], Tools.BaseConfigItem[TEXT("修改的屏幕分辨率高")]));
        //    }
        //}

        break;
    }
    case WM_DESTROY: //窗口销毁时候的消息.  
    {
        Shell_NotifyIcon(NIM_DELETE, &Tools.Nid);
        PostQuitMessage(0);
        break;
    }
    case WM_QUIT:
    {
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
}

