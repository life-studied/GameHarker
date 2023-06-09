﻿// CWndModuleList.cpp: 实现文件
//

#include "pch.h"
#include "GH3.h"
#include "afxdialogex.h"
#include "CWndModuleList.h"
#include <TlHelp32.h>


// CWndModuleList 对话框

IMPLEMENT_DYNAMIC(CWndModuleList, CDialogEx)

CWndModuleList::CWndModuleList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOD_LIST, pParent)
{

}

CWndModuleList::~CWndModuleList()
{
}

void CWndModuleList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, LstModule);
}

BOOL CWndModuleList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(LstModule.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT;
	SetWindowLongPtr(LstModule.m_hWnd, GWL_STYLE, lStyle);

	//设置list control的extend style
	DWORD dStyle = LstModule.GetExtendedStyle();
	dStyle |= LVS_EX_FULLROWSELECT;
	dStyle |= LVS_EX_GRIDLINES;
	LstModule.SetExtendedStyle(dStyle);

	//插入表头
	LstModule.InsertColumn(0, L"模块名称", 0, 100);		//column:0	像素width:200
	LstModule.InsertColumn(1, L"基址", 0, 200);
	LstModule.InsertColumn(2, L"大小", 0, 200);
	LstModule.InsertColumn(3, L"文件位置", 0, 200);

	return TRUE;
}

void CWndModuleList::GetModList(DWORD Pid)
{
	LstModule.DeleteAllItems();
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Pid);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modl{ sizeof(MODULEENTRY32) };
		BOOL BMORE = Module32First(hSnap, &modl);
		while (BMORE)
		{
			CString txt;
			txt.Format(L"%X", modl.hModule);
			LstModule.InsertItem(0, modl.szModule);
			LstModule.SetItemText(0, 1, txt);
			txt.Format(L"%X", modl.modBaseSize);
			LstModule.SetItemText(0, 2, txt);
			txt.Format(L"%s", modl.szExePath);
			LstModule.SetItemText(0, 3, txt);
			BMORE = Module32Next(hSnap, &modl);
		}
	}
	CloseHandle(hSnap);
}


BEGIN_MESSAGE_MAP(CWndModuleList, CDialogEx)
END_MESSAGE_MAP()


// CWndModuleList 消息处理程序
