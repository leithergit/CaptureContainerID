// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#pragma once
#ifdef WIN64
#import "AxisMediaParserX64.dll" no_namespace
#else
#import "AxisMediaParser.dll" no_namespace
#endif 
// CAxisMediaParser wrapper class

class CAxisMediaParser : public COleDispatchDriver
{
public:
  CAxisMediaParser(){} // Calls COleDispatchDriver default constructor
  CAxisMediaParser(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
  CAxisMediaParser(const CAxisMediaParser& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

  // Attributes
public:

  // Operations
public:


  // IAxisMediaParser methods
public:
  void Connect(long * pCookieID, long * pNumberOfStreams, VARIANT * pMediaType)
  {
    static BYTE parms[] = VTS_PI4 VTS_PI4 VTS_PVARIANT ;
    InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pCookieID, pNumberOfStreams, pMediaType);
  }
  void GetVideoSize(long * pWidth, long * pHeight)
  {
    static BYTE parms[] = VTS_PI4 VTS_PI4 ;
    InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pWidth, pHeight);
  }
  void Start()
  {
    InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }
  void Stop()
  {
    InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }
  void GetVideoCodec(long * pCodec)
  {
    static BYTE parms[] = VTS_PI4 ;
    InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pCodec);
  }
  void GetAudioCodec(long * pCodec)
  {
    static BYTE parms[] = VTS_PI4 ;
    InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pCodec);
  }
  void SendRenewStream(long dwFlags)
  {
    static BYTE parms[] = VTS_I4 ;
    InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwFlags);
  }
  void SetAdditionalMediaSource(LPCTSTR MediaURL, LPCTSTR MediaUsername, LPCTSTR MediaPassword, long Index, long reserved)
  {
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 ;
    InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MediaURL, MediaUsername, MediaPassword, Index, reserved);
  }
  void Disconnect()
  {
    InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }
  void SetProxy(LPCTSTR ProxyName, LPCTSTR Username, LPCTSTR Password)
  {
    static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
    InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ProxyName, Username, Password);
  }
  void SetPositions32(long theStartPositionLow, long theStartPositionHigh, long theStopPositionLow, long theStopPositionHigh)
  {
    static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
    InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, parms, theStartPositionLow, theStartPositionHigh, theStopPositionLow, theStopPositionHigh);
  }

  // IAxisMediaParser properties
public:
  CString GetMediaURL()
  {
    CString result;
    GetProperty(0x5, VT_BSTR, (void*)&result);
    return result;
  }
  void SetMediaURL(CString propVal)
  {
    SetProperty(0x5, VT_BSTR, propVal);
  }
  CString GetMediaUsername()
  {
    CString result;
    GetProperty(0x6, VT_BSTR, (void*)&result);
    return result;
  }
  void SetMediaUsername(CString propVal)
  {
    SetProperty(0x6, VT_BSTR, propVal);
  }
  CString GetMediaPassword()
  {
    CString result;
    GetProperty(0x7, VT_BSTR, (void*)&result);
    return result;
  }
  void SetMediaPassword(CString propVal)
  {
    SetProperty(0x7, VT_BSTR, propVal);
  }
  long GetNetworkTimeout()
  {
    long result;
    GetProperty(0x8, VT_I4, (void*)&result);
    return result;
  }
  void SetNetworkTimeout(long propVal)
  {
    SetProperty(0x8, VT_I4, propVal);
  }
  long GetStatus()
  {
    long result;
    GetProperty(0x9, VT_I4, (void*)&result);
    return result;
  }
  void SetStatus(long propVal)
  {
    SetProperty(0x9, VT_I4, propVal);
  }
  BOOL GetSynchronizeUsingRTCP()
  {
    BOOL result;
    GetProperty(0xd, VT_BOOL, (void*)&result);
    return result;
  }
  void SetSynchronizeUsingRTCP(BOOL propVal)
  {
    SetProperty(0xd, VT_BOOL, propVal);
  }
  CString GetTimeFormat()
  {
    CString result;
    GetProperty(0xe, VT_BSTR, (void*)&result);
    return result;
  }
  void SetTimeFormat(CString propVal)
  {
    SetProperty(0xe, VT_BSTR, propVal);
  }
  BOOL GetShowLoginDialog()
  {
    BOOL result;
    GetProperty(0x10, VT_BOOL, (void*)&result);
    return result;
  }
  void SetShowLoginDialog(BOOL propVal)
  {
    SetProperty(0x10, VT_BOOL, propVal);
  }
  BOOL GetShowUntrustedCertificateDialog()
  {
    BOOL result;
    GetProperty(0x11, VT_BOOL, (void*)&result);
    return result;
  }
  void SetShowUntrustedCertificateDialog(BOOL propVal)
  {
    SetProperty(0x11, VT_BOOL, propVal);
  }
  long GetNetworkSecurityFlags()
  {
    long result;
    GetProperty(0x13, VT_I4, (void*)&result);
    return result;
  }
  void SetNetworkSecurityFlags(long propVal)
  {
    SetProperty(0x13, VT_I4, propVal);
  }
  __int64 GetDuration()
  {
    __int64 result;
    GetProperty(0x15, VT_I8, (void*)&result);
    return result;
  }
  void SetDuration(__int64 propVal)
  {
    SetProperty(0x15, VT_EMPTY, propVal);
  }
  long GetDuration32()
  {
    long result;
    GetProperty(0x16, VT_I4, (void*)&result);
    return result;
  }
  void SetDuration32(long propVal)
  {
    SetProperty(0x16, VT_I4, propVal);
  }
  __int64 GetStartPosition()
  {
    __int64 result;
    GetProperty(0x18, VT_EMPTY, (void*)&result);
    return result;
  }
  void SetStartPosition(__int64 propVal)
  {
    SetProperty(0x18, VT_EMPTY, propVal);
  }
  __int64 GetStopPosition()
  {
    __int64 result;
    GetProperty(0x19, VT_EMPTY, (void*)&result);
    return result;
  }
  void SetStopPosition(__int64 propVal)
  {
    SetProperty(0x19, VT_EMPTY, propVal);
  }
  __int64 GetLiveTimeOffset()
  {
    __int64 result;
    GetProperty(0x1a, VT_I8, (void*)&result);
    return result;
  }
  void SetLiveTimeOffset(__int64 propVal)
  {
    SetProperty(0x1a, VT_EMPTY, propVal);
  }
  unsigned __int64 GetNTPTimeOffset()
  {
    unsigned __int64 result;
    GetProperty(0x1b, VT_UI8, (void*)&result);
    return result;
  }
  void SetNTPTimeOffset(unsigned __int64 propVal)
  {
    SetProperty(0x1b, VT_EMPTY, propVal);
  }

};
