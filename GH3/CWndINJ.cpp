﻿// CWndINJ.cpp: 实现文件
//

#include "pch.h"
#include "GH3.h"

#include "CWndINJ.h"
#include "afxdialogex.h"

// CWndINJ 对话框

IMPLEMENT_DYNAMIC(CWndINJ, CDialogEx)

CWndINJ::CWndINJ(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_0, pParent)
	, B_INJECT(FALSE)
	, B_PAUSE(FALSE)
	, B_DEBUG(FALSE)
{

}

CWndINJ::~CWndINJ()
{
}

BOOL CWndINJ::OnInitDialog()
{
	CDialogEx::OnInitDialog();		//调用父类的初始化函数（因为本身是一个虚函数，重载时要调用父类再写自己的）

	//设置list control的style
	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(ExeLst.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT;
	SetWindowLongPtr(ExeLst.m_hWnd, GWL_STYLE, lStyle);

	//设置list control的extend style
	DWORD dStyle = ExeLst.GetExtendedStyle();
	dStyle |= LVS_EX_FULLROWSELECT;
	dStyle |= LVS_EX_GRIDLINES;
	ExeLst.SetExtendedStyle(dStyle);

	//插入表头
	ExeLst.InsertColumn(0, L"名称", 0, 200);		//column:0	像素width:200
	ExeLst.InsertColumn(1, L"模块", 0, 400);

	return TRUE;
}

void CWndINJ::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, ExeLst);
	DDX_Check(pDX, IDC_CHECK1, B_INJECT);
	DDX_Check(pDX, IDC_CHECK2, B_PAUSE);
	DDX_Check(pDX, IDC_CHECK3, B_DEBUG);
}


BEGIN_MESSAGE_MAP(CWndINJ, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndINJ::OnBnClickedButton1)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CWndINJ::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// CWndINJ 消息处理程序


void CWndINJ::OnBnClickedButton1()
{

	wndAddGame.DoModal();



	// TODO: 在此添加控件通知处理程序代码
	//test Lst
	/*ExeLst.InsertItem(0, L"DNF");
	ExeLst.SetItemText(0, 1, L"dlls.dll");*/


	//STARTUPINFO si{};					//获取进程信息的结构体
	//si.cb = sizeof(si);					//必须要 no why
	//PROCESS_INFORMATION prinfo{};		//获取进程信息的结构体
	//
	//CreateProcess(						//固定格式
	//	L"D:\\文件合集\\game_hark\\JX2\\Sword2.exe",
	//	NULL,
	//	NULL, NULL, FALSE,
	//	CREATE_SUSPENDED,
	//	NULL,
	//	L"D:\\文件合集\\game_hark\\JX2\\",
	//	&si,
	//	&prinfo
	//);
	////此时进程暂停（管理员启动），在恢复之前做注入


	////恢复线程
	//ResumeThread(prinfo.hThread);

}


//void CWndINJ::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}
