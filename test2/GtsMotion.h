#pragma once
/********************************************************
*		Robot运动学定义									*
*		GDUT 2015										*
*		Author：CHEN JI HUI								*
*		VERSION number:	1.0								*
*		DATE:      2015-04-08							*
********************************************************/
#include "Robot.h"
#include "gts.h"
#include "Kine.h"
#include "Trajectory.h"
#include "Clibration.h"

/********************************************************
 *   运动学基类                                         *
 *******************************************************/
//运动参数
struct MotionPara
{
	short Axis[4];
	double JTag[4];		//目标关节角
	double JCurr[4];	//目前关节角
	double Vel;
	double ACC;
	double Runtime;
};

class GtsMotion
{
private:
	short sRtn;
	long sts;
	int MotionType[100];
	MotionPara motionpara[100];
	int MotionTime;
	Kine kine;
	long mask;
	bool waitMotorFlag;
	static UINT __cdecl _thread_Robot_Action(LPVOID pParam);

public:
	bool Effector(bool Switch);
	void GetJointAcc(double* JAcc);
	void GetJointVel(double* JVel);
	void Personal();
	void P2PJointTime(double* JTag, long RunTime);
	void J_PulseToAngle(short Axis, long Pulse);
	void GetSoftLimit(double* LimitP, double* LimitN);
	long J_AngleToPulse(short Axis, double Angle);
	void SetupJointLimit(int* JLimitP, int* JLimitN);
	void GetCPosPrf(double* CPosPrf);
	void GetJointPrfPos(double* JPrfPos);
	bool GetCurPos(double curpos[]);
	bool GetConveyorPos(OUT double* pos);//读取传送带辅助编码器读数
	bool SingleJointTrapVel(short AXIS, double Jtag, double Vel);
	bool SmoothStop();
	bool SingleJointJog(short Axis, double VEL, double ACC);
	bool AutoZero();
	void GetCPos(double *CPos);
	void GetJointPos(double* JPosCurr);
	void GetJointTagPos(double* jpOStAG);
	bool SetZero();
	bool ClearError();
	bool EStop();
	bool MotionClose();
	void EncoderRead(double* enc);
	bool MotionInit();
	bool SingleJointTraptime(short AXIS, double JointCurr, double JointTag, double ACC, double Runtime);
	bool P2PJointVel(double gdJTag[],double vel);
	short Pvt_CompleteLoop(IN double Point_0[3],IN double Point_1[3],IN double VelRatio,IN int LoopNum);
	short Pvt_PointToPoint(IN double Point_0[3],IN double Point_1[3],IN double VelRatio,IN int LoopNum);
	short Jog_Test(short Axis,char dir);
	short Pvt_DynamicPT(IN double Point_0[3],IN double Point_1[3],IN short linetype,short velrate);//linetype表示插补轨迹样式
	void Ena_Stop(char index);
	short Compensation(IN double POS[3],IN short EndPostype);
	short P2P_COM(IN double POS[3]);
	void WaitMotor();
	bool ConCalibrate();
	bool VisCalibrate();
	void threadFuctionRobotAction();
	GtsMotion();
	~GtsMotion();
};