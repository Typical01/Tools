#include "pch.h" 

#include "Timers.h"

#include <iostream>
#include <Windows.h>



std::chrono::system_clock::time_point Tools_Tool::Timers::GetTime()
{
	return std::chrono::system_clock::now(); // 获取当前时间
}
std::chrono::steady_clock::time_point Tools_Tool::Timers::GetTime_s()
{
	return std::chrono::high_resolution_clock::now(); // 获取当前时间
}

void Tools_Tool::Timers::AddTimer()
{
	this->TimerContainer.push_back(GetTime());
}
void Tools_Tool::Timers::AddTimer(const std::chrono::system_clock::time_point& time)
{
	this->TimerContainer.push_back(time);
}
void Tools_Tool::Timers::AddTimer(std::chrono::system_clock::time_point&& time)
{
	this->TimerContainer.push_back(time);
}
void Tools_Tool::Timers::AddTimer_s()
{
	this->TimerContainer_s.push_back(GetTime_s());
}
void Tools_Tool::Timers::AddTimer_s(const std::chrono::steady_clock::time_point& time)
{
	this->TimerContainer_s.push_back(time);
}
void Tools_Tool::Timers::AddTimer_s(std::chrono::steady_clock::time_point&& time)
{
	this->TimerContainer_s.push_back(time);
}

void Tools_Tool::Timers::SetTimer(const std::chrono::system_clock::time_point& time, int Location)
{
	this->TimerContainer[Location] = time;
}
void Tools_Tool::Timers::SetTimer(std::chrono::system_clock::time_point&& time, int Location)
{
	this->TimerContainer[Location] = time;
}
void Tools_Tool::Timers::SetTimer_s(const std::chrono::steady_clock::time_point& time, int Location)
{
	this->TimerContainer_s[Location] = time;
}
void Tools_Tool::Timers::SetTimer_s(std::chrono::steady_clock::time_point&& time, int Location)
{
	this->TimerContainer_s[Location] = time;
}

std::chrono::system_clock::time_point Tools_Tool::Timers::GetTimer(int Location)
{
	return this->TimerContainer[Location];
}
std::chrono::steady_clock::time_point Tools_Tool::Timers::GetTimer_s(int Location)
{
	return this->TimerContainer_s[Location];
}

std::wstring Tools_Tool::Timers::StringToWstring(const std::string& str)
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

std::string Tools_Tool::Timers::WstringToString(const std::wstring& wStr)
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

long long Tools_Tool::Timers::ComputationFirstToEndInterval()
{
	// first - end
	if (!this->HighPrecision) {
		return std::chrono::duration_cast<std::chrono::seconds>(
			this->TimerContainer[this->TimerContainer.size() - 1] - this->TimerContainer[0]).count();
	}
	else {
		switch (this->timeMeasure)
		{
		case tms::ms:
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(
				this->TimerContainer_s[this->TimerContainer_s.size() - 1] - this->TimerContainer_s[0]).count();
		}
		case tms::us:
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(
				this->TimerContainer_s[this->TimerContainer_s.size() - 1] - this->TimerContainer_s[0]).count();
		}
		case tms::ns:
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(
				this->TimerContainer_s[this->TimerContainer_s.size() - 1] - this->TimerContainer_s[0]).count();
		}
		break;
		}
	}
}

long long Tools_Tool::Timers::ComputationFrontToBackInterval()
{
	// front - end
	if (!this->HighPrecision) {
		return std::chrono::duration_cast<std::chrono::seconds>(
			this->TimerContainer[this->TimerContainer.size() - 1] - this->TimerContainer[this->TimerContainer.size() - 2]).count();
	}
	else {
		switch (this->timeMeasure)
		{
		case tms::ms:
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(
				this->TimerContainer_s[this->TimerContainer_s.size() - 1] - this->TimerContainer_s[this->TimerContainer_s.size() - 2]).count();
		}
		case tms::us:
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(
				this->TimerContainer_s[this->TimerContainer_s.size() - 1] - this->TimerContainer_s[this->TimerContainer_s.size() - 2]).count();
		}
		case tms::ns:
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(
				this->TimerContainer_s[this->TimerContainer_s.size() - 1] - this->TimerContainer_s[this->TimerContainer_s.size() - 2]).count();
		}
		break;
		}
	}
}

