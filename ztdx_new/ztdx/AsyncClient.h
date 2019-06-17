#pragma once


// CAsyncClient 命令目标

class CAsyncClient : public CAsyncSocket {
protected:
	//CClientDlg* m_pDlg; //指向主对话框的指针。

public:
	CAsyncClient();
	//CAsyncClient(CClientDlg* pDlg) {
	//	m_pDlg = pDlg;
	//}
	virtual ~CAsyncClient();

public:
	BOOL bConnected = FALSE; //标识是否连通了服务器。

	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};


