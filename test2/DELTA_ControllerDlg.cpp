
// test2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DELTA_Controller.h"
#include "DELTA_ControllerDlg.h"
#include "afxdialogex.h"
#include "conio.h"
#pragma comment(lib,"gts.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////全局变量///////////////////
bool SimulationCheck;	//仿真选择项外部传递
bool OnMotion;		//运动开关外部传递
int m_Int;   //速度条读取值

/////////////////////////VISION

CDELTA_ControllerDlg* m_iDELTADLg;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnExhibition();
	afx_msg void OnDestroy();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)


//	ON_COMMAND(ID_Exhibition, &CAboutDlg::OnExhibition)
	ON_WM_DESTROY()    
END_MESSAGE_MAP()

CAboutDlg dlgAbout;
// Ctest2Dlg dialog




CDELTA_ControllerDlg::CDELTA_ControllerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDELTA_ControllerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//初始化输入框数据
	m_LoopTimes = 1;
	m_Rtate = 0.0;
	m_Pos_X = 0.0;
	m_Pos_Y = 0.0;
	m_Pos_Z = 326.19012184371931;
	m_Zero_R = 0.0;
	m_Zero_X = 0.0;
	m_Zero_Y = 0.0;
	m_Zero_Z = 326.19012184371931;

	//指针初始化
	pXYPlatform = new CXYPlatform();
	pES = NULL;
	pCT = NULL;
}

CDELTA_ControllerDlg::~CDELTA_ControllerDlg()
{
	if(pES) 
		delete pES;
	if(pCT) 
		delete pCT;
	if (pXYPlatform)
		delete pXYPlatform;
}

void CDELTA_ControllerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCARA_ControllerDlg)
	DDX_Control(pDX, IDC_SLIDER_VEL,m_sliderVel);
	//}}AFX_DATA_MAP
	//  DDX_Text(pDX, IDC_EtEDITPOSITION_X, m_Pos_X);
	//  DDX_Text(pDX, IDC_EtEDITPOSITION_Y, m_Pos_Y);
	//  DDX_Text(pDX, IDC_EtEDITPOSITION_Z, m_Pos_Z);
	//  DDX_Text(pDX, IDC_EtEDITPOSITION_R, m_Rotate);
	DDX_Text(pDX, IDC_EtEDITPOSITION_LOOP, m_LoopTimes);
	DDX_Text(pDX, IDC_EtEDITPOSITION_R, m_Rtate);
	DDX_Text(pDX, IDC_EtEDITPOSITION_X, m_Pos_X);
	DDX_Text(pDX, IDC_EtEDITPOSITION_Y, m_Pos_Y);
	DDX_Text(pDX, IDC_EtEDITPOSITION_Z, m_Pos_Z);
	DDX_Text(pDX, IDC_EtSetZero_R, m_Zero_R);
	//  DDX_Control(pDX, IDC_EtSetZero_X, m_Zero_X);
	DDX_Text(pDX, IDC_EtSetZero_X, m_Zero_X);
	DDX_Text(pDX, IDC_EtSetZero_Y, m_Zero_Y);
	DDX_Text(pDX, IDC_EtSetZero_Z, m_Zero_Z);
}

BEGIN_MESSAGE_MAP(CDELTA_ControllerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDELTA_ControllerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDELTA_ControllerDlg::OnBnClickedCancel)
	ON_COMMAND(ID_SET_StrokeLimit, &CDELTA_ControllerDlg::OnSetStrokelimit)
	ON_COMMAND(ID_Monitor_Start, &CDELTA_ControllerDlg::OnMonitorStart)
