#pragma once

#include "Kine.h"

class Trajectory
{
public:
	Trajectory(void);
	~Trajectory(void);
private:
	double est_sp;            //速度
	double est_acc;           //加速度
	double est_jerk;          //加加速度
	Kine kine;   //实例化运动学

	double MaxJoint(double Angles[]);
	double Time_Line(IN double POS_0[3],IN double POS_1[3],IN short vel);
	double Dis_Line(IN double POS_0[3],IN double POS_1[3]);
public:
	void Set_est(IN double est_values[3]);      //初始化速度、加速度、加加速度
	int StraightLine(IN double POS_0[3],IN double POS_1[3],IN short vel);
	int EllipseLine(IN double POS_0[3],IN double POS_1[3],IN short vel);
	int LameLine(IN double POS_0[3],IN DOUBLE POS_1[3],IN short vel);

	//double MaxJoint(double Angles[3]);
};

