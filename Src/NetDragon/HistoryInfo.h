#pragma once
#include "afxwin.h"
#include <vector>
#include <iterator>
#include <fstream>
#include <string>

using std::ifstream;
using std::string;
// HistoryInfo 对话框

class HistoryInfo : public CDialog
{
	DECLARE_DYNAMIC(HistoryInfo)

public:
	HistoryInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~HistoryInfo();

// 对话框数据
	enum { IDD = IDD_HISTORYDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
