
// TAxisMediaCtrl.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTAxisMediaCtrlApp:
// �йش����ʵ�֣������ TAxisMediaCtrl.cpp
//

class CTAxisMediaCtrlApp : public CWinAppEx
{
public:
	CTAxisMediaCtrlApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTAxisMediaCtrlApp theApp;