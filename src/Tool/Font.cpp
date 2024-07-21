#include "pch.h"
#include "Font.h"


HFONT Tools_Tool::WindowsSystem::WindowFont::Font = NULL;

void Tools_Tool::WindowsSystem::WindowFont::SetWindowFont(HWND& hwnd)
{
	if (IsWindow(hwnd)) {
		SendMessage(hwnd, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));  //设置控件字体
		this->窗口句柄.push_back(hwnd);
	}
}

void Tools_Tool::WindowsSystem::WindowFont::SetFont(HFONT& hfont)
{
	this->Font = hfont;

	if (!this->窗口句柄.empty()) {
		for (auto tempBegin = this->窗口句柄.begin(); tempBegin != this->窗口句柄.end(); tempBegin++)
		{
			//句柄有效时
			if (IsWindow(*tempBegin)) {
				SetWindowFont(*tempBegin);
			}
		}
	}
}
