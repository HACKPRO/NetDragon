
#include "stdafx.h"
#include "Net Dragon.h"
#include "SyssetPage.h"
#include "SetServerIpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSyssetPage *sysDlg;
/////////////////////////////////////////////////////////////////////////////
// CSyssetPage property page

IMPLEMENT_DYNCREATE(CSyssetPage, CPropertyPage)

CSyssetPage::CSyssetPage() : CPropertyPage(CSyssetPage::IDD)
{
	//{{AFX_DATA_INIT(CSyssetPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_brush.CreateSolidBrush(RGB(255,255,0));
}

CSyssetPage::~CSyssetPage()
{
}

void CSyssetPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSyssetPage)
	DDX_Control(pDX, IDC_AUTOSTART, m_AutoStart);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ISSERVERSET, m_setServer);
}


BEGIN_MESSAGE_MAP(CSyssetPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSyssetPage)
	ON_BN_CLICKED(IDC_INSTALL, OnInstall)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_AUTOSTART, OnAutostart)
	ON_BN_CLICKED(IDC_PASS_ALL, OnPassAll)
	ON_BN_CLICKED(IDC_QUERY_ALL, OnQueryAll)
	ON_BN_CLICKED(IDC_DENY_ALL, OnDenyAll)

	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SETSERVER, &CSyssetPage::OnBnClickedSetserver)
	ON_BN_CLICKED(IDC_ISSERVERSET, &CSyssetPage::OnBnClickedIsserverset)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSyssetPage message handlers

BOOL CSyssetPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
    sysDlg = this;
	// ���ÿ����Զ�������ѡ��
	if(g_RuleFile.m_header.bAutoStart)
		((CButton*)GetDlgItem(IDC_AUTOSTART))->SetCheck(1);

	if(g_RuleFile.m_header.bisServerAllowed)
		((CButton*)GetDlgItem(IDC_ISSERVERSET))->SetCheck(1);

	// �û��㹤��ģʽ���õ�ѡ��
	switch(g_RuleFile.m_header.ucLspWorkMode)
	{
	case PF_PASS_ALL:
		((CButton*)GetDlgItem(IDC_PASS_ALL))->SetCheck(1);
		break;
	case PF_QUERY_ALL:
		((CButton*)GetDlgItem(IDC_QUERY_ALL))->SetCheck(1);
		break;
	case PF_DENY_ALL:
		((CButton*)GetDlgItem(IDC_DENY_ALL))->SetCheck(1);
		break;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSyssetPage::OnInstall()				// �û����"��װ"��ť
{
	CHAR szPathName[256];
	CHAR* p;

	// ע�⣬��װLSP��Ҫʹ������DLL·���������Ļ���CPIOControl���ڼ���DLLʱҲӦʹ��
	// ����·��������CPIOControl����ص�DLL���ܺ���ΪLSP��DLL�����ڴ�
	if(::GetFullPathName(PHOENIX_SERVICE_DLL_NAME, 256, szPathName, &p) != 0)
	{
		//ת��ΪUNICODE��
		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, szPathName, -1, NULL, 0);
	    wchar_t *pwText;
	    pwText = new wchar_t[dwNum];
	    if(!pwText)
	    {
	      delete []pwText;
          MessageBox(" UNICODEת��ʧ�ܣ�");
	      return;
	    }
	    MultiByteToWideChar (CP_ACP, 0, szPathName, -1, pwText, dwNum);// ��ʼת��

		if(InstallProvider(pwText))
		{
			MessageBox(" Ӧ�ò���˰�װ�ɹ���"); 
			return;
		}
	}
	MessageBox(" Ӧ�ò���˰�װʧ�ܣ�"); 
}

void CSyssetPage::OnRemove()				// �û����"ж��"��ť
{	
	if(RemoveProvider())
		MessageBox(" Ӧ�ò����ж�سɹ���");
	else
		MessageBox(" Ӧ�ò����ж��ʧ�ܣ�");
}


void CSyssetPage::OnAutostart()				// 	�û����"�����Զ�����"��ѡ�� 
{
	BOOL bCheck = m_AutoStart.GetCheck();
	g_RuleFile.m_header.bAutoStart = bCheck;

	// ��Ч���Ի����Ӧ�ð�ť
	GetOwner()->GetOwner()->GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
}

void CSyssetPage::OnPassAll()				// �û�����û����µġ��������С���ѡ��
{
	SetLspWorkMode(PF_PASS_ALL);
}

void CSyssetPage::OnQueryAll()				// �û�����û����µġ�ѯ�ʡ� ��ѡ��
{
	SetLspWorkMode(PF_QUERY_ALL);
}

void CSyssetPage::OnDenyAll()				// �û�����û����µġ��ܾ����С� ��ѡ��
{
	SetLspWorkMode(PF_DENY_ALL);
}





void CSyssetPage::SetLspWorkMode(int nWorkMode)
{
	g_RuleFile.m_header.ucLspWorkMode = nWorkMode;
	// ��Ч���Ի����Ӧ�ð�ť
	GetOwner()->GetOwner()->GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
}





void CSyssetPage::OnBnClickedSetserver()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	setServer.Create(IDD_SERVERIP,this);
	setServer.ShowWindow(SW_SHOW);
}

void CSyssetPage::OnBnClickedIsserverset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!(strcmp("",g_RuleFile.m_header.szServerIp)))
	{
		AfxMessageBox("��δ���÷�����IP����������!");
        setServer.Create(IDD_SERVERIP,this);
	    setServer.ShowWindow(SW_SHOW);
        ((CButton*)GetDlgItem(IDC_ISSERVERSET))->SetCheck(0);
		return;
	}
    BOOL bCheck = m_setServer.GetCheck();
	g_RuleFile.m_header.bisServerAllowed = bCheck;
	isServerAllowed = bCheck;
	GetOwner()->GetOwner()->GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
}

HBRUSH CSyssetPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������

	return hbr;
}
