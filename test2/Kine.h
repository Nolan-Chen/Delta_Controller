#pragma once

#include "Robot.h"
///////////////////////////////////////////////////////////
/////////Kine window

//位姿矩阵
struct MtxKine
{
public:
	double R11;
	double R12;
	double R13;
	double R21;
	double R22;
	double R23;
	double R31;
	double R32;
	double R33;

	double X;
	double Y;
	double Z;
};

//TCP矩阵
struct MtxTcp
{
	double K1;
	double K2;
	double K3;
	double M1;
	double M2;
	double M3;
	double N1;
	double N2;
	double N3;

	double u1;
	double u2;
	double u3;
};

/************************************************************************
 * 运动学基类                                                             
 ************************************************************************/
class Kine:public CEdit
{
	//Construction
public:
	Kine();

	//Attributes
protected:
	//inverse kinematics helper functions,calculates angle theta1 for YZ-pane
	bool calcAngleYZ(float x0, float y0, float z0, double *theta);

public:
	//Operations
public:

	//Implementation
public:
	void Robot_IncreTransTool(IN double* currpos, IN double* increpos, OUT double* outpos);
	void Mtx_Multiply(MtxKine* input, MtxKine* middle, MtxKine* output, int inv);
	void Trans_PosToMtx(double* pos, MtxKine* output, int inv);
	void Trans_MtxToPos(MtxKine* input, double* outpos);

	/************************************************************************
	 * 函数：IKine()                                                          
	 * 功能：逆解
	 *
	 * 输入：double* gdCPos  - 位姿数组，（x,y,x)
	 * 输出：double* gdJPos  - 关节角数组
	 * 返回：int - 0成功，其他错误
	 ************************************************************************/
	int IKine(IN double gdCPos[3],OUT double gdJPos[3]);

	/************************************************************************
	 * 函数：FKine()                                                          
	 * 功能：正解
	 *
	 * 输入：double* gdJPos  - 关节转角  （三个转角）
	 * 输出：double* gdJPos  - 正解位姿，（x,y,z)
	 * 返回：int - 0成功，其他错误
	 ************************************************************************/
	int FKine(IN double gdJPos[3],OUT double gdCPos[6]);

	virtual ~Kine();
};