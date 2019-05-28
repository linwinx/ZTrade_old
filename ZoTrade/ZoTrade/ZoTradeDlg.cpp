
// ZoTradeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZoTrade.h"
#include "ZoTradeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CCriticalSection g_clsCriticalSection;
CCriticalSection g_clsUpdateZyZs;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CZoTradeDlg �Ի���



CZoTradeDlg::CZoTradeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CZoTradeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZoTradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STOCK_LIST, m_list);
	DDX_Control(pDX, IDC_CHENGJIAO_LIST, m_CJList);
	DDX_Control(pDX, IDC_JIANKONG_LIST, m_JKList);
}

BEGIN_MESSAGE_MAP(CZoTradeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_STOCK_LIST, &CZoTradeDlg::OnNMClickStockList)
	ON_NOTIFY(NM_CLICK, IDC_JIANKONG_LIST, &CZoTradeDlg::JkOnNMClickStockList)
	ON_BN_CLICKED(IDOK, &CZoTradeDlg::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT_CREATEID, &CZoTradeDlg::OnKillfocusEdit)//��Ӷ�̬���ɱ༭���ʧȥ������Ӧ����  
	ON_EN_KILLFOCUS(IDC_EDIT_CREATEID_JK, &CZoTradeDlg::JkOnKillfocusEdit)
	ON_CBN_KILLFOCUS(IDC_COMBOX_CREATEID, &CZoTradeDlg::OnKillfocusCcomboBox)
	ON_BN_CLICKED(IDC_BTN_ADD_JK, &CZoTradeDlg::OnBnClickedBtnAddJk)
	ON_BN_CLICKED(IDC_BTN_DEL_JK, &CZoTradeDlg::OnBnClickedBtnDelJk)
	ON_MESSAGE(WM_USERMESSAGE, &CZoTradeDlg::ReceiveMessage)   // ����Զ�����Ϣӳ��꣬����������Ϊ����ԭ�� 
	ON_WM_CLOSE()
END_MESSAGE_MAP()

CZoTradeDlg::~CZoTradeDlg()
{
	m_trade.Logout();
}

