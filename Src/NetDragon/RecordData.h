#pragma once


// CRecordData 对话框

class CRecordData : public CDialog
{
	DECLARE_DYNAMIC(CRecordData)

public:
	CRecordData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRecordData();

// 对话框数据
	enum { IDD = IDD_SHOWDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_data;
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnBnClickedOk();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnClose();
	afx_msg void OnEnChangeEditdata();
};
