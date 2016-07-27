#pragma once
#include "AxisMediaParser.h"
#include "AxisMediaViewer.h"
#include "AxisMediaParserEvents.h"
#include "AxisMediaViewerEvents.h"
#include "..\IPCPlaySDK\Media.h"
#include "..\IPCPlaySDK\IPCPlaySDK.h"
#include "Utility.h"
#include <afxctl.h>
#pragma comment(lib,"..\\ipcplaysdk\\ipcplaysdk.lib")

#ifndef RectWidth
#define RectWidth(rt) (rt.right - rt.left)
#endif

#ifndef RectHeight
#define RectHeight(rt) (rt.bottom - rt.top)
#endif

struct CCameraProperty
{
private:
	
public:
	HWND hParent;
	TCHAR szCameraIP[32];
	TCHAR szAccount[16];
	TCHAR szPassword[16];
	WORD nPort;
	bool  bEnableHWAccel;
	bool  bFitWindow;
	bool  bForceIFrame;
	CAxisMediaViewer* pAxisViewer;
	CAxisMediaParser* pAxisParser;
	CAxisMediaParserEvents* pParserEvent;
	CAxisMediaViewerEvents* pViewerEvent;
	HANDLE hThreadConnect;
	DWORD dwParserCallbackCookie;
	DWORD dwViewerCallbackCookie;
	IPC_PLAYHANDLE	hIPCPlayer;
	
	bool bPlayerViaAxis;		// 是否使用安讯士SDK播放
	UINT	nPLCNode;
	CCameraProperty()
	{
		ZeroMemory(this,sizeof(CCameraProperty));
	};
	CCameraProperty(HWND hParent,LPCTSTR szIP,LPCTSTR szAccount,LPCTSTR szPWD,WORD nPort = 0)
	{
		ZeroMemory(this,sizeof(CCameraProperty));
#ifdef _DEBUG
		_tcscpy_s(szCameraIP,32,szIP);
		_tcscpy_s(this->szAccount,16,szAccount);
		_tcscpy_s(szPassword,16,szPWD);
#else
		_tcscpy(szCameraIP,szIP);
		_tcscpy(this->szAccount,szAccount);
		_tcscpy(szPassword,szPWD);
#endif

		this->hParent = hParent;
		bPlayerViaAxis = true;
		/*CreateAxisCtrl();*/
	}

	~CCameraProperty()
	{
		Stop();
		DestroyPlayer();
	}
	void EnableHWAccel(bool bEnable = true)
	{
		bEnableHWAccel = bEnable;
	}
	void EnableFitWindow(bool bEnable = true)
	{
		bFitWindow = bEnable;
	}

