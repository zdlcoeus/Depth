
// CalcOffsetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CalcOffset.h"
#include "CalcOffsetDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
		refImg = imread(str);

		imshow("ref", refImg);
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
		objImg = imread(str);

		imshow("obj", objImg);
	}
}


void CCalcOffsetDlg::OnBnClickedButton3()
{
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
}
