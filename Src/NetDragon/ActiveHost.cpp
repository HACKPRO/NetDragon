/************************************************************************/
/* 
	��������ɨ��ģ��	Code by Win.Hunter
*/
/************************************************************************/

#include "stdafx.h"
#include "Net Dragon.h"

#include <winsock2.h>
#include <Iphlpapi.h>
#include "ActiveHost.h"



// CActiveHost �Ի���


IMPLEMENT_DYNAMIC(CActiveHost, CDialog)

CActiveHost::CActiveHost(CWnd* pParent /*=NULL*/)
	: CDialog(CActiveHost::IDD, pParent)
{

}

CActiveHost::~CActiveHost()
{
}

void CActiveHost::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOSTLIST, m_Listctrl);
	DDX_Control(pDX, IDC_PROGRESS, m_progressScan);
}


BEGIN_MESSAGE_MAP(CActiveHost, CDialog)
	ON_BN_CLICKED(IDC_SCAN, &CActiveHost::OnBnClickedScan)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_HOSTLIST, &CActiveHost::OnLvnItemchangedHostlist)
END_MESSAGE_MAP()


// CActiveHost ��Ϣ�������

//void CActiveHost::OnCancel()
//{
//	DestroyWindow();
//}

void CActiveHost::OnBnClickedScan()
{
	m_thread.pDlg=this;
	m_thread.IsUseForScan = true;
	// ����������õ������ڵ���������
	// ������������ = ~ MASK - 1
	uHostNum = ~ htonl(inet_addr(m_thread.Submask)) - 1;
	if (uHostNum > 2000)//��ֹ������Ϊ255.0.0.0֮���
		uHostNum = 2000;


	//��������ʼ��
	m_progressScan.SetRange32(0,uHostNum);
	m_progressScan.SetStep(1);
	// Delete all of the items from the list view control.
	int nCount = m_Listctrl.GetItemCount();
    for (int i=0;i < nCount;i++)
    {
        m_Listctrl.DeleteItem(0);
    }
	//��ʼɨ�����߳�
    AfxBeginThread((AFX_THREADPROC)EnumLanHost,&m_thread);
	
}






BOOL CActiveHost::OnInitDialog()
{
	CDialog::OnInitDialog();

	static struct
		{
			LPSTR psztext;
			int ui;
		}columns[]={
			    _T("���"),LVCFMT_CENTER,
		    	_T("IP��ַ"),LVCFMT_CENTER,
                _T("MAC��ַ"),LVCFMT_CENTER,
				_T("������"),LVCFMT_CENTER,
				};
           		    
		m_Listctrl.InsertColumn(0,columns[0].psztext,columns[0].ui,45);
		m_Listctrl.InsertColumn(1,columns[1].psztext,columns[1].ui,120);
		m_Listctrl.InsertColumn(2,columns[2].psztext,columns[2].ui,130);
		m_Listctrl.InsertColumn(3,columns[3].psztext,columns[3].ui,120);
		m_Listctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CActiveHost::AddNewParse(char MACAddr[],char IPAddr[],char Name[])
{
	LVITEM			lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;
	lvi.iItem = m_Listctrl.GetItemCount();
	lvi.iSubItem = 0;
	lvi.pszText = "";
	lvi.cchTextMax = 64;
	m_Listctrl.InsertItem(&lvi);
	CString str;
	
	str.Format("%d",m_Listctrl.GetItemCount());
	m_Listctrl.SetItemText(lvi.iItem,0,str);
	str.Empty();
	
	//IP
	str.Format("%-15s",IPAddr);
	m_Listctrl.SetItemText(lvi.iItem,1,str);
	str.Empty();
	//MAC
	str.Format("%-15s",MACAddr);
	m_Listctrl.SetItemText(lvi.iItem,2,str);
	str.Empty();
	//Name
	str.Format("%-15s",Name);
	m_Listctrl.SetItemText(lvi.iItem,3,str);
	str.Empty();
	
	m_Listctrl.EnsureVisible(lvi.iItem,true);	
	m_Listctrl.Update(lvi.iItem);
}
void CActiveHost::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	DestroyWindow();
}

void CActiveHost::OnLvnItemchangedHostlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
