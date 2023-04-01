// CWndModuleList.cpp: 实现文件
//

#include "pch.h"
#include "GH3.h"
#include "afxdialogex.h"
#include "CWndModuleList.h"


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


BEGIN_MESSAGE_MAP(CWndModuleList, CDialogEx)
END_MESSAGE_MAP()


// CWndModuleList 消息处理程序
