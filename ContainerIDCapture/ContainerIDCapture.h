
// ContainerIDCapture.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CContainerIDCaptureApp:
// �йش����ʵ�֣������ ContainerIDCapture.cpp
//

class CContainerIDCaptureApp : public CWinAppEx
{
public:
	CContainerIDCaptureApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CContainerIDCaptureApp theApp;