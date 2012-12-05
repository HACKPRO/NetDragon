// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once


#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将它更改为适合 Windows Me 或更高版本的相应值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
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
//传递给扫描主机线程的参数

#define WM_SHOWTASK   (WM_USER + 0x100)    
#define WM_ARPCHEAT   (WM_USER + 0x101)
#define WM_SENDINFO   (WM_USER + 0x102)
struct thread
{
	char Ipaddr[40];
	char Submask[40];
	CActiveHost *pDlg;
	WarningDlg  *pDlgw;//传递给this后，变可使扫描线程与对话框通信了
	bool IsUseForScan;
	unsigned char ComPareMac[6];
};

//全局变量的声明
extern struct thread m_thread;
extern bool startup;
extern bool isServerAllowed;
extern Sender sender;
//网卡信息
extern PIP_ADAPTER_INFO pAdapterInfo;
extern PIP_ADAPTER_INFO pOrgAdapterInfo;
//被攻击者IP
extern CString attackip;
//网关MAC
extern unsigned char gatewaymac[6];
extern unsigned char mymac[6];

extern int  uHostNum;//由掩码获得的网段内的主机数量
extern unsigned char CompareMacglobal[6];

extern CPRuleFile g_RuleFile;
extern CPIOControl *g_pIoControl;
// 网段内主机信息双向链表
typedef struct _LAN_HOST_INFO {
	char IpAddr[4 * 4]; /* 主机IP地址 */
	char HostName[25]; /* 主机名 */
	unsigned char ucMacAddr[6]; /* 主机网卡地址 */
	BOOL bIsOnline; /* 是否在线 */
	struct _LAN_HOST_INFO *prev; /* 上一个主机的指针 */
	struct _LAN_HOST_INFO *next; /* 下一个主机的指针 */
}LAN_HOST_INFO, *PLAN_HOST_INFO;


UINT EnumLanHost(LPVOID pParam);//扫描主机的主线程申明
UINT scan_lan(LPVOID lparam);//扫描主机的子线程申明
CString GetFilePath(CHAR *sFilename);
CString GetFileName(CHAR *sFilename);
BOOL InstallProvider(WCHAR *pwszPathName);
BOOL RemoveProvider();
//关于ARP结构的定义

enum{ 
    MAC_ADDR_LEN  = 6,       // mac地址的长度
    IP_ADDR_LEN   = 4        // IP地址的长度
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
    MAX_ADAPTERS = 16         //最大的网络适配器数量
};

#pragma pack( push ,1 )          // 取消WINDOWS内存自动对齐，必须要

/*
*                            定义以太网帧的结构
*
*         0                 15                31                47     
*         +--------+--------+--------+--------+--------+--------+
*         |                   宿的mac地址                       |
*         +--------+--------+--------+--------+--------+--------+
*         |                   源的mac地址                       |
*         +--------+--------+--------+--------+--------+--------+
*         |     0x0806      |     数据。。。
*         +--------+--------+--------+--------+--------+--------+
*         0                 15                31                47
*/
typedef struct
{
    unsigned char  ether_dhost[MAC_ADDR_LEN];    //以太网目的地址
    unsigned char  ether_shost[MAC_ADDR_LEN];    //以太网源地址
    unsigned short ether_type;                   //以太网类型，默认0x0806
    char           data[1];                      //用以填充ARP头
} ETH_t, *pETH_t;

/*
*                               定义ARP帧的结构
*
*         0                 15                31                47     
*         +--------+--------+--------+--------+--------+--------+
*         |      0x0001     |    0x0800(IP)   |  0x06  |  0x04  | 
*         +--------+--------+--------+--------+--------+--------+
*         |      opt        |           源的mac地址   。。。            
*         +--------+--------+--------+--------+--------+--------+
*            。。。         |           源的ip地址              |
*         +--------+--------+--------+--------+--------+--------+
*         |                    宿的mac地址                      |
*         +--------+--------+--------+--------+--------+--------+
*         |           宿的ip地址              | 
*         +--------+--------+--------+--------+
*         0                 15                31                47
*/
typedef struct arphdr //arp头
{
    unsigned short arp_hardware_type;                                // 硬件类型  Ethernet 0x0001
    unsigned short arp_protocol_type;                                // 协议类型，IP，0x0800
    unsigned char  arp_hardware_length;                              // 硬件地址长度，mac地址长度
    unsigned char  arp_protocol_length;                              // 协议地址长度，ip地址长度
    unsigned short arp_operation_code;                               // [ request |  response ]
    unsigned char  arp_source_ethernet_address[MAC_ADDR_LEN];        // 发送端以太网地址
    unsigned long  arp_source_ip_address;                            // 发送端ip地址
    unsigned char  arp_destination_ethernet_address[MAC_ADDR_LEN];   // 接收端以太网地址
    unsigned long  arp_destination_ip_address;                       // 接收端ip地址
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


