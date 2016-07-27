
// ContainerIDCaptureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ContainerIDCapture.h"
#include "ContainerIDCaptureDlg.h"
#include "CameraPropertyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define _Image_Device			0
#define _Image_PLC				1
#define _Image_Camera			2
#define _Image_CameraConnect	3


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CContainerIDCaptureDlg 对话框


CContainerIDCaptureDlg::CContainerIDCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContainerIDCaptureDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pVideoWndFrame = NULL;
	m_pWndItemEdit = nullptr;
	m_bPopError = true;
	m_bPopPLCError = true;
	m_nCurSelectedCamera = -1;
	m_nCurSelectedPLC = -1;
	m_hThreadCapture = NULL;
	m_pImageList = NULL;
	m_hThreadRecvPLC = NULL;
	m_bThreadRecvPLCRun = false;
	m_bConnected = false;
	
	m_bStartCapture = false;
	m_nPLCListenPort = 5085;

	ZeroMemory(m_DemandHeightArray,sizeof(m_DemandHeightArray));
	m_DemandHeightArray[0] = 10;
	m_DemandHeightArray[1] = 20;
	m_DemandHeightArray[2] = 30;
	m_nDemannHeightCount = 3;
}

CContainerIDCaptureDlg::~CContainerIDCaptureDlg()
{
	if (m_pVideoWndFrame)
		delete m_pVideoWndFrame;
	if (m_pImageList)
		delete m_pImageList;
 	if (m_pWndItemEdit)
 		delete m_pWndItemEdit;
// 	if (m_pWndItemEditPLC)
// 		delete m_pWndItemEditPLC;
}

void CContainerIDCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CContainerIDCaptureDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	//ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CContainerIDCaptureDlg::OnBnClickedButtonConnect)
	//ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CContainerIDCaptureDlg::OnBnClickedButtonDisconnect)
	//ON_NOTIFY(NM_RCLICK, IDC_LIST_CAMERA, &CContainerIDCaptureDlg::OnNMRClickListCamera)
	//ON_NOTIFY(NM_DBLCLK, IDC_LIST_CAMERA, &CContainerIDCaptureDlg::OnNMDblclkListCamera)
	//ON_COMMAND(ID_CONNECT_CAMERA, &CContainerIDCaptureDlg::OnConnectCamera)
	ON_COMMAND(ID_CONNECT_ALLCAMERA, &CContainerIDCaptureDlg::OnConnectAllcamera)
	//ON_COMMAND(ID_DISCONNECT_CAMERA, &CContainerIDCaptureDlg::OnDisconnectCamera)
	ON_COMMAND(ID_DISCONNECT_ALL, &CContainerIDCaptureDlg::OnDisconnectAll)
	ON_COMMAND(ID_DELETE_CAMERA, &CContainerIDCaptureDlg::OnDeleteCamera)
	ON_COMMAND(ID_DELETE_ALLCAMERA, &CContainerIDCaptureDlg::OnDeleteAllcamera)
	ON_MESSAGE(WM_CTRLS_KILLFOCUS,&CContainerIDCaptureDlg::OnKillFocusCtrls)
	ON_WM_DESTROY()
	//ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT, &CContainerIDCaptureDlg::OnBnClickedButtonSnapshot)
	//ON_NOTIFY(NM_RCLICK, IDC_LIST_PLCMUDULE, &CContainerIDCaptureDlg::OnNMRClickListPlcmudule)
 	//ON_COMMAND(ID_DELETE_PLC, &CContainerIDCaptureDlg::OnDeletePlc)
 	//ON_COMMAND(ID_DELETE_ALLPLC, &CContainerIDCaptureDlg::OnDeleteAllplc)
	//ON_NOTIFY(NM_DBLCLK, IDC_LIST_PLCMUDULE, &CContainerIDCaptureDlg::OnNMDblclkListPlcmudule)
	//ON_NOTIFY(NM_CLICK, IDC_LIST_CAMERA, &CContainerIDCaptureDlg::OnNMClickListCamera)
	//ON_NOTIFY(NM_CLICK, IDC_LIST_PLCMUDULE, &CContainerIDCaptureDlg::OnNMClickListPlcmudule)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_TREECAMERA, &CContainerIDCaptureDlg::OnNMRClickTreecamera)
	ON_NOTIFY(NM_DBLCLK, IDC_TREECAMERA, &CContainerIDCaptureDlg::OnNMDblclkTreecamera)
	ON_NOTIFY(NM_CLICK, IDC_TREECAMERA, &CContainerIDCaptureDlg::OnNMClickTreecamera)
	ON_COMMAND(ID_ADD_CAMERA, &CContainerIDCaptureDlg::OnAddCamera)
	ON_COMMAND(ID_MODIFY_CAMERA, &CContainerIDCaptureDlg::OnModifyCamera)
	ON_COMMAND(ID_ADD_PLCNODE, &CContainerIDCaptureDlg::OnAddPlcnode)
	ON_COMMAND(ID_DELETE_PLCNODE, &CContainerIDCaptureDlg::OnDeletePlcnode)
	ON_COMMAND(ID_MODIFY_PLCNODE, &CContainerIDCaptureDlg::OnModifyPlcnode)
	ON_COMMAND(ID_DELETE_ALLPLCNODE, &CContainerIDCaptureDlg::OnDeleteAllplcnode)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREECAMERA, &CContainerIDCaptureDlg::OnTvnEndlabeleditTreecamera)
	ON_BN_CLICKED(IDC_BUTTON_START, &CContainerIDCaptureDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CContainerIDCaptureDlg::OnBnClickedButtonStop)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_HOISTHEIGHT, &CContainerIDCaptureDlg::OnNMDblclkListHoistheight)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CContainerIDCaptureDlg 消息处理程序
CListEdit* CreateEditForList(CListCtrl *pListCtrl,UINT nEditCtrlID)
{
	CRect rt;
	CListEdit* pItemEdit = new CListEdit;
	pItemEdit->m_bListEdit = TRUE;
	pItemEdit->Create(WS_CHILD|WS_BORDER|ES_RIGHT,CRect(0,0,1,1),pListCtrl,nEditCtrlID);

	pItemEdit->SetColor(GetSysColor(COLOR_BTNTEXT),GetSysColor(COLOR_BTNFACE));
	pItemEdit->ShowWindow(SW_HIDE);
	pListCtrl->GetSubItemRect(1,1,LVIR_BOUNDS,rt);
	rt.bottom += 3;
	rt.top -= 1;
	pItemEdit->MoveWindow(&rt);

	pItemEdit->m_nCurItem = -1;
	pItemEdit->m_nCurSubItem = -1;
	pItemEdit->SetSel(0,-1);
	return pItemEdit;
}

