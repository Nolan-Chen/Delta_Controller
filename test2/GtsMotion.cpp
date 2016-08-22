#include "StdAfx.h"
#include <math.h>
#include "GtsMotion.h"



GtsMotion::GtsMotion(void)
{}

GtsMotion::~GtsMotion(void)
{}

bool GtsMotion::MotionInit()
{
	short sRtn;
	sRtn = GT_Open();  // 打开运动控制器
	// 复位控制器
	sRtn += GT_Reset();   
	// 配置各轴为模拟量输出(闭环)模式
	/*sRtn += GT_CtrlMode(AXIS_X, 0);
	sRtn += GT_CtrlMode(AXIS_Y, 0);
	sRtn += GT_CtrlMode(AXIS_Z, 0);*/

	// 配置各轴为脉冲量输出(开环)模式
	/*sRtn += GT_CtrlMode(AXIS_X, 1);
	sRtn += GT_CtrlMode(AXIS_Y, 1);
	sRtn += GT_CtrlMode(AXIS_Z, 1);
*/
	// 配置运动控制器
	// 注意：配置文件取消了各轴的报警和限位
	sRtn += GT_LoadConfig("pulse_dir_5.cfg");
	Sleep(100);
	// 清除各轴的报警和限位
	sRtn += GT_ClrSts(AXIS_X, AXIS_Z);     
	//设置各轴PID参数
	TPid pid;
	//获取各轴PID参数
	sRtn = GT_GetPid(AXIS_X,1,&pid);
	sRtn = GT_GetPid(AXIS_Y,1,&pid);
	sRtn = GT_GetPid(AXIS_Z,1,&pid);

	pid.kp=5;
	pid.kd=5;
	pid.kvff=3000;
	pid.derivativeLimit=0;
	pid.integralLimit=32767;
	pid.limit=32767;

	//更新各轴的PID参数
	sRtn += GT_SetPid(AXIS_X, 1, &pid);
	sRtn += GT_SetPid(AXIS_Y, 1, &pid);
	sRtn += GT_SetPid(AXIS_Z, 1, &pid); 

	//位置清零
	sRtn += GT_ZeroPos(AXIS_X,AXIS_Z);
	sRtn += GT_SetEncPos(AXIS_X,0);
	sRtn += GT_SetEncPos(AXIS_Y,0);
	sRtn += GT_SetEncPos(AXIS_Z,0);

	//设置到位误差带
	sRtn = GT_SetAxisBand(AXIS_X,2,20);
	sRtn = GT_SetAxisBand(AXIS_Y,2,20);
	sRtn = GT_SetAxisBand(AXIS_Z,2,20);

	//设置平滑和紧急停止加速度
	sRtn += GT_SetStopDec(AXIS_X,SmoothStopDec,EstopDec);
	sRtn += GT_SetStopDec(AXIS_Y,SmoothStopDec,EstopDec);
	sRtn += GT_SetStopDec(AXIS_Z,SmoothStopDec,EstopDec);

	//设置正负限位
	sRtn += GT_SetSoftLimit(AXIS_X,150000,-50000);
	sRtn += GT_SetSoftLimit(AXIS_Y,150000,-50000);
	sRtn += GT_SetSoftLimit(AXIS_Z,150000,-50000);
	if (sRtn == 0)
	{
		waitMotorFlag = true;
		return TRUE;
	}
	else
		return FALSE;
}

bool GtsMotion::SetZero()
{
	//位置清零
	short sRtn;
	sRtn = GT_ZeroPos(AXIS_X,AXIS_Z);  //规划器清零
	sRtn = GT_SetEncPos(AXIS_X,0);//编码器清零
	sRtn += GT_SetEncPos(AXIS_Y,0);
	sRtn += GT_SetEncPos(AXIS_Z,0);

	if (sRtn == 0)
	{
		return TRUE;
	}
	else
		return FALSE;
}

void GtsMotion::WaitMotor()
{
	//等待电机运动到位，进入误差带
	//waitMotorFlag = false;
	do 
	{
		sRtn = GT_GetSts(AXIS_X,&sts,3);
	} while (0x800!=(sts & 0x800));
	//waitMotorFlag = true;
}

