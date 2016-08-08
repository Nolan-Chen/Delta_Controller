// Monitor.cpp : implementation file
//

#include "stdafx.h"
#include "Monitor.h"
#include "afxdialogex.h"


// CMonitor dialog

IMPLEMENT_DYNAMIC(CMonitor, CDialogEx)

CMonitor::CMonitor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMonitor::IDD, pParent)
{

}

CMonitor::~CMonitor()
{
}

void CMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMonitor, CDialogEx)
	ON_EN_CHANGE(IDC_EtSPYPOSITON_X, &CMonitor::OnEnChangeEtspypositonX)
	ON_EN_CHANGE(IDC_EtSPYSPEED_X, &CMonitor::OnEnChangeEtspyspeedX)
END_MESSAGE_MAP()


// CMonitor message handlers


void CMonitor::OnEnChangeEtspypositonX()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CMonitor::OnEnChangeEtspyspeedX()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
