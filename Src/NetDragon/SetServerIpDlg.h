#pragma once


// CSetServerIpDlg �Ի���

class CSetServerIpDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetServerIpDlg)

public:
	CSetServerIpDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetServerIpDlg();

// �Ի�������
	enum { IDD = IDD_SERVERIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString serverIp;
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnEnChangeServerip();
};
