#pragma once
#include "afxdialogex.h"
#include "CWndProcess.h"
#include "CWndModuleList.h"
#include "CWndModAnly.h"
#define MAX_PAGES 3
// CWndINC 对话框

class CWndINC : public CDialogEx
{
	DECLARE_DYNAMIC(CWndINC)

public:
	CWndINC(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndINC();

	CWndProcess wndProcess;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_1 };
#endif

protected:
	//消息函数
	virtual BOOL OnInitDialog();


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	void SetProcess(unsigned Pid,CString& wExe, bool bFirst = false);
	CString UExe;
	CString UExeRoot;
	CTabCtrl mTab;
	
	int CurPage{};
	CWndModuleList modPage[2];
	CWndModAnly modAnly;
	CDialogEx* Page[MAX_PAGES];
	bool InstallPage(CDialogEx* wnd, int IDD_Wnd, CString&& _Name, bool isShow);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton5();
};
