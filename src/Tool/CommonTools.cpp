#include "pch.h"

#include "CommonTools.h"
#include "Message.h"

#include <iostream>
#include <io.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <comdef.h>

#include <vector>
#include <string>


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
}

HRESULT Tools_Tool::WindowsSystem::CreateLink(LPCTSTR 对象路径, LPCTSTR 快捷方式路径, LPCTSTR 图标路径, LPCTSTR 快捷方式描述, LPCTSTR 目标程序的参数)
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
		psl->SetPath(对象路径);
		if (快捷方式描述)
		{
			psl->SetDescription(快捷方式描述);
		}
		if (图标路径)
		{
			psl->SetIconLocation(图标路径, 0);
		}
		if (目标程序的参数)
		{
			psl->SetArguments(目标程序的参数);
		}

		// Query IShellLink for the IPersistFile interface, used for saving the 
		// shortcut in persistent storage. 
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

		if (SUCCEEDED(hres))
		{
			/*
			WCHAR wsz[MAX_PATH];
			// Ensure that the string is Unicode.
			MultiByteToWideChar(CP_UTF8, 0, lpszPathLink, -1, wsz, MAX_PATH);
			*/
			// Save the link by calling IPersistFile::Save. 
#ifdef UNICODE
			hres = ppf->Save(快捷方式路径.c_str(), TRUE);
#else
			hres = ppf->Save(Tools_Tool::StringHandling::stow(快捷方式路径).c_str(), TRUE);
#endif
			ppf->Release();
		}
		psl->Release();
	}
	return hres;
}


std::wstring Tools_Tool::StringHandling::StringToWstring(const std::string& str)
{
	std::wstring wContext;

	if (str.empty()) {
		return wContext;
	}

#ifdef _WINDOWS
	// Windows 版本
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), nullptr, 0);
	if (len <= 0) {
		throw std::runtime_error("Failed to convert string to wide string.");
	}
	std::unique_ptr<wchar_t[]> buffer(new wchar_t[len + 1]);
	if (MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), buffer.get(), len) <= 0) {
		throw std::runtime_error("Failed to convert string to wide string.");
	}
	buffer[len] = T('\0');
	wContext.assign(buffer.get());
#else
	// POSIX 版本
	std::unique_ptr<char[]> buffer(new char[str.size() * 4]);  // 大小预留空间
	size_t out_len = str.size() * 4;
	iconv_t cd = iconv_open("UTF-16LE", "ISO-8859-1");
	if (cd == (iconv_t)-1) {
		throw std::runtime_error("Failed to open iconv descriptor.");
	}
	if (iconv(cd, const_cast<const char**>(&str.c_str()), &str.size(), reinterpret_cast<char**>(buffer.get()), &out_len) == (size_t)-1) {
		iconv_close(cd);
		throw std::runtime_error("Failed to convert string using iconv.");
	}
	iconv_close(cd);
	wContext.assign(reinterpret_cast<wchar_t*>(buffer.get()));
#endif

	return wContext;
}

std::wstring Tools_Tool::StringHandling::StringToWstring(std::string&& str)
{
	std::wstring wContext;

	if (str.empty()) {
		return wContext;
	}

#ifdef _WINDOWS
	// Windows 版本
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), nullptr, 0);
	if (len <= 0) {
		throw std::runtime_error("Failed to convert string to wide string.");
	}
	std::unique_ptr<wchar_t[]> buffer(new wchar_t[len + 1]);
	if (MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), buffer.get(), len) <= 0) {
		throw std::runtime_error("Failed to convert string to wide string.");
	}
	buffer[len] = T('\0');
	wContext.assign(buffer.get());
#else
	// POSIX 版本
	std::unique_ptr<char[]> buffer(new char[str.size() * 4]);  // 大小预留空间
	size_t out_len = str.size() * 4;
	iconv_t cd = iconv_open("UTF-16LE", "ISO-8859-1");
	if (cd == (iconv_t)-1) {
		throw std::runtime_error("Failed to open iconv descriptor.");
	}
	if (iconv(cd, const_cast<const char**>(&str.c_str()), &str.size(), reinterpret_cast<char**>(buffer.get()), &out_len) == (size_t)-1) {
		iconv_close(cd);
		throw std::runtime_error("Failed to convert string using iconv.");
	}
	iconv_close(cd);
	wContext.assign(reinterpret_cast<wchar_t*>(buffer.get()));
