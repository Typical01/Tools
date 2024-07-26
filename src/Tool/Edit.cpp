#include "pch.h"
#include "Edit.h"

void Tools_Tool::WindowsSystem::WindowEdit::SetWindowsSize()
{
    RECT rcTabDisplay_Tab;
    //获取Tab控件客户区的的大小（Tab窗口坐标系）
    GetClientRect(hWnd_标签, &rcTabDisplay_Tab);//获取Tab窗口的大小（而不是在屏幕坐标系中）
    //TabCtrl_AdjustRect(hWnd, FALSE, &rcTabDisplay_Tab);//通过窗口大小，获取客户区大小。
    MoveWindow(hWnd_标签列控件, rcTabDisplay_Tab.left, rcTabDisplay_Tab.top, rcTabDisplay_Tab.right, rcTabDisplay_Tab.bottom - 20, 0);

    RECT rcTabDisplay_Edit;
    GetClientRect(hWnd_标签列控件, &rcTabDisplay_Edit);//获取Tab窗口的大小（而不是在屏幕坐标系中）
    TabCtrl_AdjustRect(hWnd_标签列控件, FALSE, &rcTabDisplay_Edit);//通过窗口大小，获取客户区大小。

    for (auto temp = hWnd_标签编辑窗口.begin(); temp != hWnd_标签编辑窗口.end(); temp++)
    {
        MoveWindow(temp->second, rcTabDisplay_Edit.left - 5, rcTabDisplay_Edit.top, rcTabDisplay_Edit.right + 5, rcTabDisplay_Edit.bottom, 0);
    }
}

void Tools_Tool::WindowsSystem::WindowEdit::CreateEditWindows(int tempInt)
{
    auto 窗口是否存在 = hWnd_标签编辑窗口.find(tempInt);
    if (窗口是否存在 == hWnd_标签编辑窗口.end())
    {
        HWND 新建的编辑框 = CreateWindow(
            L"EDIT",
            std::to_wstring(tempInt).c_str(),
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL | ES_MULTILINE | WS_TABSTOP,
            0,
            24,
            550,
            237,
            hWnd_标签,
            NULL,
            GetModuleHandle(NULL),
            NULL);
        WH.添加窗口托管(L"新建的编辑框", 新建的编辑框, false);

        //保存编辑框
        hWnd_标签编辑窗口.insert(std::make_pair(tempInt, 新建的编辑框));

        if (IsWindow(最新的编辑窗口)) {
            ShowWindow(最新的编辑窗口, SW_HIDE);
        }
        最新的编辑窗口 = 新建的编辑框;
        ShowWindow(最新的编辑窗口, SW_SHOW);

        lgc(std::to_wstring(tempInt).c_str(), L"添加 新建的编辑框");

        //聚焦于 最新的编辑窗口
        SendMessage(hWnd_标签列控件, TCM_SETCURFOCUS, tempInt, 0);
    }
    else //如果有
    {
        ShowWindow(最新的编辑窗口, SW_HIDE);
        最新的编辑窗口 = 窗口是否存在->second;

        //显示标签对应的窗口
        ShowWindow(最新的编辑窗口, SW_SHOW);
        int i = SendMessage(hWnd_标签列控件, TCM_GETITEMCOUNT, 0, 0);
        SendMessage(hWnd_标签列控件, TCM_SETCURFOCUS, i, 0);
    }
}