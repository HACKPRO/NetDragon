/************************************************************************/
/* .
	本次程序历史记录	Code  by  Win.Hunter
*/
/************************************************************************/


#include "stdafx.h"
#include "Net Dragon.h"
#include "HistoryInfo.h"



// HistoryInfo 对话框

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


// HistoryInfo 消息处理程序

void HistoryInfo::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	DestroyWindow();
}

BOOL HistoryInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LoadInfo();
	parpcheater = arpcheater.begin();
	while(parpcheater != arpcheater.end())
    {
        m_list.AddString(*parpcheater);
		parpcheater++;
    }
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

bool HistoryInfo::LoadInfo(void)
{
	arpcheater.clear();
	ifstream infile(".\\History.log");
    string info;
    if ( !infile)
    {
        AfxMessageBox(_T("打开历史文件失败！请确定路径是否正确！"));
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
	// TODO: 在此添加控件通知处理程序代码
}
