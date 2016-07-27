
// ContainerIDCaptureDlg.h : 头文件
//

#pragma once
#include "..\AxisSDK\AxisMediaParser.h"
#include "..\AxisSDK\AxisMediaViewer.h"
#include "..\AxisSDK\AxisMediaParserEvents.h"
#include "..\AxisSDK\AxisMediaViewerEvents.h"
#include "..\AxisSDK\CameraProperty.h"
#include "..\IPCPlaySDK\Media.h"
#include "..\IPCPlaySDK\IPCPlaySDK.h"
#include "Utility.h"
#include "GlliteryStatic.h"
#include <afxctl.h>
#include <list>
using namespace std;

/*
标识         命令   块号   偏移   序号  数据长度
0  1  2  3   4  5   6  7   8  9   a  b  c  d  e  f
FA 01 00 00  00 01  01 F5  00 00  28 25 00 00 00 BE

*/
#define _PLC_IDENTIFY		0x01FA
#define _HoistHeightOffset	70			// 高度数据的在PLC数据块中的索引值
struct PLCHeader
{
	PLCHeader()
	{
		ZeroMemory(this,sizeof(PLCHeader));
		nIdentify	= 0x01FA;
		nCommand	= 0x0100;
	}
	int		nIdentify;
	short	nCommand;
	short	nBlock;
	short	nOffset;
	short   nSerialNo;
	int		nLength;
};

typedef vector<CCameraPropertyPtr> VectorCamera;
typedef list<CCameraPropertyPtr> ListCamera;

struct HoistHeight
{
	int nBlockID;
	int nCurrentHeight;		// 当前高度
};
typedef shared_ptr<HoistHeight> HoistHeightPtr;
// CContainerIDCaptureDlg 对话框
class CContainerIDCaptureDlg : public CDialog
{
// 构造
public:
	CContainerIDCaptureDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CContainerIDCaptureDlg();

// 对话框数据
	enum { IDD = IDD_CONTAINERIDCAPTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	CListEdit *m_pWndItemEdit;
	//CListEdit *m_pWndItemEditPLC;
	//CListCtrl m_wndCameraList;
	//CListCtrl m_wndPLCList;
	CListCtrl	m_wndListHoistHeight;
	CTreeCtrl	m_wndTreeCamera;
	CVideoFrame *m_pVideoWndFrame/* = NULL*/;
	CImageList   *m_pImageList;
	CGlliteryStatic m_wndStatusBar;
	int			m_nCurSelectedCamera;
	int			m_nCurSelectedPLC;
	bool		m_bPopError;
	bool		m_bPopPLCError;
	bool LoadSetting(int &nCount);
	bool SaveSetting();
	list<CCameraPropertyPtr> m_listCameraArray;
	//map<UINT ,ListCameraPtr>m_mapCamera;
	vector<HoistHeightPtr>m_vHoistHeight;
	int			m_DemandHeightArray[16];
	int			m_nDemannHeightCount;
	
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
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonDisconnect();
	
	//afx_msg void OnConnectCamera();
	afx_msg void OnConnectAllcamera();
	//afx_msg void OnDisconnectCamera();
	afx_msg void OnDisconnectAll();
	afx_msg void OnDeleteCamera();
	afx_msg void OnDeleteAllcamera();
	afx_msg LRESULT OnKillFocusCtrls(WPARAM,LPARAM);
	afx_msg void OnDestroy();
	//afx_msg void OnBnClickedButtonSnapshot();
	
// 	afx_msg void OnDeletePlc();
// 	afx_msg void OnDeleteAllplc();
	
	void CheckDirectory(TCHAR *szTodayDir);
	static UINT __stdcall  ThreadCapture(void *p);
	
	HANDLE m_hThreadCapture;
	volatile bool  m_bStartCapture;
	static UINT __stdcall ThreadRecvPLC(void *p);
	HANDLE m_hThreadRecvPLC;
	volatile bool m_bThreadRecvPLCRun;
	SHORT    m_nPLCListenPort;
	int		m_nQueryPeriod;
	bool	m_bConnected;		// 是否已经连接所有相机
	afx_msg void OnClose();
	afx_msg void OnNMRClickTreecamera(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTreecamera(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTreecamera(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddCamera();
	afx_msg void OnModifyCamera();
	afx_msg void OnAddPlcnode();
	afx_msg void OnDeletePlcnode();
	afx_msg void OnModifyPlcnode();
	afx_msg void OnDeleteAllplcnode();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTvnEndlabeleditTreecamera(NMHDR *pNMHDR, LRESULT *pResult);
	void RemoveCamera(CCameraProperty *pCamera)
	{
		for (list<CCameraPropertyPtr>::iterator it = m_listCameraArray.begin();it != m_listCameraArray.end();it ++)
			if( (*it).get() == pCamera)
			{
				m_listCameraArray.erase(it);
				break;
			}
	}

	enum AccessCode
	{
		AC_Disconnect = 0,
		AC_Delete
	};

	void AccessPLCNode(HTREEITEM hPlcItem,AccessCode nCode = AC_Delete)
	{
		if (!hPlcItem)
			return;
		
		HTREEITEM pChildItem = m_wndTreeCamera.GetChildItem(hPlcItem);
		CString strIP;
		CCameraProperty *pCamera ;
		HTREEITEM hOldItem = NULL;

		// 访问节点其下所有相机
		while (pChildItem != NULL)
		{		
			strIP =	m_wndTreeCamera.GetItemText(pChildItem);
			pCamera = (CCameraProperty *)m_wndTreeCamera.GetItemData(pChildItem);
			hOldItem = pChildItem;
			pChildItem = m_wndTreeCamera.GetNextSiblingItem(pChildItem);

			switch(nCode)
			{
			case AC_Disconnect:
				{
					pCamera->Stop();
				}
				break;
			case AC_Delete:
				{
					pCamera->Stop();
					RemoveCamera(pCamera);
					m_wndTreeCamera.DeleteItem(hOldItem);
				}
				break;
			default:
				assert(false);
				break;
			}	
		}
	}
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnNMDblclkListHoistheight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
