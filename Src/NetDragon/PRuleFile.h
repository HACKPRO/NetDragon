//////////////////////////////////////
// PRuleFile.h

// �����ļ�

#include "TypeStruct.h"
#include "PMacRes.h"

//  �ӽ�������ļ����������浽CPRuleFile�����С������෴�Ķ���
// NetDragon Personal firewall 

#ifndef __PRULEFILE_H__
#define __PRULEFILE_H__

class CPRuleFile
{
public:
	CPRuleFile();
	~CPRuleFile();

	// ���ļ����ع��˹���
	BOOL LoadRules();
	// �����˹��򱣴浽�ļ�
	BOOL SaveRules();

	// ���nCount��Ӧ�ò㣨���Ĳ㣩���˹���
	BOOL AddLspRules(RULE_ITEM *pItem, int nCount);
	

	// ɾ��һ��Ӧ�ò㣨���Ĳ㣩���˹���
	BOOL DelLspRule(int nIndex);
	

	// �ļ�����
	RULE_FILE_HEADER m_header;		// �ļ�ͷ
	RULE_ITEM *m_pLspRules;			// Ӧ�ò����ָ��
	
private:
	// ��ʼ���ļ�����
	void InitFileData();
	// �򿪴����ļ������û�л��Զ������������г�ʼ��
	BOOL OpenFile();
	// �����򱣴浽ָ���ļ�
	BOOL WriteRules(CHAR *pszPathName);

	HANDLE m_hFile;
	CHAR m_szPathName[MAX_PATH];
	int m_nLspMaxCount;
	
	BOOL m_bLoad;
};

#endif // __PRULEFILE_H__