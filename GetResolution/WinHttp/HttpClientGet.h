#pragma once
#include "HttpClient.h"

class CHttpClientGet:public CHttpClient
{
public:
    CHttpClientGet(void);
    ~CHttpClientGet(void);
public:
    BOOL TransDataToServer(const std::wstring& wstrUrl, DWORD dwTimeout, 
        VecStParam& vecParam, LPVOID lpData = NULL, DWORD dwDataLenInBytes = 0);
private:
    DWORD GetDataSize();
    BOOL GetData(LPVOID lpBuffer, DWORD dwBufferSize, DWORD& dwWrite);
    std::wstring GenerateExtInfo(const VecStParam& VecExtInfo);
    BOOL ModifyRequestHeader(HINTERNET hRequest);
    VOID AddExtInfo(VecStParam& VecExtInfo);
private:
    LPVOID m_lpData;
    DWORD m_dwDataSize;
    DWORD m_dwWriteIndex;
    VecStParam m_vecParam;
};

