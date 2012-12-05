// ARPGuardPage.cpp : ʵ���ļ�


#include "stdafx.h"
#include "Net Dragon.h"
#include "ARPGuardPage.h"
#include "RecordDialog.h"
#include "pcap.h"

// ARPGuardPage �Ի���
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


// ARPGuardPage ��Ϣ�������

BOOL ARPGuardPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	recDialog.Create(IDD_DIALOGRECORD,this );
	dlg2 = this;
	if (!startup)
	    m_list.AddString("��ǰģʽΪ��������ģʽ��");
	else
        m_list.AddString("��ǰģʽΪ��̬��ģʽ��");
	m_list.AddString("���ؼ�¼�ļ��ɹ���");
	//�򿪼�¼������
	FILE *fp;
	char temp[100];
	CString info = "��ǰ��¼������MACΪ:";
	if ( ( fp = fopen("SysMsc.dat","r")) == NULL)
    {
		// û�з����ļ�
        m_list.AddString("�����ؼ�¼�ļ�ʧ��");
    }
    else
    {
		fgets(temp,99,fp);
		fclose(fp);
		info += temp;
		m_list.AddString(info+"��");
	}


    UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void ARPGuardPage::OnBnClickedProtect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	pETH_t pPE;
    pARP_t pPA;
	if (!isResume)
    {
        isResume = !isResume;
        this->GetDlgItem(IDC_PROTECT)->SetWindowText(">>>ֹͣ����<<<");
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
		//m_list.AddString("�������������ص��߳��ѿ�����");
	    
    }
    else
    {
        isResume = !isResume;
        this->GetDlgItem(IDC_PROTECT)->SetWindowTextA("��������");
		//m_list.AddString("�������������ص��߳��ѹرա�");
		
    }
}

UINT ProtectMy(LPVOID lpParam)
{
    pcap_t *pfp;
    if( (pfp = pcap_open_live( pAdapterInfo->AdapterName , 100, 0, 500, error3 ) ) == NULL )
    {
        AfxMessageBox(_T("���������ʧ�ܣ������Ƿ�װ�����µ�WinPcap������д����ȷ�����á�"));
        return 0 ;
    }
    while(isResume)
    {
        Sleep(100);
        pcap_sendpacket( pfp, (u_char*)protectbuff, sizeof( ETH_t )+sizeof( ARP_t ) -1  );
    }
    ////////////////����
    pcap_close( pfp );
    return 0;
}
void ARPGuardPage::OnBnClickedSee()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	recDialog.ShowWindow(SW_SHOW);
}