BOOL CContainerIDCaptureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

 	m_wndListHoistHeight.SubclassDlgItem(IDC_LIST_HOISTHEIGHT,this);
 	m_wndListHoistHeight.SetExtendedStyle(m_wndListHoistHeight.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

// 	m_wndPLCList.SubclassDlgItem(IDC_LIST_PLCMUDULE,this);
// 	m_wndPLCList.SetExtendedStyle(m_wndPLCList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
// 
 	int nCols = 0;
 	m_wndListHoistHeight.InsertColumn(nCols++, _T("序号"), LVCFMT_LEFT, 60);
 	m_wndListHoistHeight.InsertColumn(nCols++, _T("吊臂高度"), LVCFMT_LEFT, 160);
	//m_wndCameraList.InsertColumn(nCols++, _T("端口"), LVCFMT_LEFT, 55);
// 
// 	nCols = 0;
// 	m_wndPLCList.InsertColumn(nCols ++,_T("序号"),LVCFMT_LEFT, 55);
// 	m_wndPLCList.InsertColumn(nCols++, _T("PLC模块编号"), LVCFMT_LEFT, 160);

	SetDlgItemText(IDC_EDIT_QUERYPERIOD,_T("200"));

	//ModifyStyle(0,WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

	// 创建List内编辑框	
 	m_pWndItemEdit = CreateEditForList(&m_wndListHoistHeight,IDC_FIELD_EDIT);
 	//m_pWndItemEditPLC = CreateEditForList(&m_wndListHoistHeight,IDC_FIELD_EDIT);

//  	m_wndListHoistHeight.InsertItem(m_wndListHoistHeight.GetItemCount(),_T(""));
//  	m_wndPLCList.InsertItem(m_wndPLCList.GetItemCount(),_T(""));

	m_pImageList = new CImageList();
	m_pImageList->Create(22,22,ILC_COLOR32 | ILC_MASK, 2, 1);  

	HICON hDeviceIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEVICE);
	m_pImageList->Add(hDeviceIcon);

	hDeviceIcon = AfxGetApp()->LoadIcon(IDI_ICON_PLCDEVICE);
	m_pImageList->Add(hDeviceIcon);

	hDeviceIcon = AfxGetApp()->LoadIcon(IDI_ICON_CAMERA);
	m_pImageList->Add(hDeviceIcon);

	hDeviceIcon = AfxGetApp()->LoadIcon(IDI_ICON_CAMERACONNECT);
	m_pImageList->Add(hDeviceIcon);

	m_wndTreeCamera.SubclassDlgItem(IDC_TREECAMERA,this);
	m_wndTreeCamera.SetImageList(m_pImageList,TVSIL_NORMAL);
	
	int nCameraCount = 0;
	LoadSetting(nCameraCount);
	CRect rtClient;
	GetDlgItemRect(IDC_STATIC_FRAME, rtClient);
	m_pVideoWndFrame = new CVideoFrame;
	if (nCameraCount == 0)
		nCameraCount = 4;
	m_pVideoWndFrame->Create(1024, rtClient, nCameraCount, this);

	m_wndStatusBar.SubclassDlgItem(IDC_STATIC_STATUSBAR,this);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CContainerIDCaptureDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CContainerIDCaptureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CContainerIDCaptureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool CContainerIDCaptureDlg::LoadSetting(int &nCount)
{
	TCHAR szPath[MAX_PATH] = { 0 };
	GetAppPath(szPath, MAX_PATH);
	_tcscat_s(szPath, MAX_PATH, _T("\\Configuration.xml"));
	if (!PathFileExists(szPath))
	{
		return false;
	}
	CMarkup xml;
	if (!xml.Load(szPath))
		return false;
	/*
	<?xml version="1.0" encoding="utf-8"?>
	<Configuration PLCListenPort="5085">
		<Snapshot>
			<HoistHeight Value = "10"/>
			<HoistHeight Value = "20"/>
			<HoistHeight Value = "30"/>
		</Snapshot>
		<CameraList>
			<PLCGroup BlockID = "501">
				<Camera IP="192.168.1.26"/>
				<Camera IP="192.168.1.30"/>
			</PLCGroup>
			<PLCGroup BlockID = "502">
				<Camera IP="192.168.1.26"/>
				<Camera IP="192.168.1.30"/>	
			</PLCGroup >
		</CameraList>
	</Configuration>
	*/

	nCount = 0;	
	TCHAR szItemText[64] = {0};
	CString strPLCNodeID;
	//插入根节点  
	CString str = _T("设备");
	CString strValue;
	HTREEITEM hRoot = m_wndTreeCamera.InsertItem(str,_Image_Device,_Image_Device);
	HTREEITEM hPLCNode = NULL;
	CString strIP,strAcount,strPassword,strPort;
	if (xml.FindElem(_T("Configuration")))
	{
		SetDlgItemText(IDC_EDIT_PLCLISTENPORT,xml.GetAttrib(_T("PLCListenPort")));
		if (xml.GetAttrib(_T("IPCPlayer")) == _T("true"))
			CheckDlgButton(IDC_CHECK_IPCPLAYSDK,BST_CHECKED);
		if (xml.GetAttrib(_T("HWAccel")) == _T("true"))
			CheckDlgButton(IDC_CHECK_ENABLEHWACCEL,BST_CHECKED);
		if (xml.GetAttrib(_T("FastConnnect")) == _T("true"))
			CheckDlgButton(IDC_CHECK_PARALLEL,BST_CHECKED);
		if (xml.GetAttrib(_T("ForceIFrame")) == _T("true"))
			CheckDlgButton(IDC_CHECK_FORCEIFRAME,BST_CHECKED);

		if (xml.FindChildElem(_T("Snapshot")))
		{
			xml.IntoElem();
			m_nDemannHeightCount = 0;
			while(xml.FindChildElem(_T("HoistHeight")))
			{
				xml.IntoElem();
				strValue =  xml.GetAttrib(_T("Value"));
				m_DemandHeightArray[m_nDemannHeightCount] = _tcstolong((LPCTSTR)strValue);
				_itot(m_nDemannHeightCount + 1,szItemText,10);
				m_wndListHoistHeight.InsertItem(m_nDemannHeightCount,szItemText);
				m_wndListHoistHeight.SetItemText(m_nDemannHeightCount,1,strValue);
				m_nDemannHeightCount ++;				
				xml.OutOfElem();
			}
			m_wndListHoistHeight.InsertItem(m_nDemannHeightCount,_T(""));
			xml.OutOfElem();
			
		}
		if (xml.FindChildElem(_T("CameraList")))
		{
			xml.IntoElem();
			while (xml.FindChildElem(_T("PLCGroup")))
			{
				xml.IntoElem();
				strPLCNodeID = xml.GetAttrib(_T("PLCNodeID"));
				UINT nPLCNodeID = _tcstolong((LPCTSTR)strPLCNodeID);
				HoistHeight *phhp = new HoistHeight;
				ZeroMemory(phhp,sizeof(HoistHeight));
				phhp->nBlockID = nPLCNodeID;
				m_vHoistHeight.push_back(HoistHeightPtr(phhp));
				_stprintf(szItemText,_T("%d"),nPLCNodeID);
				hPLCNode = m_wndTreeCamera.InsertItem(szItemText,_Image_PLC,_Image_PLC,hRoot);
				while (xml.FindChildElem(_T("Camera")))
				{
					xml.IntoElem();
					strIP		 = xml.GetAttrib(_T("IP"));
					strAcount	 = xml.GetAttrib(_T("Account"));
					strPassword	 = xml.GetAttrib(_T("Password"));
					strPort		 = xml.GetAttrib(_T("Port"));
					HTREEITEM hTreeItem = m_wndTreeCamera.InsertItem(strIP,_Image_Camera,_Image_Camera,hPLCNode);
					if (hTreeItem)
					{
						CCameraPropertyPtr pCamera = make_shared<CCameraProperty>((HWND)NULL,(LPCTSTR)strIP,(LPCTSTR)strAcount,(LPCTSTR)strPassword,(WORD)_tcstolong((LPCTSTR)strPort));
						pCamera->nPLCNode = nPLCNodeID;
						m_listCameraArray.push_back(pCamera);
						m_wndTreeCamera.SetItemData(hTreeItem,(DWORD_PTR)pCamera.get());
					}
					nCount ++;
					xml.OutOfElem();
				}
				xml.OutOfElem();
			}
			xml.OutOfElem();
		}
		//nCount = 0;	
	}

	m_wndTreeCamera.Expand(hRoot,TVE_EXPAND);
	if (m_wndTreeCamera.ItemHasChildren(hRoot))
	{
		HTREEITEM pItem = m_wndTreeCamera.GetChildItem(hRoot);
		while (pItem != NULL)
		{
			m_wndTreeCamera.Expand(pItem,TVE_EXPAND);
			pItem = m_wndTreeCamera.GetNextSiblingItem(pItem);
		}
	}
	
	return true;
}

bool CContainerIDCaptureDlg::SaveSetting()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	GetAppPath(szPath, MAX_PATH);
	_tcscat_s(szPath, MAX_PATH, _T("\\Configuration.xml"));

	TCHAR szItemText[64] = { 0 };
	CMarkup xml;
	TCHAR *szDoc = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	xml.SetDoc(szDoc);
	/*
	<?xml version="1.0" encoding="utf-8"?>
	<Configuration PLCListenPort="5085" IPCPlayer="true" HWAccel="true" FastConnnect="true" ForceIFrame="false">
		<Snapshot>
			<HoistHeight Value = "10"/>
			<HoistHeight Value = "20"/>
			<HoistHeight Value = "30"/>
		</Snapshot>
		<CameraList>
			<PLCGroup BlockID = "501">
				<Camera IP="192.168.1.26"/>
				<Camera IP="192.168.1.30"/>
			</PLCGroup>
			<PLCGroup BlockID = "502">
				<Camera IP="192.168.1.26"/>
				<Camera IP="192.168.1.30"/>	
			</PLCGroup >
		</CameraList>
	</Configuration>
	*/

 	GetDlgItemText(IDC_EDIT_PLCLISTENPORT,szItemText,64);
	bool bIPCPlayer = IsDlgButtonChecked(IDC_CHECK_IPCPLAYSDK) == BST_CHECKED;
	bool bEnableHWAccel = IsDlgButtonChecked(IDC_CHECK_ENABLEHWACCEL) == BST_CHECKED;
	bool bForceIFrame = IsDlgButtonChecked(IDC_CHECK_FORCEIFRAME) == BST_CHECKED;
	bool bFastConnect = IsDlgButtonChecked(IDC_CHECK_PARALLEL) == BST_CHECKED;

	xml.AddElem(_T("Configuration"));
	xml.AddAttrib(_T("PLCListenPort"), szItemText);
	xml.AddAttrib(_T("IPCPlayer"),bIPCPlayer?_T("true"):_T("false"));
	xml.AddAttrib(_T("HWAccel"),bEnableHWAccel?_T("true"):_T("false"));
	xml.AddAttrib(_T("FastConnnect"),bFastConnect?_T("true"):_T("false"));
	xml.AddAttrib(_T("ForceIFrame"),bForceIFrame?_T("true"):_T("false"));
	
	xml.IntoElem();	  // Configuration		

	xml.AddElem(_T("Snapshot"));
	int nItemCount = m_wndListHoistHeight.GetItemCount();
	for (int i = 0;i < nItemCount -1;i ++)
	{
		xml.AddChildElem(_T("HoistHeight"));
		m_wndListHoistHeight.GetItemText(i,1,szItemText,64);
		xml.IntoElem();
		xml.AddAttrib(_T("Value"),szItemText);
		xml.OutOfElem();
	}
	
	xml.AddElem(_T("CameraList"));	//CameraList
	xml.IntoElem();

	HTREEITEM hRoot = m_wndTreeCamera.GetRootItem();
	HTREEITEM hPLCItem = m_wndTreeCamera.GetChildItem(hRoot);
	CString strPLCNodeID;
	while (hPLCItem != NULL)
	{
		strPLCNodeID = m_wndTreeCamera.GetItemText(hPLCItem);
		UINT nPLCNodeID = _tcstolong((LPCTSTR)strPLCNodeID);
		// 添加PLC结点
		xml.AddElem(_T("PLCGroup"));		
		xml.AddAttrib(_T("PLCNodeID"),nPLCNodeID);

		HTREEITEM hCameraItem = m_wndTreeCamera.GetChildItem(hPLCItem);
		CCameraProperty *pCamera = NULL;
		while(hCameraItem)
		{	
			pCamera = (CCameraProperty *)m_wndTreeCamera.GetItemData(hCameraItem);
			if (!pCamera)
			{
				hCameraItem = m_wndTreeCamera.GetNextSiblingItem(hCameraItem);
				continue;;
			}

			xml.AddChildElem(_T("Camera"));
			xml.IntoElem();
			xml.AddAttrib(_T("IP"), pCamera->szCameraIP);
			xml.AddAttrib(_T("Account"), pCamera->szAccount);
			xml.AddAttrib(_T("Password"), pCamera->szPassword);
			xml.AddAttrib(_T("Port"), pCamera->nPort);
			xml.OutOfElem();
			
			hCameraItem = m_wndTreeCamera.GetNextSiblingItem(hCameraItem);
		}
		hPLCItem = m_wndTreeCamera.GetNextSiblingItem(hPLCItem);

	}
	xml.OutOfElem();// CameraList
	xml.OutOfElem(); // Configuration
	xml.Save(szPath);
	return true;

}



// 此处连接PLC控制器
void CContainerIDCaptureDlg::OnBnClickedButtonConnect()
{
	m_pVideoWndFrame->AdjustPanels(10);

}

// 此处断开PLC控制器

void CContainerIDCaptureDlg::OnBnClickedButtonDisconnect()
{
	m_pVideoWndFrame->AdjustPanels(4);
}


// 并发连接相机线程
UINT __stdcall   ThreadConnectCamera(void *p)
{
	CCameraProperty *pCamera = (CCameraProperty *)p;
	
	pCamera->StartPlay();
// 	CAxismediacontrol *pAxisCtrl = pCamera->pAxisCtrl;
// 	if (pAxisCtrl)
// 	{
// 		if (!pAxisCtrl->GetSafeHwnd())
// 		{// 创建控件
// 			if (!pCamera->CreateAxisCtrl())
// 			{
// 				assert(false);
// 				return 0;
// 			}
// 		}
// 		if (!pCamera->IsPlaying())
// 			pCamera->Start();
// 	}
	return 0;
}

