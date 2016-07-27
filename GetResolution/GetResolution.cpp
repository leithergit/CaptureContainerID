// GetResolution.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <windows.h>
#include <WTypes.h>

#include ".\WinHttp\HttpClientGet.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//system("pause");	
	std::string strT = "1234556";
	int nSize = strT.size();
	int nLength = strT.length();
	URL_COMPONENTS uc = { 0};

	// rtsp://root:pass@192.168.1.26/axis-media/media.amp?camera=1&videocodec=h264
	TCHAR *szUrl = _T("http://root:pass@192.168.1.26/axis-cgi/admin/param.cgi?action=list&group=Properties.Image.Resolution");

	CHttpClientGet *pHttpClient = new CHttpClientGet();
	pHttpClient->TransmiteData(szUrl,eGet,60000,_T("root"),_T("pass"));
	CHAR szRecvBuff[4096] = {0};
	DWORD dwRecvLength = 4096;
	if (!pHttpClient->ReceiveData((LPBYTE)szRecvBuff,dwRecvLength))
	{
 		_tprintf(_T("Recv data faield.\n"));
		goto Failed;

	}
	
	printf("Recv buff = %s.\n",szRecvBuff);

Failed:
	system("pause");
	return 0;
}


