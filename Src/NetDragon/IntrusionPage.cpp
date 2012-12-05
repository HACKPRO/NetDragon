/************************************************************************/
/* 
	入侵检测模块		Code by Win.Hunter
*/
/************************************************************************/

#include "stdafx.h"
#include "Net Dragon.h"
#include "IntrusionPage.h"

#include "sniffer.h"
#include "ipprotocol.h"

#include "tcpprotocol.h"
#include "udpprotocol.h"
#include "icmpprotocol.h"
#include "ethernetprotocol.h"
#include "intrusiondetect.h"


extern char TcpData[65535];
extern char UdpData[65535];
bool StopThread = TRUE;
CWinThread* pThread;
HWND hWnd;
extern struct PacketInformation packet;
extern struct IntrusionInfor IntrusionInformation;
extern struct IpHeader DisplayIP;
extern struct TcpHeader DisplayTCP;
extern struct UdpHeader DisplayUDP;
extern struct IcmpHeader DisplayIcmp;
extern struct EtherHeader DisplayEthernet;


UINT ThreadPacketCapture(LPVOID pParam);
// CIntrusionPage 对话框

IMPLEMENT_DYNAMIC(CIntrusionPage, CPropertyPage)

CIntrusionPage::CIntrusionPage()
	: CPropertyPage(CIntrusionPage::IDD)
{

}

CIntrusionPage::~CIntrusionPage()
{
}

void CIntrusionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INTRUSION, m_list_intrusion);
}


BEGIN_MESSAGE_MAP(CIntrusionPage, CPropertyPage)
	ON_BN_CLICKED(IDC_STARTDETECT, &CIntrusionPage::OnBnClickedStartdetect)
	ON_MESSAGE(WM_MY_MESSAGE_INTRUSION,OnIntrusion)
	ON_BN_CLICKED(IDC_STOP, &CIntrusionPage::OnBnClickedStop)
END_MESSAGE_MAP()


// CIntrusionPage 消息处理程序

void CIntrusionPage::OnBnClickedStartdetect()
{
	// TODO: 在此添加控件通知处理程序代码
	(CButton*)GetDlgItem (IDC_STOP)->EnableWindow(TRUE);
	StopThread = FALSE;
	hWnd = GetSafeHwnd();
	pThread = AfxBeginThread(ThreadPacketCapture, hWnd);
	pThread->m_bAutoDelete = FALSE;
	(CButton*)GetDlgItem (IDC_STARTDETECT)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_STARTDETECT)->SetWindowText("监视中");
}

LRESULT CIntrusionPage::OnIntrusion(WPARAM wParam, LPARAM lParam)
{
	static int i = 1;  
	char str[10]; 
	sprintf(str, "%d", i);
	m_list_intrusion.InsertItem(0, "");
	m_list_intrusion.SetItemText(0, 0, str);
	m_list_intrusion.SetItemText(0, 1, IntrusionInformation.time);
	m_list_intrusion.SetItemText(0, 2, IntrusionInformation.attackip);
	m_list_intrusion.SetItemText(0, 3, IntrusionInformation.information);
	i++;  
	return  0;
}


UINT ThreadPacketCapture(LPVOID pParam)
{
	PacketOperation();
	return 0;
}

BOOL CIntrusionPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_list_intrusion.InsertColumn(0, "序号", LVCFMT_LEFT, 300, 1);
	m_list_intrusion.InsertColumn(1, "时间", LVCFMT_LEFT, 300, 1);
	m_list_intrusion.InsertColumn(2, "入侵者IP", LVCFMT_LEFT, 300, 1);
	m_list_intrusion.InsertColumn(3, "攻击信息", LVCFMT_LEFT, 300, 1);
	m_list_intrusion.SetColumnWidth(0, 40);
	m_list_intrusion.SetColumnWidth(1, 150);
	m_list_intrusion.SetColumnWidth(2, 150);
	m_list_intrusion.SetColumnWidth(3, 200);

	ListView_SetExtendedListViewStyle(m_list_intrusion.m_hWnd,
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_FLATSB |
		LVS_EX_HEADERDRAGDROP);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CIntrusionPage::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	(CButton*)GetDlgItem (IDC_STOP)->EnableWindow(FALSE);
    if (StopThread == TRUE)
	{
		return;
	}
	StopThread = TRUE;
	(CButton*)GetDlgItem (IDC_STARTDETECT)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_STARTDETECT)->SetWindowText("开始");
}