bool GtsMotion::GetCurPos(double* curpos)
{
	double pVal[AXIS_Z];
	double curAng[3];
	double curcord[3];
	extern double zeroposAng[];
	int flag;

	WaitMotor();//等待电机运动到位，进入误差带
	sRtn = GT_GetEncPos(AXIS_X,pVal,AXIS_Z);//获取AXIS轴的编码器位置

	//获取关节转角：已转过角度加上零位时候的角度
	curAng[0] = (((pVal[AXIS_X-1]/PULSE)/RIGIDRATIO)*ANGLE)+zeroposAng[0];
	curAng[1] = (((pVal[AXIS_Y-1]/PULSE)/RIGIDRATIO)*ANGLE)+zeroposAng[1];
	curAng[2] = (((pVal[AXIS_Z-1]/PULSE)/RIGIDRATIO)*ANGLE)+zeroposAng[2];

	//调用运动学正解得到坐标位置
	flag = kine.FKine(curAng,curcord);
	curpos[0] = curcord[0];//传递坐标值
	curpos[1] = curcord[1];
	curpos[2] = curcord[2];
	return true;
}
bool GtsMotion::GetConveyorPos(OUT double* pos)
{
	return true;
}

bool GtsMotion::ConCalibrate()
{
	return true;
}

bool GtsMotion::VisCalibrate()
{
	return true;
}

short GtsMotion::Compensation(IN double POS[3],IN short EndPostype)
{
	
	return 0;
}

short GtsMotion::P2P_COM(IN double POS[3])
{
	TTrapPrm trap;
	double prfPos;

	double testpos[3];
	testpos[0] = POS[AXIS_X-1];
	testpos[0] = POS[AXIS_Y-1];
	testpos[0] = POS[AXIS_Z-1];

	sRtn = GT_PrfTrap(AXIS_X);   //将相应轴设为点位模式
	sRtn = GT_PrfTrap(AXIS_Y); 
	sRtn = GT_PrfTrap(AXIS_Z); 

	sRtn = GT_GetTrapPrm(AXIS_X,&trap);   //读取点位运动参数
	sRtn = GT_GetTrapPrm(AXIS_Y,&trap);
	sRtn = GT_GetTrapPrm(AXIS_Z,&trap);

	trap.acc = 10;
	trap.dec = 10;
	trap.smoothTime = 2;

	//设置点位运动参数
	sRtn = GT_SetTrapPrm(AXIS_X,&trap);
	sRtn = GT_SetTrapPrm(AXIS_Y,&trap);
	sRtn = GT_SetTrapPrm(AXIS_Z,&trap);

	//设置轴的目标位置
	sRtn = GT_SetPos(AXIS_X,POS[AXIS_X-1]);
	sRtn = GT_SetPos(AXIS_Y,POS[AXIS_Y-1]);
	sRtn = GT_SetPos(AXIS_Z,POS[AXIS_Z-1]);

	//设置轴的目标速度
	sRtn = GT_SetVel(AXIS_X,50);
	sRtn = GT_SetVel(AXIS_Y,50);
	sRtn = GT_SetVel(AXIS_Z,50);

	//启动AXIS轴的运动
	sRtn = GT_Update(1<<(AXIS_X-1));
	sRtn = GT_Update(1<<(AXIS_Y-1));
	sRtn = GT_Update(1<<(AXIS_Z-1));

	return 0;
}

bool GtsMotion::SmoothStop()
{
	sRtn=GT_Stop(15,0);//所有轴紧急停止
	if (sRtn == 0)
	{
		return TRUE;
	}
	else
		return FALSE;
}

