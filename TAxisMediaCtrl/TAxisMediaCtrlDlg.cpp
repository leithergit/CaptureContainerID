
// TAxisMediaCtrlDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CTAxisMediaCtrlDlg �Ի���




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


// CTAxisMediaCtrlDlg ��Ϣ�������

BOOL CTAxisMediaCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	SetDlgItemText(IDC_IPADDRESS1,_T("192.168.0.90"));
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTAxisMediaCtrlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
			MessageBox(_T("����AXIS�ؼ�����ʧ��."));
			return ;
		}

		CRect rtFrame;
		GetDlgItem(IDC_STATIC_FRAME)->GetWindowRect(&rtFrame);
		ScreenToClient(&rtFrame);
		if (!m_pWndAmc->Create(_T("AsixMediaCtrl"),WS_VISIBLE|WS_CHILD,rtFrame,this,1024))
		{
			MessageBox(_T("����AXIS�ؼ�ʧ��."));
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
