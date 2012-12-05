// SetServerIpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Net Dragon.h"
#include "SetServerIpDlg.h"
#include "Sender.h"
#include "SyssetPage.h"
// CSetServerIpDlg 对话框
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


// CSetServerIpDlg 消息处理程序

void CSetServerIpDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	strcpy(g_RuleFile.m_header.szServerIp, serverIp);
	sender.SetServer(serverIp);
	g_RuleFile.m_header.bisServerAllowed = true;
	isServerAllowed = true;
	
	((CButton*)(sysDlg->GetDlgItem(IDC_ISSERVERSET)))->SetCheck(1);
	AfxMessageBox("服务器已设置!");
	sysDlg->GetOwner()->GetOwner()->GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
	DestroyWindow();
}

BOOL CSetServerIpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    serverIp = g_RuleFile.m_header.szServerIp;
	UpdateData(false);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSetServerIpDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	DestroyWindow();
}
