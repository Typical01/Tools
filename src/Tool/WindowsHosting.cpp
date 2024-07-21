#include "pch.h"
#include "WindowsHosting.h"



HWND Tools_Tool::WindowsSystem::WindowHosting::Get主窗口()
{
    return this->主窗口;
}

std::vector<HWND> Tools_Tool::WindowsSystem::WindowHosting::Get子窗口()
{
    return this->子窗口Vec;
}
