// Contains the event handlers that may be sent by the COM objects.

// Callback.cpp : implementation file
//

#include "stdafx.h"
#include "AxisMediaParserEvents.h"
#include "Utility.h"
#include "TimeUtility.h"

#define AMP_TICKS_TO_SEC(t) ((t)/10000000)
#define NTP_UNIX_EPOCH_OFFSET_SEC 2208988800 //((70*365 + 17)*86400)


// CAxisMediaParserEvents

IMPLEMENT_DYNAMIC(CAxisMediaParserEvents, CCmdTarget)


CAxisMediaParserEvents::CAxisMediaParserEvents()
{
  EnableAutomation();
  m_pAxisViewer = NULL;;
  m_pAxisParser = NULL;
  m_pStreamCallBack = NULL;
  m_pStreamInfo = new StreamInfo();
}

CAxisMediaParserEvents::~CAxisMediaParserEvents()
{
	delete m_pStreamInfo;
}


void CAxisMediaParserEvents::OnFinalRelease()
{
  // When the last reference for an automation object is released
  // OnFinalRelease is called.  The base class will automatically
  // deletes the object.  Add additional cleanup required for your
  // object before calling the base class.

  CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAxisMediaParserEvents, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CAxisMediaParserEvents, CCmdTarget)
  DISP_FUNCTION_ID(CAxisMediaParserEvents, "OnError", AMP_OnError, OnError, VT_EMPTY, VTS_I4)
  DISP_FUNCTION_ID(CAxisMediaParserEvents, "OnTriggerData", AMP_OnTriggerData, OnTriggerData, VT_EMPTY, VTS_I4 VTS_UI8 VTS_UI4 VTS_I2 VTS_UI4 VTS_I2 VTS_BOOL VTS_BSTR)
  DISP_FUNCTION_ID(CAxisMediaParserEvents, "OnVideoSample", AMP_OnVideoSample, OnVideoSample, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_UI8 VTS_UI8 VTS_VARIANT)
  DISP_FUNCTION_ID(CAxisMediaParserEvents, "OnAudioSample", AMP_OnAudioSample, OnAudioSample, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_UI8 VTS_UI8 VTS_VARIANT)
  DISP_FUNCTION_ID(CAxisMediaParserEvents, "OnMetaDataSample", AMP_OnMetaDataSample, OnMetaDataSample, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_UI8 VTS_UI8 VTS_BSTR)
END_DISPATCH_MAP()

// Note: we add support for IID_IEvents to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {2617B8F8-7181-4D3B-9421-54446C113075}
static const IID IID_IEvents =
{ 0x2617B8F8, 0x7181, 0x4D3B, { 0x94, 0x21, 0x54, 0x44, 0x6C, 0x11, 0x30, 0x75 } };

BEGIN_INTERFACE_MAP(CAxisMediaParserEvents, CCmdTarget)
  INTERFACE_PART(CAxisMediaParserEvents, IID_IEvents, Dispatch)
  INTERFACE_PART(CAxisMediaParserEvents, __uuidof(IAxisMediaParserEvents), Dispatch)
END_INTERFACE_MAP()


// CAxisMediaParserEvents message handlers

void CAxisMediaParserEvents::OnError(long ErrorCode)
{
  CString anErrorMsg;
 // anErrorMsg.Format(_T("CAxisMediaParserEvents::OnError() called with error 0x%X"), ErrorCode);
 // AfxMessageBox(anErrorMsg);
}

void CAxisMediaParserEvents::OnTriggerData(LONG lCookieID, UINT64 dw64StartTime, DWORD dwUserTime, SHORT UserTimeFract, DWORD dwUnitTime, SHORT UnitTimeFract, BOOL bUnitTimeInvalid, LPCTSTR lpszTriggerData)
{
//   // Set new trigger data to dialog control variable
//   m_pLiveViewDlg->m_strTriggerData = lpszTriggerData;
// 
//   // Update GUI-thread
//   PostMessage(m_pLiveViewDlg->GetSafeHwnd(), WM_MY_UPDATE, 0, 0);
}

