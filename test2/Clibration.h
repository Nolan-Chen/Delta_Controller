#pragma once
#include"Eigen/Eigen"
#include <iostream>
#include <string>
using namespace Eigen;
using namespace std;
class Clibration
{
private:
	bool flag;
	enum IOFMODE {CONVEYOR,VISION};
	string filename1 ;
	string filename2 ;

	Matrix4d m_HCtoR;        //传送带到机器人的转换矩阵s
	double m_Factor;      //比例因子
public:
	Clibration(void);
	~Clibration(void);
	bool Con_Cal(double p1[3],double p2[3],double p3[3],double* c1,double* c2,double* c3);
	bool Vis_Cal();
	bool StoreCalData(IOFMODE mode);
	bool ShowCalData(IOFMODE mode);
};

