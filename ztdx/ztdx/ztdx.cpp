// ztdx.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ztdx.h"
#include <iostream>
#include <stdlib.h>
#include <WinSock2.h>

#include "ccommon.h"

#pragma comment(lib,"ws2_32.lib")
//生成的dll及相关依赖dll请拷贝到通达信安装目录的T0002/dlls/下面,再在公式管理器进行绑定

bool isLogin = FALSE;

void TestPlugin1(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc)
{
	//for (int i = 0; i<DataLen; i++)
	//pfOUT[DataLen - 1] = pfINa[DataLen - 1] - pfINb[DataLen - 1];
	//pfOUT[DataLen - 1] = pfOUT[DataLen - 1] / 2;

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		//printf("WSAStartup failed with error: %d\n", err);
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		//printf("Could not find a usable version of Winsock.dll\n");
		//::MessageBox(NULL, TEXT("Winsock失败？"), TEXT("提示"), 1);
		WSACleanup();
		return ;
	}
	//else
	//	printf("The Winsock 2.2 dll was found okay\n");


	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, 0);

	if (clientSock == -1)
	{
		//perror("socket创建出错！");
	//	exit(1);
		//::MessageBox(NULL, TEXT("连接失败？"), TEXT("提示"), 1);
		return;
	}

	SOCKADDR_IN addrClient;
	addrClient.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(6000);


	int ret = connect(clientSock, (sockaddr*)&addrClient, sizeof(SOCKADDR));
	if (ret)
	{
		//printf("连接失败\n");
		//::MessageBox(NULL, TEXT("连接失败？"), TEXT("提示"), 1);
		return ;
	}
	//printf("client: clientSock: %d\n", clientSock);
	//::MessageBox(NULL, TEXT("clientSock ok"), TEXT("提示"), 1);

	TcpPkgInfo sTcpPkg;
	memset((char*)&sTcpPkg, 0x00, sizeof(TcpPkgInfo));

	int recvLen0 = recv(clientSock, (char*)&sTcpPkg, sizeof(TcpPkgInfo), 0);
	if (recvLen0 > 0)
	{
		if (sTcpPkg.status == Z_OK)
		{
			isLogin = true;
			//printf("Login Success !\n");
			pfOUT[DataLen - 1] = 30.0;
			//::MessageBox(NULL, TEXT("Login Success "), TEXT("提示"), 1);
		}
	}


	//	HANDLE recvHandle = CreateThread(NULL, 0, ProcThread, &clientSock, 0, NULL);
	//	if (NULL == recvHandle)
	//	{
	//	cout << "Create Thread failed !" << endl;
	//	return -1;
	//}
	/*
	printf("longin...\n");
	TcpPkgInfo tTcpPkg;
	tTcpPkg.tcpCmdType = ACCOUNT_LOGIN;
	strcpy_s(tTcpPkg.account.accountName, "309619426390");
	int a = send(clientSock, (char*)&tTcpPkg, sizeof(TcpPkgInfo), 0);
	while (1)
	{
	if (a > 0 && isLogin)
	{
	printf("login ok a = %d\n", a);
	break;
	}
	}
	*/
	//while (1);
	//Sleep(200);
	TcpPkgInfo tTcpPkg;
	tTcpPkg.tcpCmdType = ZT_BUY;
	strcpy_s(tTcpPkg.account.accountName, "309619426390");
	strcpy_s(tTcpPkg.stock.stockId, "601165");
	tTcpPkg.stock.stockQuantity = 400;
	tTcpPkg.stock.stockPrice = 2.50;
	int a = send(clientSock, (char*)&tTcpPkg, sizeof(TcpPkgInfo), 0);
	while (1)
	{
		if (a > 0 && isLogin)
		{
			//	printf("login ok a = %d\n", a);
			//break;

			memset((char*)&tTcpPkg, 0x00, sizeof(TcpPkgInfo));

			int recvLen = recv(clientSock, (char*)&tTcpPkg, sizeof(TcpPkgInfo), 0);
			if (recvLen > 0)
			{
				if (tTcpPkg.status == Z_OK)
				{
					//isLogin = true;
					//printf("Login Success !\n");
					pfOUT[DataLen - 1] = 40.0;
				}
			}
			break;
		}
	}




	//	SendMes(clientSock);

	//HANDLE sendHandle = CreateThread(NULL, 0, ProcThread, &clientSock, 0, NULL);
	//if (NULL == sendHandle)
	//{
	//	cout << "Create Thread failed !" << endl;
	//	return -1;
	//}
	//Sleep(20000);
	closesocket(clientSock);
	//CloseHandle(sendHandle);
	//CloseHandle(recvHandle);
	WSACleanup();
}

void TestPlugin2(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc)
{
	for (int i = 0; i<DataLen; i++)
	{
		pfOUT[i] = pfINa[i] + pfINb[i] + pfINc[i];
		pfOUT[i] = pfOUT[i] / 3;
	}
}


//加载的函数
PluginTCalcFuncInfo g_CalcFuncSets[] =
{
	{ 1, (pPluginFUNC)&TestPlugin1 },
	{ 2, (pPluginFUNC)&TestPlugin2 },
	{ 0, NULL },
};

//导出给TCalc的注册函数
BOOL RegisterTdxFunc(PluginTCalcFuncInfo** pFun)
{
	if (*pFun == NULL)
	{
		(*pFun) = g_CalcFuncSets;
		return TRUE;
	}
	return FALSE;
}