// XYPlatform.cpp : 实现文件
//

#include "stdafx.h"
#include "DELTA_CONTROLLER.h"
#include "XYPlatform.h"
#include "afxdialogex.h"
#include "Robot.h"
#include <cmath>

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
	DDX_Control(pDX, IDC_XY_STATE, m_StateInform);
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
	ON_WM_CLOSE()
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
		CString strtemp;
		LoopTest(strtemp);
		//strtemp="27";
		if (m_mscomm.get_PortOpen())
		{
			if (strtemp == "06" || strtemp == "15")
			{
				timerInit = true;
				str = _T("关闭串口");
				UpdateData(true);
				h_BtnOpen->SetWindowTextW(str);//改变按钮名称为“关闭串口”
				StateShow(_T(">>>串口打开成功！"));
				SetTimer(1, 300, NULL);
				return;
			}
			str = _T("关闭串口");
			UpdateData(true);
			h_BtnOpen->SetWindowTextW(str);//改变按钮名称为“关闭串口”
			StateShow(_T(">>>串口打开状态不正常，请关闭后再打开！"));
			MessageBox(_T("串口打开状态不正常，请关闭后再打开！"));
			return;
		}
	}
	else
	{
		KillTimer(1);
		OnClickedBtnStop();
		m_mscomm.put_PortOpen(false);
		str = _T("打开串口");
		UpdateData(true);
		h_BtnOpen->SetWindowTextW(str);
		StateShow(_T(">>>串口已关闭！"));
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
	CString str;
	LoopTest(str);
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


void CXYPlatform::OnClickedBtnYrz()//电机运行的正方向与坐标轴正向相反
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
	m_SerialPort.SetCurSel(0);

	//m_XyState.
	m_StateInform.SetReadOnly(TRUE);
	m_StateInform.SetWindowTextW(_T(">>>请检查XY平台是否回零！"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CXYPlatform::OnTimer(UINT_PTR nIDEvent)//周期性检测输入端口状态
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	UpdateData(true);
	CString strtemp;
	static bool flag[6] = { 0 };
	readState(strtemp);
	
	if (timerInit == false && strtemp != "06" && strtemp != "15")
	{
		if (strFlag[0] == '1' && flag[0] == false)//Y轴左限位状态显示
		{
			flag[0] = true;
			StateShow(_T(">>>Y轴到达左限位！"));
			MessageBox(_T("Y轴到达左限位！"));
		}
		else if (strFlag[0] == '0'&&flag[0] == true)
		{
			flag[0] = false;
			StateShow(_T(">>>Y轴离开左限位..."));
		}

		if (strFlag[2] == '1' && flag[2] == false)//Y轴右限位状态显示
		{
			flag[2] = true;
			StateShow(_T(">>>Y轴到达右限位！"));
			MessageBox(_T("Y轴到达右限位！"));
		}
		else if (strFlag[2] == '0'&&flag[2] == true)
		{
			flag[2] = false;
			StateShow(_T(">>>Y轴离开右限位..."));
		}

		if (strFlag[1] == '1' && flag[1] == false)//Y轴零位状态显示
		{
			flag[1] = true;
			StateShow(_T(">>>Y轴回到零位！"));
		}
		else if (strFlag[1] == '0'&&flag[1] == true)
		{
			flag[1] = false;
			StateShow(_T(">>>Y轴离开零位..."));
		}

		if (strFlag[3] == '1' && flag[3] == false)//X轴左限位状态显示
		{
			flag[3] = true;
			StateShow(_T(">>>X轴到达左限位！"));
			MessageBox(_T("X轴到达左限位！"));
		}
		else if (strFlag[3] == '0'&&flag[3] == true)
		{
			flag[3] = false;
			StateShow(_T(">>>X轴离开左限位..."));
		}

		if (strFlag[5] == '1' && flag[5] == false)//X轴右限位状态显示
		{
			flag[5] = true;
			StateShow(_T(">>>X轴到达右限位！"));
			MessageBox(_T("X轴到达右限位！"));
		}
		else if (strFlag[5] == '0'&&flag[5] == true)
		{
			flag[5] = false;
			StateShow(_T(">>>X轴离开右限位..."));
		}

		if (strFlag[4] == '1' && flag[4] == false)//X轴零位状态显示
		{
			flag[4] = true;
			StateShow(_T(">>>X轴回到零位！"));
		}
		else if (strFlag[4] == '0'&&flag[4] == true)
		{
			flag[4] = false;
			StateShow(_T(">>>X轴离开零位..."));
		}
	}
	else if (strtemp == "15")
	{
		MessageBox(_T("信号发送失败，请重新发送！"));
	}
	else
	{
		timerInit = false;
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
	//UpdateData(true);
	CByteArray send_data;
	switch (index)
	{
	case 0:
		BYTE commstr0[11];
		commstr0[0] = 0x02;
		commstr0[1] = 0x30;
		getAddress(&commstr0[2], address);//将起始地址转换为ASCII码，并保存
		getNum(&commstr0[6], byteNum);//第二个参数为字节数，转换为ASCII码，并保存
		commstr0[8] = 0x03;
		getSumChk(&commstr0[9], &commstr0[1], 8);
		for (int i = 0; i < 11; i++)
		{
			send_data.Add(commstr0[i]);
		}
		m_mscomm.put_Output(COleVariant(send_data));
		Sleep(50);
		break;
	case 1:
		BYTE commstr1[15];
		commstr1[0] = 0x02;//开始码
		commstr1[1] = 0x31;//写入命令码
		getAddress(&commstr1[2], address);
		getNum(&commstr1[6], byteNum);
		getData(&commstr1[8], data);
		commstr1[12] = 0x03;//结束码
		getSumChk(&commstr1[13], &commstr1[1], 12);
		for (int i = 0; i < 15; i++)
		{
			send_data.Add(commstr1[i]);
		}
		m_mscomm.put_Output(COleVariant(send_data));
		Sleep(50);
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

void CXYPlatform::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnClose();
}

void CXYPlatform::getReturn(BYTE* rxdata, long* len)
{
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG k;
	variant_inp = m_mscomm.get_Input(); //读缓冲区
	safearray_inp = variant_inp;  //VARIANT型变量转换为ColeSafeArray型变量
	*len = safearray_inp.GetOneDimSize(); //得到有效数据长度
	for (k = 0; k<*len; k++)
	{
		safearray_inp.GetElement(&k, rxdata + k);
	}
}

void CXYPlatform::StateShow(CString str)
{
	m_StateInform.GetWindowTextW(m_XyState);
	m_XyState += _T("\r\n") + str;
	m_StateInform.SetWindowTextW(m_XyState);
}

void CXYPlatform::readState(CString& strtemp)
{
	int m_addressH, byteNum;
	LONG len, k;
	BYTE rxdata[512];//设置BYTE数组
	CString str;
	for (int i = 0; i<3; i++)
	{
		switch (i)
		{
		case 0:
			m_addressH = 0x80;//输入端口X0~X7起始地址十六进制表示
			byteNum = 1;
			break;
		case 1:
			m_addressH = 0x100;//中间继电器M0~M7起始地址十六进制表示
			byteNum = 1;
			break;
		case 2:
			m_addressH = 0x102;//中间继电器M16~M23起始地址十六进制表示
			byteNum = 1;
			break;
		}

		sendCommand(READ, m_addressH, byteNum, NULL);
		getReturn(rxdata, &len);//获取接收缓冲区的返回值

		for (k = 0; k < len; k++)//将数组转换为CString型变量
		{
			BYTE bt = *(char*)(rxdata);//字符型
			strtemp.Format(_T("%02X"), bt);
		}
		if (strtemp == "06")//响应请求
			break;
		//MessageBox(_T("与PLC通讯正常!", "与PLC通讯检测"));

		for (k = 2; k>1; k -= 2)//读取的字节数为2
		{
			BYTE temp = *(char*)(rxdata + k);
			BYTE bt1 = ((temp - 0x30)<10) ? (temp - 0x30) : (temp - 0x41 + 10);
			temp = *(char*)(rxdata + k - 1);
			BYTE bt2 = ((temp - 0x30)<10) ? (temp - 0x30) : (temp - 0x41 + 10);
			BYTE bt = bt1 + bt2 * 16;
			char p[10];
			strtemp = itoa(bt, p, 2);
			str.Format(_T("%08s,"), strtemp);
		}

		switch (i)
		{
		case 0:
			for (int i = 0, j = 7; i < 8; i++, j--)
				strFlag[j] = str[i];
			break;
		case 1:
			for (int i = 0, j = 7; i < 8; i++, j--)
				mFlag0[j] = str[i];
			break;
		case 2:
			for (int i = 0, j = 7; i < 8; i++, j--)
				mFlag16[j] = str[i];
			break;
		}
	}
}

void CXYPlatform::getXyState(int* state, double* pos)
{
		*state = 0;
		if (mFlag16[0] == '1' && mFlag16[2] == '1')//平台在位置1
		{
			*state = 1;
			*pos = -RECTANGLESIZE_X;
			*(pos + 1) = RECTANGLESIZE_Y;
		}
		if (mFlag16[2] == '1'&&strFlag[4] == '1')
		{
			*state = 2;
			*pos = 0;
			*(pos + 1) = RECTANGLESIZE_Y;
		}
		if (mFlag16[1] == '1'&&mFlag16[2] == '1')
		{
			*state = 3;
			*pos = RECTANGLESIZE_X;
			*(pos + 1) = RECTANGLESIZE_Y;
		}
		if (mFlag16[0] == '1'&&strFlag[1] == '1')
		{
			*state = 4;
			*pos = -RECTANGLESIZE_X;
			*(pos + 1) = 0;
		}
		if (strFlag[4] == '1'&&strFlag[1] == '1')
		{
			*state = 5;
			*pos = 0;
			*(pos + 1) = 0;
		}
		if (strFlag[1] == '1'&&mFlag16[1] == '1')
		{
			*state = 6;
			*pos = RECTANGLESIZE_X;
			*(pos + 1) = 0;
		}
		if (mFlag16[0] == '1'&&mFlag16[3] == '1')
		{
			*state = 7;
			*pos = -RECTANGLESIZE_X;
			*(pos + 1) = -RECTANGLESIZE_Y;
		}
		if (strFlag[4] == '1'&&mFlag16[3] == '1')
		{
			*state = 8;
			*pos = 0;
			*(pos + 1) = -RECTANGLESIZE_Y;
		}
		if (mFlag16[1] == '1' && mFlag16[3] == '1')
		{
			*state = 9;
			*pos = RECTANGLESIZE_X;
			*(pos + 1) = -RECTANGLESIZE_Y;
		}
		if (strFlag[0] == '1' || strFlag[2] == '1' || strFlag[3] == '1' || strFlag[5] == '1')
		{
			*state = 10;
			*pos = 220;
			*(pos + 1) = 220;
		}
}

void CXYPlatform::moveAutoZero()
{
	int stateFlag = 0, loopNum = 0;
	double xyPos[2];
	if (m_mscomm.get_PortOpen())
	{
		do
		{
			Sleep(1000);
			getXyState(&stateFlag, xyPos);
			loopNum++;
		} while (stateFlag != 0 || loopNum < 10);
	}

	int address = 256, byteNum = 2, data = 0;

	switch (stateFlag)
	{
	case 0:
		if (m_mscomm.get_PortOpen())
		{
			MessageBox(_T("XY平台有异常！"));
			Sleep(5000);
		}
		break;
	case 1:
		data = 17;
		sendCommand(WRITE, address, byteNum, data);
		break;
	case 2:
		OnClickedBtnYrz();
		break;
	case 3:
		data = 18;
		sendCommand(WRITE, address, byteNum, data);
		break;
	case 4:
		OnClickedBtnXrz();
		break;
	case 5:
		OnClickedBtnStop();
		break;
	case 6:
		OnClickedBtnXlz();
		break;
	case 7:
		data = 33;
		sendCommand(WRITE, address, byteNum, data);
		break;
	case 8:
		OnClickedBtnYlz();
		break;
	case 9:
		data = 34;
		sendCommand(WRITE, address, byteNum, data);
		break;
	case 10:
		OnClickedBtnStop();
		break;
	}
}

BOOL CXYPlatform::serialPortState()
{
	if (m_mscomm.get_PortOpen())
		return TRUE;
	else
		return FALSE;
}

void CXYPlatform::LoopTest(CString& str)
{
	CString strSend;
	CByteArray hexData, sendData;
	m_mscomm.put_InBufferCount(0);//设置和返回接收缓冲区的字节数，设为0时清空缓冲区
	m_mscomm.put_OutBufferCount(0);//设置和返回发送缓冲区的字节数，设为0时清空缓冲区
	BYTE commStr;
	commStr = 0x05;
	sendData.Add(commStr);
	m_mscomm.put_Output(COleVariant(sendData));//发送
	Sleep(100);

	BYTE rxData[512];//设置BYTE数组
	CString strtemp;
	long k, len;
	getReturn(rxData, &len);
	for (k = 0; k < len; k++)//将数组转换为CString型变量
	{
		BYTE bt = *(char*)(rxData);//字符型
		strtemp.Format(_T("%02X"), bt);
	}
	if (strtemp == "06")//响应请求
		MessageBox(_T("与PLC通讯正常!", "与PLC通讯检测"));
	else if (strtemp == "15")//响应错误
		MessageBox(_T("与PLC通讯不正常，请点击回路测试！", "与PLC通讯检测"));
	else
		MessageBox(_T("与PLC没有连接，请检查1、PLC是否上电 2、PC与PLC通信是否有问题！", "提示"));

	str = strtemp;
}

void CXYPlatform::moveXyPlatform(int data)
{
	int address = 256, byteNum = 2;
	/*OnClickedBtnStop();
	Sleep(100);*/
	sendCommand(WRITE, address, byteNum, data);
}

bool CXYPlatform::actionScheme(double* targetPos, double* deltaTarget)
{
	//XY平台控制逻辑
	int XyCurrentPosition, XyTargetPosition, XyData = 0;
	double pos[2], XyTarget_X, XyTarget_Y;//记录XY平台位置
	getXyState(&XyCurrentPosition, pos);
	if (XyCurrentPosition > 0 && XyCurrentPosition < 10)
	{
		double distance = std::sqrt(std::powf(std::fabs(*(targetPos + 0) - pos[0]), 2) + std::powf(std::fabs(*(targetPos + 1) - pos[1]), 2));
		if (distance <= 150)
		{
			//获取delta目标值
			*(deltaTarget+0) = *(targetPos + 1) - pos[1];
			*(deltaTarget + 1) = *(targetPos + 0) - pos[0];
			*(deltaTarget + 2) = *(targetPos + 2) - 247.0;
		}
		else
		{
			int X_Index = 0, Y_Index = 0;
			if (*(targetPos + 0) < -(RECTANGLESIZE_X / 2)) X_Index = 1;    //目标位置在那个格子里
			else if (*(targetPos + 0)>(RECTANGLESIZE_X / 2)) X_Index = 3;
			else X_Index = 2;
			if (*(targetPos + 1) > (RECTANGLESIZE_Y / 2)) Y_Index = 1;
			else if (*(targetPos + 1) < -(RECTANGLESIZE_Y / 2)) Y_Index = 3;
			else Y_Index = 2;
			//XyTargetPosition = (Y_Index - 1) * 3 + X_Index;
			XyTarget_X = (X_Index - 2) * RECTANGLESIZE_X;
			XyTarget_Y = -(Y_Index - 2) * RECTANGLESIZE_Y;

			//获取delta目标值
			*(deltaTarget + 0) = *(targetPos + 1) - XyTarget_Y;
			*(deltaTarget + 1) = *(targetPos + 0) - XyTarget_X;
			*(deltaTarget + 2) = *(targetPos + 2) - 247.0;

			//获取XY平台的指令数据
			if (XyTarget_X > pos[0]) XyData = 4;
			else if (XyTarget_X == pos[0]) XyData = 0;
			else XyData = 8;

			if (XyTarget_X*pos[0]<0) XyData += 256;//M8标志位，X轴是否跨越零位
			else XyData += 0;

			if (XyTarget_Y > pos[1]) XyData += 128;
			else if (XyTarget_Y == pos[1]) XyData += 0;
			else XyData += 64;

			if (XyTarget_Y*pos[1]<0) XyData += 512;//M9标志位，Y轴是否跨越零位
			else XyData += 0;

			moveXyPlatform(XyData);
			motorAction();
		}
	}
	else
	{
		return false;
	}
	return true;
}

void CXYPlatform::motorAction()
{
	Sleep(500);//定时器每隔300ms获取一次状态，因为电机启动后，等待500ms，确保可以正常获取电机状态
	do {} while (mFlag0[0]!='0' || mFlag0[1]!='0' || mFlag0[2]!='0' || mFlag0[3]!='0' || //双重保险
		mFlag0[4]!='0' || mFlag0[5]!='0' || mFlag0[6]!='0' || mFlag0[7]!='0' ||          //确保命令控制位已清空
		mFlag16[6] != '0' || mFlag16[7] != '0');                                         //确保电机已经到位
}