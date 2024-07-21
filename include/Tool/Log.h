#pragma once

#ifndef _LOG_H
#define _LOG_H

#define _CRT_SECURE_NO_WARNINGS

//#define _CONSOLE
//#define _English


#include "Tools_Tool.h"
#include "Timers.h"
//#include "CommonTools.h"

#include <iostream>
#include <cstdio>
#include <Windows.h>
#include <string>

#include <iomanip>
#include <ctime>
#include <sstream>
#include <locale>
#include <chrono>


//#include "pch.h"
namespace Tools_Tool
{
	enum TOOLS_TOOL_API LogMessage
	{
		nl, //空行: NULL String
		ts, //提示: Tips
		wr, //警告: Warning
		er  //错误: Error
	};
	typedef LogMessage lgm;

	//	#ifdef _CONSOLE
	//		#define LogW LogW_CMD
	//	#else
	//		#define LogW LogW_WIN
	//	#endif
	//
	//	#define Log LogW
	//	#define Log_CMD LogW_CMD
	//#else
	//	#ifdef _CONSOLE
	//		#define LogA LogA_CMD
	//	#else
	//		#define LogA LogA_WIN
	//	#endif
	//	
	//	#define Log LogA
	//	#define Log_CMD LogA_CMD
	//#endif

#ifdef UNICODE
	#define Uchar wchar_t
	#define Ustr std::wstring
	#define Ucout std::wcout
	#define Ucerr std::wcerr
	#define Uto_string std::to_wstring
	#define Ufreopen_s _wfreopen_s
	#define Ustrlen wcslen

#else
	#define Uchar char
	#define Ustr std::string
	#define Ucout std::cout
	#define Ucerr std::cerr
	#define Uto_string std::to_string
	#define Ufreopen_s freopen_s
	#define Ustrlen strlen

#endif


//Use of English requires comments: From translation software
//#define _English
//#define UNICODE // Visual Studio defines macros by default

//非中文: Not Chinese
#ifdef _English
#define LogString_nl ""
#define LogString_ts "Tips"
#define LogString_wr "Warning"
#define LogString_er "Error"

#else
//中文
#define Log字符串_空行 ""
#define Log字符串_提示 "提示"
#define Log字符串_警告 "警告"
#define Log字符串_错误 "错误"
#endif

#ifdef _English
#define Log_nl LogString_nl
#define Log_ts LogString_ts
#define Log_wr LogString_wr
#define Log_er LogString_er
#else
#define Log_nl Log字符串_空行
#define Log_ts Log字符串_提示
#define Log_wr Log字符串_警告
#define Log_er Log字符串_错误
#endif

	TOOLS_TOOL_API extern DWORD written;
	TOOLS_TOOL_API extern HANDLE hConsoleOutput;
	TOOLS_TOOL_API extern HANDLE hConsoleError;

	class TOOLS_TOOL_API Log {
	private:
		static bool FirstInitCMD; //控制台初始化

		bool Release;
		bool Mode_Cmd;
		bool ShowLog;
		bool ShowTime;