	void SetForceIFrame(bool bEnalbe = true)
	{
		bForceIFrame = bEnalbe;
	}
	bool GetForceIFrame()
	{
		return bForceIFrame;
	}
	bool CreatePlayer(bool bViewer = true,int nWidth = 0,int nHeight = 0)
	{
		if (!pAxisParser)
			pAxisParser = new CAxisMediaParser();
		if (!pAxisParser)
		{
			TraceMsgW(L"%s Failed to new a CAxisMediaParser Object.\n",__FUNCTION__);
			goto Failed;
		}
		if (!pAxisParser->m_lpDispatch)
		{
			if (!pAxisParser->CreateDispatch(L"AxisMediaParserLib.AxisMediaParser"))
			{
				TraceMsgW(L"%s Failed to create dispatch for AxisMediaParser!",__FUNCTION__);
				goto Failed;
			}
		}

		if (!pParserEvent)
			pParserEvent = new CAxisMediaParserEvents();
		if (!pParserEvent)
		{
			TraceMsgW(L"%s Failed to new a CAxisMediaParserEvents Object.\n",__FUNCTION__);
			goto Failed;
		}
		// Get a pointer to IUnknown for our event handler class
		IUnknown* pUnkCallback = pParserEvent->GetIDispatch(false);
		// Establish a connection between parser and event handlers
		AfxConnectionAdvise(pAxisParser->m_lpDispatch, __uuidof(IAxisMediaParserEvents), pUnkCallback, false, &dwParserCallbackCookie);

		if (!bViewer)
			goto Succeed;

		if (bPlayerViaAxis)
		{
			if (!pAxisViewer)
				pAxisViewer = new CAxisMediaViewer();
			if (!pAxisViewer)
			{
				TraceMsgW(L"%s Failed to new a CAxisMediaViewer Object.\n",__FUNCTION__);
				goto Failed;
			}
			// Create the AXIS Media Viewer COM object
			if (!pAxisViewer->m_lpDispatch)
			{
				if (!pAxisViewer->CreateDispatch(L"AxisMediaViewerLib.AxisMediaViewer"))
				{
					TraceMsgW(L"Failed to create dispatch for AxisMediaViewer!",__FUNCTION__);
					goto Failed;
				}
			}
			if (!pViewerEvent)
				pViewerEvent = new CAxisMediaViewerEvents();
			if (!pViewerEvent)
			{
				TraceMsgW(L"Failed to new a CAxisMediaViewerEvents object!",__FUNCTION__);
				goto Failed;
			}
			pUnkCallback = pViewerEvent->GetIDispatch(false);

			// Establish a connection between viewer and event handlers
			AfxConnectionAdvise(pAxisViewer->m_lpDispatch, __uuidof(IAxisMediaViewerEvents), pUnkCallback, false, &dwViewerCallbackCookie);
		}
		else
		{
			if (!hIPCPlayer)
			{
				
				if (!nWidth || !nHeight)
				{
					hIPCPlayer = ipcplay_OpenStream(hParent,NULL,0);
				}
				else
				{
					IPC_MEDIAINFO MediaHeader;
					MediaHeader.nVideoCodec = CODEC_H264;
					MediaHeader.nAudioCodec = CODEC_G711A;
					MediaHeader.nVideoWidth = nWidth;
					MediaHeader.nVideoHeight = nHeight;
					hIPCPlayer = ipcplay_OpenStream(hParent,(byte *)&MediaHeader,sizeof(IPC_MEDIAINFO));
				}
				
				if (!hIPCPlayer)
					goto Failed;
			}
		}

		goto Succeed;
Failed:
		if (pParserEvent)
		{
			delete pParserEvent;
			pParserEvent = NULL;
		}
		if (pViewerEvent)
		{
			delete pViewerEvent;
			pViewerEvent = NULL;
		}
		if (pAxisParser)
		{
			delete pAxisParser;
			pAxisParser = NULL;
		}
		if (pAxisViewer)
		{
			delete pAxisViewer;
			pAxisViewer = NULL;
		}
		return false;
Succeed:

		return true;
	}
	void DestroyPlayer()
	{
		if (pParserEvent)
		{
			delete pParserEvent;
			pParserEvent = NULL;
		}
		if (pViewerEvent)
		{
			delete pViewerEvent;
			pViewerEvent = NULL;
		}

		if (pAxisViewer)
		{
			delete pAxisViewer;
			pAxisViewer = NULL;
		}
		if (pAxisParser)
		{
			delete pAxisParser;
			pAxisParser = NULL;
		}
		if (hIPCPlayer)
		{
			ipcplay_Close(hIPCPlayer);
			hIPCPlayer = NULL;
		}
	}

