
// ZoTradeDlg.h : ͷ�ļ�
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

#define WM_USERMESSAGE WM_USER+110 //WM_USERMESSAGEΪ�Լ��������Ϣ����

// CZoTradeDlg �Ի���
class CZoTradeDlg : public CDialogEx
{
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	//BOOL OnCloseDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

// ����
public:
	CZoTradeDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CZoTradeDlg();
// �Ի�������
	enum { IDD = IDD_ZOTRADE_DIALOG };


// ʵ��
	CString str;


	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CString GetTimeInfo();


	CItemResize m_itemSize;

	CListCtrl m_list;
	CListCtrl m_CJList;
	CListCtrl m_JKList;


	int e_Item;    //�ձ༭����    
	int e_SubItem; //�ձ༭����    

	CEdit m_Edit;  //���ɵ�Ԫ�༭�����  
	bool haveeditcreate;//��־�༭���Ѿ�������  
	void createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat);//������Ԫ��༭����  
	void distroyEdit(CListCtrl *list, CEdit* distroyedit, int &Item, int &SubItem);//���ٵ�Ԫ��༭�����  

	CComboBox m_comBox;//������Ԫ�������б����  
	bool haveccomboboxcreate;//��־�����б���Ѿ�������  
	void createCcombobox(NM_LISTVIEW  *pEditCtrl, CComboBox *createccomboboxobj, int &Item, int &SubItem, bool &havecreat);//������Ԫ�������б����  
	void distroyCcombobox(CListCtrl *list, CComboBox* distroyccomboboxobj, int &Item, int &SubItem);//���ٵ�Ԫ�������б�� 

	afx_msg void  OnNMClickStockList(NMHDR *pNMHDR, LRESULT *pResult);
    
	bool m_zyByToday = TRUE;

	int jk_Item;    //�ձ༭����    
	int jk_SubItem; //�ձ༭����    

	CEdit jk_Edit;  //���ɵ�Ԫ�༭�����  
	bool jk_haveeditcreate;//��־�༭���Ѿ�������  
	void jk_createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat);//������Ԫ��༭����  
	void jk_distroyEdit(CListCtrl *list, CEdit* distroyedit, int &Item, int &SubItem);//���ٵ�Ԫ��༭�����  
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
