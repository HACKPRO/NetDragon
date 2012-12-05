/************************************************************************/
/* 
	检测可以信息		Code by Win.Hunter
*/
/************************************************************************/

#include "stdafx.h"
#include "Net Dragon.h"
#include "RecordDialog.h"


// CRecordDialog 对话框

IMPLEMENT_DYNAMIC(CRecordDialog, CDialog)

CRecordDialog::CRecordDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordDialog::IDD, pParent)
	, m_recstring(_T(""))
{
	keepon = true;

}

CRecordDialog::~CRecordDialog()
{
}

void CRecordDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITREC, m_recstring);
}


BEGIN_MESSAGE_MAP(CRecordDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CRecordDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTONCLEAN, &CRecordDialog::OnBnClickedButtonclean)
END_MESSAGE_MAP()


// CRecordDialog 消息处理程序

void CRecordDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CRecordDialog::OnBnClickedButtonclean()
{
	// TODO: 在此添加控件通知处理程序代码
	if (keepon)
	{
		this->GetDlgItem(IDC_BUTTONCLEAN)->SetWindowTextA("继续显示");
	    keepon = false;
	}
	else
	{
		this->GetDlgItem(IDC_BUTTONCLEAN)->SetWindowTextA("停止显示");
		keepon = true;
	}

	    
}
