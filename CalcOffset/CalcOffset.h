
// CalcOffset.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCalcOffsetApp: 
// �йش����ʵ�֣������ CalcOffset.cpp
//

class CCalcOffsetApp : public CWinApp
{
public:
	CCalcOffsetApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCalcOffsetApp theApp;