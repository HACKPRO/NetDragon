/************************************************************************/
/* 
	������ʼ���ģ��	Code by WinHunter
*/
/************************************************************************/


#include "stdafx.h"
#include "Net Dragon.h"
#include "MonitorPage.h"

#include "RulePage.h"

#include "Net DragonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonitorPage property page

extern CNetDragonApp theApp;

IMPLEMENT_DYNCREATE(CMonitorPage, CPropertyPage)

CMonitorPage::CMonitorPage() : CPropertyPage(CMonitorPage::IDD)
{
	//{{AFX_DATA_INIT(CMonitorPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMonitorPage::~CMonitorPage()
{
}

void CMonitorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonitorPage)
	DDX_Control(pDX, IDC_TREEMONITOR, m_MonitorTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonitorPage, CPropertyPage)
	//{{AFX_MSG_MAP(CMonitorPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREEMONITOR, &CMonitorPage::OnTvnSelchangedTreemonitor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonitorPage message handlers

BOOL CMonitorPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CMonitorPage::HandleNotifySession(SESSION *pSession, int nCode)
{
	// �õ���ǰCMonitorPage���ָ�루ע�⣬HandleNotifySession��һ����̬������
	CMonitorPage *pThis = &(((CNetDragonDlg*)theApp.m_pMainWnd)->m_MonitorPage);

	// һ���Ự�����Ըı���
	if(nCode == CODE_CHANGE_SESSION)	
		pThis->AddASession(pSession);
	// һ���Ự��ɾ��
	else if(nCode == CODE_DELETE_SESSION)
		pThis->DeleteASession(pSession, FALSE);
	// һ��Ӧ�ó����˳�������Ҫɾ����Ӧ�ó�������лỰ
	else if(nCode == CODE_APP_EXIT)
		pThis->DeleteASession(pSession, TRUE);
}

void CMonitorPage::AddASession(SESSION *pSession)
{
	TRACE(" AddASession... ");

	// ���Ȳ鿴������Ӧ�ó������û�У��Ͳ���һ���µ�Ӧ�ó�����
	// Ҫ�ڴ�Ӧ�ó���������ӻỰ
	HTREEITEM hAppItem = FindAppItem(pSession->szPathName);
	if(hAppItem == NULL) 
	{
		hAppItem = m_MonitorTree.InsertItem(pSession->szPathName);
	}

	// ͨ��SESSION�ṹ����������ʾ���ı�
	CString sText = BuildSessionText(pSession);

	// ��Ӧ�ó������£������׽��־���鿴�˻Ự�Ƿ��Ѿ����ڣ�
	// ������ڣ�������������ı�����������ڣ�Ҫ����һ���µ�����
	HTREEITEM hSessionItem = FindSessionItem(hAppItem, pSession);
	if(hSessionItem != NULL)
	{
		m_MonitorTree.SetItemText(hSessionItem, sText);
	}
	else
	{	
		hSessionItem = m_MonitorTree.InsertItem(sText,hAppItem);
		m_MonitorTree.SetItemData(hSessionItem, pSession->s);
	}
}

HTREEITEM CMonitorPage::FindAppItem(CHAR *pszPathName)
{
	// ��������Ӧ�ó��������ָ��Ӧ�ó����Ƿ����
	HTREEITEM hAppItem = m_MonitorTree.GetNextItem(TVI_ROOT, TVGN_CHILD);
	while(hAppItem != NULL)
	{
		if(m_MonitorTree.GetItemText(hAppItem).CompareNoCase(pszPathName) == 0)
			return hAppItem; // ���ڣ���������

		hAppItem = m_MonitorTree.GetNextItem(hAppItem, TVGN_NEXT);
	}
	return NULL;
}

HTREEITEM CMonitorPage::FindSessionItem(HTREEITEM hAppItem, SESSION *pSession)
{
	// �������лỰ�����ָ���Ự�Ƿ����
	HTREEITEM hSessionItem = m_MonitorTree.GetNextItem(hAppItem, TVGN_CHILD);
	while(hSessionItem != NULL)
	{
		if(pSession->s == m_MonitorTree.GetItemData(hSessionItem))
			return hSessionItem; // ���ڣ���������

		hSessionItem = m_MonitorTree.GetNextItem(hSessionItem, TVGN_NEXT);
	}
	return NULL;
}

CString CMonitorPage::BuildSessionText(SESSION *pSession)
{
	CString sText;

	CString sServType, sLocal, sRemote, sDirection;

	// ����IP��ַ
	BYTE *pByte = (BYTE *)&pSession->ulLocalIP; // ע�⣬�����IP��ַ�������ֽ�˳��
	sLocal.Format("%d.%d.%d.%d��%d", pByte[0], pByte[1], pByte[2], pByte[3], pSession->usLocalPort);

	// Զ��IP��ַ
	pByte = (BYTE *)&pSession->ulRemoteIP;
	sRemote.Format("%d.%d.%d.%d��%d", pByte[0], pByte[1], pByte[2], pByte[3], pSession->usRemotePort);

	// ��������
	sServType = "����";
	switch(pSession->nProtocol)
	{	
	case RULE_SERVICE_TYPE_ALL:
		sServType.Format("����");
		break;
	case RULE_SERVICE_TYPE_TCP:
		sServType.Format("TCP");
		break;
	case RULE_SERVICE_TYPE_UDP:
		sServType.Format("UDP");
		break;
	case RULE_SERVICE_TYPE_FTP:
		sServType.Format("FTP");
		break;
	case RULE_SERVICE_TYPE_TELNET:
		sServType.Format("TELNET");
		break;
	case RULE_SERVICE_TYPE_HTTP:
		sServType.Format("HTTP");
		break;
	case RULE_SERVICE_TYPE_NNTP:
		sServType.Format("NNTP");
		break;
	case RULE_SERVICE_TYPE_POP3:
		sServType.Format("POP3");
		break;
	case RULE_SERVICE_TYPE_SMTP:
		sServType.Format("SMTP");
		break;
	}

	// ����
	switch(pSession->ucDirection)
	{
	case RULE_DIRECTION_IN:
		sDirection = "<����";
		break;
	case RULE_DIRECTION_OUT:
		sDirection = "����>";
		break;
	case RULE_DIRECTION_IN_OUT:
		sDirection = "<����>";
		break;
	default:
		sDirection = "����";
	}

	sText.Format(" %s Э��	    ��%s�� %s ��%s�� ", sServType, sLocal, sDirection, sRemote);
	return sText;
}


void CMonitorPage::DeleteASession(SESSION *pSession, BOOL bAppExit)
{
	TRACE(" DeleteASession... ");

	HTREEITEM hAppItem = FindAppItem(pSession->szPathName);
	if(hAppItem != NULL)
	{
		if(bAppExit) // Ӧ�ó����˳���ɾ������Ӧ�ó������������ĻỰ���
		{
			m_MonitorTree.DeleteItem(hAppItem);
		}
		else		// ���Ựɾ������Ӧ�ó����������ҵ�����Ự����֮ɾ��
		{
			HTREEITEM hSessionItem = FindSessionItem(hAppItem, pSession);
			if(hSessionItem != NULL)
			{	
				m_MonitorTree.DeleteItem(hSessionItem);
			}
			// û��Session�����ˣ���Ӧ�ó�����Ҳɾ��
			if(m_MonitorTree.GetNextItem(hAppItem, TVGN_CHILD) == NULL) 
				m_MonitorTree.DeleteItem(hAppItem);
		}
	}
}

void CMonitorPage::OnTvnSelchangedTreemonitor(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
