/************************************************************************/
/* 
	程序窗体入口模块	Code by Win.Hunter
*/
/************************************************************************/

#include "stdafx.h"
#include "Net Dragon.h"
#include "Net DragonDlg.h"
#include "NoticeDlg.h"
#include "HistoryInfo.h"
#include "pcap.h"
#include "RecordDialog.h"
#include "AboutDlg.h"
#include "MonitorPage.h"
#include "RulePage.h"
#include "SyssetPage.h"
#include "Mutex.h"



bool isMonitorArp = true;
bool isRecordPacket = false;

extern CRecordDialog recDialog;

char error2[PCAP_ERRBUF_SIZE]; 
Mutex mx;


 
char sendbuff[256]; 
NOTIFYICONDATA   nid;
CNetDragonDlg    *dlg;
extern ARPGuardPage     *dlg2;

bool SaveHistoryInfo(const CString historyinfo);


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/************************************************************************/
/*  CNetDragonDlg 对话框                                                */
/************************************************************************/
CNetDragonDlg::CNetDragonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetDragonDlg::IDD, pParent) 
		, m_statistic(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	sendCount = 0;
}



BEGIN_MESSAGE_MAP(CNetDragonDlg, CDialog)
	ON_MESSAGE(WM_SHOWTASK,onShowTask)
	ON_MESSAGE(WM_ARPCHEAT,onshowarp)
	ON_MESSAGE(WM_SENDINFO,onSendInfo)
	ON_MESSAGE(PM_QUERY_ACL_NOTIFY, OnQueryAcl)
	ON_MESSAGE(PM_SESSION_NOTIFY, OnSessionNotify)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

	ON_WM_CLOSE()
	ON_COMMAND(ID_CLOSEL, &CNetDragonDlg::OnClosel)
	ON_COMMAND(ID_FILE_CLOSE, &CNetDragonDlg::OnFileClose)
	ON_COMMAND(ID_FILE_ABOUT, &CNetDragonDlg::OnFileAbout)
	ON_COMMAND(ID_SEEMAC, &CNetDragonDlg::OnSeemac)
	ON_COMMAND(ID_vcers, &CNetDragonDlg::Onvcers)
	ON_COMMAND(ID_ATTACK, &CNetDragonDlg::OnAttack)
	ON_COMMAND(ID_HISTORY, &CNetDragonDlg::OnHistory)
	
	ON_COMMAND(ID_LOCALINFO, &CNetDragonDlg::OnLocalinfo)
	ON_COMMAND(ID_SCANHOST, &CNetDragonDlg::OnScanhost)
	ON_COMMAND(ID_ABOUT, &CNetDragonDlg::OnAbout)
	ON_BN_CLICKED(IDC_APPLY, &CNetDragonDlg::OnApply)
	ON_BN_CLICKED(IDOK, &CNetDragonDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ANNUL, &CNetDragonDlg::OnBnClickedAnnul)
	ON_COMMAND(ID_WHITE, &CNetDragonDlg::OnWhite)
	ON_COMMAND(ID_BLUE, &CNetDragonDlg::OnBlue)
	ON_COMMAND(ID_COMPUT, &CNetDragonDlg::OnComput)
	ON_COMMAND(ID_32800, &CNetDragonDlg::On32800)
	ON_COMMAND(ID_HELP, &CNetDragonDlg::OnHelp)
	ON_BN_CLICKED(IDC_UPDATE, &CNetDragonDlg::OnBnClickedUpdate)
END_MESSAGE_MAP()


