



#include "stdafx.h"
#include "IntrusionPage.h"
#include "sniffer.h"
#include "ethernetprotocol.h"
#include "intrusiondetect.h"

bool gIntrusionDetect=true;
typedef void (*pcap_func_t)(unsigned char*, const struct pcap_pkthdr*, const u_char*);
void ProcessProtocolPacket(unsigned char* user, const struct pcap_pkthdr* h,
							 const unsigned char* packetdata)
{	

		EthernetOperation(packetdata);
		if(gIntrusionDetect)
		{
			IntrusionDetect();
		}
	
}

int PacketOperation()
{

	pcap_t* PcapHandle;
    int number = -1;
	char Error[PCAP_ERRBUF_SIZE];

	pcap_handler Handler;
	if ((PcapHandle = pcap_open_live(pAdapterInfo->AdapterName, 65536, 1, 1000, Error)) == NULL)
	{
		return -1;
	}

	Handler = (pcap_func_t) ProcessProtocolPacket;
	pcap_loop(PcapHandle, number, Handler, NULL);
	return 0;
}


