#pragma once

#ifndef _LOCATION_H
#define _LOCATION_H

#include "Tools_Tool.h"
#include <Windows.h>

namespace Tools_Tool {
	namespace WindowsSystem {

		class TOOLS_TOOL_API WindowLocationInfo {
#define 窗口位置信息 WindowLocationInfo
		private:
			RECT* Rect;
		public:

			//主窗口
			int Width;
			int Height;
			int LocationX;
			int LocationY;
#define 窗口宽 wWidth;
#define 窗口高 wHeight;
#define 窗口X LocationX;
#define 窗口Y LocationY;

			//客户区
			int ClientWidth;
			int ClientHeight;
			int ClientLocationX;
			int ClientLocationY;
#define 客户区宽 ClientWidth;
#define 客户区高 ClientHeight;
#define 客户区X ClientLocationX;
#define 客户区Y ClientLocationY;

		public:
			WindowLocationInfo(HWND& hwnd) {
				//获取窗口大小
				GetWindowRect(hwnd, Rect);
				ClientWidth = Rect->right;
				ClientHeight = Rect->bottom;
				ClientLocationX = Rect->left;
				ClientLocationY = Rect->top;

				//获取窗口客户区范围
				GetClientRect(hwnd, Rect);
				Width = Rect->right - Rect->left;
				Height = Rect->bottom - Rect->top;
				LocationX = Rect->left;
				LocationY = Rect->top;
			}
//
//			int GetWindowWidth();
//			int GetWindowHeight();
//			int GetWindowLocationX();
//			int GetWindowLocationY();
//#define Get窗口宽度 GetWindowWidth;
//#define Get窗口高度 GetWindowHeight;
//#define Get窗口位置X GetWindowLocationX;
//#define Get窗口位置Y GetWindowLocationY;
//
//			int GetClientWidth();
//			int GetClientHeight();
//			int GetClientLocationX();
//			int GetClientLocationY();
//#define Get客户区宽度 GetClientClientWidth;
//#define Get客户区高度 GetClientClientHeight;
//#define Get客户区位置X GetClientClientLocationX;
//#define Get客户区位置Y GetClientClientLocationY;
		};
	}
}

#endif