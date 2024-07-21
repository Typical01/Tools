#pragma once

#ifndef _TOOLS_Tool_H
#define _TOOLS_Tool_H

#ifdef TOOLS_TOOL_EXPORTS
	#define TOOLS_TOOL_API __declspec(dllexport) //导出
#else
	#define TOOLS_TOOL_API __declspec(dllimport)
#endif


#endif