	inline bool IsPlaying()
	{
		if (!pAxisParser )
			return false;
		if (pAxisParser->GetStatus() & AMP_STATUS_RUNNING) 
		{
			if (bPlayerViaAxis)
			{
				if (!pAxisViewer)
					return false;
				if (pAxisViewer->GetStatus() & AMV_STATUS_RUNNING)
					return true;
			}
			else
				return ipcplay_IsPlaying(hIPCPlayer);
			return true;
		}
		else
			return false;
	}
	bool StartRecord()
	{
		if (!CreatePlayer(false))
		{
			assert(false);
			return false;
		}
		// 是否已经连接相机
		if (pAxisParser->GetStatus() & AMP_STATUS_RUNNING)
			return true;

		CString mediaURL;
		//"axrtsphttp://192.168.1.26/axis-media/media.amp?camera=1&videocodec=h264&resolution=800x600"
		mediaURL.Format(L"axrtsphttp://%s/axis-media/media.amp?camera=1&videocodec=h264",szCameraIP);

		pAxisParser->SetMediaURL(mediaURL);
		pAxisParser->SetNetworkTimeout(5000);	// 5000ms
		pAxisParser->SetMediaUsername(szAccount);
		pAxisParser->SetMediaPassword(szPassword);
		//m_parser->SetShowLoginDialog(TRUE);
		pAxisParser->SetNetworkSecurityFlags(AMP_ALLOW_UNTRUSTED_CERTIFICATE);

		LONG lConnectionCookie;
		VARIANT varMediaType;
		LONG lNumberOfStreams;
		CSize sizeVideo;

		try
		{
			pAxisParser->Connect(&lConnectionCookie, &lNumberOfStreams, &varMediaType);
			pAxisParser->SendRenewStream(0);
			// Initialize our viewer
			pAxisViewer->Init(0, varMediaType, (INT64)hParent);
			// Set this pointer to event handler classes (will be used to update dialog control variables)
			pParserEvent->SetParser(pAxisParser);
			pAxisParser->Start();

		}
		catch (COleDispatchException* e)
		{
			TraceMsgW(e->m_strDescription);
			e->Delete();
			assert(false);
			return false;
		}
		return true;
	}

