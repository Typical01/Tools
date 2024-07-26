#include "pch.h"

#include "CharHandleOfConfigFile.h"

#include <iostream>

using namespace Tools_Tool::StringHandling;

void Tools_Tool::StringHandling::CharHandleOfConfigFile::README() const
{
	lgc(L"编码问题:", lgm::ts);
	lgc(L"乱码是因为显示了两个编码的版本, 相当于中文和英文, 不用管", lgm::ts);
	lgc(L"使用之前先统一编码[Unicode - UTF-8 带签名 BOM]: 源码编码, 执行编码, 解析编码", lgm::ts);
	lgc(L"其中, 执行与解析的编码已完成统一: UTF-8", lgm::ts);
	lgc(L"执行编码: wchar_t [] = L\"这是中文\", 传入字符同理", lgm::ts);
	lgc(L"解析编码: system(\"chcp 65001\");", lgm::ts);
	lgc(L"源码编码的 MSVC 流程为: 另存文件为 UTF-8", lgm::ts);
	lgc(L"文件->高级保存项(没有百度)-> Unicode-UTF-8 带签名BOM(不带 Windows可能会识别错误)", lgm::ts);
	lgc(L"使用方式:", lgm::ts);
	lgc(L"1.创建 CharHandleOfConfigFile对象(传入文件路径)", lgm::ts);
	lgc(L"2.使用 std::vector<std::wstring>::push_back() 添加内容", lgm::ts);
	lgc(L"例. 文件显示: 屏幕高=1920  代码: Vector.push_back(L\"屏幕高=1920\")", lgm::ts);
	lgc(L"3.使用 添加区域()", lgm::ts);
	lgc(L"例. 文件显示:[默认配置] 代码: 添加区域(L\"默认配置\", Vector)", lgm::ts);
	lgc(L"4.使用 写入文件()", lgm::ts);
}

bool Tools_Tool::StringHandling::CharHandleOfConfigFile::Init(Ustr& 传入配置文件路径, bool 解析)
{
	//保存路径
	this->配置文件_路径 = 传入配置文件路径;

	std::vector<std::wstring> 传入区域内容;

	//读取文件流
	FILE* 读取文件流指针;
	errno_t err = _wfopen_s(&读取文件流指针, this->配置文件_路径.c_str(), L"r, ccs=UTF-8"); //以读取模式打开文件
	if (err != 0)
	{
		lgc((L"文件: " + this->配置文件_路径 + L" 打开错误(或没有文件)!").c_str(), lgm::wr);
		if (读取文件流指针 == NULL) {
			lgc(L"errno_t: " + Uto_string(err), lgm::er);
		}
		return false; //没有文件, 则不进行读取和解析
	}
	lgc((L"文件: " + this->配置文件_路径 + L" 打开成功").c_str(), lgm::wr);

	if (!解析)
	{
		return true;
	}

	//读取配置文件内容
	wchar_t tempWchar_t[512] = L"";
	int 空行计数 = 0;

	lgc(L"文件: 开始读取...\n", lgm::wr);

	while (!feof(读取文件流指针)) //feof: 文件尾时, 返回非0
	{
		fgetws(tempWchar_t, 512, 读取文件流指针); //读取一行, 最大字符容量 512
		if (tempWchar_t == L"\n") //记录空行
		{
			this->空行位置统计.insert(空行计数);
			空行计数++;
		}
		传入区域内容.push_back(tempWchar_t); //保存内容

		//lg(L"读取字符: " + *tempWchar_t, lg::wr);
	}
	fclose(读取文件流指针);

	if (this->解析(传入区域内容))
	{
		return true;
	}
	else
	{
		lg(L"文件中内容过少, 或不符合格式\n", lgm::wr);
		return false;
	}

	return true;
}

bool Tools_Tool::StringHandling::CharHandleOfConfigFile::Init(Ustr&& 传入配置文件路径, bool 解析)
{
	//保存路径
	this->配置文件_路径 = 传入配置文件路径;

	std::vector<std::wstring> 传入区域内容;

	//读取文件流
	FILE* 读取文件流指针;
	errno_t err = _wfopen_s(&读取文件流指针, this->配置文件_路径.c_str(), L"r, ccs=UTF-8"); //以读取模式打开文件
	if (err != 0)
	{
		lgc((L"文件: " + this->配置文件_路径 + L" 打开错误(或没有文件)!").c_str(), lgm::wr);
		if (读取文件流指针 == NULL) {
			lgc(L"errno_t: " + Uto_string(err), lgm::er);
		}
		return false; //没有文件, 则不进行读取和解析
	}
	lgc((L"文件: " + this->配置文件_路径 + L" 打开成功").c_str(), lgm::wr);

	if (!解析)
	{
		return true;
	}

	//读取配置文件内容
	wchar_t tempWchar_t[512] = L"";
	int 空行计数 = 0;

	lgc(L"文件: 开始读取...\n", lgm::wr);

	while (!feof(读取文件流指针)) //feof: 文件尾时, 返回非0
	{
		fgetws(tempWchar_t, 512, 读取文件流指针); //读取一行, 最大字符容量 512
		if (tempWchar_t == L"\n") //记录空行
		{
			this->空行位置统计.insert(空行计数);
			空行计数++;
		}
		传入区域内容.push_back(tempWchar_t); //保存内容

		//lg(L"读取字符: " + *tempWchar_t, lg::wr);
	}
	fclose(读取文件流指针);

	if (this->解析(传入区域内容))
	{
		return true;
	}
	else
	{
		lg(L"文件中内容过少, 或不符合格式\n", lgm::wr);
		return false;
	}

	return true;
}

std::map<std::wstring, std::wstring> Tools_Tool::StringHandling::CharHandleOfConfigFile::Get指定区域内容(const wchar_t* 传入区域设置) const
{
	return this->区域总容器.find(传入区域设置)->second;
}

std::map<std::wstring, std::map<std::wstring, std::wstring>> Tools_Tool::StringHandling::CharHandleOfConfigFile::Get配置文件全内容() const
{
	return this->区域总容器;
}

bool Tools_Tool::StringHandling::CharHandleOfConfigFile::添加区域(const wchar_t* 传入区域设置, std::vector<std::wstring>& 传入区域内容)
{
	std::vector<std::wstring> temp解析后的区域内容_First(传入区域内容.size(), L"");
	std::vector<std::wstring> temp解析后的区域内容_Second(传入区域内容.size(), L"");
	std::map<std::wstring, std::wstring> 解析后的区域与内容;
	int temp计数 = 0;
	std::wstring 传入区域设置Str = 传入区域设置;

	//std::map<std::wstring, std::wstring> temp解析后的区域内容;
	for (auto tempStr = 传入区域内容.begin(); tempStr != 传入区域内容.end(); tempStr++)
	{
		if (this->区域内容解析(*tempStr, temp解析后的区域内容_First[temp计数], temp解析后的区域内容_Second[temp计数]))
		{
			this->区域内容解析(*tempStr, temp解析后的区域内容_First[temp计数], temp解析后的区域内容_Second[temp计数]);
			temp计数++;
		}
	}

	lgc(L"区域设置: " + (std::wstring)传入区域设置, lgm::wr);

	for (int i = 0; i < temp计数; i++)
	{
		//保存解析后的区域与内容
		解析后的区域与内容.insert(std::make_pair(temp解析后的区域内容_First[i], temp解析后的区域内容_Second[i]));

		lgc(L"first: [" + temp解析后的区域内容_First[i] + L"] |second:[" + temp解析后的区域内容_Second[i] + L"]", lgm::wr);
	}
	//在总容器中添加内容
	std::pair<std::map<std::wstring, std::map<std::wstring, std::wstring>>::iterator, bool> tempPair \
		= this->区域总容器.emplace(std::make_pair(传入区域设置, 解析后的区域与内容));

	if (!tempPair.second)
	{
		//lg((L"添加区域: " + (std::wstring)传入区域设置 + L"失败!").c_str(), lgm::wr);
		return false;
	}

	return true;
}

bool Tools_Tool::StringHandling::CharHandleOfConfigFile::删除区域(const wchar_t* 传入区域设置, const wchar_t* 传入区域内容)
{
	if (this->区域总容器.find(传入区域设置)->second.erase(传入区域内容))
	{
		return true;
	}

	return false;
}

bool Tools_Tool::StringHandling::CharHandleOfConfigFile::删除区域(const wchar_t* 传入区域设置, bool 确定删除)
{
	if (this->区域总容器.erase(传入区域设置))
	{
		return true;
	}

	return false;
}

void Tools_Tool::StringHandling::CharHandleOfConfigFile::修改区域(const wchar_t* 传入区域设置, const wchar_t* 传入区域内容键, const wchar_t* 传入区域内容值)
{
	this->区域总容器.find(传入区域设置)->second.at(传入区域内容键) = 传入区域内容值;
}

bool Tools_Tool::StringHandling::CharHandleOfConfigFile::区域内容解析(std::wstring& 传入区域内容, std::wstring& 传出分离后区域内容_First, std::wstring& 传出分离后区域内容_Second)
{
	//传入 传入区域内容 不为空时
	if (!传入区域内容.empty())
	{
		//格式是否非法
		int temp非法格式_等于号 = 传入区域内容.find(L'=');
		if (temp非法格式_等于号 == std::wstring::npos)
		{
			lgc((L"区域内容: 格式错误" + 传入区域内容).c_str(), lgm::wr);
			return false;
		}

		int temp等于号位置 = temp非法格式_等于号 + 1; //找到 "=" 号的元素位置, string.find 返回下标位置
		int temp值长度 = 传入区域内容.length() - temp等于号位置; //去掉 "="号后剩余的元素个数
		std::wstring temp键(temp等于号位置 - 1, ' '); //初始化
		std::wstring temp值(temp值长度, ' '); //初始化

		//保存 键
		for (int i = 0; i < temp等于号位置; i++)
		{
			temp键[i] = 传入区域内容[i];
		}
		//保存 值: 内容从"="之后(+1)开始, string.find 返回下标位置
		for (int i = temp等于号位置; i < 传入区域内容.length(); ++i) //先加 加到'='之后
		{
			//从0开始保存
			temp值[i - temp等于号位置] = 传入区域内容[i];
		}
		//找到 temp值后的 '\n', 并删除: 避免使用异常
		if (temp值.find(L"\n") != std::wstring::npos)
		{
			temp值.erase(temp值.size() - 1);
			//lgc((L"temp值: [" + temp值 + L"]").c_str());
		}

		传出分离后区域内容_First = temp键;
		传出分离后区域内容_Second = temp值;

		return true;
	}

	lgc(L"区域内容解析: 传入为空!", lgm::wr);

	return false;
}
bool Tools_Tool::StringHandling::CharHandleOfConfigFile::区域设置解析(std::wstring& 传入区域设置)
{
	//vector 不为空时
	if (!传入区域设置.empty())
	{
		//格式是否非法
		int temp非法格式_左括号 = 传入区域设置.find(L'[');
		int temp非法格式_右括号 = 传入区域设置.find(L']');
		if (temp非法格式_左括号 == std::wstring::npos || temp非法格式_右括号 == std::wstring::npos)
		{
			lgc(L"区域设置: 格式错误", lgm::wr);
			return false;
		}

		int 左括号的元素位置 = temp非法格式_左括号; //左括号的元素位置
		int 右括号的元素位置 = temp非法格式_右括号; //右括号的元素位置
		std::wstring 区域设置名称(temp非法格式_右括号 - 1, ' '); //区域设置名称

		//解析 [传入区域设置]
		//保存ConfigNmae
		for (int temp_Int = 0;
			temp_Int < 右括号的元素位置; ++temp_Int) //find返回 0, 但是 0为 '['
		{
			//保存 区域设置名称
			区域设置名称[temp_Int] = 传入区域设置[temp_Int + 1];
		}
		//lgc((L"区域设置名称: [" + 区域设置名称 + L"]").c_str());
		传入区域设置 = 区域设置名称;

		return true;
	}

	lgc(L"区域设置解析: 传入为空!", lgm::wr);
	return false;
}

bool Tools_Tool::StringHandling::CharHandleOfConfigFile::解析(std::vector<std::wstring>& 传入区域内容)
{
	//vec非空
	if (!传入区域内容.size() < 2)
	{
		std::wstring temp解析后内容_First;
		std::wstring temp解析后内容_Second;
		std::map<std::wstring, std::wstring> temp解析后区域与内容;
		std::wstring 区域设置;

		/* 区域划分 */
		for (int i = 0; i < 传入区域内容.size(); i++)
		{
			std::wstring temp解析内容 = 传入区域内容[i];
			Ss tempSs = 格式识别(temp解析内容);

			//非空行 无错误
			if (tempSs != Ss::空行 && tempSs != Ss::错误) //非空格字符和换行符
			{
				//区域设置
				if (tempSs == Ss::区域设置)
				{
					//区域设置计数增加
					this->区域设置_总数++;

					//是否到下一个区域设置
					if (this->区域设置_总数 > 1)
					{
						//保存到 总容器
						this->区域总容器.insert(std::make_pair(区域设置, temp解析后区域与内容));

						//保存后刷新
						temp解析后区域与内容.clear();
					}

					//做 区域设置的解析
					if (this->区域设置解析(传入区域内容[i]))
					{
						区域设置 = 传入区域内容[i];
					}
					else //空行 记录并跳过
					{
						//记录 首字符为空白的行
						this->空行位置统计.insert(i);
					}

					//直接跳到下次
					continue;
				}
				else //区域设置 字符行
				{
					if (this->区域设置_总数 > 0)
					{
						//找到对应区域设置位置, 内容格式非法则 不保存
						if (this->区域内容解析(temp解析内容, temp解析后内容_First, temp解析后内容_Second))
						{
							temp解析后区域与内容.insert(std::make_pair(temp解析后内容_First, temp解析后内容_Second));
						}
						else //非法格式行
						{
							//记录 首字符为空白的行
							this->空行位置统计.insert(i);
							//lg(L"空行: " + i, lgm::wr);
						}

						//直接跳到下次
						continue;
					}
				}
			}
			else if (tempSs == Ss::空行)
			{
				//记录 首字符为空白的行
				this->空行位置统计.insert(i);
			}
			else if (tempSs == Ss::错误)
			{

			}
		}
		//保存到 总容器
		this->区域总容器.insert(std::make_pair(区域设置, temp解析后区域与内容));

		return true;
	}
	else
	{
		lgc(L"解析: 传入为空!", lgm::wr);
	}
	return false;
}

配置文件字符串格式 Tools_Tool::StringHandling::CharHandleOfConfigFile::格式识别(std::wstring& 传入解析内容)
{
	if (!传入解析内容.empty())
	{
		//首字符符号
		wchar_t temp区域符号 = 传入解析内容[0];

		//字符串是否非法
		if (temp区域符号 != L' ' && temp区域符号 != L'\n') //非空格符和换行符
		{
			//首字符 为区域设置名称划分符号 '['
			if (temp区域符号 == L'[')
			{
				return Ss::区域设置;
			}
			return Ss::区域内容;
		}
		else //空字符串
		{
			return Ss::空行;
		}
	}
	else
	{
		lgc(L"格式识别: 传入为空!", lgm::wr);
		return Ss::错误;
	}
}

bool Tools_Tool::StringHandling::CharHandleOfConfigFile::格式化(std::vector<std::wstring>& 传入格式化容器)
{
	if (this->区域总容器.size() > 0)
	{
		for (auto 区域设置 = this->区域总容器.begin(); 区域设置 != this->区域总容器.end(); 区域设置++)
		{
			if (区域设置->first.find(L"\n") == std::wstring::npos)
			{
				传入格式化容器.push_back(L"[" + 区域设置->first + L"]\n"); //先输出区域设置, 带换行符
			}
			else //有换行符
			{
				传入格式化容器.push_back(L"[" + 区域设置->first + L"]"); //先输出区域设置, 带换行符
			}
			//区域内容
			for (auto temp解析后的区域内容 = 区域设置->second.begin(); temp解析后的区域内容 != 区域设置->second.end(); temp解析后的区域内容++)
			{
				if (temp解析后的区域内容->second.find(L"\n") == std::wstring::npos)
				{
					传入格式化容器.push_back(temp解析后的区域内容->first + L"=" + temp解析后的区域内容->second + L"\n"); //先输出区域设置, 带换行符
				}
				else //有换行符
				{
					传入格式化容器.push_back(temp解析后的区域内容->first + L"=" + temp解析后的区域内容->second); //先输出区域设置, 带换行符
				}
			}
		}


		/*if (传入格式化容器.size() > 2)
		{
			for (auto tempStr = 传入格式化容器.begin(); tempStr != 传入格式化容器.end(); tempStr++)
			{
				lg((L"格式化: " + *tempStr).c_str(), lgm::wr);
			}
		}*/

		return true;
	}
	else
	{
		return false;
	}
}

bool Tools_Tool::StringHandling::CharHandleOfConfigFile::写入文件(Ustr fopenMode)
{
	//#include <stdio.h>
	//Unicode 字符集: utf8 编码
	std::vector<std::wstring> tempVec写入内容;
	this->格式化(tempVec写入内容);

	//fopen_s() 使用 UTF-8模式创建 UTF-8 BOM编码的文件, 使用 fputs() fgets() 窄字符
	// _wfopen_s() 使用 UTF-8模式创建 UTF-8 BOM编码的文件, 使用 fputws() fgetws() 宽字符

	//开始写入
	FILE* 写入文件流指针;
	//w+ 打开可读写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
	//主要是清空文件内容
	Ustr FopenMode = fopenMode + L", ccs=UTF-8";
	errno_t err = _wfopen_s(&写入文件流指针, this->配置文件_路径.c_str(), FopenMode.c_str());
	if (err != 0)
	{
		lgc((L"文件: " + this->配置文件_路径 + L" 打开错误!\n").c_str(), lgm::wr);
		if (写入文件流指针 == NULL) {
			lgc(L"errno_t: " + Uto_string(err), lgm::er);
		}
		return false;
	}
	lgc((L"文件: " + this->配置文件_路径 + L" 打开成功\n").c_str(), lgm::wr);

	if (tempVec写入内容.size() > 2)
	{
		for (auto tempBegin = tempVec写入内容.begin(); tempBegin != tempVec写入内容.end(); tempBegin++)
		{
			fputws(tempBegin->c_str(), 写入文件流指针); // fputws 默认不带换行符, 格式化()中已添加
			//lg((L"写入缓冲字符: " + *tempBegin).c_str(), lgm::wr);
		}
	}
	fclose(写入文件流指针); //关闭后, 才将缓冲中的字符写入到文件中

	lgc(L"空行: " + this->空行位置统计.size(), lgm::ts);

	return true;
}

bool Tools_Tool::StringHandling::CharHandleOfConfigFile::写入文件(Ustr 传入配置文件路径, Ustr fopenMode)
{
	//#include <stdio.h>
	//Unicode 字符集: utf8 编码
	std::vector<std::wstring> tempVec写入内容;
	this->格式化(tempVec写入内容);

	//fopen_s() 使用 UTF-8模式创建 UTF-8 BOM编码的文件, 使用 fputs() fgets() 窄字符
	// _wfopen_s() 使用 UTF-8模式创建 UTF-8 BOM编码的文件, 使用 fputws() fgetws() 宽字符

	//开始写入
	FILE* 写入文件流指针;
	//w+ 打开可读写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
	//主要是清空文件内容
	Ustr FopenMode = fopenMode + L", ccs=UTF-8";
	errno_t err = _wfopen_s(&写入文件流指针, 传入配置文件路径.c_str(), FopenMode.c_str());
	if (err != 0)
	{
		lgc((L"文件: " + 传入配置文件路径 + L" 打开错误!\n").c_str(), lgm::wr);
		if (写入文件流指针 == NULL) {
			lgc(L"errno_t: " + Uto_string(err), lgm::er);
		}
		return false;
	}
	lgc((L"文件: " + 传入配置文件路径 + L" 打开成功\n").c_str(), lgm::wr);

	if (tempVec写入内容.size() > 2)
	{
		for (auto tempBegin = tempVec写入内容.begin(); tempBegin != tempVec写入内容.end(); tempBegin++)
		{
			fputws(tempBegin->c_str(), 写入文件流指针); // fputws 默认不带换行符, 格式化()中已添加
			//lg((L"写入缓冲字符: " + *tempBegin).c_str(), lgm::wr);
		}
	}
	fclose(写入文件流指针); //关闭后, 才将缓冲中的字符写入到文件中

	lgc(L"空行: " + this->空行位置统计.size(), lgm::ts);

	return true;
}
bool Tools_Tool::StringHandling::CharHandleOfConfigFile::写入文件(std::vector<Ustr>& 传入文本内容, Ustr fopenMode)
{
	FILE* 写入文件流指针;
	//w+ 打开可读写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
	//主要是清空文件内容
	Ustr FopenMode = fopenMode + L", ccs=UTF-8";
	errno_t err = _wfopen_s(&写入文件流指针, this->配置文件_路径.c_str(), FopenMode.c_str());
	if (err != 0)
	{
		lgc((L"文件: " + (std::wstring)this->配置文件_路径 + L" 打开错误!\n").c_str(), lgm::wr);
		if (写入文件流指针 == NULL) {
			lgc(L"errno_t: " + Uto_string(err), lgm::er);
		}
		return false;
	}
	lgc((L"文件: " + (std::wstring)this->配置文件_路径 + L" 打开成功\n").c_str(), lgm::wr);

	if (传入文本内容.size() > 0)
	{
		for (auto tempBegin = 传入文本内容.begin(); tempBegin != 传入文本内容.end(); tempBegin++)
		{
			fputws(tempBegin->c_str(), 写入文件流指针); // fputws 默认不带换行符, 格式化()中已添加
			lgc((Ustr)L"写入缓冲字符: " + *tempBegin, lgm::wr);
		}
	}
	fclose(写入文件流指针); //关闭后, 才将缓冲中的字符写入到文件中

	return true;
}

bool Tools_Tool::StringHandling::CharHandleOfConfigFile::写入文件(Ustr 传入配置文件路径, std::vector<Ustr>& 传入文本内容, Ustr fopenMode)
{
	FILE* 写入文件流指针;
	//w+ 打开可读写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
	//主要是清空文件内容
	Ustr FopenMode = fopenMode + L", ccs=UTF-8";
	errno_t err = _wfopen_s(&写入文件流指针, 传入配置文件路径.c_str(), FopenMode.c_str());
	if (err != 0)
	{
		lgc((L"文件: " + 传入配置文件路径 + L" 打开错误!\n").c_str(), lgm::wr);
		if (写入文件流指针 == NULL) {
			lgc(L"errno_t: " + Uto_string(err), lgm::er);
		}
		return false;
	}
	lgc((L"文件: " + 传入配置文件路径 + L" 打开成功\n").c_str(), lgm::wr);

	if (传入文本内容.size() > 0)
	{
		for (auto tempBegin = 传入文本内容.begin(); tempBegin != 传入文本内容.end(); tempBegin++)
		{
			fputws(tempBegin->c_str(), 写入文件流指针); // fputws 默认不带换行符, 格式化()中已添加
			lgc((Ustr)L"写入缓冲字符: " + *tempBegin, lgm::wr);
		}
	}
	fclose(写入文件流指针); //关闭后, 才将缓冲中的字符写入到文件中

	return true;
}

bool Tools_Tool::StringHandling::CharHandleOfConfigFile::创建文件()
{
	FILE* 创建文件流指针;
	//w+ 打开可读写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
	//主要是清空文件内容
	errno_t err = _wfopen_s(&创建文件流指针, this->配置文件_路径.c_str(), L"w+, ccs=UTF-8");
	if (err != 0)
	{
		lgc((L"文件: " + this->配置文件_路径 + L" 创建错误!\n").c_str(), lgm::wr);
		if (创建文件流指针 == NULL) {
			lgc(L"errno_t: " + Uto_string(err), lgm::er);
		}
		return false;
	}
	lgc((L"文件: " + this->配置文件_路径 + L" 创建成功\n").c_str(), lgm::wr);
	fclose(创建文件流指针);

	return true;
}

bool Tools_Tool::StringHandling::CharHandleOfConfigFile::创建文件(const wchar_t* 新文件路径)
{
	FILE* 创建文件流指针;
	//w+ 打开可读写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
	//主要是清空文件内容
	errno_t err = _wfopen_s(&创建文件流指针, 新文件路径, L"w+, ccs=UTF-8");
	if (err != 0)
	{
		lgc((L"文件: " + this->配置文件_路径 + L" 创建错误!\n").c_str(), lgm::wr);
		if (创建文件流指针 == NULL) {
			lgc(L"errno_t: " + Uto_string(err), lgm::er);
		}
		return false;
	}
	lgc((L"文件: " + this->配置文件_路径 + L" 创建成功\n").c_str(), lgm::wr);
	fclose(创建文件流指针);

	return true;
}

