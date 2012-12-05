#pragma once
#include "afxwin.h"


// ARPGuardPage 对话框
UINT ProtectMy(LPVOID lpParam);
class ARPGuardPage : public CPropertyPage
{
	DECLARE_DYNAMIC(ARPGuardPage)

public:
	ARPGuardPage();
	virtual ~ARPGuardPage();

// 对话框数据
	enum { IDD = IDD_ARPPROPPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_list;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedProtect();
public:
	afx_msg void OnBnClickedSee();
};
