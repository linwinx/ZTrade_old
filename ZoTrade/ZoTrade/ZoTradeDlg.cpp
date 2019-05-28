
// ZoTradeDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CZoTradeDlg 对话框



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
	ON_EN_KILLFOCUS(IDC_EDIT_CREATEID, &CZoTradeDlg::OnKillfocusEdit)//添加动态生成编辑框的失去焦点响应函数  
	ON_EN_KILLFOCUS(IDC_EDIT_CREATEID_JK, &CZoTradeDlg::JkOnKillfocusEdit)
	ON_CBN_KILLFOCUS(IDC_COMBOX_CREATEID, &CZoTradeDlg::OnKillfocusCcomboBox)
	ON_BN_CLICKED(IDC_BTN_ADD_JK, &CZoTradeDlg::OnBnClickedBtnAddJk)
	ON_BN_CLICKED(IDC_BTN_DEL_JK, &CZoTradeDlg::OnBnClickedBtnDelJk)
	ON_MESSAGE(WM_USERMESSAGE, &CZoTradeDlg::ReceiveMessage)   // 添加自定义消息映射宏，其他代码行为程序原有 
	ON_WM_CLOSE()
END_MESSAGE_MAP()

CZoTradeDlg::~CZoTradeDlg()
{
	m_trade.Logout();
}

