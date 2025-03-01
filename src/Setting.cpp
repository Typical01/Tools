//#include "pch.h"

#include "Setting.h"


void SelfStarting()
{
    Tstr ExePathName;
    if (Win::GetExePathName(ExePathName)) {
        if (Tools.BaseConfigItem[Tx("注册表开机自启动")] == Tx("否")) {
            lgc(Tx("注册开机自启动: 否"), Tip);

            if (Win::SetSelfStarting((Tstr)Tx("Typical_Tools"), Format(Tx("\"%\""), ExePathName).str(), false)) {
                lgc(Tx("注册开机自启动: 删除成功!"), Tip);
            }
            else {
                lgcr(Tx("注册开机自启动: 删除失败!"), Err);
            }
        }
        else {
            lgc(Tx("注册开机自启动: 是"), Tip);
            if (Win::SetSelfStarting((Tstr)Tx("Typical_Tools"), Format(Tx("\"%\""), ExePathName).str(), true)) {
                lgc(Tx("注册开机自启动: 添加成功!"), Tip);
            }
            else {
                lgcr(Tx("注册开机自启动: 添加失败!"), Err);
            }
        }
    }
}

int WindowsExeAutoRunItem()
{
    lgc(Tx("WindowsExeAutoRunItem()"));

    Win::WindowDPI();
    if (Win::AloneRun(Tools.ExeWindowClassName, Tools.ExeTitleName) == 0) {
        return 0;
    }

    LoadBaseConfig();

    SelfStarting();
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
        lgc(Tip, Tx("LoadBaseConfig() ReLoad"));
        Tools.ConfigFile.Clear();
    }
    else {
        lgc(Tx("LoadBaseConfig()"));
    }

    //先创建文件夹(否则后面的文件不能创建): \\Tools\\Config
    //Tstr ConfigDirectory = Format(Tx("%\\Config"), Tools.ExeCurrentPath);
    Tstr ConfigDirectory = Format(Tx("Config"));
    Tools.ExeConfigFilePath = Format(Tx("%%"), ConfigDirectory, Tx("\\Config.ini"));
    Tools.FileSystem.SetPath(ConfigDirectory);
    if (Tools.FileSystem.CreateDirectorys()) {
        //文件不存在时，创建
#ifdef _DEBUG
        Tools.ConfigFile.SetShowManageLog(true);
#endif
        if (!Tools.ConfigFile.Init(Format(Tx("%"), Tools.ExeConfigFilePath))) {
            lg(Format(Tx("配置文件初始化: [%]"), Tools.ConfigFile.GetConfigFilePath()));

            std::vector<Tstr> BaseConfig;
            BaseConfig.push_back(Tx("原本屏幕分辨率宽=1920"));
            BaseConfig.push_back(Tx("原本屏幕分辨率高=1080"));
            BaseConfig.push_back(Tx("修改的屏幕分辨率宽=1280"));
            BaseConfig.push_back(Tx("修改的屏幕分辨率高=1024"));
            BaseConfig.push_back(Tx("注册表开机自启动=否"));
            //BaseConfig.push_back(Tx("文件夹快捷键=否"));

            std::vector<Tstr> ping_baidu;
            ping_baidu.push_back(Tx("菜单按键=是"));
            ping_baidu.push_back(Tx("模式=打开文件"));
            ping_baidu.push_back(Tx("文件=cmd"));
            ping_baidu.push_back(Tx("参数=/k ping -t www.baidu.com"));
            ping_baidu.push_back(Tx("显示窗口=是"));
            std::vector<Tstr> BookKeyOn;
            BookKeyOn.push_back(Tx("菜单按键=是"));
            BookKeyOn.push_back(Tx("模式=管理员运行"));
            BookKeyOn.push_back(Tx("文件=cmd"));
            BookKeyOn.push_back(Tx("参数=/k sc config i8042prt start= auto"));
            BookKeyOn.push_back(Tx("显示窗口=是"));
            std::vector<Tstr> BookKeyOff;
            BookKeyOff.push_back(Tx("菜单按键=是"));
            BookKeyOff.push_back(Tx("模式=管理员运行"));
            BookKeyOff.push_back(Tx("文件=cmd"));
            BookKeyOff.push_back(Tx("参数=/k sc config i8042prt start= disabled"));
            BookKeyOff.push_back(Tx("显示窗口=是"));

            Tools.ConfigFile.AddConfig_FormatSampleText();
            Tools.ConfigFile.AddConfig(Tx("基本设置"), BaseConfig);
            //Tools.ConfigFile.AddConfig(Tx("nvidia-smi"), nv_smi);
            Tools.ConfigFile.AddConfig(Tx("ping-baidu"), ping_baidu);
            Tools.ConfigFile.AddConfig(Tx("笔记本键盘开"), BookKeyOn);
            Tools.ConfigFile.AddConfig(Tx("笔记本键盘关"), BookKeyOff);
            Tools.ConfigFile.WriteConfigFile();

        }
        else { //文件存在
        }
    }
    else {
#ifdef _DEBUG
        Tools.ConfigFile.SetShowManageLog(true);
#endif
        if (!Tools.ConfigFile.Init(Format(Tx("%"), Tools.ExeConfigFilePath))) {
            lgcr(Err, Format(Tx("读取文件[%]失败!"), Tools.ExeConfigFilePath));
        }
        else {
            lgc(Tip, Format(Tx("读取文件[%]成功!"), Tools.ExeConfigFilePath));
        }
    }

    UpdateConfig();
}

void UpdateConfig()
{
    if (!Tools.ConfigFile_AllConfig.empty()) { //修改配置后, 重新加载
        lgc(Tip, Tx("UpdateConfig() ReLoad"));
        Tools.ConfigFile_AllConfig.clear();
        Tools.BaseConfigItem.clear();
    }
    else {
        lgc(Tx("UpdateConfig()"));
    }

    //获取更新后的ConfigFile
    Tools.ConfigFile_AllConfig = Tools.ConfigFile.GetConfigMap();
    Tools.BaseConfigItem = Tools.ConfigFile.GetConfigItem(Tx("基本设置"));

    lgc(Tip, Format(Tx("\t原本屏幕分辨率宽: %"), Tools.BaseConfigItem[Tx("原本屏幕分辨率宽")]));
    lgc(Tip, Format(Tx("\t原本屏幕分辨率高: %"), Tools.BaseConfigItem[Tx("原本屏幕分辨率高")]));
    lgc(Tip, Format(Tx("\t修改的屏幕分辨率宽: %"), Tools.BaseConfigItem[Tx("修改的屏幕分辨率宽")]));
    lgc(Tip, Format(Tx("\t修改的屏幕分辨率高: %"), Tools.BaseConfigItem[Tx("修改的屏幕分辨率高")]));
    lgc(Tip, Format(Tx("\t设置_开机自启动: %"), Tools.BaseConfigItem[Tx("注册表开机自启动")]));
    //lgc(Tip, Format(Tx("\t设置_文件夹快捷键: %"), Tools.BaseConfigItem[Tx("文件夹快捷键")]));

    SelfStarting(); //更新 开机自启动

    lgc(Format(Tx("  Tools.ConfigFile 所有配置: [%]"), ToStr(Tools.ConfigFile.GetConfigMap().size())));
}

