
// TAxisMediaCtrlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TAxisMediaCtrl.h"
#include "TAxisMediaCtrlDlg.h"
#include "axismediacontrol.h"
#include "Utility.h"
enum AMC_STATUS
{
	AMC_STATUS_NONE				 = 0,
	AMC_STATUS_INITIALIZED		 = 1,
	AMC_STATUS_PLAYING			 = 2,
	AMC_STATUS_PAUSED			 = 4,
	AMC_STATUS_RECORDING		 = 8,
	AMC_STATUS_OPENING			 = 16,
	AMC_STATUS_RECONNECTING		 = 32,
	AMC_STATUS_ISSUING_PTZ_COMMAND = 512,
	AMC_STATUS_EXTENDED_TEXT	 = 1024,
	AMC_STATUS_PTZ_UIMODE_ABS	 = 2048,
	AMC_STATUS_PTZ_UIMODE_REL	 = 4096,
	AMC_STATUS_OPENING_RECEIVE_AUDIO = 65536,
	AMC_STATUS_OPENING_TRANSMIT_AUDIO = 131072,
	AMC_STATUS_RECEIVE_AUDIO	 = 262144,
	AMC_STATUS_TRANSMIT_AUDIO	 = 524288,
	AMC_STATUS_TRANSMIT_AUDIO_FILE = 1048576,
	AMC_STATUS_RECORDING_AUDIO	 = 2097152,
	AMC_STATUS_HW_DECODING		 = 4194304
};


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CTAxisMediaCtrlDlg 对话框




CTAxisMediaCtrlDlg::CTAxisMediaCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTAxisMediaCtrlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pWndAmc = NULL;
}

void CTAxisMediaCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTAxisMediaCtrlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CTAxisMediaCtrlDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CTAxisMediaCtrlDlg::OnBnClickedButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_GETSTATUS, &CTAxisMediaCtrlDlg::OnBnClickedButtonGetstatus)
END_MESSAGE_MAP()


// CTAxisMediaCtrlDlg 消息处理程序

BOOL CTAxisMediaCtrlDlg::OnInitDialog()
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
	SetDlgItemText(IDC_IPADDRESS1,_T("192.168.0.90"));
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTAxisMediaCtrlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTAxisMediaCtrlDlg::OnPaint()
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
HCURSOR CTAxisMediaCtrlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTAxisMediaCtrlDlg::OnTimer(UINT_PTR nIDEvent)
{
	

	CDialog::OnTimer(nIDEvent);
}

void CTAxisMediaCtrlDlg::OnBnClickedButtonConnect()
{
	try
	{
		m_pWndAmc = new CAxismediacontrol();

		if (!m_pWndAmc)
		{
			MessageBox(_T("创建AXIS控件对象失败."));
			return ;
		}

		CRect rtFrame;
		GetDlgItem(IDC_STATIC_FRAME)->GetWindowRect(&rtFrame);
		ScreenToClient(&rtFrame);
		if (!m_pWndAmc->Create(_T("AsixMediaCtrl"),WS_VISIBLE|WS_CHILD,rtFrame,this,1024))
		{
			MessageBox(_T("创建AXIS控件失败."));
			return ;
		}
		m_pWndAmc->ShowWindow(SW_SHOW);
		CString m_ipText;
		GetDlgItemText(IDC_IPADDRESS1,m_ipText);

		CString ctrlURL, presetURL, mediaURL;

		ctrlURL.Format(_T("http://%s/axis-cgi/com/ptz.cgi"), m_ipText);
		mediaURL.Format(_T("rtsp://%s/axis-media/media.amp?camera=1&videocodec=h264"), m_ipText);
		//presetURL.Format(_T("http://%s/axis-cgi/param.cgi?usergroup=anonymous&action=list&group=PTZ.Preset.P0"), m_ipText);
		// Firmware version 4
		presetURL.Format(_T("http://%s/axis-cgi/view/param.cgi?action=list&group=PTZ.Preset.P0"), m_ipText);

		//Stops possible streams

		m_pWndAmc->put_MediaUsername(_T("root"));
		m_pWndAmc->put_MediaPassword(_T("pass"));

		// Set the PTZ control properties
		m_pWndAmc->put_PTZControlURL(ctrlURL);
		m_pWndAmc->put_UIMode((CString)"ptz-absolute");

		// Enable PTZ-position presets from AMC context menu
		m_pWndAmc->put_PTZPresetURL(presetURL);

		// Enable joystick support
		m_pWndAmc->put_EnableJoystick(TRUE);

		// Enable area zoom
		m_pWndAmc->put_EnableAreaZoom(TRUE);

		// Enable one-click-zoom
		// m_pWndAmc->put_OneClickZoom(TRUE);

		// Set overlay settings
		m_pWndAmc->put_EnableOverlays(TRUE);
		// 		m_pWndAmc->put_ClientOverlay(/*AMC_OVERLAY_CROSSHAIR |*/
		// 			AMC_OVERLAY_VECTOR |
		// 			AMC_OVERLAY_ZOOM);

		// Show the status bar and the tool bar in the AXIS Media Control
		// 		m_pWndAmc->put_ShowStatusBar(true);
		// 		m_pWndAmc->put_ShowToolbar(true);
		m_pWndAmc->put_StretchToFit(true);
		m_pWndAmc->put_EnableContextMenu(true);
		// 		m_pWndAmc->put_ToolbarConfiguration((CString)"default,-mute,-volume,+ptz");

		// Set the media URL and the media type
		m_pWndAmc->put_MediaURL(mediaURL);
		m_pWndAmc->Play();

	}
	catch (COleDispatchException *e)
	{      
		MessageBox(e->m_strDescription);
	}

}

