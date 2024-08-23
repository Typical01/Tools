#pragma once

#ifndef _LOG_H
#define _LOG_H

#define _CRT_SECURE_NO_WARNINGS

//#define _CONSOLE
//#define _English
//#undef _WINDOWS


#include "Tools_Tool.h"
//#include "Timers.h"
//#include "CommonTools.h"
//#include "CharHandleOfConfigFile.h"

#include <iostream>
#include <cstdio>
#include <string>

#include <iomanip>
#include <ctime>
#include <sstream>
#include <locale>
#include <chrono>

#include <cwchar>
#include <cstring>
#include <stdexcept>

#ifdef _WINDOWS
	#include <Windows.h>
#else
	//Linux 和 macOS
	#include <iconv.h>
#endif

//using namespace Tools_Tool::StringHandling;


#ifdef _WINDOWS
#else
	// 定义虚拟类型
	using DWORD = unsigned long;
	using WORD = unsigned short;
	using HWND = void*;
	using HANDLE = void*;
#endif

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

#ifndef UNICODE

	#define Uchar char
	#define Ustr std::string
	#define Ucout std::cout
	#define Ucerr std::cerr
	#define Uto_string std::to_string
	#define Ustrlen std::strlen
	#define Uostringstream std::ostringstream
	#define Ufopen_s fopen_s
	#define Ufputs fputs
	#define Ufgets fgets
	#define Ufputs fputs


#else

	#define Uchar wchar_t
	#define Ustr std::wstring
	#define Ucout std::wcout
	#define Ucerr std::wcerr
	#define Uto_string std::to_wstring
	#define Ustrlen std::wcslen
	#define Uostringstream std::Uostringstream
	#define Ufopen_s Ufopen_s
	#define Ufputs Ufputs
	#define Ufgets Ufgets
	#define Ufputs Ufputs

#endif


#ifndef UNICODE
	// T("") | ""
	#define T(x) x
#else
	// T("") | ""
	#define T(x) L ## x
#endif

//Use of English requires comments: From translation software
//#define _English
//#define UNICODE // Visual Studio defines macros by default

//非中文: Not Chinese
#ifdef _English
#ifndef UNICODE

#define LogString_nl ""
#define LogString_ts "Tips"
#define LogString_wr "Warning"
#define LogString_er "Error"
#else

#define LogString_nl T("")
#define LogString_ts T("Tips")
#define LogString_wr T("Warning")
#define LogString_er T("Error")
#endif

#else
//中文
#ifndef UNICODE

#define Log字符串_空行 ""
#define Log字符串_提示 "提示"
#define Log字符串_警告 "警告"
#define Log字符串_错误 "错误"
#else