// void CContainerIDCaptureDlg::OnConnectCamera()
// {
// 	HTREEITEM hTreeItem = m_wndTreeCamera.GetSelectedItem();
// 	CCameraProperty *pCamera = (CCameraProperty *)m_wndTreeCamera.GetItemData(hTreeItem);
// 	if (pCamera)
// 	{
// 		if (pCamera->IsPlaying())
// 			return ;
// 		int nIndex = 0;
// 		for (list<CCameraPropertyPtr>::iterator it = m_listCameraArray.begin();it != m_listCameraArray.end();it ++,nIndex ++)
// 			if ((*it).get() == pCamera)
// 			{
// 				break;
// 			}
// 		bool bIPCPlaySDK = IsDlgButtonChecked(IDC_CHECK_IPCPLAYSDK) == BST_CHECKED;
// 		bool bEnableHWAccel = IsDlgButtonChecked(IDC_CHECK_ENABLEHWACCEL) == BST_CHECKED;
// 		bool bForceIFrame = IsDlgButtonChecked(IDC_CHECK_FORCEIFRAME) == BST_CHECKED;
// 		
// 		m_pVideoWndFrame->AdjustPanels(m_listCameraArray.size());
// 
// 		pCamera->SetVideoWnd(m_pVideoWndFrame->GetPanelWnd(nIndex));
// 		pCamera->EnableIPCPlay(bIPCPlaySDK);
// 		pCamera->StartPlay(0,0,bForceIFrame,bEnableHWAccel);
// 	}
// }


void CContainerIDCaptureDlg::OnConnectAllcamera()
{
	if (m_bStartCapture)
		return;

	bool bIPCPlaySDK = IsDlgButtonChecked(IDC_CHECK_IPCPLAYSDK) == BST_CHECKED;
	bool bEnableHWAccel = IsDlgButtonChecked(IDC_CHECK_ENABLEHWACCEL) == BST_CHECKED;
	bool bForceIFrame = IsDlgButtonChecked(IDC_CHECK_FORCEIFRAME) == BST_CHECKED;

	TCHAR szItemText[32] = {0};
	//int nItemCount = m_wndCameraList.GetItemCount() - 1;
	
	m_pVideoWndFrame->AdjustPanels(m_listCameraArray.size());

	int nIndex = 0;
	UINT nParallelCheck = IsDlgButtonChecked(IDC_CHECK_PARALLEL);
	for (list<CCameraPropertyPtr>::iterator it  = m_listCameraArray.begin();it != m_listCameraArray.end();it ++,nIndex ++)
	{
		CCameraPropertyPtr pCameraPtr = *it;
		if (!pCameraPtr->CompareCamera(szItemText) || 
			! pCameraPtr->CompareWnd(m_pVideoWndFrame->GetPanelWnd(nIndex)))
		{
			pCameraPtr->Stop();
			pCameraPtr->SetVideoWnd(m_pVideoWndFrame->GetPanelWnd(nIndex));
			
		}
		pCameraPtr->EnableIPCPlay(bIPCPlaySDK);
		pCameraPtr->EnableFitWindow(true);
		pCameraPtr->EnableHWAccel(bEnableHWAccel);
		pCameraPtr->SetForceIFrame(bForceIFrame);
		if (nParallelCheck != BST_CHECKED)
			pCameraPtr->StartPlay();
		else
			pCameraPtr->hThreadConnect = (HANDLE )_beginthreadex(NULL,0,ThreadConnectCamera,pCameraPtr.get(),0,0);
	}
	m_bConnected = true;
}

// void CContainerIDCaptureDlg::OnDisconnectCamera()
// {
// 	CWaitCursor Wait;
// 	HTREEITEM hTreeItem = m_wndTreeCamera.GetSelectedItem();
// 	CCameraProperty *pCamera = (CCameraProperty *)m_wndTreeCamera.GetItemData(hTreeItem);
// 	if (pCamera)
// 	{
// 		pCamera->Stop();
// 	}
// 	m_bConnected = false;
// }

void CContainerIDCaptureDlg::OnDisconnectAll()
{
	CWaitCursor Wait;
	if (m_bStartCapture)
		return;
	for (list<CCameraPropertyPtr>::iterator it  = m_listCameraArray.begin();it != m_listCameraArray.end();it ++)
	{
		if ((*it)->IsPlaying())
			(*it)->Stop();
	}
}

void CContainerIDCaptureDlg::OnDeleteCamera()
{
	if (m_bStartCapture)
		return;
	HTREEITEM hTreeItem = m_wndTreeCamera.GetSelectedItem();
	if (!hTreeItem)
		return;
	CCameraProperty *pCamera = (CCameraProperty *)m_wndTreeCamera.GetItemData(hTreeItem);
	if (!pCamera)
		return ;
	
	pCamera->Stop();
	RemoveCamera(pCamera);
	m_wndTreeCamera.DeleteItem(hTreeItem);
	SaveSetting();
}

void CContainerIDCaptureDlg::OnDeleteAllcamera()
{
	if (m_bStartCapture)
		return;
	HTREEITEM hCurItem = m_wndTreeCamera.GetSelectedItem();
	HTREEITEM hRoot = m_wndTreeCamera.GetRootItem();
	int nResID = 0;
	if (!hCurItem  || hCurItem == hRoot)
	{
		nResID = AfxMessageBox(_T("确定要删除所有的相机吗？删除后将无法恢复！"),MB_YESNO);
		if (nResID == IDYES)
		{
			HTREEITEM hRoot = m_wndTreeCamera.GetRootItem();
			HTREEITEM pChildItem = m_wndTreeCamera.GetChildItem(hRoot);
			// 删除PLC结点，并删除其下所有相机
			while (pChildItem != NULL)
			{
				HTREEITEM hOldItem = pChildItem;
				AccessPLCNode(pChildItem);
				pChildItem = m_wndTreeCamera.GetNextSiblingItem(pChildItem);
				m_wndTreeCamera.DeleteItem(hOldItem);
			}
			m_listCameraArray.clear();
		}
	}
	else
	{
		nResID = AfxMessageBox(_T("确定要删除当前PLC结点下的所有的相机吗？删除后将无法恢复！"),MB_YESNO);
		if (nResID == IDYES)
		{
			
			HTREEITEM pChildItem = m_wndTreeCamera.GetChildItem(hCurItem);
			AccessPLCNode(hCurItem);
		}
	}
	SaveSetting();
}

// 响应WM_CTRLS_KILLFOCUS消息
// LIST 内控件失去焦点
// WPARAM 高位为List窗口ID,低位为控件窗口ID,LPARAM 高位为Item值，低位为subItem值

