
// CalcOffsetDlg.h : ͷ�ļ�
//

#pragma once
#include<opencv2\opencv.hpp>

using namespace cv;

// CCalcOffsetDlg �Ի���
class CCalcOffsetDlg : public CDialogEx
{
// ����
public:
	CCalcOffsetDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALCOFFSET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
private:
	Mat refImg;   //�ο�ͼ
	Mat objImg;   //Ŀ��ͼ
	Mat templImg; //ģ��ͼ
	Mat resultImg;//���ͼ

	Mat CalDiffMap(const Mat& objImg, const Mat& refImg); //����ƫ��

protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
