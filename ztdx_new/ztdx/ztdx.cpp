// ztdx.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "ztdx.h"
#include <iostream>
#include <stdlib.h>
#include <WinSock2.h>

#include "ccommon.h"
#include "AsyncClient.h"
#pragma comment(lib,"ws2_32.lib")
//���ɵ�dll���������dll�뿽����ͨ���Ű�װĿ¼��T0002/dlls/����,���ڹ�ʽ���������а�

CAsyncClient* pSocket = NULL; //ָ��Socket��ָ�롣

void TcpConnect() {
	//1������һ���ͻ�Socket����
	pSocket = new CAsyncClient();
	if (pSocket->Create()) {
		//2�����ӷ�������
		int i = 0;
		while (!pSocket->bConnected && i < 10) {
			CString IP=_T("127.0.0.1");
			pSocket->Connect(IP, 8000);
			AfxGetApp()->PumpMessage();
			i++;
		}
		if (pSocket->bConnected) {
			//3�����°�ť״̬��
		//	((CButton*)GetDlgItem(IDC_CONNECT))->EnableWindow(FALSE);
		//	((CButton*)GetDlgItem(IDC_SEND))->EnableWindow(TRUE);

			//4���������ӳɹ���Ϣ��
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
	//1�����ͻ���Socket��Ϣ��ͻ��ӱ༭�����������Ϣ��֯�����ӣ�������
	CString IP, msg, fmt; UINT port;
	pSocket->GetSockName(IP, port); //�ͻ���Socket��Ϣ��
	//GetDlgItemText(IDC_SENDMSG, msg); //�ͻ��ӱ༭�����������Ϣ��
	msg = _T("home");
	fmt.Format(_T("%s:%d=>%s"), IP, port, msg);

	//2�������ַ�����Ϣת��Ϊchar���͡�
	char szBuff[1024] = "home";
	//int len = WideCharToMultiByte(CP_OEMCP, 0, fmt, -1, szBuff, sizeof(szBuff), 0, FALSE);

	//3���������������Ϣ��
	pSocket->Send(szBuff, strlen(szBuff) + 1);
}


void TestPlugin1(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc)
{
	//for (int i = 0; i<DataLen; i++)
	//pfOUT[DataLen - 1] = pfINa[DataLen - 1] - pfINb[DataLen - 1];
	//pfOUT[DataLen - 1] = pfOUT[DataLen - 1] / 2;
#if 1
	//1������һ���ͻ�Socket����
	pSocket = new CAsyncClient();
	if (pSocket->Create()) {
		//2�����ӷ�������
		int i = 0;
		while (!pSocket->bConnected && i < 10) {
			CString IP = _T("127.0.0.1");
			pSocket->Connect(IP, 8000);
			AfxGetApp()->PumpMessage();
			i++;
		}
		if (pSocket->bConnected) {
			//3�����°�ť״̬��
			//	((CButton*)GetDlgItem(IDC_CONNECT))->EnableWindow(FALSE);
			//	((CButton*)GetDlgItem(IDC_SEND))->EnableWindow(TRUE);

			//4���������ӳɹ���Ϣ��
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
	pSocket->GetSockName(IP1, port1); //�ͻ���Socket��Ϣ��
	//GetDlgItemText(IDC_SENDMSG, msg); //�ͻ��ӱ༭�����������Ϣ��
	msg = _T("home");
	fmt.Format(_T("%s:%d=>%s"), IP1, port1, msg);

	//2�������ַ�����Ϣת��Ϊchar���͡�
	char szBuff[1024];
	int len = WideCharToMultiByte(CP_OEMCP, 0, fmt, -1, szBuff, sizeof(szBuff), 0, FALSE);

	//3���������������Ϣ��
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


//���صĺ���
PluginTCalcFuncInfo g_CalcFuncSets[] =
{
	{ 1, (pPluginFUNC)&TestPlugin1 },
	{ 2, (pPluginFUNC)&TestPlugin2 },
	{ 0, NULL },
};

//������TCalc��ע�ắ��
BOOL RegisterTdxFunc(PluginTCalcFuncInfo** pFun)
{
	if (*pFun == NULL)
	{
		(*pFun) = g_CalcFuncSets;
		return TRUE;
	}
	return FALSE;
}