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
// Э��
#define PROTO_TCP     6
#define PROTO_UDP     17




typedef struct ip_address
{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;

#pragma pack(push, 1)//ȡ���ڴ��С�Զ�����



typedef struct _IPHeader		// 20�ֽڵ�IPͷ
{
    UCHAR     iphVerLen;      // �汾�ź�ͷ���ȣ���ռ4λ��
    UCHAR     ipTOS;          // �������� 
    USHORT    ipLength;       // ����ܳ��ȣ�������IP���ĳ���
    USHORT    ipID;			  // �����ʶ��Ωһ��ʶ���͵�ÿһ�����ݱ�
    USHORT    ipFlags;	      // ��־
    UCHAR     ipTTL;	      // ����ʱ�䣬����TTL
    UCHAR     ipProtocol;     // Э�飬������TCP��UDP��ICMP��
    USHORT    ipChecksum;     // У���
	union {
		unsigned int   ipSource;
		ip_address ipSourceByte;
	};
	union {
		unsigned int   ipDestination;
		ip_address ipDestinationByte;
	};
} IPHeader, *PIPHeader; 

typedef struct _TCPHeader		// 20�ֽڵ�TCPͷ
{
	USHORT	sourcePort;			// 16λԴ�˿ں�
	USHORT	destinationPort;	// 16λĿ�Ķ˿ں�
	ULONG	sequenceNumber;		// 32λ���к�
	ULONG	acknowledgeNumber;	// 32λȷ�Ϻ�
	UCHAR	dataoffset;			// ��4λ��ʾ����ƫ��
	UCHAR	flags;				// 6λ��־λ
								//FIN - 0x01
								//SYN - 0x02
								//RST - 0x04 
								//PUSH- 0x08
								//ACK- 0x10
								//URG- 0x20
								//ACE- 0x40
								//CWR- 0x80

	USHORT	windows;			// 16λ���ڴ�С
	USHORT	checksum;			// 16λУ���
	USHORT	urgentPointer;		// 16λ��������ƫ���� 
} TCPHeader, *PTCPHeader;

typedef struct _UDPHeader
{
    USHORT udp_source_port;       //Դ�˿ں�

    USHORT udp_destination_port;  //Ŀ�Ķ˿ں�
  
    USHORT udp_length;           //����
   
    USHORT udp_checksum;         //У���
   
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



// CAttackDlg �Ի���

class CAttackDlg : public CDialog
{
	DECLARE_DYNAMIC(CAttackDlg)

public:
	CAttackDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAttackDlg();

// �Ի�������
	enum { IDD = IDD_ATTACKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