BOOL CZoTradeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	m_itemSize.AddStatusBar(this);
	m_itemSize.AddItemRect(IDC_STOCK_LIST, this);

	//���ö�ʱ��
	SetTimer(TIMER_TIME, TIMER_TIME_INTERVAL, NULL);
	SetTimer(TIMER_UPDATE_STOCK, TIMER_UPDATE_STOCK_INTERVAL, NULL); 
	SetTimer(TIMER_UPDATE_SELL, TIMER_UPDATE_SELL_INTERVAL, NULL);
	SetTimer(TIMER_UPDATE_CHENGJIAO, TIMER_UPDATE_CHENGJIAO_INTERVAL, NULL);
	SetTimer(TIMER_UPDATE_JIANKONG, TIMER_UPDATE_JIANKONG_INTERVAL, NULL);
	SetTimer(TIMER_UPDATE_ZJSHSZ, TIMER_UPDATE_ZJSHSZ_INTERVAL, NULL);

	haveeditcreate = false;//��ʼ����־λ����ʾ��û�д����༭��  
	haveccomboboxcreate = false;//��ʼ����־λ����ʾ��û�д��������б��  
	jk_haveeditcreate = false;//��ʼ����־λ����ʾ��û�д����༭��  

	RECT  m_rect;
	m_list.GetClientRect(&m_rect); //��ȡlist�Ŀͻ���,�������ÿһ�еĿ��    
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //����list���LVS_EX_GRIDLINESΪ�����ߣ�ֻ������report����listctrl��  
	//LVS_EX_FULLROWSELECTΪѡ��ĳ��ʹ���и�����ֻ������report����listctrl��  
	m_list.InsertColumn(0, _T("����"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum);
	m_list.InsertColumn(1, _T("����"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 1.3);
	m_list.InsertColumn(2, _T("����"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 0.9);
	m_list.InsertColumn(3, _T("����"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 0.9);
	m_list.InsertColumn(4, _T("�ɱ���"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 0.9);
	m_list.InsertColumn(5, _T("��ǰ��"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 0.9);
	m_list.InsertColumn(6, _T("��ֵ"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum * 1.3);
	m_list.InsertColumn(7, _T("ӯ��"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 1.2);
	m_list.InsertColumn(8, _T("ӯ��%"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum);
	m_list.InsertColumn(9, _T("ֹӯ%"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 1.15);
	m_list.InsertColumn(10, _T("����%"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 1.15);
	m_list.InsertColumn(11, _T("ֹ��%"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 1.15);

	RECT m_cjRect;
	m_CJList.GetClientRect(&m_cjRect); //��ȡlist�Ŀͻ���,�������ÿһ�еĿ��    
	m_CJList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_CJList.InsertColumn(0, _T("����"), LVCFMT_LEFT, (m_cjRect.right - m_cjRect.left) / m_cjColnum);
	m_CJList.InsertColumn(1, _T("����"), LVCFMT_LEFT, (m_cjRect.right - m_cjRect.left) / m_cjColnum *1.1);
	m_CJList.InsertColumn(2, _T("�۸�"), LVCFMT_LEFT, (m_cjRect.right - m_cjRect.left) / m_cjColnum);
	m_CJList.InsertColumn(3, _T("����"), LVCFMT_LEFT, (m_cjRect.right - m_cjRect.left) / m_cjColnum *0.85);
	m_CJList.InsertColumn(4, _T("ʱ��"), LVCFMT_LEFT, (m_cjRect.right - m_cjRect.left) / m_cjColnum*1.1);
	m_CJList.InsertColumn(5, _T("����"), LVCFMT_LEFT, (m_cjRect.right - m_cjRect.left) / m_cjColnum*0.9);

	RECT m_jkRect;
	m_JKList.GetClientRect(&m_jkRect); //��ȡlist�Ŀͻ���,�������ÿһ�еĿ��    
	m_JKList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_JKList.InsertColumn(0, _T("����"), LVCFMT_LEFT, (m_jkRect.right - m_jkRect.left) / m_jkColnum);
	m_JKList.InsertColumn(1, _T("����"), LVCFMT_LEFT, (m_jkRect.right - m_jkRect.left) / m_jkColnum*1.1);
	m_JKList.InsertColumn(2, _T("����"), LVCFMT_LEFT, (m_jkRect.right - m_jkRect.left) / m_jkColnum*0.85);
	m_JKList.InsertColumn(3, _T("����"), LVCFMT_LEFT, (m_jkRect.right - m_jkRect.left) / m_jkColnum*0.85);
	m_JKList.InsertColumn(4, _T("����"), LVCFMT_LEFT, (m_jkRect.right - m_jkRect.left) / m_jkColnum*0.85);
	m_JKList.InsertColumn(5, _T("�µ�%"), LVCFMT_LEFT, (m_jkRect.right - m_jkRect.left) / m_jkColnum*1.15);
	m_JKList.InsertColumn(6, _T("�ص�%"), LVCFMT_LEFT, (m_jkRect.right - m_jkRect.left) / m_jkColnum*1.15);


	m_trade.Login();
	m_trade.GetChicangInfo();
	m_trade.GetGddm();

	m_trade.initIni();

	UpdateReport();

	m_trade.GetChengjiaoInfo();
	UpdateCjReport();

	UpdateJkReport();

	//m_trade.GetZqGddm(_T("300003"));
	m_trade.GetZqZj();
	m_bar_kyzj = m_trade.GetZqZjStr();
	m_bar_zjzh = m_trade.GetZjzhStr();
	m_itemSize.m_StatusBar.SetPaneText(m_itemSize.index2, m_bar_kyzj);
	m_itemSize.m_StatusBar.SetPaneText(m_itemSize.index3, m_bar_zjzh);

	m_bar_clock = GetTimeInfo();
	m_itemSize.m_StatusBar.SetPaneText(m_itemSize.index4, m_bar_clock);

	m_bar_shsz = m_trade.GetShszStr();	
	m_itemSize.m_StatusBar.SetPaneText(m_itemSize.index1, m_bar_shsz);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void CZoTradeDlg::UpdateReport()
{
	int i = 0;
	m_list.DeleteAllItems();

	for (i = 0; i < m_trade.m_stock_num; i++)
	{
		m_list.InsertItem(i, m_trade.m_report[i].zCode);// 
		m_list.SetItemText(i, 1, m_trade.m_report[i].zName);
		m_list.SetItemText(i, 2, m_trade.m_report[i].zAllNum);
		m_list.SetItemText(i, 3, m_trade.m_report[i].zValidNum);
		m_list.SetItemText(i, 4, m_trade.m_report[i].zBuyPrice);
		m_list.SetItemText(i, 5, m_trade.m_report[i].zNewPrice);
		m_list.SetItemText(i, 6, m_trade.m_report[i].zNowValue);
		m_list.SetItemText(i, 7, m_trade.m_report[i].zProfit);
		m_list.SetItemText(i, 8, m_trade.m_report[i].zProfitRate);

        UpdateZyZsReport(m_trade.m_report[i].zCode,i);
	
	}

}

void CZoTradeDlg::UpdateZyZsReport(CString zZqdm, int list_num)
{
	int i = 0, j;
	CString t_ZhiYingRate;
	CString t_ZyHlRate;
	CString t_ZhiSunRate;
	float t_valule;
	CString tmp_rate;

	for (i = 0; i < m_trade.m_zyzs_num; i++)
	{
		//TRACE("@ %s %s %d\n", zZqdm, m_trade.zz_report[i].zCode, m_trade.m_zyzs_num);
		if (zZqdm == m_trade.zz_report[i].zCode)
		{  
			j = m_trade.zz_report[i].m_index;

			if (!m_trade.zz_report[i].zZhiYingRate.IsEmpty())
			{
				tmp_rate = m_trade.zz_report[i].zZhiYingRate;
				t_valule = atof(m_trade.m_report[j].zBuyPrice) * (1 + atof(m_trade.zz_report[i].zZhiYingRate) / 100);
				t_ZhiYingRate.Format("%s|%.2f", tmp_rate, t_valule);
				m_list.SetItemText(list_num, 9, t_ZhiYingRate);
			}
			if (!m_trade.zz_report[i].zZyHlRate.IsEmpty())
			{
				tmp_rate = m_trade.zz_report[i].zZyHlRate;
				t_valule = atof(m_trade.m_report[j].zBuyPrice) * (1 + (atof(m_trade.zz_report[i].zZhiYingRate) - atof(m_trade.zz_report[i].zZyHlRate)) / 100);
				t_ZyHlRate.Format("%s|%.2f", tmp_rate, t_valule);
				m_list.SetItemText(list_num, 10, t_ZyHlRate);
			}

			if (!m_trade.zz_report[i].zZhiSunRate.IsEmpty())
			{
				tmp_rate = m_trade.zz_report[i].zZhiSunRate;
				t_valule = atof(m_trade.m_report[j].zBuyPrice) * (1 - atof(m_trade.zz_report[i].zZhiSunRate) / 100);
				t_ZhiSunRate.Format("%s|%.2f", tmp_rate, t_valule);
				m_list.SetItemText(list_num, 11, t_ZhiSunRate);
			}
		}
	}
//	TRACE("UpdateZyZsReport\n");
}

void CZoTradeDlg::UpdateCjReport()
{
	int i = 0;
	m_CJList.DeleteAllItems();
	for (i = 0; i < m_trade.m_cj_num; i++)
	{
		m_CJList.InsertItem(i, m_trade.cj_report[i].zZqdm);// 
		m_CJList.SetItemText(i, 1, m_trade.cj_report[i].zName);
		m_CJList.SetItemText(i, 2, m_trade.cj_report[i].zCjPrice);
		m_CJList.SetItemText(i, 3, m_trade.cj_report[i].zCjNum);
		m_CJList.SetItemText(i, 4, m_trade.cj_report[i].zCjTime);
		m_CJList.SetItemText(i, 5, m_trade.cj_report[i].zCjFlag);
	}
	//TRACE("UpdateCjReport\n");
}

void CZoTradeDlg::UpdateJkReport()
{
	int i = 0;
	CString t_XiaDieRate;
	CString t_XdHtRate;
	float t_valule;
	CString zZqdm;
	REPORT t_zqinfo;

	m_JKList.DeleteAllItems();
	for (i = 0; i < m_trade.m_jk_num; i++)
	{
		zZqdm = m_trade.jk_report[i].zCode;
		t_zqinfo = m_trade.GetZqInfo(zZqdm);

		m_JKList.InsertItem(i, m_trade.jk_report[i].zCode);// 
		m_JKList.SetItemText(i, 1, m_trade.jk_report[i].zName);
		m_JKList.SetItemText(i, 4, m_trade.jk_report[i].zBuyNum);

		t_valule = atof(t_zqinfo.zLast) * (1 - atof(m_trade.jk_report[i].zXiaDieRate) / 100);
		t_XiaDieRate.Format("%s|%.2f", m_trade.jk_report[i].zXiaDieRate, t_valule);
		m_JKList.SetItemText(i, 5, t_XiaDieRate);

		t_valule = atof(t_zqinfo.zLast) * (1 - (atof(m_trade.jk_report[i].zXiaDieRate) - atof(m_trade.jk_report[i].zXdHtRate)) / 100);
		t_XdHtRate.Format("%s|%.2f", m_trade.jk_report[i].zXdHtRate, t_valule);
		m_JKList.SetItemText(i, 6, t_XdHtRate);
	}
	//TRACE("UpdateJkReport\n");
}

void CZoTradeDlg::UpdateJkPrice()
{
	int i = 0;
	CString zZqdm;
	REPORT t_zqinfo;
	for (i = 0; i < m_trade.m_jk_num; i++)
	{
		zZqdm = m_trade.jk_report[i].zCode;
		t_zqinfo = m_trade.GetZqInfo(zZqdm);
		m_JKList.SetItemText(i, 2, t_zqinfo.zLast);
		m_JKList.SetItemText(i, 3, t_zqinfo.zNew);
	}
	//TRACE("UpdateJkPrice\n");
}

void CZoTradeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CZoTradeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CZoTradeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CZoTradeDlg::threadUpdating()
{
	m_trade.GetChicangInfo();
	UpdateReport();
}

UINT MyUpdataThreadProc(LPVOID pParam)
{
	CZoTradeDlg *p = (CZoTradeDlg*)pParam;
	g_clsCriticalSection.Lock();
	p->threadUpdating();
	g_clsCriticalSection.Unlock();

	return 0;
}

void CZoTradeDlg::threadCjUpdating()
{
	m_trade.GetChengjiaoInfo();
	UpdateCjReport();
}

UINT MyUpdataCjThreadProc(LPVOID pParam)
{
	CZoTradeDlg *p = (CZoTradeDlg*)pParam;
	g_clsCriticalSection.Lock();
	p->threadCjUpdating();
	g_clsCriticalSection.Unlock();

	return 0;
}

void CZoTradeDlg::threadJkUpdating()
{
	//UpdateJkReport();
	UpdateJkPrice();
}

UINT MyUpdataJkThreadProc(LPVOID pParam)
{
	CZoTradeDlg *p = (CZoTradeDlg*)pParam;
	g_clsCriticalSection.Lock();
	p->threadJkUpdating();
	g_clsCriticalSection.Unlock();

	return 0;
}

void CZoTradeDlg::threadSelling()
{
	m_trade.SellByZhiSunRate();

	if (m_zyByToday)
      m_trade.SellByTodayZyRate();
	else
	   m_trade.SellByTotalZyRate();
}

UINT MySellThreadProc(LPVOID pParam)
{
	CZoTradeDlg *p = (CZoTradeDlg*)pParam;
	g_clsCriticalSection.Lock();
	p->threadSelling();
	g_clsCriticalSection.Unlock();

	return 0;
}

void CZoTradeDlg::threadBuying()
{
	m_trade.BuyByDownRate();
}

UINT MyBuyThreadProc(LPVOID pParam)
{
	CZoTradeDlg *p = (CZoTradeDlg*)pParam;
	g_clsCriticalSection.Lock();
	p->threadBuying();
	g_clsCriticalSection.Unlock();

	return 0;
}

void CZoTradeDlg::threadZjShszUpdating()
{
	m_trade.GetZqZj();
	m_bar_kyzj = m_trade.GetZqZjStr();
	//m_itemSize.m_StatusBar.SetPaneText(m_itemSize.index2, m_bar_kyzj);
	::SendMessageA(m_itemSize.m_StatusBar.m_hWnd, SB_SETTEXT, m_itemSize.index2, (LPARAM)LPCTSTR(m_bar_kyzj));

	m_bar_shsz = m_trade.GetShszStr();
	//m_itemSize.m_StatusBar.SetPaneText(m_itemSize.index1, m_bar_shsz);
	::SendMessageA(m_itemSize.m_StatusBar.m_hWnd, SB_SETTEXT, m_itemSize.index1, (LPARAM)LPCTSTR(m_bar_shsz));
}

UINT MyUpdateZjShsThreadProc(LPVOID pParam)
{
	CZoTradeDlg *p = (CZoTradeDlg*)pParam;
	g_clsCriticalSection.Lock();
	p->threadZjShszUpdating();
	g_clsCriticalSection.Unlock();

	return 0;
}

void CZoTradeDlg::threadTimeUpdating()
{
	m_bar_clock = GetTimeInfo();
	//TRACE("threadTimeUpdating  %d %s\n", m_itemSize.index4, m_bar_clock);
	//m_itemSize.m_StatusBar.SetPaneText(m_itemSize.index4, m_bar_clock);

	::SendMessageA(m_itemSize.m_StatusBar.m_hWnd, SB_SETTEXT, m_itemSize.index4, (LPARAM)LPCTSTR(m_bar_clock));
}

UINT MyUpdateTimeThreadProc(LPVOID pParam)
{
	CZoTradeDlg *p = (CZoTradeDlg*)pParam;
	g_clsCriticalSection.Lock();
	p->threadTimeUpdating();
	g_clsCriticalSection.Unlock();

	return 0;
}

void CZoTradeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default	
	switch (nIDEvent)
	{
	case TIMER_TIME:
		//������������ 
		//������Ҫ��ʱ���ڴ˴�����KillTimer(1); 
		AfxBeginThread((AFX_THREADPROC)MyUpdateTimeThreadProc, (LPVOID)this);
		break;
	case TIMER_UPDATE_ZJSHSZ:
		AfxBeginThread((AFX_THREADPROC)MyUpdateZjShsThreadProc, (LPVOID)this);
		break;
	case TIMER_UPDATE_STOCK:
		AfxBeginThread((AFX_THREADPROC)MyUpdataThreadProc, (LPVOID)this);
		break;
	case TIMER_UPDATE_CHENGJIAO:
		AfxBeginThread((AFX_THREADPROC)MyUpdataCjThreadProc, (LPVOID)this);
		break;
	case TIMER_UPDATE_JIANKONG:
		AfxBeginThread((AFX_THREADPROC)MyUpdataJkThreadProc, (LPVOID)this);
		break;
	case TIMER_UPDATE_SELL:
		AfxBeginThread((AFX_THREADPROC)MySellThreadProc, (LPVOID)this);
		break;
	case TIMER_UPDATE_BUY:
		AfxBeginThread((AFX_THREADPROC)MyBuyThreadProc, (LPVOID)this);
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CZoTradeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rectDlg, rectBar;
	GetClientRect(rectDlg);

	if (m_itemSize.m_StatusBar && (0 != rectDlg.Width()))
	{
		m_itemSize.ResizeStatusBar(this, cx, cy);		
	}

	if (m_list && (0 != rectDlg.Width()))
	{
		m_itemSize.ResizeItem();
	}
	// TODO: Add your message handler code here
}

CString CZoTradeDlg::GetTimeInfo()
{
	CTime time = CTime::GetCurrentTime();
	CString s = time.Format("%H:%M:%S");
	CString strweek;
	switch (time.GetDayOfWeek())
	{
	case 1: strweek = _T("������"); break;
	case 2: strweek = _T("����һ"); break;
	case 3: strweek = _T("���ڶ�"); break;
	case 4: strweek = _T("������"); break;
	case 5: strweek = _T("������"); break;
	case 6: strweek = _T("������"); break;
	case 7: strweek = _T("������"); break;
	default: TRACE(_T("Error")); break;
	}
	//time.GetHour() = 0 ��ʾ�賿12;00 ��12:59
	//time.GetHour() = 4 ��ʾ�賿4��00 �� 4��59
	CString strtime;
	if ((time.GetHour() >= 0) && (time.GetHour() <= 4))
	{
		strtime = _T("�賿");
	}
	else if ((time.GetHour()>4) && (time.GetHour() <= 8))
	{
		strtime = _T("����");
	}
	else if ((time.GetHour()>8) && (time.GetHour() <= 11))
	{
		strtime = _T("����");
	}
	else if ((time.GetHour()>11) && (time.GetHour() <= 13))
	{
		strtime = _T("����");
	}
	else if ((time.GetHour()>13) && (time.GetHour() <= 17))
	{
		strtime = _T("����");
	}
	else
	{
		strtime = _T("����");
	}

	CString clock = strweek + "  " + strtime + " " + s;
	return clock;
}

void CZoTradeDlg::OnNMClickStockList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	//TRACE("iItem��%d��iSubItem��%d\n", pEditCtrl->iItem, pEditCtrl->iSubItem);
	if (pEditCtrl->iItem == -1)//������ǹ�����  
	{
		if (haveeditcreate == true)//���֮ǰ�����˱༭������ٵ�  
		{
			distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);//���ٵ�Ԫ��༭�����  
			haveeditcreate = false;
		}
		if (haveccomboboxcreate == true)//���֮ǰ�����������б������ٵ�  
		{
			distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
			haveccomboboxcreate = false;
		}
	}
	else if (pEditCtrl->iSubItem == 9 || pEditCtrl->iSubItem == 10 || pEditCtrl->iSubItem == 11)//��������Ա�ѡ��  
	{
		if (haveccomboboxcreate == true)//���֮ǰ�����˱༭������ٵ�  
		{
			distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
			haveccomboboxcreate = false;
		}
		if (haveeditcreate == true)
		{
			if (!(e_Item == pEditCtrl->iItem && e_SubItem == pEditCtrl->iSubItem))//������еĵ�Ԫ����֮ǰ�����õ�  
			{
				distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);
				haveeditcreate = false;
				createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//�����༭�� 
			
			}
			else//���еĵ�Ԫ����֮ǰ�����õ�  
			{
				
				m_Edit.SetFocus();//����Ϊ����   
				//m_Edit.SetSel(0, -1);

			}
		}
		else
		{
			e_Item = pEditCtrl->iItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���  
			e_SubItem = pEditCtrl->iSubItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���  
			createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//�����༭��  
		}
	}
	else if (pEditCtrl->iSubItem >= 12)//������Ա�ѡ��ڵ�Ԫ�����������б���  
	{
		if (haveeditcreate == true)//���֮ǰ�����˱༭������ٵ�  
		{
			distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);
			haveeditcreate = false;
		}
		if (haveccomboboxcreate == true)
		{
			if (!(e_Item == pEditCtrl->iItem && e_SubItem == pEditCtrl->iSubItem))//������еĵ�Ԫ����֮ǰ�����õ�  
			{
				distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
				haveccomboboxcreate = false;
				createCcombobox(pEditCtrl, &m_comBox, e_Item, e_SubItem, haveccomboboxcreate);//�����༭��  
				m_comBox.AddString(_T("��"));
				m_comBox.AddString(_T("Ů"));
				m_comBox.ShowDropDown();//�Զ�����  
			}
			else//���еĵ�Ԫ����֮ǰ�����õ�  
			{
				m_comBox.SetFocus();//����Ϊ����   
			}
		}
		else
		{
			e_Item = pEditCtrl->iItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���  
			e_SubItem = pEditCtrl->iSubItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���  
			createCcombobox(pEditCtrl, &m_comBox, e_Item, e_SubItem, haveccomboboxcreate);//�����༭��  
			m_comBox.AddString(_T("��"));
			m_comBox.AddString(_T("Ů"));
			m_comBox.ShowDropDown();//�Զ�����  
		}
	}
	*pResult = 0;
}

void CZoTradeDlg::JkOnNMClickStockList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	//printf("�У�%d���У�%d\n", pEditCtrl->iItem, pEditCtrl->iSubItem);
	if (pEditCtrl->iItem == -1)//������ǹ�����  
	{
		if (jk_haveeditcreate == true)//���֮ǰ�����˱༭������ٵ�  
		{
			distroyEdit(&m_JKList, &jk_Edit, jk_Item, jk_SubItem);//���ٵ�Ԫ��༭�����  
			jk_haveeditcreate = false;
		}
	}
	else if (pEditCtrl->iSubItem == 4 || pEditCtrl->iSubItem == 5 || pEditCtrl->iSubItem == 6)//��������Ա�ѡ��  
	{
		if (jk_haveeditcreate == true)
		{
			if (!(jk_Item == pEditCtrl->iItem && jk_SubItem == pEditCtrl->iSubItem))//������еĵ�Ԫ����֮ǰ�����õ�  
			{
				distroyEdit(&m_JKList, &jk_Edit, jk_Item, jk_SubItem);
				jk_haveeditcreate = false;
				jk_createEdit(pEditCtrl, &jk_Edit, jk_Item, jk_SubItem, jk_haveeditcreate);//�����༭��  
			}
			else//���еĵ�Ԫ����֮ǰ�����õ�  
			{
				jk_Edit.SetFocus();//����Ϊ����   
			}
		}
		else
		{
			jk_Item = pEditCtrl->iItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���  
			jk_SubItem = pEditCtrl->iSubItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���  
			jk_createEdit(pEditCtrl, &jk_Edit, jk_Item, jk_SubItem, jk_haveeditcreate);//�����༭��  
		}
	}
	*pResult = 0;
}

void CZoTradeDlg::createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat)//������Ԫ��༭����  
//pEditCtrlΪ�б����ָ�룬createditΪ�༭��ָ�����  
//ItemΪ������Ԫ�����б��е��У�SubItem��Ϊ�У�havecreatΪ���󴴽���׼  
{
	Item = pEditCtrl->iItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���  
	SubItem = pEditCtrl->iSubItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���  
	createdit->Create(ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN,
		CRect(0, 0, 0, 0), this, IDC_EDIT_CREATEID);//�����༭�����,IDC_EDIT_CREATEIDΪ�ؼ�ID��3000�������¿�ͷ����  
	havecreat = true;
	createdit->SetFont(this->GetFont(), FALSE);//��������,����������Ļ�������ֻ��ͻأ�ĸо�  
	createdit->SetParent(&m_list);//��list control����Ϊ������,���ɵ�Edit������ȷ��λ,���Ҳ����Ҫ  
	CRect  EditRect;
	m_list.GetSubItemRect(e_Item, e_SubItem, LVIR_LABEL, EditRect);//��ȡ��Ԫ��Ŀռ�λ����Ϣ  
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_list.GetColumnWidth(e_SubItem) - 1, EditRect.bottom - 1);//+1��-1�����ñ༭�����ڵ�ס�б���е�������  
	CString strItem = m_list.GetItemText(e_Item, e_SubItem);//�����Ӧ��Ԫ���ַ�  
	createdit->SetWindowText(strItem);//����Ԫ���ַ���ʾ�ڱ༭����  
	createdit->MoveWindow(&EditRect);//���༭��λ�÷�����Ӧ��Ԫ����  
	createdit->ShowWindow(SW_SHOW);//��ʾ�༭���ڵ�Ԫ������  
	createdit->SetFocus();//����Ϊ����   
	createdit->SetSel(-1);//���ù�����ı������ֵ����  
}

void CZoTradeDlg::jk_createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat)//������Ԫ��༭����  
//pEditCtrlΪ�б����ָ�룬createditΪ�༭��ָ�����  
//ItemΪ������Ԫ�����б��е��У�SubItem��Ϊ�У�havecreatΪ���󴴽���׼  
{
	Item = pEditCtrl->iItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���  
	SubItem = pEditCtrl->iSubItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���  
	createdit->Create(ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN,
		CRect(0, 0, 0, 0), this, IDC_EDIT_CREATEID_JK);//�����༭�����,IDC_EDIT_CREATEIDΪ�ؼ�ID��3000�������¿�ͷ����  
	havecreat = true;
	createdit->SetFont(this->GetFont(), FALSE);//��������,����������Ļ�������ֻ��ͻأ�ĸо�  
	createdit->SetParent(&m_JKList);//��list control����Ϊ������,���ɵ�Edit������ȷ��λ,���Ҳ����Ҫ  
	CRect  EditRect;
	m_JKList.GetSubItemRect(jk_Item, jk_SubItem, LVIR_LABEL, EditRect);//��ȡ��Ԫ��Ŀռ�λ����Ϣ  
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_JKList.GetColumnWidth(jk_SubItem) - 1, EditRect.bottom - 1);//+1��-1�����ñ༭�����ڵ�ס�б���е�������  
	CString strItem = m_JKList.GetItemText(jk_Item, jk_SubItem);//�����Ӧ��Ԫ���ַ�  
	createdit->SetWindowText(strItem);//����Ԫ���ַ���ʾ�ڱ༭����  
	createdit->MoveWindow(&EditRect);//���༭��λ�÷�����Ӧ��Ԫ����  
	createdit->ShowWindow(SW_SHOW);//��ʾ�༭���ڵ�Ԫ������  
	createdit->SetFocus();//����Ϊ����   
	createdit->SetSel(-1);//���ù�����ı������ֵ����  
}

void CZoTradeDlg::distroyEdit(CListCtrl *list, CEdit* distroyedit, int &Item, int &SubItem)
{
	CString meditdata;
	distroyedit->GetWindowText(meditdata);
	list->SetItemText(Item, SubItem, meditdata);//�����Ӧ��Ԫ���ַ�  
	distroyedit->DestroyWindow();//���ٶ����д�����Ҫ�����٣���Ȼ�ᱨ��  
}

void CZoTradeDlg::createCcombobox(NM_LISTVIEW  *pEditCtrl, CComboBox *createccomboboxobj, int &Item, int &SubItem, bool &havecreat)//������Ԫ�������б����  
//pEditCtrlΪ�б����ָ�룬createccomboboxΪ�����б��ָ�����  
//ItemΪ������Ԫ�����б��е��У�SubItem��Ϊ�У�havecreatΪ���󴴽���׼  
{
	Item = pEditCtrl->iItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���  
	SubItem = pEditCtrl->iSubItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���  
	havecreat = true;
	createccomboboxobj->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_OEMCONVERT, CRect(0, 0, 0, 0), this, IDC_COMBOX_CREATEID);
	createccomboboxobj->SetFont(this->GetFont(), FALSE);//��������,����������Ļ�������ֻ��ͻأ�ĸо�  
	createccomboboxobj->SetParent(&m_list);//��list control����Ϊ������,���ɵ�Ccombobox������ȷ��λ,���Ҳ����Ҫ  
	CRect  EditRect;
	m_list.GetSubItemRect(e_Item, e_SubItem, LVIR_LABEL, EditRect);//��ȡ��Ԫ��Ŀռ�λ����Ϣ  
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_list.GetColumnWidth(e_SubItem) - 1, EditRect.bottom - 1);//+1��-1�����ñ༭�����ڵ�ס�б���е�������  
	CString strItem = m_list.GetItemText(e_Item, e_SubItem);//�����Ӧ��Ԫ���ַ�  
	createccomboboxobj->SetWindowText(strItem);//����Ԫ���ַ���ʾ�ڱ༭����  
	createccomboboxobj->MoveWindow(&EditRect);//���༭��λ�÷�����Ӧ��Ԫ����  
	createccomboboxobj->ShowWindow(SW_SHOW);//��ʾ�༭���ڵ�Ԫ������  
}

void CZoTradeDlg::distroyCcombobox(CListCtrl *list, CComboBox* distroyccomboboxobj, int &Item, int &SubItem)
{
	CString meditdata;
	distroyccomboboxobj->GetWindowText(meditdata);
	list->SetItemText(Item, SubItem, meditdata);//������Ӧ��Ԫ���ַ�  
	distroyccomboboxobj->DestroyWindow();//���ٶ����д�����Ҫ�����٣���Ȼ�ᱨ��  
}

void CZoTradeDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}

