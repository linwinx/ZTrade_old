// ztdx.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ztdx.h"
#include <WinSock2.h>

#include "ccommon.h"

#pragma comment(lib,"ws2_32.lib")
//生成的dll及相关依赖dll请拷贝到通达信安装目录的T0002/dlls/下面,再在公式管理器进行绑定

void TestPlugin1(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc)
{
	//for (int i = 0; i<DataLen; i++)
	pfOUT[DataLen - 1] = pfINa[DataLen - 1] - pfINb[DataLen - 1];
	pfOUT[DataLen - 1] = pfOUT[DataLen - 1] / 2;

	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSock == -1)
	{
		return;
	}

	SOCKADDR_IN addrClient;
	addrClient.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(6000);

	int ret = connect(clientSock, (sockaddr*)&addrClient, sizeof(SOCKADDR));
	if (ret)
	{		
		return ;
	}

	TcpPkgInfo tTcpPkg;
	tTcpPkg.tcpCmdType = ZT_BUY;
	strcpy_s(tTcpPkg.account.accountName, "309619426390");
	strcpy_s(tTcpPkg.stock.stockId, "6001165");
	tTcpPkg.stock.stockQuantity = 400;
	tTcpPkg.stock.stockPrice = 2.50;
	int a = send(clientSock, (char*)&tTcpPkg, sizeof(TcpPkgInfo), 0);
	while (1)
	{
		if (a > 0) break;
	}
	closesocket(clientSock);
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