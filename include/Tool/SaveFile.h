#pragma once

//SaveFile.h 文件操作
#ifndef _SAVEFILE_H
#define _SAVEFILE_H
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Tools_Tool.h"

#include "ToolsConfigFile.h"
#include "CommonTools.h"


using namespace std;
using namespace Tools_Tool::StringHandling;

namespace Tools_Tool {

    //---------------------------------------------------------------文件操作 FileOperations-----------------------------------------------------------

    class TOOLS_TOOL_API FileOperations
    {
    private:
        std::string program_Path; //程序路径·····
        std::string project_Path; //项目路径

        //文件路径名
        std::string file_PathAndName; //配置文件路径

        //文件后缀
        std::string file_Suffix; //FileOperations-config.txt 配置文件

    public:
        FileOperations()
            : program_Path("\0"), project_Path("\0"), file_PathAndName("\0"), file_Suffix("FileOperations-config.txt") { }

        //转移steam.exe位置
        int copyFileSteam(std::string path, std::string project_Path, std::string program_Suffix);

        //还原steam.exe位置
        int copyFileSteamReduction(std::string path, std::string project_Path, std::string program_Suffix);

        //从配置文件中获取 steam.exe 文件的位置
        void GetFilePath();

        /*
        //获取 项目程序路径
        void GetProgramPath(ToolsConfigFile& ToolsCfgF);
        */
        //复制文件
        int copyFile(char* SourceFile, char* NewFile, void (*CerrPrint)());

        //复制文件
        int copyFile(char* SourceFile, char* NewFile);

        //执行复制文件操作
        void copyFile_Run();

        //删除文件
        void deleteFile();

        //保存数据
        void saveData(string& filepath, string& filaName, string& fileSuffixConfig); //未完成


    };
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif