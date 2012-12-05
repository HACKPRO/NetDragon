


struct UDP
{
	unsigned short SrcPort;
	unsigned short DstPort;
	unsigned short Length;
	unsigned short Checksum;
};
struct UdpHeader
{
	char SrcPort[1024];
	char DstPort[1024];
	char Length[1024];
	char Checksum[1024];
};
int UDPOperation(const unsigned char* packetdata);
