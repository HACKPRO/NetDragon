// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// Net Dragon.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

//ȫ�ֱ���
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

// �õ�������������������Ϣ�����߳�
// ��ڲ���: thread�ṹ��������IP,MASK,�����������߳�ͨ�ŵĴ���ָ��
UINT EnumLanHost(LPVOID pParam)
{
	struct thread *threada=(struct thread*)pParam;
    unsigned int uHostByte; // ����λ
	int i;
	ULONG uMacLength = 6;

	
	CActiveHost *dlg = (CActiveHost*)threada->pDlg;
	
	
	
	WarningDlg *dlgw = (WarningDlg*)threada->pDlgw;
	
	// ������ʱ����,���IP��MAC
	char TempIpAddr[4 * 4];
	char MACAddr[40];
	
	CWinThread *pT[2001];
	
	
    PLAN_HOST_INFO pLanHostInfo, pNextHostInfo; // ��Զָ�������β��
    HANDLE hThread[2001]; // �߳�����ָ��
    
   
    uHostByte = htonl(inet_addr(threada->Ipaddr)) & 0xffffff00;// ��IP��ַ�õ�����λ
    // ��ʼ������
	pLanHostInfo = new LAN_HOST_INFO;
	memset(pLanHostInfo, 0, sizeof(LAN_HOST_INFO));
	pLanHostInfo->prev = NULL;
    // ��ʼ���ж��߳�ARPɨ��,����uHostNum���߳�ɨ��
	// Scan Range: 1 ~ uHostNum
	for (i = 0, uHostByte ++; i < uHostNum; i ++, uHostByte ++)
	{
		// ����IP��ַ
		memset(TempIpAddr, 0, strlen(TempIpAddr));
		sprintf(TempIpAddr, "%d.%d.%d.%d", 
			(uHostByte & 0xff000000) >> 0x18,//ȡ��IPλ������24λ
			(uHostByte & 0x00ff0000) >> 0x10,//ȡ��IPλ������16λ
			(uHostByte & 0x0000ff00) >> 0x08,//ȡ��IPλ������8λ
			(uHostByte & 0x000000ff));//ȡ�����һ��IPλ����ʼΪ0���Ժ�ÿ�μ�1
		// ��������
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

		Sleep(2); // �ȴ�����������ϣ������¸�ֵ
	}
	hThread[i]=NULL;

	if(i != 0)//i=0���ʾû�п����߳�
	{
		// �ȴ��̷߳���,�˳�����
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
				 
	     

				dlg->AddNewParse(MACAddr,pLanHostInfo->IpAddr,strlen(pLanHostInfo->HostName) > 0 ? pLanHostInfo->HostName : "δ֪");
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
			dlgw->SetDlgItemTextA(IDC_STATICRESULT,"δ�ҵ�������IP�������Ժ�����");
			
		    return 0;
		}
		dlgw->IPResult = pLanHostInfo->IpAddr;
		CString info = "�ҵ�������MAC�����ǿ��ܲ�׼ȷ";
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

	// ��������
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
		// ���������Ϣ����������
		pInfo->bIsOnline = TRUE;
		memcpy(pInfo->ucMacAddr, ucMacAddr, sizeof(ucMacAddr));

		// �õ�������
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