/************************************************************************/
/*  创建窗体，构建窗体结构及其ICON图标                                  */
/************************************************************************/
BOOL CNetDragonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	dlg = this;
	CNoticeDlg noticedlg;
	noticedlg.m_notice = "\r\n软件功能：\r\n1、监视APR欺骗与攻击.有效的保护网卡MAC.\r\n2、监视网络访问行为和信息,协助分析网络交互.\r\n3、网络防火墙有效控制本机进程的网络访问.\r\n4、全天监控/记录入侵者的IP和攻击信息.\r\n5、快速扫描到局域网内存活的主机.\r\n6、攻击/监听局域网内主机，获取QQ号码.\r\n\r\n软件/BUG/交流QQ群 ： ① 18203920  ② 18129647\r\n版权/其他纠纷问题  ： admin@HackProLabs.com\r\n\r\n(C) HackPro Labs\t\twww.HackProLabs.com";
	noticedlg.DoModal();
	
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
    nid.hWnd=this->m_hWnd;
    nid.uID=IDI_HOST;
    nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
    nid.uCallbackMessage=WM_SHOWTASK;  //自定义的消息名称 
    nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
    strcpy(nid.szTip,_T("Net \r\nPowered by HackPro Labs"));//信息提示条为“计划任务提醒”
    Shell_NotifyIcon(NIM_ADD,&nid);//在托盘区添加图标
	
    CHAR sz[256];
	//设置共享属性
	::GetModuleFileName(NULL, sz, 256);
	g_pIoControl->SetPhoenixInstance(m_hWnd, sz);
	// 向属性页中加入标签，标签名由各个子对话框的标题栏决定
	m_sheet.AddPage(&arpguardpage);
	m_sheet.AddPage(&m_MonitorPage);
	m_sheet.AddPage(&m_RulePage);
	m_sheet.AddPage(&m_IntrusionPage);
    m_sheet.AddPage(&m_SyssetPage);
    // 用Create来创建一个属性页
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE, WS_EX_CONTROLPARENT);
		
	// 调整属性页的大小和位置
	RECT rect;
	GetWindowRect(&rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	m_sheet.SetWindowPos(NULL, 
		0, 0, width - 4, height - 25, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);

	//防御开始
    UpdateData(true);
    isMonitorArp = true;
	
	//ARP防御主线程
    CWinThread* m_pWinThread = AfxBeginThread(StartArpMonitor,NULL,THREAD_PRIORITY_NORMAL,0,0,0);
    m_pWinThread->m_bAutoDelete = true;
    

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

/************************************************************************/
/*  如果向对话框添加最小化按钮，则需要下面的代码                        */
/*  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，                */
/*  这将由框架自动完成。                                                */
/************************************************************************/
void CNetDragonDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

/************************************************************************/
/*  当用户拖动最小化窗口时系统调用此函数取得光标显示。                  */
/************************************************************************/
HCURSOR CNetDragonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/************************************************************************/
/*  启动ARP监视器                                                       */
/************************************************************************/
UINT StartArpMonitor(LPVOID lpParam)
{
    CTime theTime;
    FILE *recfp=NULL;
    int k,i;
    unsigned long ArpRequest=0 , ArpResponse=0 ,ArpCheat=0, ArpScan=0;//数据包统计
    
    pcap_t *pfp;//handle of winpcap
    
    
    struct pcap_pkthdr *pkt_header;//the information of a packet
    u_char *pkt_data;//point to the packet
	//用于检测全网段顺序扫描
    unsigned char mac1[6]={0},mac2[6]={0},mac3[6]={0},rmac[6]={0};
    unsigned char iplastchar1=0,iplastchar2=0,iplastchar3=0,ipscanfirst=0;
    unsigned long iplasttemp=0;
	//用于检测ARP欺骗
	char mac11[18]={0},mac22[18]={0},macTemp[18],macTemp2[18];
    mac11[0]=mac22[0]='\0';
	unsigned char p_getedmac_dyamic[6];
	unsigned char p_getedmac_static[6];
	unsigned char p_getedmac_static_temp[6];

	CString scanIP,scanMAC,scanTime,scanIPTemp = "0";
	
	memset(p_getedmac_static_temp, 0xff, sizeof(p_getedmac_static_temp));
	
    ULONG macAddLen=6;
	unsigned char sendmac[6];
    memset(sendmac, 0xff, sizeof(sendmac));
	int count = 0;
	bool ischeated = true;
	char result[100];

	dlg2->SetDlgItemInt(IDC_REPLY,ArpResponse,1);
	dlg2->SetDlgItemInt(IDC_REQUEST,ArpRequest,1);
	dlg2->SetDlgItemInt(IDC_SCAN,ArpScan,1);
	dlg2->SetDlgItemInt(IDC_CHEAT,ArpCheat,1);

	
    //记录文件开启
    if (isRecordPacket == true)
    {
        recfp = fopen("ARPpacket.txt","w");
        if (recfp==NULL)
        {
            isRecordPacket = 0;
            AfxMessageBox("创建记录文件失败!记录功能取消!");
        }
        else
        {
            fputs("该文件保存从程序开始时接受及发送的ARP数据包\r\n可供有一定网络经验的人进行分析\r\n\r\n Powered By HackProLa\r\n格式      \t发送端IP         \t发送端MAC         \t接收者IP\r\n\r\n",recfp);
        }
    }

    while(isMonitorArp)
	{
        if((pfp = pcap_open_live( pAdapterInfo->AdapterName ,  100, 0, 1, error2 ) ) == NULL )//非混杂模式
        {
            AfxMessageBox(_T("无法打开当前网卡!\r\n\r\n请确定WinPcap驱动已经正常安装.压缩包里自带了Winpcap的安装程序,请先安装."));
            isMonitorArp = false;
            exit(1);
        }
		
	
        while( isMonitorArp && (k=pcap_next_ex(pfp,&pkt_header,(const u_char**)&pkt_data))>=0 )//相当于回调函数的功能哈
		{
            if (k==0)
                continue;
            //此处为arp request的包
            if (*(unsigned short *)(pkt_data+12) == htons(2054) && *(unsigned short*)(pkt_data+20)==htons(1))//2054换算过来就是OX0806哈
            {
                ArpRequest++;
				dlg2->SetDlgItemInt(IDC_REQUEST,ArpRequest,1);

                if (isRecordPacket == true)
                {
                    theTime = CTime::GetCurrentTime();
                    sprintf(result," ARP REQUEST:\t%d.%d.%d.%d\t%02x-%02x-%02x-%02x-%02x-%02x\t%d.%d.%d.%d",*(unsigned char*)(pkt_data+28),*(unsigned char*)(pkt_data+29),*(unsigned char*)(pkt_data+30),*(unsigned char*)(pkt_data+31),*(unsigned char*)(pkt_data+22),*(unsigned char*)(pkt_data+23),*(unsigned char*)(pkt_data+24),*(unsigned char*)(pkt_data+25),*(unsigned char*)(pkt_data+26),*(unsigned char*)(pkt_data+27),*(unsigned char*)(pkt_data+38),*(unsigned char*)(pkt_data+39),*(unsigned char*)(pkt_data+40),*(unsigned char*)(pkt_data+41));
                    fputs(theTime.Format("%Y-%b-%d %H:%M:%S"),recfp);//上面的格式为发送端IP-发送端MAC-目的IP地址
                    fputs(result,recfp);
                    fputs("\r\n",recfp);
                }
				
                if(*(unsigned char*)(pkt_data+37)==255 || *(unsigned char*)(pkt_data+37)==0)//一般普通的MAC扫描这个目的以太网的MAC位置不是0就是FF。
				{
                    //普通MAC获得扫描
                    memcpy(mac3,mac2,6*sizeof(unsigned char));
                    memcpy(mac2,mac1,6*sizeof(unsigned char));//复制网关
                    iplastchar3 = iplastchar2;
                    iplastchar2 = iplastchar1;
                    memcpy(mac1,pkt_data+22,6*sizeof(unsigned char));//发送端以太网地址存入mac1
                    iplastchar1 = *(unsigned char*) (pkt_data+41);//目的IP地址倒数最后一位存入iplastchar1

                    if (*(unsigned long*)(pkt_data+28) == inet_addr("19.87.11.27"))//发送端IP是特定IP则关闭
                    {
                        //遇到指定IP关闭程序
                        AfxMessageBox("你干的坏事太多啦-__-!");

                        exit(0);
                    }
                    else if (*(unsigned long*)(pkt_data+28) == inet_addr((char*)pAdapterInfo->GatewayList.IpAddress.String) || *(unsigned long*)(pkt_data+28) == inet_addr((char*)pAdapterInfo->IpAddressList.IpAddress.String))//网关和自己的IP为信任IP
                    {
                        //信任IP
                        continue;
                    }

                    
                    if (memcmp(mac1,mac2,6*sizeof(unsigned char)) != 0 || memcmp(mac2,mac3,6*sizeof(unsigned char)) != 0)
                    {
                        continue;//检测全网段顺序扫描的算法设计，先判断发送端MAC是否相等
                    }
                    
                    if (iplastchar1 != iplastchar2 + 1 || iplastchar2 != iplastchar3 + 1)
                    {
                        continue;//再判断目标IP是否连续。
                    }
                    
                    if (iplasttemp!=*(unsigned long*)(pkt_data+28) || ipscanfirst>=iplastchar3)
                    {
                        theTime = CTime::GetCurrentTime();
  
                        
                        scanIP.Format("%d.%d.%d.%d",*(unsigned char*)(pkt_data+28),*(unsigned char*)(pkt_data+29),*(unsigned char*)(pkt_data+30),*(unsigned char*)(pkt_data+31));
                        scanMAC.Format("%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",mac1[0],mac1[1],mac1[2],mac1[3],mac1[4],mac1[5]);
						if( scanIPTemp != scanIP)
						{
							ArpScan++;
							scanIPTemp = scanIP;
							scanTime = theTime.Format("%Y-%b-%d %H:%M:%S");
							sprintf(result,"\t全网段顺序扫描\t %s",scanIP);
							recDialog.m_recstring = "\r\n" + recDialog.m_recstring;
							recDialog.m_recstring = result + recDialog.m_recstring;
							recDialog.m_recstring = theTime.Format("%Y-%b-%d %H:%M:%S") + recDialog.m_recstring;
						    if(isServerAllowed)
						    {
					 		    
							    sender.SetInfo(scanIP+"#2");
							    PostMessage(dlg->m_hWnd,WM_SENDINFO,0,NULL);
						    }
							
							recDialog.UpdateData(false);
							
						}
						ipscanfirst = iplastchar3;
					    iplasttemp = *(unsigned long*)(pkt_data+28);
                    }
                    

                        dlg2->SetDlgItemInt(IDC_SCAN,ArpScan,1);
                        

                }
			}
			//ARP RESPONSE部分
            else if (*(unsigned short *)(pkt_data+12) == htons(2054) && *(unsigned short*)(pkt_data+20)==htons(2))
			{

                ArpResponse++;
                if (isRecordPacket == true)
                {
                    theTime = CTime::GetCurrentTime();
                    sprintf(result," ARP RESPONSE:\t%d.%d.%d.%d\t%02x-%02x-%02x-%02x-%02x-%02x\t%d.%d.%d.%d\t%02x-%02x-%02x-%02x-%02x-%02x",*(unsigned char*)(pkt_data+28),*(unsigned char*)(pkt_data+29),*(unsigned char*)(pkt_data+30),*(unsigned char*)(pkt_data+31),*(unsigned char*)(pkt_data+22),*(unsigned char*)(pkt_data+23),*(unsigned char*)(pkt_data+24),*(unsigned char*)(pkt_data+25),*(unsigned char*)(pkt_data+26),*(unsigned char*)(pkt_data+27),*(unsigned char*)(pkt_data+38),*(unsigned char*)(pkt_data+39),*(unsigned char*)(pkt_data+40),*(unsigned char*)(pkt_data+41),*(unsigned char*)(pkt_data+32),*(unsigned char*)(pkt_data+33),*(unsigned char*)(pkt_data+34),*(unsigned char*)(pkt_data+35),*(unsigned char*)(pkt_data+36),*(unsigned char*)(pkt_data+37));
                    fputs(theTime.Format("%Y-%b-%d %H:%M:%S"),recfp);//上面的格式为发送端IP-发送端MAC-接收端IP和接收端MAC
                    fputs(result,recfp);
                    fputs("\r\n",recfp);
                }
				if (*(unsigned long*)(pkt_data+28) == inet_addr((char*)pAdapterInfo->GatewayList.IpAddress.String)) //如果发送端IP是网关,那么特别检查下
				{
	                if (!startup)
					{
						count++;
						if (count > 50)
							count = 0;
						for(i=0;i<6;i++)
						{
						   p_getedmac_dyamic[i]=*(unsigned char*)(pkt_data+22+i);//传入发送端以太网地址
						}
		                if (!memcmp(p_getedmac_dyamic,mymac,sizeof(mymac)))//自己IP为信任IP
                        {
                          //信任IP
                          continue;
                        }

						sprintf(macTemp,"%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",p_getedmac_dyamic[0],p_getedmac_dyamic[1],p_getedmac_dyamic[2],p_getedmac_dyamic[3],p_getedmac_dyamic[4],p_getedmac_dyamic[5]);
						if (mac22[0]=='\0' || (strcmp(mac11,macTemp) && strcmp(mac22,macTemp)))//这个算法是为下面作铺垫,若有源MAC不同的RESPONSE,则一个是网关,一个是欺骗者
						{
							strcpy(mac22,mac11);
							strcpy(mac11,macTemp);
						}
						//此算法原理是若有ARP欺骗,则比较先前发往网关的REQUEST后的RESPONSE与欺骗者的RESPONSE则可

						if (strcmp(mac11,mac22)!=0 && mac22[0]!='\0') //若MAC2!=0,那么上面的if判断至少收到了3个以上RESPONSE包,且至少有2个RESPONSE包源MAC不同
						{
							ArpCheat++;
							if (recDialog.keepon)
							{
							
								theTime = CTime::GetCurrentTime();
								if (recDialog.m_recstring.GetLength() > 7000)//防止占用内存过大,影响速度
								recDialog.m_recstring.Empty();
								sprintf(result,"\t网关欺骗 MAC地址1:%s\tMAC地址2:%s\t其中之一必为欺骗者",mac11,mac22);
								recDialog.m_recstring = "\r\n" + recDialog.m_recstring;
								recDialog.m_recstring = result + recDialog.m_recstring;
								recDialog.m_recstring = theTime.Format("%Y-%b-%d %H:%M:%S") + recDialog.m_recstring;
								recDialog.UpdateData(false);
								dlg2->SetDlgItemInt(IDC_CHEAT,ArpCheat,1);
								mac22[0]='\0';
							}
						}
					}
					else
				     {
						for(i=0;i<6;i++)
						{
						   p_getedmac_static[i]=*(unsigned char*)(pkt_data+22+i);//传入发送端以太网地址
						}
						if (!memcmp(p_getedmac_static,mymac,sizeof(mymac)))//自己MAC为信任MAC
                        {
                          //信任MAC
                          continue;
                        }



						if (memcmp(p_getedmac_static,gatewaymac,sizeof(gatewaymac)))
						{  
					       /*if (recDialog.m_recstring.GetLength() > 7000)
						   recDialog.m_recstring.Empty();//防止占用内存过大,影响速度*/

						   if (memcmp(p_getedmac_static_temp,p_getedmac_static,sizeof(p_getedmac_static_temp)))
						   {
							   ArpCheat++;
							   memset(CompareMacglobal,0xff,6);
							   memcpy(CompareMacglobal,p_getedmac_static,sizeof(p_getedmac_static));

							   PostMessage(dlg->m_hWnd,WM_ARPCHEAT,0,NULL);

							   memcpy(p_getedmac_static_temp,p_getedmac_static,sizeof(p_getedmac_static));
 
							   theTime = CTime::GetCurrentTime();
							   
							   
                               sprintf(macTemp2,"%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",p_getedmac_static[0],p_getedmac_static[1],p_getedmac_static[2],p_getedmac_static[3],p_getedmac_static[4],p_getedmac_static[5]);
							   if(isServerAllowed)
							   {
								   CString xx = macTemp2;
								   sender.SetInfo(xx+"#1");
								   PostMessage(dlg->m_hWnd,WM_SENDINFO,0,NULL);
							   }
							   sprintf(result,"网关欺骗MAC地址:%s",macTemp2);
							   recDialog.m_recstring = "\r\n" + recDialog.m_recstring;
							   recDialog.m_recstring = result + recDialog.m_recstring;
							   recDialog.m_recstring = theTime.Format("%Y-%b-%d%H:%M:%S") + recDialog.m_recstring;
							   recDialog.UpdateData(false);
							   SaveHistoryInfo(theTime.Format("%Y-%b-%d%H:%M:%S") + result);
							  
							   dlg2->SetDlgItemInt(IDC_CHEAT,ArpCheat,1);
						   }
						}
					}
				}
			}

           

            
            dlg2->SetDlgItemInt(IDC_REPLY,ArpResponse,1);
			if (count > 3 && ischeated && !startup)//如果可疑,则打开主动防御.条件是非静态模式下
			{
			    pETH_t pPE;
			    pARP_t pPA;
                unsigned char ucsend1[6],ucsend2[6];
				memset( sendbuff , 0 , 256 );
                memset(ucsend1, 0xff, 6);
				memset(ucsend2, 0, 6);
				pPE = (pETH_t)sendbuff ;
				pPA = (pARP_t)pPE->data;
				pPE->ether_type = htons( ETH_TYPE );
				memcpy( pPE->ether_dhost  ,ucsend1, 6);
				memcpy( pPA->arp_destination_ethernet_address ,ucsend2, 6);
				pPA->arp_hardware_type = htons( ARP_HARDWARE );
				pPA->arp_protocol_type = htons( IP_TYPE );
				pPA->arp_operation_code = htons( ARP_REQUEST );
				pPA->arp_hardware_length = MAC_ADDR_LEN;
				pPA->arp_protocol_length = IP_ADDR_LEN;
				pPA->arp_destination_ip_address = inet_addr( (char*)pAdapterInfo->GatewayList.IpAddress.String );
				pPA->arp_source_ip_address = inet_addr((char*)pAdapterInfo->IpAddressList.IpAddress.String);
				memcpy( pPE->ether_shost  ,mymac , 6);
				memcpy( pPA->arp_source_ethernet_address ,mymac , 6);
				
				CWinThread* m_pWinThread = AfxBeginThread(sendrequest,(LPVOID)NULL,THREAD_PRIORITY_NORMAL,0,0,0);
                m_pWinThread->m_bAutoDelete = true;
				ischeated = false;
			
			}
		}
        pcap_close( pfp );
        if (isRecordPacket == true)
        {
            fclose(recfp);
            
        }
	
	}
  
    return 0;
}


UINT sendrequest(LPVOID laParam)
{
    pcap_t *pfp;
    if( (pfp = pcap_open_live( pAdapterInfo->AdapterName , 100, 0, 1000, error2 ) ) == NULL )
    {
        AfxMessageBox(_T("打开网卡句柄失败！请检查是否请安装WinPcap v4.0.2 或更高版本,或者填写了正确的设置。"));
        return 0 ;
    }
    while(true)
    {
        Sleep(500);
        pcap_sendpacket( pfp, (u_char*)sendbuff,  sizeof( ETH_t )+sizeof( ARP_t ) -1  );
    }
    ////////////////保护
    pcap_close( pfp );
    return 0;
}

LRESULT CNetDragonDlg::onShowTask(WPARAM wParam,LPARAM lParam) 
//wParam接收的是图标的ID，而lParam接收的是鼠标的行为 
{ 
    if(wParam!=IDI_HOST)
        return 1;
    switch(lParam) 
    { 
    case WM_RBUTTONUP://右键起来时弹出快捷菜单 
        { 
         CPoint pt;
 		 GetCursorPos(&pt);
		 CMenu menu;
		 menu.LoadMenu(IDR_POPUPMENU);
		 menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,	pt.x,pt.y,this);	
        } 
        break; 
    case WM_LBUTTONDOWN://单击左键的处理 
        {
            this->ShowWindow(SW_SHOW);//显示主窗口 
            
        }
        break;
    } 
    return 0; 
}
void CNetDragonDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(GetDlgItem(IDC_APPLY)->IsWindowEnabled())
	{
		if(AfxMessageBox("要保存所做的修改吗？", MB_YESNO) == IDYES)
		{
			OnApply();
		}
	}

	Shell_NotifyIcon(NIM_DELETE,&nid);	//在托盘区删除图标
	CDialog::OnCancel();
	exit(0);
}