BOOL CZoTradeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	m_itemSize.AddStatusBar(this);
	m_itemSize.AddItemRect(IDC_STOCK_LIST, this);

	//设置定时器
	SetTimer(TIMER_TIME, TIMER_TIME_INTERVAL, NULL);
	SetTimer(TIMER_UPDATE_STOCK, TIMER_UPDATE_STOCK_INTERVAL, NULL); 
	SetTimer(TIMER_UPDATE_SELL, TIMER_UPDATE_SELL_INTERVAL, NULL);
	SetTimer(TIMER_UPDATE_CHENGJIAO, TIMER_UPDATE_CHENGJIAO_INTERVAL, NULL);
	SetTimer(TIMER_UPDATE_JIANKONG, TIMER_UPDATE_JIANKONG_INTERVAL, NULL);
	SetTimer(TIMER_UPDATE_ZJSHSZ, TIMER_UPDATE_ZJSHSZ_INTERVAL, NULL);

	haveeditcreate = false;//初始化标志位，表示还没有创建编辑框  
	haveccomboboxcreate = false;//初始化标志位，表示还没有创建下拉列表框  
	jk_haveeditcreate = false;//初始化标志位，表示还没有创建编辑框  

	RECT  m_rect;
	m_list.GetClientRect(&m_rect); //获取list的客户区,方便调节每一列的宽度    
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //设置list风格，LVS_EX_GRIDLINES为网格线（只适用与report风格的listctrl）  
	//LVS_EX_FULLROWSELECT为选中某行使整行高亮（只适用与report风格的listctrl）  
	m_list.InsertColumn(0, _T("代码"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum);
	m_list.InsertColumn(1, _T("名称"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 1.3);
	m_list.InsertColumn(2, _T("数量"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 0.9);
	m_list.InsertColumn(3, _T("可卖"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 0.9);
	m_list.InsertColumn(4, _T("成本价"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 0.9);
	m_list.InsertColumn(5, _T("当前价"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 0.9);
	m_list.InsertColumn(6, _T("市值"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum * 1.3);
	m_list.InsertColumn(7, _T("盈亏"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 1.2);
	m_list.InsertColumn(8, _T("盈亏%"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum);
	m_list.InsertColumn(9, _T("止盈%"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 1.15);
	m_list.InsertColumn(10, _T("回落%"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 1.15);
	m_list.InsertColumn(11, _T("止损%"), LVCFMT_LEFT, (m_rect.right - m_rect.left) / m_colnum* 1.15);

	RECT m_cjRect;
	m_CJList.GetClientRect(&m_cjRect); //获取list的客户区,方便调节每一列的宽度    
	m_CJList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_CJList.InsertColumn(0, _T("代码"), LVCFMT_LEFT, (m_cjRect.right - m_cjRect.left) / m_cjColnum);
	m_CJList.InsertColumn(1, _T("名称"), LVCFMT_LEFT, (m_cjRect.right - m_cjRect.left) / m_cjColnum *1.1);
	m_CJList.InsertColumn(2, _T("价格"), LVCFMT_LEFT, (m_cjRect.right - m_cjRect.left) / m_cjColnum);
	m_CJList.InsertColumn(3, _T("数量"), LVCFMT_LEFT, (m_cjRect.right - m_cjRect.left) / m_cjColnum *0.85);
	m_CJList.InsertColumn(4, _T("时间"), LVCFMT_LEFT, (m_cjRect.right - m_cjRect.left) / m_cjColnum*1.1);
	m_CJList.InsertColumn(5, _T("类型"), LVCFMT_LEFT, (m_cjRect.right - m_cjRect.left) / m_cjColnum*0.9);

	RECT m_jkRect;
	m_JKList.GetClientRect(&m_jkRect); //获取list的客户区,方便调节每一列的宽度    
	m_JKList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_JKList.InsertColumn(0, _T("代码"), LVCFMT_LEFT, (m_jkRect.right - m_jkRect.left) / m_jkColnum);
	m_JKList.InsertColumn(1, _T("名称"), LVCFMT_LEFT, (m_jkRect.right - m_jkRect.left) / m_jkColnum*1.1);
	m_JKList.InsertColumn(2, _T("昨收"), LVCFMT_LEFT, (m_jkRect.right - m_jkRect.left) / m_jkColnum*0.85);
	m_JKList.InsertColumn(3, _T("最新"), LVCFMT_LEFT, (m_jkRect.right - m_jkRect.left) / m_jkColnum*0.85);
	m_JKList.InsertColumn(4, _T("数量"), LVCFMT_LEFT, (m_jkRect.right - m_jkRect.left) / m_jkColnum*0.85);
	m_JKList.InsertColumn(5, _T("下跌%"), LVCFMT_LEFT, (m_jkRect.right - m_jkRect.left) / m_jkColnum*1.15);
	m_JKList.InsertColumn(6, _T("回调%"), LVCFMT_LEFT, (m_jkRect.right - m_jkRect.left) / m_jkColnum*1.15);


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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CZoTradeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
		//做该做的事情 
		//当不需要的时候在此处调用KillTimer(1); 
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
	case 1: strweek = _T("星期天"); break;
	case 2: strweek = _T("星期一"); break;
	case 3: strweek = _T("星期二"); break;
	case 4: strweek = _T("星期三"); break;
	case 5: strweek = _T("星期四"); break;
	case 6: strweek = _T("星期五"); break;
	case 7: strweek = _T("星期六"); break;
	default: TRACE(_T("Error")); break;
	}
	//time.GetHour() = 0 表示凌晨12;00 到12:59
	//time.GetHour() = 4 表示凌晨4：00 到 4：59
	CString strtime;
	if ((time.GetHour() >= 0) && (time.GetHour() <= 4))
	{
		strtime = _T("凌晨");
	}
	else if ((time.GetHour()>4) && (time.GetHour() <= 8))
	{
		strtime = _T("早上");
	}
	else if ((time.GetHour()>8) && (time.GetHour() <= 11))
	{
		strtime = _T("上午");
	}
	else if ((time.GetHour()>11) && (time.GetHour() <= 13))
	{
		strtime = _T("中午");
	}
	else if ((time.GetHour()>13) && (time.GetHour() <= 17))
	{
		strtime = _T("下午");
	}
	else
	{
		strtime = _T("晚上");
	}

	CString clock = strweek + "  " + strtime + " " + s;
	return clock;
}

void CZoTradeDlg::OnNMClickStockList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	//TRACE("iItem：%d，iSubItem：%d\n", pEditCtrl->iItem, pEditCtrl->iSubItem);
	if (pEditCtrl->iItem == -1)//点击到非工作区  
	{
		if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉  
		{
			distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象  
			haveeditcreate = false;
		}
		if (haveccomboboxcreate == true)//如果之前创建了下拉列表框就销毁掉  
		{
			distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
			haveccomboboxcreate = false;
		}
	}
	else if (pEditCtrl->iSubItem == 9 || pEditCtrl->iSubItem == 10 || pEditCtrl->iSubItem == 11)//如果不是性别选项  
	{
		if (haveccomboboxcreate == true)//如果之前创建了编辑框就销毁掉  
		{
			distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
			haveccomboboxcreate = false;
		}
		if (haveeditcreate == true)
		{
			if (!(e_Item == pEditCtrl->iItem && e_SubItem == pEditCtrl->iSubItem))//如果点中的单元格不是之前创建好的  
			{
				distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);
				haveeditcreate = false;
				createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//创建编辑框 
			
			}
			else//点中的单元格是之前创建好的  
			{
				
				m_Edit.SetFocus();//设置为焦点   
				//m_Edit.SetSel(0, -1);

			}
		}
		else
		{
			e_Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理  
			e_SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理  
			createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//创建编辑框  
		}
	}
	else if (pEditCtrl->iSubItem >= 12)//如果是性别选项，在单元格处生成下拉列表项  
	{
		if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉  
		{
			distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);
			haveeditcreate = false;
		}
		if (haveccomboboxcreate == true)
		{
			if (!(e_Item == pEditCtrl->iItem && e_SubItem == pEditCtrl->iSubItem))//如果点中的单元格不是之前创建好的  
			{
				distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
				haveccomboboxcreate = false;
				createCcombobox(pEditCtrl, &m_comBox, e_Item, e_SubItem, haveccomboboxcreate);//创建编辑框  
				m_comBox.AddString(_T("男"));
				m_comBox.AddString(_T("女"));
				m_comBox.ShowDropDown();//自动下拉  
			}
			else//点中的单元格是之前创建好的  
			{
				m_comBox.SetFocus();//设置为焦点   
			}
		}
		else
		{
			e_Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理  
			e_SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理  
			createCcombobox(pEditCtrl, &m_comBox, e_Item, e_SubItem, haveccomboboxcreate);//创建编辑框  
			m_comBox.AddString(_T("男"));
			m_comBox.AddString(_T("女"));
			m_comBox.ShowDropDown();//自动下拉  
		}
	}
	*pResult = 0;
}

void CZoTradeDlg::JkOnNMClickStockList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	//printf("行：%d，列：%d\n", pEditCtrl->iItem, pEditCtrl->iSubItem);
	if (pEditCtrl->iItem == -1)//点击到非工作区  
	{
		if (jk_haveeditcreate == true)//如果之前创建了编辑框就销毁掉  
		{
			distroyEdit(&m_JKList, &jk_Edit, jk_Item, jk_SubItem);//销毁单元格编辑框对象  
			jk_haveeditcreate = false;
		}
	}
	else if (pEditCtrl->iSubItem == 4 || pEditCtrl->iSubItem == 5 || pEditCtrl->iSubItem == 6)//如果不是性别选项  
	{
		if (jk_haveeditcreate == true)
		{
			if (!(jk_Item == pEditCtrl->iItem && jk_SubItem == pEditCtrl->iSubItem))//如果点中的单元格不是之前创建好的  
			{
				distroyEdit(&m_JKList, &jk_Edit, jk_Item, jk_SubItem);
				jk_haveeditcreate = false;
				jk_createEdit(pEditCtrl, &jk_Edit, jk_Item, jk_SubItem, jk_haveeditcreate);//创建编辑框  
			}
			else//点中的单元格是之前创建好的  
			{
				jk_Edit.SetFocus();//设置为焦点   
			}
		}
		else
		{
			jk_Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理  
			jk_SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理  
			jk_createEdit(pEditCtrl, &jk_Edit, jk_Item, jk_SubItem, jk_haveeditcreate);//创建编辑框  
		}
	}
	*pResult = 0;
}

void CZoTradeDlg::createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat)//创建单元格编辑框函数  
//pEditCtrl为列表对象指针，createdit为编辑框指针对象，  
//Item为创建单元格在列表中的行，SubItem则为列，havecreat为对象创建标准  
{
	Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理  
	SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理  
	createdit->Create(ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN,
		CRect(0, 0, 0, 0), this, IDC_EDIT_CREATEID);//创建编辑框对象,IDC_EDIT_CREATEID为控件ID号3000，在文章开头定义  
	havecreat = true;
	createdit->SetFont(this->GetFont(), FALSE);//设置字体,不设置这里的话上面的字会很突兀的感觉  
	createdit->SetParent(&m_list);//将list control设置为父窗口,生成的Edit才能正确定位,这个也很重要  
	CRect  EditRect;
	m_list.GetSubItemRect(e_Item, e_SubItem, LVIR_LABEL, EditRect);//获取单元格的空间位置信息  
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_list.GetColumnWidth(e_SubItem) - 1, EditRect.bottom - 1);//+1和-1可以让编辑框不至于挡住列表框中的网格线  
	CString strItem = m_list.GetItemText(e_Item, e_SubItem);//获得相应单元格字符  
	createdit->SetWindowText(strItem);//将单元格字符显示在编辑框上  
	createdit->MoveWindow(&EditRect);//将编辑框位置放在相应单元格上  
	createdit->ShowWindow(SW_SHOW);//显示编辑框在单元格上面  
	createdit->SetFocus();//设置为焦点   
	createdit->SetSel(-1);//设置光标在文本框文字的最后  
}

void CZoTradeDlg::jk_createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat)//创建单元格编辑框函数  
//pEditCtrl为列表对象指针，createdit为编辑框指针对象，  
//Item为创建单元格在列表中的行，SubItem则为列，havecreat为对象创建标准  
{
	Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理  
	SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理  
	createdit->Create(ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN,
		CRect(0, 0, 0, 0), this, IDC_EDIT_CREATEID_JK);//创建编辑框对象,IDC_EDIT_CREATEID为控件ID号3000，在文章开头定义  
	havecreat = true;
	createdit->SetFont(this->GetFont(), FALSE);//设置字体,不设置这里的话上面的字会很突兀的感觉  
	createdit->SetParent(&m_JKList);//将list control设置为父窗口,生成的Edit才能正确定位,这个也很重要  
	CRect  EditRect;
	m_JKList.GetSubItemRect(jk_Item, jk_SubItem, LVIR_LABEL, EditRect);//获取单元格的空间位置信息  
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_JKList.GetColumnWidth(jk_SubItem) - 1, EditRect.bottom - 1);//+1和-1可以让编辑框不至于挡住列表框中的网格线  
	CString strItem = m_JKList.GetItemText(jk_Item, jk_SubItem);//获得相应单元格字符  
	createdit->SetWindowText(strItem);//将单元格字符显示在编辑框上  
	createdit->MoveWindow(&EditRect);//将编辑框位置放在相应单元格上  
	createdit->ShowWindow(SW_SHOW);//显示编辑框在单元格上面  
	createdit->SetFocus();//设置为焦点   
	createdit->SetSel(-1);//设置光标在文本框文字的最后  
}

void CZoTradeDlg::distroyEdit(CListCtrl *list, CEdit* distroyedit, int &Item, int &SubItem)
{
	CString meditdata;
	distroyedit->GetWindowText(meditdata);
	list->SetItemText(Item, SubItem, meditdata);//获得相应单元格字符  
	distroyedit->DestroyWindow();//销毁对象，有创建就要有销毁，不然会报错  
}

void CZoTradeDlg::createCcombobox(NM_LISTVIEW  *pEditCtrl, CComboBox *createccomboboxobj, int &Item, int &SubItem, bool &havecreat)//创建单元格下拉列表框函数  
//pEditCtrl为列表对象指针，createccombobox为下拉列表框指针对象，  
//Item为创建单元格在列表中的行，SubItem则为列，havecreat为对象创建标准  
{
	Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理  
	SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理  
	havecreat = true;
	createccomboboxobj->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_OEMCONVERT, CRect(0, 0, 0, 0), this, IDC_COMBOX_CREATEID);
	createccomboboxobj->SetFont(this->GetFont(), FALSE);//设置字体,不设置这里的话上面的字会很突兀的感觉  
	createccomboboxobj->SetParent(&m_list);//将list control设置为父窗口,生成的Ccombobox才能正确定位,这个也很重要  
	CRect  EditRect;
	m_list.GetSubItemRect(e_Item, e_SubItem, LVIR_LABEL, EditRect);//获取单元格的空间位置信息  
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_list.GetColumnWidth(e_SubItem) - 1, EditRect.bottom - 1);//+1和-1可以让编辑框不至于挡住列表框中的网格线  
	CString strItem = m_list.GetItemText(e_Item, e_SubItem);//获得相应单元格字符  
	createccomboboxobj->SetWindowText(strItem);//将单元格字符显示在编辑框上  
	createccomboboxobj->MoveWindow(&EditRect);//将编辑框位置放在相应单元格上  
	createccomboboxobj->ShowWindow(SW_SHOW);//显示编辑框在单元格上面  
}

