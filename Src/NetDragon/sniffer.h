


#include <stdlib.h>
#include <stdio.h>
#include "stdafx.h"
#include "pcap.h"

struct PacketInformation
{
	char DestinationMac[100];
	char SourceMac[100];
	char NetType[100];
	char SourceAddr[100];
	char SourcePort[100];
	char DestinationAddr[100];
	char DestinationPort[100];
};
int PacketOperation();
int DumpFileOperation();
