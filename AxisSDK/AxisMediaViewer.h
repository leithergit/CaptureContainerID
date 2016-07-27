// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard
#pragma once

#ifdef WIN64
#import "AxisMediaViewerX64.dll" no_namespace
#else
#import "AxisMediaViewer.dll" no_namespace
#endif 

// CAxisMediaViewer wrapper class

class CAxisMediaViewer : public COleDispatchDriver
{
public:
  CAxisMediaViewer(){} // Calls COleDispatchDriver default constructor
  CAxisMediaViewer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
  CAxisMediaViewer(const CAxisMediaViewer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

  // Attributes
public:

  // Operations
public:


  // IAxisMediaViewer methods
public:
  void AddFilter(LPUNKNOWN pIFilter, LPCTSTR FilterName, long Flags)
  {
    static BYTE parms[] = VTS_UNKNOWN VTS_BSTR VTS_I4 ;
    InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pIFilter, FilterName, Flags);
  }
  void Init(long NumberOfStreams, VARIANT& MediaType, __int64 hWnd)
  {
    static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_I8 ;
    InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NumberOfStreams, &MediaType, hWnd);
  }
  void Init32(long NumberOfStreams, VARIANT& MediaType, long hWndLow, long hWndHigh)
  {
    static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_I4 VTS_I4 ;
    InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NumberOfStreams, &MediaType, hWndLow, hWndHigh);
  }
  void Start()
  {
    InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }
  void Stop()
  {
    InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }
  void RenderVideoSample(long SampleFlags, unsigned __int64 StartTime, unsigned __int64 StopTime, VARIANT& SampleArray)
  {
    static BYTE parms[] = VTS_I4 VTS_UI8 VTS_UI8 VTS_VARIANT ;
    InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SampleFlags, StartTime, StopTime, &SampleArray);
  }
  void RenderVideoSample32(long SampleFlags, long StartTimeLow, long StartTimeHigh, long StopTimeLow, long StopTimeHigh, VARIANT& SampleArray)
  {
    static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_VARIANT ;
    InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SampleFlags, StartTimeLow, StartTimeHigh, StopTimeLow, StopTimeHigh, &SampleArray);
  }
  void RenderAudioSample(long SampleFlags, unsigned __int64 StartTime, unsigned __int64 StopTime, VARIANT& SampleArray)
  {
    static BYTE parms[] = VTS_I4 VTS_UI8 VTS_UI8 VTS_VARIANT ;
    InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SampleFlags, StartTime, StopTime, &SampleArray);
  }
  void RenderAudioSample32(long SampleFlags, long StartTimeLow, long StartTimeHigh, long StopTimeLow, long StopTimeHigh, VARIANT& SampleArray)
  {
    static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_VARIANT ;
    InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SampleFlags, StartTimeLow, StartTimeHigh, StopTimeLow, StopTimeHigh, &SampleArray);
  }
  void GetVideoSize(long * pWidth, long * pHeight)
  {
    static BYTE parms[] = VTS_PI4 VTS_PI4 ;
    InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pWidth, pHeight);
  }
  void DisplayModeChanged()
  {
    InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }
  void RepaintVideo()
  {
    InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }
  void SetVideoPosition(long Left, long Top, long Right, long Bottom)
  {
    static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
    InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Left, Top, Right, Bottom);
  }
  void GetVideoCodec(long * pCodec)
  {
    static BYTE parms[] = VTS_PI4 ;
    InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pCodec);
  }
  void GetAudioCodec(long * pCodec)
  {
    static BYTE parms[] = VTS_PI4 ;
    InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pCodec);
  }
  void RenderMetadataSample(long SampleFlags, unsigned __int64 StartTime, unsigned __int64 StopTime, LPCTSTR MetaData)
  {
    static BYTE parms[] = VTS_I4 VTS_UI8 VTS_UI8 VTS_BSTR ;
    InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SampleFlags, StartTime, StopTime, MetaData);
  }
  void RenderMetadataSample32(long SampleFlags, long StartTimeLow, long StartTimeHigh, long StopTimeLow, long StopTimeHigh, LPCTSTR MetaData)
  {
    static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BSTR ;
    InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SampleFlags, StartTimeLow, StartTimeHigh, StopTimeLow, StopTimeHigh, MetaData);
  }
  void Pause()
  {
    InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
  }
  void FrameStep(long StepLength)
  {
    static BYTE parms[] = VTS_I4 ;
    InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, StepLength);
  }
  BOOL Flush()
  {
    BOOL result;
    InvokeHelper(0x1f, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  void SetStartTime(__int64 StartTime)
  {
    static BYTE parms[] = VTS_I8 ;
    InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms, StartTime);
  }
  void H264AssignHWDecodingAdaptor(long AdaptorID, long AdaptorIndex)
  {
    static BYTE parms[] = VTS_I4 VTS_I4 ;
    InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, parms, AdaptorID, AdaptorIndex);
  }

  // IAxisMediaViewer properties