void CTAxisMediaCtrlDlg::OnBnClickedButtonDisconnect()
{
	if (m_pWndAmc)
	{
		m_pWndAmc->Stop();
		delete m_pWndAmc;
	}
}

void CTAxisMediaCtrlDlg::OnBnClickedButtonGetstatus()
{
	CHAR * pStatusArray[] = 
	{
		AMC_STATUS_INITIALIZED",			//AMC is initialized and ready. 
		"AMC_STATUS_FLAG_PLAYING",			//A media stream/file is playing. 
		"AMC_STATUS_FLAG_PAUSED",			//Playing of a media stream/file is paused. 
		"AMC_STATUS_FLAG_RECORDING",		//A media stream/file is being recorded. 
		"AMC_STATUS_FLAG_OPENING",			//AMC is opening a media stream/file. 
		"AMC_STATUS_FLAG_RECONNECTING",		//AMC is performing reconnection attempts. 
		"AMC_STATUS_FLAG_ISSUING_PTZ_COMMAND", //Issuing a Pan, Tilt, Zoom command to the device. 
		"AMC_STATUS_FLAG_EXTENDED_TEXT",	//An extended text/message is displayed in the status bar. 
		"AMC_STATUS_FLAG_PTZ_UIMODE_ABS",	//UIMode is set to ptz-absolute. 
		"AMC_STATUS_FLAG_PTZ_UIMODE_REL",	//UIMode is set to ptz-relative or ptz-relative-no-cross. 
		"AMC_STATUS_FLAG_OPENING_RECEIVE_AUDIO", //The stream for receiveing audio is being opened. 
		"AMC_STATUS_FLAG_OPENING_TRANSMIT_AUDIO", //AMC is opening the stream for transmitting audio. 
		"AMC_STATUS_FLAG_RECEIVE_AUDIO",	//Receiving audio. 
		"AMC_STATUS_FLAG_TRANSMIT_AUDIO ",	//Transmitting audio. 
		"AMC_STATUS_FLAG_TRANSMIT_AUDIO_FILE", //An audio file is being transmitted. 
		"AMC_STATUS_FLAG_RECORDING_AUDIO",	//Recording audio. 
	};
	long nStatusArray[] = 
	{
		1 ,
		2, 
		4,
		8,
		16, 
		32,
		512, 
		1024, 
		2048,
		4096 , 
		65536 ,
		131072 ,
		262144 ,
		524288,
		1048576, 
		2097152 
	};

	if (m_pWndAmc)
	{
		LONG nStatrus = m_pWndAmc->get_Status();
		for (int i = 0;i < sizeof(nStatusArray)/sizeof(long);i ++)
		{
			if ((nStatrus & nStatusArray[i]) == nStatusArray[i])
				TraceMsgA("Status = %s.\n",pStatusArray[i]);
		}
	}

}
