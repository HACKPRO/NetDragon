#pragma once
#include "afxcmn.h"


// WarningDlg �Ի���

class WarningDlg : public CDialog
{
	DECLARE_DYNAMIC(WarningDlg)

public:
	WarningDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~WarningDlg();

// �Ի�������
	enum { IDD = IDD_WARNINGDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