void CNetDragonDlg::OnClosel()
{
	if(GetDlgItem(IDC_APPLY)->IsWindowEnabled())
	{
		if(AfxMessageBox("要保存所做的修改吗？", MB_YESNO) == IDYES)
		{
			OnApply();
		}
	}
	Shell_NotifyIcon(NIM_DELETE,&nid);	//在托盘区删除图标
	CDialog::OnCancel();
}


/************************************************************************/
/*  托盘图标退出                                                        */
/************************************************************************/
void CNetDragonDlg::OnFileClose()
{
	if(GetDlgItem(IDC_APPLY)->IsWindowEnabled())
	{
		if(AfxMessageBox("要保存所做的修改吗？", MB_YESNO) == IDYES)
		{
			OnApply();
		}
	}
	Shell_NotifyIcon(NIM_DELETE,&nid);	//在托盘区删除图标
	exit(0);
}

void CNetDragonDlg::OnFileAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg aboutdlg;
	aboutdlg.DoModal();
}

void CNetDragonDlg::OnSeemac()
{
	// TODO: 在此添加命令处理程序代码
	FILE *fp;
	char temp[100];
	CString info = "当前记录的网关MAC为:";
	if ( ( fp = fopen("SysMsc.dat","r")) == NULL)
    {
                //没有发现文件
        AfxMessageBox("打开MAC记录文件失败!");
    }
    else
    {
		fgets(temp,99,fp);
		fclose(fp);
		info += temp;
		AfxMessageBox(info);
	}
}

