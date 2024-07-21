#include "pch.h" //DLL标准
#include <utility> //DLL标准
#include <limits.h> //DLL标准

#include "StringConversion.h"

#include <Windows.h>

std::wstring Tools_Tool::StringHandling::ANSIToUnicode(const std::string& from)
{
	if (from.empty())
	{
		return std::wstring();
	}

	size_t unicodeLen = 0;
	wchar_t* pUnicode = nullptr;
	if (unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, from.c_str(), -1, NULL, 0))
	{
		pUnicode = new wchar_t[unicodeLen + 1];
		memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
		if (!::MultiByteToWideChar(CP_ACP, 0, from.c_str(), -1, (LPWSTR)pUnicode, unicodeLen))
		{
			delete[] pUnicode;//clean up if failed;  
			return std::wstring();
		}
	}
	else
	{
		return std::wstring();
	}
	std::wstring strRet(pUnicode);
	delete[] pUnicode;//clean up if success;

	return std::move(strRet);
}