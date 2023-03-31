// CWndProcess.cpp: 实现文件
//

#include "pch.h"
#include "GH3.h"
#include "afxdialogex.h"
#include "CWndProcess.h"
#include <TlHelp32.h>
#include "CWndINC.h"

CWndINC* wndINC;


// CWndProcess 对话框

IMPLEMENT_DYNAMIC(CWndProcess, CDialogEx)

CWndProcess::CWndProcess(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESS_LST, pParent)
{

}

CWndProcess::~CWndProcess()
{
}

BOOL CWndProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(ProcessLst.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT;
	SetWindowLongPtr(ProcessLst.m_hWnd, GWL_STYLE, lStyle);

	//设置list control的extend style
	DWORD dStyle = ProcessLst.GetExtendedStyle();
	dStyle |= LVS_EX_FULLROWSELECT;
	dStyle |= LVS_EX_GRIDLINES;
	ProcessLst.SetExtendedStyle(dStyle);

	//插入表头
	ProcessLst.InsertColumn(0, L"PID", 0, 200);		//column:0	像素width:200
	ProcessLst.InsertColumn(1, L"可执行文件名", 0, 400);

	RefreshProcess();
	return TRUE;
}

void CWndProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, ProcessLst);
}

void CWndProcess::RefreshProcess()
{
	ProcessLst.DeleteAllItems();

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);	//获取一个进程快照
	if (INVALID_HANDLE_VALUE == hSnap)	//判断函数是否失败
	{
		AfxMessageBox(L"获取进程列表失败！检查是否拥有管理员权限！");
		return;
	}
	PROCESSENTRY32 pe{ sizeof(pe) };		//结构体
	BOOL BMORE = Process32First(hSnap, &pe);	//获取进程快照的第一个进程，进程信息存储在pe中
	while (BMORE)			//循环获取下一个进程，插入到列表中
	{
		CString Txt;
		Txt.Format(L"%d", pe.th32ProcessID);
		ProcessLst.InsertItem(0, Txt);
		ProcessLst.SetItemText(0, 1, pe.szExeFile);

		BMORE = Process32Next(hSnap, &pe);
	}

	CloseHandle(hSnap);
}


BEGIN_MESSAGE_MAP(CWndProcess, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CWndProcess::OnNMDblclkList2)
END_MESSAGE_MAP()


// CWndProcess 消息处理程序


void CWndProcess::OnNMDblclkList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NM_LISTVIEW* View = (NM_LISTVIEW*)pNMHDR;
	if (View->iItem != -1)
	{
		CString wPid = ProcessLst.GetItemText(View->iItem,0);
		CString wExe = ProcessLst.GetItemText(View->iItem,1);
		DWORD dPid = wcstoul(wPid.GetBuffer(), 0, 10);

		wndINC->SetProcess(dPid, wExe, IsFirst);
	}

	OnCancel();
}

void CWndProcess::Init(LPVOID lPtr, BOOL _First)
{
	wndINC = (CWndINC*)lPtr;
	IsFirst = _First;
}
