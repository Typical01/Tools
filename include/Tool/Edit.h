#pragma once

#ifndef _EDIT_H
#define _EDIT_H

#include "Tools_Tool.h"
#include "Log.h"
#include "WindowHosting.h"

#include <Windows.h>
#include <commctrl.h>
#include <map>
#include <string>

namespace Tools_Tool {
	namespace WindowsSystem {

		class TOOLS_TOOL_API WindowEdit {
		private:
			WindowHosting WH;

			HWND hWnd_标签;
			HWND 最新的编辑窗口;
			HWND hWnd_标签列控件;
			std::map<int, HWND> hWnd_标签编辑窗口;

		public:
			WindowEdit() { }

		public:

			void SetWindowsSize();
			void CreateEditWindows(int tempInt);
		};
		using 窗口控件 = WindowEdit;
	}
}

#endif