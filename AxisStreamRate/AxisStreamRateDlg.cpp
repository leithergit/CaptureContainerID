
// AxisStreamRateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AxisStreamRate.h"
#include "AxisStreamRateDlg.h"
#include "Utility.h"

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


// CAxisStreamRateDlg �Ի���




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


// CAxisStreamRateDlg ��Ϣ�������

BOOL CAxisStreamRateDlg::OnInitDialog()
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

	SetDlgItemText(IDC_IPADDRESS,_T("192.168.1.30"));
	SetDlgItemText(IDC_EDIT_ACCOUNT,_T("root"));
	SetDlgItemText(IDC_EDIT_PASSWORD,_T("pass"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAxisStreamRateDlg::OnPaint()
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
		RECT rtFrame;
		GetDlgItemRect(IDC_STATIC_FRAME,rtFrame);
		
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
			AfxMessageBox(_T("��������Ч��IP��ַ."),MB_ICONSTOP);
			return ;
		}
		if (IsEmptyString(szAccount) || IsEmptyString(szPassword))
		{
			AfxMessageBox(_T("��������Ч���û���������."),MB_ICONSTOP);
			return ;
		}
		HWND hVideoFrame = GetDlgItem(IDC_STATIC_FRAME)->GetSafeHwnd();
		m_pCamera = new CCameraProperty(hVideoFrame,szCameraIP,szAccount,szPassword);
		m_pCamera->EnableIPCPlay(true);		
		m_pCamera->StartPlay();
		SetDlgItemText(IDC_BUTTON_START,_T("ֹͣ����"));
		SetTimer(ID_TIMER,50,NULL);
	}
	else
	{
		m_pCamera->Stop(true);
		delete m_pCamera;
		m_pCamera = NULL;
		SetDlgItemText(IDC_BUTTON_START,_T("��ʼ����"));
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