LRESULT CContainerIDCaptureDlg::OnKillFocusCtrls(WPARAM w,LPARAM l)
{
	UINT nListID = HIWORD(w);
	UINT nCtrlID = LOWORD(w);
	int nItem = HIWORD(l);
	int nSubItem = LOWORD(l);
	TCHAR szItemText[64] = {0};
	if (nCtrlID == m_pWndItemEdit->GetDlgCtrlID())
	{
		int nItemCount = m_wndListHoistHeight.GetItemCount();
		if (nSubItem == 1)		// IP地址
		{
			m_wndListHoistHeight.GetItemText(nItem,nSubItem,szItemText,64);
			if (_tcslen(szItemText) == 0)
			{
				m_pWndItemEdit->ShowWindow(SW_SHOW);
				m_pWndItemEdit->SetFocus();
				return 0;
				
			}
			else if (nItemCount == nItem + 1)		// 且是最后一行,则增加新的空行
			{
				_itot(nItemCount,szItemText,10);
				m_wndListHoistHeight.SetItemText(nItem,0,szItemText);
				m_wndListHoistHeight.InsertItem(nItemCount,_T(""));
			}
			SaveSetting();
		}
	}
// 	if (nCtrlID == m_pWndItemEditPLC->GetDlgCtrlID())
// 	{
// 		int nItemCount = m_wndPLCList.GetItemCount();
// 		if (nSubItem == 1)		// IP地址
// 		{
// 			m_wndPLCList.GetItemText(nItem,nSubItem,szItemText,64);
// 			if (_tcslen(szItemText) == 0)
// 				return 0;
// 			if (!IsNumeric(szItemText,_tcslen(szItemText)))		// ID无效
// 			{
// 				TraceMsgA("%s m_bPopError = %s.\n",__FUNCTION__,m_bPopError?"true":"false");
// 				if (!m_bPopPLCError)
// 				{
// 					//MessageBox(_T("您输入的PLC编号有误,必须为数字."),_T("提示"),MB_OK|MB_ICONSTOP);
// 					m_wndStatusBar.SetWindowText(_T("您输入的PLC编号有误,请重新输入."));
// 					m_wndStatusBar.SetAlarmGllitery();
// 					m_bPopPLCError = true;
// 				}
// 				m_pWndItemEditPLC->ShowWindow(SW_SHOW);
// 				m_pWndItemEditPLC->SetFocus();
// 				return 0;
// 
// 			}
// 			else if (nItemCount == nItem + 1)		// 且是最后一行,则增加新的空行
// 			{
// 				_itot(nItemCount,szItemText,10);
// 				m_wndPLCList.SetItemText(nItem,0,szItemText);
// 				m_wndPLCList.InsertItem(nItemCount,_T(""));
// 			}
// 			SaveSetting();
// 		}
// 	}
	return 0;
}
void CContainerIDCaptureDlg::OnDestroy()
{
	CDialog::OnDestroy();
	SaveSetting();
}
// 
// void CContainerIDCaptureDlg::OnBnClickedButtonSnapshot()
// {
// // 	int nPanel =  m_pVideoWndFrame->GetCurPanel();
// // 	if (nPanel != -1 && nPanel < m_vCameraArray.size())
// // 	{
// // 		SYSTEMTIME systime;
// // 		GetLocalTime(&systime);
// // 		TCHAR szFileName[1024] = {0};
// // 		TCHAR szAppPath[1024] = {0};
// // 		GetAppPath(szAppPath,1024);
// // 
// // 		_stprintf(szFileName,_T("%s\\SnapShot_%04d%02d%02d_%02d%02d%02d_%03d.bmp"),szAppPath,systime.wYear,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond,systime.wMilliseconds);
// // 
// // 		m_vCameraArray[nPanel]->SnapShot(szFileName,XIFF_BMP);
// // 	}
// 	CWaitCursor Wait;
// 	if (m_hThreadCapture)
// 	{
// 		m_bCaptureRun = false;
// 		m_wndCameraList.EnableWindow(TRUE);
// 		m_wndPLCList.EnableWindow(TRUE);
// 		EnableDlgItem(IDC_EDIT_QUERYPERIOD,TRUE);
// 		SetDlgItemText(IDC_BUTTON_SNAPSHOT,_T("截图测试"));
// 		WaitForSingleObject(m_hThreadCapture,INFINITE);
// 		CloseHandle(m_hThreadCapture);
// 		m_hThreadCapture = NULL;
// 	}
// 	else
// 	{
// 		if (m_wndCameraList.GetItemCount() == 0)
// 		{
// 			AfxMessageBox(_T("请先添加相机."),MB_ICONSTOP);
// 			return ;
// 
// 		}
// 		m_bCaptureRun = true;
// 		m_nQueryPeriod = GetDlgItemInt(IDC_EDIT_QUERYPERIOD);
// 		if (m_nQueryPeriod == 0)
// 			m_nQueryPeriod = 200;
// 		m_wndCameraList.EnableWindow(FALSE);
// 		m_wndPLCList.EnableWindow(FALSE);
// 		EnableDlgItem(IDC_EDIT_QUERYPERIOD,FALSE);
// 		SetDlgItemText(IDC_BUTTON_SNAPSHOT,_T("停止测试"));
// 		OnConnectAllcamera();
// 		m_hThreadCapture = (HANDLE)_beginthreadex(NULL,0,ThreadCapture,this,0,0);
// 	}
// }


void CContainerIDCaptureDlg::CheckDirectory(TCHAR *szTodayDir)
{
	TCHAR szDirName[1024] = {0};
	TCHAR szAppPath[1024] = {0};
	GetAppPath(szAppPath,1024);

	TCHAR szOldCurrentDir[1024];
	GetCurrentDirectory(1024,szOldCurrentDir);
	
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	_stprintf(szTodayDir,_T("%s\\Capture\\%04d%02d%02d"),szAppPath,systime.wYear,systime.wMonth,systime.wDay);
	if (!PathFileExists(szTodayDir))
	{
		CreateDirectoryTree(szTodayDir);
	}
	SetCurrentDirectory(szTodayDir);
	
	for (list<CCameraPropertyPtr>::iterator it = m_listCameraArray.begin();it != m_listCameraArray.end();it ++)
	{
		_stprintf(szDirName,_T("%s"),(*it)->szCameraIP);
		if (!PathFileExists(szDirName))
		{
			CreateDirectory(szDirName,NULL);
		}
	}
	SetCurrentDirectory(szOldCurrentDir);
}
UINT CContainerIDCaptureDlg::ThreadCapture(void *p)
{
	CContainerIDCaptureDlg *pThis = (CContainerIDCaptureDlg *)p;
	double dfLastCapture = GetExactTime();
	int nCurCamera = 0;
	bool bPause = false;
	TCHAR szFileName[1024] = {0};
	TCHAR szAppPath[1024] = {0};
	GetAppPath(szAppPath,1024);
	SYSTEMTIME systime;
	SYSTEMTIME sysTimeToday ;
	GetLocalTime(&sysTimeToday);
	TCHAR szTodayDir[1024] = {0};
	pThis->CheckDirectory(szTodayDir);
	list<CCameraPropertyPtr>::iterator itloop = pThis->m_listCameraArray.begin();
	while(pThis->m_bStartCapture)
	{
		if (bPause )
		{
			if (TimeSpanEx(dfLastCapture)*1000 < (double)pThis->m_nQueryPeriod)
			{
				Sleep( 10);
				continue;
			}
			else
				bPause = false;
		}
		if (itloop != pThis->m_listCameraArray.end())
		{
			CCameraProperty *pCamera = (*itloop).get();
			if (pCamera->IsPlaying())
			{
				GetLocalTime(&systime);
				if (systime.wDay != sysTimeToday.wDay ||
					systime.wMonth != systime.wMonth ||
					systime.wYear != sysTimeToday.wYear)
				{
					pThis->CheckDirectory(szTodayDir);
					memcpy(&sysTimeToday,&systime,sizeof(SYSTEMTIME));
				}
				_stprintf(szFileName,_T("%s\\%s\\SnapShot_%04d%02d%02d_%02d%02d%02d_%02d.bmp"),szTodayDir,pCamera->szCameraIP,systime.wYear,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond,systime.wMilliseconds);
				pCamera->SnapShot(szFileName,XIFF_BMP);
				dfLastCapture = GetExactTime();
			}
			itloop ++;
			if (itloop == pThis->m_listCameraArray.end())
			{
				itloop = pThis->m_listCameraArray.begin();
				bPause = true;
			}
		}

		Sleep(10);
	}
	return 0;
}


