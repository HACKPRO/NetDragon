/************************************************************************/
/* 
	��������        Code by Win.Hunter
*/
/************************************************************************/

#include "stdafx.h"
#include "Net Dragon.h"
#include "NoticeDlg.h"
#include <Iphlpapi.h>
#include <winsock2.h>

// CNoticeDlg �Ի���

IMPLEMENT_DYNAMIC(CNoticeDlg, CDialog)

CNoticeDlg::CNoticeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNoticeDlg::IDD, pParent)
	, m_notice(_T(""))
	
	, m_staticmac(_T(""))
{

}

CNoticeDlg::~CNoticeDlg()
{
}

void CNoticeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITNOTICE, m_notice);
	DDX_Control(pDX, IDC_ADAPTER, m_adapter);

	DDX_Text(pDX, IDC_EDITMAC, m_staticmac);
	DDV_MaxChars(pDX, m_staticmac, 17);
}


BEGIN_MESSAGE_MAP(CNoticeDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_ADAPTER, &CNoticeDlg::OnCbnSelchangeAdapter)
	ON_BN_CLICKED(IDOK, &CNoticeDlg::OnBnClickedOk)
	
	
	ON_BN_CLICKED(IDC_RADIOSTATIC, &CNoticeDlg::OnBnClickedRadiostatic)
	ON_BN_CLICKED(IDC_BUTTONCANCELSTATIC, &CNoticeDlg::OnBnClickedButtoncancelstatic)
	ON_EN_CHANGE(IDC_EDITNOTICE, &CNoticeDlg::OnEnChangeEditnotice)
END_MESSAGE_MAP()




/************************************************************************/
/*  ��������������                                                      */
/************************************************************************/
void CNoticeDlg::SetAdapter(void)
{
	ULONG ulLen = 0;
    unsigned int i=0;
    // Ϊ�������ṹ�����ڴ�
    ::GetAdaptersInfo(pAdapterInfo,&ulLen);
    pAdapterInfo = (PIP_ADAPTER_INFO)::GlobalAlloc(GPTR, ulLen);

    // ȡ�ñ����������ṹ��Ϣ
    if(::GetAdaptersInfo(pAdapterInfo,&ulLen) ==  ERROR_SUCCESS)
    {
        char adaptername[100];
        pOrgAdapterInfo = pAdapterInfo;
        while(pAdapterInfo != NULL)
        {
            m_adapter.InsertString(i,pAdapterInfo->Description );
            strcpy(adaptername,"\\Device\\NPF_");
            strcat(adaptername,pAdapterInfo->AdapterName);
            strcpy(pAdapterInfo->AdapterName,adaptername);
            pAdapterInfo = pAdapterInfo->Next;
            i++;
        }
    }
}

/************************************************************************/
/*  һ�㲻���ڹ��캯������ó�Ա��������������oninit                    */
/************************************************************************/
BOOL CNoticeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem(IDOK)->EnableWindow(false);//��ʾȷ����ť��Ч
    SetAdapter();//���������Ϣ
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


