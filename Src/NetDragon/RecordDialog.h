#pragma once


// CRecordDialog 对话框

class CRecordDialog : public CDialog
{
	DECLARE_DYNAMIC(CRecordDialog)

public:
	CRecordDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRecordDialog();

// 对话框数据
	enum { IDD = IDD_DIALOGRECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_recstring;
	BOOL keepon;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedButtonclean();
};
