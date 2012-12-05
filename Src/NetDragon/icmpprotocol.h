


struct ICMP
{
	unsigned char Type;
	unsigned char  Code;
	unsigned short  Checksum;
};
struct IcmpHeader
{
	char type[100];
	char code[100];
	char checksum[100];
	char information[100];
};
void ICMPOperation(const unsigned char* packetdata);