/************************************************************************/
/*  ������������õ�IP��ַ������                                        */
/************************************************************************/
void CNoticeDlg::OnCbnSelchangeAdapter()
{
    CString m_adaptername;
    m_adapter.GetLBText(m_adapter.GetCurSel(),m_adaptername);
    pAdapterInfo = pOrgAdapterInfo;
    while(pAdapterInfo->Description != m_adaptername)
        pAdapterInfo = pAdapterInfo->Next;

    FILE *fp;
	char temp[100];
	bool isAdapterOk = true;	// �޸����ظ���ʾ��������

	//��һ�δ򿪵Ĵ��� SysMsc.dat
    if ((fp=fopen("SysMsc.dat","r"))==NULL)
    {
		AfxMessageBox("��л���ĵ�һ��ʹ��,���龲̬��ARP���������MAC��ʵ��(һ�����������ȷ��,��������������)\r\n�������,�����и��ģ�");
		
		ULONG macAddLentemp=6;
        unsigned char tempmac[6];
        memset(tempmac, 0xff, sizeof(gatewaymac));

		// ��������Ƿ����
		if (!(SendARP(inet_addr((char*)pAdapterInfo->GatewayList.IpAddress.String), (IPAddr) NULL,(PULONG) tempmac, &macAddLentemp) == NO_ERROR))
		{
		    AfxMessageBox("����:�����쳣,��ȷ����������,����������!");
			isAdapterOk = false;
		}
		m_staticmac.Format("%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",tempmac[0],tempmac[1],tempmac[2],tempmac[3],tempmac[4],tempmac[5]);
    }
	if (fp != NULL)
	{
        fgets(temp,99,fp);//adapter name
	    m_staticmac = (LPSTR) temp;
	}
	UpdateData(false);
	strcpy( m_thread.Ipaddr,(char*)pAdapterInfo->GatewayList.IpAddress.String);
	strcpy( m_thread.Submask,(char*)pAdapterInfo->IpAddressList.IpMask.String);


	//����ARPREQUEST���������MAC
	ULONG macAddLen=6;
	memset(gatewaymac, 0xff, sizeof(gatewaymac));
	if ( isAdapterOk  && !(SendARP(inet_addr((char*)pAdapterInfo->GatewayList.IpAddress.String), (IPAddr) NULL,(PULONG) gatewaymac, &macAddLen) == NO_ERROR))
	{
		AfxMessageBox("����:�����쳣,��ȷ����������,����������!");
	}
	
	SendARP(inet_addr((char*)pAdapterInfo->IpAddressList.IpAddress.String), (IPAddr) NULL,(PULONG) mymac, &macAddLen);

	GetDlgItem(IDOK)->EnableWindow(true);
	((CComboBox*)GetDlgItem(IDC_ADAPTER))->EnableWindow(false);
}

/************************************************************************/
/* �˳�����                                                             */
/************************************************************************/
void CNoticeDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	exit(0);
}

/************************************************************************/
/* ȷ���������ã�����������                                             */
/************************************************************************/
void CNoticeDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	CDialog::OnOK();
}

/************************************************************************/
/* ʹ�þ�̬MAC�����ã�����������                                      */
/************************************************************************/
void CNoticeDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
 	if (!(m_staticmac.IsEmpty()))
    {
		FILE *fp=NULL;
	    fp = fopen("SysMsc.dat","w");
	    fputs(m_staticmac,fp);
	    fclose(fp);
	}
    //ѡ��̬��ʱ�Ĵ���
	if (startup)
	{
        sscanf(m_staticmac,"%x-%x-%x-%x-%x-%x",&gatewaymac[0],&gatewaymac[1],&gatewaymac[2],&gatewaymac[3],&gatewaymac[4],&gatewaymac[5]);
        StaticARP((unsigned char*)pAdapterInfo->GatewayList.IpAddress.String, &gatewaymac[0]);
	}
    OnOK();
}


void CNoticeDlg::OnBnClickedRadiostatic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	startup=true;
	this->GetDlgItem(IDC_EDITMAC)->ShowWindow(SW_SHOW);
	this->GetDlgItem(IDC_STATICF)->ShowWindow(SW_SHOW);
	this->GetDlgItem(IDC_BUTTONCANCELSTATIC)->ShowWindow(SW_SHOW);
}

/************************************************************************/
/* ȡ����̬MAC��ʹ����������                                            */
/************************************************************************/
void CNoticeDlg::OnBnClickedButtoncancelstatic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	startup=false;
	this->GetDlgItem(IDC_EDITMAC)->ShowWindow(SW_HIDE);
	this->GetDlgItem(IDC_STATICF)->ShowWindow(SW_HIDE);
	this->GetDlgItem(IDC_BUTTONCANCELSTATIC)->ShowWindow(SW_HIDE);
}

void CNoticeDlg::OnEnChangeEditnotice()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
