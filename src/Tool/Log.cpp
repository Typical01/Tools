#include "pch.h"

#include "Log.h"


//控制台初始化
bool Tools_Tool::Log::FirstInitCMD = false;

DWORD Tools_Tool::written = 0;
HWND Tools_Tool::hConsole = nullptr;
HANDLE Tools_Tool::hConsoleOutput = nullptr;
HANDLE Tools_Tool::hConsoleError = nullptr;
FILE* Tools_Tool::LogFileStream = nullptr;
bool Tools_Tool::LogFileWrite = false;
bool Tools_Tool::LogAllOutput = false;


Tools_Tool::Log::~Log()
{
	if (LogFileStream != nullptr) {
		fclose(LogFileStream);
	}
}

void Tools_Tool::Log::SetAllShowLog(bool showLog)
{
	lg.SetShowLog(showLog);
	lgc.SetShowLog(showLog);
	lgr.SetShowLog(showLog);
	lgcr.SetShowLog(showLog);
}

void Tools_Tool::Log::SetAllConsoleTimeShow(bool showTime)
{
	lg.SetConsoleTimeShow(showTime);
	lgc.SetConsoleTimeShow(showTime);
	lgr.SetConsoleTimeShow(showTime);
	lgcr.SetConsoleTimeShow(showTime);
}

void Tools_Tool::Log::SetAllConsoleShow(bool showConsole, bool isTips)
{
	lg.SetConsoleShow(showConsole, isTips);
	lgc.SetConsoleShow(showConsole, isTips);
	lgr.SetConsoleShow(showConsole, isTips);
	lgcr.SetConsoleShow(showConsole, isTips);
}

void Tools_Tool::Log::SetShowLog(bool showLog)
{
	this->ShowLog = showLog;
}

void Tools_Tool::Log::SetConsoleTimeShow(bool showTime)
{
	this->ShowTime = showTime;
}

std::wstring Tools_Tool::Log::StringToWstring(const std::string& str)
{
	std::wstring wContext;

	if (str.empty()) {
		return wContext;
	}

#ifdef _WINDOWS
	// Windows 版本
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), nullptr, 0);
	if (len <= 0) {
		throw std::runtime_error("Failed to convert string to wide string.");
	}
	std::unique_ptr<wchar_t[]> buffer(new wchar_t[len + 1]);
	if (MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), buffer.get(), len) <= 0) {
		throw std::runtime_error("Failed to convert string to wide string.");
	}
	buffer[len] = T('\0');
	wContext.assign(buffer.get());
#else
	// POSIX 版本
	std::unique_ptr<char[]> buffer(new char[str.size() * 4]);  // 大小预留空间
	size_t out_len = str.size() * 4;
	iconv_t cd = iconv_open("UTF-16LE", "ISO-8859-1");
	if (cd == (iconv_t)-1) {
		throw std::runtime_error("Failed to open iconv descriptor.");
	}
	if (iconv(cd, const_cast<const char**>(&str.c_str()), &str.size(), reinterpret_cast<char**>(buffer.get()), &out_len) == (size_t)-1) {
		iconv_close(cd);
		throw std::runtime_error("Failed to convert string using iconv.");
	}
	iconv_close(cd);
	wContext.assign(reinterpret_cast<wchar_t*>(buffer.get()));
#endif

	return wContext;
}

std::string Tools_Tool::Log::WstringToString(const std::wstring& wStr)
{
	std::string context;

	if (wStr.empty()) {
		return context;
	}

#ifdef _WINDOWS
	// Windows 版本
	int len = WideCharToMultiByte(CP_UTF8, 0, wStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (len <= 0) {
		throw std::runtime_error("Failed to convert wide string to string.");
	}
	std::unique_ptr<char[]> buffer(new char[len + 1]);
	if (WideCharToMultiByte(CP_UTF8, 0, wStr.c_str(), -1, buffer.get(), len, nullptr, nullptr) <= 0) {
		throw std::runtime_error("Failed to convert wide string to string.");
	}
	buffer[len] = '\0';
	context.assign(buffer.get());
#else
	// POSIX 版本
	std::unique_ptr<char[]> buffer(new char[wStr.size() * 4]);  // 大小预留空间
	size_t out_len = wStr.size() * 4;
	iconv_t cd = iconv_open("ISO-8859-1", "UTF-16LE");
	if (cd == (iconv_t)-1) {
		throw std::runtime_error("Failed to open iconv descriptor.");
	}
	size_t in_len = wStr.size() * sizeof(wchar_t);
	const char* in = reinterpret_cast<const char*>(wStr.c_str());
	if (iconv(cd, const_cast<const char**>(&in), &in_len, reinterpret_cast<char**>(buffer.get()), &out_len) == (size_t)-1) {
		iconv_close(cd);
		throw std::runtime_error("Failed to convert wide string using iconv.");
	}
	iconv_close(cd);
	context.assign(buffer.get(), buffer.get() + (wStr.size() * 4 - out_len));
#endif

	return context;
}

void Tools_Tool::Log::SetConsoleTextColor(WORD wColor)
{
	if (hConsoleOutput != INVALID_HANDLE_VALUE) {
		SetConsoleTextAttribute(hConsoleOutput, wColor);
	}
}

void Tools_Tool::Log::SetConsoleTextColor_Error(WORD wColor)
{
	if (hConsoleError != INVALID_HANDLE_VALUE) {
		SetConsoleTextAttribute(hConsoleError, wColor);
	}
}
