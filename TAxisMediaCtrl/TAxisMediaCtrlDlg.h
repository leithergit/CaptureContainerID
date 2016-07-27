
// TAxisMediaCtrlDlg.h : 头文件
//

#pragma once
#include "axismediacontrol.h"

// CTAxisMediaCtrlDlg 对话框
class CTAxisMediaCtrlDlg : public CDialog
{
// 构造
public:
	CTAxisMediaCtrlDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TAXISMEDIACTRL_DIALOG };
	CAxismediacontrol *m_pWndAmc;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