void LoadShellConfig()
{
    if (!Tools.ShellConfig.empty()) { //修改配置后
        lgc(Tip, Tx("ShellConfigInit() ReLoad"));
        Tools.ShellConfig.clear(); //清空Shell配置
        Tools.WindowShell.Clear(); //清空 程序启动项/菜单项
    }
    else {
        lgc(Tx("ShellConfigInit()"));
    }

    for (auto tempConfig = Tools.ConfigFile_AllConfig.begin(); tempConfig != Tools.ConfigFile_AllConfig.end(); tempConfig++) {
        const Tstr Config = tempConfig->first;
        
        //执行Shell配置
        if (Config != Tx("基本设置")) {
            auto tempConfigItem_Invalid = tempConfig->second.end(); //无效配置项
            std::map<Tstr, Tstr>::iterator tempConfigItem_Iter;

            lgc(Format(Tx("ShellConfig: [%]"), Config));

            Tstr OperateMode;
            Tstr File;
            Tstr Args;
            Tstr WindowShow;
            Tstr MenuButton;

            //读取配置信息
            tempConfigItem_Iter = tempConfig->second.find(Tx("模式"));
            if (tempConfigItem_Invalid != tempConfigItem_Iter) {
                OperateMode = tempConfigItem_Iter->second;
                lgc(Format(Tx("  操作模式: [%]"), OperateMode));
            }
            else {
                lgcr(Format(Tx("  ShellConfig: [%]没有 操作模式[%]"), Config, OperateMode), War);
            }
            tempConfigItem_Iter = tempConfig->second.find(Tx("文件"));
            if (tempConfigItem_Invalid != tempConfigItem_Iter) {
                File = tempConfigItem_Iter->second;
                lgc(Format(Tx("  文件: %"), File));
            }
            else {
                lgcr(Format(Tx("  ShellConfig: [%]没有 文件[%]"), Config, OperateMode), War);
            }
            tempConfigItem_Iter = tempConfig->second.find(Tx("参数"));
            if (tempConfigItem_Invalid != tempConfigItem_Iter) {
                Args = tempConfigItem_Iter->second;
                lgc(Format(Tx("  参数: %"), Args));
            }
            else {
                lgcr(Format(Tx("  ShellConfig: [%]没有 参数[%]"), Config, OperateMode), War);
            }
            tempConfigItem_Iter = tempConfig->second.find(Tx("显示窗口"));
            if (tempConfigItem_Invalid != tempConfigItem_Iter) {
                WindowShow = tempConfigItem_Iter->second;
                lgc(Format(Tx("  显示窗口: [%]"), WindowShow));
            }
            else {
                lgcr(Format(Tx("  ShellConfig: [%]没有 显示窗口[%]"), Config, OperateMode), War);
            }
            tempConfigItem_Iter = tempConfig->second.find(Tx("菜单按键"));
            if (tempConfigItem_Invalid != tempConfigItem_Iter) {
                MenuButton = tempConfigItem_Iter->second;
                lgc(Format(Tx("  菜单按键: [%]"), MenuButton));
            }
            else {
                lgcr(Format(Tx("  ShellConfig: [%]没有 菜单按键[%]"), Config, OperateMode), War);
            }

            Tools.ShellConfig.push_back(ShellConfig(Config, OperateMode, File, Args, WindowShow, MenuButton));
        }
        else {
            lgc(Format(Tx("OtherConfig: [%]"), Config));
        }
    }
}



void WindowsWindowClassRegister()
{
    lgc(Tx("WindowsWindowClassRegister()"));

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
    wndclass.lpszClassName = Tools.ExeWindowClassName.c_str();
    Tools.WindowHost.RegisterWindowClass(wndclass);
}

void WindowsWindowCreate()
{
    lgc(Tx("WindowsWindowCreate()"));

    //窗口创建时会产生消息: WM_CREATE
    //要在 WM_CREATE 之前设置好Menu选项的 ID
    //按键
    Tools.ID_SetScreenResolution = WindowHost::GetHMENU();

    Tools.hWnd_Tray = CreateWindowEx(
    // 此处使用WS_EX_TOOLWINDOW 属性来隐藏显示在任务栏上的窗口程序按钮  
        WS_EX_TOOLWINDOW,
        //0,
        Tools.ExeWindowClassName.c_str(), Tools.ExeTitleName.c_str(),
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL, NULL, Tools.hIns, NULL);
    Tools.WindowHost.AddWindowHost(Tx("hWnd_Tray"), Tools.hWnd_Tray);

    // 不要修改TaskbarCreated，这是系统任务栏自定义的消息  
    Tools.WM_TASKBARCREATED = RegisterWindowMessage(Tx("TaskbarCreated"));
}

