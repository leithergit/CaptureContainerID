
// AxisStreamRate.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAxisStreamRateApp:
// �йش����ʵ�֣������ AxisStreamRate.cpp
//

class CAxisStreamRateApp : public CWinAppEx
{
public:
	CAxisStreamRateApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAxisStreamRateApp theApp;