// CWndAddGame.cpp: 实现文件
//

#include "pch.h"
#include "GH3.h"
#include "CWndINJ.h"
#include "afxdialogex.h"
#include "CWndAddGame.h"


// CWndAddGame 对话框

IMPLEMENT_DYNAMIC(CWndAddGame, CDialogEx)

CWndAddGame::CWndAddGame(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADDGAME, pParent)
	, GameName(_T(""))
	, GamePath(_T(""))
	, GameFullPath(_T(""))
	, GameCmds(_T(""))
	, DllPath(_T(""))
{

}

CWndAddGame::~CWndAddGame()
{
}

void CWndAddGame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, GameName);
	DDX_Text(pDX, IDC_EDIT2, GamePath);
	DDX_Text(pDX, IDC_EDIT3, GameFullPath);
	DDX_Text(pDX, IDC_EDIT4, GameCmds);
	DDX_Text(pDX, IDC_EDIT5, DllPath);
}


BEGIN_MESSAGE_MAP(CWndAddGame, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndAddGame::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWndAddGame::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CWndAddGame::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CWndAddGame::OnBnClickedButton3)
END_MESSAGE_MAP()


// CWndAddGame 消息处理程序


void CWndAddGame::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	TCHAR szFilters[] = _T("exe (*.exe)|*.exe|dat (*.dat)|*.dat|All Files (*.*)|*.*||");
	CFileDialog dialog(
		TRUE,		//TRUE为打开，FALSE为另存为
		NULL,		//文件扩展名
		NULL,		//文件名
		6,			//不用管，就是6
		szFilters	//文件格式，事先创建好的格式数组
	);		
	if (dialog.DoModal() == IDOK)	//打开并且选择了
	{
		GameFullPath = dialog.GetPathName();		//获取文件路径，包含文件名
		GameName = dialog.GetFileTitle();
		GamePath = dialog.GetFolderPath() + L"\\";
		GameCmds = L"";
		UpdateData(FALSE);		//获取完再更新到编辑框里
	}
}


void CWndAddGame::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	TCHAR szFilters[] = _T("动态链接库 (*.dll)|*.dll|All Files (*.*)|*.*||");
	CFileDialog dialog(
		TRUE,		//TRUE为打开，FALSE为另存为
		NULL,		//文件扩展名
		NULL,		//文件名
		6,			//不用管，就是6
		szFilters	//文件格式，事先创建好的格式数组
	);
	if (dialog.DoModal() == IDOK)	//打开并且选择了
	{
		DllPath = dialog.GetPathName();
		UpdateData(FALSE);
	}
}


void CWndAddGame::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

void CWndAddGame::Init(void* _father,bool Create,int _index)
{
	father = _father;
	isCreate = Create;
	index = _index;
}

void CWndAddGame::SetData(CString& _GameName, CString& _GamePath, CString& _GameExe, CString& _GameCmds, CString& _DllPath)
{
	GameName = _GameName;
	GameFullPath = _GameExe;
	GamePath = _GamePath;
	GameCmds = _GameCmds;
	DllPath = _DllPath;

}


void CWndAddGame::OnBnClickedButton3()
{
	//保存数据
	CWndINJ* p = (CWndINJ*)father;
	UpdateData(TRUE);
	if (isCreate)
	{
		p->AddGame(GameName, GameFullPath, GamePath, GameCmds, DllPath);
		
	}
	else
	{
		p->SaveGame(GameName, GameFullPath, GamePath, GameCmds, DllPath, index);   
	}

	CDialog::OnCancel();
}
