
// OpenCVTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// COpenCVTestApp: 
// �йش����ʵ�֣������ OpenCVTest.cpp
//

class COpenCVTestApp : public CWinApp
{
public:
	COpenCVTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern COpenCVTestApp theApp;