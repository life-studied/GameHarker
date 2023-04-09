// CWndModAnly.cpp: 实现文件
//

#include "pch.h"
#include "GH3.h"
#include "afxdialogex.h"
#include "CWndModAnly.h"


// CWndModAnly 对话框

IMPLEMENT_DYNAMIC(CWndModAnly, CDialogEx)

CWndModAnly::CWndModAnly(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOD_ANLY, pParent)
{

}

CWndModAnly::~CWndModAnly()
{
}

BOOL CWndModAnly::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(LstAnly.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT;
	SetWindowLongPtr(LstAnly.m_hWnd, GWL_STYLE, lStyle);

	//设置list control的extend style
	DWORD dStyle = LstAnly.GetExtendedStyle();
	dStyle |= LVS_EX_FULLROWSELECT;
	dStyle |= LVS_EX_GRIDLINES;
	LstAnly.SetExtendedStyle(dStyle);

	//插入表头
	LstAnly.InsertColumn(0, L"模块名称", 0, 100);		//column:0	像素width:200
	LstAnly.InsertColumn(1, L"基址", 0, 200);
	LstAnly.InsertColumn(2, L"大小", 0, 200);
	LstAnly.InsertColumn(3, L"文件位置", 0, 200);
	LstAnly.InsertColumn(4, L"状态", 0, 200);

	return TRUE;
}

void CWndModAnly::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, LstAnly);
}


BEGIN_MESSAGE_MAP(CWndModAnly, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CWndModAnly::OnNMCustomdrawList1)
END_MESSAGE_MAP()


// CWndModAnly 消息处理程序


void CWndModAnly::OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NMLVCUSTOMDRAW* lp = (NMLVCUSTOMDRAW*)pNMHDR;
	auto& cmd = lp->nmcd;		//获取消息，进行解读
	switch (cmd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		DWORD dIndex = LstAnly.GetItemData(cmd.dwItemSpec);
		COLORREF color[3]{ RGB(0,0,255),RGB(255,0,0),RGB(0,0,0) };
		lp->clrText = color[dIndex];
		
		*pResult = CDRF_DODEFAULT;
		break;
	}

}
