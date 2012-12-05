////////////////////////////////////////
// PIOControl.h�ļ�

// ����DLL�����ڴ�

#include "PMacRes.h"
#include "TypeStruct.h"

#ifndef __PIOCONTROL_H__
#define __PIOCONTROL_H__

class CPIOControl
{
public:
	CPIOControl();
	~CPIOControl();
	// ���ù���ģʽ
	void SetWorkMode(int nWorkMode);
	// ��ȡ����ģʽ
	int GetWorkMode();

	// ���ù����ļ�
	void SetRuleFile(RULE_FILE_HEADER *pHeader, RULE_ITEM *pRules);
	// ������ģ����
	void SetPhoenixInstance(HWND hWnd, CHAR *pszPathName);

	// ��ȡѯ�ʵ�Ӧ�ó��������ѯ�ʵĽ��
	LPCSTR GetQueryApp(int nIndex);
	void SetQueryApp(int nIndex, BOOL bPass);

	// ��ȡһ���Ự��Ϣ
	void GetSession(SESSION *pSession, int nIndex);

private:
	PFNLSPIoControl m_fnIoControl;
	HMODULE			m_hLSPModule;	
	LSP_IO_CONTROL m_IoControl;
};

#endif // __PIOCONTROL_H__