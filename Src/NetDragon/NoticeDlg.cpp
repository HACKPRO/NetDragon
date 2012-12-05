/************************************************************************/
/* 
	网卡配置        Code by Win.Hunter
*/
/************************************************************************/

#include "stdafx.h"
#include "Net Dragon.h"
#include "NoticeDlg.h"
#include <Iphlpapi.h>
#include <winsock2.h>

// CNoticeDlg 对话框

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
/*  配置网卡适配器                                                      */
/************************************************************************/
void CNoticeDlg::SetAdapter(void)
{
	ULONG ulLen = 0;
    unsigned int i=0;
    // 为适配器结构申请内存
    ::GetAdaptersInfo(pAdapterInfo,&ulLen);
    pAdapterInfo = (PIP_ADAPTER_INFO)::GlobalAlloc(GPTR, ulLen);

    // 取得本地适配器结构信息
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
/*  一般不能在构造函数里调用成员函数。所以重载oninit                    */
/************************************************************************/
BOOL CNoticeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem(IDOK)->EnableWindow(false);//显示确定按钮无效
    SetAdapter();//获得网卡信息
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


/************************************************************************/
/*  传递由网卡获得的IP地址和掩码                                        */
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
	bool isAdapterOk = true;	// 修复不重复提示网卡网络

	//第一次打开的处理 SysMsc.dat
    if ((fp=fopen("SysMsc.dat","r"))==NULL)
    {
		AfxMessageBox("感谢您的第一次使用,请检查静态绑定ARP项里的网关MAC真实性(一般情况下是正确的,除非您正被攻击)\r\n如果错误,请自行更改．");
		
		ULONG macAddLentemp=6;
        unsigned char tempmac[6];
        memset(tempmac, 0xff, sizeof(gatewaymac));

		// 检查网卡是否可用
		if (!(SendARP(inet_addr((char*)pAdapterInfo->GatewayList.IpAddress.String), (IPAddr) NULL,(PULONG) tempmac, &macAddLentemp) == NO_ERROR))
		{
		    AfxMessageBox("警告:网卡异常,请确保网卡启用,网络已连接!");
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


	//发送ARPREQUEST，获得网关MAC
	ULONG macAddLen=6;
	memset(gatewaymac, 0xff, sizeof(gatewaymac));
	if ( isAdapterOk  && !(SendARP(inet_addr((char*)pAdapterInfo->GatewayList.IpAddress.String), (IPAddr) NULL,(PULONG) gatewaymac, &macAddLen) == NO_ERROR))
	{
		AfxMessageBox("警告:网卡异常,请确保网卡启用,网络已连接!");
	}
	
	SendARP(inet_addr((char*)pAdapterInfo->IpAddressList.IpAddress.String), (IPAddr) NULL,(PULONG) mymac, &macAddLen);

	GetDlgItem(IDOK)->EnableWindow(true);
	((CComboBox*)GetDlgItem(IDC_ADAPTER))->EnableWindow(false);
}

/************************************************************************/
/* 退出程序                                                             */
/************************************************************************/
void CNoticeDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	exit(0);
}

/************************************************************************/
/* 确认网卡设置，进入主程序                                             */
/************************************************************************/
void CNoticeDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialog::OnOK();
}

/************************************************************************/
/* 使用静态MAC绑定配置，进入主程序                                      */
/************************************************************************/
void CNoticeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
 	if (!(m_staticmac.IsEmpty()))
    {
		FILE *fp=NULL;
	    fp = fopen("SysMsc.dat","w");
	    fputs(m_staticmac,fp);
	    fclose(fp);
	}
    //选择静态绑定时的处理
	if (startup)
	{
        sscanf(m_staticmac,"%x-%x-%x-%x-%x-%x",&gatewaymac[0],&gatewaymac[1],&gatewaymac[2],&gatewaymac[3],&gatewaymac[4],&gatewaymac[5]);
        StaticARP((unsigned char*)pAdapterInfo->GatewayList.IpAddress.String, &gatewaymac[0]);
	}
    OnOK();
}


void CNoticeDlg::OnBnClickedRadiostatic()
{
	// TODO: 在此添加控件通知处理程序代码
	startup=true;
	this->GetDlgItem(IDC_EDITMAC)->ShowWindow(SW_SHOW);
	this->GetDlgItem(IDC_STATICF)->ShowWindow(SW_SHOW);
	this->GetDlgItem(IDC_BUTTONCANCELSTATIC)->ShowWindow(SW_SHOW);
}

/************************************************************************/
/* 取消静态MAC，使用网卡配置                                            */
/************************************************************************/
void CNoticeDlg::OnBnClickedButtoncancelstatic()
{
	// TODO: 在此添加控件通知处理程序代码
	startup=false;
	this->GetDlgItem(IDC_EDITMAC)->ShowWindow(SW_HIDE);
	this->GetDlgItem(IDC_STATICF)->ShowWindow(SW_HIDE);
	this->GetDlgItem(IDC_BUTTONCANCELSTATIC)->ShowWindow(SW_HIDE);
}

void CNoticeDlg::OnEnChangeEditnotice()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
