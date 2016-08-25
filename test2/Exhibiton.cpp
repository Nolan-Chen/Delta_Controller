// Exhibiton.cpp : implementation file
//

#include "stdafx.h"
#include "DELTA_CONTROLLER.h"
#include "Exhibiton.h"
#include "afxdialogex.h"


// CExhibiton dialog

IMPLEMENT_DYNAMIC(CExhibiton, CDialogEx)

CExhibiton::CExhibiton(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExhibiton::IDD, pParent)
	, m_RunTime_Loop(0)
{
	m_Point1_X = 125.0;
	m_Point1_Y = 125.0;
	m_Point1_Z = 400.0;
	m_Point2_X = -125.0;
	m_Point2_Y = 125.0;
	m_Point2_Z = 400.0;
	m_Point3_X = -125.0;
	m_Point3_Y = -125.0;
	m_Point3_Z = 400.0;
	m_Point4_X = 125.0;
	m_Point4_Y = -125.0;
	m_Point4_Z = 400.0;
	m_LoopTimes = 1;
	m_looptimes_al = 0;
}

CExhibiton::~CExhibiton()
{
}

void CExhibiton::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POINT1_X, m_Point1_X);
	DDX_Text(pDX, IDC_POINT1_Y, m_Point1_Y);
	DDX_Text(pDX, IDC_POINT1_Z, m_Point1_Z);
	DDV_MinMaxDouble(pDX, m_Point1_Z, 225.0, 640.0);
	DDX_Text(pDX, IDC_POINT2_X, m_Point2_X);
	DDX_Text(pDX, IDC_POINT2_Y, m_Point2_Y);
	DDX_Text(pDX, IDC_POINT2_Z, m_Point2_Z);
	DDV_MinMaxDouble(pDX, m_Point2_Z, 225.0, 640.0);
	DDX_Text(pDX, IDC_POINT3_X, m_Point3_X);
	DDX_Text(pDX, IDC_POINT3_Y, m_Point3_Y);
	DDX_Text(pDX, IDC_POINT3_Z, m_Point3_Z);
	DDV_MinMaxDouble(pDX, m_Point3_Z, 225.0, 640.0);
	DDX_Text(pDX, IDC_POINT4_X, m_Point4_X);
	DDX_Text(pDX, IDC_POINT4_Y, m_Point4_Y);
	DDX_Text(pDX, IDC_POINT4_Z, m_Point4_Z);
	DDV_MinMaxDouble(pDX, m_Point4_Z, 225.0, 640.0);
	DDX_Text(pDX, IDC_LOOP_TIMES, m_LoopTimes);
	DDX_Text(pDX, IDC_looptimes, m_looptimes_al);
	DDX_Text(pDX, IDC_RUN_TIME, m_RunTime_Loop);
}


BEGIN_MESSAGE_MAP(CExhibiton, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CExhibiton::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CExhibiton::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_looptimes, &CExhibiton::OnEnChangelooptimes)
END_MESSAGE_MAP()


// CExhibiton message handlers


void CExhibiton::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
	DestroyWindow();
}

void CExhibiton::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);         //刷新对话框，获取坐标点值
	AfxBeginThread(_threadRobotAction, (void*)this);
}

UINT __cdecl CExhibiton::_threadRobotAction(LPVOID pParam)
{
	CExhibiton *exhibition = (CExhibiton*)pParam;

	exhibition->threadExhibitionRobotAction();
	//UpdateData(FALSE);
	return 0;
}

void CExhibiton::threadExhibitionRobotAction()
{
	short flag=1;
	double Point0[3],Point1[3],Point2[3],Point3[3],Point4[3]; //当前点和四个目标点的坐标
	double P0[3];     //记录初始点
	extern double ovalue[];    //控制器零位相对于逆解零位的坐标
	extern int m_Int;

	//UpdateData(TRUE);         //刷新对话框，获取坐标点值

	//gtsmotion.WaitMotor();    //等待电机到位
	gtsmotion.GetCurPos(Point0);

	P0[0] = ovalue[0];
	P0[1] = ovalue[1];
	P0[2] = ovalue[2];

	Point1[0] = m_Point1_X;
	Point1[1] = m_Point1_Y;
	Point1[2] = m_Point1_Z;

	Point2[0] = m_Point2_X;
	Point2[1] = m_Point2_Y;
	Point2[2] = m_Point2_Z;

	Point3[0] = m_Point3_X;
	Point3[1] = m_Point3_Y;
	Point3[2] = m_Point3_Z;

	Point4[0] = m_Point4_X;
	Point4[1] = m_Point4_Y;
	Point4[2] = m_Point4_Z;

	extern long T_gap;         //步长时间间隔
	extern int n;                 //步数

	//velrate = m_deltacontrollerDlg.m_sliderVel.GetPos();

	flag = gtsmotion.Pvt_DynamicPT(Point0,Point1,1,m_Int);   //直线运动到第一点
	gtsmotion.WaitMotor();    //等待电机到位
	double t1=GetTickCount();//程序段开始前取得系统运行时间(ms)

	for (int i=0;i<m_LoopTimes;i++)
	{
		gtsmotion.GetCurPos(Point0);
		if(flag == 0)
			flag = gtsmotion.Pvt_DynamicPT(Point0,Point2,1,m_Int);   //如果前一段函数运行出错，则直线运行到第二点
		else
			flag = gtsmotion.Pvt_DynamicPT(Point0,Point2,1,m_Int);   //如果前一线段运行没错，则椭圆线到达第二点

		//gtsmotion.WaitMotor();    //等待电机到位
		gtsmotion.GetCurPos(Point0);
		if(flag == 0)
			flag = gtsmotion.Pvt_DynamicPT(Point0,Point3,1,m_Int);
		else
			flag = gtsmotion.Pvt_DynamicPT(Point0,Point3,1,m_Int);

		//gtsmotion.WaitMotor();    //等待电机到位
		gtsmotion.GetCurPos(Point0);
		if(flag == 0)
			flag = gtsmotion.Pvt_DynamicPT(Point0,Point4,1,m_Int);   //如果前一段函数运行出错，则直线运行到第四点
		else
			flag = gtsmotion.Pvt_DynamicPT(Point0,Point4,1,m_Int);   //如果前一线段运行没错，则椭圆线到达第二点

		//gtsmotion.WaitMotor();    //等待电机到位
		gtsmotion.GetCurPos(Point0);
		if(flag == 0)
			flag = gtsmotion.Pvt_DynamicPT(Point0,Point1,1,m_Int);   //如果前一段函数运行出错，则直线运行到第一点
		else
			flag = gtsmotion.Pvt_DynamicPT(Point0,Point1,1,m_Int);   //如果前一线段运行没错，则椭圆线到达第一点

		/*m_looptimes_al++;*/
	}
	gtsmotion.WaitMotor();    //等待电机到位
	double t2=GetTickCount();//程序段结束后取得系统运行时间(ms)
	m_RunTime_Loop = (double)(t2-t1)/(2*m_LoopTimes);
	m_looptimes_al = 60*1000/m_RunTime_Loop;
	//UpdateData(FALSE);

	gtsmotion.GetCurPos(Point0);//回去初始点
	flag = gtsmotion.Pvt_DynamicPT(Point0,P0,1,m_Int);
}

void CExhibiton::OnDestroy()
{
	//CDialogEx::OnDestroy();
	CDialogEx::PostNcDestroy();  
	// TODO: Add your message handler code here
	//delete this;
}


void CExhibiton::OnEnChangelooptimes()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
