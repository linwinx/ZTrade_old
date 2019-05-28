#pragma once

#ifndef ITEMRESIZE_H
#define ITMERESIZE_H

/************************************************************************/
/* ����:molin00@qq.com                                                  */
/************************************************************************/

#include <stdio.h>
#include <vector>
#include "resource.h"
using namespace std;

static UINT BASED_CODE indicators[] =
//״̬����ָʾ���б�����δ������ַ�����������Resource View��String Table����Ӷ���
{
	ID_INDICATOR_SH50,
	ID_INDICATOR_ZJ,
	ID_INDICATOR_ACC,//����Resource View��String Table����Ӷ���
	ID_INDICATOR_TIME,
};

typedef struct _RESIZEINFO
{
	UINT nID;
	CWnd* pParent;
	float fLeft;
	float fRight;
	float fTop;
	float fBottom;
}RESIZEINFO;

class CItemResize
{
	CRect m_rectDlg;

	//�ڴ��ڴ���ʱ�������Ҫ�Զ�������������С�Ŀؼ�ID��
	vector<RESIZEINFO> m_vResizeID;
public:
	CItemResize(void);
	~CItemResize(void);

	void AddItemRect(UINT nID, CWnd* pParent);
	void ResizeItem();

	CStatusBar m_StatusBar;
	int index1, index2, index3, index4;   //��ʶ�������
	int nWidth1, nWidth2, nWidth3, nWidth4; //����IDS_INDICATOR_MESSAGE, IDS_INDICATOR_TIME���

	UINT  nID1, nID2, nID3, nID4;    //���ڽ������ID
	UINT nStyle; //���ڽ��������
	void  AddStatusBar(CWnd* pParent);
	void  ResizeStatusBar(CWnd* pParent, int cx, int cy);
};

#endif
