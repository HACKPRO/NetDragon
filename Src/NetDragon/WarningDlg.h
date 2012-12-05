#pragma once
#include "afxcmn.h"


// WarningDlg 对话框

class WarningDlg : public CDialog
{
	DECLARE_DYNAMIC(WarningDlg)

public:
	WarningDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~WarningDlg();

// 对话框数据
	enum { IDD = IDD_WARNINGDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
public:
	unsigned char CompareMac[6];
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedButtontrace();
public:
	CProgressCtrl m_progressctrl;
public:
	CString IPResult;
};
