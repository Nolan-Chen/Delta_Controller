// XYPlatform.cpp : 实现文件
//

#include "stdafx.h"
#include "DELTA_CONTROLLER.h"
#include "XYPlatform.h"
#include "afxdialogex.h"


// CXYPlatform 对话框

IMPLEMENT_DYNAMIC(CXYPlatform, CDialogEx)

CXYPlatform::CXYPlatform(CWnd* pParent /*=NULL*/)
: CDialogEx(CXYPlatform::IDD, pParent)
, m_XyState(_T("")), READ(0), WRITE(1)
{

}

CXYPlatform::~CXYPlatform()
{
}

void CXYPlatform::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM, m_mscomm);
	DDX_Text(pDX, IDC_XY_STATE, m_XyState);
	DDX_Control(pDX, IDC_SERIAL_PORT, m_SerialPort);
}


BEGIN_MESSAGE_MAP(CXYPlatform, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CXYPlatform::OnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_STOP, &CXYPlatform::OnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_LOOP, &CXYPlatform::OnClickedBtnLoop)
	ON_BN_CLICKED(IDC_BTN_XRZ, &CXYPlatform::OnClickedBtnXrz)
	ON_BN_CLICKED(IDC_BTN_XLZ, &CXYPlatform::OnClickedBtnXlz)
	ON_BN_CLICKED(IDC_BTN_YRZ, &CXYPlatform::OnClickedBtnYrz)
	ON_BN_CLICKED(IDC_BTN_YLZ, &CXYPlatform::OnClickedBtnYlz)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CXYPlatform 消息处理程序


void CXYPlatform::OnClickedBtnOpen()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_BTN_OPEN, str);
	CWnd* h_BtnOpen = GetDlgItem(IDC_BTN_OPEN);//指向控件的caption
	if (!m_mscomm.get_PortOpen())
	{
		m_mscomm.put_CommPort((m_SerialPort.GetCurSel() + 1));    //默认选择com3
		m_mscomm.put_Settings(_T("9600,E,7,1"));
		m_mscomm.put_InputMode(1);
		m_mscomm.put_RThreshold(1);  //参数1表示每当串口接收缓冲区中有多于或等于1个字符时将引发一个接收数据的OnComm事件
		m_mscomm.put_InBufferSize(1024);
		m_mscomm.put_OutBufferSize(1024);
		m_mscomm.put_Handshaking(0); //握手信号
		m_mscomm.put_InputLen(0);  //设置和返回input每次读出的字节数，设为0时读出接收缓冲区中的内容
		m_mscomm.put_InBufferCount(0);//设置和返回接收缓冲区的字节数，设为0时清空接收缓冲区
		m_mscomm.put_OutBufferCount(0);//设置和返回发送缓冲区的字节数，设为0时清空发送缓冲区
		m_mscomm.put_PortOpen(TRUE);//打开串口
		SetTimer(1, 300, NULL);
		if (m_mscomm.get_PortOpen())
		{
			str = _T("关闭串口");
			UpdateData(true);
			h_BtnOpen->SetWindowTextW(str);//改变按钮名称为“关闭串口”
		}
	}
	else
	{
		KillTimer(1);
		m_mscomm.put_PortOpen(false);
		str = _T("打开串口");
		UpdateData(true);
		h_BtnOpen->SetWindowTextW(str);
	}
}


void CXYPlatform::OnClickedBtnStop()
{
	// TODO:  在此添加控件通知处理程序代码
	int address = 256, byteNum = 2, data = 0;
	sendCommand(WRITE, address, byteNum, data);
}


void CXYPlatform::OnClickedBtnLoop()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strSend;
	CByteArray hexData, sendData;
	m_mscomm.put_InBufferCount(0);//设置和返回接收缓冲区的字节数，设为0时清空缓冲区
	m_mscomm.put_OutBufferCount(0);//设置和返回发送缓冲区的字节数，设为0时清空缓冲区
	BYTE commStr;
	commStr = 0x05;
	sendData.Add(commStr);
	m_mscomm.put_Output(COleVariant(sendData));//发送
	Sleep(100);

	VARIANT variantInp;
	COleSafeArray safeArrayInp;
	LONG len, k;
	BYTE rxData[512];//设置BYTE数组
	CString strtemp;
	variantInp = m_mscomm.get_Input();//读缓冲区
	safeArrayInp = variantInp; //VARIANT型变量转换为ColeSafeArray型变量
	len = safeArrayInp.GetOneDimSize();//得到有效数据长度
	for (k = 0; k < len; k++)
	{
		safeArrayInp.GetElement(&k, rxData + k);//转换为BYTE型数组
	}
	for (k = 0; k < len; k++)//将数组转换为CString型变量
	{
		BYTE bt = *(char*)(rxData);//字符型
		strtemp.Format(_T("%02X"), bt);
	}
	if (strtemp == "06")//响应请求
		MessageBox(_T("与PLC通讯正常!", "与PLC通讯检测"));
	else if (strtemp == "15")//响应错误
		MessageBox(_T("与PLC通讯不正常!", "与PLC通讯检测"));
	else
		MessageBox(_T("与PLC没有连接!", "提示"));
}


void CXYPlatform::OnClickedBtnXrz()
{
	// TODO:  在此添加控件通知处理程序代码
	int address = 256, byteNum = 2, data = 1;
	sendCommand(WRITE, address, byteNum, data);
}


void CXYPlatform::OnClickedBtnXlz()
{
	// TODO:  在此添加控件通知处理程序代码
	int address = 256, byteNum = 2, data = 2;
	sendCommand(WRITE, address, byteNum, data);
}


