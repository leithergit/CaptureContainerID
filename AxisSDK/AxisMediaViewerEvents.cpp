// Contains the event handlers that may be sent by the COM objects.

// Callback.cpp : implementation file
//

#include "stdafx.h"
#include "AxisMediaViewerEvents.h"
#include "Utility.h"

enum AMV_FUNCTION_ID
{
	AMV_OnDecodedImage = 100,
	AMV_OnDecodedImage32 = 101,
	AMV_OnMediaPosition = 102,
	AMV_OnMediaPosition32 = 103
};

enum AMV_COLOR_SPACE
{
	AMV_CS_NULL = 0,
	AMV_CS_RGB24 = 1,
	AMV_CS_YV12 = 2,
	AMV_CS_YUY2 = 3,
	AMV_CS_RGB32 = 4
};

struct __declspec(uuid("c98a99f6-6311-469f-861a-164916aa45bf"))
	/* dispinterface */ IAxisMediaViewerEvents;

// CAxisMediaViewerEvents

IMPLEMENT_DYNAMIC(CAxisMediaViewerEvents, CCmdTarget)


CAxisMediaViewerEvents::CAxisMediaViewerEvents()
{
  EnableAutomation();
  InitializeCriticalSection(&m_csSnapshot);
  m_bCaptureVideo = false;
}

CAxisMediaViewerEvents::~CAxisMediaViewerEvents()
{
	DeleteCriticalSection(&m_csSnapshot);
}


void CAxisMediaViewerEvents::OnFinalRelease()
{
  // When the last reference for an automation object is released
  // OnFinalRelease is called.  The base class will automatically
  // deletes the object.  Add additional cleanup required for your
  // object before calling the base class.

  CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAxisMediaViewerEvents, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CAxisMediaViewerEvents, CCmdTarget)
  DISP_FUNCTION_ID(CAxisMediaViewerEvents, "OnDecodedImage", AMV_OnDecodedImage, OnDecodedImage, VT_EMPTY, VTS_UI8 VTS_I2 VTS_VARIANT)
  DISP_FUNCTION_ID(CAxisMediaViewerEvents, "OnMediaPosition", AMV_OnMediaPosition, OnMediaPosition, VT_EMPTY, VTS_UI8)
END_DISPATCH_MAP()

// Note: we add support for IID_IEvents to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {7C883F81-78ED-41BE-976A-72B47C1F6639}
static const IID IID_IEvents =
{ 0x7C883F81, 0x78ED, 0x41BE, { 0x97, 0x6A, 0x72, 0xB4, 0x7C, 0x1F, 0x66, 0x39 } };

BEGIN_INTERFACE_MAP(CAxisMediaViewerEvents, CCmdTarget)
  INTERFACE_PART(CAxisMediaViewerEvents, IID_IEvents, Dispatch)
  INTERFACE_PART(CAxisMediaViewerEvents, __uuidof(IAxisMediaViewerEvents), Dispatch)
END_INTERFACE_MAP()


// CAxisMediaViewerEvents message handlers

void CAxisMediaViewerEvents::OnDecodedImage(UINT64 dw64StartTime, SHORT nColorSpace, VARIANT &SampleArray)
{
  // Only interested in RGB24 and RGB32
  if ((nColorSpace != AMV_CS_RGB24) && (nColorSpace != AMV_CS_RGB32))
    return;
  CAutoLock lock(&m_csSnapshot);
  if (m_bCaptureVideo)
  {
	  lock.Unlock();
    // Define local variables
    COleSafeArray SafeArray;
    void* pBuffer;
    DWORD dwBufferSize;
    HANDLE hFile;
    DWORD dwReturn;

    // Attach image buffer to our MFC helper class
    SafeArray.Attach(SampleArray);
    SafeArray.AccessData((void**) &pBuffer);
   dwBufferSize = SafeArray.GetOneDimSize();
// 	SYSTEMTIME systime;
// 	GetLocalTime(&systime);
// 	TCHAR szFileName[1024] = {0};
// 	TCHAR szAppPath[1024] = {0};
// 	GetAppPath(szAppPath,1024);
// 	
// 	_stprintf(szFileName,_T("%s\\SnapShot_%04d%02d%02d_%02d%02d%02d_%03d.bmp"),szAppPath,systime.wYear,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond,systime.wMilliseconds);
    hFile = CreateFile(m_szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    BITMAPFILEHEADER bfh = {
      'M' << 8 | 'B', // "BM"
      (sizeof BITMAPFILEHEADER) + dwBufferSize,
      0,
      0,
      (sizeof BITMAPFILEHEADER) + (sizeof BITMAPINFOHEADER)
    };

    // Write BMP file header and image data to file
    WriteFile(hFile, &bfh, sizeof BITMAPFILEHEADER, &dwReturn, NULL);
    WriteFile(hFile, pBuffer, dwBufferSize, &dwReturn, NULL);
    CloseHandle(hFile);

    // Release buffer from our MFC helper class
    SafeArray.UnaccessData();
    SampleArray = SafeArray.Detach();

    m_bCaptureVideo = false;
  }
}

void CAxisMediaViewerEvents::OnMediaPosition(LONGLONG CurrentPosition)
{
  //m_MediaTime = CurrentPosition / 10000; // Convert to milli-seconds
}