void CZoTradeDlg::distroyCcombobox(CListCtrl *list, CComboBox* distroyccomboboxobj, int &Item, int &SubItem)
{
	CString meditdata;
	distroyccomboboxobj->GetWindowText(meditdata);
	list->SetItemText(Item, SubItem, meditdata);//更新相应单元格字符  
	distroyccomboboxobj->DestroyWindow();//销毁对象，有创建就要有销毁，不然会报错  
}

void CZoTradeDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
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
	if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉  
	{
		distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象  
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
	if (haveccomboboxcreate == true)//如果之前创建了下拉列表框就销毁掉  
	{
		distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
		haveccomboboxcreate = false;
	}
}

void CZoTradeDlg::JkOnKillfocusEdit()
{
	if (jk_haveeditcreate == true)//如果之前创建了编辑框就销毁掉  
	{
		distroyEdit(&m_JKList, &jk_Edit, jk_Item, jk_SubItem);//销毁单元格编辑框对象  
		jk_haveeditcreate = false;
		AfxBeginThread((AFX_THREADPROC)MyEditJkThreadProc, (LPVOID)this);
	}
}

void CZoTradeDlg::OnKillfocusCcomboBox()
{
	if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉  
	{
		distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象  
		haveeditcreate = false;
	}
	if (haveccomboboxcreate == true)//如果之前创建了下拉列表框就销毁掉  
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
		//自定义消息处理，recvint和recvstr为传递过来的值，可根据需要进行选择处理
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
