/************************************************************************/
/* 
	定义应用程序的类行为。
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
/*  CNetDragonApp 构造                                                  */
/************************************************************************/
CNetDragonApp::CNetDragonApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CNetDragonApp 对象
CNetDragonApp theApp;
CPRuleFile g_RuleFile;				// 管理规则文件
CPIOControl *g_pIoControl = NULL;	// 管理DLL共享数据
CInitSock initSock;                 // 初始化Winsock库

/************************************************************************/
/*   程序入口                                                           */
/************************************************************************/
BOOL CNetDragonApp::InitInstance()
{
    if(  CreateMutex(NULL,   true,   "Net Dragon")   !=   NULL   )
    {
        if( GetLastError()   ==   ERROR_ALREADY_EXISTS   )
        {   
            AfxMessageBox("程序已经启动了。");
            exit(0);
        }
    }

	// 验证是否安装WinPcap
	if (fopen("C:\\WINDOWS\\system32\\wpcap.dll","r")==NULL)
	{
		AfxMessageBox(_T("警告:本软件依赖WinPcap，请安装WinPcap v4.0.2 或更高版本。"));
		exit(0) ;
	}

	// 加载过滤文件
	if(!g_RuleFile.LoadRules())
	{
		AfxMessageBox("加载配置文件出错！");
		return FALSE;
	}

	// 创建DLL I/O控制对象，加载DLL模块
	g_pIoControl = new CPIOControl;

	// 应用文件中的数据，设置应用层过滤规则
	sender.SetServer(g_RuleFile.m_header.szServerIp);
	isServerAllowed = g_RuleFile.m_header.bisServerAllowed;
	ApplyFileData();
	skinppLoadSkin(_T(".\\Skin\\bbq.ssk"));

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("WinHunter in HackPro Labs"));

	CNetDragonDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//  对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CNetDragonApp::ApplyFileData()
{
	// 设置工作模式
	g_pIoControl->SetWorkMode(g_RuleFile.m_header.ucLspWorkMode);

	// 设置应用层规则文件
	g_pIoControl->SetRuleFile(&g_RuleFile.m_header, g_RuleFile.m_pLspRules);

	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CNetDragonApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(g_pIoControl != NULL)
	{
		g_pIoControl->SetWorkMode(PF_PASS_ALL);
		g_pIoControl->SetPhoenixInstance(NULL, "");
		delete g_pIoControl;
	}
	
	return CWinApp::ExitInstance();
}

/************************************************************************/
/* 设置随系统启动                                                       */
/************************************************************************/
BOOL CNetDragonApp::SetAutoStart(BOOL bStart)
{
	// 根键、子键名称、和到子键的句柄
	HKEY hRoot = HKEY_LOCAL_MACHINE;
    CHAR *szSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY hKey;

	// 打开指定子键
	DWORD dwDisposition = REG_OPENED_EXISTING_KEY;	// 如果不存在不创建
	LONG lRet = ::RegCreateKeyEx(hRoot, szSubKey, 0, NULL, 
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	if(lRet != ERROR_SUCCESS)
		return FALSE;

	if(bStart)
	{
		// 得到当前执行文件的文件名（包含路径）
		char szModule[MAX_PATH] ;
		::GetModuleFileNameA(NULL, szModule, MAX_PATH);
		// 创建一个新的键值，设置键值数据为文件名
		lRet = ::RegSetValueExA(hKey, "NetDragon", 0, REG_SZ, (BYTE*)szModule, strlen(szModule));
	}
	else
	{
		// 删除本程序的键值
		lRet = ::RegDeleteValueA(hKey, "NetDragon");
	}

	// 关闭子键句柄
	::RegCloseKey(hKey);
	return lRet == ERROR_SUCCESS;
}
