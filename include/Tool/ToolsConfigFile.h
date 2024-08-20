#pragma once

//工具配置文件
#ifndef _TOOLSCONFIGFILE_H
#define _TOOLSCONFIGFILE_H

#include "Tools_Tool.h"

#include "CommonTools.h"
#include "CharHandleOfConfigFile.h"
#include "Log.h"

#include <Windows.h>

using namespace Tools_Tool::StringHandling;

namespace Tools_Tool {
	//工具箱配置文件
	class TOOLS_TOOL_API ToolsConfigFile
	{
	private:
		wchar_t 程序所在路径[MAX_PATH] = { 0 };

		std::map<std::wstring, std::wstring> 基本设置内容;
		std::map<std::wstring, std::map<std::wstring, std::wstring>> 配置文件全内容;
		std::vector<std::wstring> 清单内容;
	
	private:
		// "C:\\Users\\22793\\source\\repos\\Tools\\Debug\\Tools.exe"
		std::wstring 程序_路径名 = L"";
		// "C:\\Users\\22793\\source\\repos\\Tools\\Debug"
		std::wstring 程序父文件夹_路径名 = L"";
		// "C:\\Users\\22793\\source\\repos\\Tools\\Debug\\config\\ToolsConfig.ini"
		std::wstring 工具箱配置文件_路径名 = L"";
		// "C:\\Users\\22793\\source\\repos\\Tools\\Debug\\config\\List.txt"
		std::wstring 清单文件_路径名 = L"";

		std::wstring 笔记本键盘关_路径名 = L"";
		std::wstring 笔记本键盘开_路径名 = L"";
		std::wstring smi_路径名 = L"";
		std::wstring ping_路径名 = L"";

	private:
		std::wstring ini后缀 = L".ini"; 
		std::wstring txt后缀 = L".txt"; 
		std::wstring exe后缀 = L".exe"; 

		//工具箱程序 文件名: \\Tools
		std::wstring 程序_名 = L"";
		//config 文件夹名: \\config
		std::wstring 配置文件夹_名 = L"\\config";
		//工具箱配置 文件名: \\ToolsConfig
		std::wstring 工具箱配置文件_名 = L"\\ToolsConfig";
		//清单 文件名: \\List
		std::wstring 清单文件_名 = L"\\List";


		std::wstring 笔记本键盘关_名 = L"\\NoteKeyboardOff.bat - 快捷方式";
		std::wstring 笔记本键盘开_名 = L"\\NoteKeyboardOn.bat - 快捷方式";
		std::wstring smi_名 = L"\\nvidia 1080.bat - 快捷方式";
		std::wstring ping_名 = L"\\ping baidu.bat";

	private:
		bool 清单状态 = false;

	public:
		ToolsConfigFile()
		{

		}
		ToolsConfigFile(std::wstring& 程序_名)
		{
			初始化(程序_名);
		};

		void 初始化(std::wstring 程序_名);

	public:
		//获取 工具箱配置文件路径
		std::wstring Get工具箱配置文件路径() const;

		std::map<std::wstring, std::wstring> Get基本设置内容() const;
		std::map<std::wstring, std::map<std::wstring, std::wstring>> Get配置文件全内容() const;

		std::wstring Get程序父文件夹路径() const;
		std::wstring Get程序配置文件夹路径() const;

		Ustr Get程序路径() const;

	public:
		//打开配置文件: windows
		void 创建配置文件();
		//打开清单文件: windows
		void 创建清单文件();

	public:
		void 打开配置文件();
		void 清单开关();
	};
}
#endif
