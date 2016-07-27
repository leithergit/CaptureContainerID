
// TAxisMediaCtrlDlg.h : ͷ�ļ�
//

#pragma once
#include "axismediacontrol.h"

// CTAxisMediaCtrlDlg �Ի���
class CTAxisMediaCtrlDlg : public CDialog
{
// ����
public:
	CTAxisMediaCtrlDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TAXISMEDIACTRL_DIALOG };
	CAxismediacontrol *m_pWndAmc;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonGetstatus();
};
