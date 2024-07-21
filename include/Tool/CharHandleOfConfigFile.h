#pragma once

//配置文件的字符处理
#ifndef _CHARHANDLEOFCONFIGFILE_H
#define _CHARHANDLEOFCONFIGFILE_H

#include "Tools_Tool.h"
#include "CommonTools.h"
#include "Log.h"

#include <map>
#include <set>
#include <string>
#include <vector>


namespace Tools_Tool {
	namespace StringHandling
	{
		enum TOOLS_TOOL_API 配置文件字符串格式
		{
			区域设置, 
			区域内容, 
			空行, //以换行符 或 空格开头
			错误 //字符串传入为空
		};

		// 配置文件的字符处理
		// 先运行 README() 看使用简介
		class TOOLS_TOOL_API CharHandleOfConfigFile
		{
		private:
			std::wstring 配置文件_路径;

			//区域总容器
			std::map<std::wstring, std::map<std::wstring, std::wstring>> 区域总容器; //名称在前, 内容在后

			//区域设置_总数
			int 区域设置_总数;
			
			std::set<int> 空行位置统计;

			bool 解析文本;

		public:
			// 自述: 打印使用简介的文本
			void README() const;

			CharHandleOfConfigFile()
				: 区域设置_总数(0)
			{
			}
			CharHandleOfConfigFile(const wchar_t* 传入配置文件路径, bool 解析 = true)
				: 区域设置_总数(0)
			{
				Init(传入配置文件路径, 解析);
			}


		public:
			// 初始化
			bool Init(const wchar_t* 传入配置文件路径, bool 解析 = true);

			// 获取 指定区域中所有的内容
			std::map<std::wstring, std::wstring> Get指定区域内容(const wchar_t* 传入区域设置) const;
			// 获取 全部配置
			std::map<std::wstring, std::map<std::wstring, std::wstring>> Get配置文件全内容() const;

			/* 写入区域设置项 vector
			* 区域设置传入格式(不用检查): 默认  ;"[]"
			* 区域内容传入格式(内容是带格式的字符串, 需要检查): str=char*  ;"="
			*/
			bool 添加区域(const wchar_t* 传入区域设置, std::vector<std::wstring>& 传入区域内容);

			// 删除区域设置中的一条内容: 删除至少一条内容时为 true, 否则为没有对应的内容可以删除
			bool 删除区域(const wchar_t* 传入区域设置, const wchar_t* 传入区域内容);
			// 删除一条区域设置: 删除至少一条内容时为 true, 否则为没有对应的内容可以删除; 需要传入 bool 以免误删
			bool 删除区域(const wchar_t* 传入区域设置,  bool 确定删除);

			// 修改区域设置项
			void 修改区域(const wchar_t* 传入区域设置, const wchar_t* 传入区域内容键, const wchar_t* 传入区域内容值);

			// 将格式化后的配置文件内容写入 指定的文件
			bool 写入文件();
			// 将格式化后的配置文件内容写入 指定的文件
			bool 写入文件(const wchar_t* 传入配置文件路径);

			//只用于创建 UTF-8 BOM文件
			bool 创建文件();
			bool 创建文件(const wchar_t* 新文件路径);

		private:
			/* 字符串格式化:
			* map<string, string>("张三", "18") | string("信息")
			* "张三=18" | "[信息]"
			*/
			bool 格式化(std::vector<std::wstring>& 传入格式化容器);
			/* 解析:
			* 张三=18 | [信息]
			* map<string, string>("张三", "18") | string(信息)
			*/
			bool 解析(std::vector<std::wstring>& 传入区域内容);
			配置文件字符串格式 格式识别(std::wstring& 传入解析内容);

		private:
			// 区域设置 中括号字符串分离
			bool 区域设置解析(std::wstring& 传入区域设置);
			// 区域内容 等号字符串分离
			bool 区域内容解析(std::wstring& 传入区域内容, std::wstring& 传出分离后区域内容_First, std::wstring& 传出分离后区域内容_Second);

		public:
			void test();
		};

	}
}
namespace 工具库 = Tools_Tool;

typedef Tools_Tool::StringHandling::配置文件字符串格式 Ss;

#endif