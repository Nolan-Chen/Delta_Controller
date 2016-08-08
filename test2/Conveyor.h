#pragma once
#include"GtsMotion.h"

// CConveyor 对话框

class CConveyor : public CDialogEx
{
	DECLARE_DYNAMIC(CConveyor)
private:
	CButton m_GetFirstPos;
	CButton m_GetSecondPos;
	CButton m_GetThirdPos;
	short flag;
	GtsMotion m_gtsmotion;
	double m_robpos1[3];
	double m_conpos1;
	double m_robpos2[3];
	double m_conpos2;
	double m_robpos3[3];
	double m_conpos3;
	double m_ScaleFactor;//传送带编码器与机器人之间的比例因子
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CConveyor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConveyor();
	// 对话框数据
	enum { IDD = IDD_Conveyor1 };
	afx_msg void OnBnClickedBtthirdpos();
	afx_msg void OnBnClickedBtfirstpos();
	afx_msg void OnBnClickedBtsecondpos();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCalact();
};
