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
		using Ss = 配置文件字符串格式;

		// 配置文件的字符处理
		// 先运行 README() 看使用简介
		class TOOLS_TOOL_API CharHandleOfConfigFile
		{
		private:
			Ustr 配置文件_路径;

			//区域总容器
			std::map<Ustr, std::map<Ustr, Ustr>> 区域总容器; //名称在前, 内容在后

			//区域设置_总数
			int 区域设置_总数;
			
			std::set<int> 空行位置统计;

		public:
			// 自述: 打印使用简介的文本
			void README() const;

			CharHandleOfConfigFile()
				: 区域设置_总数(0)
			{
			}

		public:
			// 初始化
			bool Init(Ustr& 传入配置文件路径, bool 解析 = true);
			bool Init(Ustr&& 传入配置文件路径, bool 解析 = true);
			bool Init_Str(Ustr 传入配置文件路径, Ustr& 文件内容);

			// 获取 指定区域中所有的内容
			std::map<Ustr, Ustr> Get指定区域内容(Ustr 传入区域设置) const;
			// 获取 全部配置
			std::map<Ustr, std::map<Ustr, Ustr>> Get配置文件全内容() const;

			/* 写入区域设置项 vector
			* 区域设置传入格式(不用检查): 默认  ;"[]"
			* 区域内容传入格式(内容是带格式的字符串, 需要检查): str=char*  ;"="
			*/
			bool 添加区域(Ustr 传入区域设置, std::vector<Ustr>& 传入区域内容);

			// 删除区域设置中的一条内容: 删除至少一条内容时为 true, 否则为没有对应的内容可以删除
			bool 删除区域(Ustr 传入区域设置, Ustr 传入区域内容);
			// 删除一条区域设置: 删除至少一条内容时为 true, 否则为没有对应的内容可以删除; 需要传入 bool 以免误删
			bool 删除区域(Ustr 传入区域设置,  bool 确定删除);

			// 修改区域设置项
			void 修改区域(Ustr 传入区域设置, Ustr 传入区域内容键, Ustr 传入区域内容值);

			//#include <stdio.h>
			//Unicode 字符集: utf8 编码
			//fopen_s() 使用 UTF-8模式创建 UTF-8 BOM编码的文件, 使用 fputs() fgets() 窄字符
			// Ufopen_s() 使用 UTF-8模式创建 UTF-8 BOM编码的文件, 使用 Ufputs() Ufgets() 宽字符
			bool 写入文件(Ustr fopenMode = T("w+"));
			bool 写入文件(Ustr 传入配置文件路径, Ustr fopenMode = T("w+"));
			bool 写入文件(std::vector<Ustr>& 传入文本内容, Ustr fopenMode = T("w+"));
			bool 写入文件_Str(Ustr& 传入文本内容, Ustr fopenMode = T("w+"));
			static bool 写入文件(Ustr 传入配置文件路径, std::vector<Ustr>& 传入文本内容, Ustr fopenMode = T("w+"));

			bool 创建文件();
			bool 创建文件(Ustr 新文件路径);

		private:
			bool 格式化(std::vector<Ustr>& 传入格式化容器);
			bool 解析(std::vector<Ustr>& 传入区域内容);
			配置文件字符串格式 格式识别(Ustr& 传入解析内容);

		private:
			// 区域设置 中括号字符串分离
			bool 区域设置解析(Ustr& 传入区域设置);
			// 区域内容 等号字符串分离
			bool 区域内容解析(Ustr& 传入区域内容, Ustr& 传出分离后区域内容_First, Ustr& 传出分离后区域内容_Second);

		public:

			/*r 以只读方式打开文件，该文件必须存在。
			r+ 以可读写方式打开文件，该文件必须存在。
			rb+ 读写打开一个二进制文件，只允许读写数据。
			w 打开只写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
			w+ 打开可读写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
			a 以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）
			a+ 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。 （原来的EOF符不保留）
			wb 只写打开或新建一个二进制文件；只允许写数据。
			wb+ 读写打开或建立一个二进制文件，允许读和写。
			ab+ 读写打开一个二进制文件，允许读或在文件末追加数据。
			rt+ 读写打开一个文本文件，允许读和写。
			wt+ 读写打开或着建立一个文本文件；允许读写。
			at+ 读写打开一个文本文件，允许读或在文本末追加数据。*/
			static bool 文件操作(Ustr 文件路径, Ustr 文件打开方式 = T("r"), Ustr 日志 = T("打开"));
			static bool 文件操作(FILE** 文件流, Ustr 文件路径, Ustr 文件打开方式 = T("r"), Ustr 日志 = T("打开"));
		};
		using CfgFile = CharHandleOfConfigFile;
	}
}
namespace 工具库 = Tools_Tool;

#endif