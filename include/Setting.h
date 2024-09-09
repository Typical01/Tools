#pragma once

#ifndef _SETTING_H
#define _SETTING_H

#include "resource.h"
#include "ToolsConfigFile.h"
#include "Log.h" 
#include "Timers.h" 
#include "CommonTools.h" 
#include "WindowHosting.h"
#include "Message.h"
#include "Shell.h"
#include "Hotkey.h"
//#include "Edit.h"
#include "Shell.h"

#include <Windows.h>
#include <string>

using namespace Typical_Tool;
using namespace Typical_Tool::WindowsSystem;
using namespace Typical_Tool::StringHandling;
using Typical_Tool::WindowsSystem::WindowHosting;
using Typical_Tool::WindowsSystem::WindowHotkey;
//using Typical_Tool::WindowsSystem::WindowEdit;

class Settings {
public:
#define WM_TRAY WM_USER + 1
	//窗口参数
	HINSTANCE hIns;
	HWND hWnd_托盘;
	HWND hWnd_设置;
	//hWnd_确认, hWnd_取消;
	//HWND hWnd_标签;
	HMENU hMenu;
	LPTSTR Icon;
	NOTIFYICONDATA nid = { 0 };
	UINT WM_TASKBARCREATED;

	const char* 程序_名 = _T("Tools");
	//SetWindowLongPtr(hwndEdit, GWLP_WNDPROC, (LONG_PTR)SubclassedEditProc);
	const char* 程序_窗口类名 = _T("典型一号的工具箱");
	const char* 程序_标签窗口类名 = _T("典型一号的工具箱_标签");
	
	const char* 程序_托盘名 = _T("工具箱托盘菜单");
	const char* 程序_标题栏名 = _T("典型一号的工具箱 v1.0");
	const char* 程序_标题栏名_设置窗口 = _T("工具箱设置");
	const char* 程序_标题栏名_标签窗口 = _T("工具箱标签");

	//窗口操作
	WinHost wh;
	//WindowLocationInfo 设置窗口位置;
	//WindowEdit we;
	WindowShell ws;

	//设置
	CharHandleOfConfigFile 配置文件;
	Typical_Tool::ToolsConfigFile 工具箱配置文件;
	std::map<Ustr, Ustr> 基本设置内容;
	std::map<Ustr, std::map<Ustr, Ustr>> 配置文件全内容;

	int 进度条百分比值 = 0;
	bool 修改屏幕分辨率 = false;
	Ustr 笔记本键盘关闭 = _T("sc config i8042prt start= disabled"); //关闭
	Ustr 笔记本键盘开启 = _T("sc config i8042prt start= auto"); //开启
	Ustr 设置_原屏幕分辨率宽;
	Ustr 设置_原屏幕分辨率高;
	Ustr 设置_修改屏幕分辨率宽;
	Ustr 设置_修改屏幕分辨率高;
	Ustr 设置_笔记本键盘开关状态;
	Ustr 设置_Repos;
	Ustr 设置_Lib;
	Ustr 设置_Nvidia;
	Ustr 设置_开机自启动;

	//菜单句柄
	//std::map<Ustr, int> hMenuMap;
	int ID_帮助;
	int ID_退出;
	int ID_工具箱设置;
	//int ID_标签;
	int ID_笔记本键盘开关;
	int ID_修改屏幕分辨率;
	int ID_Ping;

	int 确认按钮;
	int 取消按钮;

	//快捷键
	int 菜单_修改分辨率;
	int 菜单_打开Repos;
	int 菜单_打开Lib;
	int 标签_全选;
	//int 标签_保存;
	//int 标签_打开;
	//int 标签_切换状态;
	
	//bool 标签_是否修改;

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
void 打开配置文件();

void 菜单生成(HMENU 菜单);
void 菜单选择(int 菜单选项ID);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK TipsWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif