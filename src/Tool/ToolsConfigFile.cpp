#include "pch.h" //DLL标准
#include <utility> //DLL标准
#include <limits.h> //DLL标准

#include "ToolsConfigFile.h"
#include "CommonTools.h"
#include "CharHandleOfConfigFile.h"


using namespace Tools_Tool::WindowsSystem;

//----------------------------- class Tools_Tool::ToolsConfigFile

void Tools_Tool::ToolsConfigFile::初始化(std::wstring 程序_名)
{
	this->程序_名 = L"\\" + 程序_名;

	//获取当前程序的全路径
	GetModuleFileNameW(NULL, this->程序所在路径, MAX_PATH);

	//工具箱程序所在路径
	this->程序_路径名 = this->程序所在路径;

	int 工具箱文件_路径名长度;
	//Tools.exe 长度
	工具箱文件_路径名长度 = (this->程序_名 + this->exe后缀).size();

	int temp程序父文件夹_路径名长度;
	//程序父文件夹路径 = 程序全路径 - 程序名
	temp程序父文件夹_路径名长度 = this->程序_路径名.size() - 工具箱文件_路径名长度;

	std::wstring temp程序父文件夹_路径名(L" ", temp程序父文件夹_路径名长度);
	for (int i = 0; i < temp程序父文件夹_路径名长度; i++)
	{
		temp程序父文件夹_路径名[i] = this->程序_路径名[i];
	}
	this->程序父文件夹_路径名 = temp程序父文件夹_路径名;


	//获取全路径
	this->清单文件_路径名 = this->程序父文件夹_路径名 + this->配置文件夹_名 + this->清单文件_名 + this->txt后缀;
	this->工具箱配置文件_路径名 = this->程序父文件夹_路径名 + this->配置文件夹_名 + this->工具箱配置文件_名 + this->ini后缀;

	this->笔记本键盘关_路径名 = this->程序父文件夹_路径名 + this->配置文件夹_名 + this->笔记本键盘关_名;
	this->笔记本键盘开_路径名 = this->程序父文件夹_路径名 + this->配置文件夹_名 + this->笔记本键盘开_名;
	this->smi_路径名 = this->程序父文件夹_路径名 + this->配置文件夹_名 + this->smi_名;
	this->ping_路径名 = this->程序父文件夹_路径名 + this->配置文件夹_名 + this->ping_名;


	/*lg((L"工具箱配置文件_路径:" + this->工具箱配置文件_路径).c_str(), lgm::wr);
	lg((L"清单文件_路径:" + this->清单文件_路径).c_str(), lgm::wr);
	lg((L"程序父文件夹_路径名:" + this->程序父文件夹_路径名).c_str(), lgm::wr);
	lg((L"工具箱程序_路径:" + this->工具箱程序_路径).c_str(), lgm::wr);*/


	//先创建文件夹(否则后面的文件不能创建): \\Tools\\config
	if (WindowsSystem::CreateFolder(WstringToString(this->程序父文件夹_路径名 + this->配置文件夹_名)))
	{
		lgc((L"路径: " + this->程序父文件夹_路径名 + L" 文件夹创建成功!").c_str(), lgm::wr);
	}
	else
	{
		lgc((L"路径: " + this->程序父文件夹_路径名 + L" 已有对应文件夹!").c_str(), lgm::wr);
	}
	//创建对应文件
	this->创建配置文件();
	this->创建清单文件();
}

std::wstring Tools_Tool::ToolsConfigFile::Get工具箱配置文件路径() const
{
	return this->工具箱配置文件_路径名;
}

std::map<std::wstring, std::wstring> Tools_Tool::ToolsConfigFile::Get基本设置内容() const
{
	return this->基本设置内容;
}

std::map<std::wstring, std::map<std::wstring, std::wstring>> Tools_Tool::ToolsConfigFile::Get配置文件全内容() const
{
	return this->配置文件全内容;
}

std::wstring Tools_Tool::ToolsConfigFile::Get程序父文件夹路径() const
{
	return this->程序父文件夹_路径名;
}

std::wstring Tools_Tool::ToolsConfigFile::Get程序配置文件夹路径() const
{
	return this->程序父文件夹_路径名 + this->配置文件夹_名;
}

void Tools_Tool::ToolsConfigFile::创建配置文件()
{
	CharHandleOfConfigFile 配置文件;

	//文件不存在时，创建
	if (!配置文件.Init(this->工具箱配置文件_路径名.c_str()))
	{
		std::vector<std::wstring> 区域内容;
		区域内容.push_back(L"原本屏幕分辨率宽=1920");
		区域内容.push_back(L"原本屏幕分辨率高=1080");
		区域内容.push_back(L"修改的屏幕分辨率宽=1280");
		区域内容.push_back(L"修改的屏幕分辨率高=1024");
		区域内容.push_back(L"repos=C:\\Users\\22793\\source\\repos");
		区域内容.push_back(L"Lib=C:\\Typical\\ProgramProject\\C++\\Libs");
		区域内容.push_back(L"笔记本键盘开关状态=开");
		区域内容.push_back(L"nvidia-smi=关");

		配置文件.添加区域(L"基本设置", 区域内容);
		配置文件.写入文件();

		//第一次创建文件后, 进行配置文件解析和内容获取
		this->基本设置内容 = 配置文件.Get指定区域内容(L"基本设置");
		this->配置文件全内容 = 配置文件.Get配置文件全内容();
	}
	else //文件存在
	{
		this->基本设置内容 = 配置文件.Get指定区域内容(L"基本设置");
		this->配置文件全内容 = 配置文件.Get配置文件全内容();
	}
}


void Tools_Tool::ToolsConfigFile::创建清单文件()
{
	CharHandleOfConfigFile 清单文件;

	//文件不存在时，创建
	if (!清单文件.Init(this->清单文件_路径名.c_str(), false))
	{
		清单文件.创建文件();
	}
	else
	{
		//使用记事本打开编辑文件, 所以不用多做修改
	}
}

void Tools_Tool::ToolsConfigFile::打开配置文件()
{
	StartFile(this->工具箱配置文件_路径名.c_str());
}

void Tools_Tool::ToolsConfigFile::清单开关()
{
	//暂时为: 用记事本打开
	if (this->清单状态)
	{
		//直接关闭全部: 笔记本类名的窗口
		SendMessage(FindWindowW(L"Notepad", NULL), WM_SYSCOMMAND, SC_CLOSE, 0);
		this->清单状态 = false;
	}
	else
	{
		StartFile(this->清单文件_路径名.c_str());
		this->清单状态 = true;
	}
}
