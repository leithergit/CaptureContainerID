// Contains the event handlers that may be sent by the COM objects.

#pragma once
#include "AutoLock.h"

// CAxisMediaViewerEvents command target

class CAxisMediaViewerEvents : public CCmdTarget
{
  DECLARE_DYNAMIC(CAxisMediaViewerEvents)

public:
  CAxisMediaViewerEvents();
  virtual ~CAxisMediaViewerEvents();

  virtual void OnFinalRelease();
  TCHAR m_szFileName[1024];
  void SnapShot(TCHAR *szFileName)
  {
	  CAutoLock lock(&m_csSnapshot);
	  m_bCaptureVideo = true;
	  _tcscpy(m_szFileName,szFileName);
  }

protected:
  CRITICAL_SECTION m_csSnapshot;
  DECLARE_MESSAGE_MAP()
  DECLARE_DISPATCH_MAP()
  DECLARE_INTERFACE_MAP()

  void OnDecodedImage(UINT64 dw64StartTime, SHORT nColorSpace, VARIANT &SampleArray);
  void OnMediaPosition(LONGLONG CurrentPosition);
  volatile bool m_bCaptureVideo;
};
