#pragma once
#include "afxwin.h"
#include "TypeStruct.h"
/////////////////////////////////////////////////////////////////////////////
// CRulePage dialog

class CRulePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRulePage)

// Construction
public:
	CRulePage();
	~CRulePage();
	// 编辑一个规则，如果nEditIndex == -1，则添加一个新的规则
	void EditARule(RULE_ITEM *pItem, int nEditIndex = -1);

	void UpdateList();

	int InitAddRule(LPCSTR pszQueryApp = NULL);

	static BOOL AddQueryRule(LPCSTR pszQueryApp);

// Dialog Data
	//{{AFX_DATA(CRulePage)
	enum { IDD = IDD_RULE_CONTROL };
	CListCtrl	m_rules;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRulePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	int m_nListIndex;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRulePage)
	afx_msg void OnAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnDel();
	afx_msg void OnClickRules(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEdit();
	afx_msg void OnItemchangedRules(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkRules(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
