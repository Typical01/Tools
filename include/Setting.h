#pragma once

#ifndef _SETTING_H
#define _SETTING_H

#include "ToolsConfigFile.h"
#include "Log.h"

#include <Windows.h>
#include <string>

using namespace Tools_Tool;
using namespace Tools_Tool::WindowsSystem;

class Tools {
public:
	//Windows
	const wchar_t* 程序_名 = L"Tools";
	const wchar_t* 程序_窗口类名 = L"典型一号的工具箱";
	const wchar_t* 程序_窗口类名2 = L"典型一号的工具箱2";
	const wchar_t* 程序_标题栏名 = L"典型一号的工具箱 v1.0";
	const wchar_t* 程序_标题栏名_子窗口 = L"典型一号的工具箱_子窗口test";

	//Icon
#define IDI_ICON256x 100

//按键
#define ID_帮助 1099
#define ID_退出 1000
#define ID_工具箱配置 1001
#define ID_清单 1002
#define ID_笔记本键盘开关 1003
#define ID_修改屏幕分辨率 1004
#define ID_Repos 1005
#define ID_Lib 1006
#define ID_Ping 1007
#define ID_子窗口test 1008

//热键ID


	Tools_Tool::ToolsConfigFile 工具箱配置文件;

	int 进度条百分比值 = 0;

	HINSTANCE hInstance;
	HWND hWnd, hWnd_子窗口, hWnd_进度条;
	HMENU hMenu;

	NOTIFYICONDATA nid;

	bool 修改屏幕分辨率 = false;

	//笔记本键盘
	std::wstring 笔记本键盘关闭 = L"sc config i8042prt start= disabled"; //关闭
	std::wstring 笔记本键盘开启 = L"sc config i8042prt start= auto"; //开启

	//配置文件内容
	std::wstring 配置文件_原本屏幕分辨率宽;
	std::wstring 配置文件_原本屏幕分辨率高;
	std::wstring 配置文件_修改的屏幕分辨率宽;
	std::wstring 配置文件_修改的屏幕分辨率高;
	std::wstring 配置文件_笔记本键盘开关状态;
	std::wstring 配置文件_Repos;
	std::wstring 配置文件_Lib;
	std::wstring 配置文件_nvidia;

	//更新配置文件内容
	void UpdateConfig();

	/*
	* 单实例程序
	* 设置DPI
	* 保存进程句柄
	*/
	void Windows程序启动项(HINSTANCE&);

	void Windows窗口类注册(LPCWSTR icon);

	void Windows窗口创建();

	void Windows窗口初始化(LPCWSTR icon);
};
static Tools Setting;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK ChildWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//进度条消息过程
LONG_PTR CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif