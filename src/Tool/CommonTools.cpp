#include "pch.h" //DLL标准

#include "CommonTools.h"

#include <iostream>
#include <io.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <comdef.h>

#include <vector>
#include <string>


void Tools_Tool::WindowsSystem::ShellMessage::operator=(int message)
{
	if (message < 32)
	{
		this->Status = false;

		//ShellExecute() 成功操作, 则传入为句柄
		this->ErrorCode = message;
		lgc((L"Shell消息[" + this->信息 + L"]: 错误😒 -> 代码(" + to_wstring(message) + L")").c_str(), lgm::er);
	}
	else
	{
		this->Status = true;
		this->ErrorCode = message;
		lgc((L"Shell消息[" + this->信息 + L"]: 成功 👌").c_str());
	}
}

int Tools_Tool::WindowsSystem::ShellMessage::GetErrorCode()
{
	return this->ErrorCode;
}

bool Tools_Tool::WindowsSystem::ShellMessage::isSucceed()
{
	return this->Status;
}

void Tools_Tool::WindowsSystem::RegisterHotKeyMessage::operator=(int message)
{
	if (message > 0)
	{
		lgc((L"热键注册[" + this->信息 + L"]: 成功👌").c_str());
	}
	else
	{
		lgc((L"热键注册[" + this->信息 + L"]: 错误😒 -> 代码(" + to_wstring(message) + L")").c_str(), lgm::er);
	}
}


void Tools_Tool::WindowsSystem::SetDisplaySize(int displayWidth, int displayHeight)
{
	//初始化
	DEVMODE NewDevMode;
	EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &NewDevMode);

	//记录修改信息
	NewDevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	NewDevMode.dmPelsWidth = displayWidth;
	NewDevMode.dmPelsHeight = displayHeight;

	//根据修改信息 修改屏幕分辨率
	ChangeDisplaySettings(&NewDevMode, 0);
}

void Tools_Tool::WindowsSystem::StartFile(const wchar_t* filePath)
{
	ShellExecuteW(NULL, L"open", filePath, NULL, NULL, SW_SHOW);
}

DWORD Tools_Tool::WindowsSystem::FindProcessIDByName(const std::string& processName) //0 not found ; other found; processName "processName.exe" 根据名称查找进程ID
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return(0);
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap); // clean the snapshot object 清理快照对象
		return(0);
	}
	DWORD processId = 0;
	do
	{
		if (std::wstring(pe32.szExeFile) == Tools_Tool::StringHandling::ANSIToUnicode(processName)) //进程名称
		{
			processId = pe32.th32ProcessID; //进程ID
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));
	CloseHandle(hProcessSnap);
	return(processId);
}

long Tools_Tool::WindowsSystem::GetProcessIdFromName(const char* name)
{
	HANDLE hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hsnapshot == INVALID_HANDLE_VALUE)
	{
		cout << "Create TlHelp32 Error!" << endl;
		return -1;
	}

	PROCESSENTRY32 processer;
	processer.dwSize = sizeof(PROCESSENTRY32);

	int flag = Process32First(hsnapshot, &processer);
	while (flag != 0)
	{
		_bstr_t processName(processer.szExeFile);  //WCHAR字符串转换成CHAR字符串
		if (strcmp(processName, name) == 0)
		{
			return processer.th32ProcessID;        //返回进程ID
		}
		flag = Process32Next(hsnapshot, &processer);
	}

	CloseHandle(hsnapshot);
	return -2;
}

bool Tools_Tool::WindowsSystem::isProgramRunning(const std::string& program_name)
{
	bool ret = false;
	HANDLE info_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //拍摄系统中所有进程的快照: TH32CS_SNAPPROCESS
	if (info_handle == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot fail!!\n\n");
		return false;
	}

	PROCESSENTRY32W program_info;
	program_info.dwSize = sizeof(PROCESSENTRY32W);  //设置结构体大小
	int bResult = Process32FirstW(info_handle, &program_info); //获取所有进程中第一个进程的信息
	if (!bResult)
	{
		printf("Process32FirstW fail!!\n\n");
		return false;
	}

	while (bResult)
	{
		char* pro_name = (char*)Tools_Tool::StringHandling::WcharToChar(program_info.szExeFile).c_str();
		if (program_name == std::string(pro_name))
		{
			ret = true;
			break;
		}
		//获得下一个进程的进程信息
		bResult = Process32Next(info_handle, &program_info);
	}
	CloseHandle(info_handle);//关闭句柄
	return ret;
}

TOOLS_TOOL_API bool Tools_Tool::WindowsSystem::IsUserAdmin()
{
	BOOL retVal = FALSE;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup;
	BOOL result = AllocateAndInitializeSid(
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&AdministratorsGroup);

	if (result)
	{
		if (!CheckTokenMembership(NULL, AdministratorsGroup, &retVal))
		{
			retVal = FALSE;
		}
		FreeSid(AdministratorsGroup);
	}
	return retVal;

	//BOOL isAdminToken = FALSE; //管理员令牌
	//HANDLE hToken = NULL;

	//// 获取当前进程的访问令牌
	//if (OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &hToken))
	//{
	//	// 管理员组的SID
	//	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	//	PSID pAdminSid = NULL;

	//	if (AllocateAndInitializeSid(&NtAuthority,
	//		2,
	//		SECURITY_BUILTIN_DOMAIN_RID,
	//		DOMAIN_ALIAS_RID_ADMINS,
	//		0, 0, 0, 0, 0, 0,
	//		&pAdminSid) == TRUE)
	//	{
	//		// 检查令牌中是否包含管理员组的SID
	//		if (CheckTokenMembership(hToken, pAdminSid, &isAdminToken) != 0)
	//		{
	//			// 如果CheckTokenMembership返回非零值，表示成功
	//			// 并且isAdminToken变量将设置为TRUE或FALSE

	//			if (!isAdminToken)
	//			{
	//				// 你可能希望在这里记录一个错误，但不是通过printf，因为它可能在一个没有权限的窗口中
	//			}

	//			// 释放管理员组的SID
	//			FreeSid(pAdminSid);

	//			// 关闭令牌句柄
	//			CloseHandle(hToken);

	//			// 返回检查结果
	//			return isAdminToken;
	//		}

	//		// 释放管理员组的SID
	//		FreeSid(pAdminSid);
	//	}
	//	// 关闭访问令牌句柄
	//	CloseHandle(hToken);
	//}

	//return false;
}

bool Tools_Tool::WindowsSystem::GainAdminPrivileges(const wchar_t* strApp)
{
	ShellMessage UserAdmin(_T("申请管理员权限"));

	if (!IsUserAdmin()) { //非管理员权限, 则申请

		//ShellExecuteEx(SHELLEXECUTEINFO)
		/*SHELLEXECUTEINFO execinfo;
		memset(&execinfo, 0, sizeof(execinfo));
		execinfo.lpFile = strApp;
		execinfo.cbSize = sizeof(execinfo);
		execinfo.lpVerb = _T("runas");
		execinfo.fMask = SEE_MASK_NO_CONSOLE;
		execinfo.nShow = SW_SHOWDEFAULT;

		ShellExecuteEx(&execinfo);*/

		//ShellExecute(...)
		UserAdmin = (int)ShellExecute(NULL, _T("runas"), strApp, NULL, NULL, SW_SHOWNORMAL);
	}

	//成功申请时, 退出当前进程
	if (UserAdmin.isSucceed()) {
		return true;
	}

	return false;
}

bool Tools_Tool::WindowsSystem::SetAutoRun(const wchar_t* valueName, const wchar_t* exePath)
{
	LONG result;
	HKEY hKey;

	const wchar_t* regPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

	// 打开注册表项
	result = RegOpenKeyExW(HKEY_CURRENT_USER, regPath, 0, KEY_SET_VALUE, &hKey);
	if (result != ERROR_SUCCESS) {
		std::cerr << "RegOpenKeyEx failed: " << result << std::endl;
		return false;
	}

	// 设置注册表值
	result = RegSetValueExW(hKey, valueName, 0, REG_SZ, (const BYTE*)exePath, (_tcslen(exePath) + 1) * sizeof(char));
	if (result != ERROR_SUCCESS) {
		std::cerr << "RegSetValueEx failed: " << result << std::endl;
		RegCloseKey(hKey);
		return false;
	}

	RegCloseKey(hKey);
	return true;
}

HRESULT Tools_Tool::WindowsSystem::CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, LPCWSTR lpszIcon, LPCWSTR lpszDesc, LPCWSTR lpszArgs)
{
	HRESULT hres;
	IShellLink* psl;

	// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
	// has already been called.
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	DWORD errorCode = ::GetLastError();

	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;

		// Set the path to the shortcut target and add the description. 
		psl->SetPath(lpszPathObj);
		if (lpszDesc)
		{
			psl->SetDescription(lpszDesc);
		}
		if (lpszIcon)
		{
			psl->SetIconLocation(lpszIcon, 0);
		}
		if (lpszArgs)
		{
			psl->SetArguments(lpszArgs);
		}

		// Query IShellLink for the IPersistFile interface, used for saving the 
		// shortcut in persistent storage. 
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

		if (SUCCEEDED(hres))
		{
			/*
			WCHAR wsz[MAX_PATH];
			// Ensure that the string is Unicode.
			MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);
			*/
			// Save the link by calling IPersistFile::Save. 
			hres = ppf->Save(lpszPathLink, TRUE);
			ppf->Release();
		}
		psl->Release();
	}
	return hres;
}

std::wstring Tools_Tool::StringHandling::StringToWstring(const std::string& str)
{
	std::wstring wContext = L"";
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	WCHAR* buffer = new WCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';
	wContext.append(buffer);
	delete[] buffer;

	return wContext;
}


//将wstring转换成string  
std::string Tools_Tool::StringHandling::WstringToString(std::wstring wstr)
{
	string result;
	//获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//宽字节编码转换成多字节编码  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}

std::wstring Tools_Tool::StringHandling::CharToWchar(const char* c, size_t m_encode)
{
	std::wstring str;
	int len = MultiByteToWideChar(m_encode, 0, c, strlen(c), NULL, 0);
	wchar_t* m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(m_encode, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	str = m_wchar;
	delete[] m_wchar;
	return str;
}

std::string Tools_Tool::StringHandling::WcharToChar(const wchar_t* wp, size_t m_encode)
{
	std::string str;
	int len = WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	char* m_char = new char[len + 1];
	WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	str = m_char;
	delete[] m_char;
	return str;
}

std::string Tools_Tool::StringHandling::UTF8ToGB2312(const char* str)
{
	std::string result;
	WCHAR* strSrc;
	LPSTR szRes;

	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//获得临时变量的大小
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}

std::string Tools_Tool::StringHandling::UTF8ToGBK(const char* utf8_str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}

std::string Tools_Tool::StringHandling::GBKToUTF8(const char* gbk_str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gbk_str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gbk_str, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	std::string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}

void Tools_Tool::WindowsSystem::CreateFiles(const string& FilePath)
{
	string command;
	command = "mkdir -p " + FilePath;
	system(command.c_str());
}

bool Tools_Tool::WindowsSystem::GetAllFolder(std::string path, std::vector<std::string>& folder) {
	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t folderInfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &folderInfo)) != -1) {
		do {
			if ((folderInfo.attrib & _A_SUBDIR)) {
				if (strcmp(folderInfo.name, ".") != 0 && strcmp(folderInfo.name, "..") != 0) {
					folder.push_back(p.assign(path).append("\\").append(folderInfo.name));
				}
			}
			else {
				std::cout << "not a folder!\n";
			}
		} while (_findnext(hFile, &folderInfo) == 0);
		_findclose(hFile); //结束查找 
	}
	return true;
}

void Tools_Tool::WindowsSystem::GetAllFiles(std::string path, vector<std::string>& files)
{
	intptr_t hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}

std::vector<std::string> Tools_Tool::WindowsSystem::GetAllFolder(std::string path)
{
	vector<std::string> folder;

	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t folderInfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &folderInfo)) != -1) {
		do {
			if ((folderInfo.attrib & _A_SUBDIR)) {
				if (strcmp(folderInfo.name, ".") != 0 && strcmp(folderInfo.name, "..") != 0) {
					folder.push_back(p.assign(path).append("\\").append(folderInfo.name));
				}
			}
			else {
				std::cout << "not a folder!\n";
			}
		} while (_findnext(hFile, &folderInfo) == 0);
		_findclose(hFile); //结束查找
	}

	return folder;
}

vector<std::string> Tools_Tool::WindowsSystem::GetAllFiles(std::string path)
{
	vector<std::string> files;

	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}

	return files;
}

bool Tools_Tool::WindowsSystem::CreateFolder(std::string folderPath)
{
	//属性不为目录时
	if (GetFileAttributesA(folderPath.c_str()) != FILE_ATTRIBUTE_DIRECTORY)
	{
		//创建目录
		if (CreateDirectoryA(folderPath.c_str(), NULL))
		{
			return true;
		}
		
		//已有文件夹, 虽然创建失败, 但是需求达到了
		return true;
	}

	return false;
}

void Tools_Tool::WindowsSystem::OpenFolder(const std::string& path)
{
	//字符集 支持Unicode 和 多字节
#ifdef  _UNICODE
	std::wstring tmp = Tools_Tool::StringHandling::StringToWstring(path);
	LPCWSTR dir = tmp.c_str();
	ShellExecute(NULL, NULL, dir, NULL, NULL, SW_SHOWNORMAL);
#elif _MBCS
	LPCSTR dir = path.c_str();
	ShellExecute(NULL, NULL, dir, NULL, NULL, SW_SHOWNORMAL);
#endif 
}

void Tools_Tool::WindowsSystem::MoveCursorLocation(int x, int y) {
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}

float Tools_Tool::GameTools::GetFps()
{
	//临时统计
	static int tempCount = 0;
	static Timers timer; //时间
	static float fps; //帧率

	if (tempCount > FPS_COUNT)
	{
		//将时间节点拓展到两个
		if (timer.GetTimerSize() < 2)
			timer.AddTimer();

		tempCount = 0;
		timer.SetTimer(timer.GetTime(), 2);
		float tempTime = timer.计算首尾间隔();
		fps = FPS_COUNT / (tempTime / 1000.0f); //获取的系统时间为毫秒数
		timer.SetTimer(timer.GetTime(), 1);
	}

	tempCount++;
	return fps;
}
