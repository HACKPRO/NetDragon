#pragma once


// CRecordDialog �Ի���

class CRecordDialog : public CDialog
{
	DECLARE_DYNAMIC(CRecordDialog)

public:
	CRecordDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRecordDialog();

// �Ի�������
	enum { IDD = IDD_DIALOGRECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_recstring;
	BOOL keepon;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedButtonclean();
};
