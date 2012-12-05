


struct ids
{

	int protocoltype;

	int port;
};
struct IntrusionInfor
{
	char time[100];
	char attackip[100];

	char information[1024];
};

int IntrusionDetect(void);