//	ON_NOTIFY(NM_OUTOFMEMORY, IDC_SLIDER_VEL, &CDELTA_ControllerDlg::OnNMOutofmemorySliderVel)
//	ON_BN_CLICKED(IDC_BtLINEX_P, &CDELTA_ControllerDlg::OnBnClickedBtlinexP)
//	ON_BN_CLICKED(IDC_BtLINEX_N, &CDELTA_ControllerDlg::OnBnClickedBtlinexN)
	ON_BN_CLICKED(IDC_BtSTOP, &CDELTA_ControllerDlg::OnBnClickedBtstop)
	ON_COMMAND(ID_Help_About, &CDELTA_ControllerDlg::OnHelpAbout)
	ON_BN_CLICKED(IDC_BtHANDZERO, &CDELTA_ControllerDlg::OnBnClickedBthandzero)
	ON_BN_CLICKED(IDC_BUTTON2, &CDELTA_ControllerDlg::OnBnClickedButton2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_VEL, &CDELTA_ControllerDlg::OnNMCustomdrawSliderVel)
	ON_BN_CLICKED(IDC_BtAUTOZERO, &CDELTA_ControllerDlg::OnBnClickedBtautozero)
	ON_BN_CLICKED(IDC_BtJog1_P, &CDELTA_ControllerDlg::OnBnClickedBtjog1P)
	ON_BN_CLICKED(IDC_BtJog1_N, &CDELTA_ControllerDlg::OnBnClickedBtjog1N)
	ON_BN_CLICKED(IDC_BtJog2_P, &CDELTA_ControllerDlg::OnBnClickedBtjog2P)
	ON_BN_CLICKED(IDC_BtJog2_N, &CDELTA_ControllerDlg::OnBnClickedBtjog2N)
	ON_BN_CLICKED(IDC_BtJog3_P, &CDELTA_ControllerDlg::OnBnClickedBtjog3P)
	ON_BN_CLICKED(IDC_BtJog3_N, &CDELTA_ControllerDlg::OnBnClickedBtjog3N)
	ON_BN_CLICKED(IDC_BUTTON3, &CDELTA_ControllerDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BtStopOff, &CDELTA_ControllerDlg::OnBnClickedBtstopoff)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON4, &CDELTA_ControllerDlg::OnBnClickedButton4)
	ON_COMMAND(ID_Exhibition, &CDELTA_ControllerDlg::OnExhibition)
	ON_COMMAND(ID_Conveyor, &CDELTA_ControllerDlg::OnConveyor)
	ON_EN_CHANGE(IDC_EtEDITPOSITION_X, &CDELTA_ControllerDlg::OnEnChangeEteditpositionX)
	ON_COMMAND(ID_XYPLATEFORM_MONITOR, &CDELTA_ControllerDlg::OnXyplateformMonitor)
END_MESSAGE_MAP()


// Ctest2Dlg message handlers

BOOL CDELTA_ControllerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	m_Menu.LoadMenu(IDR_MENU1);  
	ModifyStyle(WS_THICKFRAME, 0, SWP_FRAMECHANGED|SWP_DRAWFRAME);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_iDELTADLg = this;

	UpdateData(FALSE);

	//初始化指针变量
	pES = NULL;
	pCT = NULL;

	//设置速度滑条的初始状态
	OnMotion = TRUE;
	m_sliderVel.SetRange(2, 100);
	m_sliderVel.SetTicFreq(1);
	m_sliderVel.SetPageSize(5);
	m_sliderVel.SetPos(20);
	m_Int=m_sliderVel.GetPos();
	CString str = _T("30%");
	SetDlgItemText(IDC_STATIC_VEL, str);