void CNetDragonDlg::Onvcers()
{
	// TODO: 在此添加命令处理程序代码
	::ShellExecute(NULL,"open","http://www.HackProLabs.com",NULL,NULL,SW_SHOW);
}

void CNetDragonDlg::OnAttack()
{
	// TODO: 在此添加命令处理程序代码
	attackdlg.Create(IDD_ATTACKDLG,this);
	attackdlg.ShowWindow(SW_SHOW);
	
}

void CNetDragonDlg::OnHistory()
{
	// TODO: 在此添加命令处理程序代码
	historydlg.Create(IDD_HISTORYDLG,this);
	historydlg.ShowWindow(SW_SHOW);

}

LRESULT CNetDragonDlg::onshowarp(WPARAM wParam,LPARAM lParam)
{
	if(sendCount++ > 5)
		return 0;
	else
	{
       warningdlg.Create(IDD_WARNINGDLG,0);
       warningdlg.ShowWindow(SW_SHOW);
	}
	return 0;
}
LRESULT CNetDragonDlg::onSendInfo(WPARAM wParam,LPARAM lParam)
{
	CWinThread* m_pWinThread = AfxBeginThread(startSendInfo,NULL,THREAD_PRIORITY_NORMAL,0,0,0);
    m_pWinThread->m_bAutoDelete = true;
	
	return 0;
}
/*void CNetDragonDlg::OnBnClickedButtontrace()
{
	// TODO: 在此添加控件通知处理程序代码
	
}*/

