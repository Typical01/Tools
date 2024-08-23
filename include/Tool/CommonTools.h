﻿#pragma once

//常用工具
#ifndef _COMMONTOOLS_H
#define _COMMONTOOLS_H

#include "Tools_Tool.h"
#include "Log.h"
#include "Timers.h"
#include "Shell.h"

#include <Windows.h>
#include <tlhelp32.h>

#include <vector>
#include <ShObjIdl.h>
#include <ShlGuid.h>


using namespace std;

namespace Tools_Tool {
	//Windows系统操作----------------------------------------------------------------------------------------
	namespace WindowsSystem
	{

		//显示--------------------------------------------------------------------------------------------------------------------

		/*设置屏幕分辨率 运行
			* **必看**
			* 分辨率(width/height): 一定成比例, 如: 1920 x 1080(16:9), 1280 x 1024(4:3)
		*/
		TOOLS_TOOL_API void SetDisplaySize(int widthValue, int HeightValue);


		//程序操作----------------------------------------------------------------------------------------------------------------


		//进程DPI_AWARENESS_CONTEXT_SYSTEM_AWARE
		template<class T = bool>
		void WindowDPI()
		{
			//设置DPI感知级别(可选，仅Windows 10 1703及更高版本）
			if (SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE) == NULL) { //传入的值无效
				lgc(T("传入的值无效"), T("Windows DPI"));
			}
			else {
				lgc(T("DPI感知(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE) 设置成功!"), T("Windows DPI"));
				lgc();
				lgc();
			}
		}

		template<class T = bool>
		int 单实例运行(Ustr windowClassName, Ustr windowTitleName)
		{
			//程序启动初始化
			HWND handle = FindWindow(windowClassName.c_str(), windowTitleName.c_str());
			if (handle != NULL)
			{
				lgr((Ustr)T("应用程序已在运行") + windowTitleName, lgm::wr);
				return 0;
			}
			return 1;
		}

		//获取程序ID
		template<class T = bool>
		TOOLS_TOOL_API DWORD FindProcessIDByName(const Ustr& processName)
		{
			HANDLE hProcessSnap;
			PROCESSENTRY32 pe32;
			hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (hProcessSnap == INVALID_HANDLE_VALUE)
			{
				return(0);
			}
			pe32.dwSize = sizeof(PROCESSENTRY32);
			if (!Process32First(hProcessSnap, &pe32))
			{
				CloseHandle(hProcessSnap); // clean the snapshot object 清理快照对象
				return(0);
			}
			DWORD processId = 0;
			do
			{
				if ((Ustr)pe32.szExeFile == processName) //进程名称
				{
					processId = pe32.th32ProcessID; //进程ID
					break;
				}
			} while (Process32Next(hProcessSnap, &pe32));
			CloseHandle(hProcessSnap);
			return(processId);
		}

		//是否为管理员
		TOOLS_TOOL_API bool IsUserAdmin();

		//获得管理员权限
		template<class T = bool>
		TOOLS_TOOL_API bool GainAdminPrivileges(Ustr strApp)
		{
			ShellMessage temp;

			if (!IsUserAdmin()) { //非管理员权限, 则申请
				ShellMessage UserAdmin(T("申请管理员权限"), (int)ShellExecute(NULL, T("runas"), strApp.c_str(), NULL, NULL, SW_SHOWNORMAL));
				temp = UserAdmin;
			}
			//成功申请时, 退出当前进程
			if (temp.IsSucceed()) {
				return true;
			}

			return false;
		}


