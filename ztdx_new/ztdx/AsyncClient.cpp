// AsyncClient.cpp : ʵ���ļ�
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


// CAsyncClient ��Ա����


//�������Ϸ�����ʱ���á�
void CAsyncClient::OnConnect(int nErrorCode) {
	bConnected = TRUE; //�������ӱ�־Ϊ�档
}

//�з�������������ʱ���á�
void CAsyncClient::OnReceive(int nErrorCode) {
	//1��������ʱ���ַ����������������ݻ������Լ�ʵ�ʽ������ݵĴ�С�����ͱ�����
	WCHAR wzBuff[1024]; CHAR szBuff[1024]; int len;

	//2���������ݡ�
	len = Receive(szBuff, sizeof(szBuff));
	szBuff[len] = '\0';

	//3�������յ�����ת��Ϊ���ַ��󣬲��������¼�б��С�
	MultiByteToWideChar(CP_ACP, 0, szBuff, -1, wzBuff, len);
	//m_pDlg->lbMsg.InsertString(0, wzBuff);
}
