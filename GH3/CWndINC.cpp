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
	InstallPage(&modAnly, IDD_MOD_ANLY, L"分析对比", TRUE);

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
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CWndINC::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON5, &CWndINC::OnBnClickedButton5)
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

void CWndINC::SetProcess(unsigned Pid, CString& wExe, bool bFirst)

{
	if (bFirst)
	{
		UExe = wExe;
		modPage[0].GetModList(Pid);
	}
	else
	{
		UExeRoot = wExe;
		modPage[1].GetModList(Pid);
	}
		
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


void CWndINC::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	//nType：对应不同的操作类型
	// 0：拖动边框
	// 1：最小化
	// 2：最大化
	if (nType == 1)	return;

	CRect rect;
	mTab.GetClientRect(&rect);		//获取mTab的rect信息

	rect.top += 1;							//微调
	rect.left += 1;
	rect.right += 1;
	rect.bottom -= 25;
	for (int i = 0; i < CurPage; i++)
	{
		if (Page[i]) Page[i]->MoveWindow(&rect);				//调整大小
	}
}


void CWndINC::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int n = mTab.GetCurSel();		//获取当前选项数字

	for (int i = 0; i < CurPage; i++)
	{
		Page[i]->ShowWindow(i == n);	//展示当前选项卡，隐藏其余选项卡
	}
}


void CWndINC::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	// 分析比对
	int CountNormal = modPage[0].LstModule.GetItemCount();
	int CountRoot = modPage[1].LstModule.GetItemCount();
	if ((CountNormal) && (CountRoot))		//不为空
	{
		modAnly.LstAnly.DeleteAllItems();
		for (int i = 0; i < CountNormal; i++)
		{
			CString  txt = modPage[0].LstModule.GetItemText(i, 3);		//获取文件位置的txt
			for (int j = 0; j < CountRoot; j++)		//循环比对
			{
				//标记：1正常 2减少 3增加
				if (modPage[1].LstModule.GetItemText(j, 3).MakeUpper() == txt.MakeUpper())		//找到了相同的模块
				{
					modPage[1].LstModule.SetItemData(j, 1);		//设置属性
					modPage[0].LstModule.SetItemData(i, 1);
					break;
				}
			}
		}

		CString wTxt[2]{ L"减少",L"正常" };
		DWORD dState[2]{ 0,2 };
		for (int i = 0; i < CountNormal; i++)
		{
			DWORD state = modPage[0].LstModule.GetItemData(i);
			modAnly.LstAnly.InsertItem(0, modPage[0].LstModule.GetItemText(i, 0));
			modAnly.LstAnly.SetItemText(0, 1, modPage[0].LstModule.GetItemText(i, 1));
			modAnly.LstAnly.SetItemText(0, 2, modPage[0].LstModule.GetItemText(i, 2));
			modAnly.LstAnly.SetItemText(0, 3, modPage[0].LstModule.GetItemText(i, 3));
			modAnly.LstAnly.SetItemText(0, 4, wTxt[state]);
			modAnly.LstAnly.SetItemData(0, dState[state]);
		}

		for (int i = 0; i < CountRoot; i++)
		{
			DWORD state = modPage[1].LstModule.GetItemData(i);
			if (state == 0)
			{
				modAnly.LstAnly.InsertItem(0, modPage[1].LstModule.GetItemText(i, 0));
				modAnly.LstAnly.SetItemText(0, 1, modPage[1].LstModule.GetItemText(i, 1));
				modAnly.LstAnly.SetItemText(0, 2, modPage[1].LstModule.GetItemText(i, 2));
				modAnly.LstAnly.SetItemText(0, 3, modPage[1].LstModule.GetItemText(i, 3));
				modAnly.LstAnly.SetItemText(0, 4, L"增加");
				modAnly.LstAnly.SetItemData(0, 1);
			}
		}
	}
}
