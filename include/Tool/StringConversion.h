#pragma once

//字符串处理
#ifndef _STRINGHELPER_H
#define _STRINGHELPER_H

#include "Tools_Tool.h"

#include <string>

namespace Tools_Tool {
	//字符处理
	namespace StringHandling
	{
#if 0
		static std::string UnicodeToANSI(const std::wstring& from);
		static std::string UnicodeToUTF8(const std::wstring& from);

		static std::wstring UTF8ToUnicode(const std::string& from);
		static std::string UTF8ToANSI(const std::string& from);

		static std::string ANSIToUTF8(const std::string& from);
#endif
		TOOLS_TOOL_API std::wstring ANSIToUnicode(const std::string& from);
	}
}
#endif