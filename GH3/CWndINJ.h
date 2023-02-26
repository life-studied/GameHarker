﻿#pragma once

#include "afxdialogex.h"


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
	//消息函数
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 程序List
	CListCtrl ExeLst;
	afx_msg void OnBnClickedButton1();
};