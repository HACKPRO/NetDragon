// SetServerIpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Net Dragon.h"
#include "SetServerIpDlg.h"
#include "Sender.h"
#include "SyssetPage.h"
// CSetServerIpDlg �Ի���
extern Sender sender;
extern CSyssetPage *sysDlg;
IMPLEMENT_DYNAMIC(CSetServerIpDlg, CDialog)

CSetServerIpDlg::CSetServerIpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetServerIpDlg::IDD, pParent)
	, serverIp(_T(""))
{

}

CSetServerIpDlg::~CSetServerIpDlg()
{
}

void CSetServerIpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SERVERIP, serverIp);
	DDV_MaxChars(pDX, serverIp, 15);
}


BEGIN_MESSAGE_MAP(CSetServerIpDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSetServerIpDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetServerIpDlg ��Ϣ�������

void CSetServerIpDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	strcpy(g_RuleFile.m_header.szServerIp, serverIp);
	sender.SetServer(serverIp);
	g_RuleFile.m_header.bisServerAllowed = true;
	isServerAllowed = true;
	
	((CButton*)(sysDlg->GetDlgItem(IDC_ISSERVERSET)))->SetCheck(1);
	AfxMessageBox("������������!");
	sysDlg->GetOwner()->GetOwner()->GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
	DestroyWindow();
}

BOOL CSetServerIpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    serverIp = g_RuleFile.m_header.szServerIp;
	UpdateData(false);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSetServerIpDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	DestroyWindow();
}
