#pragma once
#include "stdafx.h"
class CIPCManager
{
public:
	CIPCManager(void)
	{

	};
	virtual ~CIPCManager(void) = 0;
	
	virtual int ConnectIPC(LPCTSTR szIPCURL,WORD nPort) = 0;
	virtual int Disconnect() = 0;
	virtual int InputIPCStream(byte *szStream,int nStreamLength) = 0;
	virtual int InputStream(byte *szStream,int nStreamLength) = 0;
	virtual int Start() = 0;
	virtual int Stop() =0;
	
};
