
// ZoTradeDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "ItemResize.h"
#include "TradeApi.h"
#include "ADD_JK.h"

#define TIMER_TIME 110
#define TIMER_TIME_INTERVAL 500

#define TIMER_UPDATE_STOCK 111
#define TIMER_UPDATE_STOCK_INTERVAL 3000

#define TIMER_UPDATE_SELL 112
#define TIMER_UPDATE_SELL_INTERVAL 1500

#define TIMER_UPDATE_BUY 113
#define TIMER_UPDATE_BUY_INTERVAL 1500

#define TIMER_UPDATE_CHENGJIAO  114
#define TIMER_UPDATE_CHENGJIAO_INTERVAL 5000

#define TIMER_UPDATE_JIANKONG  115
#define TIMER_UPDATE_JIANKONG_INTERVAL 2000

#define TIMER_UPDATE_ZJSHSZ  116
#define TIMER_UPDATE_ZJSHSZ_INTERVAL 3000

#define WM_USERMESSAGE WM_USER+110 //WM_USERMESSAGE为自己定义的消息名称

// CZoTradeDlg 对话框
class CZoTradeDlg : public CDialogEx
{
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	//BOOL OnCloseDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

// 构造
public:
	CZoTradeDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CZoTradeDlg();
// 对话框数据
	enum { IDD = IDD_ZOTRADE_DIALOG };


// 实现
	CString str;


	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CString GetTimeInfo();


	CItemResize m_itemSize;

	CListCtrl m_list;
	CListCtrl m_CJList;
	CListCtrl m_JKList;


	int e_Item;    //刚编辑的行    
	int e_SubItem; //刚编辑的列    

	CEdit m_Edit;  //生成单元编辑框对象  
	bool haveeditcreate;//标志编辑框已经被创建  
	void createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat);//创建单元格编辑框函数  
	void distroyEdit(CListCtrl *list, CEdit* distroyedit, int &Item, int &SubItem);//销毁单元格编辑框对象  

	CComboBox m_comBox;//生产单元格下拉列表对象  
	bool haveccomboboxcreate;//标志下拉列表框已经被创建  
	void createCcombobox(NM_LISTVIEW  *pEditCtrl, CComboBox *createccomboboxobj, int &Item, int &SubItem, bool &havecreat);//创建单元格下拉列表框函数  
	void distroyCcombobox(CListCtrl *list, CComboBox* distroyccomboboxobj, int &Item, int &SubItem);//销毁单元格下拉列表框 

	afx_msg void  OnNMClickStockList(NMHDR *pNMHDR, LRESULT *pResult);
    
	bool m_zyByToday = TRUE;

	int jk_Item;    //刚编辑的行    
	int jk_SubItem; //刚编辑的列    

	CEdit jk_Edit;  //生成单元编辑框对象  
	bool jk_haveeditcreate;//标志编辑框已经被创建  
	void jk_createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat);//创建单元格编辑框函数  
	void jk_distroyEdit(CListCtrl *list, CEdit* distroyedit, int &Item, int &SubItem);//销毁单元格编辑框对象  
	afx_msg void  JkOnNMClickStockList(NMHDR *pNMHDR, LRESULT *pResult);

public:
	afx_msg void OnBnClickedOk();

	void OnKillfocusEdit();
	void JkOnKillfocusEdit();
	void OnKillfocusCcomboBox();
	void UpdateReport();
	afx_msg void threadUpdating();
	void StartSocket();
	void threadSelling();
	void threadBuying();
//TradeApi
	const unsigned int m_colnum = 13;
	const unsigned int m_cjColnum = 6;
	const unsigned int m_jkColnum = 7;
	TradeApi m_trade;

	void UpdateCjReport();
	void threadCjUpdating();

	void UpdateZyZsReport(CString zZqdm, int list_num);
	void threadEditZyZs();

	void threadJkUpdating();
	void UpdateJkReport();
	void UpdateJkPrice();
	void threadEditJK();

	afx_msg void OnBnClickedBtnAddJk();
	afx_msg void OnBnClickedBtnDelJk();
	void threadDelJkItem();
	int jk_del_index[MAX_STOCK_NUM];
	int jk_del_num;

	ADD_JK  Dlg;

	LRESULT ReceiveMessage(WPARAM wParam, LPARAM lParam);
	void addMainJkItem();
	JKREPORT m_add_jk;
	void OpenDlgJk();

	CString m_bar_clock;
	CString m_bar_kyzj;
	CString m_bar_zjzh;
	CString m_bar_shsz;
	void threadZjShszUpdating();
	void threadTimeUpdating();

};
