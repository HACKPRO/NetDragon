// ��Ϣ���ͽӿ�

#include "StdAfx.h"
#include <winsock2.h>


Sender::Sender(void)
{
}
Sender::~Sender(void)
{
}

void Sender::SetInfo(CString info)
{
	sendInfo.push_back(info+"\r\n");
}

void Sender::SetServer(CString server)
{
	serverIp = server;
}

bool Sender::SendInfo(void){
	if(sendInfo.empty())
		return false;
	// �����׽���
	SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s == INVALID_SOCKET)
	{
		AfxMessageBox("��������ʧ�ܣ�");
		return 0;
	}

	// ��дԶ�̵�ַ��Ϣ
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(7668);
	servAddr.sin_addr.S_un.S_addr = inet_addr(serverIp);
	
	if(::connect(s, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		AfxMessageBox("�޷����ӵ�ָ������");
		return 0;
	}
	psendInfo = sendInfo.begin();
	while(psendInfo != sendInfo.end())
    {
       	::send(s, *psendInfo, strlen(*psendInfo)+1, 0);
		//sendInfo.erase(psendInfo);
		psendInfo++;
    }
	// �ر��׽���
	::closesocket(s);
	sendInfo.clear();
	return true;
}