#endif

	return wContext;
}

std::string Tools_Tool::StringHandling::WstringToString(const std::wstring& wStr)
{
	std::string context;

	if (wStr.empty()) {
		return context;
	}

#ifdef _WINDOWS
	// Windows 版本
	int len = WideCharToMultiByte(CP_UTF8, 0, wStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (len <= 0) {
		throw std::runtime_error("Failed to convert wide string to string.");
	}
	std::unique_ptr<char[]> buffer(new char[len + 1]);
	if (WideCharToMultiByte(CP_UTF8, 0, wStr.c_str(), -1, buffer.get(), len, nullptr, nullptr) <= 0) {
		throw std::runtime_error("Failed to convert wide string to string.");
	}
	buffer[len] = '\0';
	context.assign(buffer.get());
#else
	// POSIX 版本
	std::unique_ptr<char[]> buffer(new char[wStr.size() * 4]);  // 大小预留空间
	size_t out_len = wStr.size() * 4;
	iconv_t cd = iconv_open("ISO-8859-1", "UTF-16LE");
	if (cd == (iconv_t)-1) {
		throw std::runtime_error("Failed to open iconv descriptor.");
	}
	size_t in_len = wStr.size() * sizeof(wchar_t);
	const char* in = reinterpret_cast<const char*>(wStr.c_str());
	if (iconv(cd, const_cast<const char**>(&in), &in_len, reinterpret_cast<char**>(buffer.get()), &out_len) == (size_t)-1) {
		iconv_close(cd);
		throw std::runtime_error("Failed to convert wide string using iconv.");
	}
	iconv_close(cd);
	context.assign(buffer.get(), buffer.get() + (wStr.size() * 4 - out_len));
#endif

	return context;
}

std::string Tools_Tool::StringHandling::WstringToString(std::wstring&& wStr)
{
	std::string context;

	if (wStr.empty()) {
		return context;
	}

#ifdef _WINDOWS
	// Windows 版本
	int len = WideCharToMultiByte(CP_UTF8, 0, wStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (len <= 0) {
		throw std::runtime_error("Failed to convert wide string to string.");
	}
	std::unique_ptr<char[]> buffer(new char[len + 1]);
	if (WideCharToMultiByte(CP_UTF8, 0, wStr.c_str(), -1, buffer.get(), len, nullptr, nullptr) <= 0) {
		throw std::runtime_error("Failed to convert wide string to string.");
	}
	buffer[len] = '\0';
	context.assign(buffer.get());
#else
	// POSIX 版本
	std::unique_ptr<char[]> buffer(new char[wStr.size() * 4]);  // 大小预留空间
	size_t out_len = wStr.size() * 4;
	iconv_t cd = iconv_open("ISO-8859-1", "UTF-16LE");
	if (cd == (iconv_t)-1) {
		throw std::runtime_error("Failed to open iconv descriptor.");
	}
	size_t in_len = wStr.size() * sizeof(wchar_t);
	const char* in = reinterpret_cast<const char*>(wStr.c_str());
	if (iconv(cd, const_cast<const char**>(&in), &in_len, reinterpret_cast<char**>(buffer.get()), &out_len) == (size_t)-1) {
		iconv_close(cd);
		throw std::runtime_error("Failed to convert wide string using iconv.");
	}
	iconv_close(cd);
	context.assign(buffer.get(), buffer.get() + (wStr.size() * 4 - out_len));
#endif

	return context;
}

bool Tools_Tool::WindowsSystem::Get目录所有文件夹(std::string path, std::vector<std::string>& folder) {
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

void Tools_Tool::WindowsSystem::Get目录所有文件(std::string path, vector<std::string>& files)
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
					Get目录所有文件(p.assign(path).append("\\").append(fileinfo.name), files);
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

std::vector<std::string> Tools_Tool::WindowsSystem::Get目录所有文件夹(std::string path)
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

vector<std::string> Tools_Tool::WindowsSystem::Get目录所有文件(std::string path)
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
					Get目录所有文件(p.assign(path).append("\\").append(fileinfo.name), files);
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