	void EnableIPCPlay(bool bEnable)
	{
		if (bEnable)
			bPlayerViaAxis = false;
		else
			bPlayerViaAxis = true;
	}
	void SetVideoWnd(HWND hWnd)
	{
		assert(hWnd != NULL);
		hParent = hWnd;
	}
	HWND GetVideoWnd()
	{
		return hParent;
	}
	bool Connect(CString mediaURL,bool bForceIFrame,VARIANT &varMediaType)
	{
		LONG lConnectionCookie;
		
		LONG lNumberOfStreams;
		try
		{
			if (!(pAxisParser->GetStatus() & AMP_STATUS_RUNNING))
			{
				pAxisParser->SetMediaURL(mediaURL);
				pAxisParser->SetNetworkTimeout(5000);	// 5000ms
				pAxisParser->SetMediaUsername(szAccount);
				pAxisParser->SetMediaPassword(szPassword);
				//m_parser->SetShowLoginDialog(TRUE);
				pAxisParser->SetNetworkSecurityFlags(AMP_ALLOW_UNTRUSTED_CERTIFICATE);
				// Connect to our Axis camera
				pAxisParser->Connect(&lConnectionCookie, &lNumberOfStreams, &varMediaType);
				if (bForceIFrame)
				{
					pAxisParser->SendRenewStream(0);
				}
				pParserEvent->SetParser(pAxisParser);
				if (!bPlayerViaAxis)
					pParserEvent->SetIPCCallBack(StreamCallBack,this);
				pAxisParser->Start();
			}
			return true;
		}
		catch (COleDispatchException* e)
		{
			TraceMsgW(L"%s A Exception occured:%s",__FUNCTIONW__,e->m_strDescription);
			e->Delete();
			//assert(false);
			return false;
		}

	}
	bool StartPlay(WORD nVideoWidth = 0,WORD nVideoHeight = 0)
	{
		if (!CreatePlayer(true,nVideoWidth,nVideoHeight))
		{
			assert(false);
			return false;
		}

		CString ctrlURL, presetURL, mediaURL;
		//"axrtsphttp://192.168.1.26/axis-media/media.amp?camera=1&videocodec=h264&resolution=800x600"
		if (!nVideoWidth || !nVideoHeight)
			mediaURL.Format(L"axrtsphttp://%s/axis-media/media.amp?camera=1&videocodec=h264",szCameraIP);
		else
			mediaURL.Format(L"axrtsphttp://%s/axis-media/media.amp?camera=1&videocodec=h264&resolution=%dx%d",szCameraIP,nVideoWidth,nVideoHeight);

		VARIANT varMediaType;
		CSize sizeVideo;
		try
		{
			if (!bPlayerViaAxis)
			{
				if (hIPCPlayer)
				{
					ipcplay_SetMaxFrameSize(hIPCPlayer,1024*512);
					ipcplay_Start(hIPCPlayer,false,bFitWindow,bEnableHWAccel);
				}
				else
					return false;
			}
			int nRetryCount = 0;
			bool bSucceed = false;
			while(nRetryCount < 3)
			{
				if (Connect(mediaURL,bForceIFrame,varMediaType))
				{
					bSucceed = true;
					break;
				}
				else
					nRetryCount ++;
			}
			if (!bSucceed)
				return false;
			if (bPlayerViaAxis)
			{
				if (!(pAxisViewer->GetStatus() & AMV_STATUS_RUNNING))
				{
					//pAxisViewer->SetVideoRenderer(AMV_VIDEO_RENDERER_EVR);
					pAxisViewer->SetColorSpace(AMV_CS_RGB32);
					pAxisViewer->SetLiveMode(TRUE);

					// Initialize our viewer
					// pAxisViewer->SetH264EnableHWDecoding(bEnableHWAccel);
					pAxisViewer->Init(0, varMediaType, (INT64)hParent);
					TraceMsgW(L"%s pAxisView Attach hWnd:%08X.\n",__FUNCTIONW__,hParent);
					pParserEvent->SetViewer(pAxisViewer);
					// Get video size in pixels from our viewer
					pAxisViewer->GetVideoSize(&sizeVideo.cx, &sizeVideo.cy);
					if (sizeVideo.cx != nVideoWidth || sizeVideo.cy != nVideoHeight)
					{
						TraceMsgW(L"************************************************************************\n");
						TraceMsgW(L"*   Warning:The video size(%d,%d) is defferent from requested(%d,%d)       *\n",sizeVideo.cx,sizeVideo.cy,nVideoWidth,nVideoHeight);
						TraceMsgW(L"************************************************************************\n");
					}

					RECT rtWnd;
					::GetWindowRect(hParent,&rtWnd);
					pAxisViewer->SetVideoPosition(0, 0, RectWidth(rtWnd), RectHeight(rtWnd));
					pAxisViewer->Start();
				}
			}
		}
		catch (COleDispatchException* e)
		{
			TraceMsgW(L"%s A Exception occured:%s",__FUNCTIONW__,e->m_strDescription);
			e->Delete();
			assert(false);
			return false;
		}

		return true;
	}
	bool Stop(bool bRefresh = true)
	{
		try
		{
			// Stop parser and viewer
			if (!pAxisParser)
				return false;
			if((pAxisParser->GetStatus() & AMP_STATUS_RUNNING))
				pAxisParser->Stop();

			if (bPlayerViaAxis)
			{
				if (pAxisViewer && (pAxisViewer->GetStatus() & AMV_STATUS_RUNNING))
				{
					pAxisViewer->Stop();
					if (bRefresh)
						::InvalidateRect(hParent,nullptr,TRUE);
				}
			}
			else
			{
				if (hIPCPlayer)
				{
					ipcplay_Stop(hIPCPlayer);
					if (bRefresh)
						ipcplay_Refresh(hIPCPlayer);
				}
			}

		}
		catch (COleDispatchException* e)
		{
			TraceMsgW(L"%s COleDispatchException occured:%s.\n",__FUNCTION__,e->m_strDescription);
			assert(false);
			e->Delete();
		}

		// Get a pointer to IUnknown for our event handler class
		IUnknown* pUnkCallback = pParserEvent->GetIDispatch(false);

		// Terminate the connection between parser and event handlers
		if (pAxisParser->m_lpDispatch)
			AfxConnectionUnadvise(pAxisParser->m_lpDispatch, __uuidof(IAxisMediaParserEvents), pUnkCallback, false, dwParserCallbackCookie);

		// Terminate the connection between parser and event handlers
		if (bPlayerViaAxis &&
			pAxisViewer &&
			pAxisParser->m_lpDispatch  )
		{
			// Get a pointer to IUnknown for our event handler class
			pUnkCallback = pViewerEvent->GetIDispatch(false);
			AfxConnectionUnadvise(pAxisViewer->m_lpDispatch, __uuidof(IAxisMediaViewerEvents), pUnkCallback, false, dwViewerCallbackCookie);
		}
		return true;
	}