void CXYPlatform::OnClickedBtnYrz()
{
	// TODO:  在此添加控件通知处理程序代码
	int address = 256, byteNum = 2, data = 16;
	sendCommand(WRITE, address, byteNum, data);
}


void CXYPlatform::OnClickedBtnYlz()
{
	// TODO:  在此添加控件通知处理程序代码
	int address = 256, byteNum = 2, data = 32;
	sendCommand(WRITE, address, byteNum, data);
}


BOOL CXYPlatform::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString str;
	int i;
	for (i = 0; i < 15; i++)
	{
		str.Format(_T("com %d"), i + 1);
		m_SerialPort.InsertString(i, str);
	}
	m_SerialPort.SetCurSel(2);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CXYPlatform::OnTimer(UINT_PTR nIDEvent)//周期性检测输入端口状态
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	UpdateData(true);
	CByteArray send_data;
	BYTE commstr[11];
	commstr[0] = 0x02;
	commstr[1] = 0x30;
	int m_addressH = 0x80;//起始地址十六进制表示
	getAddress(&commstr[2], m_addressH);//将起始地址转换为ASCII码，并保存
	getNum(&commstr[6], 32);//第二个参数为字节数，转换为ASCII码，并保存
	commstr[8] = 0x03;
	getSumChk(&commstr[9], &commstr[1], 8);
	for (int i = 0; i < 11; i++)
	{
		send_data.Add(commstr[i]);
	}
	m_mscomm.put_Output(COleVariant(send_data));
	Sleep(200);

	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len, k;
	BYTE rxdata[512];//设置BYTE数组
	CString strtemp, str;
	variant_inp = m_mscomm.get_Input();//读取缓冲区
	safearray_inp = variant_inp;//VARIANT型变量转换为COleSafeArray型变量
	len = safearray_inp.GetOneDimSize();//得到有效数据长度
	for (k = 0; k < len; k++)
	{
		safearray_inp.GetElement(&k, rxdata + k);
	}
	for (k = 2; k>1; k -= 2)//读取的字节数为2
	{
		BYTE temp = *(char*)(rxdata + k);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CXYPlatform::getAddress(BYTE *data, int i)
{
	BYTE b = i & 0xf;//按位与，保留低四字节
	data[3] = (b < 10) ? (b + 0x30) : (b + 0x41 - 0xa);
	b = (i >> 4) & 0xf;//i右移4位，再按位与，即保留第五到第八位
	data[2] = (b < 10) ? (b + 0x30) : (b + 0x41 - 0xa);
	b = (i >> 8) & 0xf;
	data[1] = (b < 10) ? (b + 0x30) : (b + 0x41 - 0xa);
	b = (i >> 12) & 0xf;
	data[0] = (b < 10) ? (b + 0x30) : (b + 0x41 - 0xa);
}

void CXYPlatform::getNum(BYTE *data, int i)//i为字节数
{
	BYTE b = i & 0xf;//按位与
	data[1] = (b < 10) ? (b + 0x30) : (b + 0x41 - 0xa);
	b = (i >> 4) & 0xf;
	data[0] = (b < 10) ? (b + 0x30) : (b + 0x41 - 0xa);
}

void CXYPlatform::getData(BYTE *data, int i)
{
	BYTE b = i & 0xf;
	data[1] = (b<10) ? (b + 0x30) : (b + 0x41 - 0xa);
	b = (i >> 4) & 0xf;
	data[0] = (b<10) ? (b + 0x30) : (b + 0x41 - 0xa);
	b = (i >> 8) & 0xf;
	data[3] = (b<10) ? (b + 0x30) : (b + 0x41 - 0xa);
	b = (i >> 12) & 0xf;
	data[2] = (b<10) ? (b + 0x30) : (b + 0x41 - 0xa);
}

void CXYPlatform::getSumChk(BYTE *datades, BYTE *datascr, int len)//计算校验码
{
	int a = 0;
	for (int i = 0; i < len; i++)
	{
		a += (*(datascr + i));
	}
	getNum(datades, a);
}

void CXYPlatform::sendCommand(const short index, int address, int byteNum, int data)
{
	UpdateData(true);
	switch (index)
	{
	case 0:
		break;
	case 1:
		CByteArray send_data;
		BYTE commstr[15];
		commstr[0] = 0x02;//开始码
		commstr[1] = 0x31;//写入命令码
		getAddress(&commstr[2], address);
		getNum(&commstr[6], byteNum);
		getData(&commstr[8], data);
		commstr[12] = 0x03;//结束码
		getSumChk(&commstr[13], &commstr[1], 12);
		for (int i = 0; i < 15; i++)
		{
			send_data.Add(commstr[i]);
		}
		m_mscomm.put_Output(COleVariant(send_data));
		Sleep(100);
		break;
	}
}

void CXYPlatform::moveXLf()
{
	int address = 256, byteNum = 2, data = 8;
	sendCommand(WRITE, address, byteNum, data);
}

void CXYPlatform::moveXRf()
{
	int address = 256, byteNum = 2, data = 4;
	sendCommand(WRITE, address, byteNum, data);
}

void CXYPlatform::moveYLf()
{
	int address = 256, byteNum = 2, data = 128;
	sendCommand(WRITE, address, byteNum, data);
}

void CXYPlatform::moveYRf()
{
	int address = 256, byteNum = 2, data = 64;
	sendCommand(WRITE, address, byteNum, data);
}