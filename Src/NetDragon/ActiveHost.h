#pragma once
#include "afxwin.h"



class CActiveHost : public CDialog
{
	DECLARE_DYNAMIC(CActiveHost)

public:
	CActiveHost(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CActiveHost();

// �Ի�������
	enum { IDD = IDD_SCANDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
//	virtual void OnCancel();
public:
	CListCtrl m_Listctrl;
	CProgressCtrl m_progressScan;
public:
	afx_msg void OnBnClickedScan();
public:
	virtual BOOL OnInitDialog();
	void AddNewParse(char MACAddr[],char IPAddr[],char Name[]);
protected:
	virtual void OnCancel();
public:
	afx_msg void OnLvnItemchangedHostlist(NMHDR *pNMHDR, LRESULT *pResult);
};
