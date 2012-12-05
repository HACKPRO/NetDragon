


#include "stdafx.h"
#include "IntrusionPage.h"
#include <stdio.h>
#include "pcap.h"
#include "ethernetprotocol.h"
#include "ipprotocol.h"
#include "sniffer.h"
struct EtherHeader DisplayEthernet;
extern HWND hWnd;
extern bool StopThread;
struct PacketInformation packet;
int EthernetOperation(const unsigned char* packetdata)
{
	int NetType;
    struct	ETHERNET* ethernet;
	unsigned char *MAC;
	if (StopThread == TRUE)
		AfxEndThread(1, 1);
	sprintf(packet.SourceMac, "%s", "");
	sprintf(packet.DestinationMac, "%s", "");
	sprintf(packet.NetType, "%s", "");
	sprintf(packet.DestinationAddr, "%s", "");
	sprintf(packet.DestinationPort, "%s", "");
	sprintf(packet.SourceAddr, "%s", "");
	sprintf(packet.SourcePort, "%s", "");
	ethernet = (struct	ETHERNET *) packetdata;  
	NetType = ntohs(ethernet->NetType);  
	MAC=ethernet->SourceMac ;
	sprintf(DisplayEthernet.SourceMac, "%02x:%02x:%02x:%02x:%02x:%02x",*MAC,*(MAC+1),*(MAC+2),*(MAC+3),*(MAC+4),*(MAC+5));
	strcpy(packet.SourceMac,DisplayEthernet.SourceMac);
	MAC=ethernet->DestinationMac;
	sprintf(DisplayEthernet.DestinationMac, "%02x:%02x:%02x:%02x:%02x:%02x",*MAC,*(MAC+1),*(MAC+2),*(MAC+3),*(MAC+4),*(MAC+5));
	strcpy(packet.DestinationMac,DisplayEthernet.DestinationMac);
	packetdata += sizeof(struct	ETHERNET);
	switch (NetType)
	{
	case 0x0800:
		sprintf(DisplayEthernet.NetType, "%s", "IP");
		
		IPOperation(packetdata);
		
		return 0;
	default:
		sprintf(DisplayEthernet.NetType, "%s", "δ֪������");
		sprintf(packet.NetType, "%s","δ֪������");
		return 0;
	}
	return 1;
}

