
// GH3Dlg.h: 头文件
//

#pragma once
#include "CWndINC.h"
#include "CWndINJ.h"
#define MAX_PAGES 10
// CGH3Dlg 对话框
class CGH3Dlg : public CDialogEx
{
// 构造
public:
	CGH3Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GH3_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

		
public:
	CTabCtrl mTab;
	CWndINC PC;
	CWndINJ PJ;

	int CurPage = 0;
	CDialogEx* Page[MAX_PAGES];
	bool InstallPage(CDialogEx* wnd, int IDD_Wnd, CString&& _Name, bool isShow = false);
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnBnClickedOk();
	CString wAppPath;
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
