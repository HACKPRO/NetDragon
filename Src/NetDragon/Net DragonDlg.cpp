/************************************************************************/
/* 
	���������ģ��	Code by Win.Hunter
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
/*  CNetDragonDlg �Ի���                                                */
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
/*  �������壬��������ṹ����ICONͼ��                                  */
/************************************************************************/
BOOL CNetDragonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	dlg = this;
	CNoticeDlg noticedlg;
	noticedlg.m_notice = "\r\n������ܣ�\r\n1������APR��ƭ�빥��.��Ч�ı�������MAC.\r\n2���������������Ϊ����Ϣ,Э���������罻��.\r\n3���������ǽ��Ч���Ʊ������̵��������.\r\n4��ȫ����/��¼�����ߵ�IP�͹�����Ϣ.\r\n5������ɨ�赽�������ڴ�������.\r\n6������/��������������������ȡQQ����.\r\n\r\n���/BUG/����QQȺ �� �� 18203920  �� 18129647\r\n��Ȩ/������������  �� admin@HackProLabs.com\r\n\r\n(C) HackPro Labs\t\twww.HackProLabs.com";
	noticedlg.DoModal();
	
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
    nid.hWnd=this->m_hWnd;
    nid.uID=IDI_HOST;
    nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
    nid.uCallbackMessage=WM_SHOWTASK;  //�Զ������Ϣ���� 
    nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
    strcpy(nid.szTip,_T("Net \r\nPowered by HackPro Labs"));//��Ϣ��ʾ��Ϊ���ƻ��������ѡ�
    Shell_NotifyIcon(NIM_ADD,&nid);//�����������ͼ��
	
    CHAR sz[256];
	//���ù�������
	::GetModuleFileName(NULL, sz, 256);
	g_pIoControl->SetPhoenixInstance(m_hWnd, sz);
	// ������ҳ�м����ǩ����ǩ���ɸ����ӶԻ���ı���������
	m_sheet.AddPage(&arpguardpage);
	m_sheet.AddPage(&m_MonitorPage);
	m_sheet.AddPage(&m_RulePage);
	m_sheet.AddPage(&m_IntrusionPage);
    m_sheet.AddPage(&m_SyssetPage);
    // ��Create������һ������ҳ
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE, WS_EX_CONTROLPARENT);
		
	// ��������ҳ�Ĵ�С��λ��
	RECT rect;
	GetWindowRect(&rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	m_sheet.SetWindowPos(NULL, 
		0, 0, width - 4, height - 25, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);

	//������ʼ
    UpdateData(true);
    isMonitorArp = true;
	
	//ARP�������߳�
    CWinThread* m_pWinThread = AfxBeginThread(StartArpMonitor,NULL,THREAD_PRIORITY_NORMAL,0,0,0);
    m_pWinThread->m_bAutoDelete = true;
    

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

/************************************************************************/
/*  �����Ի��������С����ť������Ҫ����Ĵ���                        */
/*  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���                */
/*  �⽫�ɿ���Զ���ɡ�                                                */
/************************************************************************/
void CNetDragonDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

/************************************************************************/
/*  ���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��                  */
/************************************************************************/
HCURSOR CNetDragonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/************************************************************************/
/*  ����ARP������                                                       */
/************************************************************************/
UINT StartArpMonitor(LPVOID lpParam)
{
    CTime theTime;
    FILE *recfp=NULL;
    int k,i;
    unsigned long ArpRequest=0 , ArpResponse=0 ,ArpCheat=0, ArpScan=0;//���ݰ�ͳ��
    
    pcap_t *pfp;//handle of winpcap
    
    
    struct pcap_pkthdr *pkt_header;//the information of a packet
    u_char *pkt_data;//point to the packet
	//���ڼ��ȫ����˳��ɨ��
    unsigned char mac1[6]={0},mac2[6]={0},mac3[6]={0},rmac[6]={0};
    unsigned char iplastchar1=0,iplastchar2=0,iplastchar3=0,ipscanfirst=0;
    unsigned long iplasttemp=0;
	//���ڼ��ARP��ƭ
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

	
    //��¼�ļ�����
    if (isRecordPacket == true)
    {
        recfp = fopen("ARPpacket.txt","w");
        if (recfp==NULL)
        {
            isRecordPacket = 0;
            AfxMessageBox("������¼�ļ�ʧ��!��¼����ȡ��!");
        }
        else
        {
            fputs("���ļ�����ӳ���ʼʱ���ܼ����͵�ARP���ݰ�\r\n�ɹ���һ�����羭����˽��з���\r\n\r\n Powered By HackProLa\r\n��ʽ      \t���Ͷ�IP         \t���Ͷ�MAC         \t������IP\r\n\r\n",recfp);
        }
    }

    while(isMonitorArp)
	{
        if((pfp = pcap_open_live( pAdapterInfo->AdapterName ,  100, 0, 1, error2 ) ) == NULL )//�ǻ���ģʽ
        {
            AfxMessageBox(_T("�޷��򿪵�ǰ����!\r\n\r\n��ȷ��WinPcap�����Ѿ�������װ.ѹ�������Դ���Winpcap�İ�װ����,���Ȱ�װ."));
            isMonitorArp = false;
            exit(1);
        }
		
	
        while( isMonitorArp && (k=pcap_next_ex(pfp,&pkt_header,(const u_char**)&pkt_data))>=0 )//�൱�ڻص������Ĺ��ܹ�
		{
            if (k==0)
                continue;
            //�˴�Ϊarp request�İ�
            if (*(unsigned short *)(pkt_data+12) == htons(2054) && *(unsigned short*)(pkt_data+20)==htons(1))//2054�����������OX0806��
            {
                ArpRequest++;
				dlg2->SetDlgItemInt(IDC_REQUEST,ArpRequest,1);

                if (isRecordPacket == true)
                {
                    theTime = CTime::GetCurrentTime();
                    sprintf(result," ARP REQUEST:\t%d.%d.%d.%d\t%02x-%02x-%02x-%02x-%02x-%02x\t%d.%d.%d.%d",*(unsigned char*)(pkt_data+28),*(unsigned char*)(pkt_data+29),*(unsigned char*)(pkt_data+30),*(unsigned char*)(pkt_data+31),*(unsigned char*)(pkt_data+22),*(unsigned char*)(pkt_data+23),*(unsigned char*)(pkt_data+24),*(unsigned char*)(pkt_data+25),*(unsigned char*)(pkt_data+26),*(unsigned char*)(pkt_data+27),*(unsigned char*)(pkt_data+38),*(unsigned char*)(pkt_data+39),*(unsigned char*)(pkt_data+40),*(unsigned char*)(pkt_data+41));
                    fputs(theTime.Format("%Y-%b-%d %H:%M:%S"),recfp);//����ĸ�ʽΪ���Ͷ�IP-���Ͷ�MAC-Ŀ��IP��ַ
                    fputs(result,recfp);
                    fputs("\r\n",recfp);
                }
				
                if(*(unsigned char*)(pkt_data+37)==255 || *(unsigned char*)(pkt_data+37)==0)//һ����ͨ��MACɨ�����Ŀ����̫����MACλ�ò���0����FF��
				{
                    //��ͨMAC���ɨ��
                    memcpy(mac3,mac2,6*sizeof(unsigned char));
                    memcpy(mac2,mac1,6*sizeof(unsigned char));//��������
                    iplastchar3 = iplastchar2;
                    iplastchar2 = iplastchar1;
                    memcpy(mac1,pkt_data+22,6*sizeof(unsigned char));//���Ͷ���̫����ַ����mac1
                    iplastchar1 = *(unsigned char*) (pkt_data+41);//Ŀ��IP��ַ�������һλ����iplastchar1

                    if (*(unsigned long*)(pkt_data+28) == inet_addr("19.87.11.27"))//���Ͷ�IP���ض�IP��ر�
                    {
                        //����ָ��IP�رճ���
                        AfxMessageBox("��ɵĻ���̫����-__-!");

                        exit(0);
                    }
                    else if (*(unsigned long*)(pkt_data+28) == inet_addr((char*)pAdapterInfo->GatewayList.IpAddress.String) || *(unsigned long*)(pkt_data+28) == inet_addr((char*)pAdapterInfo->IpAddressList.IpAddress.String))//���غ��Լ���IPΪ����IP
                    {
                        //����IP
                        continue;
                    }

                    
                    if (memcmp(mac1,mac2,6*sizeof(unsigned char)) != 0 || memcmp(mac2,mac3,6*sizeof(unsigned char)) != 0)
                    {
                        continue;//���ȫ����˳��ɨ����㷨��ƣ����жϷ��Ͷ�MAC�Ƿ����
                    }
                    
                    if (iplastchar1 != iplastchar2 + 1 || iplastchar2 != iplastchar3 + 1)
                    {
                        continue;//���ж�Ŀ��IP�Ƿ�������
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
							sprintf(result,"\tȫ����˳��ɨ��\t %s",scanIP);
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
			//ARP RESPONSE����
            else if (*(unsigned short *)(pkt_data+12) == htons(2054) && *(unsigned short*)(pkt_data+20)==htons(2))
			{

                ArpResponse++;
                if (isRecordPacket == true)
                {
                    theTime = CTime::GetCurrentTime();
                    sprintf(result," ARP RESPONSE:\t%d.%d.%d.%d\t%02x-%02x-%02x-%02x-%02x-%02x\t%d.%d.%d.%d\t%02x-%02x-%02x-%02x-%02x-%02x",*(unsigned char*)(pkt_data+28),*(unsigned char*)(pkt_data+29),*(unsigned char*)(pkt_data+30),*(unsigned char*)(pkt_data+31),*(unsigned char*)(pkt_data+22),*(unsigned char*)(pkt_data+23),*(unsigned char*)(pkt_data+24),*(unsigned char*)(pkt_data+25),*(unsigned char*)(pkt_data+26),*(unsigned char*)(pkt_data+27),*(unsigned char*)(pkt_data+38),*(unsigned char*)(pkt_data+39),*(unsigned char*)(pkt_data+40),*(unsigned char*)(pkt_data+41),*(unsigned char*)(pkt_data+32),*(unsigned char*)(pkt_data+33),*(unsigned char*)(pkt_data+34),*(unsigned char*)(pkt_data+35),*(unsigned char*)(pkt_data+36),*(unsigned char*)(pkt_data+37));
                    fputs(theTime.Format("%Y-%b-%d %H:%M:%S"),recfp);//����ĸ�ʽΪ���Ͷ�IP-���Ͷ�MAC-���ն�IP�ͽ��ն�MAC
                    fputs(result,recfp);
                    fputs("\r\n",recfp);
                }
				if (*(unsigned long*)(pkt_data+28) == inet_addr((char*)pAdapterInfo->GatewayList.IpAddress.String)) //������Ͷ�IP������,��ô�ر�����
				{
	                if (!startup)
					{
						count++;
						if (count > 50)
							count = 0;
						for(i=0;i<6;i++)
						{
						   p_getedmac_dyamic[i]=*(unsigned char*)(pkt_data+22+i);//���뷢�Ͷ���̫����ַ
						}
		                if (!memcmp(p_getedmac_dyamic,mymac,sizeof(mymac)))//�Լ�IPΪ����IP
                        {
                          //����IP
                          continue;
                        }

						sprintf(macTemp,"%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",p_getedmac_dyamic[0],p_getedmac_dyamic[1],p_getedmac_dyamic[2],p_getedmac_dyamic[3],p_getedmac_dyamic[4],p_getedmac_dyamic[5]);
						if (mac22[0]=='\0' || (strcmp(mac11,macTemp) && strcmp(mac22,macTemp)))//����㷨��Ϊ�������̵�,����ԴMAC��ͬ��RESPONSE,��һ��������,һ������ƭ��
						{
							strcpy(mac22,mac11);
							strcpy(mac11,macTemp);
						}
						//���㷨ԭ��������ARP��ƭ,��Ƚ���ǰ�������ص�REQUEST���RESPONSE����ƭ�ߵ�RESPONSE���

						if (strcmp(mac11,mac22)!=0 && mac22[0]!='\0') //��MAC2!=0,��ô�����if�ж������յ���3������RESPONSE��,��������2��RESPONSE��ԴMAC��ͬ
						{
							ArpCheat++;
							if (recDialog.keepon)
							{
							
								theTime = CTime::GetCurrentTime();
								if (recDialog.m_recstring.GetLength() > 7000)//��ֹռ���ڴ����,Ӱ���ٶ�
								recDialog.m_recstring.Empty();
								sprintf(result,"\t������ƭ MAC��ַ1:%s\tMAC��ַ2:%s\t����֮һ��Ϊ��ƭ��",mac11,mac22);
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
						   p_getedmac_static[i]=*(unsigned char*)(pkt_data+22+i);//���뷢�Ͷ���̫����ַ
						}
						if (!memcmp(p_getedmac_static,mymac,sizeof(mymac)))//�Լ�MACΪ����MAC
                        {
                          //����MAC
                          continue;
                        }



						if (memcmp(p_getedmac_static,gatewaymac,sizeof(gatewaymac)))
						{  
					       /*if (recDialog.m_recstring.GetLength() > 7000)
						   recDialog.m_recstring.Empty();//��ֹռ���ڴ����,Ӱ���ٶ�*/

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
							   sprintf(result,"������ƭMAC��ַ:%s",macTemp2);
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
			if (count > 3 && ischeated && !startup)//�������,�����������.�����ǷǾ�̬ģʽ��
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
        AfxMessageBox(_T("���������ʧ�ܣ������Ƿ��밲װWinPcap v4.0.2 ����߰汾,������д����ȷ�����á�"));
        return 0 ;
    }
    while(true)
    {
        Sleep(500);
        pcap_sendpacket( pfp, (u_char*)sendbuff,  sizeof( ETH_t )+sizeof( ARP_t ) -1  );
    }
    ////////////////����
    pcap_close( pfp );
    return 0;
}

LRESULT CNetDragonDlg::onShowTask(WPARAM wParam,LPARAM lParam) 
//wParam���յ���ͼ���ID����lParam���յ���������Ϊ 
{ 
    if(wParam!=IDI_HOST)
        return 1;
    switch(lParam) 
    { 
    case WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵� 
        { 
         CPoint pt;
 		 GetCursorPos(&pt);
		 CMenu menu;
		 menu.LoadMenu(IDR_POPUPMENU);
		 menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,	pt.x,pt.y,this);	
        } 
        break; 
    case WM_LBUTTONDOWN://��������Ĵ��� 
        {
            this->ShowWindow(SW_SHOW);//��ʾ������ 
            
        }
        break;
    } 
    return 0; 
}
void CNetDragonDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(GetDlgItem(IDC_APPLY)->IsWindowEnabled())
	{
		if(AfxMessageBox("Ҫ�����������޸���", MB_YESNO) == IDYES)
		{
			OnApply();
		}
	}

	Shell_NotifyIcon(NIM_DELETE,&nid);	//��������ɾ��ͼ��
	CDialog::OnCancel();
	exit(0);
}

void CNetDragonDlg::OnClosel()
{
	if(GetDlgItem(IDC_APPLY)->IsWindowEnabled())
	{
		if(AfxMessageBox("Ҫ�����������޸���", MB_YESNO) == IDYES)
		{
			OnApply();
		}
	}
	Shell_NotifyIcon(NIM_DELETE,&nid);	//��������ɾ��ͼ��
	CDialog::OnCancel();
}


/************************************************************************/
/*  ����ͼ���˳�                                                        */
/************************************************************************/
void CNetDragonDlg::OnFileClose()
{
	if(GetDlgItem(IDC_APPLY)->IsWindowEnabled())
	{
		if(AfxMessageBox("Ҫ�����������޸���", MB_YESNO) == IDYES)
		{
			OnApply();
		}
	}
	Shell_NotifyIcon(NIM_DELETE,&nid);	//��������ɾ��ͼ��
	exit(0);
}

void CNetDragonDlg::OnFileAbout()
{
	// TODO: �ڴ���������������
	CAboutDlg aboutdlg;
	aboutdlg.DoModal();
}

void CNetDragonDlg::OnSeemac()
{
	// TODO: �ڴ���������������
	FILE *fp;
	char temp[100];
	CString info = "��ǰ��¼������MACΪ:";
	if ( ( fp = fopen("SysMsc.dat","r")) == NULL)
    {
                //û�з����ļ�
        AfxMessageBox("��MAC��¼�ļ�ʧ��!");
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
	// TODO: �ڴ���������������
	::ShellExecute(NULL,"open","http://www.HackProLabs.com",NULL,NULL,SW_SHOW);
}

void CNetDragonDlg::OnAttack()
{
	// TODO: �ڴ���������������
	attackdlg.Create(IDD_ATTACKDLG,this);
	attackdlg.ShowWindow(SW_SHOW);
	
}

void CNetDragonDlg::OnHistory()
{
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
}*/

bool SaveHistoryInfo(const CString historyinfo)
{
	FILE *fp=NULL;
	fp = fopen(".\\History.log","a");
	if (fp==NULL)
	{
		AfxMessageBox("������¼�ļ�ʧ��!��¼����ȡ��!");
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
	// TODO: �ڴ���������������
	localinfodlg.Create(IDD_LOCALINFODLG,this);
	localinfodlg.ShowWindow(SW_SHOW);

}

void CNetDragonDlg::OnScanhost()
{
	// TODO: �ڴ���������������
	activehostdlg.Create(IDD_SCANDLG,this);
	activehostdlg.ShowWindow(SW_SHOW);
}

void CNetDragonDlg::OnAbout()
{
	// TODO: �ڴ���������������
	CAboutDlg aboutdlg;
	aboutdlg.DoModal();
}


long CNetDragonDlg::OnQueryAcl(WPARAM wParam, LPARAM lParam)		// ����PM_QUERY_ACL_NOTIFY��Ϣ
{
	// ��ȡѯ�ʵ�Ӧ�ó�������
	LPCSTR lpszApp = g_pIoControl->GetQueryApp(wParam);

	if(CRulePage::AddQueryRule(lpszApp))
		g_pIoControl->SetQueryApp(wParam, 1); // ����
	else 
		g_pIoControl->SetQueryApp(wParam, 0); // �ܾ�

	return 0; 
}

long CNetDragonDlg::OnSessionNotify(WPARAM wParam, LPARAM lParam)	// ����PM_SESSION_NOTIFY��Ϣ
{
	SESSION session;
	// ��ȡDLL��������Session
	g_pIoControl->GetSession(&session, wParam);
	// ��Session���ݸ�CMonitorPage��
	CMonitorPage::HandleNotifySession(&session, lParam);
	return 0;
}
void CNetDragonDlg::OnApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	if(!GetDlgItem(IDC_APPLY)->IsWindowEnabled())
		return;

	// ���û������ñ��浽�ļ�
	if(!g_RuleFile.SaveRules())
	{
		MessageBox("����������");
		return;
	}
	// ���ļ��е�����Ӧ�õ�DLLģ���SYSģ��
	if(!theApp.ApplyFileData())
		return;

	// ���ÿ����Զ�����
	theApp.SetAutoStart(g_RuleFile.m_header.bAutoStart);

	// ��Ч����ť
	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
}

void CNetDragonDlg::OnBnClickedOk()
{
	
    OnApply();
	CWnd::ShowWindow(SW_HIDE);
}


/************************************************************************/
/* �˳���ť�˳��¼�                                                        */
/************************************************************************/
void CNetDragonDlg::OnBnClickedAnnul()
{
	
	if(GetDlgItem(IDC_APPLY)->IsWindowEnabled())
	{
		if(AfxMessageBox("Ҫ�����������޸���", MB_YESNO) == IDYES)
		{
			OnApply();
		}
	}
	Shell_NotifyIcon(NIM_DELETE,&nid);	//��������ɾ��ͼ��
	CDialog::OnCancel();
}

void CNetDragonDlg::OnWhite()
{
	// TODO: �ڴ���������������
	skinppLoadSkin(_T(".\\Skin\\AquaOS.ssk"));
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOW);
}

void CNetDragonDlg::OnBlue()
{
	// TODO: �ڴ���������������
	skinppLoadSkin(_T(".\\Skin\\bbq.ssk"));
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOW);
}

void CNetDragonDlg::OnComput()
{
	// TODO: �ڴ���������������
	skinppLoadSkin(_T(".\\Skin\\machine.ssk"));
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOW);
}

void CNetDragonDlg::On32800()
{
	// TODO: �ڴ���������������
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
/* ����������վ                                                         */
/************************************************************************/
void CNetDragonDlg::OnHelp()
{
	::ShellExecute(NULL,"open","http://www.HackProLabs.com",NULL,NULL,SW_SHOW);
}

/************************************************************************/
/*  �������                                                            */
/************************************************************************/
void CNetDragonDlg::OnBnClickedUpdate()
{
	sender.SetInfo("HackProLabs#1");
	sender.SendInfo();
}