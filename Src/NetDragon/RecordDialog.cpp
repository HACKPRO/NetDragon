/************************************************************************/
/* 
	��������Ϣ		Code by Win.Hunter
*/
/************************************************************************/

#include "stdafx.h"
#include "Net Dragon.h"
#include "RecordDialog.h"


// CRecordDialog �Ի���

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


// CRecordDialog ��Ϣ�������

void CRecordDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CRecordDialog::OnBnClickedButtonclean()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (keepon)
	{
		this->GetDlgItem(IDC_BUTTONCLEAN)->SetWindowTextA("������ʾ");
	    keepon = false;
	}
	else
	{
		this->GetDlgItem(IDC_BUTTONCLEAN)->SetWindowTextA("ֹͣ��ʾ");
		keepon = true;
	}

	    
}
