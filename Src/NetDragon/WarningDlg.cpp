/************************************************************************/
/* 
	
*/
/************************************************************************/

#include "stdafx.h"
#include "Net Dragon.h"
#include "WarningDlg.h"


// WarningDlg 对话框

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


// WarningDlg 消息处理程序

void WarningDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	DestroyWindow();
}

BOOL WarningDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	memset(CompareMac,0xff,6);
	memcpy(CompareMac,CompareMacglobal,6);
    char macTemp[40];
	// TODO:  在此添加额外的初始化
	sprintf(macTemp,"%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",CompareMac[0],CompareMac[1],CompareMac[2],CompareMac[3],CompareMac[4],CompareMac[5]);
	SetDlgItemText(IDC_STATICMAC,macTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void WarningDlg::OnBnClickedButtontrace()
{
	// TODO: 在此添加控件通知处理程序代码
	m_thread.pDlgw=this;
	m_thread.IsUseForScan = false;
	memset(m_thread.ComPareMac,0xff,6);
	memcpy(m_thread.ComPareMac,CompareMac,6);
	uHostNum = ~ htonl(inet_addr(m_thread.Submask)) - 1;
	if (uHostNum > 2000)//防止掩码设为255.0.0.0之类的
		uHostNum = 2000;
	//进度条初始化
	m_progressctrl.SetRange32(0,uHostNum);
	m_progressctrl.SetStep(1);
	AfxBeginThread((AFX_THREADPROC)EnumLanHost,&m_thread);

}