long long Tools_Tool::Timers::ComputationInterval(int LocationBegin, int LocationEnd)
{
	if (LocationBegin < 0) {
		throw std::runtime_error("Tools_Tool::Timers::ComputationInterval(): LocationBegin < 0");

		return 0;
	}
	else if (LocationEnd > this->TimerContainer.size() - 1) {
		throw std::runtime_error("Tools_Tool::Timers::ComputationInterval(): LocationEnd > TimerContainer.size() - 1");

		return 0;
	}

	// front - end
	if (!this->HighPrecision) {
		return std::chrono::duration_cast<std::chrono::seconds>(
			this->TimerContainer[LocationEnd] - this->TimerContainer[LocationBegin]).count();
	}
	else {
		switch (this->timeMeasure)
		{
		case tms::ms:
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(
				this->TimerContainer_s[LocationEnd] - this->TimerContainer_s[LocationBegin]).count();
		}
		case tms::us:
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(
				this->TimerContainer_s[LocationEnd] - this->TimerContainer_s[LocationBegin]).count();
		}
		case tms::ns:
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(
				this->TimerContainer_s[LocationEnd] - this->TimerContainer_s[LocationBegin]).count();
		}
		break;
		}
	}
}

long long Tools_Tool::Timers::TransformTime(long long& time, TimeMeasure tms, TimeMeasure transformLaterTms)
{
	switch (tms)
	{
	case TimeMeasure::sec:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			//time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::seconds(time)).count();
			break;
		}
		case TimeMeasure::ms:
		{
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(time)).count();
			break;
		}
		case TimeMeasure::us:
		{
			time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds(time)).count();
			break;
		}
		case TimeMeasure::ns:
		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(time)).count();
			break;
		}
		}

		break;
	}
	case TimeMeasure::ms:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(time)).count();
			break;
		}
		case TimeMeasure::ms:
		{
			//time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(time)).count();
			break;
		}
		case TimeMeasure::us:
		{
			time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds(time)).count();
			break;
		}
		case TimeMeasure::ns:
		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(time)).count();
			break;
		}
		}

		break;
	}
	case TimeMeasure::us:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::microseconds(time)).count();
			break;
		}
		case TimeMeasure::ms:
		{
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::microseconds(time)).count();
			break;
		}
		case TimeMeasure::us:
		{
			//time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::microseconds(time)).count();
			break;
		}
		case TimeMeasure::ns:
		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::microseconds(time)).count();
			break;
		}
		}

		break;
	}
	case TimeMeasure::ns:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::nanoseconds(time)).count();
			break;
		}
		case TimeMeasure::ms:
		{
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds(time)).count();
			break;
		}
		case TimeMeasure::us:
		{
			time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::nanoseconds(time)).count();
			break;
		}
		case TimeMeasure::ns:
		{
			//time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::nanoseconds(time)).count();
			break;
		}
		}

		break;
	}
	}

	return time;
}
long long Tools_Tool::Timers::TransformTime(long long&& time, TimeMeasure tms, TimeMeasure transformLaterTms)
{
	switch (tms)
	{
	case TimeMeasure::sec:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			//time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::seconds(time)).count();
			break;
		}
		case TimeMeasure::ms:
		{
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(time)).count();
			break;
		}
		case TimeMeasure::us:
		{
			time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds(time)).count();
			break;
		}
		case TimeMeasure::ns:
		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(time)).count();
			break;
		}
		}

		break;
	}
	case TimeMeasure::ms:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(time)).count();
			break;
		}
		case TimeMeasure::ms:
		{
			//time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(time)).count();
			break;
		}
		case TimeMeasure::us:
		{
			time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds(time)).count();
			break;
		}
		case TimeMeasure::ns:
		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(time)).count();
			break;
		}
		}

		break;
	}
	case TimeMeasure::us:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::microseconds(time)).count();
			break;
		}
		case TimeMeasure::ms:
		{
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::microseconds(time)).count();
			break;
		}
		case TimeMeasure::us:
		{
			//time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::microseconds(time)).count();
			break;
		}
		case TimeMeasure::ns:
		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::microseconds(time)).count();
			break;
		}
		}

		break;
	}
	case TimeMeasure::ns:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::nanoseconds(time)).count();
			break;
		}
		case TimeMeasure::ms:
		{
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds(time)).count();
			break;
		}
		case TimeMeasure::us:
		{
			time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::nanoseconds(time)).count();
			break;
		}
		case TimeMeasure::ns:
		{
			//time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::nanoseconds(time)).count();
			break;
		}
		}

		break;
	}
	}

	return time;
}
bool Tools_Tool::Timers::TransformTimes(long long& time, TimeMeasure tms, TimeMeasure transformLaterTms)
{
	bool isSucceed = false;

	switch (tms)
	{
	case TimeMeasure::sec:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			//time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::seconds(time)).count();
			isSucceed = false;
			break;
		}
		case TimeMeasure::ms:
		{
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::us:
		{
			time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ns:
		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(time)).count();
			isSucceed = true;
			break;
		}
		}

		break;
	}
	case TimeMeasure::ms:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ms:
		{
			//time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(time)).count();
			isSucceed = false;
			break;
		}
		case TimeMeasure::us:
		{
			time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ns:
		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(time)).count();
			isSucceed = true;
			break;
		}
		}

		break;
	}
	case TimeMeasure::us:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::microseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ms:
		{
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::microseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::us:
		{
			//time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::microseconds(time)).count();
			isSucceed = false;
			break;
		}
		case TimeMeasure::ns:
		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::microseconds(time)).count();
			isSucceed = true;
			break;
		}
		}

		break;
	}
	case TimeMeasure::ns:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::nanoseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ms:
		{
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::us:
		{
			time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::nanoseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ns:
		{
			//time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::nanoseconds(time)).count();
			isSucceed = false;
			break;
		}
		}

		break;
	}
	}

	return isSucceed;
}
long long Tools_Tool::Timers::TransformTimes(long long& time, bool& isSucceed, TimeMeasure tms, TimeMeasure transformLaterTms)
{
	switch (tms)
	{
	case TimeMeasure::sec:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			//time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::seconds(time)).count();
			isSucceed = false;
			break;
		}
		case TimeMeasure::ms:
		{
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::us:
		{
			time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ns:
		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(time)).count();
			isSucceed = true;
			break;
		}
		}

		break;
	}
	case TimeMeasure::ms:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ms:
		{
			//time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(time)).count();
			isSucceed = false;
			break;
		}
		case TimeMeasure::us:
		{
			time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ns:
		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(time)).count();
			isSucceed = true;
			break;
		}
		}

		break;
	}
	case TimeMeasure::us:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::microseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ms:
		{
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::microseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::us:
		{
			//time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::microseconds(time)).count();
			isSucceed = false;
			break;
		}
		case TimeMeasure::ns:
		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::microseconds(time)).count();
			isSucceed = true;
			break;
		}
		}

		break;
	}
	case TimeMeasure::ns:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::nanoseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ms:
		{
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::us:
		{
			time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::nanoseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ns:
		{
			//time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::nanoseconds(time)).count();
			isSucceed = false;
			break;
		}
		}

		break;
	}
	}

	return time;
}
long long Tools_Tool::Timers::TransformTimes(long long&& time, bool& isSucceed, TimeMeasure tms, TimeMeasure transformLaterTms)
{
	switch (tms)
	{
	case TimeMeasure::sec:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			//time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::seconds(time)).count();
			isSucceed = false;
			break;
		}
		case TimeMeasure::ms:
		{
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::us:
		{
			time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ns:
		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(time)).count();
			isSucceed = true;
			break;
		}
		}

		break;
	}
	case TimeMeasure::ms:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ms:
		{
			//time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(time)).count();
			isSucceed = false;
			break;
		}
		case TimeMeasure::us:
		{
			time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ns:
		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(time)).count();
			isSucceed = true;
			break;
		}
		}

		break;
	}
	case TimeMeasure::us:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::microseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ms:
		{
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::microseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::us:
		{
			//time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::microseconds(time)).count();
			isSucceed = false;
			break;
		}
		case TimeMeasure::ns:
		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::microseconds(time)).count();
			isSucceed = true;
			break;
		}
		}

		break;
	}
	case TimeMeasure::ns:
	{
		switch (transformLaterTms)
		{
		case TimeMeasure::sec:
		{
			time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::nanoseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ms:
		{
			time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::us:
		{
			time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::nanoseconds(time)).count();
			isSucceed = true;
			break;
		}
		case TimeMeasure::ns:
		{
			//time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::nanoseconds(time)).count();
			isSucceed = false;
			break;
		}
		}

		break;
	}
	}

	return time;
}

void Tools_Tool::Timers::sleep_s(long long ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Tools_Tool::Timers::sleep(long long sec)
{
	for (long long temp = 0; temp < sec; temp++) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		lgc(T("暂停: ") + Uto_string(temp) + T("秒"), lgm::wr);
	}
}

std::vector<std::chrono::system_clock::time_point> Tools_Tool::Timers::GetTimerContainer()
{
	return this->TimerContainer;
}
std::vector<std::chrono::steady_clock::time_point> Tools_Tool::Timers::GetTimerContainer_s()
{
	return this->TimerContainer_s;
}

int Tools_Tool::Timers::GetTimerSize()
{
	return this->TimerContainer.size();
}

int Tools_Tool::Timers::GetTimerSize_s()
{
	return this->TimerContainer_s.size();
}
