#pragma once
#include "afxdialogex.h"
#include "CWndAddGame.h"
#include "INJECT.h"
// CWndINJ 对话框

class CWndINJ : public CDialogEx
{
	DECLARE_DYNAMIC(CWndINJ)

public:
	CWndINJ(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndINJ();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_0 };
#endif

protected:
	INJECT m_INJECT;
	//消息函数
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 程序List
	CListCtrl ExeLst;
	afx_msg void OnBnClickedButton1();
	// 注入
// 注入
	BOOL B_INJECT;
	// 暂停
	BOOL B_PAUSE;
	// 调试
	BOOL B_DEBUG;

	//
	CWndAddGame wndAddGame;
	CString AppPath;
	CString GamesIni;
	void Init(CString& _AppPath);
	void AddGame(
		CString& _GameName,
		CString& _GameExe,
		CString& _GamePath,
		CString& _GameCmds,
		CString& _dlls);

	void LoadGames();		//从配置文件加载游戏到列表
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuSet();
	afx_msg void OnMenuDel();
};
