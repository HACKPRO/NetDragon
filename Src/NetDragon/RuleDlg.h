#pragma once
#include "afxwin.h"


class CRuleDlg : public CDialog
{
public:
	static RULE_ITEM m_RuleItem;	// 要添加的规则
	static BOOL		 m_bAppQuery;	// 是不是来自DLL模块的询问
	static CString   m_sPathName;	// 如果m_bAppQuery为TRUE，此变量包含了
									// 发出询问的应用程序的名称

// Construction
public:
	CRuleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRuleDlg)
	enum { IDD = IDD_RULE };
	CComboBox	m_ComboType;
	CStatic	m_RuleTitle;
	CEdit	m_EditPort;
	CEdit	m_EditMemo;
	CComboBox	m_ComboDirection;
	CComboBox	m_ComboAction;
	CComboBox	m_ComboApp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRuleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRuleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeType();
	virtual void OnOK();
	afx_msg void OnSelchangeApplication();
	afx_msg void OnAppBrowser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeDirection();
public:
	afx_msg void OnBnClickedOk();
};