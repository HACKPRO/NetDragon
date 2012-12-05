// Net DragonDlg.h : 头文件
//
#include "AttackDlg.h"
#include "HistoryInfo.h"
#include "LocalInfoDlg.h"
#include "ARPGuardPage.h"
#include "MonitorPage.h"
#include "RulePage.h"
#include "PRuleFile.h"
#include "PIOControl.h"
#include "SyssetPage.h"
#include "IntrusionPage.h"
#pragma once
UINT StartArpMonitor(LPVOID lpParam);

UINT sendrequest(LPVOID lpParam);

UINT startSendInfo(LPVOID lpParam);

// CNetDragonDlg 对话框
class CNetDragonDlg : public CDialog
{
// 构造
public:
	CNetDragonDlg(CWnd* pParent = NULL);


// 对话框数据
	enum { IDD = IDD_NetDragon_DIALOG };



// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg LRESULT onShowTask(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT onshowarp(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT onSendInfo(WPARAM wParam,LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg long OnQueryAcl(WPARAM wParam, LPARAM lParam);
	afx_msg long OnSessionNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	CAttackDlg attackdlg;
	HistoryInfo historydlg;
	WarningDlg  warningdlg;
    LocalInfoDlg localinfodlg;
	CActiveHost activehostdlg;
    ARPGuardPage arpguardpage;
	CPropertySheet	m_sheet;
	CRulePage		m_RulePage;		
	CMonitorPage	m_MonitorPage;	
	CSyssetPage		m_SyssetPage;
	CIntrusionPage  m_IntrusionPage;
	
public:
	CString m_statistic;
    int sendCount;



public:
	afx_msg void OnClose();
public:
	afx_msg void OnClosel();
public:
	afx_msg void OnFileClose();
public:
	afx_msg void OnFileAbout();
public:
	afx_msg void OnSeemac();
public:
	afx_msg void Onvcers();
public:
	afx_msg void OnAttack();
public:
	afx_msg void OnHistory();

public:
	afx_msg void OnLocalinfo();
public:
	afx_msg void OnScanhost();
public:
	afx_msg void OnAbout();
public:
	afx_msg void OnApply();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedAnnul();
public:
	afx_msg void OnWhite();
public:
	afx_msg void OnBlue();
public:
	afx_msg void OnComput();
public:
	afx_msg void On32800();
public:
	afx_msg void OnHelp();
public:
	afx_msg void OnBnClickedUpdate();
};
