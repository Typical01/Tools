#include "pch.h"
#include "Shell.h"


Tools_Tool::WindowsSystem::Shell消息 Tools_Tool::WindowsSystem::WindowShell::执行(Ustr 操作名, Ustr Shell操作, Ustr Shell文件, Ustr Shell参数, int 窗口显示)
{
	//Shell消息 temp(L"nvidia-smi", (int)ShellExecute(NULL, L"runas", L"cmd", L"nvidia-smi -lgc 1080", NULL, SW_SHOWNORMAL));
	Shell消息 temp(操作名, (int)ShellExecute(NULL, Shell操作.c_str(), Shell文件.c_str(), Shell参数.c_str(), NULL, 窗口显示));
	return temp;
}
