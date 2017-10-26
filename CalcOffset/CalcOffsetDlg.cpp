
// CalcOffsetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CalcOffset.h"
#include "CalcOffsetDlg.h"
#include "afxdialogex.h"
#include <math.h>
#include <io.h>
#include <fcntl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void InitConsoleWindow()
{
	AllocConsole();
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle, _O_TEXT);
	FILE * hf = _fdopen(hCrt, "w");
	*stdout = *hf;
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCalcOffsetDlg �Ի���



CCalcOffsetDlg::CCalcOffsetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CALCOFFSET_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCalcOffsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCalcOffsetDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_READ_REF, &CCalcOffsetDlg::OnBnClickedBtnReadRef)
	ON_BN_CLICKED(IDC_BTN_READ_OBJ, &CCalcOffsetDlg::OnBnClickedBtnReadObj)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalcOffsetDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


Mat CCalcOffsetDlg::CalDiffMap(const Mat & objImg, const Mat & refImg)
// objImg, refImg������CV_8U���ͣ���������
{
	Mat diffMap;          //ƫ��ͼ
	int cellsize = 19;    //���С
	int left = -20;       //�����ƫ����
	int right = 30;       //�����ƫ����
	double err_last = 0; //�ϴ�ƥ������
	int step_last = 0; //�ϴ�ƥ���ƫ��
	int m = objImg.rows;  //ͼƬ����
	int n = objImg.cols;  //ͼƬ����

	diffMap = Mat::zeros(m, n, CV_8S);

	//ѭ��ƥ��
	for (int i = (cellsize + 1) / 2; i <= m - (cellsize - 1) / 2; i++)
	{
		for (int j = (cellsize + 1) / 2; j <= n - (cellsize - 1) / 2; j++)
		{
			Mat cellMatrix = objImg(Range(i - (cellsize + 1) / 2, i + (cellsize - 1) / 2)
				, Range(j - (cellsize + 1) / 2, j + (cellsize - 1) / 2));//ģ�嵥Ԫ
			int beststep = 0;//���ƫ����
			double minerr = 100;//��С���
			int step;
			int x1, y1, x2, y2;
			double err;

			////������С��Χ����
			//if (err_last != 0)//��һ�β���С��Χ����
			//{
			//	for (int minstep = -3; minstep <= 3; minstep++)//�����ƥ��㸡����Χ
			//	{
			//		step = minstep + step_last;
			//		x1 = j + step - (cellsize + 1) / 2;
			//		y1 = i - (cellsize + 1) / 2;
			//		x2 = j + step + (cellsize - 1) / 2;
			//		y2 = i + (cellsize - 1) / 2;
			//		if (x1 >= 0 && y1 >= 0 && x2 <= n && y2 <= m)
			//		{
			//			//ʹ��ŷʽ������������
			//			err = 0;
			//			for (int k = y1; k < y2; k++)
			//			{
			//				for (int l = x1; l < x2; l++)
			//				{
			//					err += pow(cellMatrix.at<uchar>(k - y1, l - x1) - refImg.at<uchar>(k, l), 2);
			//				}
			//			}
			//			err = err / (cellsize*cellsize * 255 * 255);

			//			//�ҵ���Сerr�����ҳ�����Ӧ��step
			//			if (err < minerr)
			//			{
			//				minerr = err;
			//				beststep = step;
			//			}
			//		}
			//	}
			//	if (abs(minerr - err_last) < 0.005)
			//		//if (minerr < 0.05)
			//	{
			//		diffMap.at<char>(i, j) = beststep;
			//		step_last = beststep;
			//		err_last = minerr;
			//		continue;
			//	}
			//}

			//���С��Χ�Ѳ���������ȫ��Χ����
			minerr = 100;
			beststep = 0;

			////��ʼ��ʱ
			//LARGE_INTEGER t1, t2, tc;
			//QueryPerformanceFrequency(&tc);
			//QueryPerformanceCounter(&t1);

			//for (int istep = left; istep <= right; istep++)
			//{
			//	x1 = j + istep - (cellsize + 1) / 2;
			//	y1 = i - (cellsize + 1) / 2;
			//	x2 = j + istep + (cellsize - 1) / 2;
			//	y2 = i + (cellsize - 1) / 2;
			//	if (x1 >= 0 && y1 >= 0 && x2 <= n && y2 <= m)
			//	{
			//		//ʹ��ŷʽ������������
			//		err = 0;
			//		for (int k = y1; k < y2; k++)
			//		{
			//			for (int l = x1; l < x2; l++)
			//			{
			//				err += pow(cellMatrix.at<uchar>(k - y1, l - x1) - refImg.at<uchar>(k, l), 2);
			//			}
			//		}
			//		err = err / (cellsize*cellsize * 255 * 255);

			//		//�ҵ���Сerr�����ҳ�����Ӧ��step
			//		if (err < minerr)
			//		{
			//			minerr = err;
			//			beststep = istep;
			//		}
			//	}
			//}

			//ʹ��opencv�Դ���ƥ���㷨����
			x1 = max(0, j + left - (cellsize + 1) / 2);
			y1 = max(0, i - (cellsize + 1) / 2);
			x2 = min(n, j + right + (cellsize - 1) / 2);
			y2 = min(m, i + (cellsize - 1) / 2);
			Mat subRefImg = refImg(Range(y1, y2), Range(x1, x2));

			int result_cols = subRefImg.cols - cellMatrix.cols + 1;
			int result_rows = subRefImg.rows - cellMatrix.rows + 1;
			Mat resultImg;
			resultImg.create(result_rows, result_cols, CV_32FC1);
			matchTemplate(subRefImg, cellMatrix, resultImg, 1);  //Ĭ��ʹ�÷���1
			normalize(resultImg, resultImg, 0, 1, NORM_MINMAX, -1, Mat());

			double minVal; double maxVal; Point minLoc; Point maxLoc;
			Point matchLoc;
			minMaxLoc(resultImg, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
			matchLoc = minLoc;
			if (x1 == 0)
			{
				beststep = matchLoc.x - j + (cellsize + 1) / 2;
			}
			else
			{
				beststep = matchLoc.x + left;
			}
			minerr = minVal;

			////ֹͣ��ʱ
			//QueryPerformanceCounter(&t2);
			//CString strTime;
			//strTime.Format(_T("time:%fms"), (t2.QuadPart-t1.QuadPart)*1000.0/tc.QuadPart);
			//AfxMessageBox(strTime);

			if (minerr > 0.075)//����ֱ�Ӹ�ֵ�ٽ�����ƫ��ֵ
			{
				diffMap.at<char>(i, j) = step_last;
			}
			else
			{
				diffMap.at<char>(i, j) = beststep;
				step_last = beststep;
				err_last = minerr;
			}
		}
	}
	return diffMap;
}


// CCalcOffsetDlg ��Ϣ�������
BOOL CCalcOffsetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCalcOffsetDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCalcOffsetDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCalcOffsetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCalcOffsetDlg::OnBnClickedBtnReadRef()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString filter = _T("ͼƬ(*.bmp;*.jpg)");
	CFileDialog openFileDlg(TRUE, _T("bmp"), _T("*.bmp;*.jpg"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, filter, NULL);

	if (openFileDlg.DoModal() == IDOK)
	{
		CString fileName = openFileDlg.GetPathName();
		USES_CONVERSION;
		std::string str(W2A(fileName));
		refImg = imread(str, CV_8U);

		//imshow("ref", refImg);
	}
}


void CCalcOffsetDlg::OnBnClickedBtnReadObj()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString filter = _T("ͼƬ(*.bmp;*.jpg)");
	CFileDialog openFileDlg(TRUE, _T("bmp"), _T("*.bmp;*.jpg"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, filter, NULL);

	if (openFileDlg.DoModal() == IDOK)
	{
		CString fileName = openFileDlg.GetPathName();
		USES_CONVERSION;
		std::string str(W2A(fileName));
		objImg = imread(str, CV_8U);

		//imshow("obj", objImg);
	}
}


void CCalcOffsetDlg::OnBnClickedButton3()
{
	/*
	//����ƥ�����
	int cell_size = 19;

	////��ʼ��ʱ
	//long t1 = GetTickCount();

	//ѭ������OffSet
	for (int i = 0; i < refImg.rows-cell_size; i++)
	{
		//��ʼ��ʱ
		long t1 = GetTickCount();

		for (int j = 0; j < refImg.cols-cell_size; j++)
		{
			//��ȡcell matrix
			Mat templImg = refImg(Rect(j, i, cell_size, cell_size));

			//��λƥ��
			Mat dispImg;
			objImg.copyTo(dispImg);
			int result_cols = objImg.cols - templImg.cols + 1;
			int result_rows = objImg.rows - templImg.rows + 1;
			resultImg.create(result_rows, result_cols, CV_32FC1);
			matchTemplate(objImg, templImg, resultImg, 1);  //Ĭ��ʹ�÷���1
			normalize(resultImg, resultImg, 0, 1, NORM_MINMAX, -1, Mat());

			double minVal; double maxVal; Point minLoc; Point maxLoc;
			Point matchLoc;
			minMaxLoc(resultImg, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

			matchLoc = minLoc;

		}


		//ֹͣ��ʱ
		long t2 = GetTickCount();
		CString strTime;
		strTime.Format(_T("time:%dms"), t2 - t1);
		AfxMessageBox(strTime);
	}

	////ֹͣ��ʱ
	//long t2 = GetTickCount();
	//CString strTime;
	//strTime.Format(_T("time:%dms"), t2 - t1);
	//AfxMessageBox(strTime);
	*/



	//��ʼ��ʱ
	long t1 = GetTickCount();

	Mat diffImg = CalDiffMap(objImg, refImg);
	Mat showImg;
	normalize(diffImg, showImg, -128, 127, NORM_MINMAX);
	showImg.convertTo(showImg, CV_8U, 1, 128);
	medianBlur(showImg, showImg, 5);
	imshow("diff", showImg);

	//ֹͣ��ʱ
	long t2 = GetTickCount();
	CString strTime;
	strTime.Format(_T("time:%dms"), t2 - t1);
	AfxMessageBox(strTime);
}
