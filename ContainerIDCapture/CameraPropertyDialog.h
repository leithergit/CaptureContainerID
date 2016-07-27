#pragma once
#include "..\AxisSDK\CameraProperty.h"

// CCameraPropertyDialog dialog

class CCameraPropertyDialog : public CDialog
{
	DECLARE_DYNAMIC(CCameraPropertyDialog)

public:
	CCameraPropertyDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCameraPropertyDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_CAMERAPROPERTY };
	
	CCameraProperty *GetCameraProperty()
	{
		return m_pCamera;
	}
	void SetCameraProperty(CCameraProperty *pCamera)
	{
		m_pCamera = pCamera;
	}

	void SetPLCNode(int nPLCNode)
	{
		m_nPLCNode = nPLCNode;
	}

	void SetViewMode(bool bViewMode = true)
	{
		m_nViewMode = true;
	}
protected:
	CCameraProperty *m_pCamera;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	INT		m_nPLCNode;
	bool	m_nViewMode;		// 查看模式，不允许修改
};
