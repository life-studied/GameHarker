// CWndINJ.cpp: 实现文件
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
	ExeLst.InsertColumn(1, L"可执行文件", 0, 400);
	ExeLst.InsertColumn(2, L"文件夹", 0, 400);
	ExeLst.InsertColumn(3, L"命令行参数", 0, 400);
	ExeLst.InsertColumn(4, L"注入模块", 0, 400);

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
	wndAddGame.Init(this);
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

void CWndINJ::Init(CString& _AppPath)
{
	AppPath = _AppPath;
	GamesIni.Format(L"%s\\Set\\Games.ini",AppPath);

	//加载配置文件内容
	LoadGames();
}

void CWndINJ::AddGame(CString& _GameName, CString& _GameExe, CString& _GamePath, CString& _GameCmds, CString& _dlls)
{
	//AfxMessageBox(_GameName);
	//从配置文件中读取、
	int count = GetPrivateProfileIntW(
		L"main",		//索引
		L"count",		//对应的键
		0,				//读取失败的值
		GamesIni		//读取的文件名
	);
	

	CString key;
	key.Format(L"count_%d", count);

	//写入到配置文件中，与上述一致
	WritePrivateProfileStringW(key, L"GameName", _GameName, GamesIni);
	WritePrivateProfileStringW(key, L"GameExe", _GameExe, GamesIni);
	WritePrivateProfileStringW(key, L"GamePath", _GamePath, GamesIni);
	WritePrivateProfileStringW(key, L"GameCmds", _GameCmds, GamesIni);
	WritePrivateProfileStringW(key, L"GameDlls", _dlls, GamesIni);

	count++;

	CString wCount;
	wCount.Format(L"%d", count);
	WritePrivateProfileStringW(L"main", L"count", wCount, GamesIni);

	int icount = ExeLst.GetItemCount();
	ExeLst.InsertItem(icount, _GameName);			//申请一行并插入第0列
	ExeLst.SetItemText(icount, 1, _GameExe);	//插入第i行的第1列
	ExeLst.SetItemText(icount, 2, _GamePath);
	ExeLst.SetItemText(icount, 3, _GameCmds);
	ExeLst.SetItemText(icount, 4, _dlls);
}

void CWndINJ::LoadGames()
{
	//读取配置数目
	int count = GetPrivateProfileIntW(
		L"main",		//索引
		L"count",		//对应的键
		0,				//读取失败的值
		GamesIni		//读取的文件名
	);

	for (int i = 0; i < count; i++)
	{
		CString GameName, GameExe, GamePath, GameCmds, GameDlls, _AppName;
		_AppName.Format(L"count_%d", i);
			
		wchar_t wRead[0xff];		//重点
		GetPrivateProfileStringW(
			_AppName,		//索引
			L"GameName",	//键
			L"",			//失败的默认值
			wRead,	//返回到的缓冲区，不要用CString的GameName，有可能出错
			0xff,				//缓冲区的最大值
			GamesIni);		//配置文件名和路径
		GameName.Format(L"%s", wRead);	//复制进来

		GetPrivateProfileStringW(
			_AppName,		//索引
			L"GamePath",	//键
			L"",			//失败的默认值
			wRead,	//返回到的缓冲区，不要用CString的GameName，有可能出错
			0xff,				//缓冲区的最大值
			GamesIni);		//配置文件名和路径
		GamePath.Format(L"%s", wRead);	//复制进来

		GetPrivateProfileStringW(
			_AppName,		//索引
			L"GameCmds",	//键
			L"",			//失败的默认值
			wRead,	//返回到的缓冲区，不要用CString的GameName，有可能出错
			0xff,				//缓冲区的最大值
			GamesIni);		//配置文件名和路径
		GameCmds.Format(L"%s", wRead);	//复制进来

		GetPrivateProfileStringW(
			_AppName,		//索引
			L"GameExe",	//键
			L"",			//失败的默认值
			wRead,	//返回到的缓冲区，不要用CString的GameName，有可能出错
			0xff,				//缓冲区的最大值
			GamesIni);		//配置文件名和路径
		GameExe.Format(L"%s", wRead);	//复制进来

		GetPrivateProfileStringW(
			_AppName,		//索引
			L"GameDlls",	//键
			L"",			//失败的默认值
			wRead,	//返回到的缓冲区，不要用CString的GameName，有可能出错
			0xff,				//缓冲区的最大值
			GamesIni);		//配置文件名和路径
		GameDlls.Format(L"%s", wRead);	//复制进来


		ExeLst.InsertItem(i, GameName);			//申请一行并插入第0列
		ExeLst.SetItemText(i, 1, GameExe);	//插入第i行的第1列
		ExeLst.SetItemText(i, 2, GamePath);
		ExeLst.SetItemText(i, 3, GameCmds);
		ExeLst.SetItemText(i, 4, GameDlls);
	}
		
}


//void CWndINJ::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}