#define Log字符串_空行 T("")
#define Log字符串_提示 T("提示")
#define Log字符串_警告 T("警告")
#define Log字符串_错误 T("错误")
#endif

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
	TOOLS_TOOL_API extern HWND hConsole;
	TOOLS_TOOL_API extern HANDLE hConsoleOutput;
	TOOLS_TOOL_API extern HANDLE hConsoleError;
	
	TOOLS_TOOL_API extern FILE* LogFileStream;
	TOOLS_TOOL_API extern bool LogFileWrite;
	TOOLS_TOOL_API extern bool LogAllOutput;


	class TOOLS_TOOL_API Log {
	private:
		static bool FirstInitCMD; //控制台初始化

		bool CMD; //控制台
		bool Release;

		bool ShowTime;
		bool ShowLog;

	public:
		Log(bool cmd, bool release)
			: CMD(cmd), Release(release), ShowTime(true), ShowLog(true) {	}

		~Log();

	public:
		/*
		* 控制台初始化状态: false(需要初始化), true(跳过初始化)
		*/
		template<class Temp = bool>
		void Init()
		{
			//控制台初始化
			if (!FirstInitCMD)
			{
				Ucout.imbue(std::locale(T("")));

#ifndef _CONSOLE
#ifdef _WINDOWS
#if 0
				//分离控制台
				if (FreeConsole() == 0)
				{
#ifdef _English
					MessageBox(0, T("log: detached console failure!"), T("error"), MB_ICONSTOP);
					MessageBox(0, (T("error code: ") + Uto_string(GetLastError())).c_str(), T(Log_er), MB_ICONSTOP);
#else

					MessageBox(0, T("log: 分离控制台失败!"), T(Log_er), MB_ICONSTOP);
					MessageBox(0, (T("错误代码: ") + Uto_string(GetLastError())).c_str(), T(Log_er), MB_ICONSTOP);
#endif
				}

				//分配控制台
				if (AllocConsole() == 0)
				{
#ifdef _English
					MessageBox(0, T("log: assignment console failure!"), T(Log_er), MB_ICONSTOP);
					MessageBox(0, (T("error code: ") + Uto_string(GetLastError())).c_str(), T(Log_er), MB_ICONSTOP);
#else
					MessageBox(0, T("log: 分配控制台失败!"), T(Log_er), MB_ICONSTOP);
					MessageBox(0, (T("错误代码: ") + Uto_string(GetLastError())).c_str(), T(Log_er), MB_ICONSTOP);
#endif
				}
#endif


				//GetStdHandle() -> setconsoletextcolor()的过程中
				//好像不能将其放在静态对象的类(static class Log lg)中, 需要获取到标准输出/错误/其他的句柄后, 再修改颜色
				//否则, 将不打印文本信息!!!

				hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
				if (hConsoleOutput == INVALID_HANDLE_VALUE) {
#ifdef _English
					MessageBox(0, T("log: get STD_OUTPUT_HANDLE fail!"), T(Log_er), MB_ICONSTOP);
#else
					MessageBox(0, T("log: 获取 STD_OUTPUT_HANDLE fail!"), T(Log_er), MB_ICONSTOP);
#endif
				}

				hConsoleError = GetStdHandle(STD_ERROR_HANDLE);
				if (hConsoleError == INVALID_HANDLE_VALUE) {
#ifdef _English
					MessageBox(0, T("log: get STD_ERROR_HANDLE fail!"), T(Log_er), MB_ICONSTOP);
#else
					MessageBox(0, T("log: 获取 STD_ERROR_HANDLE fail!"), T(Log_er), MB_ICONSTOP);
#endif
				}

				// 获取控制台窗口的句柄
				hConsole = GetConsoleWindow();

#endif

				//跨平台文件流(宽字符版本)
				if (LogFileWrite) {
					std::chrono::system_clock::time_point now = std::chrono::system_clock::now();;
					std::time_t tm = std::chrono::system_clock::to_time_t(now);
					std::tm* now_tm = std::localtime(&tm);
					Uostringstream oss;
					oss << std::put_time(now_tm, T("%Y-%m-%d_%H-%M-%S")); // 自定义时间格式
	
					//获取 当前路径/Log/Log文件名.txt 
					//创建文件夹 Log
					Ustr Log_FolderName = Get程序目录路径() + T("\\Log");
					if (CreateFolder(Log_FolderName)) {
						Ucout << T("Log 文件夹: 创建成功!\n\n");
					}

					//Log文件名: 格式化日期时间(年-月-日_时-分-秒) + _程序名.txt
					Ustr Log_FileName = oss.str() + T("_") + Get程序名() + T(".txt");
					Ustr Log_FilePath = Log_FolderName + T("\\") + Log_FileName;

					//打开文件
					errno_t err = Ufopen_s(&LogFileStream, Log_FilePath.c_str(), T("a+, ccs=UTF-8")); //追加模式打开文件
					if (err != 0)
					{
						Ucout << (Ustr)T("文件: ") + Log_FilePath + T(" 打开错误!") << std::endl;
						if (LogFileStream == nullptr) {
							Ucout << (Ustr)T("LogFileStream errno_t: ") + Uto_string(err) << std::endl;
						}
						//return false; //没有文件, 则不进行读取和解析
					}
					Ucout << (Ustr)T("文件: ") + Log_FilePath + T(" 打开成功") << std::endl;
				}
#endif

				//完成初始化
				FirstInitCMD = true; 
			}
			else { //跳过初始化
				return;
			}
		}

	private:

		std::wstring StringToWstring(const std::string& str);
		std::string WstringToString(const std::wstring& wStr);
		void SetConsoleTextColor(WORD wColor);
		void SetConsoleTextColor_Error(WORD wColor);

		//非 WORD 版本不会还原文本颜色
		template<class T = bool>
		void ConsoleOutput(Ustr& text)
		{
			if (hConsoleOutput != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleOutput, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
				}
			}
		}
		template<class T = bool>
		void ConsoleOutput(Ustr& text, WORD wColor)
		{
			SetConsoleTextColor(wColor);

			if (hConsoleOutput != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleOutput, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
					SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
			}
			SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		template<class T = bool>
		void ConsoleOutput(Ustr&& text)
		{
			if (hConsoleOutput != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleOutput, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
				}
			}
		}
		template<class T = bool>
		void ConsoleOutput(Ustr&& text, WORD wColor)
		{
			SetConsoleTextColor(wColor);

			if (hConsoleOutput != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleOutput, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
					SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
			}
			SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		template<class T = bool>
		void ConsoleOutput_Error(Ustr& text)
		{
			if (hConsoleError != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleError, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
				}
				Ucout.flush(); // 刷新缓冲区;
			}
		}
		template<class T = bool>
		void ConsoleOutput_Error(Ustr& text, WORD wColor)
		{
			SetConsoleTextColor_Error(wColor);

			if (hConsoleError != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleError, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
					SetConsoleTextColor_Error(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
				Ucout.flush(); // 刷新缓冲区;
			}
			SetConsoleTextColor_Error(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		template<class T = bool>
		void ConsoleOutput_Error(Ustr&& text)
		{
			if (hConsoleError != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleError, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
				}
			}
		}
		template<class T = bool>
		void ConsoleOutput_Error(Ustr&& text, WORD wColor)
		{
			SetConsoleTextColor_Error(wColor);

			if (hConsoleError != INVALID_HANDLE_VALUE) {
				if (!WriteConsole(hConsoleError, text.c_str(), Ustrlen(text.c_str()), &written, NULL)) {
					SetConsoleTextColor_Error(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
				Ucout.flush(); // 刷新缓冲区;
			}
			SetConsoleTextColor_Error(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}

		// _WINDOWS || _CONSOLE
		template<class T = bool>
		void Logs_ustr_ustr(Ustr& text, Ustr& title)
		{
			if (CMD)
			{
				Ustr temp = FormattingConsoleTimeShow(title) + T(": ") + text + T("\n");
#ifdef _WINDOWS
				ConsoleOutput(temp);
#else
				Ucout << temp;
#endif

				//文件写入 log日志
				if (LogFileWrite) {
					if (LogAllOutput) {
						Ufputs(temp.c_str(), LogFileStream);
					}
				}

				return;
			}
			MessageBox(NULL, text.c_str(), title.c_str(), MB_OK);
		}
		template<class T = bool>
		void Logs_ustr_ustr(Ustr&& text, Ustr&& title)
		{
			if (CMD)
			{
				Ustr temp = FormattingConsoleTimeShow(title) + T(": ") + text + T("\n");
#ifdef _WINDOWS
				ConsoleOutput(temp);
#else
				Ucout << temp;
#endif
				//文件写入 log日志
				if (LogFileWrite) {
					if (LogAllOutput) {
						Ufputs(temp.c_str(), LogFileStream);
					}
				}

				return;
			}
			MessageBox(NULL, text.c_str(), title.c_str(), MB_OK);
		}
		template<class T = bool>
		void Logs_ustr_ustr(Ustr& text, Ustr&& title)
		{
			if (CMD)
			{
				Ustr temp = FormattingConsoleTimeShow(title) + T(": ") + text + T("\n");
#ifdef _WINDOWS
				ConsoleOutput(temp);
#else
				Ucout << temp;
#endif

				//文件写入 log日志
				if (LogFileWrite) {
					if (LogAllOutput) {
						Ufputs(temp.c_str(), LogFileStream);
					}
				}

				return;
			}
			MessageBox(NULL, text.c_str(), title.c_str(), MB_OK);
		}
		template<class T = bool>
		void Logs_ustr_ustr(Ustr&& text, Ustr& title)
		{
			if (CMD)
			{
				Ustr temp = FormattingConsoleTimeShow(title) + T(": ") + text + T("\n");
#ifdef _WINDOWS
				ConsoleOutput(temp);
#else
				Ucout << temp;
#endif

				//文件写入 log日志
				if (LogFileWrite) {
					if (LogAllOutput) {
						Ufputs(temp.c_str(), LogFileStream);
					}
				}

				return;
			}
			MessageBox(NULL, text.c_str(), title.c_str(), MB_OK);
		}

		template<class T = bool>
		void Logs_ustr(Ustr& text, LogMessage lgm)
		{
			switch (lgm)
			{
			case LogMessage::ts:
			{
				if (CMD)
				{
					Ustr temp = FormattingConsoleTimeShow(T(Log_ts)) + T(": ") + text + T("\n");
#ifdef _WINDOWS
					ConsoleOutput(temp, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
					Ucout << temp;
#endif

					//文件写入 log日志
					if (LogFileWrite) {
						if (LogAllOutput) {
							Ufputs(temp.c_str(), LogFileStream);
						}
					}

					break;
				}
				MessageBox(NULL, text.c_str(), T(Log_ts), MB_OK);

				break;
			}
			case LogMessage::wr:
			{
				if (CMD)
				{
					Ustr temp = FormattingConsoleTimeShow(T(Log_wr)) + T(": ") + text + T("\n");
#ifdef _WINDOWS
					ConsoleOutput(temp, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
					Ucout << temp;
#endif
					//文件写入 log日志
					if (LogFileWrite) {
						if (LogAllOutput) {
							Ufputs(temp.c_str(), LogFileStream);
						}
					}

					break;
				}
				MessageBox(NULL, text.c_str(), T(Log_wr), MB_ICONWARNING);

				break;
			}
			case LogMessage::er:
			{
				if (CMD)
				{
					Ustr temp = FormattingConsoleTimeShow(T(Log_er)) + T(": ") + text + T("\n");
#ifdef _WINDOWS
					ConsoleOutput_Error(temp, FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
					Ucerr << temp;
#endif
					//文件写入 log日志
					if (LogFileWrite) {
						Ufputs(temp.c_str(), LogFileStream);
					}

					break;
				}
				MessageBox(NULL, text.c_str(), T(Log_er), MB_ICONSTOP);

				break;
			}
			case LogMessage::nl:
			{
				if (CMD)
				{
					Ustr temp = FormattingConsoleTimeShow(text) + T("\n");
#ifdef _WINDOWS
					ConsoleOutput(temp);
#else
					Ucout << temp;
#endif
					//文件写入 log日志
					if (LogFileWrite) {
						if (LogAllOutput) {
							Ufputs(temp.c_str(), LogFileStream);
						}
					}

					break;
				}
				MessageBox(NULL, text.c_str(), T(Log_nl), MB_OK);
				
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
				if (CMD)
				{
					Ustr temp = FormattingConsoleTimeShow(T(Log_ts)) + T(": ") + text + T("\n");
#ifdef _WINDOWS
					ConsoleOutput(temp, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
					Ucout << temp;
#endif
					//文件写入 log日志
					if (LogFileWrite) {
						if (LogAllOutput) {
							Ufputs(temp.c_str(), LogFileStream);
						}
					}

					break;
				}
				MessageBox(NULL, text.c_str(), T(Log_ts), MB_OK);

				break;
			}
			case LogMessage::wr:
			{
				if (CMD)
				{
					Ustr temp = FormattingConsoleTimeShow(T(Log_wr)) + T(": ") + text + T("\n");
#ifdef _WINDOWS
					ConsoleOutput(temp, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
					Ucout << temp;
#endif
					//文件写入 log日志
					if (LogFileWrite) {
						if (LogAllOutput) {
							Ufputs(temp.c_str(), LogFileStream);
						}
					}

					break;
				}
				MessageBox(NULL, text.c_str(), T(Log_wr), MB_ICONWARNING);

				break;
			}
			case LogMessage::er:
			{
				if (CMD)
				{
					Ustr temp = FormattingConsoleTimeShow(T(Log_er)) + T(": ") + text + T("\n");
#ifdef _WINDOWS
					ConsoleOutput_Error(temp, FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
					Ucerr << temp;
#endif
					//文件写入 log日志
					if (LogFileWrite) {
						Ufputs(temp.c_str(), LogFileStream);
					}

					break;
				}
				MessageBox(NULL, text.c_str(), T(Log_er), MB_ICONSTOP);

				break;
			}
			case LogMessage::nl:
			{
				if (CMD)
				{
					Ustr temp = FormattingConsoleTimeShow(text) + T("\n");
#ifdef _WINDOWS
					ConsoleOutput(temp);
#else
					Ucout << temp;
#endif
					//文件写入 log日志
					if (LogFileWrite) {
						if (LogAllOutput) {
							Ufputs(temp.c_str(), LogFileStream);
						}
					}

					break;
				}
				MessageBox(NULL, text.c_str(), T(Log_nl), MB_OK);

				break;
			}
			}
		}
		template<class T = bool>
		void Logs_lgm(LogMessage lgm)
		{
			if (lgm == LogMessage::nl)
			{
				if (this->CMD)
				{
					Ustr temp = T("\n");
					ConsoleOutput(temp);

					//文件写入 log日志
					if (LogFileWrite) {
						if (LogAllOutput) {
							Ufputs(temp.c_str(), LogFileStream);
						}
					}

					return;
				}
			}
		}


		// _DEBUG || NDEBUG
		template<class T = bool>
		void Logs(Ustr& text, Ustr& title)
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
		template<class T = bool>
		void Logs(Ustr&& text, Ustr&& title)
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
		template<class T = bool>
		void Logs(Ustr& text, Ustr&& title)
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
		template<class T = bool>
		void Logs(Ustr&& text, Ustr& title)
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
		
		template<class T = bool>
		void Logs(Ustr& text, LogMessage lgm)
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
		template<class T = bool>
		void Logs(Ustr&& text, LogMessage lgm)
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
		template<class T = bool>
		void Logs(LogMessage lgm)
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

	public:

		//显示/隐藏 Log消息
		template<class T = bool>
		void operator()(Ustr& text, Ustr& title)
		{
			Init();
			if (ShowLog)
			{
				Logs(text, title);
			}
		}
		template<class T = bool>
		void operator()(Ustr&& text, Ustr&& title)
		{
			Init();
			if (ShowLog)
			{
				Logs(text, title);
			}
		}
		template<class T = bool>
		void operator()(Ustr& text, Ustr&& title)
		{
			Init();
			if (ShowLog)
			{
				Logs(text, title);
			}
		}
		template<class T = bool>
		void operator()(Ustr&& text, Ustr& title)
		{
			Init();
			if (ShowLog)
			{
				Logs(text, title);
			}
		}
		template<class T = bool>
		void operator()(Ustr& text, LogMessage lgm = lgm::nl)
		{
			Init();
			if (ShowLog)
			{
				Logs(text, lgm);
			}
		}
		template<class T = bool>
		void operator()(Ustr&& text, LogMessage lgm = lgm::nl)
		{
			Init();
			if (ShowLog)
			{
				Logs(text, lgm);
			}
		}
		template<class T = bool>
		void operator()(LogMessage lgm = lgm::nl)
		{
			Init();
			if (ShowLog)
			{
				Logs(lgm);
			}
		}


	public:

		template<class T = bool>
		Ustr FormattingConsoleTimeShow(Ustr& text)
		{
			if (ShowTime) {
				std::chrono::system_clock::time_point now = std::chrono::system_clock::now();;
				// 获取当前时间点（自epoch以来的时间）
				// 将时间点转换为time_t（用于localtime函数）
				std::time_t tm = std::chrono::system_clock::to_time_t(now);
				// 使用localtime函数将time_t转换为本地时间（std::tm结构）
				std::tm* now_tm = std::localtime(&tm);

				// 使用 std::put_time 格式化时间
				Uostringstream oss;
				oss << std::put_time(now_tm, T("%Y-%m-%d %H:%M:%S")); // 自定义时间格式
				Ustr temp = (Ustr)T("[") + oss.str() + T("]") + text;

				return temp;
			}
			return text;
		}
		template<class T = bool>
		Ustr FormattingConsoleTimeShow(Ustr&& text)
		{
			if (ShowTime) {
				std::chrono::system_clock::time_point now = std::chrono::system_clock::now();;
				// 获取当前时间点（自epoch以来的时间）
				// 将时间点转换为time_t（用于localtime函数）
				std::time_t tm = std::chrono::system_clock::to_time_t(now);
				// 使用localtime函数将time_t转换为本地时间（std::tm结构）
				std::tm* now_tm = std::localtime(&tm);

				// 使用 std::put_time 格式化时间
				Uostringstream oss;
				oss << std::put_time(now_tm, T("%Y-%m-%d %H:%M:%S")); // 自定义时间格式
				Ustr temp = (Ustr)T("[") + oss.str() + T("]") + text;

				return temp;
			}
			return text;
		}

		template<class T = bool>
		Ustr 提取程序名(Ustr path)
		{
			//匹配 '\' && '/' 任意
			size_t lastSepPos = path.find_last_of(T("\\/"));
			if (lastSepPos != std::wstring::npos) {
				return path.substr(lastSepPos + 1, ((Ustr)T(".exe")).length()); // 提取文件名部分
			}

			// 去掉 .exe 后缀
			size_t exePos = path.find_last_of(T(".exe"));
			if (exePos != Ustr::npos && exePos == path.length() - 4) {
				path = path.substr(0, exePos); // 去掉 .exe 后缀
			}

			return path; // 如果找不到路径分隔符，则返回整个路径
		}
		template<class T = bool>
		Ustr 提取程序目录路径(Ustr path)
		{
			size_t lastSepPos = path.find_last_of(T("\\/"));
			if (lastSepPos != std::wstring::npos) {
				return path.substr(0, lastSepPos + 1); // 包括最后一个路径分隔符
			}
			return T(""); // 如果找不到路径分隔符，则返回空字符串
		}
		template<class T = bool>
		Ustr Get程序名()
		{
			Uchar exePath[MAX_PATH];
			Ustr exeName;

			//获取当前程序的全路径
			DWORD length = GetModuleFileName(NULL, exePath, MAX_PATH);

			if (length > 0 && length < MAX_PATH) {
				exeName = 提取程序名(exePath);
				//lgc(T("当前可执行文件的名称: ") + exeName);
			}
			else {
				//lgc(T("无法获取当前可执行文件的路径!"));
			}
			return exeName;
		}
		template<class T = bool>
		Ustr Get程序目录路径()
		{
			Uchar exePath[MAX_PATH];
			Ustr folderName;

			//获取当前程序的全路径
			DWORD length = GetModuleFileName(NULL, exePath, MAX_PATH);

			if (length > 0 && length < MAX_PATH) {
				folderName = 提取程序目录路径(exePath);
				//lgc(T("当前程序目录路径名: ") + folderName);
			}
			else {
				//lgc(T("无法获取当前可执行文件的路径!"));
			}

			return folderName;
		}

		template<class T = bool>
		bool CreateFolder(const Ustr& folderPath)
		{
			DWORD attributes = GetFileAttributes(folderPath.c_str());

			// 检查路径是否存在且不是目录  
			if (attributes == INVALID_FILE_ATTRIBUTES)
			{
				// 路径不存在或出错，尝试创建目录  
				if (CreateDirectory(folderPath.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
				{
					//lgc(T("文件夹") + folderPath + T("创建成功(或已存在)!"));
					// 创建成功或路径已存在  
					return true;
				}
				//lgc(T("文件夹") + folderPath + T("创建失败!"));
				// 创建失败且不是因为路径已存在  
				return false;
			}
			else if (attributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//lgc(T("文件夹") + folderPath + T("已存在"));
				// 路径已经是一个目录  
				return true;
			}
			//lgc(T("文件夹") + folderPath + T("创建失败(路径存在, 但不是目录)!"));
			// 路径存在但不是目录（可能是一个文件）  
			return false;
		}


	public:

		void SetShowLog(bool showLog);
#define 设置日志显示 SetShowLog
		void SetConsoleTimeShow(bool showTime);
#define 设置时间显示 SetConsoleTimeShow

		template<class T = bool>
		void SetConsoleShow(bool showConsole, bool isTips = false)
		{
			//显示/隐藏 窗口
			if (showConsole) {
				ShowWindow(hConsole, SW_SHOWDEFAULT);
				if (isTips) {
					MessageBox(0, T("Windows: Show CMD"), Log_er, MB_ICONSTOP);
				}
			}
			else {
				ShowWindow(hConsole, SW_HIDE);
				if (isTips) {
					MessageBox(0, T("Windows: Not Show CMD"), Log_er, MB_ICONSTOP);
				}
			}
		}
#define 设置控制台显示 SetConsoleShow

	public:

			static void SetAllShowLog(bool showLog);
#define 设置日志显示_全部 SetAllShowLog
			static void SetAllConsoleTimeShow(bool showTime);
#define 设置时间显示_全部 SetAllConsoleTimeShow
			static void SetAllConsoleShow(bool showConsole, bool isTips = false);
#define 设置控制台显示_全部 SetAllConsoleShow
			/*
			* level:
			* er: Error log level output
			* !=3: All log level output
			*/
			template<class T = bool>
			static void SetLogFileWrite(bool logFileWrite, int logLevel = lgm::er)
			{
				LogFileWrite = logFileWrite;

				//输出所有级别
				if (logLevel != lgm::er) {
					LogAllOutput = true;
#ifdef _WINDOWS
					ConsoleOutput(T("日志: 输出 所有 级别"), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
					ConsoleOutput(T("日志: 输出 所有 级别"));
#endif
				}
				else {
					LogAllOutput = false;
#ifdef _WINDOWS
					ConsoleOutput_Error(T("日志: 输出 错误 级别"), FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
					ConsoleOutput_Error(T("日志: 输出 错误 级别"));
#endif
				}
			}
#define 设置日志写入 SetLogFileWrite
	};


	//模式 mode: (_CONSOLE | _WINDOWS) && #ifdef _DEBUG
	static Log lg(false, false);
	//模式 mode: (_CONSOLE) && #ifdef _DEBUG
	static Log lgc(true, false);
	//模式 mode: (_CONSOLE | _WINDOWS) && #ifndef _DEBUG
	static Log lgr(false, true);
	//模式 mode: (_CONSOLE) && #ifndef _DEBUG
	static Log lgcr(true, true);


	template<class Temp = bool>
	void README()
	{
		lgcr(T("_WINDOWS"), lgm::nl);
		lgcr(lgm::nl);
		lgcr(T("Tools_Tool::README()"), lgm::nl);
		lgcr(T(" 支持 Unicode: #define UNICODE"), lgm::nl);
		lgcr(T(" 使用方法-函数对象, 代码如下:"), lgm::nl);
		lgcr(T(" 输出[_CONSOLE | _WINDOWS](_DEBUG)"), T("lg()"));
		lgcr(T(" 输出[_CONSOLE | _WINDOWS](_DEBUG | Release)"), T("lgr()"));
		lgcr(T(" CMD输出[_WINDOWS](_DEBUG)"), T("lgc()"));
		lgcr(T(" CMD输出[_WINDOWS](_DEBUG | Release)"), T("lgcr()"));
		lgcr(lgm::nl);
		lgcr(T("lgm"), T("enum"));
		lgcr(T("\t#ifndef _English"), lgm::nl);
		lgcr(T("\tlgm::nl,  //空标题(不带标题): 只输出内容"), lgm::nl);
		lgcr(T("\tlgm::ts,  //提示"), lgm::nl);
		lgcr(T("\tlgm::wr,  //警告"), lgm::nl);
		lgcr(T("\tlgm::er   //错误"), lgm::nl);
		lgcr(lgm::nl);
		lgcr(T("日志显示开关"), lgm::nl);
		lgcr(T("\tTools_Tool::设置日志显示(bool)"), lgm::nl);
		lgcr(T("\tTools_Tool::设置时间显示(bool)"), lgm::nl);
		lgcr(T("\tTools_Tool::Log::设置日志显示(bool)"), lgm::nl);
		lgcr(T("\tTools_Tool::Log::设置时间显示(bool)"), lgm::nl);
		lgcr(lgm::nl);
		lgcr(lgm::nl);
		lgcr(lgm::nl);
		lgcr(lgm::nl);
		lgcr(T("Tools_Tool::README()"), lgm::nl);
		lgcr(T(" Unicode support: #define UNICODE"), lgm::nl);
		lgcr(T(" Using method - function object, the code is as follows:"), lgm::nl);
		lgcr(T(" Print [_CONSOLE | _WINDOWS](_DEBUG)"), T("lg()"));
		lgcr(T(" Print [_CONSOLE | _WINDOWS](_DEBUG | Release)"), T("lgr()"));
		lgcr(T(" Command Print [_WINDOWS](_DEBUG)"), T("lgc()"));
		lgcr(T(" Command Print [_WINDOWS](_DEBUG | Release)"), T("lgcr()"));
		lgcr(lgm::nl);
		lgcr(lgm::nl);
		lgcr(T("lgm"), T("enum"));
		lgcr(T("\t#ifdef _English"), lgm::nl);
		lgcr(T("\tlgm::nl,  //Empty title (without title): Only the content is output"), lgm::nl);
		lgcr(T("\tlgm::ts,  //Tips"), lgm::nl);
		lgcr(T("\tlgm::wr,  //Warning"), lgm::nl);
		lgcr(T("\tlgm::er   //Error"), lgm::nl);
		lgcr(lgm::nl);
		lgcr(T("Log display switch:"), lgm::nl);
		lgcr(T("\tTools_Tool::SetShowLog(bool)"), lgm::nl);
		lgcr(T("\tTools_Tool::SetConsoleTimeShow(bool)"), lgm::nl);
		lgcr(T("\tTools_Tool::Log::SetShowLog(bool)"), lgm::nl);
		lgcr(T("\tTools_Tool::Log::SetConsoleTimeShow(bool)"), lgm::nl);
	}

}


#endif