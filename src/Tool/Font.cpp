#include "pch.h"
#include "Font.h"


HFONT Tools_Tool::WindowsSystem::WindowFont::Font = NULL;

void Tools_Tool::WindowsSystem::WindowFont::SetFont(HFONT hFont)
{
	this->Font = hFont;
}

void Tools_Tool::WindowsSystem::WindowFont::SetWindowFont(HWND hwnd)
{
	if (IsWindow(hwnd)) {
		SendMessage(hwnd, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));  //设置控件字体
	}
}
