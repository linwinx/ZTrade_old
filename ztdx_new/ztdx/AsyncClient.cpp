// AsyncClient.cpp : 实现文件
//

#include "stdafx.h"
//#include "Client.h"
#include "AsyncClient.h"


// CAsyncClient

CAsyncClient::CAsyncClient()
{
}

CAsyncClient::~CAsyncClient() {
	if (m_hSocket != INVALID_SOCKET) Close();
}


// CAsyncClient 成员函数


//当链接上服务器时调用。
void CAsyncClient::OnConnect(int nErrorCode) {
	bConnected = TRUE; //更新连接标志为真。
}

//有服务器发送数据时调用。
void CAsyncClient::OnReceive(int nErrorCode) {
	//1、定义临时宽字符缓冲区、接收数据缓冲区以及实际接收数据的大小的整型变量。
	WCHAR wzBuff[1024]; CHAR szBuff[1024]; int len;

	//2、接收数据。
	len = Receive(szBuff, sizeof(szBuff));
	szBuff[len] = '\0';

	//3、将接收的数据转换为宽字符后，插入聊天记录列表中。
	MultiByteToWideChar(CP_ACP, 0, szBuff, -1, wzBuff, len);
	//m_pDlg->lbMsg.InsertString(0, wzBuff);
}
