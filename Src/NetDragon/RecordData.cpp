// RecordData.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Net Dragon.h"
#include "RecordData.h"


// CRecordData �Ի���

IMPLEMENT_DYNAMIC(CRecordData, CDialog)

CRecordData::CRecordData(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordData::IDD, pParent)
	, m_data(_T(""))
{

}

CRecordData::~CRecordData()
{
}

void CRecordData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITDATA, m_data);
}


BEGIN_MESSAGE_MAP(CRecordData, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CRecordData::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CRecordData::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDITDATA, &CRecordData::OnEnChangeEditdata)
END_MESSAGE_MAP()


// CRecordData ��Ϣ�������

void CRecordData::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_data.Empty();
}

void CRecordData::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CRecordData::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	OnOK();
}

void CRecordData::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	OnOK();
}

void CRecordData::OnEnChangeEditdata()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
