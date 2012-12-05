// ARPGuardPage.cpp : 实现文件


#include "stdafx.h"
#include "Net Dragon.h"
#include "ARPGuardPage.h"
#include "RecordDialog.h"
#include "pcap.h"

// ARPGuardPage 对话框
ARPGuardPage     *dlg2;
CRecordDialog recDialog;
bool isResume = false;
char protectbuff[256];
char error3[PCAP_ERRBUF_SIZE];
IMPLEMENT_DYNAMIC(ARPGuardPage, CPropertyPage)

ARPGuardPage::ARPGuardPage()
	: CPropertyPage(ARPGuardPage::IDD)
{

}

ARPGuardPage::~ARPGuardPage()
{
}

void ARPGuardPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(ARPGuardPage, CPropertyPage)
	ON_BN_CLICKED(IDC_PROTECT, &ARPGuardPage::OnBnClickedProtect)
	ON_BN_CLICKED(IDC_SEE, &ARPGuardPage::OnBnClickedSee)
END_MESSAGE_MAP()


// ARPGuardPage 消息处理程序

BOOL ARPGuardPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	recDialog.Create(IDD_DIALOGRECORD,this );
	dlg2 = this;
	if (!startup)
	    m_list.AddString("当前模式为主动防御模式。");
	else
        m_list.AddString("当前模式为静态绑定模式。");
	m_list.AddString("加载记录文件成功。");
	//打开记录的网关
	FILE *fp;
	char temp[100];
	CString info = "当前记录的网关MAC为:";
	if ( ( fp = fopen("SysMsc.dat","r")) == NULL)
    {
		// 没有发现文件
        m_list.AddString("打开网关记录文件失败");
    }
    else
    {
		fgets(temp,99,fp);
		fclose(fp);
		info += temp;
		m_list.AddString(info+"。");
	}


    UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void ARPGuardPage::OnBnClickedProtect()
{
	// TODO: 在此添加控件通知处理程序代码
	pETH_t pPE;
    pARP_t pPA;
	if (!isResume)
    {
        isResume = !isResume;
        this->GetDlgItem(IDC_PROTECT)->SetWindowText(">>>停止保护<<<");
        memset( protectbuff , 0 , 256 );

        pPE = (pETH_t)protectbuff ;
        pPA = (pARP_t)pPE->data;
        pPE->ether_type = htons( ETH_TYPE );
        memcpy( pPE->ether_dhost  ,gatewaymac , 6);
        memcpy( pPA->arp_destination_ethernet_address ,gatewaymac , 6);
        pPA->arp_hardware_type = htons( ARP_HARDWARE );
        pPA->arp_protocol_type = htons( IP_TYPE );
        pPA->arp_operation_code = htons( ARP_REPLY );
        pPA->arp_hardware_length = MAC_ADDR_LEN;
        pPA->arp_protocol_length = IP_ADDR_LEN;
        pPA->arp_destination_ip_address = inet_addr( (char*)pAdapterInfo->GatewayList.IpAddress.String );
        pPA->arp_source_ip_address = inet_addr((char*)pAdapterInfo->IpAddressList.IpAddress.String);
        memcpy( pPE->ether_shost  ,mymac , 6);
        memcpy( pPA->arp_source_ethernet_address ,mymac , 6);
        
        CWinThread* m_pWinThread = AfxBeginThread(ProtectMy,(LPVOID)NULL,THREAD_PRIORITY_NORMAL,0,0,0);
        m_pWinThread->m_bAutoDelete = true;
		//m_list.AddString("保护本机到网关的线程已开启。");
	    
    }
    else
    {
        isResume = !isResume;
        this->GetDlgItem(IDC_PROTECT)->SetWindowTextA("保护网关");
		//m_list.AddString("保护本机到网关的线程已关闭。");
		
    }
}

UINT ProtectMy(LPVOID lpParam)
{
    pcap_t *pfp;
    if( (pfp = pcap_open_live( pAdapterInfo->AdapterName , 100, 0, 500, error3 ) ) == NULL )
    {
        AfxMessageBox(_T("打开网卡句柄失败！请检查是否安装了最新的WinPcap或者填写了正确的设置。"));
        return 0 ;
    }
    while(isResume)
    {
        Sleep(100);
        pcap_sendpacket( pfp, (u_char*)protectbuff, sizeof( ETH_t )+sizeof( ARP_t ) -1  );
    }
    ////////////////保护
    pcap_close( pfp );
    return 0;
}
void ARPGuardPage::OnBnClickedSee()
{
	// TODO: 在此添加控件通知处理程序代码
	recDialog.ShowWindow(SW_SHOW);
}
