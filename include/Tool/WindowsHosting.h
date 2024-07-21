#pragma once

#ifndef _WINDOWSHOSTING_H
#define _WINDOWSHOSTING_H

#include "Log.h"
#include "Font.h"
#include "CommonTools.h"

#include <Windows.h>
#include <map>
#include <string>
#include <vector>

namespace Tools_Tool {

	namespace WindowsSystem {

		class TOOLS_TOOL_API WindowHosting {
#define 窗口托管 WindowHosting
		
		private:
			WindowFont WinFont;

			HWND 主窗口;
			std::vector<HWND> 子窗口Vec;

		public:
			WindowHosting() = delete;
			WindowHosting(WNDCLASS& wndClass)
				: WinFont()
			{
				//设置进程的DPI感知级别
				WindowDPI();
			}

			//进程DPI_AWARENESS_CONTEXT_SYSTEM_AWARE
			template<class T = bool>
			void WindowDPI()
			{
				//设置DPI感知级别(可选，仅Windows 10 1703及更高版本）
				if (SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE) == NULL) { //传入的值无效
					lgc(TEXT("传入的值无效"), TEXT("\t\t\tWindows DPI"));
				}
				else {
					lgc(TEXT("DPI感知(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE) 设置成功!"), TEXT("\t\t\tWindows DPI"));
					lgc();
					lgc();
				}
			}
			template<class T = bool>
			bool 添加主窗口托管(HWND mainWindow)
			{
				if (mainWindow == NULL) {
					//创建失败
					lg(TEXT("主窗口创建失败!"));
					return false;
				}

				WinFont.SetWindowFont(mainWindow);
				this->主窗口 = mainWindow;
				return true;
			}
			template<class T = bool>
			bool 添加子窗口托管(HWND childWindow, HMENU childMenuID)
			{
				if (childWindow == NULL) {
					//创建失败
					lg(TEXT("子窗口创建失败!"), std::to_wstring((int)childMenuID));
					return false;
				}

				WinFont.SetWindowFont(childWindow);
				this->子窗口Vec.push_back(childWindow);
				return true;
			}

		public:
			HWND Get主窗口();
			std::vector<HWND> Get子窗口();

		public:
			template<class T = bool>
			static int 单实例运行(LPCTSTR windowClassName, LPCTSTR windowTitleName)
			{
				//程序启动初始化
				HWND handle = FindWindow(windowClassName, windowTitleName);
				if (handle != NULL)
				{
					lg(TEXT("应用程序已在运行"), windowTitleName);
					return 0;
				}
			}

			/* 成功获取到管理员权限后, 返回 true
			* 同时, 可以将发出申请的程序退出, 使拥有管理员权限的程序单例运行
			* code:
			*	if (Tools_Tool::WindowsSystem::WindowHosting::获取管理员权限(true)) { return 0; }
			*/
			template<class T = bool>
			static bool 获取管理员权限(bool isGet = true)
			{
				if (isGet) {
					//获取当前程序的全路径
					Uchar 程序路径[MAX_PATH] = TEXT("");
					GetModuleFileName(NULL, 程序路径, MAX_PATH);
					//获得管理员权限
					if (GainAdminPrivileges(程序路径)) {
						return true;
					}
					else {
						return false;
					}
				}
			}
			template<class T = bool>
			static int 注册窗口类(WNDCLASS& wndClass)
			{
				if (!RegisterClass(&wndClass))
				{
					lg(TEXT("窗口类注册失败!"), wndClass.lpszClassName);
					return 0;
				}
			}
			
			/* 1.WindowHosting::单实例运行 | 可以跟 WindowHosting::获取管理员权限
			* 2.创建 WNDCLASS wc 窗口类
			* 3.WindowHosting::注册窗口类
			* 4.托管 WindowHosting(wc)
			* 5.创建主窗口 & 子窗口后, 将其加入托管 WindowHosting::添加主窗口托管/添加子窗口托管();
			* 6.其他初始化
			*/
			template<class T = bool>
			static void README()
			{

			}
		};
	}
}

#endif