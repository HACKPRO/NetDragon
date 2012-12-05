// RecordData.cpp : 实现文件
//

#include "stdafx.h"
#include "Net Dragon.h"
#include "RecordData.h"


// CRecordData 对话框

IMPLEMENT_DYNAMIC(CRecordData, CDialog)

CRecordData::CRecordData(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordData::IDD, pParent)
	, m_data(_T(""))
{

}

CRecordData::~CRecordData()
{
}

void CRecordData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITDATA, m_data);
}


BEGIN_MESSAGE_MAP(CRecordData, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CRecordData::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CRecordData::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDITDATA, &CRecordData::OnEnChangeEditdata)
END_MESSAGE_MAP()


// CRecordData 消息处理程序

void CRecordData::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_data.Empty();
}

void CRecordData::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CRecordData::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	OnOK();
}

void CRecordData::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	OnOK();
}

void CRecordData::OnEnChangeEditdata()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
