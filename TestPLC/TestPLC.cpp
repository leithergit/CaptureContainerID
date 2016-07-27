// TestPLC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include "utility.h"
#include <boost\smart_ptr.hpp>
#include <list>
#include <map>
#include <process.h>
#include <Windows.h>
using namespace boost;
using namespace std;
/*
标识         命令   块号   偏移   序号 数据长度
FA 01 00 00  00 01  01 F5  00 00  28 25 00 00 00 BE

*/
struct PLCHeader
{
	PLCHeader()
	{
		ZeroMemory(this,sizeof(PLCHeader));
		nIdentify	= 0x01FA;
		nCommand	= 0x0100;
	}
	int		nIdentify;
	short	nCommand;
	short	nBlock;
	short	nOffset;
	short   nSerialNo;
	int		nLength;
};
struct StreamCounter
{
	StreamCounter()
	{
		ZeroMemory(this,sizeof(StreamCounter));
	}
	int nRecv;
	int nSend;
};
typedef shared_ptr<StreamCounter> StreamCounterPtr;

struct PLCClient
{
	PLCClient(char *szBuffer,int nLength,sockaddr_in *sockClient)
	{
		ZeroMemory(this,sizeof(PLCClient));
		memcpy(&plc,szBuffer,nLength);
		memcpy(this->szBuffer,szBuffer,nLength);
		memcpy(&ClientAddr,sockClient,sizeof(sockaddr_in));
	}
	PLCHeader	plc;
	char szBuffer[256];
	int nBufferLength;
	sockaddr_in ClientAddr;
	StreamCounterPtr pCounter;
};
typedef shared_ptr<PLCClient> PLCClientPtr;
CRITICAL_SECTION  g_csListClient;
list<PLCClientPtr> g_listClient;
volatile bool g_bSendThreadRun = false;
HANDLE g_hSendThread = NULL;
SOCKET sServer = INVALID_SOCKET; 
#define _PLC_IDENTIFY 0x01FA

UINT __stdcall SendThread(void *p)
{

	char szBuffer[256] = {0};
	int nRecvPackages = 1;
	SOCKET s = socket(AF_INET,SOCK_DGRAM,0); 
	char szCountRecv[16] = {0};
	char szCountSent[16] = {0};
	list<PLCClientPtr>listClient;
	while(g_bSendThreadRun)
	{
		EnterCriticalSection(&g_csListClient);
		if (g_listClient.size() > 0)
			listClient.swap(g_listClient);
		LeaveCriticalSection(&g_csListClient);
		if (listClient.size() == 0)
		{
			Sleep(10);
		}

		for (list<PLCClientPtr>::iterator it = listClient.begin();it != listClient.end();it ++)
		{
			PLCClientPtr pClient = *it;
			int &nRecv = pClient->pCounter->nRecv;
			int &nSend = pClient->pCounter->nSend;
			_itoa(nRecv,szCountRecv,10);
			_itoa(nSend,szCountSent,10);
			strcat(pClient->szBuffer,"_");
			strcat(pClient->szBuffer,szCountRecv);
			strcat(pClient->szBuffer,"_");
			strcat(pClient->szBuffer,szCountSent);
			int nResult = sendto(sServer,pClient->szBuffer,strlen(pClient->szBuffer),0,(SOCKADDR *)&pClient->ClientAddr,sizeof(pClient->ClientAddr));
			if (nResult > 0)
			{
				nSend ++;
				continue;
			}
			else if (!nResult)
			break;
			else if (nResult == SOCKET_ERROR)
			{
				DWORD dwError = GetLastError();
				bool bSocketFailed = false;
				switch(dwError)
				{
				case WSAEWOULDBLOCK:
				case WSAEINPROGRESS:
					continue;
				case WSAEMSGSIZE:
					break;
				default:
					bSocketFailed = true;
					break;
				}
				if (bSocketFailed)
					break;
			}
		}
		listClient.clear();

		Sleep(10);
	}
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	InitializeCriticalSection(&g_csListClient);
	
	//InitWinsock(); 
	WSADATA wsaData; 
	if (WSAStartup(MAKEWORD(2,0),&wsaData)) //调用Windows Sockets DLL 
	{ 
		printf("Winsock无法初始化!\n"); 
		WSACleanup(); 
		return 0; 
	} 
	PLCHeader plcHeader;
	printf("服务器开始创建SOCKET。\n"); 
	struct sockaddr_in local; 
	struct sockaddr_in from; 
	int fromlen =sizeof(from); 
	local.sin_family=AF_INET; 
	local.sin_port=htons(5085); ///监听端口 
	local.sin_addr.s_addr=INADDR_ANY; ///本机 
	sServer=socket(AF_INET,SOCK_DGRAM,0); 
	bind(sServer,(struct sockaddr*)&local,sizeof(local)); 
	g_bSendThreadRun = true;
	int nResult = 0;
	char buffer[1024]="\0";
	char szHexText[1024];	
	map<UINT64,StreamCounterPtr>ClientMap;
	UINT nThreadAddr = 0;
	g_hSendThread = (HANDLE)_beginthreadex(NULL,0,SendThread,0,0,&nThreadAddr);
	short nInput = 20;
	nInput = ntohs(nInput);
	
	while (1) 
	{ 
		ZeroMemory(buffer,1024);
		if ((nResult = recvfrom(sServer,buffer,sizeof(buffer),0,(struct sockaddr*)&from,&fromlen))!=SOCKET_ERROR) 
		{ 
			ZeroMemory(szHexText,1024);
// 			Hex2AscStringA((unsigned char *)buffer,nResult,szHexText,1024);
// 			printf(szHexText);
// 			printf("\n");
			PLCHeader *pPlcHeader = (PLCHeader *)buffer;
			if (pPlcHeader->nIdentify != _PLC_IDENTIFY)
				continue;

			int nDataLength = ntohl(pPlcHeader->nLength);
			if ( nDataLength <= 0 )
				continue;

			int nOffset = ntohs(pPlcHeader->nOffset);
			if (nOffset < 0)
				continue;
			byte *pData  = (byte *)&buffer[sizeof(PLCHeader)];
			WORD nHoist = 0;
			if (nOffset < 70)
			{
				memcpy(&nHoist,&pData[70 - nOffset],2);	
				nHoist = ntohs(nHoist);
			}
			int nBlock = ntohs(pPlcHeader->nBlock);
			//if (nBlock == 501)
			{
				printf("BlockID = %d\tDataLength = %d.\tOffset = %d\tHoistHeight = %d\n",ntohs(pPlcHeader->nBlock),nDataLength,nOffset,nHoist);
				//Hex2AscStringA((unsigned char *)buffer,nResult,szHexText,1024);
				//printf(szHexText);
				printf("\n");
			}
			
// 			UINT64 nClientID = MAKEUINT64((ULONG)from.sin_addr.s_addr,from.sin_port);
// 			map<UINT64,StreamCounterPtr>::iterator itFind =  ClientMap.find(nClientID);
// 			PLCClientPtr pClient = make_shared<PLCClient>((char *)buffer,nResult,(sockaddr_in *)&from);
// 			if (itFind != ClientMap.end())
// 			{
// 				itFind->second->nRecv ++;
// 				pClient->pCounter = itFind->second;
// 			}
// 			else
// 			{
// 				pClient->pCounter = make_shared<StreamCounter>();
// 				ClientMap.insert(pair<UINT64,StreamCounterPtr>(nClientID,pClient->pCounter));
// 			}
// 			g_listClient.push_back(pClient);
		} 
		Sleep(10); 
	} 
	closesocket(sServer);
	WSACleanup();    
	return 0;
}