void CZoTradeDlg::threadEditZyZs()
{
	CString zZqdm;
	int i;
	bool zFlag = FALSE;
	CString t_value;

	zZqdm = m_list.GetItemText(e_Item, 0);
	TRACE("item[%d,%d]:= %s %s\n", e_Item, e_SubItem, zZqdm, m_list.GetItemText(e_Item, e_SubItem));

	//if (m_list.GetItemText(e_Item, e_SubItem).IsEmpty()) return;

	for (i = 0; i < m_trade.m_zyzs_num; i++)
	{
		if (m_trade.zz_report[i].zCode == zZqdm)
		{
			AfxExtractSubString(t_value, (LPCTSTR)m_list.GetItemText(e_Item, e_SubItem), 0, '|');
			if (e_SubItem == 9) m_trade.zz_report[i].zZhiYingRate = t_value;
			if (e_SubItem == 10) m_trade.zz_report[i].zZyHlRate = t_value;
			if (e_SubItem == 11) m_trade.zz_report[i].zZhiSunRate = t_value;
			//TRACE("%d %d %s ---\n", e_Item, e_SubItem, m_trade.zz_report[i].zZhiYingRate);
			zFlag = TRUE;
			break;
		}
	}

	if(!zFlag) {
		m_trade.zz_report[m_trade.m_zyzs_num].zCode = m_list.GetItemText(e_Item, 0);
		AfxExtractSubString(t_value, (LPCTSTR)m_list.GetItemText(e_Item, e_SubItem), 0, '|');

		if (e_SubItem == 9) m_trade.zz_report[m_trade.m_zyzs_num].zZhiYingRate = t_value;
		if (e_SubItem == 10) m_trade.zz_report[m_trade.m_zyzs_num].zZyHlRate = t_value;
		if (e_SubItem == 11) m_trade.zz_report[m_trade.m_zyzs_num].zZhiSunRate = t_value;
		m_trade.m_zyzs_num += 1;
	}

	m_trade.writeZyZsIni();
}

