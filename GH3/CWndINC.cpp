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

	//mTab.InsertItem(0, L"正常进程");
	//mTab.InsertItem(1, L"感染进程");
	//mTab.InsertItem(2, L"分析结果");

	InstallPage(&modPage[0], IDD_MOD_LIST, L"正常进程", TRUE);
	InstallPage(&modPage[1], IDD_MOD_LIST, L"感染进程", TRUE);
	return TRUE;
}

void CWndINC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, UExe);
	DDX_Text(pDX, IDC_EDIT2, UExeRoot);
	DDX_Control(pDX, IDC_TAB1, mTab);
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

bool CWndINC::InstallPage(CDialogEx* wnd, int IDD_Wnd, CString&& _Name, bool isShow)
{
	if (CurPage >= MAX_PAGES) return false;
	mTab.InsertItem(CurPage, _Name);	//在选项卡中插入对应的标签和位置	
	
	Page[CurPage] = wnd;
	Page[CurPage]->Create(IDD_Wnd,this);		//变量绑定控件
	Page[CurPage]->SetParent(&mTab);		//设置当前对话框为父类
	
	Page[CurPage]->ShowWindow(isShow);		//展示控件的对话框

	//此时出现问题：出现的对话框窗体与底层的窗体不匹配
	//解决：获取底层窗体的大小，并赋值给上层的对话框

	CRect rect;
	mTab.GetClientRect(&rect);		//获取mTab的rect信息

	rect.top += 1;							//微调
	rect.left += 1;
	rect.right += 1;
	rect.bottom += 1;

	Page[CurPage]->MoveWindow(&rect);				//赋值
	

	CurPage++;
	return true;
}