	void SnapShot(TCHAR *szFileName,SNAPSHOT_FORMAT nFormat = XIFF_BMP)
	{
		if (IsPlaying())
		{
			if (bPlayerViaAxis)
				pViewerEvent->SnapShot(szFileName);
			else
				ipcplay_SnapShot(hIPCPlayer,szFileName,nFormat);
		}
	}

	bool CompareCamera(TCHAR *szCameraIP)
	{
		return (_tcscmp(this->szCameraIP,szCameraIP) == 0 );
	}
	bool CompareWnd(HWND hParent)
	{
		return (this->hParent == hParent);
	}
	void SetCameraProperty(HWND hParent,TCHAR *szIP,TCHAR *szAccount,TCHAR *szPWD,WORD nPort = 0)
	{
#ifdef _DEBUG
		_tcscpy_s(szCameraIP,32,szIP);
		_tcscpy_s(this->szAccount,16,szAccount);
		_tcscpy_s(szPassword,16,szPWD);
#else
		_tcscpy(szCameraIP,szIP);
		_tcscpy(this->szAccount,szAccount);
		_tcscpy(szPassword,szPWD);
#endif
		this->nPort = nPort;
		this->hParent = hParent;
	}
	static void __stdcall StreamCallBack(LONG lSampleType,BYTE *pSteamInput,LONG nStreamLen, LONG lSampleFlags, UINT64 dw64StartTime, UINT64 dw64StopTime,void *pUserPtr)
	{
		CCameraProperty *pThis =  (CCameraProperty *)pUserPtr;
		
		int nFrameType = 0;
		TCHAR *szSampleType[] = {
			_T("AMP_VST_H264_VIDEO_CONFIG"),
			_T("AMP_VST_H264_VIDEO_IDR"),
			_T("AMP_VST_H264_VIDEO_NON_IDR"),
			_T("AMP_VST_H264_VIDEO_SEI")
		};

		switch(lSampleType)
		{
		case AMP_VST_H264_VIDEO_CONFIG:				// H264 video configuration header.   
			nFrameType = IPC_GOV_FRAME;
			break;
		case AMP_VST_H264_VIDEO_IDR:				// H264 video IDR-frame (I-frame).   
				nFrameType = IPC_I_FRAME;
			break;
		case AMP_VST_H264_VIDEO_NON_IDR:			// H264 video non IDR-frame (P-frame).   
				nFrameType = IPC_P_FRAME;
			break;
		case AMP_VST_H264_VIDEO_SEI:				// H.264 SEI header.  
			{
				TraceMsgW(_T("%s SammpleType = %s\tLength = %d.\n"),__FUNCTIONW__,szSampleType[lSampleType - AMP_VST_H264_VIDEO_CONFIG],nStreamLen);
				return ;
			}
			break;
		}
		//TraceMsgW(_T("%s SammpleType = %s\tLength = %d.\n"),__FUNCTIONW__,szSampleType[lSampleType - AMP_VST_H264_VIDEO_CONFIG],nStreamLen);
		ipcplay_InputIPCStream(pThis->hIPCPlayer, pSteamInput, nFrameType, nStreamLen, 0, dw64StartTime/10000);

	}

};
typedef shared_ptr<CCameraProperty> CCameraPropertyPtr;