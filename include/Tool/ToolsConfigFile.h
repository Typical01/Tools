#pragma once

//工具配置文件
#ifndef _TOOLSCONFIGFILE_H
#define _TOOLSCONFIGFILE_H

#include "Tools_Tool.h"

#include "CommonTools.h"
#include "CharHandleOfConfigFile.h"
#include "Log.h"
#include "Shell.h"

#include <Windows.h>

using namespace Tools_Tool::StringHandling;
using Tools_Tool::WindowsSystem::WindowShell;

namespace Tools_Tool {
	//工具箱配置文件
	class TOOLS_TOOL_API ToolsConfigFile
	{
	private:
		Uchar 程序所在路径[MAX_PATH] = { 0 };

		// "C:\\Users\\22793\\source\\repos\\Tools\\Debug\\Tools.exe"
		Ustr 程序_路径名 = T("");
		// "C:\\Users\\22793\\source\\repos\\Tools\\Debug"
		Ustr 程序父文件夹_路径名 = T("");
		// "C:\\Users\\22793\\source\\repos\\Tools\\Debug\\config\\ToolsConfig.ini"
		Ustr 工具箱配置文件_路径名 = T("");

	private:
		//工具箱程序 文件名: \\Tools
		Ustr 程序_名 = T("");
		//工具箱配置 文件名: \\ToolsConfig
		Ustr 工具箱配置文件_名 = T("\\ToolsConfig");

	public:
		ToolsConfigFile() {}

		void 初始化(Ustr 程序_名);

	public:
		//获取 工具箱配置文件路径
		Ustr Get工具箱配置文件路径() const;

		Ustr Get程序父文件夹路径() const;
		Ustr Get程序配置文件夹路径() const;
		Ustr Get程序路径() const;

	public:
		void 打开配置文件();
	};
}
#endif