short GtsMotion::Pvt_CompleteLoop(IN double Point_0[3],IN double Point_1[3],IN double VelRatio,IN int LoopNum)
{
	long mask = 0;
	int flag;        //轨迹规划函数返回值
	double time[1024];
	extern double wayPoint1[];   //路径点关节转角数组序列
	extern double wayPoint2[];
	extern double wayPoint3[];
	extern long T_gap;         //步长时间间隔
	extern int  n;                 //步数
	short table_x = TABLEX;
	short table_y = TABLEY;
	short table_z = TABLEZ;
	extern double ovalue[];    //控制器零位相对于逆解零位的坐标
	extern double zeroposAng[];//相对零位的关节角

	Trajectory traline;
	for (int i=0;i<3;i++)        //每个控制位置加上初始零位坐标
	{
		Point_0[i] = Point_0[i]+ovalue[i];
		Point_1[i] = Point_1[i]+ovalue[i];
	}
	flag = traline.StraightLine(Point_0,Point_1,VelRatio);

	for (int i=0;i<=2*n;i++)     
	{
		time[i] = i*(T_gap*100/VelRatio);
		wayPoint1[i] = (wayPoint1[i]-zeroposAng[0])*PULSE*RIGIDRATIO/ANGLE; //每个计算值减去初始零位关节角
		wayPoint2[i] = (wayPoint2[i]-zeroposAng[1])*PULSE*RIGIDRATIO/ANGLE; //再乘上脉冲当量，计算脉冲数
		wayPoint3[i] = (wayPoint3[i]-zeroposAng[2])*PULSE*RIGIDRATIO/ANGLE;
	}


	//A1轴中间计算值
	double a1[1024],b1[1024],c1[1024];
	//A2轴中间计算值
	double a2[1024],b2[1024],c2[1024];
	//A3轴中间计算值
	double a3[1024],b3[1024],c3[1024];

	//设置为PVT模式
	sRtn = GT_PrfPvt(AXIS_X);
	sRtn = GT_PrfPvt(AXIS_Y);
	sRtn = GT_PrfPvt(AXIS_Z);

	//发送数据
	sRtn = GT_PvtTableComplete(table_x,2*n,&time[0],&wayPoint1[0],&a1[0],&b1[0],&c1[0],0,0);
	sRtn = GT_PvtTableComplete(table_y,2*n,&time[0],&wayPoint2[0],&a2[0],&b2[0],&c2[0],0,0);
	sRtn = GT_PvtTableComplete(table_z,2*n,&time[0],&wayPoint3[0],&a3[0],&b3[0],&c3[0],0,0);

	// 选择数据表
	sRtn = GT_PvtTableSelect(AXIS_X, table_x); 
	sRtn = GT_PvtTableSelect(AXIS_Y, table_y); 
	sRtn = GT_PvtTableSelect(AXIS_Z, table_z); 

	//设置为循环执行
	sRtn = GT_SetPvtLoop(AXIS_X,LoopNum);
	sRtn = GT_SetPvtLoop(AXIS_Y,LoopNum);
	sRtn = GT_SetPvtLoop(AXIS_Z,LoopNum);

	//启动运动控制卡
	mask += 1<<(AXIS_X-1);
	mask += 1<<(AXIS_Y-1);
	mask += 1<<(AXIS_Z-1);
	sRtn = GT_PvtStart(mask);

	if (sRtn == 0)
	{
		return FALSE;
	}
	else{return TRUE;}
}

