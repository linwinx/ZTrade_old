// ADD_JK.cpp : 实现文件
//

#include "stdafx.h"
#include "ZoTrade.h"
#include "ADD_JK.h"
#include "afxdialogex.h"


CCriticalSection g_clsUpdateAddJk;

// ADD_JK 对话框

IMPLEMENT_DYNAMIC(ADD_JK, CDialogEx)

ADD_JK::ADD_JK(CWnd* pParent /*=NULL*/)
	: CDialogEx(ADD_JK::IDD, pParent)
{

}

ADD_JK::~ADD_JK()
{
}

void ADD_JK::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ADD_JK, CDialogEx)
	ON_BN_CLICKED(IDOK, &ADD_JK::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT_ZQDM, &ADD_JK::ZqdmOnKillfocusEdit)
END_MESSAGE_MAP()

void ADD_JK::AddJkItem()
{
	//CString t_Zqdm, t_ZqName, t_BuyNum, t_XiaDie, t_HuiTiao;
	JKREPORT jk;
	GetDlgItem(IDC_EDIT_ZQDM)->GetWindowText(jk.zCode);
	GetDlgItem(IDC_EDIT_ZQNAME)->GetWindowText(jk.zName);
	GetDlgItem(IDC_EDIT_BUYNUM)->GetWindowText(jk.zBuyNum);
	GetDlgItem(IDC_EDIT_XIADIE)->GetWindowText(jk.zXiaDieRate);
	GetDlgItem(IDC_EDIT_HUITIAO)->GetWindowText(jk.zXdHtRate);

	CString str;
	str = "ZoTrade";//对话框标题 
	CWnd *pDlg = CWnd::FindWindow(NULL, str);
	pDlg->SendMessage(WM_USERMESSAGE, MSG_ADD_JK, (LPARAM)&jk);

}

// ADD_JK 消息处理程序
UINT AddJkItemThreadProc(LPVOID pParam)
{
	ADD_JK *p = (ADD_JK*)pParam;
	g_clsUpdateAddJk.Lock();
	p->AddJkItem();
	g_clsUpdateAddJk.Unlock();

	return 0;
}

void ADD_JK::OnBnClickedOk()
{
	AfxBeginThread((AFX_THREADPROC)AddJkItemThreadProc, (LPVOID)this);
	//ADD_JK::OnOK();  

	//AddJkItem();
}

void ADD_JK::UpdateZqName()
{
	CString t_Zqdm;
	REPORT t_zqinfo;
	GetDlgItem(IDC_EDIT_ZQDM)->GetWindowText(t_Zqdm);

	t_zqinfo = m_add_trade.GetZqInfo(t_Zqdm);
	if (t_zqinfo.zName.IsEmpty()) 
	{
		GetDlgItem(IDC_EDIT_ZQNAME)->SetWindowText(_T("Error"));
	}
	else 
	{
		GetDlgItem(IDC_EDIT_ZQNAME)->SetWindowText(t_zqinfo.zName);
	}

}

UINT MyAddJkThreadProc(LPVOID pParam)
{
	ADD_JK *p = (ADD_JK*)pParam;
	g_clsUpdateAddJk.Lock();
	p->UpdateZqName();
	g_clsUpdateAddJk.Unlock();

	return 0;
}

void ADD_JK::ZqdmOnKillfocusEdit()
{
	AfxBeginThread((AFX_THREADPROC)MyAddJkThreadProc, (LPVOID)this);
}