UINT MyEditZyZsThreadProc(LPVOID pParam)
{
	CZoTradeDlg *p = (CZoTradeDlg*)pParam;
	g_clsCriticalSection.Lock();
	p->threadEditZyZs();
	g_clsCriticalSection.Unlock();

	return 0;
}

void CZoTradeDlg::threadEditJK()
{
	CString zZqdm;
	int i;
	bool zFlag = FALSE;
	CString t_value;

	zZqdm = m_JKList.GetItemText(jk_Item, 0);
	TRACE("item[%d,%d]:= %s %s\n", jk_Item, jk_SubItem, zZqdm, m_JKList.GetItemText(jk_Item, jk_SubItem));

	for (i = 0; i < m_trade.m_jk_num; i++)
	{
		if (m_trade.jk_report[i].zCode == zZqdm)
		{
			AfxExtractSubString(t_value, (LPCTSTR)m_JKList.GetItemText(jk_Item, jk_SubItem), 0, '|');
			if (jk_SubItem == 4) m_trade.jk_report[i].zBuyNum = t_value;
			if (jk_SubItem == 5) m_trade.jk_report[i].zXiaDieRate = t_value;
			if (jk_SubItem == 6) m_trade.jk_report[i].zXdHtRate = t_value;
			zFlag = TRUE;
		}
	}

	if (!zFlag) {
		m_trade.jk_report[m_trade.m_jk_num].zCode = m_JKList.GetItemText(jk_Item, 0);
		AfxExtractSubString(t_value, (LPCTSTR)m_JKList.GetItemText(jk_Item, jk_SubItem), 0, '|');

		if (jk_SubItem == 4) m_trade.jk_report[m_trade.m_jk_num].zBuyNum = t_value;
		if (jk_SubItem == 5) m_trade.jk_report[m_trade.m_jk_num].zXiaDieRate = t_value;
		if (jk_SubItem == 6) m_trade.jk_report[m_trade.m_jk_num].zXdHtRate = t_value;
		m_trade.m_jk_num += 1;
	}

	m_trade.writeJKIni();

	UpdateJkReport();
}

