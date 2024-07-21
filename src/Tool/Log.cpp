#include "pch.h"

#include "Log.h"


//控制台初始化
bool Tools_Tool::Log::FirstInitCMD = true;

//bool Tools_Tool::Log::this->ShowLog = true;	   //显示日志
//bool Tools_Tool::Log::this->ShowTime = true;    //显示时间

DWORD  Tools_Tool::written;
HANDLE Tools_Tool::hConsoleOutput;
HANDLE Tools_Tool::hConsoleError;

void Tools_Tool::Log::SetShowLog(bool showLog)
{
	this->ShowLog = showLog;
}

void Tools_Tool::Log::SetConsoleTimeShow(bool show)
{
	this->ShowTime = show;
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

void Tools_Tool::SetShowLog(bool showLog)
{
	lg.SetShowLog(showLog);
	lgc.SetShowLog(showLog);
	lgr.SetShowLog(showLog);
	lgcr.SetShowLog(showLog);
}

void Tools_Tool::SetConsoleTimeShow(bool show)
{
	lg.SetConsoleTimeShow(show);
	lgc.SetConsoleTimeShow(show);
	lgr.SetConsoleTimeShow(show);
	lgcr.SetConsoleTimeShow(show);
}
