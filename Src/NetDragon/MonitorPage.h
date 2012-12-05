
#pragma once
#include "afxwin.h"
#include "TypeStruct.h"

class CMonitorPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CMonitorPage)

// Construction
public:
	CMonitorPage();
	~CMonitorPage();

	BOOL m_bStop;

	static void HandleNotifySession(SESSION *pSession, int nCode);



	void AddASession(SESSION *pSession);
	void DeleteASession(SESSION *pSession, BOOL bAppExit);

	HTREEITEM FindAppItem(CHAR *pszPathName);
	HTREEITEM FindSessionItem(HTREEITEM hAppItem, SESSION *pSession);

	CString BuildSessionText(SESSION *pSession);

// Dialog Data
	//{{AFX_DATA(CMonitorPage)
	enum { IDD = IDD_MONITOR };
	CTreeCtrl	m_MonitorTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMonitorPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMonitorPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTvnSelchangedTreemonitor(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.