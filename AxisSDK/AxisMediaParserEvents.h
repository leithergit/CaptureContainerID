// Contains the event handlers that may be sent by the COM objects.

#pragma once

// CAxisMediaParserEvents command target
#include "AxisMediaViewer.h"
#include "AxisMediaParser.h"
#include <assert.h>
#include <list>
#include "AutoLock.h"
#include "TimeUtility.h"
using namespace  std;
#define _Frame_PERIOD			10.0f		///< 一个帧率区间
struct FrameInfo
{
	time_t  tRecvTime;				///< 收到帧的时间 单位毫秒
	int		nFrameSize;
};

struct StreamInfo
{
	UINT	nFrameID;
	bool	bRecvIFrame;		///< 是否收到第个I帧
	int		nVideoFrameCount;
	int		nAudioFrameCount;
	__int64 nVideoBytes;		///< 收到视频总字节数
	__int64 nAudioBytes;		///< 收到音频总字节数
	time_t  tFirstTime;			///< 接收数据的起始时间	单位毫秒
	time_t	tLastTime;			///< 最后一次接收数据时间 单位毫秒
	CRITICAL_SECTION	csFrameList;
	list<FrameInfo>FrameInfoList;
	StreamInfo()
	{
		ZeroMemory(this, offsetof(StreamInfo, csFrameList));
		::InitializeCriticalSection(&csFrameList);
	}
	~StreamInfo()
	{
		::DeleteCriticalSection(&csFrameList);
	}

	int GetFrameRate()
	{
		CAutoLock lock(&csFrameList);
		if (FrameInfoList.size() > 2)
		{
			time_t tSpan = FrameInfoList.back().tRecvTime - FrameInfoList.front().tRecvTime;
			if (!tSpan)
				tSpan = 1000;
			int nSize = FrameInfoList.size();
			return nSize * 1000 / (int)(tSpan);
		}
		else
			return 1;
	}

	int	GetVideoCodeRate(int nUnit = 1024)	///< 取得视频码率(Kbps)
	{
		time_t tSpan = 25;
		CAutoLock lock(&csFrameList);
		if (FrameInfoList.size() > 1)
			tSpan = FrameInfoList.back().tRecvTime - FrameInfoList.front().tRecvTime;
		__int64 nTotalBytes = 0;

		for (list<FrameInfo>::iterator it = FrameInfoList.begin();
			it != FrameInfoList.end();
			it++)
			nTotalBytes += (*it).nFrameSize;

		if (tSpan == 0)
			tSpan = 1;
		__int64 nRate = (nTotalBytes  * 8*1000 / ((int)tSpan*nUnit));
		return (int)nRate;
	}
	int GetAudioCodeRate(int nUnit = 1024)
	{
		time_t tSpan = (tLastTime - tFirstTime);
		if (tSpan == 0)
			tSpan = 1;
		return (int)(nAudioBytes * 1000 / (tSpan*nUnit));
	}
	void inline Reset()
	{
		ZeroMemory(this, offsetof(StreamInfo, csFrameList));
		CAutoLock lock(&csFrameList);
		FrameInfoList.clear();
	}
	void PushFrameInfo(int nFrameSize)
	{
		time_t tNow = (time_t)(GetExactTime() * 1000);
		CAutoLock lock(&csFrameList);
		if (FrameInfoList.size() > 1)
		{
			for (list<FrameInfo>::iterator it = FrameInfoList.begin();
				it != FrameInfoList.end();)
			{
				if ((tNow - (*it).tRecvTime) > _Frame_PERIOD * 1000)
					it = FrameInfoList.erase(it);
				else
					it++;
			}
		}

		FrameInfo fi;
		fi.tRecvTime = tNow;
		fi.nFrameSize = nFrameSize;
		FrameInfoList.push_back(fi);
	}
};
typedef void(__stdcall *STDStreamCallBack)(LONG lSampleType,BYTE *pSteamInput,LONG nStreamLen, LONG lSampleFlags, UINT64 dw64StartTime, UINT64 dw64StopTime,void *pUserPtr);
class CAxisMediaParserEvents : public CCmdTarget
{
	 DECLARE_DYNAMIC(CAxisMediaParserEvents)

public:
	  CAxisMediaParserEvents();
	  virtual ~CAxisMediaParserEvents();

	  virtual void OnFinalRelease();
	  CAxisMediaViewer *m_pAxisViewer;
	  CAxisMediaParser *m_pAxisParser;
	  LONGLONG		m_MediaTime;
	  CTime			m_cameraTime;
	  CCriticalSection m_csCallback;

	 
	  STDStreamCallBack	m_pStreamCallBack;
	  void *		m_pUserPtr;
	  void SetParser(CAxisMediaParser *pParser)
	  {
		  assert(pParser != NULL);
		  m_pAxisParser = pParser;
	  }
	  void SetViewer(CAxisMediaViewer *pViewer)
	  {
		  assert(pViewer != NULL);
		  m_pAxisViewer = pViewer;
	  }

	  void SetIPCCallBack(void *pCallBack,void *pUserPtr)
	  {
		  m_csCallback.Lock();
		  m_pStreamCallBack = (STDStreamCallBack)pCallBack;
		  m_pUserPtr = pUserPtr;
		  m_csCallback.Unlock();
	  }
	
	  StreamInfo* m_pStreamInfo;
	  
	  int GetStreamRate()
	  {
		  if (m_pStreamInfo)
			return m_pStreamInfo->GetVideoCodeRate(1000);
		  else
			  return 0;
	  }

	  int GetFPS()
	  {
		  if (m_pStreamInfo)
			return m_pStreamInfo->GetFrameRate();
		  else
			  return 0;
	  }
	 
protected:
	  DECLARE_MESSAGE_MAP()
	  DECLARE_DISPATCH_MAP()
	  DECLARE_INTERFACE_MAP()


	  void OnError(long ErrorCode);
	  void OnTriggerData(LONG lCookieID, UINT64 dw64StartTime, DWORD dwUserTime, SHORT UserTimeFract, DWORD dwUnitTime, SHORT UnitTimeFract, BOOL bUnitTimeInvalid, LPCTSTR lpszTriggerData);
	  void OnVideoSample(LONG lCookieID, LONG lSampleType, LONG lSampleFlags, UINT64 dw64StartTime, UINT64 dw64StopTime, VARIANT &SampleArray);
	  void OnAudioSample(LONG lCookieID, LONG lSampleType, LONG lSampleFlags, UINT64 dw64StartTime, UINT64 dw64StopTime, VARIANT &SampleArray);
	  void OnMetaDataSample(LONG lCookieID, LONG lSampleType, LONG lSampleFlags, UINT64 dw64StartTime, UINT64 dw64StopTime, LPCTSTR lpszMetaData);
};