void CContainerIDCaptureDlg::OnClose()
{
	SaveSetting();
	CDialog::OnClose();
}

void CContainerIDCaptureDlg::OnNMRClickTreecamera(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pia = (LPNMITEMACTIVATE)pNMHDR;
	POINT point,ScreenPoint;
	GetCursorPos(&point);
	ScreenPoint = point;
	UINT uFlags;
	
	m_wndTreeCamera.ScreenToClient(&point);
	HTREEITEM hTreeItem = m_wndTreeCamera.HitTest(point, &uFlags);

	if ((hTreeItem != NULL) && (TVHT_ONITEM & uFlags))
	{
		m_wndTreeCamera.SelectItem(hTreeItem);
	}

	CMenu menu;
	menu.LoadMenu(IDR_MENU_POPUP);
	CMenu *pPopMenu = menu.GetSubMenu(0);
	if (!pPopMenu)
		return;
	for (int i = 0;i < pPopMenu->GetMenuItemCount();i ++)
		pPopMenu->EnableMenuItem(i,MF_DISABLED|MF_BYPOSITION);
	/*
	
	连接相机	
	断开相机
	-------------
	添加相机
	删除相机
	修改相机
	删除所有相机
	-------------
	添加PLC结点
	删除PLC结点
	修改PLC结点
	删除所有PLC结点
	*/
	*pResult = 0;

	if (m_bStartCapture)		// 若截图已经开始，则禁用所有配置菜单
	{
		pPopMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,ScreenPoint.x,ScreenPoint.y,this);
		return ;
	}
// 	if (!m_bConnected)
// 	{
		if (hTreeItem)
		{
			CCameraProperty *pCamera = (CCameraProperty *)m_wndTreeCamera.GetItemData(hTreeItem);
			if (pCamera)
			{// 当前结点为相机
				
				pPopMenu->EnableMenuItem(ID_DELETE_CAMERA,MF_ENABLED|MF_BYCOMMAND);
				pPopMenu->EnableMenuItem(ID_MODIFY_CAMERA,MF_ENABLED|MF_BYCOMMAND);
				
			}
			else
			{// 非相机结点，可能为根或PLC结点
				if (hTreeItem == m_wndTreeCamera.GetRootItem())
				{// 根结点
					pPopMenu->EnableMenuItem(ID_CONNECT_ALLCAMERA,MF_ENABLED|MF_BYCOMMAND);		// 断所有相机
					pPopMenu->EnableMenuItem(ID_DISCONNECT_ALL,MF_ENABLED|MF_BYCOMMAND);		// 断所有相机
					pPopMenu->EnableMenuItem(ID_DELETE_ALLCAMERA,MF_ENABLED|MF_BYCOMMAND);		// 删除所有相机
					pPopMenu->EnableMenuItem(ID_ADD_PLCNODE,MF_ENABLED|MF_BYCOMMAND);			// 添加PLC
					pPopMenu->EnableMenuItem(ID_DELETE_ALLPLCNODE,MF_ENABLED|MF_BYCOMMAND);		// 删除所有PLC
				}
				else
				{
					// 当前结点为PLC
					pPopMenu->EnableMenuItem(ID_CONNECT_ALLCAMERA,MF_ENABLED|MF_BYCOMMAND);		// 断所有相机
					pPopMenu->EnableMenuItem(ID_ADD_CAMERA,MF_ENABLED|MF_BYCOMMAND);			// 添加相机
					pPopMenu->EnableMenuItem(ID_DISCONNECT_ALL,MF_ENABLED|MF_BYCOMMAND);		// 断开当前结点下的所有相机
					pPopMenu->EnableMenuItem(ID_DELETE_ALLCAMERA,MF_ENABLED|MF_BYCOMMAND);		// 删除当前结点下的所有相机

					pPopMenu->EnableMenuItem(ID_MODIFY_PLCNODE,MF_ENABLED|MF_BYCOMMAND);
					pPopMenu->EnableMenuItem(ID_DELETE_PLCNODE,MF_ENABLED|MF_BYCOMMAND);
				}	
			}
		}
		else
		{
			pPopMenu->EnableMenuItem(ID_CONNECT_ALLCAMERA,MF_ENABLED|MF_BYCOMMAND);		// 断所有相机
			pPopMenu->EnableMenuItem(ID_DISCONNECT_ALL,MF_ENABLED|MF_BYCOMMAND);		// 断所有相机
			pPopMenu->EnableMenuItem(ID_DELETE_ALLCAMERA,MF_ENABLED|MF_BYCOMMAND);		// 删除所有相机

			pPopMenu->EnableMenuItem(ID_ADD_PLCNODE,MF_ENABLED|MF_BYCOMMAND);			// 添加PLC
			pPopMenu->EnableMenuItem(ID_DELETE_ALLPLCNODE,MF_ENABLED|MF_BYCOMMAND);		// 删除所有PLC
		}
// 	}
// 	else
// 	{
// 		pPopMenu->EnableMenuItem(ID_DISCONNECT_ALL,MF_ENABLED|MF_BYCOMMAND);		// 断开所有相机
// 	}
	
	pPopMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,ScreenPoint.x,ScreenPoint.y,this);

}

void CContainerIDCaptureDlg::OnNMDblclkTreecamera(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 相机是否已执行连接操作
	HTREEITEM hTreeItem = m_wndTreeCamera.GetSelectedItem();
	CCameraProperty *pCamera = (CCameraProperty *)m_wndTreeCamera.GetItemData(hTreeItem);
	if (pCamera)
	{
		int nIndex = 0;
		for (list<CCameraPropertyPtr>::iterator it = m_listCameraArray.begin();it != m_listCameraArray.end();it ++,nIndex ++)
			if ((*it).get() == pCamera)
			{
				break;
			}
		bool bIPCPlaySDK = IsDlgButtonChecked(IDC_CHECK_IPCPLAYSDK) == BST_CHECKED;
		bool bEnableHWAccel = IsDlgButtonChecked(IDC_CHECK_ENABLEHWACCEL) == BST_CHECKED;
		bool bForceIFrame = IsDlgButtonChecked(IDC_CHECK_FORCEIFRAME) == BST_CHECKED;

		if (m_listCameraArray.size() >  m_pVideoWndFrame->GetPanelCount())
			m_pVideoWndFrame->AdjustPanels(m_listCameraArray.size());
	
		pCamera->EnableIPCPlay(bIPCPlaySDK);
		pCamera->EnableFitWindow(true);
		pCamera->EnableHWAccel(bEnableHWAccel);
		pCamera->SetForceIFrame(bForceIFrame);

		pCamera->StartPlay();
	}
	*pResult = 0;
}