void CAxisMediaParserEvents::OnVideoSample(LONG lCookieID, LONG lSampleType, LONG lSampleFlags, UINT64 dw64StartTime, UINT64 dw64StopTime, VARIANT &SampleArray)
{
	COleSafeArray SafeArray;
	DWORD dwBufferSize;
	void* pBuffer;
	DWORD dwReturn;
	try
	{
		// Setup our MFC helper class from incoming VARIANT and retrieve void* pointer
		SafeArray.Attach(SampleArray);
		SafeArray.AccessData(&pBuffer);
		dwBufferSize = SafeArray.GetOneDimSize();
		if (m_pStreamInfo)
			m_pStreamInfo->PushFrameInfo(dwBufferSize);

		// Release incoming VARIANT from our MFC helper class
		SafeArray.UnaccessData();
		SampleArray = SafeArray.Detach();
		// Let viewer render video sample

		// Add LiveTimeOffset to original timestamp for optimal latency when rendering
		LONGLONG renderStartTime = 0;
		LONGLONG renderStopTime = 1;
		ULONGLONG liveOffset = m_pAxisParser->GetLiveTimeOffset();
		if ((LONGLONG)dw64StartTime + liveOffset > 0)
		{
		  renderStartTime = (LONGLONG)dw64StartTime + liveOffset;
		  renderStopTime = (LONGLONG)dw64StopTime + liveOffset;
		}
		else
		{
			renderStartTime = dw64StartTime;
			renderStopTime = dw64StopTime;
		}
		
		if (m_pAxisViewer)
		{
			m_pAxisViewer->RenderVideoSample(lSampleFlags, renderStartTime, renderStopTime, SampleArray);
		}
		
		m_csCallback.Lock();
		if (m_pStreamCallBack)
			m_pStreamCallBack(lSampleType,(byte *)pBuffer,dwBufferSize,lSampleFlags,dw64StartTime,dw64StopTime,m_pUserPtr);
		m_csCallback.Unlock();
	}
	catch (COleDispatchException* e)
	{
		TraceMsgW(_T("%s There is a Excpetion:%s.\n"),__FUNCTION__,e->m_strDescription);
		e->Delete();
	}
}

void CAxisMediaParserEvents::OnAudioSample(LONG lCookieID, LONG lSampleType, LONG lSampleFlags, UINT64 dw64StartTime, UINT64 dw64StopTime, VARIANT &SampleArray)
{
	COleSafeArray SafeArray;
	DWORD dwBufferSize;
	void* pBuffer;
	DWORD dwReturn;
	try
	{
		SafeArray.Attach(SampleArray);
		SafeArray.AccessData(&pBuffer);
		dwBufferSize = SafeArray.GetOneDimSize();

		// Release incoming VARIANT from our MFC helper class
		SafeArray.UnaccessData();
		SampleArray = SafeArray.Detach();

		// Add LiveTimeOffset to original timestamp for optimal latency when rendering
		LONGLONG renderStartTime = 0;
		LONGLONG renderStopTime = 1;
		ULONGLONG liveOffset = m_pAxisParser->GetLiveTimeOffset();
		if ((LONGLONG)dw64StartTime + liveOffset > 0)
		{
		  renderStartTime = (LONGLONG)dw64StartTime + liveOffset;
		  renderStopTime = (LONGLONG)dw64StopTime + liveOffset;
		}
		else
		{
			renderStartTime = dw64StartTime;
			renderStopTime = dw64StopTime;
		}
		if (m_pAxisViewer)
		{
			m_pAxisViewer->RenderAudioSample(lSampleFlags, renderStartTime, renderStopTime, SampleArray);
		}
		//if (m_pStreamCallBack)
		//	m_pStreamCallBack(lSampleType,(byte *)pBuffer,dwBufferSize,lSampleFlags,dw64StartTime,dw64StopTime,m_pUserPtr);
	}
	catch (COleDispatchException* e)
	{
		TraceMsgW(_T("%s There is a Excpetion:%s.\n"),__FUNCTION__,e->m_strDescription);
		e->Delete();
	}
}

void CAxisMediaParserEvents::OnMetaDataSample(LONG lCookieID, LONG lSampleType, LONG lSampleFlags, UINT64 dw64StartTime, UINT64 dw64StopTime, LPCTSTR lpszMetaData)
{
  // Process metadata samples here

}
