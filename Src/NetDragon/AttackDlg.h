#pragma once

#include <winsock2.h>

UINT ListenThread(LPVOID lpParam);
UINT SendArpPacket(LPVOID lpParam );
UINT SpoofThread(LPVOID lparam);
UINT SpoofThread2(LPVOID lparam);
BOOL StaticARP(unsigned char szIPAddr[], BYTE bPhysAddr[]);
void AnalyzePacket(const u_char *pkt_data, unsigned int pkt_len);
BOOL SaveLog(const void *data, unsigned int size);

unsigned int swapu32(unsigned int n);




#ifndef __PROTOINFO_H__
#define __PROTOINFO_H__


#define ETHERTYPE_IP    0x0800
#define ETHERTYPE_ARP   0x0806

#define ARPHRD_ETHER 	1
#define ARP_LEN		 48
// 协议
#define PROTO_TCP     6
#define PROTO_UDP     17




typedef struct ip_address
{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;

#pragma pack(push, 1)//取消内存大小自动对齐



typedef struct _IPHeader		// 20字节的IP头
{
    UCHAR     iphVerLen;      // 版本号和头长度（各占4位）
    UCHAR     ipTOS;          // 服务类型 
    USHORT    ipLength;       // 封包总长度，即整个IP报的长度
    USHORT    ipID;			  // 封包标识，惟一标识发送的每一个数据报
    USHORT    ipFlags;	      // 标志
    UCHAR     ipTTL;	      // 生存时间，就是TTL
    UCHAR     ipProtocol;     // 协议，可能是TCP、UDP、ICMP等
    USHORT    ipChecksum;     // 校验和
	union {
		unsigned int   ipSource;
		ip_address ipSourceByte;
	};
	union {
		unsigned int   ipDestination;
		ip_address ipDestinationByte;
	};
} IPHeader, *PIPHeader; 

typedef struct _TCPHeader		// 20字节的TCP头
{
	USHORT	sourcePort;			// 16位源端口号
	USHORT	destinationPort;	// 16位目的端口号
	ULONG	sequenceNumber;		// 32位序列号
	ULONG	acknowledgeNumber;	// 32位确认号
	UCHAR	dataoffset;			// 高4位表示数据偏移
	UCHAR	flags;				// 6位标志位
								//FIN - 0x01
								//SYN - 0x02
								//RST - 0x04 
								//PUSH- 0x08
								//ACK- 0x10
								//URG- 0x20
								//ACE- 0x40
								//CWR- 0x80

	USHORT	windows;			// 16位窗口大小
	USHORT	checksum;			// 16位校验和
	USHORT	urgentPointer;		// 16位紧急数据偏移量 
} TCPHeader, *PTCPHeader;

typedef struct _UDPHeader
{
    USHORT udp_source_port;       //源端口号

    USHORT udp_destination_port;  //目的端口号
  
    USHORT udp_length;           //长度
   
    USHORT udp_checksum;         //校验和
   
} UDPHeader,*PUDPHeader;
typedef struct _QQHeader
{
    UCHAR    begin_tag;    //always 0x02 till now
    USHORT   sender_ver;   //client version, details below
    USHORT   qq_cmd;       //command code, QQ_CMD_XXX 
    USHORT   sq_no;        //sequence number.
    UCHAR    data[1];      //following are data.
}QQHeader,*PQQHeader;



#endif // __PROTOINFO_H__
#pragma pack(pop)



// CAttackDlg 对话框

class CAttackDlg : public CDialog
{
	DECLARE_DYNAMIC(CAttackDlg)

public:
	CAttackDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAttackDlg();

// 对话框数据
	enum { IDD = IDD_ATTACKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();

public:
	afx_msg void OnBnClickedScanhost();
public:
	CActiveHost activehostdlg;
public:
	CString m_attackip;
	CString m_forgemac;
	CString m_localip;
	CString m_localmac;
	CString m_gatewayip;
	CString m_gatewaymac;

public:
	bool InitArp(void);

public:
	afx_msg void OnBnClickedGetlocalinformation();
public:
	UINT m_sendinterval;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOutline();


public:
	afx_msg void OnBnClickedArpcheat();
public:
	afx_msg void OnBnClickedShowdata();

public:
	afx_msg void OnBnClickedConflict();
public:
	afx_msg void OnBnClickedHelp();
public:
	afx_msg void OnBnClickedOpendata();

	afx_msg void OnEnChangeEditinfo();
	afx_msg void OnEnChangeEditakip();
};