bool SaveHistoryInfo(const CString historyinfo)
{
	FILE *fp=NULL;
	fp = fopen(".\\History.log","a");
	if (fp==NULL)
	{
		AfxMessageBox("创建记录文件失败!记录功能取消!");
		return false;
	}
	else
	{
		fputs(historyinfo,fp);
		fputs("\n",fp);
		fclose(fp);
		
	}
	return true;
}

void CNetDragonDlg::OnLocalinfo()
{
	// TODO: 在此添加命令处理程序代码
	localinfodlg.Create(IDD_LOCALINFODLG,this);
	localinfodlg.ShowWindow(SW_SHOW);

}

void CNetDragonDlg::OnScanhost()
{
	// TODO: 在此添加命令处理程序代码
	activehostdlg.Create(IDD_SCANDLG,this);
	activehostdlg.ShowWindow(SW_SHOW);
}

void CNetDragonDlg::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg aboutdlg;
	aboutdlg.DoModal();
}


long CNetDragonDlg::OnQueryAcl(WPARAM wParam, LPARAM lParam)		// 处理PM_QUERY_ACL_NOTIFY消息
{
	// 获取询问的应用程序名称
	LPCSTR lpszApp = g_pIoControl->GetQueryApp(wParam);

	if(CRulePage::AddQueryRule(lpszApp))
		g_pIoControl->SetQueryApp(wParam, 1); // 放行
	else 
		g_pIoControl->SetQueryApp(wParam, 0); // 拒绝

	return 0; 
}