	public:
		Log(bool release, bool mode_Cmd)
			: Release(release), Mode_Cmd(mode_Cmd), ShowLog(true), ShowTime(true)
		{
			if (FirstInitCMD)
			{
				Ucout.imbue(std::locale(""));

#ifndef _CONSOLE
				if (FreeConsole() == 0)
				{
#ifdef _English
					MessageBox(0, TEXT("log: detached console failure!"), TEXT("error"), 0);
					MessageBox(0, (TEXT("error code: ") + Uto_string(GetLastError())).c_str(), TEXT(Log_er), 0);
#else

					MessageBox(0, TEXT("log: 分离控制台失败!"), TEXT(Log_er), 0);
					MessageBox(0, (TEXT("错误代码: ") + Uto_string(GetLastError())).c_str(), TEXT(Log_er), 0);
#endif
				}
				if (AllocConsole() == 0)
				{
#ifdef _English
					MessageBox(0, TEXT("log: assignment console failure!"), TEXT(Log_er), 0);
					MessageBox(0, (TEXT("error code: ") + Uto_string(GetLastError())).c_str(), TEXT(Log_er), 0);
#else
					MessageBox(0, TEXT("log: 分配控制台失败!"), TEXT(Log_er), 0);
					MessageBox(0, (TEXT("错误代码: ") + Uto_string(GetLastError())).c_str(), TEXT(Log_er), 0);
#endif
				}
				FILE* cmd_w_stream;
				Ufreopen_s(&cmd_w_stream, TEXT("conout$"), TEXT("wt"), stdout);
#endif

				//GetStdHandle() -> setconsoletextcolor()的过程中
				//好像不能将其放在静态对象的类(static class Log lg)中, 需要获取到标准输出/错误/其他的句柄后, 再修改颜色
				//否则, 将不打印文本信息!!!

				hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
				if (hConsoleOutput == INVALID_HANDLE_VALUE) {
#ifdef _English
					MessageBox(0, TEXT("log: get STD_OUTPUT_HANDLE fail!"), TEXT(Log_er), 0);
#else
					MessageBox(0, TEXT("log: 获取 STD_OUTPUT_HANDLE fail!"), TEXT(Log_er), 0);
#endif
				}

				hConsoleError = GetStdHandle(STD_ERROR_HANDLE);
				if (hConsoleError == INVALID_HANDLE_VALUE) {
#ifdef _English
					MessageBox(0, TEXT("log: get STD_ERROR_HANDLE fail!"), TEXT(Log_er), 0);
#else
					MessageBox(0, TEXT("log: 获取 STD_ERROR_HANDLE fail!"), TEXT(Log_er), 0);
#endif
				}

				FirstInitCMD = false;
			}
		}


	public:
		void SetShowLog(bool showLog);
#define 设置日志显示 SetShowLog
		void SetConsoleTimeShow(bool show);
#define 设置时间显示 SetConsoleTimeShow


	private:
		std::wstring StringToWstring(const std::string& str);

		template<class Temp = bool>
		void SetConsoleTextColor(WORD wColor)
		{
			if (hConsoleOutput != INVALID_HANDLE_VALUE) {
				SetConsoleTextAttribute(hConsoleOutput, wColor);
			}
		}
		template<class Temp = bool>
		void SetConsoleTextColor_Error(WORD wColor)
		{
			if (hConsoleError != INVALID_HANDLE_VALUE) {
				SetConsoleTextAttribute(hConsoleError, wColor);
			}
		}

