#include "StdAfx.h"
#include "Clibration.h"
#include <fstream>
#include <cmath>


Clibration::Clibration(void)
{
	filename1 = "Conveyor Calibration Data";
	filename2 = "Vision Calibration Data";
}

Clibration::~Clibration(void)
{
}

bool Clibration::Con_Cal(double p1[3],double p2[3],double p3[3],double* c1,double* c2,double* c3)
{
	double lR,lC,xc0R,ycoR,zcoR,module2c,module3c;
	Vector3d xC;Vector3d yC;Vector3d zC;

	//计算比例因子
	lR = sqrt(pow(p2[0]-p1[0],2)+pow(p2[1]-p1[1],2)+pow(p2[2]-p1[2],2));  //一二点间机器人末端距离
	lC = *c2 - *c1;     //一二点间传送带读数差
	m_Factor = lR/lC;

	//计算传送带坐标系原点
	xc0R = (p3[0]*pow(p1[0],2)-2*p1[0]*p2[0]*p3[0]-p1[0]*p1[1]*p2[1]+p1[0]*p1[1]*p3[1]+p1[0]*pow(p2[1],2)-
		p1[0]*p2[1]*p3[1]-p1[0]*p1[2]*p1[2]+p1[0]*p1[2]*p3[2]+p1[0]*pow(p2[2],2)-p1[0]*p2[2]*p3[2]+
		p3[0]*pow(p2[0],2)+p2[0]*pow(p1[1],2)-p2[0]*p1[1]*p2[1]-p2[0]*p1[1]*p3[1]+p2[0]*p2[1]*p3[1]+
		p2[0]*pow(p1[2],2)-p2[0]*p1[2]*p2[2]-p2[0]*p1[2]*p3[2]+p2[0]*p2[2]*p3[2])/
		(pow(p1[0]-p2[0],2)*pow(p1[1]-p2[1],2)*pow(p1[2]-p2[2],2));
	ycoR = (p2[1]*pow(p1[0],2)-p1[0]*p2[0]*p1[1]-p1[0]*p2[0]*p2[1]+p1[0]*p3[0]*p1[1]-p1[0]*p3[0]*p2[1]+
		p1[1]*pow(p2[0],2)-p2[0]*p3[0]*p1[1]+p2[0]*p3[0]*p2[1]+p3[1]*pow(p1[1],2)-2*p1[1]*p2[1]*p3[1]-
		p1[1]*p1[2]*p2[2]+p1[1]*p1[2]*p3[2]-p1[1]*pow(p2[2],2)-p1[1]*p2[2]*p3[2]+p3[2]*pow(p2[1],2)+
		p2[1]*pow(p1[2],2)-p2[1]*p1[2]*p2[2]-p2[1]*p1[2]*p3[2]+p2[1]*p2[2]*p3[2])/
		(pow(p1[0]-p2[0],2)*pow(p1[1]-p2[1],2)*pow(p1[2]-p2[2],2));
	zcoR = (p2[2]*pow(p1[0],2)-p1[0]*p2[0]*p1[2]-p1[0]*p2[0]*p2[2]+p1[0]*p3[0]*p1[2]-p1[0]*p3[0]*p2[2]+
		p1[0]*p3[0]*p1[2]-p1[0]*p3[0]*p2[2]+p1[2]*pow(p2[0],2)-p2[0]*p3[0]*p1[2]+p2[0]*p3[0]*p2[2]+
		p2[2]*pow(p1[1],2)-p1[1]*p2[1]*p1[2]-p1[1]*p2[1]*p2[2]+p1[1]*p3[1]*p1[2]-p1[1]*p3[1]*p2[2]+
		p1[2]*pow(p2[1],2)-p2[1]*p3[1]*p2[2]+pow(p1[2],2)*p3[2]-2*p1[2]*p2[2]*p3[2]+pow(p2[2],2)*p3[2])/
		(pow(p1[0]-p2[0],2)*pow(p1[1]-p2[1],2)*pow(p1[2]-p2[2],2));

	//计算传送带坐标系矢量
	module2c = sqrt(pow(p2[0]-xc0R,2)+pow(p2[1]-ycoR,2)+pow(p2[2]-zcoR,2));
	module3c = sqrt(pow(p3[0]-xc0R,2)+pow(p3[1]-ycoR,2)+pow(p3[2]-zcoR,2));
	xC(0) = (p2[0]-xc0R)/module2c; xC(1) = (p2[1]-ycoR)/module2c; xC(2) = (p2[2]-zcoR)/module2c;
	yC(0) = (p3[0]-xc0R)/module3c; yC(1) = (p3[1]-ycoR)/module3c; yC(2) = (p3[2]-zcoR)/module3c;
	zC = xC.cross(yC);

	//转换矩阵赋值
	m_HCtoR <<  xC(0),yC(0),zC(0),xc0R,
		        xC(1),yC(1),zC(1),ycoR,
				xC(2),yC(2),zC(2),ycoR,
				0,    0,    0,    1;

	flag = StoreCalData(CONVEYOR);
	return true;
}

bool Clibration::Vis_Cal()
{
	return true;
}

bool Clibration::StoreCalData(IOFMODE mode)
{
	ofstream fout;
	switch (mode)
	{
	case CONVEYOR:fout.open(filename1);
		if (!fout.is_open())
		{
			printf("Can not open file...\n");
			return false;
		}
		fout<<"比例因子："<<m_Factor<<endl;
		fout<<"转换矩阵："<<m_HCtoR<<endl;
		fout.close();
		break;
	case VISION:fout.open(filename2);
		break;
	}
	return true;
}

bool Clibration::ShowCalData(IOFMODE mode)
{
	ifstream fin;
	switch (mode)
	{
	case CONVEYOR:fin.open(filename1);
		if (!fin.is_open())
		{
			printf("Can not open file...\n");
			return false;
		}
		break;
	case VISION:fin.open(filename2);
		break;
	}
	return true;
}