long CNetDragonDlg::OnSessionNotify(WPARAM wParam, LPARAM lParam)	// 处理PM_SESSION_NOTIFY消息
{
	SESSION session;
	// 获取DLL发送来的Session
	g_pIoControl->GetSession(&session, wParam);
	// 将Session传递给CMonitorPage类
	CMonitorPage::HandleNotifySession(&session, lParam);
	return 0;
}
void CNetDragonDlg::OnApply()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if(!GetDlgItem(IDC_APPLY)->IsWindowEnabled())
		return;

	// 将用户的设置保存到文件
	if(!g_RuleFile.SaveRules())
	{
		MessageBox("保存规则出错。");
		return;
	}
	// 将文件中的数据应用到DLL模块和SYS模块
	if(!theApp.ApplyFileData())
		return;

	// 设置开机自动启动
	theApp.SetAutoStart(g_RuleFile.m_header.bAutoStart);

	// 无效本按钮
	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
}

void CNetDragonDlg::OnBnClickedOk()
{
	
    OnApply();
	CWnd::ShowWindow(SW_HIDE);
}


/************************************************************************/
/* 退出按钮退出事件                                                        */
/************************************************************************/
void CNetDragonDlg::OnBnClickedAnnul()
{
	
	if(GetDlgItem(IDC_APPLY)->IsWindowEnabled())
	{
		if(AfxMessageBox("要保存所做的修改吗？", MB_YESNO) == IDYES)
		{
			OnApply();
		}
	}
	Shell_NotifyIcon(NIM_DELETE,&nid);	//在托盘区删除图标
	CDialog::OnCancel();
}