		/* 成功获取到管理员权限后, 返回 true
		* 同时, 可以将发出申请的程序退出, 使拥有管理员权限的程序单例运行
		* code:
		*	if (Tools_Tool::WindowsSystem::WindowHosting::获取管理员权限(true)) { return 0; }
		*/
		template<class T = bool>
		bool 获取管理员权限(bool isGet = true)
		{
			if (isGet) {
				//获取当前程序的全路径
				Uchar 程序路径[MAX_PATH] = T("");
				GetModuleFileName(NULL, 程序路径, MAX_PATH);
				//获得管理员权限
				if (GainAdminPrivileges(程序路径)) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}

		//添加注册表项以实现 开机自启动
		template<class T = bool>
		TOOLS_TOOL_API bool SetAutoRun(Ustr valueName, Ustr exePath)
		{
			LONG result;
			HKEY hKey;

			Ustr regPath = T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

			// 打开注册表项  
			result = RegOpenKeyEx(HKEY_CURRENT_USER, regPath.c_str(), 0, KEY_SET_VALUE, &hKey);
			if (result != ERROR_SUCCESS) {
				lgc(T("打开密钥失败: %ld") + result, lgm::er); // 假设 lgc 能够处理 wstring 和日志级别  
				return false;
			}

			// 设置注册表值  
			result = RegSetValueEx(hKey, valueName.c_str(), 0, REG_SZ, (const BYTE*)exePath.c_str(), (Ustrlen(exePath.c_str()) + 1) * sizeof(wchar_t));
			if (result != ERROR_SUCCESS) {
				lgc(T("设置注册表值失败: %ld") + result, lgm::er); // 假设 lgc 能够处理 wstring 和日志级别 
				RegCloseKey(hKey);
				return false;
			}

			RegCloseKey(hKey);
			lgc(T("注册表注册成功!"), lgm::wr);
			return true;
		}

		//创建快捷方式: 程序/目标路径/图标
		/* 需要初始化 Com组件, code:
		* CoInitialize(0);
		* CreateLink(...);
		* CoUninitialize(); */
		TOOLS_TOOL_API HRESULT CreateLink(LPCTSTR 对象路径, LPCTSTR 对象名, 
			LPCTSTR 图标路径 = NULL, LPCTSTR 快捷方式描述 = NULL, LPCTSTR 目标程序的参数 = NULL);

		//文件操作---------------------------------------------------------------------------------------------------------

		template<class T = bool>
		TOOLS_TOOL_API Ustr 提取程序名(Ustr path)
		{
			//匹配 '\' && '/' 任意
			size_t lastSepPos = path.find_last_of(T("\\/"));
			if (lastSepPos != std::wstring::npos) {
				return path.substr(lastSepPos + 1); // 提取文件名部分
			}

			// 去掉 .exe 后缀
			size_t exePos = path.find_last_of(T(".exe"));
			if (exePos != Ustr::npos && exePos == path.length() - 4) {
				path = path.substr(0, exePos); // 去掉 .exe 后缀
			}

			return path; // 如果找不到路径分隔符，则返回整个路径
		}

		template<class T = bool>
		TOOLS_TOOL_API Ustr 提取程序目录路径(Ustr path)
		{
			size_t lastSepPos = path.find_last_of(T("\\/"));
			if (lastSepPos != std::wstring::npos) {
				//return path.substr(0, lastSepPos + 1); // 包括最后一个路径分隔符
				return path.substr(0, lastSepPos); // 不包括最后一个路径分隔符
			}
			return T(""); // 如果找不到路径分隔符，则返回空字符串
		}
		template<class T = bool>
		TOOLS_TOOL_API Ustr Get程序名()
		{
			Uchar exePath[MAX_PATH];
			Ustr exeName;

			//获取当前程序的全路径
			DWORD length = GetModuleFileName(NULL, exePath, MAX_PATH);

			if (length > 0 && length < MAX_PATH) {
				exeName = 提取程序名(exePath);
				lgc(T("当前可执行文件的名称: " + exeName));
			}
			else {
				lgc(T("无法获取当前可执行文件的路径!"));
			}
			return exeName;
		}
		template<class T = bool>
		TOOLS_TOOL_API Ustr Get程序目录路径()
		{
			Uchar exePath[MAX_PATH];
			Ustr folderName;

			//获取当前程序的全路径
			DWORD length = GetModuleFileName(NULL, exePath, MAX_PATH);

			if (length > 0 && length < MAX_PATH) {
				folderName = 提取程序目录路径(exePath);
				lgc(T("当前程序目录路径名: " + folderName));
			}
			else {
				lgc(T("无法获取当前可执行文件的路径!"));
			}

			return folderName;
		}
		TOOLS_TOOL_API bool Get目录所有文件夹(std::string path, std::vector<std::string>& folder);
		TOOLS_TOOL_API std::vector<std::string> Get目录所有文件夹(std::string path);

		TOOLS_TOOL_API void Get目录所有文件(std::string path, std::vector<std::string>& files);
		TOOLS_TOOL_API std::vector<std::string> Get目录所有文件(std::string path);

		template<class T = bool>
		TOOLS_TOOL_API bool CreateFolder(Ustr folderPath)
		{
			DWORD attributes = GetFileAttributes(folderPath.c_str());

			// 检查路径是否存在且不是目录  
			if (attributes == INVALID_FILE_ATTRIBUTES)
			{
				// 路径不存在或出错，尝试创建目录  
				if (CreateDirectory(folderPath.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
				{
					lgc(T("文件夹") + folderPath + T("创建成功!"));
					return true;
				}
				lgc(T("文件夹") + folderPath + T("创建失败!"));
				// 创建失败且不是因为路径已存在  
				return false;
			}
			else if (attributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				lgc(T("文件夹") + folderPath + T("已存在"));
				// 路径已经是一个目录  
				return true;
			}
			lgc(T("文件夹") + folderPath + T("创建失败(路径存在, 但不是目录)!"));
			// 路径存在但不是目录（可能是一个文件）  
			return false;
		}
		template<class T = bool>
		TOOLS_TOOL_API void OpenFolder(const Ustr& path)
		{
			ShellExecute(NULL, NULL, path.c_str(), NULL, NULL, SW_SHOWNORMAL);
		}

		//控制台----------------------------------------------------------------------------------------------------------

		//移动光标到目标位置
		TOOLS_TOOL_API void MoveCursorLocation(int x, int y);
	}
	namespace WinSys = WindowsSystem;
	

	//字符处理---------------------------------------------------------------------------------------------------------------
	namespace StringHandling 
	{

		//字符转换-------------------------------------------------------------------------------------------------------
		TOOLS_TOOL_API std::wstring StringToWstring(const std::string& str);
		TOOLS_TOOL_API std::wstring StringToWstring(std::string&& str);
#define stow StringToWstring
		TOOLS_TOOL_API std::string WstringToString(const std::wstring& wstr);
		TOOLS_TOOL_API std::string WstringToString(std::wstring&& wstr);
#define wtos WstringToString

		//编码转换---------------------------------------------------------------------------------------------------------
	}
	namespace 字符处理 = StringHandling;

	namespace GameTools
	{
		//帧率-----------------------------------------------------------------------------------------------------------

#ifndef FPS_COUNT
#define FPS_COUNT 60 //Fps 统计的间隔(帧率)
#endif
		//获取 FPS
		TOOLS_TOOL_API float GetFps();
	}
	namespace 游戏工具 = GameTools;
}

#endif