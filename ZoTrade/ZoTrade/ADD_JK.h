#pragma once
#include "TradeApi.h"
// ADD_JK �Ի���

#define WM_USERMESSAGE WM_USER+110 //WM_USERMESSAGEΪ�Լ��������Ϣ����

class ADD_JK : public CDialogEx
{
	DECLARE_DYNAMIC(ADD_JK)

public:
	ADD_JK(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ADD_JK();

// �Ի�������
	enum { IDD = IDD_DLG_ADD_JK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void ZqdmOnKillfocusEdit();
	void UpdateZqName();
	void AddJkItem();
   
	TradeApi m_add_trade;

};
