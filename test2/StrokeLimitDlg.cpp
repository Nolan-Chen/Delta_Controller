// StrokeLimitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StrokeLimitDlg.h"
#include "afxdialogex.h"


// CStrokeLimitDlg dialog

IMPLEMENT_DYNAMIC(CStrokeLimitDlg, CDialogEx)

CStrokeLimitDlg::CStrokeLimitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStrokeLimitDlg::IDD, pParent)
{

}

CStrokeLimitDlg::~CStrokeLimitDlg()
{
}

void CStrokeLimitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStrokeLimitDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CStrokeLimitDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CStrokeLimitDlg message handlers


void CStrokeLimitDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
