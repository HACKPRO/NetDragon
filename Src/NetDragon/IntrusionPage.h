#pragma once
#include "afxcmn.h"


// CIntrusionPage �Ի���


#define WM_MY_MESSAGE_INTRUSION (WM_USER+107)

class CIntrusionPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CIntrusionPage)

public:
	CIntrusionPage();
	virtual ~CIntrusionPage();

// �Ի�������
	enum { IDD = IDD_INTRUSION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartdetect();
	afx_msg LRESULT  OnIntrusion(WPARAM wParam, LPARAM lParam);
public:
	CListCtrl m_list_intrusion;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedStop();
};
