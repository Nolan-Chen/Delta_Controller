#pragma once
#include "resource.h"


// CMonitor dialog

class CMonitor : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitor)

public:
	CMonitor(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMonitor();

// Dialog Data
	enum { IDD = IDD_Monitor_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEtspypositonX();
	afx_msg void OnEnChangeEtspyspeedX();
};
