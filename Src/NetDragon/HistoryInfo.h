#pragma once
#include "afxwin.h"
#include <vector>
#include <iterator>
#include <fstream>
#include <string>

using std::ifstream;
using std::string;
// HistoryInfo �Ի���

class HistoryInfo : public CDialog
{
	DECLARE_DYNAMIC(HistoryInfo)

public:
	HistoryInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~HistoryInfo();

// �Ի�������
	enum { IDD = IDD_HISTORYDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_list;
protected:
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
private:
	std::vector<CString> arpcheater;          
    std::vector<CString>::iterator parpcheater;
	bool LoadInfo(void);
public:
	afx_msg void OnLbnSelchangeList2();
};
