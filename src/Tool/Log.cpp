#include "pch.h"

#include "Log.h"


//控制台初始化
bool Tools_Tool::Log::FirstInitCMD = true;
bool Tools_Tool::Log::ShowTime = true;

DWORD  Tools_Tool::written;
HANDLE Tools_Tool::hConsoleOutput;
HANDLE Tools_Tool::hConsoleError;


void Tools_Tool::Log::SetShowLog(bool showLog)
{
	ShowLog = showLog;
}

void Tools_Tool::Log::SetConsoleTimeShow(bool showTime)
{
	ShowTime = showTime;
}

void Tools_Tool::Log::SetConsoleShow(bool showConsole)
{
	FirstInitCMD = showConsole;
}

std::wstring Tools_Tool::Log::StringToWstring(const std::string& str)
{
	std::wstring wContext = L"";
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	WCHAR* buffer = new WCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';
	wContext.append(buffer);
	delete[] buffer;

	return wContext;
}
