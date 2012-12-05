


#include "stdafx.h"
#include "IntrusionPage.h"
#include <stdio.h>
#include <sys/types.h>
#include "icmpprotocol.h"
#include "sniffer.h"
#include "intrusiondetect.h"
struct IcmpHeader DisplayIcmp;	
extern struct ids Intrusion;
struct ICMP IcmpProtocol;
extern HWND hWnd;
void ICMPOperation(const unsigned char* packetdata)
{
	Intrusion.protocoltype =3;
	struct ICMP* Icmp;
	Icmp = (struct ICMP *) packetdata;
	sprintf(DisplayIcmp.type, "%d", Icmp->Type);
	IcmpProtocol.Type =Icmp->Type;
	switch (Icmp->Type)
	{
	case 0:
		sprintf(DisplayIcmp.information, "%s", "����Ӧ��");
		break;
	case 8:
		sprintf(DisplayIcmp.information, "%s", "��������");
		break;
	case 13:
		sprintf(DisplayIcmp.information, "%s", "ʱ�������");
		break;
	case 14:
		sprintf(DisplayIcmp.information, "%s", "ʱ���Ӧ��");
		break;
	case 17:
		sprintf(DisplayIcmp.information, "%s", "��ַ��������");
		break;
	case 18:
		sprintf(DisplayIcmp.information, "%s", "��ַ����Ӧ��");
		break;
	default:
		sprintf(DisplayIcmp.information, "%s", "ICMP����δ֪");
		break;
	}
	sprintf(DisplayIcmp.code, "%d", Icmp->Code);
	IcmpProtocol.Code =Icmp->Code ;
	sprintf(DisplayIcmp.checksum, "%d", ntohs(Icmp->Checksum));
	IcmpProtocol.Checksum =ntohs(Icmp->Checksum); 
	return;
}