short GtsMotion::Pvt_PointToPoint(IN double Point_0[3],IN double Point_1[3],IN double VelRatio,IN int LoopNum)
{
	long mask;
	int flag;        //轨迹规划函数返回值
	double time[1024];
	extern double wayPoint1[];   //路径点关节转角数组序列
	extern double wayPoint2[];
	extern double wayPoint3[];
	extern long T_gap;         //步长时间间隔
	extern int  n;                 //步数
	short table_x = TABLEX;
	short table_y = TABLEY;
	short table_z = TABLEZ;
	extern double ovalue[];    //控制器零位相对于逆解零位的坐标
	extern double zeroposAng[];//相对零位的关节角

	Trajectory traline;
	for (int i=0;i<3;i++)        //每个控制位置加上初始零位坐标
	{
		Point_0[i] = Point_0[i]+ovalue[i];
		Point_1[i] = Point_1[i]+ovalue[i];
	}
	flag = traline.StraightLine(Point_0,Point_1,VelRatio);

	for (int i=0;i<=n;i++)     
	{
		time[i] = i*T_gap;
		wayPoint1[i] = (wayPoint1[i]-zeroposAng[0])*PULSE*RIGIDRATIO/ANGLE; //每个计算值减去初始零位关节角
		wayPoint2[i] = (wayPoint2[i]-zeroposAng[1])*PULSE*RIGIDRATIO/ANGLE; //再乘上脉冲当量，计算脉冲数
		wayPoint3[i] = (wayPoint3[i]-zeroposAng[2])*PULSE*RIGIDRATIO/ANGLE;
	}

	double a1[1024],b1[1024],c1[1024];//A1轴中间计算值
	double a2[1024],b2[1024],c2[1024];//A2轴中间计算值
	double a3[1024],b3[1024],c3[1024];//A3轴中间计算值

	//设置为PVT模式
	sRtn = GT_PrfPvt(AXIS_X);
	sRtn = GT_PrfPvt(AXIS_Y);
	sRtn = GT_PrfPvt(AXIS_Z);

	//发送数据
	sRtn = GT_PvtTableComplete(table_x,n,&time[0],&wayPoint1[0],&a1[0],&b1[0],&c1[0],0,0);
	sRtn = GT_PvtTableComplete(table_y,n,&time[0],&wayPoint2[0],&a2[0],&b2[0],&c2[0],0,0);
	sRtn = GT_PvtTableComplete(table_z,n,&time[0],&wayPoint3[0],&a3[0],&b3[0],&c3[0],0,0);

	// 选择数据表
	sRtn = GT_PvtTableSelect(AXIS_X, table_x); 
	sRtn = GT_PvtTableSelect(AXIS_Y, table_y); 
	sRtn = GT_PvtTableSelect(AXIS_Z, table_z); 

	//设置为循环执行
	sRtn = GT_SetPvtLoop(AXIS_X,1);
	sRtn = GT_SetPvtLoop(AXIS_Y,1);
	sRtn = GT_SetPvtLoop(AXIS_Z,1);

	//启动运动控制卡
	mask = 1<<(AXIS_X-1);
	mask = 1<<(AXIS_Y-1);
	mask = 1<<(AXIS_Z-1);
	sRtn = GT_PvtStart(mask);

	if (sRtn == 0)
	{
		return FALSE;
	}
	else{return TRUE;}
}

short GtsMotion::Jog_Test(short Axis,char dir)
{
	TJogPrm jog;
	double prfPos,prfVel;

	sRtn = GT_PrfJog(Axis);    //将Axis轴设置为Jog模式
	sRtn = GT_GetJogPrm(Axis,&jog); //读取Jog运动参数
	jog.acc = 0.0625;
	jog.dec = 0.0625;

	sRtn = GT_SetJogPrm(Axis,&jog); //设置Jog运动参数
	if (dir == 'P') sRtn = GT_SetVel(Axis,5);
	else sRtn = GT_SetVel(Axis,-5);
	sRtn = GT_Update(1<<(Axis-1));

	if (sRtn == 0)
	{
		return 0;
	}
	else return 1;
}

void GtsMotion::Ena_Stop(char index)
{
	switch (index)
	{
	case 'E'://驱动器使能
		sRtn += GT_AxisOn(AXIS_X);
		sRtn += GT_AxisOn(AXIS_Y);
		sRtn += GT_AxisOn(AXIS_Z);
		break;
	case 'S'://驱动器使能关闭
		sRtn += GT_AxisOff(AXIS_X);
		sRtn += GT_AxisOff(AXIS_Y);
		sRtn += GT_AxisOff(AXIS_Z);
		break;
	case 'C'://驱动器使能关闭
		sRtn += GT_AxisOff(AXIS_X);
		sRtn += GT_AxisOff(AXIS_Y);
		sRtn += GT_AxisOff(AXIS_Z);
		GT_Close();  //关闭运动控制卡
		break;
	}
}

