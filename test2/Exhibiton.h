#pragma once
#include "GtsMotion.h"
//#include "DELTA_ControllerDlg.h"

// CExhibiton dialog

class CExhibiton : public CDialogEx
{
	DECLARE_DYNAMIC(CExhibiton)
private:
	static UINT __cdecl _threadRobotAction(LPVOID pParam);

public:
	CExhibiton(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExhibiton();

// Dialog Data
	enum { IDD = IDD_Exhibition_Show };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	double m_Point1_X;
	double m_Point1_Y;
	double m_Point1_Z;
	double m_Point2_X;
	double m_Point2_Y;
	double m_Point2_Z;
	double m_Point3_X;
	double m_Point3_Y;
	double m_Point3_Z;
	double m_Point4_X;
	double m_Point4_Y;
	double m_Point4_Z;
	ULONGLONG m_LoopTimes;
	//CDELTA_ControllerDlg m_deltacontrollerDlg;
	void threadExhibitionRobotAction();

	GtsMotion gtsmotion;
	ULONGLONG m_looptimes_al;
	double m_RunTime_Loop;
	afx_msg void OnEnChangelooptimes();
};
