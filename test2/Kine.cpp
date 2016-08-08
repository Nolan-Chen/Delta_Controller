#pragma once

#include "stdafx.h"
#include "DELTA_Controller.h"
#include "Kine.h"
#include <math.h>
#include "gts.h"


Kine::Kine()
{
}

Kine::~Kine()
{
}
bool Kine::calcAngleYZ(float x0, float y0, float z0, double *theta)
{// inverse kinematics helper functions, calculates angle theta1 (for YZ-pane)
	float y1 = -0.5 * 0.57735 * SIDELENGTH_FIXED; // f/2 * tg 30
	y0 -= 0.5 * 0.57735    * SIDELENGTH_END;    // shift center to edge
	// z = a + b*y
	float a = (x0*x0 + y0*y0 + z0*z0 +PL*PL - NL*NL - y1*y1)/(2*z0);
	float b = (y1-y0)/z0;
	// discriminant
	float d = -(a+b*y1)*(a+b*y1)+PL*(b*b*PL+PL); 
	if (d < 0) return false; // non-existing point
	float yj = (y1 - a*b - sqrt(d))/(b*b + 1); // choosing outer point
	float zj = a + b*yj;
	*theta = 180.0*atan(-zj/(y1 - yj))/PI + ((yj>y1)?180.0:0.0);
	return true;
}

int Kine::IKine(IN double* gdCPos,OUT double* gdJPos)
{
	//		位姿	-	Tcp矩阵	-	关节弧度	-	关节角度
	//	 id_cPos	-	lm_Tcp	-	ld_jRad		-	id_jPos
	double temp[3]={0};
	int i=0;
	MtxTcp lm_Tcp;

	/////////////////////////////////////////////位姿
	double x=gdCPos[0];
	double y=gdCPos[1];
	double z=gdCPos[2];

	/////////////////////////////////////////////Tcp矩阵
	lm_Tcp.K1 = ((((x*x+y*y+z*z)-2*x*(SR-Mr)+(SR-Mr)*(SR-Mr)+(PL*PL-NL*NL))/
		(2*PL))-(SR-Mr-x));
	lm_Tcp.M1 = (-2*z);
	lm_Tcp.N1 = ((((x*x+y*y+z*z)-2*x*(SR-Mr)+(SR-Mr)*(SR-Mr)+(PL*PL-NL*NL))/
		(2*PL))+(SR-Mr-x));

	lm_Tcp.K2 = (((x*x+y*y+z*z)+(x-sqrt(3.0)*y)*(SR-Mr)+(SR-Mr)*(SR-Mr)+
		(PL*PL-NL*NL))/PL)-(x-sqrt(3.0)*y)-2*(SR-Mr);
	lm_Tcp.M2 = -4*z;
	lm_Tcp.N2 = (((x*x+y*y+z*z)+(x-sqrt(3.0)*y)*(SR-Mr)+(SR-Mr)*(SR-Mr)+
		(PL*PL-NL*NL))/PL)+(x-sqrt(3.0)*y)+2*(SR-Mr);

	lm_Tcp.K3 = (((x*x+y*y+z*z)+(x+sqrt(3.0)*y)*(SR-Mr)+(SR-Mr)*(SR-Mr)
		+(PL*PL-NL*NL))/PL)-(x+sqrt(3.0)*y)-2*(SR-Mr);
	lm_Tcp.M3 = -4*z;
	lm_Tcp.N3 = (((x*x+y*y+z*z)+(x+sqrt(3.0)*y)*(SR-Mr)+(SR-Mr)*(SR-Mr)
		+(PL*PL-NL*NL))/PL)+(x+sqrt(3.0)*y)+2*(SR-Mr);

	///////////////////////////////////////////////////////temp
	temp[0] = lm_Tcp.M1*lm_Tcp.M1-4*lm_Tcp.K1*lm_Tcp.N1;
	temp[1] = lm_Tcp.M2*lm_Tcp.M2-4*lm_Tcp.K2*lm_Tcp.N2;
	temp[2] = lm_Tcp.M3*lm_Tcp.M3-4*lm_Tcp.K3*lm_Tcp.N3;

	if (temp[0]<0||temp[1]<0||temp[2]<0)
	{
		return 0;
	} 
	else
	{
		//////////////////////////////////////////////中间变量
		lm_Tcp.u1 = (-lm_Tcp.M1-sqrt(temp[0]))/(2*lm_Tcp.K1);
		lm_Tcp.u2 = (-lm_Tcp.M2-sqrt(temp[1]))/(2*lm_Tcp.K2);
		lm_Tcp.u3 = (-lm_Tcp.M3-sqrt(temp[2]))/(2*lm_Tcp.K3);

		/////////////////////////////////////////////弧度
		gdJPos[0] = 2*atan(lm_Tcp.u1)*PI_DEG;
		gdJPos[1] = 2*atan(lm_Tcp.u2)*PI_DEG;
		gdJPos[2] = 2*atan(lm_Tcp.u3)*PI_DEG;

		double test[3] = {gdJPos[0],gdJPos[1],gdJPos[2]};

		return 1;
	}
}
int Kine::FKine(IN double* gdJPos,OUT double* gdCPos)
{
	// forward kinematics: (theta1, theta2, theta3) -> (x0, y0, z0)
	// returned status: 0=OK, -1=non-existing position
	float t = (SIDELENGTH_FIXED-SIDELENGTH_END)*1/sqrt(3.0)/2;
	float dtr = PI/180.0;

	gdJPos[0] *= dtr;//degree to radias
	gdJPos[1] *= dtr;
	gdJPos[2] *= dtr;

	float y1 = -(t + PL*cos(gdJPos[0]));
	float z1 = -PL*sin(gdJPos[0]);

	float y2 = (t + PL*cos(gdJPos[1]))*0.5;
	float x2 = y2*sqrt(3.0);
	float z2 = -PL*sin(gdJPos[1]);

	float y3 = (t + PL*cos(gdJPos[2]))*0.5;
	float x3 = -y3*sqrt(3.0);
	float z3 = -PL*sin(gdJPos[2]);

	float dnm = (y2-y1)*x3-(y3-y1)*x2;

	float w1 = y1*y1 + z1*z1;
	float w2 = x2*x2 + y2*y2 + z2*z2;
	float w3 = x3*x3 + y3*y3 + z3*z3;

	// x = (a1*z + b1)/dnm
	float a1 = (z2-z1)*(y3-y1)-(z3-z1)*(y2-y1);
	float b1 = -((w2-w1)*(y3-y1)-(w3-w1)*(y2-y1))/2.0;

	// y = (a2*z + b2)/dnm;
	float a2 = -(z2-z1)*x3+(z3-z1)*x2;
	float b2 = ((w2-w1)*x3 - (w3-w1)*x2)/2.0;

	// a*z^2 + b*z + c = 0
	float a = a1*a1 + a2*a2 + dnm*dnm;
	float b = 2*(a1*b1 + a2*(b2-y1*dnm) - z1*dnm*dnm);
	float c = (b2-y1*dnm)*(b2-y1*dnm) + b1*b1 + dnm*dnm*(z1*z1 - NL*NL);

	// discriminant
	float d = b*b - (float)4.0*a*c;
	if (d < 0) 
	{
		short sRtn;
		sRtn=GT_Stop(15,0);//所有轴紧急停止

		return 0; // non-existing point
	}
	else
	{
		gdCPos[2] = -(float)0.5*(b+sqrt(d))/a;
		gdCPos[1] = (a1*gdCPos[2] + b1)/dnm;
		gdCPos[0] = (a2*gdCPos[2] + b2)/dnm;

		gdCPos[2] = -gdCPos[2];     //在调试过程中发现正解的第一和第三个坐标值是逆转的
		gdCPos[0] = -gdCPos[0];

		return 1;
	}
}
