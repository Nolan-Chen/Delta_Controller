// test2Dlg.h : header file
#pragma once
#include "StrokeLimitDlg.h"
#include "Monitor.h"
#include "GtsMotion.h"
#include "Exhibiton.h"
#include "Conveyor.h"
#include "XYPlatform.h"
#include "afxwin.h"

// Ctest2Dlg dialog
class CDELTA_ControllerDlg : public CDialogEx
{
// Construction
public:

	CMenu m_Menu;
	CDELTA_ControllerDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CDELTA_ControllerDlg();

// Dialog Data
	enum { IDD = IDD_Delta_Controller_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CExhibiton* pES;
	CConveyor* pCT;
	CXYPlatform* pXYPlatform;
//	int m_LoopTimes;
	double m_Rtate;
	double m_Pos_X;
	double m_Pos_Y;
	double m_Pos_Z;
	double m_Zero_R;
	double m_Zero_X;
	double m_Zero_Y;
	double m_Zero_Z;
	CStrokeLimitDlg m_iStrLimDlg;
	CMonitor m_iMonitor;
	CSliderCtrl	m_sliderVel;
	//滑条的速度...................
	CListBox* m_pResultList;
	bool OnetimeFlag;
	GtsMotion m_gtsmotion;
	Kine zerokine;

public:
	afx_msg void OnBnClickedBtstop();
	afx_msg void OnHelpAbout();
	afx_msg void OnBnClickedBthandzero();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSetStrokelimit();
	afx_msg void OnMonitorStart();
	afx_msg void OnNMCustomdrawSliderVel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtautozero();
	afx_msg void OnBnClickedBtjog1P();
	afx_msg void OnBnClickedBtjog1N();
	afx_msg void OnBnClickedBtjog2P();
	afx_msg void OnBnClickedBtjog2N();
	afx_msg void OnBnClickedBtjog3P();
	afx_msg void OnBnClickedBtjog3N();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedBtstopoff();
	afx_msg void OnClose();
//	afx_msg void OnBnClickedButton4();
	afx_msg void OnExhibition();
	afx_msg void OnConveyor();
	afx_msg void OnEnChangeEteditpositionX();
	afx_msg void OnXyplateformMonitor();

//	afx_msg void OnBnClickedButton4();
//	CListBox m_StateInform;
};
