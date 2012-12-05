// Net DragonDlg.h : ͷ�ļ�
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

// CNetDragonDlg �Ի���
class CNetDragonDlg : public CDialog
{
// ����
public:
	CNetDragonDlg(CWnd* pParent = NULL);


// �Ի�������
	enum { IDD = IDD_NetDragon_DIALOG };



// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
