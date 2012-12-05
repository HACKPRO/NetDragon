#pragma once
#include "afxwin.h"

// CNoticeDlg 对话框


BOOL StaticARP(unsigned char szIPAddr[], BYTE bPhysAddr[]);
class CNoticeDlg : public CDialog
{
	DECLARE_DYNAMIC(CNoticeDlg)

public:
	CNoticeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNoticeDlg();

// 对话框数据
	enum { IDD = IDD_NOTICEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
