/************************************************************************/
/* 
	
*/
/************************************************************************/

#include "stdafx.h"
#include "Net Dragon.h"
#include "WarningDlg.h"


// WarningDlg �Ի���

IMPLEMENT_DYNAMIC(WarningDlg, CDialog)

WarningDlg::WarningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(WarningDlg::IDD, pParent)
	, IPResult(_T(""))
{

}

WarningDlg::~WarningDlg()
{
}

void WarningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESSTRACE, m_progressctrl);
}


BEGIN_MESSAGE_MAP(WarningDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTONTRACE, &WarningDlg::OnBnClickedButtontrace)
END_MESSAGE_MAP()


// WarningDlg ��Ϣ�������

void WarningDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	DestroyWindow();
}

BOOL WarningDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	memset(CompareMac,0xff,6);
	memcpy(CompareMac,CompareMacglobal,6);
    char macTemp[40];
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	sprintf(macTemp,"%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",CompareMac[0],CompareMac[1],CompareMac[2],CompareMac[3],CompareMac[4],CompareMac[5]);
	SetDlgItemText(IDC_STATICMAC,macTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void WarningDlg::OnBnClickedButtontrace()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_thread.pDlgw=this;
	m_thread.IsUseForScan = false;
	memset(m_thread.ComPareMac,0xff,6);
	memcpy(m_thread.ComPareMac,CompareMac,6);
	uHostNum = ~ htonl(inet_addr(m_thread.Submask)) - 1;
	if (uHostNum > 2000)//��ֹ������Ϊ255.0.0.0֮���
		uHostNum = 2000;
	//��������ʼ��
	m_progressctrl.SetRange32(0,uHostNum);
	m_progressctrl.SetStep(1);
	AfxBeginThread((AFX_THREADPROC)EnumLanHost,&m_thread);

}
