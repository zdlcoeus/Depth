
// CalcOffsetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CalcOffset.h"
#include "CalcOffsetDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
		refImg = imread(str);

		imshow("ref", refImg);
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
		objImg = imread(str);

		imshow("obj", objImg);
	}
}


void CCalcOffsetDlg::OnBnClickedButton3()
{
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
}
