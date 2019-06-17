// ztdx.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ztdx.h"
#include <iostream>
#include <stdlib.h>
#include <WinSock2.h>

#include "ccommon.h"
#include "AsyncClient.h"
#pragma comment(lib,"ws2_32.lib")
//生成的dll及相关依赖dll请拷贝到通达信安装目录的T0002/dlls/下面,再在公式管理器进行绑定

CAsyncClient* pSocket = NULL; //指向Socket的指针。

void TcpConnect() {
	//1、构造一个客户Socket对象。
	pSocket = new CAsyncClient();
	if (pSocket->Create()) {
		//2、连接服务器。
		int i = 0;
		while (!pSocket->bConnected && i < 10) {
			CString IP=_T("127.0.0.1");
			pSocket->Connect(IP, 8000);
			AfxGetApp()->PumpMessage();
			i++;
		}
		if (pSocket->bConnected) {
			//3、更新按钮状态。
		//	((CButton*)GetDlgItem(IDC_CONNECT))->EnableWindow(FALSE);
		//	((CButton*)GetDlgItem(IDC_SEND))->EnableWindow(TRUE);

			//4、报告连接成功信息。
		//	lbMsg.AddString(_T("Connection success."));
		//	AfxMessageBox(_T("connect ok!"));
		}
		else {
			delete pSocket;
			pSocket = NULL;
		//	AfxMessageBox(_T("The connection server failed!"));
		}
	}
	else {
		delete pSocket;
		pSocket = NULL;
	//	AfxMessageBox(_T("Failed to create the socket!"));
	}
}


void SendStock() {
	//1、将客户的Socket信息与客户从编辑框中输入的信息组织（连接）起来。
	CString IP, msg, fmt; UINT port;
	pSocket->GetSockName(IP, port); //客户的Socket信息。
	//GetDlgItemText(IDC_SENDMSG, msg); //客户从编辑框中输入的信息。
	msg = _T("home");
	fmt.Format(_T("%s:%d=>%s"), IP, port, msg);

	//2、将宽字符的信息转换为char类型。
	char szBuff[1024] = "home";
	//int len = WideCharToMultiByte(CP_OEMCP, 0, fmt, -1, szBuff, sizeof(szBuff), 0, FALSE);

	//3、向服务器发送信息。
	pSocket->Send(szBuff, strlen(szBuff) + 1);
}


void TestPlugin1(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc)
{
	//for (int i = 0; i<DataLen; i++)
	//pfOUT[DataLen - 1] = pfINa[DataLen - 1] - pfINb[DataLen - 1];
	//pfOUT[DataLen - 1] = pfOUT[DataLen - 1] / 2;
#if 1
	//1、构造一个客户Socket对象。
	pSocket = new CAsyncClient();
	if (pSocket->Create()) {
		//2、连接服务器。
		int i = 0;
		while (!pSocket->bConnected && i < 10) {
			CString IP = _T("127.0.0.1");
			pSocket->Connect(IP, 8000);
			AfxGetApp()->PumpMessage();
			i++;
		}
		if (pSocket->bConnected) {
			//3、更新按钮状态。
			//	((CButton*)GetDlgItem(IDC_CONNECT))->EnableWindow(FALSE);
			//	((CButton*)GetDlgItem(IDC_SEND))->EnableWindow(TRUE);

			//4、报告连接成功信息。
			//	lbMsg.AddString(_T("Connection success."));
			//	AfxMessageBox(_T("connect ok!"));
			pfOUT[DataLen - 1] = 10.0;
		}
		else {
			delete pSocket;
			pSocket = NULL;
			//	AfxMessageBox(_T("The connection server failed!"));
			pfOUT[DataLen - 1] = 20.0;
		}
	}
	else {
		delete pSocket;
		pSocket = NULL;
		//	AfxMessageBox(_T("Failed to create the socket!"));
		pfOUT[DataLen - 1] = 30.0;
	}


	CString IP1, msg, fmt; UINT port1;
	pSocket->GetSockName(IP1, port1); //客户的Socket信息。
	//GetDlgItemText(IDC_SENDMSG, msg); //客户从编辑框中输入的信息。
	msg = _T("home");
	fmt.Format(_T("%s:%d=>%s"), IP1, port1, msg);

	//2、将宽字符的信息转换为char类型。
	char szBuff[1024];
	int len = WideCharToMultiByte(CP_OEMCP, 0, fmt, -1, szBuff, sizeof(szBuff), 0, FALSE);

	//3、向服务器发送信息。
	int st = pSocket->Send(szBuff, strlen(szBuff)+1);
#endif
	//delete pSocket;
	//pSocket = NULL;
	//pSocket->KillSocket();
	pfOUT[DataLen - 1] = 40.0;
	while (1)
	{
		if (st > 0) break;
	}

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