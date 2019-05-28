#include "stdafx.h"
#include "ItemResize.h"


CItemResize::CItemResize(void)
{
}

CItemResize::~CItemResize(void)
{
}

void CItemResize::AddItemRect(UINT nID, CWnd* pParent)
{
	RESIZEINFO rf = { 0 };
	rf.nID = nID;
	rf.pParent = pParent;
	CRect rect;
	pParent->GetClientRect(&m_rectDlg);
	CWnd *pItem = pParent->GetDlgItem(nID);
	ASSERT(pItem); //�жϿؼ�ָ���Ƿ���Ч
	pItem->GetWindowRect(&rect);
	pParent->ScreenToClient(&rect);

	rf.fLeft = rect.left;
	rf.fRight = m_rectDlg.Width() - rect.right;
	rf.fTop = rect.top;
	rf.fBottom = m_rectDlg.Height() - rect.bottom;

	m_vResizeID.push_back(rf);
}

void CItemResize::ResizeItem()
{
	for (UINT i = 0; i<m_vResizeID.size(); i++)
	{
		m_vResizeID[i].pParent->GetClientRect(&m_rectDlg);
		CWnd *pItem = m_vResizeID[i].pParent->GetDlgItem(m_vResizeID[i].nID);
		if (pItem)
		{
			CRect rect;
			rect.left = m_vResizeID[i].fLeft;
			rect.right = m_rectDlg.Width() - m_vResizeID[i].fRight;
			rect.top = m_vResizeID[i].fTop;
			rect.bottom = m_rectDlg.Height() - m_vResizeID[i].fBottom;
			pItem->MoveWindow(rect);//���ÿؼ���С
		}
	}
}

void CItemResize::AddStatusBar(CWnd* pParent)
{

	if (!m_StatusBar.CreateEx(pParent, SBT_TOOLTIPS, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, AFX_IDW_STATUS_BAR) ||
		!m_StatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT)))//����ָʾ������
	{
		TRACE0("δ�ܴ���״̬��\n");
		return;      // δ�ܴ���
	}

	//��ȡIDS_INDICATOR_MESSAGE,IDS_INDICATOR_TIME�������Ͷ�Ӧ�Ŀ��
	index1 = m_StatusBar.CommandToIndex(ID_INDICATOR_SH50);
	index2 = m_StatusBar.CommandToIndex(ID_INDICATOR_ZJ);
	index3 = m_StatusBar.CommandToIndex(ID_INDICATOR_ACC);
	index4 = m_StatusBar.CommandToIndex(ID_INDICATOR_TIME);

	m_StatusBar.GetPaneInfo(index1, nID1, nStyle, nWidth1);
	m_StatusBar.GetPaneInfo(index2, nID2, nStyle, nWidth2);
	m_StatusBar.GetPaneInfo(index3, nID3, nStyle, nWidth3);
	m_StatusBar.GetPaneInfo(index4, nID4, nStyle, nWidth4);

	CRect rect;
	pParent->GetClientRect(&rect);
	m_StatusBar.SetPaneInfo(index1, nID1, nStyle, rect.Width() * 12 / 32 - 15);//����ָʾ�����ڵ��й���Ϣ
	m_StatusBar.SetPaneInfo(index2, nID2, nStyle, rect.Width() * 8 / 32 - 15);
	m_StatusBar.SetPaneInfo(index3, nID3, nStyle, rect.Width() * 6 / 32 - 15);
	m_StatusBar.SetPaneInfo(index4, nID4, nStyle, rect.Width() * 8 / 32 - 15);

	pParent->RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);//����Ҫ****
	m_StatusBar.GetStatusBarCtrl().SetBkColor(RGB(225, 225, 225));//���ñ���
	//m_StatusBar.SetPaneText(index1, _T("ָʾ��1"), 0);
	//m_StatusBar.SetPaneText(index2, _T("ָʾ��2"), 0);
	//m_StatusBar.SetPaneText(index3, _T("ָʾ��3"), 0);
}

void CItemResize::ResizeStatusBar(CWnd* pParent, int cx, int cy)
{
	CRect rectDlg, rectBar;
	pParent->GetClientRect(rectDlg);
	CStatusBar *m_pwndStatusBar = (CStatusBar*)&(m_StatusBar); //��ȡָ��m_wndStatusBar��ָ��
	m_pwndStatusBar->GetClientRect(&rectBar);//��ȡ��ǰStatusBar�Ĵ�С����

	m_StatusBar.SetPaneInfo(index1, nID1, nStyle, rectDlg.Width() * 12 / 32 - 15);//����ָʾ�����ڵ��й���Ϣ
	m_StatusBar.SetPaneInfo(index2, nID2, nStyle, rectDlg.Width() * 8 / 32 - 15);
	m_StatusBar.SetPaneInfo(index3, nID3, nStyle, rectDlg.Width() * 6 / 32 - 15);
	m_StatusBar.SetPaneInfo(index4, nID4, nStyle, rectDlg.Width() * 8 / 32 - 15);
	m_pwndStatusBar->MoveWindow(0, cy - rectBar.Height(), rectDlg.Width(), rectBar.Height()); //�ƶ�StatusBar
}