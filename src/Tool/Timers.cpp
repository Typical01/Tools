#include "pch.h" //DLL标准
#include <utility> //DLL标准
#include <limits.h> //DLL标准

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
void Tools_Tool::Timers::AddTimer(std::chrono::system_clock::time_point& time)
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
void Tools_Tool::Timers::AddTimer_s(std::chrono::steady_clock::time_point& time)
{
	this->TimerContainer_s.push_back(time);
}
void Tools_Tool::Timers::AddTimer_s(std::chrono::steady_clock::time_point&& time)
{
	this->TimerContainer_s.push_back(time);
}

void Tools_Tool::Timers::SetTimer(std::chrono::system_clock::time_point& time, int Location)
{
	this->TimerContainer[Location] = time;
}
void Tools_Tool::Timers::SetTimer(std::chrono::system_clock::time_point&& time, int Location)
{
	this->TimerContainer[Location] = time;
}
void Tools_Tool::Timers::SetTimer_s(std::chrono::steady_clock::time_point& time, int Location)
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

long long Tools_Tool::Timers::ComputationFirstToEndInterval()
{
	// first - end
	if (!this->isHighPrecision) {
		return std::chrono::duration_cast<std::chrono::milliseconds>(
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
	if (!this->isHighPrecision) {
		return std::chrono::duration_cast<std::chrono::milliseconds>(
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
		lgc(TEXT("时间戳位置小于 0"), lgm::er);

		return 0;
	}
	else if (LocationEnd > this->TimerContainer.size() - 1) {
		lgc(TEXT("时间戳位置大于 Vector.size() - 1"), lgm::er);

		return 0;
	}

	// front - end
	if (!this->isHighPrecision) {
		return std::chrono::duration_cast<std::chrono::milliseconds>(
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
	std::this_thread::sleep_for(std::chrono::seconds(sec));
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
