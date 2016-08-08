
// test2.h : main header file for the PROJECT_NAME application
//

#pragma once	//一个文件中多次引用该头文件时该宏指示编译器只包含一次

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif	//在包含此文件之前先包含“stdafx.h”以生PCH文件

#include "resource.h"		// main symbols，定义资源ID


// CDeltaControllerApp:
// See test2.cpp for the implementation of this class
//

class CDeltaControllerApp : public CWinApp	//声明用户类
{
public:
	CDeltaControllerApp();	//声明无参构造函数

// Overrides
public:
	virtual BOOL InitInstance();//重载CWinApp类的InitInstance()函数

// Implementation

	DECLARE_MESSAGE_MAP()	//消息映射声明宏
};

extern CDeltaControllerApp theApp;	//声明了一个全局的用户程序对象