UINT MyEditJkThreadProc(LPVOID pParam)
{
	CZoTradeDlg *p = (CZoTradeDlg*)pParam;
	g_clsCriticalSection.Lock();
	p->threadEditJK();
	g_clsCriticalSection.Unlock();

	return 0;
}

void CZoTradeDlg::OnKillfocusEdit()
{
	//TRACE("OnKillfocusEdit\n");
	if (haveeditcreate == true)//���֮ǰ�����˱༭������ٵ�  
	{
		distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);//���ٵ�Ԫ��༭�����  
		haveeditcreate = false;
		MSG msg;
		GetMessage(&msg, NULL, 0, 0);
		TRACE("OnKillfocusEdit -- %d\n", msg.message);
		if (msg.message != WA_INACTIVE)
		{
			//TRACE("OnKillfocusEdit  m_list null\n");
			AfxBeginThread((AFX_THREADPROC)MyEditZyZsThreadProc, (LPVOID)this);
		}

	}
	if (haveccomboboxcreate == true)//���֮ǰ�����������б������ٵ�  
	{
		distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
		haveccomboboxcreate = false;
	}
}

void CZoTradeDlg::JkOnKillfocusEdit()
{
	if (jk_haveeditcreate == true)//���֮ǰ�����˱༭������ٵ�  
	{
		distroyEdit(&m_JKList, &jk_Edit, jk_Item, jk_SubItem);//���ٵ�Ԫ��༭�����  
		jk_haveeditcreate = false;
		AfxBeginThread((AFX_THREADPROC)MyEditJkThreadProc, (LPVOID)this);
	}
}

