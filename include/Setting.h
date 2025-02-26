#pragma once

#ifndef _SETTING_H
#define _SETTING_H

#include "resource.h"

#include "libTypical\Tool.h"

using namespace Typical_Tool;
using namespace Typical_Tool::WindowsSystem;
using namespace Typical_Tool::StringManage;
using Typical_Tool::WindowsSystem::WindowHost;

class Settings {
public:
#define WM_TRAY WM_USER + 1
	//窗口参数
	HINSTANCE hIns;
	HWND hWnd_Tray;
	HWND hWnd_Setting;
	HMENU hMenu;
	LPTSTR Icon;
	NOTIFYICONDATA nid = { 0 };
	UINT WM_TASKBARCREATED;

	Tstr ExeName = Tx("Tools"); //程序名
	Tstr ExeCurrentPath = FileSystem::GetCurrentPath(); //当前程序的工作路径
	Tstr ExeWindowClassName = Tx("Typical_Tools"); //程序窗口类名
	
	Tstr ExeTrayName = Tx("典型一号的工具箱"); //程序托盘名
	Tstr ExeTitleName = Tx("典型一号的工具箱 v1.0_Plus"); //程序标题栏名

	//窗口操作
	WindowHost wh;
	WindowShell ws;

	//设置
	FileSystem FileSystem;
	ConfigFileTextManage ConfigFile; //配置文件
	std::map<Tstr, Tstr> BaseConfigItem; //[基本设置] 配置项
	std::map<Tstr, std::map<Tstr, Tstr>> ConfigFile_AllConfig; //配置文件 全内容
	std::vector<ShellConfig> ShellConfig; //[Shell] 配置项

	bool SetScreenResolution = false; //修改屏幕分辨率

	//菜单句柄
	int ID_Help; //帮助
	int ID_Quit; //退出
	int ID_ToolsConfig; //工具箱配置
	int ID_SetScreenResolution; //修改屏幕分辨率

	//快捷键
	int Menu_ScreenResolution; //菜单 修改分辨率

public:
	Settings() {}
};
static Settings Tools;

//更新ConfigFile内容
void UpdateConfig();

/*Windows程序启动项
* 单实例程序
* 设置DPI
* 保存进程句柄
*/
int WindowsExeAutoRunItem();
//Windows窗口类注册
void WindowsWindowClassRegister();
//Windows窗口创建
void WindowsWindowCreate();

//[Shell] 配置初始化
void ShellConfigInit();
//[基本设置] 配置初始化
void BaseConfigInit();

//菜单创建
void CreateMenu(HMENU 菜单);
//菜单选择
void SelectMenu(int 菜单选项ID);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif