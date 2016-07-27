
// AxisStreamRateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AxisStreamRate.h"
#include "AxisStreamRateDlg.h"
#include "Utility.h"

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


// CAxisStreamRateDlg 对话框




CAxisStreamRateDlg::CAxisStreamRateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAxisStreamRateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pCamera = NULL;
}

void CAxisStreamRateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAxisStreamRateDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_START, &CAxisStreamRateDlg::OnBnClickedButtonStart)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CAxisStreamRateDlg 消息处理程序

BOOL CAxisStreamRateDlg::OnInitDialog()
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

	SetDlgItemText(IDC_IPADDRESS,_T("192.168.1.30"));
	SetDlgItemText(IDC_EDIT_ACCOUNT,_T("root"));
	SetDlgItemText(IDC_EDIT_PASSWORD,_T("pass"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAxisStreamRateDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAxisStreamRateDlg::OnPaint()
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
		RECT rtFrame;
		GetDlgItemRect(IDC_STATIC_FRAME,rtFrame);
		
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAxisStreamRateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAxisStreamRateDlg::OnBnClickedButtonStart()
{
	CWaitCursor Wait;
	if (!m_pCamera)
	{
		TCHAR szCameraIP[32] = {0};
		TCHAR szAccount[32] = {0};
		TCHAR szPassword[32] = {0};
		GetDlgItemText(IDC_IPADDRESS,szCameraIP,32);
		GetDlgItemText(IDC_EDIT_ACCOUNT,szAccount,32);
		GetDlgItemText(IDC_EDIT_PASSWORD,szPassword,32);

		if (!IsValidIPAddress(szCameraIP))
		{
			AfxMessageBox(_T("请输入有效的IP地址."),MB_ICONSTOP);
			return ;
		}
		if (IsEmptyString(szAccount) || IsEmptyString(szPassword))
		{
			AfxMessageBox(_T("请输入有效的用户名和密码."),MB_ICONSTOP);
			return ;
		}
		HWND hVideoFrame = GetDlgItem(IDC_STATIC_FRAME)->GetSafeHwnd();
		m_pCamera = new CCameraProperty(hVideoFrame,szCameraIP,szAccount,szPassword);
		m_pCamera->EnableIPCPlay(true);		
		m_pCamera->StartPlay();
		SetDlgItemText(IDC_BUTTON_START,_T("停止测试"));
		SetTimer(ID_TIMER,50,NULL);
	}
	else
	{
		m_pCamera->Stop(true);
		delete m_pCamera;
		m_pCamera = NULL;
		SetDlgItemText(IDC_BUTTON_START,_T("开始测试"));
		KillTimer(ID_TIMER);
	}
}

void CAxisStreamRateDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == ID_TIMER)
	{
		if (m_pCamera)
		{
			int nCodeStreamRate = m_pCamera->pParserEvent->GetStreamRate();
			int nFPS =  m_pCamera->pParserEvent->GetFPS();
			TCHAR szText[64] = {0};
			_stprintf(szText,_T("%d Kbit/s   FPS:%d"),nCodeStreamRate,nFPS);
			SetDlgItemText(IDC_EDIT_STREAMRATE,szText);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CAxisStreamRateDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_pCamera)
	{
		m_pCamera->Stop(true);
		delete m_pCamera;
		m_pCamera = NULL;		
		KillTimer(ID_TIMER);
	}
}