//	HICON m_hicn1 = AfxGetApp()->LoadIcon(IDI_ESTOP);
//	m_EStop.SetIcon(m_hicn1);

	//初始化控制器和驱动器
	m_pResultList=(CListBox*)GetDlgItem(IDC_DISPLAY);
	if (m_gtsmotion.MotionInit())
	{
		int index = m_pResultList->InsertString (-1,_T(">>通讯成功，初始化成功"));
		m_pResultList->SetCurSel(index);
		OnBnClickedBthandzero();
		//启动关节监控
		OnetimeFlag = TRUE;
		SetTimer(1,50,NULL);
	}
	else
	{
		int index = m_pResultList->InsertString(-1,_T(">>通讯失败，初始化失败"));
		m_pResultList->SetCurSel(index);
		extern double wayPoint2[];   //路径点关节转角数组序列
		extern long T_gap;         //步长时间间隔
		extern int n;                 //步数
		int flag;
		Trajectory tratest;
		CString strtest;

		/*flag = zerokine.IKine(P1,P0);
		flag = zerokine.FKine(P0,P1);*/
		//flag = tratest.StraightLine(P0,P1);
		//flag = gtsmotion.Pvt_CompleteLoop(P0,P1,1,0);
		/*strtest.Format(_T("%d"),flag);
		index = m_pResultList->AddString(strtest);
		m_pResultList->SetCurSel(index);*/
	}

	pXYPlatform->Create(IDD_XYDELTADLG);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDELTA_ControllerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDELTA_ControllerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDELTA_ControllerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDELTA_ControllerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CDELTA_ControllerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}



void CDELTA_ControllerDlg::OnSetStrokelimit()
{
	// TODO: Add your command handler code here
	m_iStrLimDlg.DoModal();
}

void CDELTA_ControllerDlg::OnMonitorStart()
{
	// TODO: Add your command handler code here
	m_iMonitor.DoModal();
}

void CDELTA_ControllerDlg::OnExhibition()
{
	// TODO: Add your command handler code here
	//采用成员变量创建一个非模态对话框
	if(pES)delete pES;
	pES = new CExhibiton();   //给指针分配内存
	pES->Create(IDD_Exhibition_Show); //创建一个非模态对话框
	pES->ShowWindow(SW_SHOWNORMAL);  //显示非模态对话框
}

void CDELTA_ControllerDlg::OnBnClickedBtstop()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_gtsmotion.SmoothStop())
	{
		int index = m_pResultList->InsertString(-1, _T(">>停止运动"));
		m_pResultList->SetCurSel(index);
	}	
}


void CDELTA_ControllerDlg::OnHelpAbout()
{
	// TODO: 在此添加命令处理程序代码
	dlgAbout.DoModal();
}

void CDELTA_ControllerDlg::OnNMCustomdrawSliderVel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_Int=m_sliderVel.GetPos();
	UpdateData(FALSE);
	CString str;
	str.Format(_T("%d"),m_Int);
	str = str+'%';
	SetDlgItemText(IDC_STATIC_VEL, str);
	*pResult = 0;
}

	double ovalue[3];    //控制器零位相对于逆解零位的坐标
	double zeroposAng[3];//相对零位的关节角
	double P0[3],P1[3];    //线段起点与终点

void CDELTA_ControllerDlg::OnBnClickedBthandzero()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL gtsflag;
	int zeroflag;
	gtsflag = gtsmotion.SetZero();

	UpdateData(TRUE);

	//将起点、终点初始化为零点
	P1[0] = P0[0] = ovalue[0] = m_Zero_X;
	P1[1] = P0[1] = ovalue[1] = m_Zero_Y;
	P1[2] = P0[2] = ovalue[2] = m_Zero_Z;

	zeroflag = zerokine.IKine(ovalue,zeroposAng);
}


void CDELTA_ControllerDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	short flag;
	double tranp0[3];
	short int velrate;

	UpdateData(TRUE);         //刷新对话框，获取目标位置值
	gtsmotion.GetCurPos(tranp0);  //得到当前坐标值
	velrate = m_sliderVel.GetPos();

	//获取目标值
	P1[0] = m_Pos_X;
	P1[1] = m_Pos_Y;
	P1[2] = m_Pos_Z;

	P0[0] = tranp0[0];
	P0[1] = tranp0[1];
	P0[2] = tranp0[2];

	/*flag = gtsmotion.Pvt_CompleteLoop(P0,P1,m_Int,m_LoopTimes);*/
	flag = gtsmotion.Pvt_DynamicPT(P0,P1,1,velrate);

	if (flag == 0)
	{
		int index = m_pResultList->InsertString (-1,_T(">>超出工作空间，请重新输入"));
		m_pResultList->SetCurSel(index);
	}
}



