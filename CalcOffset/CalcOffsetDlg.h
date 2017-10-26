
// CalcOffsetDlg.h : 头文件
//

#pragma once
#include<opencv2\opencv.hpp>

using namespace cv;

// CCalcOffsetDlg 对话框
class CCalcOffsetDlg : public CDialogEx
{
// 构造
public:
	CCalcOffsetDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALCOFFSET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
private:
	Mat refImg;   //参考图
	Mat objImg;   //目标图
	Mat templImg; //模板图
	Mat resultImg;//结果图

	Mat CalDiffMap(const Mat& objImg, const Mat& refImg); //计算偏差

protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnReadRef();
	afx_msg void OnBnClickedBtnReadObj();
	afx_msg void OnBnClickedButton3();
};
