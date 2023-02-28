
// GH3Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GH3.h"
#include "GH3Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGH3Dlg 对话框



CGH3Dlg::CGH3Dlg(CWnd* pParent /*=nullptr*/) 
	: CDialogEx(IDD_GH3_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGH3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, mTab);
}

BEGIN_MESSAGE_MAP(CGH3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CGH3Dlg::OnTcnSelchangeTab1)
//	ON_BN_CLICKED(IDOK, &CGH3Dlg::OnBnClickedOk)
ON_WM_SIZE()
END_MESSAGE_MAP()


// CGH3Dlg 消息处理程序

BOOL CGH3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	TCHAR FileModule[0x100]{};
	//获取当前exe路径
	GetModuleFileName(
		NULL,		//设置为NULL获取的是exe路径，否则获取的是模块路径
		FileModule,
		0x100
	);		
	int i;
	for (i = 0x100 - 1; FileModule[i] != L'\\'; i--);

	FileModule[i] = 0;
	wAppPath.Format(L"%s", FileModule);
	//AfxMessageBox(FileModule);
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InstallPage(&PJ, IDD_PAGE_0, L"注入", true);
	InstallPage(&PC, IDD_PAGE_1, L"注入检测");


	//初始化
	PJ.Init(wAppPath);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


bool CGH3Dlg::InstallPage(CDialogEx* wnd, int IDD_Wnd, CString&& _Name, bool isShow)
{
	if (CurPage >= MAX_PAGES) return false;
	
	mTab.InsertItem(CurPage, _Name);	//在选项卡中插入对应的标签和位置	
	Page[CurPage] = wnd;
	Page[CurPage]->Create(IDD_Wnd);		//变量绑定控件
	Page[CurPage]->SetParent(this);		//设置当前对话框为父类
	Page[CurPage]->ShowWindow(isShow);		//展示控件的对话框


	//此时出现问题：出现的对话框窗体与底层的窗体不匹配
	//解决：获取底层窗体的大小，并赋值给上层的对话框
	
	CRect rect;
	mTab.GetClientRect(&rect);		//获取mTab的rect信息

	rect.top += 45;							//微调
	rect.left += 20;
	rect.right += 5;
	rect.bottom += 5;

	Page[CurPage]->MoveWindow(&rect);				//赋值


	CurPage++;
	return true;
}

void CGH3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGH3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGH3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGH3Dlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int n = mTab.GetCurSel();		//获取当前选项数字

	for (int i = 0; i < CurPage; i++)
	{
		Page[i]->ShowWindow(i == n);	//展示当前选项卡，隐藏其余选项卡
	}
}


//void CGH3Dlg::OnBnClickedOk()
//{
	// TODO: 在此添加控件通知处理程序代码
//	CDialogEx::OnOK();
//}


void CGH3Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	//nType：对应不同的操作类型
	// 0：拖动边框
	// 1：最小化
	// 2：最大化
	if (nType == 1)	return;
	// TODO: 在此处添加消息处理程序代码

	CRect rect;
	mTab.GetClientRect(&rect);		//获取mTab的rect信息

	rect.top += 45;							//微调
	rect.left += 20;
	rect.right += 5;
	rect.bottom += 5;
	for (int i = 0; i < CurPage; i++)
	{
		if(Page[i]) Page[i]->MoveWindow(&rect);				//调整大小
	}
	
}
