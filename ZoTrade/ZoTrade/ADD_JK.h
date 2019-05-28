#pragma once
#include "TradeApi.h"
// ADD_JK 对话框

#define WM_USERMESSAGE WM_USER+110 //WM_USERMESSAGE为自己定义的消息名称

class ADD_JK : public CDialogEx
{
	DECLARE_DYNAMIC(ADD_JK)

public:
	ADD_JK(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ADD_JK();

// 对话框数据
	enum { IDD = IDD_DLG_ADD_JK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void ZqdmOnKillfocusEdit();
	void UpdateZqName();
	void AddJkItem();
   
	TradeApi m_add_trade;

};
