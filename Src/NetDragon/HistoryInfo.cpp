/************************************************************************/
/* .
	���γ�����ʷ��¼	Code  by  Win.Hunter
*/
/************************************************************************/


#include "stdafx.h"
#include "Net Dragon.h"
#include "HistoryInfo.h"



// HistoryInfo �Ի���

IMPLEMENT_DYNAMIC(HistoryInfo, CDialog)

HistoryInfo::HistoryInfo(CWnd* pParent /*=NULL*/)
	: CDialog(HistoryInfo::IDD, pParent)
{


}

HistoryInfo::~HistoryInfo()
{
}

void HistoryInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
}


BEGIN_MESSAGE_MAP(HistoryInfo, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST2, &HistoryInfo::OnLbnSelchangeList2)
END_MESSAGE_MAP()


// HistoryInfo ��Ϣ�������

void HistoryInfo::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	DestroyWindow();
}

BOOL HistoryInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LoadInfo();
	parpcheater = arpcheater.begin();
	while(parpcheater != arpcheater.end())
    {
        m_list.AddString(*parpcheater);
		parpcheater++;
    }
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

bool HistoryInfo::LoadInfo(void)
{
	arpcheater.clear();
	ifstream infile(".\\History.log");
    string info;
    if ( !infile)
    {
        AfxMessageBox(_T("����ʷ�ļ�ʧ�ܣ���ȷ��·���Ƿ���ȷ��"));
        return false;
    }
	CString infotemp;
    while( infile >> info )
    {
		infotemp = info.data();
        arpcheater.push_back(infotemp);
    }
    
    return true;
	
}

void HistoryInfo::OnLbnSelchangeList2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