void CContainerIDCaptureDlg::OnNMClickTreecamera(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CContainerIDCaptureDlg::OnAddCamera()
{
	HTREEITEM hCurItem = m_wndTreeCamera.GetSelectedItem();
	if (!hCurItem)
		return ;
	CString strPLCNode =  m_wndTreeCamera.GetItemText(hCurItem);
	CCameraPropertyDialog dlg;
	dlg.SetPLCNode((int)_tcstolong((LPCTSTR)strPLCNode));
	if (dlg.DoModal() == IDOK)
	{
		CCameraProperty *pCamera = dlg.GetCameraProperty();
		m_listCameraArray.push_back(CCameraPropertyPtr(pCamera));
		HTREEITEM hNewItem = m_wndTreeCamera.InsertItem(pCamera->szCameraIP,_Image_Camera,_Image_Camera, hCurItem);
		m_wndTreeCamera.SetItemData(hNewItem,(DWORD_PTR)pCamera);
		m_wndTreeCamera.Expand(hCurItem,TVE_EXPAND);
		SaveSetting();
	}
}

void CContainerIDCaptureDlg::OnModifyCamera()
{
	HTREEITEM hCurItem = m_wndTreeCamera.GetSelectedItem();
	if (!hCurItem)
		return ;
	HTREEITEM hParent = m_wndTreeCamera.GetParentItem(hCurItem);
	if (!hParent)
		return ;
	CString strPLCNode =  m_wndTreeCamera.GetItemText(hParent);
	CCameraProperty *pCamera = (CCameraProperty *)m_wndTreeCamera.GetItemData(hCurItem);
	if (!pCamera)
		return ;

	CCameraPropertyDialog dlg;
	dlg.SetPLCNode((int)_tcstolong((LPCTSTR)strPLCNode));
	dlg.SetCameraProperty(pCamera);	
	if (dlg.DoModal() == IDOK)
	{
		//CCameraProperty *pCamera = dlg.GetCameraProperty();
		//CCameraPropertyPtr CameraPtr(pCamera);
		//m_listCameraArray.push_back(CCameraPropertyPtr(pCamera));
		m_wndTreeCamera.SetItemText(hCurItem,pCamera->szCameraIP);
		SaveSetting();
	}
	
}

void CContainerIDCaptureDlg::OnAddPlcnode()
{
	HTREEITEM hRoot = m_wndTreeCamera.GetRootItem();
	HTREEITEM hNewPlcNode = m_wndTreeCamera.InsertItem(_T("新建PLC结点"),_Image_PLC,_Image_PLC,hRoot);
	m_wndTreeCamera.EditLabel(hNewPlcNode);
}

void CContainerIDCaptureDlg::OnDeletePlcnode()
{
	HTREEITEM hCurItem = m_wndTreeCamera.GetSelectedItem();
	if (!hCurItem)
		return;
	AccessPLCNode(hCurItem);
	m_wndTreeCamera.DeleteItem(hCurItem);
	SaveSetting();
}

void CContainerIDCaptureDlg::OnModifyPlcnode()
{
	HTREEITEM hCurItem = m_wndTreeCamera.GetSelectedItem();
	if (!hCurItem)
		return;

	HTREEITEM hRoot = m_wndTreeCamera.GetRootItem();
	if (m_wndTreeCamera.GetParentItem(hCurItem) != hRoot)
		return ;
	m_wndTreeCamera.EditLabel(hCurItem);
}

void CContainerIDCaptureDlg::OnDeleteAllplcnode()
{
	HTREEITEM hRoot = m_wndTreeCamera.GetRootItem();

	HTREEITEM pChildItem = m_wndTreeCamera.GetChildItem(hRoot);
	// 删除PLC结点，并删除其下所有相机
	while (pChildItem != NULL)
	{
		HTREEITEM hOldItem = pChildItem;
		pChildItem = m_wndTreeCamera.GetNextSiblingItem(pChildItem);
		m_wndTreeCamera.DeleteItem(hOldItem);
	}
	m_listCameraArray.clear();
	SaveSetting();
}


BOOL CContainerIDCaptureDlg::PreTranslateMessage(MSG* pMsg)
{
	if (!IsCancelDialogMessage(pMsg))
		return CDialog::PreTranslateMessage(pMsg);
	else
		return TRUE;
}

void CContainerIDCaptureDlg::OnTvnEndlabeleditTreecamera(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	
	if(pTVDispInfo->item.pszText==0) 
		return;//用户取消修改操作
	m_wndTreeCamera.SetItemText(pTVDispInfo->item.hItem,pTVDispInfo->item.pszText);//设置新数据
	SaveSetting();
	*pResult = 0;
}

void CContainerIDCaptureDlg::OnBnClickedButtonStart()
{
	m_nPLCListenPort = (WORD)GetDlgItemInt(IDC_EDIT_PLCLISTENPORT);
	if (m_nPLCListenPort <= 0)
	{
		AfxMessageBox(_T("PLC端口无效,请输入有效的端口值(1~65535)"),MB_OK|MB_ICONSTOP);
		return ;
	}
	CWaitCursor Wait;
	int nItemCount = m_wndListHoistHeight.GetItemCount();
	if (nItemCount <= 1)
	{
		AfxMessageBox(_T("请先配置抓图的吊臂高度"),MB_OK|MB_ICONSTOP);
		return ;
	}
	m_nDemannHeightCount = 0;
	TCHAR szText[64] = {0};
	for (int i = 0;i < nItemCount-1;i ++)
	{
		m_wndListHoistHeight.GetItemText(i,1,szText,64);
		m_DemandHeightArray[m_nDemannHeightCount ++] = _ttoi(szText);
	}
	
	this->OnConnectAllcamera();
	m_bThreadRecvPLCRun = true;
	m_hThreadRecvPLC = (HANDLE)_beginthreadex(NULL,0,ThreadRecvPLC,this,0,0);
	SetTimer(ID_RefreshHeight,200,NULL);
	m_bStartCapture = true;
	EnableDlgItem(IDC_BUTTON_START,FALSE);
	EnableDlgItem(IDC_BUTTON_STOP,TRUE);
	EnableDlgItem(IDC_EDIT_PLCLISTENPORT,FALSE);
}

void CContainerIDCaptureDlg::OnBnClickedButtonStop()
{
	CWaitCursor Wait;
	m_bThreadRecvPLCRun = false;	
	m_bStartCapture = false;
	HANDLE hArray[2] = {m_hThreadCapture,m_hThreadRecvPLC};
	WaitForMultipleObjects(2,hArray,TRUE,INFINITE);
	this->OnDisconnectAll();
	KillTimer(ID_RefreshHeight);
	m_bStartCapture = false;
	EnableDlgItem(IDC_BUTTON_START,TRUE);
	EnableDlgItem(IDC_BUTTON_STOP,FALSE);
	EnableDlgItem(IDC_EDIT_PLCLISTENPORT,TRUE);
}


UINT CContainerIDCaptureDlg::ThreadRecvPLC(void *p)
{
	CContainerIDCaptureDlg *pThis = (CContainerIDCaptureDlg *)p;
	WSADATA wsaData; 
	if (WSAStartup(MAKEWORD(2,0),&wsaData)) //调用Windows Sockets DLL 
	{ 
		WSACleanup(); 
		return 0; 
	} 
	shared_ptr<CRunlog> pRunlog = make_shared<CRunlog>(_T("PLCRunlog"));

	struct sockaddr_in local; 
	struct sockaddr_in ClientAddr; 
	int fromlen =sizeof(ClientAddr); 
	local.sin_family=AF_INET; 
	local.sin_port=htons(pThis->m_nPLCListenPort); ///监听端口 
	local.sin_addr.s_addr=INADDR_ANY; ///本机 
	SOCKET sServer = socket(AF_INET,SOCK_DGRAM,0); 
	bind(sServer,(struct sockaddr*)&local,sizeof(local)); 
	int nResult = 0;
	byte szRecvBuff[2048]={0};
	TCHAR szLogTitle[128] = {0};
	TCHAR szClientIP[64]  = {0};
	WORD  nClientPort = 0;
	int nDataLength = 0;
	short nBlockID = 0;
	short nOffset = 0;
	short nHoistHeight = 0;

	double dfLastCapture = GetExactTime();
	
	TCHAR szFileName[1024] = {0};
	TCHAR szAppPath[1024] = {0};
	GetAppPath(szAppPath,1024);
	SYSTEMTIME systime;
	SYSTEMTIME sysTimeToday ;
	GetLocalTime(&sysTimeToday);
	TCHAR szTodayDir[1024] = {0};
	pThis->CheckDirectory(szTodayDir);
	
	
	while (pThis->m_bThreadRecvPLCRun) 
	{ 
		ZeroMemory(szRecvBuff,2048);
		if ((nResult = recvfrom(sServer,(char *)szRecvBuff,sizeof(szRecvBuff),0,(struct sockaddr*)&ClientAddr,&fromlen))!=SOCKET_ERROR) 
		{ 
			UINT64 nClientID = MAKEUINT64((ULONG)ClientAddr.sin_addr.s_addr,ClientAddr.sin_port);
			_tcscpy(szClientIP,inet_ntow(ClientAddr.sin_addr));
			nClientPort = ClientAddr.sin_port;
			_stprintf(szLogTitle,_T("Recved PLC block from %s:%d:.\n"),szClientIP,nClientPort);
			pRunlog->RunlogBin((LPTSTR)szLogTitle,szRecvBuff,nResult);
			/// 解析得到的数据
			PLCHeader *pPlcHeader = (PLCHeader *)szRecvBuff;
			if (pPlcHeader->nIdentify != _PLC_IDENTIFY)
			{
				pRunlog->Runlog(_T("无效的PLCHeader标志(%02X %02X %02X %02X).\n"),szRecvBuff[0],szRecvBuff[1],szRecvBuff[2],szRecvBuff[3]);
				Sleep(10);
				continue;
			}
			nDataLength = ntohl(pPlcHeader->nLength);
			if (nDataLength <= 0 )
			{
				pRunlog->Runlog(_T("没有数据块或数据长度无效(%02x %02X).\n"),szRecvBuff[12],szRecvBuff[13],szRecvBuff[14],szRecvBuff[15]);
				continue;
			}
			
			nOffset = ntohs(pPlcHeader->nOffset);
			if (pPlcHeader->nOffset < 0 || nOffset > _HoistHeightOffset)
			{
				pRunlog->Runlog(_T("地址偏移无效(%02X %02X)."),szRecvBuff[8],szRecvBuff[9]);
				continue;
			}
		
			int nPlcBlockID = ntohs(pPlcHeader->nBlock);			
			byte *pData  = (byte *)&szRecvBuff[sizeof(PLCHeader)];
			WORD nHoistHeight = 0;
			memcpy(&nHoistHeight,&pData[70 - nOffset],2);	
			nHoistHeight = ntohs(nHoistHeight);
			bool bSnapshot = false;
			for (vector<HoistHeightPtr>::iterator it = pThis->m_vHoistHeight.begin();it != pThis->m_vHoistHeight.end();it ++)
			{
				if ((*it)->nBlockID == nPlcBlockID)
				{
					(*it)->nCurrentHeight = nHoistHeight;
					break;
				}
			}

			for (int i = 0;i < pThis->m_nDemannHeightCount;i ++)
			{
				if (nHoistHeight == pThis->m_DemandHeightArray[i])
				{
					bSnapshot = true;
					break;
				}
			}
			if (bSnapshot)
			{
				for (ListCamera::iterator itloop = pThis->m_listCameraArray.begin();itloop != pThis->m_listCameraArray.end();itloop ++)
				{
					CCameraProperty *pCamera = (*itloop).get();
					if (pCamera->nPLCNode == nPlcBlockID && pCamera->IsPlaying())
					{
						GetLocalTime(&systime);
						if (systime.wDay != sysTimeToday.wDay ||
							systime.wMonth != systime.wMonth ||
							systime.wYear != sysTimeToday.wYear)
						{
							pThis->CheckDirectory(szTodayDir);
							memcpy(&sysTimeToday,&systime,sizeof(SYSTEMTIME));
						}
						_stprintf(szFileName,_T("%s\\%s\\SnapShot_H%d_T%04d%02d%02d_%02d%02d%02d_%03d.bmp"),szTodayDir,pCamera->szCameraIP,nHoistHeight,systime.wYear,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond,systime.wMilliseconds);
						pCamera->SnapShot(szFileName,XIFF_BMP);
						dfLastCapture = GetExactTime();
					}
				}
			}
		} 
		Sleep(10); 
	} 
	closesocket(sServer);
	WSACleanup();    


	return 0;
}
void CContainerIDCaptureDlg::OnNMDblclkListHoistheight(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem < 0 || pNMItemActivate->iSubItem != 1)
		return;
	CRect rt;
	m_wndListHoistHeight.GetSubItemRect(pNMItemActivate->iItem,1,LVIR_BOUNDS,rt);
	rt.bottom += 3;
	rt.top -= 1;

	TCHAR szText[256] = {0};
	m_wndListHoistHeight.GetItemText(pNMItemActivate->iItem,pNMItemActivate->iSubItem,szText,256);
	m_pWndItemEdit->SetWindowText(szText);
	m_pWndItemEdit->MoveWindow(&rt);				
	m_pWndItemEdit->SetFocus();
	m_pWndItemEdit->m_nCurItem = pNMItemActivate->iItem;
	m_pWndItemEdit->m_nCurSubItem = pNMItemActivate->iSubItem;
	m_pWndItemEdit->SetSel(0,-1);	
	m_pWndItemEdit->ShowWindow(SW_SHOW);
	
	*pResult = 0;
}

void CContainerIDCaptureDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == ID_RefreshHeight)
	{
		HTREEITEM hRoot = m_wndTreeCamera.GetRootItem();
		HTREEITEM hPLCItem = m_wndTreeCamera.GetChildItem(hRoot);	
		CString strPLCNodeID;
		while (hPLCItem != NULL)
		{
			strPLCNodeID = m_wndTreeCamera.GetItemText(hPLCItem);
			UINT nPLCNodeID = _tcstolong((LPCTSTR)strPLCNodeID);
			int nCurHeight = -1;
			for (vector<HoistHeightPtr>::iterator it = m_vHoistHeight.begin();it != m_vHoistHeight.end();it ++)
			{
				if ((*it)->nBlockID == nPLCNodeID)
				{
					nCurHeight = (*it)->nCurrentHeight;
					break;
				}
			}
			if (nCurHeight > 0)
				strPLCNodeID.Format(_T("%d:(Height:%d)"),nPLCNodeID,nCurHeight);
			else
				strPLCNodeID.Format(_T("%d:(Height:-)"),nPLCNodeID,nCurHeight);
			m_wndTreeCamera.SetItemText(hPLCItem,strPLCNodeID);

			hPLCItem = m_wndTreeCamera.GetNextSiblingItem(hPLCItem);

		}
	}

	CDialog::OnTimer(nIDEvent);
}
