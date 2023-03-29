#pragma once
#include "afxdialogex.h"


// CWndAddGame 对话框

class CWndAddGame : public CDialogEx
{
	DECLARE_DYNAMIC(CWndAddGame)

public:
	CWndAddGame(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndAddGame();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDGAME};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString GameName;
	CString GamePath;
	CString GameFullPath;
	CString GameCmds;
	CString DllPath;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();

public:
	void* father;
	bool isCreate;
	int index;
	void Init(void* _father, bool Create = true,int _index = 0);
	void SetData(
		CString& _GameName,
		CString& _GamePath,
		CString& _GameExe,
		CString& _GameCmds,
		CString& _DllPath);
	afx_msg void OnBnClickedButton3();
};
