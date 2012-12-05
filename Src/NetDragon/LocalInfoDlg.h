#pragma once


// LocalInfoDlg 对话框

// ARP条目信息
typedef struct tagARPInfo             
{
	char szIPAddrStr[16];			  // IP 
	char szHWAddrStr[18];             // MAC
	DWORD dwType;                     // 类型
}INFO_ARP, *PINFO_ARP;

class LocalInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(LocalInfoDlg)

public:
	LocalInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LocalInfoDlg();

// 对话框数据
	enum { IDD = IDD_LOCALINFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void AddARPInfoToList(CListCtrl& list);
public:
	virtual BOOL OnInitDialog();
public:
	CString m_localip;
public:
	CString m_localmac;
public:
	CString m_gatewayip;
public:
	CString m_netmask;
public:
	CString m_gatewaymac;
public:
	CString m_relatemac;
public:
	bool LoadLocalInfo(void);
protected:
	virtual void OnCancel();
public:
	afx_msg void OnStnClickedStaticLocalip();
};
