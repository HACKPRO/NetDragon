/************************************************************************/
/* 
	����Ӧ�ó��������Ϊ��
*/
/************************************************************************/
#include "stdafx.h"
#include "Net Dragon.h"
#include "Net DragonDlg.h"
#include "initsock.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNetDragonApp
BEGIN_MESSAGE_MAP(CNetDragonApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


/************************************************************************/
/*  CNetDragonApp ����                                                  */
/************************************************************************/
CNetDragonApp::CNetDragonApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CNetDragonApp ����
CNetDragonApp theApp;
CPRuleFile g_RuleFile;				// ��������ļ�
CPIOControl *g_pIoControl = NULL;	// ����DLL��������
CInitSock initSock;                 // ��ʼ��Winsock��

/************************************************************************/
/*   �������                                                           */
/************************************************************************/
BOOL CNetDragonApp::InitInstance()
{
    if(  CreateMutex(NULL,   true,   "Net Dragon")   !=   NULL   )
    {
        if( GetLastError()   ==   ERROR_ALREADY_EXISTS   )
        {   
            AfxMessageBox("�����Ѿ������ˡ�");
            exit(0);
        }
    }

	// ��֤�Ƿ�װWinPcap
	if (fopen("C:\\WINDOWS\\system32\\wpcap.dll","r")==NULL)
	{
		AfxMessageBox(_T("����:���������WinPcap���밲װWinPcap v4.0.2 ����߰汾��"));
		exit(0) ;
	}

	// ���ع����ļ�
	if(!g_RuleFile.LoadRules())
	{
		AfxMessageBox("���������ļ�����");
		return FALSE;
	}

	// ����DLL I/O���ƶ��󣬼���DLLģ��
	g_pIoControl = new CPIOControl;

	// Ӧ���ļ��е����ݣ�����Ӧ�ò���˹���
	sender.SetServer(g_RuleFile.m_header.szServerIp);
	isServerAllowed = g_RuleFile.m_header.bisServerAllowed;
	ApplyFileData();
	skinppLoadSkin(_T(".\\Skin\\bbq.ssk"));

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("WinHunter in HackPro Labs"));

	CNetDragonDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CNetDragonApp::ApplyFileData()
{
	// ���ù���ģʽ
	g_pIoControl->SetWorkMode(g_RuleFile.m_header.ucLspWorkMode);

	// ����Ӧ�ò�����ļ�
	g_pIoControl->SetRuleFile(&g_RuleFile.m_header, g_RuleFile.m_pLspRules);

	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CNetDragonApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(g_pIoControl != NULL)
	{
		g_pIoControl->SetWorkMode(PF_PASS_ALL);
		g_pIoControl->SetPhoenixInstance(NULL, "");
		delete g_pIoControl;
	}
	
	return CWinApp::ExitInstance();
}

/************************************************************************/
/* ������ϵͳ����                                                       */
/************************************************************************/
BOOL CNetDragonApp::SetAutoStart(BOOL bStart)
{
	// �������Ӽ����ơ��͵��Ӽ��ľ��
	HKEY hRoot = HKEY_LOCAL_MACHINE;
    CHAR *szSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY hKey;

	// ��ָ���Ӽ�
	DWORD dwDisposition = REG_OPENED_EXISTING_KEY;	// ��������ڲ�����
	LONG lRet = ::RegCreateKeyEx(hRoot, szSubKey, 0, NULL, 
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	if(lRet != ERROR_SUCCESS)
		return FALSE;

	if(bStart)
	{
		// �õ���ǰִ���ļ����ļ���������·����
		char szModule[MAX_PATH] ;
		::GetModuleFileNameA(NULL, szModule, MAX_PATH);
		// ����һ���µļ�ֵ�����ü�ֵ����Ϊ�ļ���
		lRet = ::RegSetValueExA(hKey, "NetDragon", 0, REG_SZ, (BYTE*)szModule, strlen(szModule));
	}
	else
	{
		// ɾ��������ļ�ֵ
		lRet = ::RegDeleteValueA(hKey, "NetDragon");
	}

	// �ر��Ӽ����
	::RegCloseKey(hKey);
	return lRet == ERROR_SUCCESS;
}