void CDELTA_ControllerDlg::OnBnClickedBtautozero()
{
	// TODO: 在此添加控件通知处理程序代码
	short flag;
	extern double ovalue[];
	double tranp0[3];
	short int velrate;

	UpdateData(TRUE);         //刷新对话框，获取目标位置值
	gtsmotion.GetCurPos(tranp0);  //得到当前坐标值
	velrate = m_sliderVel.GetPos();

	//获取目标值
	P1[0] = ovalue[0];
	P1[1] = ovalue[1];
	P1[2] = ovalue[2];

	P0[0] = tranp0[0];
	P0[1] = tranp0[1];
	P0[2] = tranp0[2];

	/*flag = gtsmotion.Pvt_CompleteLoop(P0,P1,m_Int,1);*/
	flag = gtsmotion.Pvt_DynamicPT(P0,P1,1,velrate);

	if (flag == 0)
	{
		int index = m_pResultList->InsertString (-1,_T(">>超出工作空间"));
		m_pResultList->SetCurSel(index);
	}
}


void CDELTA_ControllerDlg::OnBnClickedBtjog1P()
{
	// TODO: Add your control notification handler code here
	gtsmotion.Jog_Test(AXIS_X,'P');
}


void CDELTA_ControllerDlg::OnBnClickedBtjog1N()
{
	// TODO: Add your control notification handler code here
	gtsmotion.Jog_Test(AXIS_X,'N');
}


void CDELTA_ControllerDlg::OnBnClickedBtjog2P()
{
	// TODO: Add your control notification handler code here
	gtsmotion.Jog_Test(AXIS_Y,'P');
}


void CDELTA_ControllerDlg::OnBnClickedBtjog2N()
{
	// TODO: Add your control notification handler code here
	gtsmotion.Jog_Test(AXIS_Y,'N');
}


void CDELTA_ControllerDlg::OnBnClickedBtjog3P()
{
	// TODO: Add your control notification handler code here
	gtsmotion.Jog_Test(AXIS_Z,'P');
}


void CDELTA_ControllerDlg::OnBnClickedBtjog3N()
{
	// TODO: Add your control notification handler code here
	gtsmotion.Jog_Test(AXIS_Z,'N');
}


void CDELTA_ControllerDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	m_gtsmotion.Ena_Stop('E');
}


void CDELTA_ControllerDlg::OnBnClickedBtstopoff()
{
	// TODO: Add your control notification handler code here
	m_gtsmotion.Ena_Stop('S');
}


void CDELTA_ControllerDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_gtsmotion.Ena_Stop('C');
	CDialogEx::OnClose();
}


void CDELTA_ControllerDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here


	UpdateData(TRUE);         //刷新对话框，获取循环次数

	extern long T_gap;         //步长时间间隔
	extern int n;                 //步数
	for (int i=0;i<m_LoopTimes;i++)
	{
		OnBnClickedButton2();
		OnBnClickedBtautozero();
	}
}


void CAboutDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: Add your message handler code here
	delete this;
}


void CDELTA_ControllerDlg::OnConveyor()
{
	// TODO: 在此添加命令处理程序代码
	//采用成员变量创建一个非模态对话框
	if(pCT)delete pCT;
	pCT = new CConveyor();   //给指针分配内存
	pCT->Create(IDD_Conveyor1); //创建一个非模态对话框
	pCT->ShowWindow(SW_SHOWNORMAL);  //显示非模态对话框
}


void CDELTA_ControllerDlg::OnEnChangeEteditpositionX()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CDELTA_ControllerDlg::OnXyplateformMonitor()
{
	// TODO:  在此添加命令处理程序代码
	pXYPlatform->ShowWindow(SW_SHOWNORMAL);
}
