// stdafx.cpp : 只包括标准包含文件的源文件
// Net Dragon.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

//全局变量
struct thread m_thread;
Sender sender;
PIP_ADAPTER_INFO pAdapterInfo = NULL;
PIP_ADAPTER_INFO pOrgAdapterInfo = NULL;
CString attackip;
unsigned char gatewaymac[6];
unsigned char mymac[6];
bool startup = false;
bool isServerAllowed = false;

int  uHostNum;
unsigned char CompareMacglobal[6];

// 得到网段内所有主机的信息，主线程
// 入口参数: thread结构，包含了IP,MASK,还有用于与线程通信的窗口指针
UINT EnumLanHost(LPVOID pParam)
{
	struct thread *threada=(struct thread*)pParam;
    unsigned int uHostByte; // 主机位
	int i;
	ULONG uMacLength = 6;

	
	CActiveHost *dlg = (CActiveHost*)threada->pDlg;
	
	
	
	WarningDlg *dlgw = (WarningDlg*)threada->pDlgw;
	
	// 两个临时变量,存放IP与MAC
	char TempIpAddr[4 * 4];
	char MACAddr[40];
	
	CWinThread *pT[2001];
	
	
    PLAN_HOST_INFO pLanHostInfo, pNextHostInfo; // 永远指向链表接尾部
    HANDLE hThread[2001]; // 线程数组指针
    
   
    uHostByte = htonl(inet_addr(threada->Ipaddr)) & 0xffffff00;// 由IP地址得到主机位
    // 初始化链表
	pLanHostInfo = new LAN_HOST_INFO;
	memset(pLanHostInfo, 0, sizeof(LAN_HOST_INFO));
	pLanHostInfo->prev = NULL;
    // 开始进行多线程ARP扫描,创建uHostNum个线程扫描
	// Scan Range: 1 ~ uHostNum
	for (i = 0, uHostByte ++; i < uHostNum; i ++, uHostByte ++)
	{
		// 构造IP地址
		memset(TempIpAddr, 0, strlen(TempIpAddr));
		sprintf(TempIpAddr, "%d.%d.%d.%d", 
			(uHostByte & 0xff000000) >> 0x18,//取出IP位后右移24位
			(uHostByte & 0x00ff0000) >> 0x10,//取出IP位后右移16位
			(uHostByte & 0x0000ff00) >> 0x08,//取出IP位后右移8位
			(uHostByte & 0x000000ff));//取出最后一个IP位，初始为0，以后每次加1
		// 构造链表
		pNextHostInfo = new LAN_HOST_INFO;
		memset(pNextHostInfo, 0, sizeof(LAN_HOST_INFO));
		memcpy(pLanHostInfo->IpAddr, TempIpAddr, sizeof(TempIpAddr));
		pLanHostInfo->next = pNextHostInfo;
		pNextHostInfo->prev = pLanHostInfo;
		pNextHostInfo->next = NULL;
		if(threada->IsUseForScan)
		{
			dlg->m_progressScan.StepIt();
		}
		else
		{
			dlgw->m_progressctrl.StepIt();
		}

        pT[i] = AfxBeginThread((AFX_THREADPROC)scan_lan,pLanHostInfo);
		hThread[i]=pT[i]->m_hThread;


		pLanHostInfo = pLanHostInfo->next;

		Sleep(2); // 等待参数传递完毕，再重新赋值
	}
	hThread[i]=NULL;

	if(i != 0)//i=0则表示没有开启线程
	{
		// 等待线程返回,退出函数
	    WaitForMultipleObjects(uHostNum,hThread,TRUE,-1);
	}
	if (threada->IsUseForScan)
	{


		for (i=0; pLanHostInfo->prev != NULL; )
		{
			pLanHostInfo = pLanHostInfo->prev;
			if (pLanHostInfo->bIsOnline)
			{
				memset(MACAddr, 0, strlen(MACAddr));
				sprintf(MACAddr, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X", 
				pLanHostInfo->ucMacAddr[0], pLanHostInfo->ucMacAddr[1],
				pLanHostInfo->ucMacAddr[2], pLanHostInfo->ucMacAddr[3], 
				pLanHostInfo->ucMacAddr[4], pLanHostInfo->ucMacAddr[5]);
				 
	     

				dlg->AddNewParse(MACAddr,pLanHostInfo->IpAddr,strlen(pLanHostInfo->HostName) > 0 ? pLanHostInfo->HostName : "未知");
				i++;
			}
		}

			dlg->m_progressScan.SetPos(0);
	}
	else 
	{
		
		while(pLanHostInfo->prev != NULL)
		{
			pLanHostInfo = pLanHostInfo->prev;
			if (!memcmp(threada->ComPareMac,pLanHostInfo->ucMacAddr,6))
				break;

			
		}
		if(pLanHostInfo->prev == NULL)
		{
			dlgw->SetDlgItemTextA(IDC_STATICRESULT,"未找到攻击者IP。。请稍候再试");
			
		    return 0;
		}
		dlgw->IPResult = pLanHostInfo->IpAddr;
		CString info = "找到攻击者MAC，但是可能不准确";
		info += pLanHostInfo->IpAddr;
		dlgw->m_progressctrl.SetPos(0);
		AfxMessageBox(info);
	}


    
	AfxEndThread(0);
	


    
	return 1;
}