void CZoTradeDlg::OnKillfocusCcomboBox()
{
	if (haveeditcreate == true)//���֮ǰ�����˱༭������ٵ�  
	{
		distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);//���ٵ�Ԫ��༭�����  
		haveeditcreate = false;
	}
	if (haveccomboboxcreate == true)//���֮ǰ�����������б������ٵ�  
	{
		distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
		haveccomboboxcreate = false;
	}
}
void CZoTradeDlg::OpenDlgJk()
{
	Dlg.DoModal();
}

UINT DlgAddJkThreadProc(LPVOID pParam)
{
	CZoTradeDlg *p = (CZoTradeDlg*)pParam;
	g_clsCriticalSection.Lock();
	p->OpenDlgJk();
	g_clsCriticalSection.Unlock();

	return 0;
}

void CZoTradeDlg::OnBnClickedBtnAddJk()
{
	//AfxBeginThread((AFX_THREADPROC)DlgAddJkThreadProc, (LPVOID)this);
	Dlg.DoModal();
}


void CZoTradeDlg::threadDelJkItem()
{
	CString zZqdm;
	int i, j, k;
	bool zFlag = FALSE;
	CString t_value;

	for (k = jk_del_num - 1; k >= 0; k--)
	{
		zZqdm = m_JKList.GetItemText(jk_del_index[k], 0);
		TRACE("item[%d]:= del %s\n", jk_del_index[k], zZqdm);

		for (i = 0; i < m_trade.m_jk_num; i++)
		{
			if (m_trade.jk_report[i].zCode == zZqdm)
			{
				//m_trade.jk_report[i].zCode.Empty();
				//m_trade.jk_report[i].zName.Empty();
				//m_trade.jk_report[i].zBuyNum.Empty();
				//m_trade.jk_report[i].zXiaDieRate.Empty();
				//m_trade.jk_report[i].zXdHtRate.Empty();
				//m_trade.m_jk_num -= 1;
				for (j = i; j < m_trade.m_jk_num; j++)
				{
					m_trade.jk_report[j].zCode = m_trade.jk_report[j + 1].zCode;
					m_trade.jk_report[j].zName = m_trade.jk_report[j + 1].zName;
					m_trade.jk_report[j].zBuyNum = m_trade.jk_report[j + 1].zBuyNum;
					m_trade.jk_report[j].zXiaDieRate = m_trade.jk_report[j + 1].zXiaDieRate;
					m_trade.jk_report[j].zXdHtRate = m_trade.jk_report[j + 1].zXdHtRate;
				}
				m_trade.m_jk_num -= 1;
			}
		}
	}
	m_trade.writeJKIni();
	UpdateJkReport();
}