void Tools_Tool::StringHandling::CharHandleOfConfigFile::test()
{
	//UTF-8 字符集流程: 源码, 执行, 解析
	//Windows 默认根据地区的 ANSI 来确定字符编码
	//中文区: ANSI == GBK(GB2312), 其他同理

	//源码
	//源码字符集: 另存文件为 UTF-8 [MSVC: 高级保存项->Unicode - UTF-8 带签名BOM(不带 Windows可能会识别错误)]

	//执行: 写入的文本一定是 utf-8
	//#pragma execution_wchar_tacter_set("utf-8") // c++11可以使用 L 标识符
	//C++11 支持: wchar_t *utf8[] = L"这是中文";

	//解析
	// 1.
	//将程序的locale设置为中国大陆使用的UTF-8编码
	//setlocale(LC_ALL, "zh_CN.UTF-8");
	//控制台显示乱码纠正
	// 2.
	//SetConsoleOutputCP(CP_UTF8); //和 chcp 65001同理
	//system(L"chcp 65001"); //设置控制台输出的字符集
	// 3.
	/*
	std::wcout << L"修改前: ";
	std::wcout << std::wcout.getloc().name() << std::endl;
	std::locale locale_wcout("chs"); //locale 设置 std::wcout 区域为中文区
	std::wcout.imbue(locale_wcout);  //设置语言环境
	std::wcin.imbue(locale_wcout);  //设置语言环境
	std::wcout << L"修改后: ";
	std::wcout << std::wcout.getloc().name() << std::endl;
	*/

	/*
	r 以只读方式打开文件，该文件必须存在。
　　	r+ 以可读写方式打开文件，该文件必须存在。
　　	rb+ 读写打开一个二进制文件，只允许读写数据。
　　	rt+ 读写打开一个文本文件，允许读和写。

　	w 打开只写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
　	w+ 打开可读写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。

　　	a 以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）
　　	a+ 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。 （原来的EOF符不保留）

　　	wb 只写打开或新建一个二进制文件；只允许写数据。
　　	wb+ 读写打开或建立一个二进制文件，允许读和写。
　　	wt+ 读写打开或着建立一个文本文件；允许读写。

　　	at+ 读写打开一个文本文件，允许读或在文本末追加数据。
　　	ab+ 读写打开一个二进制文件，允许读或在文件末追加数据。
	*/

	Tools_Tool::WindowsSystem::CreateFolder("C:\\Test");
	std::wcout.imbue(locale("chs"));  //设置语言环境

	const wchar_t* 文件路径 = L"C:\\Test\\test.txt";
	wchar_t tempChar[512] = L"";

	std::vector<std::wstring> 写入文本内容;
	std::vector<std::wstring> 读取文本内容;
	写入文本内容.push_back(L"[屏幕设置]\n");
	写入文本内容.push_back(L"屏幕高=1080\n");
	写入文本内容.push_back(L"屏幕宽=1920\n");
	写入文本内容.push_back(L"屏幕高1=1080\n");
	写入文本内容.push_back(L"屏幕宽1=1920\n");
	写入文本内容.push_back(L"屏幕高2=1080\n");
	写入文本内容.push_back(L"屏幕宽2=1920\n");


	//读取文件流
	FILE* 读取文件流指针;
	errno_t err读取文件流指针 = _wfopen_s(&读取文件流指针, 文件路径, L"r, ccs=UTF-8"); //以读取模式打开文件,  UTF-8模式创建新文件的编码为 UTF-8 BOM
	//以读取模式打开文件, Unicode模式创建新文件的编码为 UTF-8; 且无法存入和输出字符
	//errno_t err = _wfopen_s(&读取文件流指针, 文件路径, L"w+, ccs=Unicode");
	if (err读取文件流指针 != 0)
	{
		lg(L"警告: 文件中内容过少, 或不符合格式\n", lgm::wr);
		lg(L"文件创建失败\n\n\n", lgm::wr);
		return; //没有文件, 则不进行读取和解析
	}

	while (!feof(读取文件流指针)) //feof: 文件尾时, 返回非0
	{
		fgetws(tempChar, 512, 读取文件流指针); //读取一行, 最大字符容量 512
		读取文本内容.push_back(tempChar); //保存内容

		lg(L"读取字符: " + *tempChar, lgm::wr);
	}
	fclose(读取文件流指针);
	std::wcout << std::endl;

	//读取文件流
	FILE* 写入文件流指针;
	errno_t err写入文件流指针 = _wfopen_s(&写入文件流指针, 文件路径, L"w+, ccs=UTF-8"); //以读取模式打开文件,  UTF-8模式创建新文件的编码为 UTF-8 BOM
	//以读取模式打开文件, Unicode模式创建新文件的编码为 UTF-8; 且无法存入和输出字符
	//errno_t err = _wfopen_s(&读取文件流指针, 文件路径, L"w+, ccs=Unicode");
	if (err写入文件流指针 != 0)
	{
		lg(L"文件创建失败\n", lgm::wr);
		return; //没有文件, 则不进行读取和解析
	}

	//读取配置文件内容
	for (auto tempBegin = 写入文本内容.begin(); tempBegin != 写入文本内容.end(); tempBegin++)
	{
		fputws(tempBegin->c_str(), 写入文件流指针); // fputs 默认不带换行符, 格式化()中已添加

		lg((L"写入缓冲字符: " + *tempBegin).c_str(), lgm::wr);
	}
	fclose(写入文件流指针);
	std::wcout << L"文件创建成功, 路径: " << 文件路径 << L"\n\n\n";
}