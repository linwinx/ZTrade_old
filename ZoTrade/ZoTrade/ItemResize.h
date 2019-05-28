#pragma once

#ifndef ITEMRESIZE_H
#define ITMERESIZE_H

/************************************************************************/
/* 作者:molin00@qq.com                                                  */
/************************************************************************/

#include <stdio.h>
#include <vector>
#include "resource.h"
using namespace std;

static UINT BASED_CODE indicators[] =
//状态栏的指示器列表，如有未定义的字符串名，需在Resource View的String Table中添加定义
{
	ID_INDICATOR_SH50,
	ID_INDICATOR_ZJ,
	ID_INDICATOR_ACC,//需在Resource View的String Table中添加定义
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

	//在窗口创建时，添加需要自动按比例调整大小的控件ID。
	vector<RESIZEINFO> m_vResizeID;
public:
	CItemResize(void);
	~CItemResize(void);

	void AddItemRect(UINT nID, CWnd* pParent);
	void ResizeItem();

	CStatusBar m_StatusBar;
	int index1, index2, index3, index4;   //标识面板索引
	int nWidth1, nWidth2, nWidth3, nWidth4; //保存IDS_INDICATOR_MESSAGE, IDS_INDICATOR_TIME宽度

	UINT  nID1, nID2, nID3, nID4;    //用于接收面板ID
	UINT nStyle; //用于接收面板风格
	void  AddStatusBar(CWnd* pParent);
	void  ResizeStatusBar(CWnd* pParent, int cx, int cy);
};

#endif
