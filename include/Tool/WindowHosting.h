#pragma once

#ifndef _WINDOWSHOSTING_H
#define _WINDOWSHOSTING_H

#include "Log.h"
#include "Font.h"
#include "Location.h"
#include "CommonTools.h"

#include <Windows.h>
#include <map>
#include <string>
#include <vector>

namespace Tools_Tool {

	namespace WindowsSystem {

		class TOOLS_TOOL_API WindowHosting {
#define 窗口托管 WindowHosting
		public:
			static UINT WM_TASKBARCREATED_WH;
			static int 进度条进度百分比;

		private:
			WindowFont WinFont;
			std::map<Ustr, HWND> 窗口;

			static bool 注册进度条类;
			static const wchar_t* 进度条主窗口类名;
			static const wchar_t* 进度条类名;
			static HWND 进度条;
			static HWND 进度条主窗口;
			static int 进度条步进;
		private:
			static HINSTANCE hIns;
			static int hMenu;

		public:
			WindowHosting()
				: WinFont()
			{

			}

			bool 添加窗口托管(Ustr windowName, HWND& window, int showWindow = 5);
			void 设置字体(HFONT hFont);
		public:

			//进程DPI_AWARENESS_CONTEXT_SYSTEM_AWARE
			static void WindowDPI();

			static int 单实例运行(LPCWSTR windowClassName, LPCWSTR windowTitleName);

			/* 成功获取到管理员权限后, 返回 true
			* 同时, 可以将发出申请的程序退出, 使拥有管理员权限的程序单例运行
			* code:
			*	if (Tools_Tool::WindowsSystem::WindowHosting::获取管理员权限(true)) { return 0; }
			*/
			static bool 获取管理员权限(bool isGet = true);
			static int 注册窗口类(WNDCLASS& wndClass);


			/* 1.WindowHosting::单实例运行 | 可以跟 WindowHosting::获取管理员权限
			* 2.创建 WNDCLASS wc 窗口类
			* 3.WindowHosting::注册窗口类
			* 4.托管 WindowHosting::WindowHosting(wc)
			* 5.创建主窗口 & 子窗口后, 将其加入托管 WindowHosting::添加主窗口托管/添加子窗口托管();
			* 6.其他初始化
			*/
			template<class T = bool>
			static void README()
			{

			}

		public:
			std::map<Ustr, HWND>& Get窗口();
			static int GetHMENU();

			// WNDCLASS wndclass = { 0 }
			static void 注册进度条窗口类(HINSTANCE& hInstance);
			static void 创建进度条(Ustr 进度条标题栏名);
			static void 增加进度条进度(int 进度百分比);
			static void 增加进度条进度();
			static void 设置进度条步进(int 步进);
			
		};
		using WinHost = WindowHosting;
		using 窗口托管 = WindowHosting;

		//进度条消息过程
		LRESULT CALLBACK WindowProcedureMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		LONG_PTR CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}
}

#endif