		//非 WORD 版本不会还原文本颜色
		template<class Temp = bool>
		void ConsoleOutput(Ustr& text)
		{
			if (hConsoleOutput != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleOutput, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
					//MessageBox(NULL, (TEXT("Tools_Tool::Log::ConsoleOutput(Ustr& text)\nError Code: ") + Uto_string(GetLastError())).c_str(), TEXT(TEXT(Log_er)), MB_ICONSTOP);
				}
			}
		}
		template<class Temp = bool>
		void ConsoleOutput(Ustr& text, WORD wColor)
		{
			SetConsoleTextColor(wColor);

			if (hConsoleOutput != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleOutput, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
					SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					//MessageBox(NULL, (TEXT("Tools_Tool::Log::ConsoleOutput(Ustr& text)\nError Code: ") + Uto_string(GetLastError())).c_str(), TEXT(TEXT(Log_er)), MB_ICONSTOP);
				}
			}
			SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		//非 WORD 版本不会还原文本颜色
		template<class Temp = bool>
		void ConsoleOutput(Ustr&& text)
		{
			if (hConsoleOutput != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleOutput, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
					//MessageBox(NULL, (TEXT("Tools_Tool::Log::ConsoleOutput(Ustr& text)\nError Code: ") + Uto_string(GetLastError())).c_str(), TEXT(TEXT(Log_er)), MB_ICONSTOP);
				}
			}
		}
		template<class Temp = bool>
		void ConsoleOutput(Ustr&& text, WORD wColor)
		{
			SetConsoleTextColor(wColor);

			if (hConsoleOutput != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleOutput, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
					SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					//MessageBox(NULL, (TEXT("Tools_Tool::Log::ConsoleOutput(Ustr& text)\nError Code: ") + Uto_string(GetLastError())).c_str(), TEXT(TEXT(Log_er)), MB_ICONSTOP);
				}
			}
			SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		//非 WORD 版本不会还原文本颜色
		template<class Temp = bool>
		void ConsoleOutput_Error(Ustr& text)
		{
			if (hConsoleError != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleError, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
					//MessageBox(NULL, (TEXT("Tools_Tool::Log::ConsoleOutput_Error(Ustr& text)\nError Code: ") + Uto_string(GetLastError())).c_str(), TEXT(TEXT(Log_er)), MB_ICONSTOP);
				}
			}
		}
		template<class Temp = bool>
		void ConsoleOutput_Error(Ustr& text, WORD wColor)
		{
			SetConsoleTextColor_Error(wColor);

			if (hConsoleError != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleError, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
					SetConsoleTextColor_Error(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					//MessageBox(NULL, (TEXT("Tools_Tool::Log::ConsoleOutput_Error(Ustr& text)\nError Code: ") + Uto_string(GetLastError())).c_str(), TEXT(TEXT(Log_er)), MB_ICONSTOP);
				}
			}
			SetConsoleTextColor_Error(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		//非 WORD 版本不会还原文本颜色
		template<class Temp = bool>
		void ConsoleOutput_Error(Ustr&& text)
		{
			if (hConsoleError != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleError, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
					//MessageBox(NULL, (TEXT("Tools_Tool::Log::ConsoleOutput_Error(Ustr& text)\nError Code: ") + Uto_string(GetLastError())).c_str(), TEXT(TEXT(Log_er)), MB_ICONSTOP);
				}
			}
		}
		template<class Temp = bool>
		void ConsoleOutput_Error(Ustr&& text, WORD wColor)
		{
			SetConsoleTextColor_Error(wColor);

			if (hConsoleError != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleError, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
					SetConsoleTextColor_Error(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					//MessageBox(NULL, (TEXT("Tools_Tool::Log::ConsoleOutput_Error(Ustr& text)\nError Code: ") + Uto_string(GetLastError())).c_str(), TEXT(TEXT(Log_er)), MB_ICONSTOP);
				}
			}
			SetConsoleTextColor_Error(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		template<class Temp = bool>
		Ustr FormattingConsoleTimeShow(Ustr& text)
		{
			if (this->ShowTime) {
				std::chrono::system_clock::time_point now = std::chrono::system_clock::now();;
				// 获取当前时间点（自epoch以来的时间）
				// 将时间点转换为time_t（用于localtime函数）
				std::time_t tm = std::chrono::system_clock::to_time_t(now);
				// 使用localtime函数将time_t转换为本地时间（std::tm结构）
				std::tm* now_tm = std::localtime(&tm);

				// 使用 std::put_time 格式化时间
				std::ostringstream oss;
				oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S"); // 自定义时间格式
				Ustr temp = (Ustr)TEXT("[") + StringToWstring(oss.str()) + TEXT("]") + text;

				return temp;
			}
			return text;
		}
		template<class Temp = bool>
		Ustr FormattingConsoleTimeShow(Ustr&& text)
		{
			if (this->ShowTime) {
				std::chrono::system_clock::time_point now = std::chrono::system_clock::now();;
				// 获取当前时间点（自epoch以来的时间）
				// 将时间点转换为time_t（用于localtime函数）
				std::time_t tm = std::chrono::system_clock::to_time_t(now);
				// 使用localtime函数将time_t转换为本地时间（std::tm结构）
				std::tm* now_tm = std::localtime(&tm);

				// 使用 std::put_time 格式化时间
				std::ostringstream oss;
				oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S"); // 自定义时间格式
				Ustr temp = (Ustr)TEXT("[") + StringToWstring(oss.str()) + TEXT("]") + text;

				return temp;
			}
			return text;
		}


		template<class T = bool>
		void Logs_ustr_ustr(Ustr& text, Ustr& title)
		{
			if (Mode_Cmd)
			{
				ConsoleOutput(FormattingConsoleTimeShow(title) + TEXT(": ") + text + TEXT("\n"));

				return;
			}
#ifndef _CONSOLE
			MessageBox(NULL, text.c_str(), title.c_str(), MB_OK);
#else
			ConsoleOutput(FormattingConsoleTimeShow(title) + TEXT(": ") + text + TEXT("\n"));
#endif
		}
		template<class T = bool>
		void Logs_ustr_ustr(Ustr&& text, Ustr&& title)
		{
			if (Mode_Cmd)
			{
				ConsoleOutput(FormattingConsoleTimeShow(title) + TEXT(": ") + text + TEXT("\n"));

				return;
			}
#ifndef _CONSOLE
			MessageBox(NULL, text.c_str(), title.c_str(), MB_OK);
#else
			ConsoleOutput(FormattingConsoleTimeShow(title) + TEXT(": ") + text + TEXT("\n"));
#endif
		}
		template<class T = bool>
		void Logs_ustr_ustr(Ustr& text, Ustr&& title)
		{
			if (Mode_Cmd)
			{
				ConsoleOutput(FormattingConsoleTimeShow(title) + TEXT(": ") + text + TEXT("\n"));

				return;
			}
#ifndef _CONSOLE
			MessageBox(NULL, text.c_str(), title.c_str(), MB_OK);
#else
			ConsoleOutput(FormattingConsoleTimeShow(title) + TEXT(": ") + text + TEXT("\n"));
#endif
		}
		template<class T = bool>
		void Logs_ustr_ustr(Ustr&& text, Ustr& title)
		{
			if (Mode_Cmd)
			{
				ConsoleOutput(FormattingConsoleTimeShow(title) + TEXT(": ") + text + TEXT("\n"));

				return;
			}
#ifndef _CONSOLE
			MessageBox(NULL, text.c_str(), title.c_str(), MB_OK);
#else
			ConsoleOutput(FormattingConsoleTimeShow(title) + TEXT(": ") + text + TEXT("\n"));
#endif
		}

		template<class T = bool>
		void Logs_ustr(Ustr& text, LogMessage lgm)
		{
			switch (lgm)
			{
			case LogMessage::ts:
			{
				if (Mode_Cmd)
				{
					ConsoleOutput(FormattingConsoleTimeShow(TEXT(Log_ts)) + TEXT(": ") + text + TEXT("\n"));
					break;
				}
#ifndef _CONSOLE
				MessageBox(NULL, text.c_str(), TEXT(Log_ts), MB_OK);
#else
				ConsoleOutput(FormattingConsoleTimeShow(TEXT(Log_ts)) + TEXT(": ") + text + TEXT("\n"));
#endif
				break;
			}
			case LogMessage::wr:
			{
				//SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //黄色: red | green

				if (Mode_Cmd)
				{
					//Ustr ConsoleText = (Ustr)FormattingConsoleTimeShow(TEXT(Log_wr)) + TEXT(": ") + text + TEXT("\n");
					ConsoleOutput((Ustr)FormattingConsoleTimeShow(TEXT(Log_wr)) + TEXT(": ") + text + TEXT("\n"),
						FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

					break;
				}
#ifndef _CONSOLE
				MessageBox(NULL, text.c_str(), TEXT(Log_wr), MB_ICONWARNING);
#else
				//Ustr ConsoleText = (Ustr)FormattingConsoleTimeShow(TEXT(Log_wr)) + TEXT(": ") + text + TEXT("\n");
				ConsoleOutput((Ustr)FormattingConsoleTimeShow(TEXT(Log_wr)) + TEXT(": ") + text + TEXT("\n"),
					FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#endif

				break;
			}
			case LogMessage::er:
			{
				if (Mode_Cmd)
				{
					//Ustr ConsoleText = (Ustr)FormattingConsoleTimeShow(TEXT(Log_er)) + TEXT(": ") + text + TEXT("\n");
					ConsoleOutput_Error((Ustr)FormattingConsoleTimeShow(TEXT(Log_er)) + TEXT(": ") + text + TEXT("\n"),
						FOREGROUND_RED | FOREGROUND_INTENSITY);

					break;
				}
#ifndef _CONSOLE
				MessageBox(NULL, text.c_str(), TEXT(Log_er), MB_ICONSTOP);
#else
				//Ustr ConsoleText = (Ustr)FormattingConsoleTimeShow(TEXT(Log_er)) + TEXT(": ") + text + TEXT("\n");
				ConsoleOutput_Error((Ustr)FormattingConsoleTimeShow(TEXT(Log_er)) + TEXT(": ") + text + TEXT("\n"),
					FOREGROUND_RED | FOREGROUND_INTENSITY);
#endif

				break;
			}
			case LogMessage::nl:
			{
				if (Mode_Cmd)
				{
					ConsoleOutput(FormattingConsoleTimeShow(text) + TEXT("\n"));
					break;
				}
#ifndef _CONSOLE
				MessageBox(NULL, text.c_str(), TEXT(Log_nl), MB_OK);
#else
				ConsoleOutput(FormattingConsoleTimeShow(text) + TEXT("\n"));
#endif
				break;
			}
			}
		}
		template<class T = bool>
		void Logs_ustr(Ustr&& text, LogMessage lgm)
		{
			switch (lgm)
			{
			case LogMessage::ts:
			{
				if (Mode_Cmd)
				{
					ConsoleOutput(FormattingConsoleTimeShow(TEXT(Log_ts)) + TEXT(": ") + text + TEXT("\n"));
					break;
				}
#ifndef _CONSOLE
				MessageBox(NULL, text.c_str(), TEXT(Log_ts), MB_OK);
#else
				ConsoleOutput(FormattingConsoleTimeShow(TEXT(Log_ts)) + TEXT(": ") + text + TEXT("\n"));
#endif
				break;
			}
			case LogMessage::wr:
			{
				//SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //黄色: red | green

				if (Mode_Cmd)
				{
					//Ustr ConsoleText = (Ustr)FormattingConsoleTimeShow(TEXT(Log_wr)) + TEXT(": ") + text + TEXT("\n");
					ConsoleOutput((Ustr)FormattingConsoleTimeShow(TEXT(Log_wr)) + TEXT(": ") + text + TEXT("\n"),
						FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

					break;
				}
#ifndef _CONSOLE
				MessageBox(NULL, text.c_str(), TEXT(Log_wr), MB_ICONWARNING);
#else
				//Ustr ConsoleText = (Ustr)FormattingConsoleTimeShow(TEXT(Log_wr)) + TEXT(": ") + text + TEXT("\n");
				ConsoleOutput((Ustr)FormattingConsoleTimeShow(TEXT(Log_wr)) + TEXT(": ") + text + TEXT("\n"),
					FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#endif

				break;
			}
			case LogMessage::er:
			{
				if (Mode_Cmd)
				{
					//Ustr ConsoleText = (Ustr)FormattingConsoleTimeShow(TEXT(Log_er)) + TEXT(": ") + text + TEXT("\n");
					ConsoleOutput_Error((Ustr)FormattingConsoleTimeShow(TEXT(Log_er)) + TEXT(": ") + text + TEXT("\n"),
						FOREGROUND_RED | FOREGROUND_INTENSITY);

					break;
				}
#ifndef _CONSOLE
				MessageBox(NULL, text.c_str(), TEXT(Log_er), MB_ICONSTOP);
#else
				//Ustr ConsoleText = (Ustr)FormattingConsoleTimeShow(TEXT(Log_er)) + TEXT(": ") + text + TEXT("\n");
				ConsoleOutput_Error((Ustr)FormattingConsoleTimeShow(TEXT(Log_er)) + TEXT(": ") + text + TEXT("\n"),
					FOREGROUND_RED | FOREGROUND_INTENSITY);
#endif

				break;
			}
			case LogMessage::nl:
			{
				if (Mode_Cmd)
				{
					ConsoleOutput(FormattingConsoleTimeShow(text) + TEXT("\n"));
					break;
				}
#ifndef _CONSOLE
				MessageBox(NULL, text.c_str(), TEXT(Log_nl), MB_OK);
#else
				ConsoleOutput(FormattingConsoleTimeShow(text) + TEXT("\n"));
#endif
				break;
			}
			}
		}
		template<class T = bool>
		void Logs_lgm(LogMessage lgm)
		{
			if (lgm == LogMessage::nl)
			{
				if (this->Mode_Cmd)
				{
					ConsoleOutput(TEXT("\n"));
					return;
				}

				ConsoleOutput(TEXT("\n"));
			}
		}


		template<class T = bool>
		void Logs(Ustr& text, Ustr& title)
		{
			if (ShowLog)
			{
#ifdef _DEBUG
				Logs_ustr_ustr(text, title);
				return;
#endif
				if (Release)
				{
					Logs_ustr_ustr(text, title);
				}
			}
		}
		template<class T = bool>
		void Logs(Ustr&& text, Ustr&& title)
		{
			if (ShowLog)
			{
#ifdef _DEBUG
				Logs_ustr_ustr(text, title);
				return;
#endif
				if (Release)
				{
					Logs_ustr_ustr(text, title);
				}
			}
		}
		template<class T = bool>
		void Logs(Ustr& text, Ustr&& title)
		{
			if (ShowLog)
			{
#ifdef _DEBUG
				Logs_ustr_ustr(text, title);
				return;
#endif
				if (Release)
				{
					Logs_ustr_ustr(text, title);
				}
			}
		}
		template<class T = bool>
		void Logs(Ustr&& text, Ustr& title)
		{
			if (ShowLog)
			{
#ifdef _DEBUG
				Logs_ustr_ustr(text, title);
				return;
#endif
				if (Release)
				{
					Logs_ustr_ustr(text, title);
				}
			}
		}

		template<class T = bool>
		void Logs(Ustr& text, LogMessage lgm)
		{
			if (ShowLog)
			{
#ifdef _DEBUG
				Logs_ustr(text, lgm);
				return;
#endif
				if (Release)
				{
					Logs_ustr(text, lgm);
				}
			}
		}
		template<class T = bool>
		void Logs(Ustr&& text, LogMessage lgm)
		{
			if (ShowLog)
			{
#ifdef _DEBUG
				Logs_ustr(text, lgm);
				return;
#endif
				if (Release)
				{
					Logs_ustr(text, lgm);
				}
			}
		}
		template<class T = bool>
		void Logs(LogMessage lgm)
		{
			if (ShowLog)
			{
#ifdef _DEBUG
				Logs_lgm(lgm);
				return;
#endif
				if (Release)
				{
					Logs_lgm(lgm);
				}
			}
		}

	public:
		template<class T = bool>
		void operator()(Ustr& text, Ustr& title)
		{
			Logs(text, title);
		}
		template<class T = bool>
		void operator()(Ustr&& text, Ustr&& title)
		{
			Logs(text, title);
		}
		template<class T = bool>
		void operator()(Ustr& text, Ustr&& title)
		{
			Logs(text, title);
		}
		template<class T = bool>
		void operator()(Ustr&& text, Ustr& title)
		{
			Logs(text, title);
		}
		template<class T = bool>
		void operator()(Ustr& text, LogMessage lgm = lgm::ts)
		{
			Logs(text, lgm);
		}
		template<class T = bool>
		void operator()(Ustr&& text, LogMessage lgm = lgm::ts)
		{
			Logs(text, lgm);
		}
		template<class T = bool>
		void operator()(LogMessage lgm = lgm::nl)
		{
			Logs(lgm);
		}
	};

	//模式 mode: _DEBUG & (_CONSOLE | _WINDOWS)
	static Log lg(false, false);
	//模式 mode: _DEBUG & (_CONSOLE | _WINDOWS)
	static Log lgc(false, true);
	//模式 mode: #ifndef _DEBUG(Release) & (_CONSOLE | _WINDOWS)
	static Log lgr(true, false);
	//模式 mode: #ifndef _DEBUG(Release) & _CONSOLE
	static Log lgcr(true, true);

	template<class Temp = bool>
	void README()
	{
		lgc(TEXT("Tools_Tool::使用说明()"), lgm::nl);
		lgc(TEXT(" 支持 Unicode: #define UNICODE"), lgm::nl);
		lgc(TEXT(" 使用方法-函数对象, 代码如下:"), lgm::nl);
		lgc(TEXT(" 输出[_CONSOLE | _WINDOWS](_DEBUG)"), TEXT("lg()"));
		lgc(TEXT(" 输出[_CONSOLE | _WINDOWS](_DEBUG | Release)"), TEXT("lgr()"));
		lgc(TEXT(" CMD输出[_WINDOWS](_DEBUG)"), TEXT("lgc()"));
		lgc(TEXT(" CMD输出[_WINDOWS](_DEBUG | Release)"), TEXT("lgcr()"));
		lgc(TEXT("\n"), lgm::nl);
		lgc(TEXT("lgm"), TEXT("enum"));
		lgc(TEXT("\t#ifdef _English"), lgm::nl);
		lgc(TEXT("\tlgm::nl,  //Empty title (without title): Only the content is output"), lgm::nl);
		lgc(TEXT("\tlgm::ts,  //Tips"), lgm::nl);
		lgc(TEXT("\tlgm::wr,  //Warning"), lgm::nl);
		lgc(TEXT("\tlgm::er   //Error"), lgm::nl);
		lgc(TEXT("\n"), lgm::nl);
		lgc(TEXT("\t#ifndef _English"), lgm::nl);
		lgc(TEXT("\tlgm::nl,  //空标题(不带标题): 只输出内容"), lgm::nl);
		lgc(TEXT("\tlgm::ts,  //提示"), lgm::nl);
		lgc(TEXT("\tlgm::wr,  //警告"), lgm::nl);
		lgc(TEXT("\tlgm::er   //错误"), lgm::nl);
		lgc(TEXT("\n"), lgm::nl);
		lgc(TEXT("日志显示开关"), lgm::nl);
		lgc(TEXT("\tTools_Tool::设置日志显示(bool)"), lgm::nl);
		lgc(TEXT("\tTools_Tool::设置时间显示(bool)"), lgm::nl);
		lgc(TEXT("\tTools_Tool::Log::设置日志显示(bool)"), lgm::nl);
		lgc(TEXT("\tTools_Tool::Log::设置时间显示(bool)"), lgm::nl);
		lgc(TEXT(""), lgm::nl);
		lgc(TEXT(""), lgm::nl);
		lgc(TEXT(""), lgm::nl);
		lgc(TEXT(""), lgm::nl);
		lgc(TEXT("Tools_Tool::README()"), lgm::nl);
		lgc(TEXT(" Unicode support: #define UNICODE"), lgm::nl);
		lgc(TEXT(" Using method - function object, the code is as follows:"), lgm::nl);
		lgc(TEXT(" Print [_CONSOLE | _WINDOWS](_DEBUG)"), TEXT("lg()"));
		lgc(TEXT(" Print [_CONSOLE | _WINDOWS](_DEBUG | Release)"), TEXT("lgr()"));
		lgc(TEXT(" Command Print [_WINDOWS](_DEBUG)"), TEXT("lgc()"));
		lgc(TEXT(" Command Print [_WINDOWS](_DEBUG | Release)"), TEXT("lgcr()"));
		lgc(TEXT("\n"), lgm::nl);
		lgc(TEXT("\n"), lgm::nl);
		lgc(TEXT("lgm"), TEXT("enum"));
		lgc(TEXT("\t#ifdef _English"), lgm::nl);
		lgc(TEXT("\tlgm::nl,  //Empty title (without title): Only the content is output"), lgm::nl);
		lgc(TEXT("\tlgm::ts,  //Tips"), lgm::nl);
		lgc(TEXT("\tlgm::wr,  //Warning"), lgm::nl);
		lgc(TEXT("\tlgm::er   //Error"), lgm::nl);
		lgc(TEXT("\n"), lgm::nl);
		lgc(TEXT("\t#ifndef _English"), lgm::nl);
		lgc(TEXT("\tlgm::nl,  //空标题(不带标题): 只输出内容"), lgm::nl);
		lgc(TEXT("\tlgm::ts,  //提示"), lgm::nl);
		lgc(TEXT("\tlgm::wr,  //警告"), lgm::nl);
		lgc(TEXT("\tlgm::er   //错误"), lgm::nl);
		lgc(TEXT("\n"), lgm::nl);
		lgc(TEXT("Log display switch:"), lgm::nl);
		lgc(TEXT("\tTools_Tool::SetShowLog(bool)"), lgm::nl);
		lgc(TEXT("\tTools_Tool::SetConsoleTimeShow(bool)"), lgm::nl);
		lgc(TEXT("\tTools_Tool::Log::SetShowLog(bool)"), lgm::nl);
		lgc(TEXT("\tTools_Tool::Log::SetConsoleTimeShow(bool)"), lgm::nl);
	}

	void SetShowLog(bool showLog);
#define 设置日志显示 SetShowLog
	void SetConsoleTimeShow(bool show);
#define 设置时间显示 SetConsoleTimeShow

#define 使用说明 README
}


#endif