#pragma once
#include "afxwin.h"

// CNoticeDlg �Ի���


BOOL StaticARP(unsigned char szIPAddr[], BYTE bPhysAddr[]);
class CNoticeDlg : public CDialog
{
	DECLARE_DYNAMIC(CNoticeDlg)

public:
	CNoticeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNoticeDlg();

// �Ի�������
	enum { IDD = IDD_NOTICEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_notice;
public:
	CComboBox m_adapter;
public:
	void SetAdapter(void);
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnCbnSelchangeAdapter();
protected:
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOk();


public:
	afx_msg void OnBnClickedRadiostatic();
public:
	afx_msg void OnBnClickedButtoncancelstatic();

public:
	CString m_staticmac;
	afx_msg void OnEnChangeEditnotice();
};
