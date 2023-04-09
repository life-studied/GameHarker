#pragma once
#include "afxdialogex.h"


// CWndModAnly 对话框

class CWndModAnly : public CDialogEx
{
	DECLARE_DYNAMIC(CWndModAnly)

public:
	CWndModAnly(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndModAnly();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOD_ANLY };
#endif

protected:
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl LstAnly;
	afx_msg void OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult);
};
