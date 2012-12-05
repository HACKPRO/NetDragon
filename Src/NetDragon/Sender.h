#pragma once

#include <vector>


class Sender
{
public:
	Sender(void);
public:
	~Sender(void);
public:
	void SetInfo(CString info);
	void SetServer(CString server);
	bool SendInfo(void);
private:
	std::vector<CString> sendInfo;          
    std::vector<CString>::iterator psendInfo;
	CString serverIp;
};