public:
  BOOL GetVMR9()
  {
    BOOL result;
    GetProperty(0xe, VT_BOOL, (void*)&result);
    return result;
  }
  void SetVMR9(BOOL propVal)
  {
    SetProperty(0xe, VT_BOOL, propVal);
  }
  short GetColorSpace()
  {
    short result;
    GetProperty(0xf, VT_I2, (void*)&result);
    return result;
  }
  void SetColorSpace(short propVal)
  {
    SetProperty(0xf, VT_I2, propVal);
  }
  long GetPlayOptions()
  {
    long result;
    GetProperty(0x10, VT_I4, (void*)&result);
    return result;
  }
  void SetPlayOptions(long propVal)
  {
    SetProperty(0x10, VT_I4, propVal);
  }
  long GetVolume()
  {
    long result;
    GetProperty(0x11, VT_I4, (void*)&result);
    return result;
  }
  void SetVolume(long propVal)
  {
    SetProperty(0x11, VT_I4, propVal);
  }
  long GetStatus()
  {
    long result;
    GetProperty(0x12, VT_I4, (void*)&result);
    return result;
  }
  void SetStatus(long propVal)
  {
    SetProperty(0x12, VT_I4, propVal);
  }
  BOOL GetEnableOnDecodedImage()
  {
    BOOL result;
    GetProperty(0x15, VT_BOOL, (void*)&result);
    return result;
  }
  void SetEnableOnDecodedImage(BOOL propVal)
  {
    SetProperty(0x15, VT_BOOL, propVal);
  }
  long GetH264VideoDecodingMode()
  {
    long result;
    GetProperty(0x16, VT_I4, (void*)&result);
    return result;
  }
  void SetH264VideoDecodingMode(long propVal)
  {
    SetProperty(0x16, VT_I4, propVal);
  }
  long GetMPEG4VideoDecodingMode()
  {
    long result;
    GetProperty(0x17, VT_I4, (void*)&result);
    return result;
  }
  void SetMPEG4VideoDecodingMode(long propVal)
  {
    SetProperty(0x17, VT_I4, propVal);
  }
  double GetPlaybackRate()
  {
    double result;
    GetProperty(0x19, VT_R8, (void*)&result);
    return result;
  }
  void SetPlaybackRate(double propVal)
  {
    SetProperty(0x19, VT_R8, propVal);
  }
  BOOL GetLiveMode()
  {
    BOOL result;
    GetProperty(0x18, VT_BOOL, (void*)&result);
    return result;
  }
  void SetLiveMode(BOOL propVal)
  {
    SetProperty(0x18, VT_BOOL, propVal);
  }
  long GetMJPEGVideoDecodingMode()
  {
    long result;
    GetProperty(0x1a, VT_I4, (void*)&result);
    return result;
  }
  void SetMJPEGVideoDecodingMode(long propVal)
  {
    SetProperty(0x1a, VT_I4, propVal);
  }
  BOOL GetH264EnableHWDecoding()
  {
    BOOL result;
    GetProperty(0x21, VT_BOOL, (void*)&result);
    return result;
  }
  void SetH264EnableHWDecoding(BOOL propVal)
  {
    SetProperty(0x21, VT_BOOL, propVal);
  }
  long GetVideoRenderer()
  {
    long result;
    GetProperty(0x23, VT_I4, (void*)&result);
    return result;
  }
  void SetVideoRenderer(long propVal)
  {
    SetProperty(0x23, VT_I4, propVal);
  }

};
