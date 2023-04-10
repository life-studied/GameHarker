// CWndMain.cpp: 实现文件
//

#include "pch.h"
#include "Dlls.h"
#include "afxdialogex.h"
#include "CWndMain.h"


// CWndMain 对话框

IMPLEMENT_DYNAMIC(CWndMain, CDialogEx)

CWndMain::CWndMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WNDMAIN, pParent), isAnly(false)
{

}

CWndMain::~CWndMain()
{
}

void CWndMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWndMain, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndMain::OnBnClickedButton1)
END_MESSAGE_MAP()


// CWndMain 消息处理程序


void CWndMain::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!isAnly)
	{
		Myinfo.GetInfo();
		Myinfo.HidInfo(L"Dlls.dll");
		isAnly = true;
	}
	
}
