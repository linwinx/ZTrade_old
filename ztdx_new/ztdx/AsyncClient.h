#pragma once


// CAsyncClient ����Ŀ��

class CAsyncClient : public CAsyncSocket {
protected:
	//CClientDlg* m_pDlg; //ָ�����Ի����ָ�롣

public:
	CAsyncClient();
	//CAsyncClient(CClientDlg* pDlg) {
	//	m_pDlg = pDlg;
	//}
	virtual ~CAsyncClient();

public:
	BOOL bConnected = FALSE; //��ʶ�Ƿ���ͨ�˷�������

	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};


