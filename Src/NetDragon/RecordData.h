#pragma once


// CRecordData �Ի���

class CRecordData : public CDialog
{
	DECLARE_DYNAMIC(CRecordData)

public:
	CRecordData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRecordData();

// �Ի�������
	enum { IDD = IDD_SHOWDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