void LoadToolsMenu(HMENU Menu)
{
    int MenuItemCount = GetMenuItemCount(Menu);
    if (MenuItemCount != 0) { //已有选项时: 重新加载配置
        lgc(Tip, Tx("LoadToolsMenu() ReLoad"));
        for (int i = MenuItemCount - 1; i >= 0; i--) { //向前遍历: 避免删除后继续使用改动后的数组而越界
            DeleteMenu(Menu, i, MF_BYPOSITION);
        }
    }
    else {
        lgc(Tx("LoadToolsMenu()"));
    }

    //为Menu添加选项  
    AppendMenu(Menu, MF_STRING, Tools.ID_SetScreenResolution, Tx("修改屏幕分辨率"));
    AppendMenu(Menu, MF_SEPARATOR, NULL, Tx("分割线"));

    //Shell插入位置: Menu项总数 - 4
    Tools.WindowShell.ShellOperate(Menu, Tools.ShellConfig);
    Tools.WindowShell.ExeRunItemShell();

    Tools.ID_ToolsConfig = WindowHost::GetHMENU();
    Tools.ID_Help = WindowHost::GetHMENU();
    Tools.ID_Quit = WindowHost::GetHMENU();

    AppendMenu(Menu, MF_SEPARATOR, NULL, Tx("分割线"));
    AppendMenu(Menu, MF_STRING, Tools.ID_ToolsConfig, Tx("配置"));
    AppendMenu(Menu, MF_STRING, Tools.ID_Help, Tx("帮助"));
    AppendMenu(Menu, MF_STRING, Tools.ID_Quit, Tx("退出"));

    Tools.Menu_ScreenResolution = WindowHost::GetHMENU();

    //Win::RegisterHotKeyMessage RegisterHotKey_SetScreenResolution(Tx("Ctrl + Alt + F9"), 
    // RegisterHotKey(Tools.hWnd_Tray, Tools.Menu_ScreenResolution, MOD_CONTROL | MOD_ALT, VK_F9));
}

