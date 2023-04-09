#pragma once
#include "afxdialogex.h"


// CWndModuleList 对话框

class CWndModuleList : public CDialogEx
{
	DECLARE_DYNAMIC(CWndModuleList)

public:
	CWndModuleList(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndModuleList();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOD_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl LstModule;

	void GetModList(DWORD Pid);
};
