#pragma once
#include "Resource.h"

// CStrokeLimitDlg dialog

class CStrokeLimitDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStrokeLimitDlg)

public:
	CStrokeLimitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStrokeLimitDlg();

// Dialog Data
	enum { IDD = IDD_SET_StrokeLimit_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