UINT MyDelJkThreadProc(LPVOID pParam)
{
	CZoTradeDlg *p = (CZoTradeDlg*)pParam;
	g_clsCriticalSection.Lock();
	p->threadDelJkItem();
	g_clsCriticalSection.Unlock();

	return 0;
}

void CZoTradeDlg::OnBnClickedBtnDelJk()
{
	int i;
	jk_del_num = 0;
	for (i = 0; i<m_JKList.GetItemCount(); i++)
	{
		if (m_JKList.GetItemState(i, LVNI_ALL | LVNI_SELECTED) == LVIS_SELECTED)
		{
			TRACE("Delete item %d\n",i);
			jk_del_index[jk_del_num++] = i;
			AfxBeginThread((AFX_THREADPROC)MyDelJkThreadProc, (LPVOID)this);
		}
	}
}

void CZoTradeDlg::addMainJkItem()
{
	int i;
	bool isExsit = FALSE;
	//TRACE("ssssss  %d \n", m_trade.m_jk_num);
	for (i = 0; i < m_trade.m_jk_num; i++)
	{
		if (m_trade.jk_report[i].zCode == m_add_jk.zCode)
		{
			isExsit = TRUE;
			break;
		}
	}

	if (!isExsit)
	{
		m_trade.jk_report[m_trade.m_jk_num].zCode = m_add_jk.zCode;
		m_trade.jk_report[m_trade.m_jk_num].zName = m_add_jk.zName;
		m_trade.jk_report[m_trade.m_jk_num].zBuyNum = m_add_jk.zBuyNum;
		m_trade.jk_report[m_trade.m_jk_num].zXiaDieRate = m_add_jk.zXiaDieRate;
		m_trade.jk_report[m_trade.m_jk_num].zXdHtRate = m_add_jk.zXdHtRate;

		m_trade.m_jk_num += 1;

		m_trade.writeJKIni();
		UpdateJkReport();
	}
}

UINT AddMainJkItemThreadProc(LPVOID pParam)
{
	CZoTradeDlg *p = (CZoTradeDlg*)pParam;
	g_clsCriticalSection.Lock();
	p->addMainJkItem();
	g_clsCriticalSection.Unlock();

	return 0;
}


LRESULT CZoTradeDlg::ReceiveMessage(WPARAM wParam, LPARAM lParam)
{
	int recvint = (int)wParam;
    
	switch (recvint)
	{
	case MSG_ADD_JK:
	{
		JKREPORT *jk;
		jk = (JKREPORT *)lParam;
		//�Զ�����Ϣ����recvint��recvstrΪ���ݹ�����ֵ���ɸ�����Ҫ����ѡ����
		m_add_jk = *jk;
		TRACE("ReceiveMessage %s %s\n", m_add_jk.zCode, m_add_jk.zName);

		AfxBeginThread((AFX_THREADPROC)AddMainJkItemThreadProc, (LPVOID)this);
		break;
     }
	case MSG_DOING:
		
		break;
	default:
		break;

	}
   return true;
}