void SelectMenu(int MenuItemID)
{
    if (MenuItemID == Tools.ID_ToolsConfig)
    {
        lg(Format(Tx("菜单项ID: ID_ToolsConfig Path: [%]"), Tools.ConfigFile.GetConfigFilePath()));

        Tools.WindowShell.ExecuteAnalyze(Tx("打开配置文件"), Tx("打开文件"), Tools.ConfigFile.GetConfigFilePath());
        //lgr(Tx("修改后需要重启程序!"));
        int Result = MessageBox(NULL, Tx("修改完成后, 点击[确定]\n即可更新托盘菜单的选项"), Tx("提示"), MB_OK | MB_ICONQUESTION);
        if (Result == IDOK) { //用户确定修改完成
            LoadBaseConfig(true);
            LoadShellConfig();
            LoadToolsMenu(Tools.hMenu);
        }
    }
    else if (MenuItemID == Tools.ID_SetScreenResolution)
    {
        lg(Tx("菜单项ID: ID_SetScreenResolution"));
        if (Tools.SetScreenResolution)
        {
            //SetScreenResolution
            SetDisplaySize(
                std::stoi(Tools.BaseConfigItem.find(Tx("原本屏幕分辨率宽"))->second),
                std::stoi(Tools.BaseConfigItem.find(Tx("原本屏幕分辨率高"))->second)
            );
            Tools.SetScreenResolution = false;

            lgc(Tip, Format(Tx("屏幕分辨率修改成功:\n[%] x [%]"), 
                Tools.BaseConfigItem.find(Tx("原本屏幕分辨率宽"))->second, Tools.BaseConfigItem.find(Tx("原本屏幕分辨率高"))->second));
        }
        else
        {
            //SetScreenResolution
            SetDisplaySize(
                std::stoi(Tools.BaseConfigItem.find(Tx("修改的屏幕分辨率宽"))->second),
                std::stoi(Tools.BaseConfigItem.find(Tx("修改的屏幕分辨率高"))->second)
            );
            Tools.SetScreenResolution = true;

            lgc(Tip, Format(Tx("屏幕分辨率修改成功:\n[%] x [%]"), 
                Tools.BaseConfigItem.find(Tx("修改的屏幕分辨率宽"))->second, Tools.BaseConfigItem.find(Tx("修改的屏幕分辨率高"))->second));
        }
    }
    else if (MenuItemID == Tools.ID_Help)
    {
        lgc(Tx("菜单项ID: ID_Help"));
        MessageBox(NULL, Tx("分辨率: 宽高比例请在系统设置中查看!\n\t(设置->系统->屏幕->缩放和布局->显示器分辨率)\n\
"), Tx("快捷键帮助"), NULL);
    }

    else if (MenuItemID == Tools.ID_Quit)
    {
        lgc(Tx("菜单项ID: ID_Quit"));
        PostQuitMessage(0);
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
        lgc(Tx("WndProc::WM_CREATE"));
        
        // NOTIFYICONDATA: 用于与任务栏通知区域（也称为系统托盘）中的图标进行交互
        Tools.Nid.cbSize = sizeof(NOTIFYICONDATA);
        // hWnd: 传入的 hWnd, 否则 Shell_NotifyIcon添加托盘图标时会报错: 2147500037
        Tools.Nid.hWnd = hWnd;
        Tools.Nid.uID = 1;
        Tools.Nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_SHOWTIP;
        Tools.Nid.uCallbackMessage = WM_TRAY;
        Tools.Nid.hIcon = LoadIcon(Tools.hIns, MAKEINTRESOURCE(Tools.Icon));
        if (Tools.Nid.hIcon == NULL) {
            lg(Tx("菜单图标资源无效!"), Err);
        }
        lstrcpy(Tools.Nid.szTip, Tools.ExeTrayName.c_str());

        Tools.hMenu = CreatePopupMenu(); //生成Menu

        LoadToolsMenu(Tools.hMenu);

        //lgc(Tx("Shell_NotifyIcon之前 ErrorCode:") + ToStr(GetLastError()), Err);
        if (!Shell_NotifyIcon(NIM_ADD, &Tools.Nid)) {
            lgc(Format(Tx("Shell_NotifyIcon ErrorCode: [%]"), ToStr(GetLastError())), Err);
        }

        break;
    }
    case WM_TRAY: //连续使用该程序时候的消息.
    {
        switch (lParam)
        {
        case WM_RBUTTONDOWN:
        {
            //lgc(Tx("按下鼠标右键"));
            GetCursorPos(&pt); //取鼠标坐标  
            ::SetForegroundWindow(hWnd); //解决在Menu外单击左键Menu不消失的问题  
            //EnableMenuItem(hmenu, IDR_PAUSE, MF_GRAYED); //让Menu中的某一项变灰

            MenuItemID = TrackPopupMenu(Tools.hMenu, TPM_RIGHTBUTTON | TPM_RETURNCMD,
                pt.x, pt.y, 0, hWnd, NULL); //显示Menu并获取选项ID 
            //lgc(Tx("MenuItemID: ") + ToStr(MenuItemID), War);
            if (MenuItemID == 0) {
                lgc(Tx("TrackPopupMenu: ") + ToStr(GetLastError()), Err);
            }

            SelectMenu(MenuItemID);
            
            break;
        }
        break;
        }
    }
    case WM_HOTKEY:
    {        
        //lgc(Tx("快捷键: WM_HOTKEY"));

        //按键消息为: Ctrl+Alt+F9 时: 修改屏幕分辨率
        if (Tools.Menu_ScreenResolution == wParam)
        {
            lgc(Tx("快捷键: 修改分辨率"));
            if (Tools.SetScreenResolution)
            {
                //SetScreenResolution
                SetDisplaySize(
                    std::stoi(Tools.BaseConfigItem[Tx("原本屏幕分辨率宽")]),
                    std::stoi(Tools.BaseConfigItem[Tx("原本屏幕分辨率高")])
                );
                Tools.SetScreenResolution = false;

                lgc(Format(Tx("屏幕分辨率修改成功:\n[%] x [%]"), Tools.BaseConfigItem[Tx("原本屏幕分辨率宽")], Tools.BaseConfigItem[Tx("原本屏幕分辨率高")]));
            }
            else
            {
                //SetScreenResolution
                SetDisplaySize(
                    std::stoi(Tools.BaseConfigItem[Tx("修改的屏幕分辨率宽")]),
                    std::stoi(Tools.BaseConfigItem[Tx("修改的屏幕分辨率高")])
                );
                Tools.SetScreenResolution = true;

                lgc(Format(Tx("屏幕分辨率修改成功:\n[%] x [%]"), Tools.BaseConfigItem[Tx("修改的屏幕分辨率宽")], Tools.BaseConfigItem[Tx("修改的屏幕分辨率高")]));
            }
        }

        break;
    }
    case WM_DESTROY: //窗口销毁时候的消息.  
    {
        Shell_NotifyIcon(NIM_DELETE, &Tools.Nid);
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