void CNetDragonDlg::OnWhite()
{
	// TODO: 在此添加命令处理程序代码
	skinppLoadSkin(_T(".\\Skin\\AquaOS.ssk"));
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOW);
}

void CNetDragonDlg::OnBlue()
{
	// TODO: 在此添加命令处理程序代码
	skinppLoadSkin(_T(".\\Skin\\bbq.ssk"));
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOW);
}

void CNetDragonDlg::OnComput()
{
	// TODO: 在此添加命令处理程序代码
	skinppLoadSkin(_T(".\\Skin\\machine.ssk"));
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOW);
}

void CNetDragonDlg::On32800()
{
	// TODO: 在此添加命令处理程序代码
	skinppLoadSkin(_T(".\\Skin\\bbq.ssk"));
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOW);
}

UINT startSendInfo(LPVOID lpParam)
{
	mx.Lock();
	sender.SendInfo();
	return 0;
	mx.Unlock();
}

/************************************************************************/
/* 帮助跳入网站                                                         */
/************************************************************************/
void CNetDragonDlg::OnHelp()
{
	::ShellExecute(NULL,"open","http://www.HackProLabs.com",NULL,NULL,SW_SHOW);
}

/************************************************************************/
/*  软件升级                                                            */
/************************************************************************/
void CNetDragonDlg::OnBnClickedUpdate()
{
	sender.SetInfo("HackProLabs#1");
	sender.SendInfo();
}