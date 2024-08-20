#pragma once

#ifndef _SHELL_H
#define _SHELL_H

#include "Tools_Tool.h"
#include "Message.h"
#include <Windows.h>

namespace Tools_Tool {
	namespace WindowsSystem {

		class TOOLS_TOOL_API WindowShell {
		private:

		public:
			WindowShell() {
				
			}

			// 窗口显示: 0(SW_SHOW) 隐藏窗口
			/* 
			* Shell操作(Ustr): runas / cmd / open / explore
			* Shell参数(Ustr): /k
			*/
			static Shell消息 执行(Ustr 操作名, Ustr Shell操作, Ustr Shell文件, Ustr Shell参数, int 窗口显示 = 5);
		};
	}
}

#endif