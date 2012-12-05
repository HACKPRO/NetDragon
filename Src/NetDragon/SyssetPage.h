#pragma once
#include "afxwin.h"
#include "SetServerIpDlg.h"

class CSyssetPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSyssetPage)

// Construction
public:
	CSyssetPage();
	~CSyssetPage();

	void SetLspWorkMode(int nWorkMode);
	

// Dialog Data
	//{{AFX_DATA(CSyssetPage)
	enum { IDD = IDD_SYSSET };
	CButton	m_AutoStart;
	CSetServerIpDlg setServer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSyssetPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSyssetPage)
	afx_msg void OnInstall();
	afx_msg void OnRemove();
	afx_msg void OnAutostart();
	afx_msg void OnPassAll();
	afx_msg void OnQueryAll();
	afx_msg void OnDenyAll();
	virtual BOOL OnInitDialog();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedSetserver();
public:
	afx_msg void OnBnClickedIsserverset();
public:
	CButton m_setServer;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CBrush m_brush;
};