UINT scan_lan(LPVOID lparam)
{
	int flag;
	char IpAddr[16];
	u_char ucMacAddr[6];
	PLAN_HOST_INFO pInfo;
	unsigned long nRemoteAddr;
	struct hostent *pHostent; 
	memset(IpAddr, 0, sizeof(IpAddr));
	memset(ucMacAddr, 0xff, sizeof(ucMacAddr));

	// 拷贝参数
	pInfo = (PLAN_HOST_INFO) lparam;
	memcpy(IpAddr, pInfo->IpAddr, sizeof(IpAddr));
	ULONG macAddLen=6;
	memset(ucMacAddr, 0xff, sizeof(ucMacAddr));
	if (SendARP(inet_addr(IpAddr), (IPAddr) NULL,(PULONG) ucMacAddr, &macAddLen) == NO_ERROR)
		flag = 1;
	else
		flag =0;

	if (flag)
	{
		// 存活主机信息保存至链表
		pInfo->bIsOnline = TRUE;
		memcpy(pInfo->ucMacAddr, ucMacAddr, sizeof(ucMacAddr));

		// 得到主机名
		nRemoteAddr = inet_addr(IpAddr);
		pHostent = new struct hostent;
		memset(pHostent, 0, sizeof(struct hostent));
		pHostent = gethostbyaddr((char*)&nRemoteAddr, 4, AF_INET);
		if (pHostent)
			memcpy(pInfo->HostName, pHostent->h_name, strlen(pHostent->h_name));
	}
	else
		pInfo->bIsOnline = FALSE;
    return 0;
}



CString GetFilePath(CHAR *sFilename) 
{
	CHAR sDrive[_MAX_DRIVE];
	CHAR sDir[_MAX_DIR];
	CHAR sFname[_MAX_FNAME];
	CHAR sExt[_MAX_EXT];

	_tsplitpath(sFilename, sDrive, sDir, sFname, sExt);

	CString rVal(CString(sDrive) + CString(sDir));
	int nLen = rVal.GetLength();

	if (rVal.GetAt(nLen-1) != '\\')
		rVal += "\\";

	return rVal;
}  

CString GetFileName(CHAR *sFilename) 
{
	CHAR sDrive[_MAX_DRIVE];
	CHAR sDir[_MAX_DIR];
	CHAR sFname[_MAX_FNAME];
	CHAR sExt[_MAX_EXT];

	_tsplitpath(sFilename, sDrive, sDir, sFname, sExt);

	CString rVal;
	rVal.Format("%s%s", sFname, sExt);

	return rVal;
}  