
// ZoTrade.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CZoTradeApp: 
// �йش����ʵ�֣������ ZoTrade.cpp
//

class CZoTradeApp : public CWinApp
{
public:
	CZoTradeApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CZoTradeApp theApp;