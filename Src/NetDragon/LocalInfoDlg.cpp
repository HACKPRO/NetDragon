/************************************************************************/
/* 
	获取当前主机信息模块	Code by Win.Hunter
*/
/************************************************************************/

#include "stdafx.h"
#include "Net Dragon.h"
#include "LocalInfoDlg.h"
#include <Iphlpapi.h>
#include <winsock2.h>

// LocalInfoDlg 对话框

IMPLEMENT_DYNAMIC(LocalInfoDlg, CDialog)

LocalInfoDlg::LocalInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LocalInfoDlg::IDD, pParent)
	, m_localip(_T(""))
	, m_localmac(_T(""))
	, m_gatewayip(_T(""))
	, m_netmask(_T(""))
	, m_gatewaymac(_T(""))
	, m_relatemac(_T(""))
{

}

LocalInfoDlg::~LocalInfoDlg()
{
}

void LocalInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LocalInfoDlg, CDialog)
	ON_STN_CLICKED(IDC_STATIC_LOCALIP, &LocalInfoDlg::OnStnClickedStaticLocalip)
END_MESSAGE_MAP()


// LocalInfoDlg 消息处理程序

void LocalInfoDlg::AddARPInfoToList(CListCtrl& list)
{
	char tempChar;
	DWORD dwListSize = 1;
	DWORD dwRet;
    INFO_ARP		ARPList[500];
	in_addr inaddr;
	int k;
	short temp;
	char szStr[3];
	list.DeleteAllItems();

	dwRet = GetIpNetTable((PMIB_IPNETTABLE)&tempChar, &dwListSize, TRUE);  // 关键函数
	if (dwRet == ERROR_INSUFFICIENT_BUFFER)
	{
		PMIB_IPNETTABLE pIpNetTable = (PMIB_IPNETTABLE)new(char[dwListSize]);
		dwRet = GetIpNetTable(pIpNetTable, &dwListSize, TRUE);
		if (dwRet == ERROR_SUCCESS)
		{
			for (int i=0; i<(int)pIpNetTable->dwNumEntries; i++)
			{
				inaddr.S_un.S_addr = pIpNetTable->table[i].dwAddr;
				strcpy( ARPList[i].szIPAddrStr, inet_ntoa(inaddr) );                 // IP

                //MAC
				strcpy(ARPList[i].szHWAddrStr, "");
				for (k=0; k<6; ++k)
				{
					temp = (short)(*(pIpNetTable->table[i].bPhysAddr + k));
					_itoa(temp, szStr, 16);
					if (strlen(szStr) == 1)	
						strcat(ARPList[i].szHWAddrStr, "0");
					strcat(ARPList[i].szHWAddrStr, szStr);
					if (k<5)	strcat(ARPList[i].szHWAddrStr, "-");         // 加上 - 
				}

				ARPList[i].dwType = pIpNetTable->table[i].dwType;         // Type


				list.InsertItem(i,ARPList[i].szIPAddrStr);
				list.SetItemText(i,1,ARPList[i].szHWAddrStr);
				switch(ARPList[i].dwType) 
				{                         // 根据type的值来转换成字符显示
				case 3:
					list.SetItemText(i,2,"Dynamic");
					break;
				case 4:
					list.SetItemText(i,2,"Static");
					break;
				case 1:
					list.SetItemText(i,2,"Invalid");
				default:
					list.SetItemText(i,2,"Other");
				}
					
			}
		}
		delete pIpNetTable;
	}
}

BOOL LocalInfoDlg::OnInitDialog()
{
	

	// TODO:  在此添加额外的初始化
	LoadLocalInfo();
	
	CListCtrl *list = (CListCtrl*)GetDlgItem(IDC_ARPLIST);// 初始化ARP列表样式
	list->ModifyStyle(NULL,LVS_REPORT);
   
	list->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_GRIDLINES);
    list->InsertColumn(0,"IP");                                   // 每个字段内容
	list->InsertColumn(1,"MAC");
	list->InsertColumn(2,"TYPE");
    list->SetColumnWidth(0,110);                                  // 每个字段宽度
	list->SetColumnWidth(1,146);
	list->SetColumnWidth(2,70);
	AddARPInfoToList(*list); 

	SetDlgItemText(IDC_STATIC_LOCALIP,m_localip);
	SetDlgItemText(IDC_STATIC_GATEWAYIP,m_gatewayip);
	SetDlgItemText(IDC_STATIC_NETMASK,m_netmask);
	SetDlgItemText(IDC_STATIC_LOCALMAC,m_localmac);
	SetDlgItemText(IDC_STATIC_GATEWAYMAC,m_gatewaymac);
	SetDlgItemText(IDC_STATIC_RELATEMAC,m_relatemac);
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

bool LocalInfoDlg::LoadLocalInfo(void)
{
	m_localip = (char*)pAdapterInfo->IpAddressList.IpAddress.String;
	m_netmask = (char*)pAdapterInfo->IpAddressList.IpMask.String;
    m_gatewayip = (char*)pAdapterInfo->GatewayList.IpAddress.String;
	m_gatewaymac.Format("%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",gatewaymac[0],gatewaymac[1],gatewaymac[2],gatewaymac[3],gatewaymac[4],gatewaymac[5]);
	m_localmac.Format("%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",mymac[0],mymac[1],mymac[2],mymac[3],mymac[4],mymac[5]);

	FILE *fp;
	char temp[100];
	
	if ( ( fp = fopen("SysMsc.dat","r")) == NULL)
    {
                //没有发现文件
        m_relatemac = "No Record!";
    }
    else
    {
		fgets(temp,99,fp);
		fclose(fp);
		m_relatemac = temp;
		
	}
   
	return false;
}

void LocalInfoDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	DestroyWindow();
}

void LocalInfoDlg::OnStnClickedStaticLocalip()
{
	// TODO: 在此添加控件通知处理程序代码
}
