#pragma once

//杆长
//各个轴的限位
#define AXIS1LimitP 128*4*50*2500/360
#define AXIS1LimitN -128*4*50*2500/360
#define AXIS2LimitP 123*4*50*2500/360
#define AXIS2LimitN -123*4*50*2500/360
#define AXIS3LimitP 25*4*1.5*2500/16    //乘以4是因为在调试过程中发现实际位置与规划位置差4倍,在控制卡规划器当量变换中将已将规划器输出与轴输出的比例设为1比4。限位的也要相应的增加4倍
#define AXIS3LimitN -55*4*1.5*2500/16   //乘以4是因为在调试过程中发现实际位置与规划位置差4倍，在控制卡规划器当量变换中将已将规划器输出与轴输出的比例设为1比4。限位的也要相应的增加4倍
#define AXIS4LimitP 180*4*3*2500/360
#define AXIS4LimitN -180*4*3*2500/360

//电机轴号
#define TABLEX 1
#define TABLEY 2
#define TABLEZ 3
#define AXIS_X 1
#define AXIS_Y 2
#define AXIS_Z 3

//电机平滑停止和紧急停止减速度
#define SmoothStopDec 100
#define EstopDec 1000

//////////////////////////////////////杆长
#define SR 160.0      //静平台外接圆半径
#define Mr 50.0       //动平台外接圆半径
#define PL 200.0      //主动臂长度
#define NL 450.0      //从动臂长度

//////////////////////////////////////静平台角度
#define S_Ang_1 0.0
#define S_Ang_2 120.0
#define S_Ang_3 240.0

//////////////////////////////////////圆周率相关
#define PI 3.1415926535897
#define PI2 6.2831853071796
#define PI_RAD 0.0174532925199       //角度转换为弧度
#define PI_DEG 57.2957795130823      //弧度转换为角度参数

//////////////////////////////////////参数描述宏
#define IN     //输入参数
#define OUT    //输出参数

//////////////////////////////////////脉冲当量计算值
#define PULSE 10000
#define ANGLE 360
#define RIGIDRATIO 41

/////////////////////////////////////正解参数
#define SIDELENGTH_FIXED  160*3.4641016   //sidelength of the fix/end platform triangle,
#define SIDELENGTH_END    50*3.4641016 	 //not the centre-privot distance

/////////////////////////////////////XY平台参数
#define RECTANGLESIZE_X 160    //
#define RECTANGLESIZE_Y 220    //

