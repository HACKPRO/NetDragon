#pragma once


// CSetServerIpDlg 对话框

class CSetServerIpDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetServerIpDlg)

public:
	CSetServerIpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetServerIpDlg();

// 对话框数据
	enum { IDD = IDD_SERVERIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
