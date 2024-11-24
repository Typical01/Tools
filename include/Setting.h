#pragma once

#ifndef _SETTING_H
#define _SETTING_H

#include "resource.h"

#include "Log.h" 

#include "ToolsConfigFile.h"
#include "Timers.h" 
#include "CommonTools.h" 
#include "WindowHosting.h"
#include "Message.h"
#include "Shell.h"
#include "Hotkey.h"
#include "Shell.h"

#include <Windows.h>
#include <string>

using namespace Typical_Tool;
using namespace Typical_Tool::WindowsSystem;
using namespace Typical_Tool::StringManage;
using Typical_Tool::WindowsSystem::WindowHosting;
using Typical_Tool::WindowsSystem::WindowHotkey;

#define _L(x) L ## x

class Settings {
public:
#define WM_TRAY WM_USER + 1
	//窗口参数
	HINSTANCE hIns;
	HWND hWnd_托盘;
	HWND hWnd_设置;
	HMENU hMenu;
	LPTSTR Icon;
	NOTIFYICONDATA nid = { 0 };
	UINT WM_TASKBARCREATED;

	Tstr 程序_名 = "Tools";
	const wchar_t* 程序_窗口类名 = L"Typical_Tools";
	
	Tstr 程序_托盘名 = "典型一号的工具箱";
	Tstr 程序_标题栏名 = "典型一号的工具箱 v1.0_Plus";

	//窗口操作
	WinHost wh;
	WindowShell ws;

	//设置
	ConfigFileTextManage 配置文件;
	Typical_Tool::ToolsConfigFile 工具箱配置文件;
	std::map<Tstr, Tstr> 基本设置内容;
	std::map<Tstr, std::map<Tstr, Tstr>> 配置文件全内容;
	std::vector<ShellConfig> ShellConfig;

	bool 修改屏幕分辨率 = false;

	//菜单句柄
	int ID_帮助;
	int ID_退出;
	int ID_工具箱设置;
	int ID_修改屏幕分辨率;

	//快捷键
	int 菜单_修改分辨率;
	int 菜单_打开Repos;
	int 菜单_打开Lib;

public:
	Settings() {}
};
static Settings Tools;

//更新配置文件内容
void UpdateConfig();

/*
* 单实例程序
* 设置DPI
* 保存进程句柄
*/
int Windows程序启动项();
void Windows窗口类注册();
void Windows窗口创建();

void 初始化();
void 配置初始化();

void 菜单生成(HMENU 菜单);
void 菜单选择(int 菜单选项ID);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif