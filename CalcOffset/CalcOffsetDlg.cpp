
// CalcOffsetDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CCalcOffsetDlg 对话框



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
// objImg, refImg必须是CV_8U类型！！！！！
{
	Mat diffMap;          //偏移图
	int cellsize = 19;    //块大小
	int left = -20;       //左最大偏移量
	int right = 30;       //右最大偏移量
	double err_last = 0; //上次匹配的误差
	int step_last = 0; //上次匹配的偏移
	int m = objImg.rows;  //图片行数
	int n = objImg.cols;  //图片列数

	diffMap = Mat::zeros(m, n, CV_8S);

	//循环匹配
	for (int i = (cellsize + 1) / 2; i <= m - (cellsize - 1) / 2; i++)
	{
		for (int j = (cellsize + 1) / 2; j <= n - (cellsize - 1) / 2; j++)
		{
			Mat cellMatrix = objImg(Range(i - (cellsize + 1) / 2, i + (cellsize - 1) / 2)
				, Range(j - (cellsize + 1) / 2, j + (cellsize - 1) / 2));//模板单元
			int beststep = 0;//最佳偏移量
			double minerr = 100;//最小误差
			int step;
			int x1, y1, x2, y2;
			double err;

			////首先在小范围搜索
			//if (err_last != 0)//第一次不能小范围搜索
			//{
			//	for (int minstep = -3; minstep <= 3; minstep++)//在最佳匹配点浮动范围
			//	{
			//		step = minstep + step_last;
			//		x1 = j + step - (cellsize + 1) / 2;
			//		y1 = i - (cellsize + 1) / 2;
			//		x2 = j + step + (cellsize - 1) / 2;
			//		y2 = i + (cellsize - 1) / 2;
			//		if (x1 >= 0 && y1 >= 0 && x2 <= n && y2 <= m)
			//		{
			//			//使用欧式距离计算相关性
			//			err = 0;
			//			for (int k = y1; k < y2; k++)
			//			{
			//				for (int l = x1; l < x2; l++)
			//				{
			//					err += pow(cellMatrix.at<uchar>(k - y1, l - x1) - refImg.at<uchar>(k, l), 2);
			//				}
			//			}
			//			err = err / (cellsize*cellsize * 255 * 255);

			//			//找到最小err，并找出所对应的step
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

			//如果小范围搜不到，则在全范围搜索
			minerr = 100;
			beststep = 0;

			////开始计时
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
			//		//使用欧式距离计算相关性
			//		err = 0;
			//		for (int k = y1; k < y2; k++)
			//		{
			//			for (int l = x1; l < x2; l++)
			//			{
			//				err += pow(cellMatrix.at<uchar>(k - y1, l - x1) - refImg.at<uchar>(k, l), 2);
			//			}
			//		}
			//		err = err / (cellsize*cellsize * 255 * 255);

			//		//找到最小err，并找出所对应的step
			//		if (err < minerr)
			//		{
			//			minerr = err;
			//			beststep = istep;
			//		}
			//	}
			//}

			//使用opencv自带的匹配算法测试
			x1 = max(0, j + left - (cellsize + 1) / 2);
			y1 = max(0, i - (cellsize + 1) / 2);
			x2 = min(n, j + right + (cellsize - 1) / 2);
			y2 = min(m, i + (cellsize - 1) / 2);
			Mat subRefImg = refImg(Range(y1, y2), Range(x1, x2));

			int result_cols = subRefImg.cols - cellMatrix.cols + 1;
			int result_rows = subRefImg.rows - cellMatrix.rows + 1;
			Mat resultImg;
			resultImg.create(result_rows, result_cols, CV_32FC1);
			matchTemplate(subRefImg, cellMatrix, resultImg, 1);  //默认使用方法1
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

			////停止计时
			//QueryPerformanceCounter(&t2);
			//CString strTime;
			//strTime.Format(_T("time:%fms"), (t2.QuadPart-t1.QuadPart)*1000.0/tc.QuadPart);
			//AfxMessageBox(strTime);

			if (minerr > 0.075)//误差大，直接赋值临近像素偏移值
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


// CCalcOffsetDlg 消息处理程序
BOOL CCalcOffsetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCalcOffsetDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCalcOffsetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCalcOffsetDlg::OnBnClickedBtnReadRef()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filter = _T("图片(*.bmp;*.jpg)");
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
	// TODO: 在此添加控件通知处理程序代码
	CString filter = _T("图片(*.bmp;*.jpg)");
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
	//设置匹配参数
	int cell_size = 19;

	////开始计时
	//long t1 = GetTickCount();

	//循环计算OffSet
	for (int i = 0; i < refImg.rows-cell_size; i++)
	{
		//开始计时
		long t1 = GetTickCount();

		for (int j = 0; j < refImg.cols-cell_size; j++)
		{
			//获取cell matrix
			Mat templImg = refImg(Rect(j, i, cell_size, cell_size));

			//定位匹配
			Mat dispImg;
			objImg.copyTo(dispImg);
			int result_cols = objImg.cols - templImg.cols + 1;
			int result_rows = objImg.rows - templImg.rows + 1;
			resultImg.create(result_rows, result_cols, CV_32FC1);
			matchTemplate(objImg, templImg, resultImg, 1);  //默认使用方法1
			normalize(resultImg, resultImg, 0, 1, NORM_MINMAX, -1, Mat());

			double minVal; double maxVal; Point minLoc; Point maxLoc;
			Point matchLoc;
			minMaxLoc(resultImg, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

			matchLoc = minLoc;

		}


		//停止计时
		long t2 = GetTickCount();
		CString strTime;
		strTime.Format(_T("time:%dms"), t2 - t1);
		AfxMessageBox(strTime);
	}

	////停止计时
	//long t2 = GetTickCount();
	//CString strTime;
	//strTime.Format(_T("time:%dms"), t2 - t1);
	//AfxMessageBox(strTime);
	*/



	//开始计时
	long t1 = GetTickCount();

	Mat diffImg = CalDiffMap(objImg, refImg);
	Mat showImg;
	normalize(diffImg, showImg, -128, 127, NORM_MINMAX);
	showImg.convertTo(showImg, CV_8U, 1, 128);
	medianBlur(showImg, showImg, 5);
	imshow("diff", showImg);

	//停止计时
	long t2 = GetTickCount();
	CString strTime;
	strTime.Format(_T("time:%dms"), t2 - t1);
	AfxMessageBox(strTime);
}
