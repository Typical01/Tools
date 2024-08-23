#include "pch.h"

#include "ToolsConfigFile.h"
#include "CommonTools.h"
#include "CharHandleOfConfigFile.h"


using namespace Tools_Tool::WindowsSystem;

//----------------------------- class Tools_Tool::ToolsConfigFile

void Tools_Tool::ToolsConfigFile::初始化(Ustr 程序_名)
{
	this->程序_名 = T("\\") + 程序_名;

	//获取当前程序的全路径
	GetModuleFileName(NULL, this->程序所在路径, MAX_PATH);

	//工具箱程序所在路径
	this->程序_路径名 = this->程序所在路径;

	//程序父文件夹路径名
	int 工具箱文件_路径名长度;
	工具箱文件_路径名长度 = (this->程序_名 + T(".exe")).size(); //Tools.exe 长度
	int temp程序父文件夹_路径名长度;
	temp程序父文件夹_路径名长度 = this->程序_路径名.size() - 工具箱文件_路径名长度; //程序父文件夹路径 = 程序全路径 - 程序名
	Ustr temp程序父文件夹_路径名(T(" "), temp程序父文件夹_路径名长度);
	for (int i = 0; i < temp程序父文件夹_路径名长度; i++)
	{
		temp程序父文件夹_路径名[i] = this->程序_路径名[i];
	}
	this->程序父文件夹_路径名 = temp程序父文件夹_路径名;

	//配置文件
	this->工具箱配置文件_路径名 = this->程序父文件夹_路径名 + T("\\Config") + T("\\ToolsConfig.ini");
}

Ustr Tools_Tool::ToolsConfigFile::Get工具箱配置文件路径() const
{
	return this->工具箱配置文件_路径名;
}

Ustr Tools_Tool::ToolsConfigFile::Get程序父文件夹路径() const
{
	return this->程序父文件夹_路径名;
}

Ustr Tools_Tool::ToolsConfigFile::Get程序路径() const
{
	return this->程序_路径名;
}

void Tools_Tool::ToolsConfigFile::打开配置文件()
{
	lgc(T("打开配置文件"));

	auto temp = WinShell::执行(T("打开文件"), T("open"), this->工具箱配置文件_路径名);
	if (temp.IsSucceed()) {
		lgc(T("成功: 打开配置文件"), lgm::wr);
	}
}
