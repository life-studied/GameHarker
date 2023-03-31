#pragma once
#include "afxdialogex.h"


// CWndProcess 对话框

class CWndProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CWndProcess)

public:
	CWndProcess(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndProcess();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_LST };
#endif

protected:
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl ProcessLst;
	void RefreshProcess();
	afx_msg void OnNMDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	void Init(LPVOID lPtr, BOOL _First = TRUE);
	BOOL IsFirst;

};
