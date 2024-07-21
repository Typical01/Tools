#pragma once

//常用工具
#ifndef _COMMONTOOLS_H
#define _COMMONTOOLS_H

#include "Tools_Tool.h"
#include "Log.h"
#include "Timers.h"
#include "StringConversion.h"

#include <Windows.h>

#include <vector>
#include <ShObjIdl.h>
#include <ShlGuid.h>

//#pragma comment(lib, "Advapi32.lib")

using namespace std;

namespace Tools_Tool {
	//Windows系统操作----------------------------------------------------------------------------------------
	namespace WindowsSystem
	{
		class TOOLS_TOOL_API ShellMessage
		{
		private:
			std::wstring 信息;
			int ErrorCode; //错误代码
			bool Status; //是否成功

		public:
			ShellMessage(std::wstring Shell信息)
				: 信息(Shell信息), ErrorCode(0), Status(false)
			{}

			void operator=(int message);

			//错误代码
			int GetErrorCode();

			//是否成功
			bool isSucceed();
		};

		class TOOLS_TOOL_API RegisterHotKeyMessage
		{
		private:
			std::wstring 信息;

		public:
			RegisterHotKeyMessage(std::wstring RegisterHotKey热键信息)
				: 信息(RegisterHotKey热键信息)
			{}

			void operator=(int message);
		};
		typedef ShellMessage Shell消息;
		typedef RegisterHotKeyMessage 热键注册消息;

		//显示--------------------------------------------------------------------------------------------------------------------

		/*设置屏幕分辨率 运行
			* **必看**
			* 分辨率(width/height): 一定成比例, 如: 1920 x 1080(16:9), 1280 x 1024(4:3)
		*/
		TOOLS_TOOL_API void SetDisplaySize(int widthValue, int HeightValue);


		//程序操作----------------------------------------------------------------------------------------------------------------
		
		//打开 应用程序文件
		TOOLS_TOOL_API void StartFile(const wchar_t* filePath);

		//获取程序ID
		TOOLS_TOOL_API DWORD FindProcessIDByName(const std::string& processName);

		//获取程序PID
		TOOLS_TOOL_API long GetProcessIdFromName(const char* name);

		//程序是否正在运行
		TOOLS_TOOL_API bool isProgramRunning(const std::string& program_name);

		//是否为管理员
		TOOLS_TOOL_API bool IsUserAdmin();

		//获得管理员权限
		TOOLS_TOOL_API bool GainAdminPrivileges(const wchar_t* strApp); //精简 ElevateNow/IsRunAsAdministrator后的，且可以传入路径指定程序

		//添加注册表项以实现 开机自启动
		TOOLS_TOOL_API bool SetAutoRun(const wchar_t* valueName, const wchar_t* exePath);

		//创建快捷方式: 程序/目标路径/图标
		/* 需要初始化 Com组件, code:
		* CoInitialize(0);
		* CreateLink(...);
		* CoUninitialize(); */
		TOOLS_TOOL_API HRESULT CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, 
			LPCWSTR lpszIcon = NULL, LPCWSTR lpszDesc = NULL, LPCWSTR lpszArgs = NULL);

		//文件操作---------------------------------------------------------------------------------------------------------

		//创建文件
		TOOLS_TOOL_API void CreateFiles(const string& FilePath);

		//获取当前目录下的所有文件夹
		TOOLS_TOOL_API bool GetAllFolder(std::string path, std::vector<std::string>& folder);

		//获取所有的文件名
		TOOLS_TOOL_API void GetAllFiles(std::string path, std::vector<std::string>& files);

		//获取当前目录下所有的文件 夹
		TOOLS_TOOL_API std::vector<std::string> GetAllFolder(std::string path);

		//获取所有的文件 名
		TOOLS_TOOL_API std::vector<std::string> GetAllFiles(std::string path);

		//创建文件夹
		TOOLS_TOOL_API bool CreateFolder(std::string folderPath);

		//打开文件夹
		TOOLS_TOOL_API void OpenFolder(const std::string& path);

		//控制台----------------------------------------------------------------------------------------------------------

		//移动光标到目标位置
		TOOLS_TOOL_API void MoveCursorLocation(int x, int y);
	}
	namespace Win系统操作 = WindowsSystem;
	

	//字符处理---------------------------------------------------------------------------------------------------------------
	namespace StringHandling {

		//字符转换-------------------------------------------------------------------------------------------------------
		//std::string 转 std::wstring(宽字符)
		TOOLS_TOOL_API std::wstring StringToWstring(const std::string& str);
#define stow StringToWstring
		TOOLS_TOOL_API std::string WstringToString(std::wstring wstr);
#define wtos WstringToString

		TOOLS_TOOL_API std::wstring CharToWchar(const char* c, size_t m_encode = CP_ACP);

		TOOLS_TOOL_API std::string WcharToChar(const wchar_t* wp, size_t m_encode = CP_ACP);

		//编码转换---------------------------------------------------------------------------------------------------------
		
		//UTF-8 转 GB2312
		TOOLS_TOOL_API std::string UTF8ToGB2312(const char* str);
		//UTF8 转 GBK
		TOOLS_TOOL_API std::string UTF8ToGBK(const char* utf8_str);
		//GBK 转 UTF8
		TOOLS_TOOL_API  std::string GBKToUTF8(const char* gbk_str);

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