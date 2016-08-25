#pragma once
#include "mscomm.h"
#include "afxwin.h"


// CXYPlatform 对话框

class CXYPlatform : public CDialogEx
{
	DECLARE_DYNAMIC(CXYPlatform)

public:
	CXYPlatform(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CXYPlatform();

// 对话框数据
	enum { IDD = IDD_XYDELTADLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	const short WRITE, READ;
	void getAddress(BYTE *data, int i);//获取软元件起始地址
	void getNum(BYTE *data, int i);//获取字节数
	void getData(BYTE *data, int i);//获取写入的数据
	void getSumChk(BYTE *datades, BYTE *datascr, int len);//获取检验和
	void sendCommand(const short index, int address, int byteNum, int data);
	void StateShow(CString str);
	void getReturn(BYTE* rxdata, long* len);
	virtual BOOL OnInitDialog();

	afx_msg void OnClickedBtnOpen();
	afx_msg void OnClickedBtnLoop();
	void LoopTest(CString* str);
	afx_msg void OnClickedBtnXrz();
	afx_msg void OnClickedBtnXlz();
	afx_msg void OnClickedBtnYrz();
	afx_msg void OnClickedBtnYlz();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();

	CMscomm m_mscomm;
	CString m_XyState;
	CComboBox m_SerialPort;
	CEdit m_StateInform;
	CString strFlag[8], mFlag0[8], mFlag16[8];
	bool timerInit;

public:
	// 串口通讯对象
	afx_msg void OnClickedBtnStop();
	void moveXLf();
	void moveXRf();
	void moveYLf();
	void moveYRf();
	void getXyState(int* state, double* pos);
	void moveAutoZero();
	void moveXyPlatform(int data);
	BOOL serialPortState();
};
