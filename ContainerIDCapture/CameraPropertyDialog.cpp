// CameraPropertyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ContainerIDCapture.h"
#include "CameraPropertyDialog.h"


// CCameraPropertyDialog dialog

IMPLEMENT_DYNAMIC(CCameraPropertyDialog, CDialog)

CCameraPropertyDialog::CCameraPropertyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraPropertyDialog::IDD, pParent)
{
	m_pCamera = NULL;
	m_nPLCNode = 0;
	m_nViewMode = false;
}

CCameraPropertyDialog::~CCameraPropertyDialog()
{
}

void CCameraPropertyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCameraPropertyDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CCameraPropertyDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CCameraPropertyDialog message handlers

void CCameraPropertyDialog::OnBnClickedOk()
{
	if (!m_nViewMode)
	{
		TCHAR szIP[32],szAccount[32],szPassword[32];
		WORD nPort = 0;
		GetDlgItemText(IDC_IPADDRESS,szIP,32);
		GetDlgItemText(IDC_EDIT_ACCOUNT,szAccount,32);
		GetDlgItemText(IDC_EDIT_PASSWORD,szPassword,32);
		nPort = GetDlgItemInt(IDC_EDIT_PORT);
		if (m_pCamera)
		{
			m_pCamera->SetCameraProperty(NULL,szIP,szAccount,szPassword,nPort);

		}
		else
		{
			m_pCamera = new CCameraProperty(NULL,szIP,szAccount,szPassword,nPort);
		}
		m_pCamera->nPLCNode = m_nPLCNode;
	}

	OnOK();
}

BOOL CCameraPropertyDialog::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam  == VK_ESCAPE) 
			return TRUE;
		if ( pMsg->wParam == VK_RETURN)
		{	
			NextDlgCtrl();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CCameraPropertyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_pCamera)
	{
		SetDlgItemText(IDC_IPADDRESS,m_pCamera->szCameraIP);
		SetDlgItemText(IDC_EDIT_ACCOUNT,m_pCamera->szAccount);
		SetDlgItemText(IDC_EDIT_PASSWORD,m_pCamera->szPassword);
		SetDlgItemInt(IDC_EDIT_PORT,m_pCamera->nPort);
	}
	SetDlgItemInt(IDC_EDIT_PLCNODE,m_nPLCNode);
	if (m_nViewMode)
	{
		SendDlgItemMessage(IDC_IPADDRESS,EM_SETREADONLY);
		SendDlgItemMessage(IDC_EDIT_ACCOUNT,EM_SETREADONLY);
		SendDlgItemMessage(IDC_EDIT_PASSWORD,EM_SETREADONLY);
		SendDlgItemMessage(IDC_EDIT_PORT,EM_SETREADONLY);
		SendDlgItemMessage(IDC_EDIT_PLCNODE,EM_SETREADONLY);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
