// CWndINC.cpp: 实现文件
//

#include "pch.h"
#include "GH3.h"
#include "afxdialogex.h"
#include "CWndINC.h"


// CWndINC 对话框

IMPLEMENT_DYNAMIC(CWndINC, CDialogEx)

CWndINC::CWndINC(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_1, pParent)
	, UExe(_T(""))
	, UExeRoot(_T(""))
{

}

CWndINC::~CWndINC()
{
}

BOOL CWndINC::OnInitDialog()
{
	CDialogEx::OnInitDialog();		
	return TRUE;
}

void CWndINC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, UExe);
	DDX_Text(pDX, IDC_EDIT2, UExeRoot);
}


BEGIN_MESSAGE_MAP(CWndINC, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndINC::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWndINC::OnBnClickedButton2)
END_MESSAGE_MAP()


// CWndINC 消息处理程序


void CWndINC::OnBnClickedButton1()
{
	wndProcess.Init(this);
	wndProcess.DoModal();
	// TODO: 在此添加控件通知处理程序代码
}


void CWndINC::OnBnClickedButton2()
{
	wndProcess.Init(this, FALSE);
	wndProcess.DoModal();
	// TODO: 在此添加控件通知处理程序代码
}

void CWndINC::SetProcess(unsigned Pid, CString& wExe, BOOL bFirst)
{
	if (bFirst)
		UExe = wExe;
	else
		UExeRoot = wExe;

	UpdateData(FALSE);
}
