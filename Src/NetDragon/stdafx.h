// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once


#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0410 // ��������Ϊ�ʺ� Windows Me ����߰汾����Ӧֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 6.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0600	// ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT




#include "SkinPPWTL.h"
#pragma comment(lib, "SkinPPWTL.lib")
#include <winsock2.h>
#include "Resource.h"
#include "ActiveHost.h"
#include "Iphlpapi.h"
#include "WarningDlg.h"
#include <afxdlgs.h>
#include "atlbase.h"

#include <winsock2.h>

#pragma comment(lib, "Rpcrt4.lib") 

#pragma comment(lib, "wpcap.lib")
#pragma comment(lib, "Iphlpapi.lib")
#include "PMacRes.h"
#include "TypeStruct.h"
#include "PRuleFile.h"
#include "PIOControl.h"
#include "Sender.h"
//���ݸ�ɨ�������̵߳Ĳ���

#define WM_SHOWTASK   (WM_USER + 0x100)    
#define WM_ARPCHEAT   (WM_USER + 0x101)
#define WM_SENDINFO   (WM_USER + 0x102)
struct thread
{
	char Ipaddr[40];
	char Submask[40];
	CActiveHost *pDlg;
	WarningDlg  *pDlgw;//���ݸ�this�󣬱��ʹɨ���߳���Ի���ͨ����
	bool IsUseForScan;
	unsigned char ComPareMac[6];
};

//ȫ�ֱ���������
extern struct thread m_thread;
extern bool startup;
extern bool isServerAllowed;
extern Sender sender;
//������Ϣ
extern PIP_ADAPTER_INFO pAdapterInfo;
extern PIP_ADAPTER_INFO pOrgAdapterInfo;
//��������IP
extern CString attackip;
//����MAC
extern unsigned char gatewaymac[6];
extern unsigned char mymac[6];

extern int  uHostNum;//�������õ������ڵ���������
extern unsigned char CompareMacglobal[6];

extern CPRuleFile g_RuleFile;
extern CPIOControl *g_pIoControl;
// ������������Ϣ˫������
typedef struct _LAN_HOST_INFO {
	char IpAddr[4 * 4]; /* ����IP��ַ */
	char HostName[25]; /* ������ */
	unsigned char ucMacAddr[6]; /* ����������ַ */
	BOOL bIsOnline; /* �Ƿ����� */
	struct _LAN_HOST_INFO *prev; /* ��һ��������ָ�� */
	struct _LAN_HOST_INFO *next; /* ��һ��������ָ�� */
}LAN_HOST_INFO, *PLAN_HOST_INFO;


UINT EnumLanHost(LPVOID pParam);//ɨ�����������߳�����
UINT scan_lan(LPVOID lparam);//ɨ�����������߳�����
CString GetFilePath(CHAR *sFilename);
CString GetFileName(CHAR *sFilename);
BOOL InstallProvider(WCHAR *pwszPathName);
BOOL RemoveProvider();
//����ARP�ṹ�Ķ���

enum{ 
    MAC_ADDR_LEN  = 6,       // mac��ַ�ĳ���
    IP_ADDR_LEN   = 4        // IP��ַ�ĳ���
};
enum{
    ETH_TYPE  =  0x0806 ,
    IP_TYPE =    0x0800 
};
enum{
    ARP_REQUEST  = 0x0001 ,   //arp request
    ARP_REPLY    = 0x0002 ,   //arp reply
};
enum {
    ARP_HARDWARE =  0x0001   
};
enum{
    MAX_ADAPTERS = 16         //������������������
};

#pragma pack( push ,1 )          // ȡ��WINDOWS�ڴ��Զ����룬����Ҫ

/*
*                            ������̫��֡�Ľṹ
*
*         0                 15                31                47     
*         +--------+--------+--------+--------+--------+--------+
*         |                   �޵�mac��ַ                       |
*         +--------+--------+--------+--------+--------+--------+
*         |                   Դ��mac��ַ                       |
*         +--------+--------+--------+--------+--------+--------+
*         |     0x0806      |     ���ݡ�����
*         +--------+--------+--------+--------+--------+--------+
*         0                 15                31                47
*/
typedef struct
{
    unsigned char  ether_dhost[MAC_ADDR_LEN];    //��̫��Ŀ�ĵ�ַ
    unsigned char  ether_shost[MAC_ADDR_LEN];    //��̫��Դ��ַ
    unsigned short ether_type;                   //��̫�����ͣ�Ĭ��0x0806
    char           data[1];                      //�������ARPͷ
} ETH_t, *pETH_t;

/*
*                               ����ARP֡�Ľṹ
*
*         0                 15                31                47     
*         +--------+--------+--------+--------+--------+--------+
*         |      0x0001     |    0x0800(IP)   |  0x06  |  0x04  | 
*         +--------+--------+--------+--------+--------+--------+
*         |      opt        |           Դ��mac��ַ   ������            
*         +--------+--------+--------+--------+--------+--------+
*            ������         |           Դ��ip��ַ              |
*         +--------+--------+--------+--------+--------+--------+
*         |                    �޵�mac��ַ                      |
*         +--------+--------+--------+--------+--------+--------+
*         |           �޵�ip��ַ              | 
*         +--------+--------+--------+--------+
*         0                 15                31                47
*/
typedef struct arphdr //arpͷ
{
    unsigned short arp_hardware_type;                                // Ӳ������  Ethernet 0x0001
    unsigned short arp_protocol_type;                                // Э�����ͣ�IP��0x0800
    unsigned char  arp_hardware_length;                              // Ӳ����ַ���ȣ�mac��ַ����
    unsigned char  arp_protocol_length;                              // Э���ַ���ȣ�ip��ַ����
    unsigned short arp_operation_code;                               // [ request |  response ]
    unsigned char  arp_source_ethernet_address[MAC_ADDR_LEN];        // ���Ͷ���̫����ַ
    unsigned long  arp_source_ip_address;                            // ���Ͷ�ip��ַ
    unsigned char  arp_destination_ethernet_address[MAC_ADDR_LEN];   // ���ն���̫����ַ
    unsigned long  arp_destination_ip_address;                       // ���ն�ip��ַ
} ARP_t, *pARP_t;

#pragma pack(  pop  )






#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