short GtsMotion::Pvt_DynamicPT(IN double Point_0[3],IN double Point_1[3],IN short linetype,short velrate)
{
	short space;
	int flag;        //轨迹规划函数返回值
	long time[1024]={0};
	short int vel = 0;
	extern double wayPoint1[];   //路径点关节转角数组序列
	extern double wayPoint2[];
	extern double wayPoint3[];
	extern long T_gap;         //步长时间间隔
	extern int  n;                 //步数
	extern double zeroposAng[];//相对零位的关节角
	Trajectory traline;

	vel = 4*velrate;

	if(linetype==1)flag = traline.StraightLine(Point_0,Point_1,vel);
	else if(linetype==2)flag = traline.EllipseLine(Point_0,Point_1,vel);
	if (flag==0)return 0;

	double testzeroang[3][1024];   //检验给定的脉冲值是否有错
	double testpos[3];

	testpos[0] = wayPoint1[0];
	testpos[1] = wayPoint2[0];
	testpos[2] = wayPoint3[0];

	wayPoint1[0] = (wayPoint1[0]-zeroposAng[0])*PULSE*RIGIDRATIO/ANGLE;
	wayPoint2[0] = (wayPoint2[0]-zeroposAng[1])*PULSE*RIGIDRATIO/ANGLE;
	wayPoint3[0] = (wayPoint3[0]-zeroposAng[2])*PULSE*RIGIDRATIO/ANGLE;

	for (int i=1;i<=n;i++)     
	{
		time[i] = (long)(i*T_gap);

		wayPoint1[i] = (wayPoint1[i]-zeroposAng[0])*PULSE*RIGIDRATIO/ANGLE-wayPoint1[0]; //每个计算值减去初始零位关节角
		wayPoint2[i] = (wayPoint2[i]-zeroposAng[1])*PULSE*RIGIDRATIO/ANGLE-wayPoint2[0]; //再乘上脉冲当量，计算脉冲数
		wayPoint3[i] = (wayPoint3[i]-zeroposAng[2])*PULSE*RIGIDRATIO/ANGLE-wayPoint3[0];

		testzeroang[0][i] = wayPoint1[i];
		testzeroang[1][i] = wayPoint2[i];
		testzeroang[2][i] = wayPoint3[i];
	}
	testzeroang[0][0] = wayPoint1[0];
	testzeroang[1][0] = wayPoint2[0];
	testzeroang[2][0] = wayPoint3[0];

	//将AXIS轴设置为PT动态模式
	sRtn = GT_PrfPt(AXIS_X,PT_MODE_STATIC);
	sRtn = GT_PrfPt(AXIS_Y,PT_MODE_STATIC);
	sRtn = GT_PrfPt(AXIS_Z,PT_MODE_STATIC);

	//清空AXIS轴的FIFO
	sRtn = GT_PtClear(AXIS_X);
	sRtn = GT_PtClear(AXIS_Y);
	sRtn = GT_PtClear(AXIS_Z);

	//设置FIFO大小
	sRtn = GT_SetPtMemory(AXIS_X,1);
	sRtn = GT_SetPtMemory(AXIS_Y,1);
	sRtn = GT_SetPtMemory(AXIS_Z,1);

	for (int i=1;i<=n;i++)
	{
		sRtn = GT_PtSpace(AXIS_X,&space);
		if (space>0)
		{
				sRtn = GT_PtData(AXIS_X,wayPoint1[i],time[i]);
				sRtn = GT_PtData(AXIS_Y,wayPoint2[i],time[i]);
				sRtn = GT_PtData(AXIS_Z,wayPoint3[i],time[i]);
		}
		else break;
	}

	//启动电机
	//AfxBeginThread(_thread_Robot_Action, (void*)this);
	mask = 0;
	mask += 1<<(AXIS_X-1);
	mask += 1<<(AXIS_Y-1);
	mask += 1<<(AXIS_Z-1);
	sRtn = GT_PtStart(mask);
	//WaitMotor();//等待电机运动到位，进入误差带
	return 1;
}

UINT __cdecl GtsMotion::_thread_Robot_Action(LPVOID pParam)
{
	GtsMotion *gts = (GtsMotion*)pParam;

	gts->threadFuctionRobotAction();
	return 0;
}

void GtsMotion::threadFuctionRobotAction()
{
	mask = 0;
	mask += 1<<(AXIS_X-1);
	mask += 1<<(AXIS_Y-1);
	mask += 1<<(AXIS_Z-1);
	sRtn = GT_PtStart(mask);
	WaitMotor();//等待电机运动到位，进入误差带
}