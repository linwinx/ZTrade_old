#pragma once
#include <windows.h>
#include <winbase.h>
#include <afxinet.h>
#include <iostream>

using namespace std;

// some fixed length
#define	STKLIB_MAX_CODE			8
#define	STKLIB_MAX_CODE2		16	// ���ݽϳ���Ʊ����
#define	STKLIB_MAX_NAME			16
#define	STKLIB_MAX_NAMEENU		80
#define	STKLIB_MAX_SHORTNAME	16
#define	STKLIB_MAX_DOMAIN		32
#define	STKLIB_MAX_PROVINCE		32

#define	MAX_STOCK_NUM		32
#define	ITEM_NUM		4

enum MSG_TYPE  {
	MSG_ADD_JK = 0,       
	MSG_DOING,
};

//��ѯ����
enum QUERY_HISTORY_TYPE  {
	QH_WEITUO = 0,       //0��ʷί�� 
	QH_CHENGJIAO,        //1��ʷ�ɽ�
	QH_JIAOGE,          //2���
};

enum QUERY_TYPE  {
	Q_ZI_JING = 0,      //0�ʽ� 
	Q_GU_FEN,           //1�ɷ�
	Q_TODAY_WEITUO,     //2����ί��
	Q_TODAY_CHENGJIAO,  //3���ճɽ�
	Q_CAN_CANCEL,       //4�ɳ���
	Q_GUDONG_CODE,      //5�ɶ�����
	Q_RONGZI_YUER,      //6�������
	Q_RONGQUAN_YUER,    //7��ȯ���
	Q_CAN_RONGQUANG,    //8����֤ȯ
};

///0�ʽ�
///�ʽ��ʺ�	   ����	�ʽ����	�����ʽ�	��ȡ�ʽ�	��;�ʽ�	�����ʽ�	������ֵ	���ʲ�	  ������־	������Ϣ
///308919343982	0	18210.06	31511.74	18210.06	13301.68	0.000	     7833.50	39345.24	0
typedef struct zjreport_t {
	CString	zZjzh;		// �ʽ��ʺ�
	CString zKyzj;                     //�����ʽ�
	CString zTotalzj;                      //���ʲ�
} ZJREPORT;

///֤ȯ����	֤ȯ����	���ռ�	�񿪼�	��ծ��Ϣ	��ǰ��	��һ��	�����	������	���ļ�	�����	��һ��	�����	������	������	������	��һ��	������	������	���ļ�	�����	��һ��	������	������	������	������	����������	��С���׹���	��С����䶯��λ	��С�����䶯��λ	�ʺ����	����	��ծ��ʶ	��ͣ�۸�	��ͣ�۸�	ȡ��̨����	������Ϣ
///300003	����ҽ��	33.70	33.35	0.00		33.37	33.37	33.36	33.35	33.34	33.33	475		61		28		14		264		33.38	33.39	33.40	33.41	33.42	347		19		236		216		20			0			100				0.01				0.01				0			0	255

// �������ݽṹ CStock::dataReport
typedef struct report_t {
	CString	zCode;		// ֤ȯ����
	CString	zName;		// ֤ȯ����

	CString	zLast;					// ���գ�Ԫ��
	CString	zOpen;					// �񿪣�Ԫ��
	CString	zNew;						// ���£�Ԫ��

	CString	zFisrtBuyPrice;            // �����1
	CString	zFisrtSellPrice;			// ������1

	CString zSiType;
	CString zGddm;

	CString	zHighPrice;						// ��߼�
	CString	zLowPrice;						// ��ͼ�

} REPORT;

///֤ȯ���� ֤ȯ���� �ɷ���� ���ùɷ�  �ɱ��� ��ǰ��   ������ֵ    ����ӯ��   ӯ������(%) ���׶������� �쳣���� ��;�ɷ� ��ǰӵ��   �ɶ�����	�ʺ���� ����������	  �ʽ��ʺ�	   �ͻ�����	  ����������	����ϯλ	��Ʒ��־	��ע	������Ϣ
///001965	���̹�·	1500	1500	11.573	9.26	13890.000	-3469.430	-19.99	       0	       0	   0	    1500	0149182203	   0	     0	     309619426390	194546902	����A��	     397517	

// �ֲ����ݽṹ 
typedef struct ireport_t {
	CString	zCode;		// ֤ȯ����
	CString	zName;		// ֤ȯ����
	CString	zAllNum;					// �ɷ����
	CString	zValidNum;					// ��������
	CString zBuyPrice;                     //�ɱ���
	CString	zNewPrice;						// ��ǰ��
	CString	zNowValue;					// ������ֵ
	CString zProfit;                     //����ӯ��
	CString	zProfitRate;						// ӯ������

	//CString zZhiYingPrice;                     //ֹӯ�۸�
	//CString	zZhiSunPrice;						// ֹ��۸�
	//CString zZhiYingRate;                     //ֹӯ����
	//CString	zZhiSunRate;						// ֹ�����

	CString	zGddm;                          //�ɶ�����
	CString	zHighPrice;						// ��߼�
	CString	zLowPrice;						// ��ͼ�
} IREPORT;

//ֹӯֹ�� Zot.ini
typedef struct zzreport_t {
	CString	zCode;		// ֤ȯ����
	CString zZhiYingRate;                     //ֹӯ����
	CString zZyHlRate;                      //ӯ��������
	CString	zZhiSunRate;						// ֹ�����
	int m_index;
} ZYZSREPORT;

//��� Zot.ini
typedef struct jkreport_t {
	CString	zCode;		// ֤ȯ����
	CString	zName;		// ֤ȯ����
	CString	zBuyNum;      //��������
	CString zXiaDieRate;                     //�µ�����
	CString zXdHtRate;                      //�µ��ص�����
} JKREPORT;

///3���ճɽ�
///ί��ʱ��	ί�б��	֤ȯ����	֤ȯ����	������־	������־	ί������	ί�м۸�	ί������	�ɽ��۸�	�ɽ�����	�ɽ����	�ɽ�ʱ��	�ʺ����	�ɶ�����	�ʽ��ʺ�	�ͻ�����	�ɶ�����	����������	�ɽ����	������Ϣ
///09:35:29	NNZPPACZ	000933		���ɷ�					1		����		7.4600		1800		7.4600		- 1800		0.00		09 : 36 : 43	0	0149198420	308919343982	195801929	��־��	����A��	0102000001304887
///09:53:49	NNZPPMSR	000933		���ɷ�					1		����		7.4000		1800		7.4000		1800		13320.00	09 : 54 : 26	0	0149198420	308919343982	195801929	��־��	����A��	0102000002942038

typedef struct wreport_t {
	CString	zWtdm;      //ί�б��
	CString	zZqdm;		// ֤ȯ����
	CString	zName;		// ֤ȯ����

	CString zCjFlag;     //��������
	CString	zCjPrice;	// �ɽ��۸�
	CString	zCjNum;		// �ɽ�����
	CString zCjSum;      //�ɽ����
	CString	zCjTime;	// �ɽ�ʱ��

} WREPORT;

//����
enum SEND_TYPE  {
	S_BUY = 0,          //0����
	S_SELL,             //1����
	S_RONGZI_BUY,       //2��������
	S_RONGZI_SELL,      //3��ȯ����
	S_BUYQUAN_HUANQUAN, //4��ȯ��ȯ
	S_SELLQUAN_HUANQUAN,//5��ȯ����
	S_XIANQUAN_HUANQUAN,//6��ȯ��ȯ
};

///�ʽ��ʺ�		�ɶ�����	�ɶ�����	�ʺ����	����ϯλ	������ȯ��ʶ	����ʽ�����ʶ	������Ϣ
///309619426390	0149182203	����		0			397517			0				0
///309619426390	A139336457	����		1			35614			0				0
typedef struct gdlist_t {
	CString	zZjzh;		// �ʽ��ʺ�
	CString	zGddm;		// �ɶ�����
	CString	zGdname;		// �ɶ�����
	CString	zSiType;		// �ʺ����
} GDLIST;

//�����ĵ�
//

//1.����API����Trade.dll�ļ��ĵ����������������º�����
//�������9��������
// void  OpenTdx();//��ͨ����
// void  CloseTdx();//�ر�ͨ����
//  int  Logon(char* IP, short Port, char* Version, short YybID, char* AccountNo,char* TradeAccount, char* JyPassword,   char* TxPassword, char* ErrInfo);//��¼�ʺ�
// void  Logoff(int ClientID);//ע��
// void  QueryData(int ClientID, int Category, char* Result, char* ErrInfo);//��ѯ���ཻ������
// void  SendOrder(int ClientID, int Category ,int PriceType,  char* Gddm,  char* Zqdm , float Price, int Quantity,  char* Result, char* ErrInfo);//�µ�
// void  CancelOrder(int ClientID, char* ExchangeID, char* hth, char* Result, char* ErrInfo);//����
// void  GetQuote(int ClientID, char* Zqdm, char* Result, char* ErrInfo);//��ȡ�嵵����
// void  Repay(int ClientID, char* Amount, char* Result, char* ErrInfo);//������ȯ�˻�ֱ�ӻ���


//��ͨ������������5��������(��Щȯ�̶������������������٣��������������Ŀ����ѯȯ��)
// void  QueryHistoryData(int ClientID, int Category, char* StartDate, char* EndDate, char* Result, char* ErrInfo);//��ѯ������ʷ����
// void  QueryDatas(int ClientID, int Category[], int Count, char* Result[], char* ErrInfo[]);//���˻�������ѯ���ཻ������
// void  SendOrders(int ClientID, int Category[] , int PriceType[], char* Gddm[],  char* Zqdm[] , float Price[], int Quantity[],  int Count, char* Result[], char* ErrInfo[]);//���˻������µ�
// void  CancelOrders(int ClientID, char* ExchangeID[], char* hth[], int Count, char* Result[], char* ErrInfo[]);//���˻���������
// void  GetQuotes(int ClientID, char* Zqdm[], int Count, char* Result[], char* ErrInfo[]);//���˻�������ȡ�嵵����


///���׽ӿ�ִ�к����ʧ�ܣ����ַ���ErrInfo�����˳�����Ϣ����˵����
///����ɹ������ַ���Result�����˽������,��ʽΪ������ݣ�������֮��ͨ��\n�ַ��ָ������֮��ͨ��\t�ָ���
///Result��\n��\t�ָ��������ַ����������ѯ�ɶ�����ʱ���صĽ���ַ������� 

///"�ɶ�����\t�ɶ�����\t�ʺ����\t������Ϣ\n
///0000064567\t\t0\t\nA000064567\t\t1\t\n
///2000064567\t\t2\t\nB000064567\t\t3\t"

///��ô�����֮��ͨ���ָ��ַ����� ���Իָ�Ϊ���м��еı����ʽ������



//2.APIʹ������Ϊ: Ӧ�ó����ȵ���OpenTdx��ͨ����ʵ����һ��ʵ���¿���ͬʱ��¼��������˻���ÿ�������˻���֮ΪClientID.
//ͨ������Logon���ClientID��Ȼ����Ե�������API���������ClientID���в�ѯ���µ�; Ӧ�ó����˳�ʱӦ����Logoffע��ClientID, ������CloseTdx�ر�ͨ����ʵ��. 
//OpenTdx��CloseTdx������Ӧ�ó�����ֻ�ܱ�����һ��.API���ж����Զ��������ܣ�Ӧ�ó���ֻ�����API�������صĳ�����Ϣ�����ʵ��������ɡ�


//3. ������������˵��

/// <summary>
/// ��ͨ����ʵ��
/// </summary>
///void   OpenTdx();
typedef void(__stdcall* OpenTdxDelegate)();


/// <summary>
/// �ر�ͨ����ʵ��
/// </summary>
///void   CloseTdx();
typedef void(__stdcall* CloseTdxDelegate)();


/// <summary>
/// �����˻���¼
/// </summary>
/// <param name="IP">ȯ�̽��׷�����IP</param>
/// <param name="Port">ȯ�̽��׷������˿�</param>
/// <param name="Version">����ͨ���ſͻ��˵İ汾��</param>
/// <param name="YybID">Ӫҵ�����룬�뵽��ַ http://www.chaoguwaigua.com/downloads/qszl.htm ��ѯ</param>
/// <param name="AccountNo">�����ĵ�¼�˺ţ�ȯ��һ��ʹ���ʽ��ʻ���ͻ���</param>
/// <param name="TradeAccount">�����˺ţ�һ�����¼�ʺ���ͬ. ���¼ȯ��ͨ�����������ѯ�ɶ��б��ɶ��б��ڵ��ʽ��ʺž��ǽ����ʺ�, �����ѯ���������վ���ȵ��ʴ���Ŀ</param>
/// <param name="JyPassword">��������</param>
/// <param name="TxPassword">ͨѶ����</param>
/// <param name="ErrInfo">��APIִ�з��غ�������������˴�����Ϣ˵����һ��Ҫ����256�ֽڵĿռ䡣û����ʱΪ���ַ�����</param>
/// <returns>�ͻ���ID��ʧ��ʱ����-1</returns>
///  int  Logon(char* IP, short Port, char* Version,short YybID,  char* AccountNo,char* TradeAccount, char* JyPassword,   char* TxPassword, char* ErrInfo);
typedef int(__stdcall* LogonDelegate)(char* IP, short Port, char* Version, short YybID, char* AccountNo, char* TradeAccount, char* JyPassword, char* TxPassword, char* ErrInfo);

/// <summary>
/// �����˻�ע��
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// void  Logoff(int ClientID);
typedef void(__stdcall* LogoffDelegate)(int ClientID);

/// <summary>
/// ��ѯ���ֽ�������
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Category">��ʾ��ѯ��Ϣ�����࣬0�ʽ�  1�ɷ�   2����ί��  3���ճɽ�     4�ɳ���   5�ɶ�����  6�������   7��ȯ���  8����֤ȯ</param>
/// <param name="Result">��APIִ�з��غ�Result�ڱ����˷��صĲ�ѯ����, ��ʽΪ������ݣ�������֮��ͨ��\n�ַ��ָ������֮��ͨ��\t�ָ���һ��Ҫ����1024*1024�ֽڵĿռ䡣����ʱΪ���ַ�����</param>
/// <param name="ErrInfo">ͬLogon������ErrInfo˵��</param>
/// void  QueryData(int ClientID, int Category, char* Result, char* ErrInfo);
typedef void(__stdcall* QueryDataDelegate)(int ClientID, int Category, char* Result, char* ErrInfo);

///0�ʽ�
///�ʽ��ʺ�	   ����	�ʽ����	�����ʽ�	��ȡ�ʽ�	��;�ʽ�	�����ʽ�	������ֵ	���ʲ�	  ������־	������Ϣ
///308919343982	0	18210.06	31511.74	18210.06	13301.68	0.000	     7833.50	39345.24	0

//1�ɷ�
///֤ȯ���� ֤ȯ���� �ɷ���� ���ùɷ�  �ɱ��� ��ǰ��   ������ֵ    ����ӯ��   ӯ������(%) ���׶������� �쳣���� ��;�ɷ� ��ǰӵ��   �ɶ�����	�ʺ���� ����������	  �ʽ��ʺ�	   �ͻ�����	  ����������	����ϯλ	��Ʒ��־	��ע	������Ϣ
///001965	���̹�·	1500	1500	11.573	9.26	13890.000	-3469.430	-19.99	       0	       0	   0	    1500	0149182203	   0	     0	     309619426390	194546902	����A��	     397517	

///2����ί��
///ί��ʱ��	ί�б��	֤ȯ����	֤ȯ����	������־	������־	ί������	ί�м۸�	ί������	ί�н��	�ɽ��۸�	�ɽ�����	�ɽ����	�ѳ�����	������־	�ʺ����	�ɶ�����	����������	�ͻ�����		�ʽ��ʺ�	�ɶ�����	�Ϸ���־	�Ϸ���Ϣ	�ɳ�����־	����������	������Ϣ
///09:53:49	NNZPPMSR	000933		���ɷ�					1		����		7.4000		1800		13320.00	7.400		1800		13320.00	0			�ѳɽ�		0			0149198420	����A��		195801929	308919343982	��־��		�Ϸ�		ί�кϷ�		0		00
///09:35:29	NNZPPACZ	000933		���ɷ�					1		����		7.4600		1800		13428.00	0.000		0			0.00		1800		�ѳ�		0			0149198420	����A��		195801929	308919343982	��־��		�Ϸ�		ί�кϷ�		0		00

///3���ճɽ�
///ί��ʱ��	ί�б��	֤ȯ����	֤ȯ����	������־	������־	ί������	ί�м۸�	ί������	�ɽ��۸�	�ɽ�����	�ɽ����	�ɽ�ʱ��	�ʺ����	�ɶ�����	�ʽ��ʺ�	�ͻ�����	�ɶ�����	����������	�ɽ����	������Ϣ
///09:35:29	NNZPPACZ	000933		���ɷ�					1		����		7.4600		1800		7.4600		- 1800		0.00		09 : 36 : 43	0	0149198420	308919343982	195801929	��־��	����A��	0102000001304887
///09:53:49	NNZPPMSR	000933		���ɷ�					1		����		7.4000		1800		7.4000		1800		13320.00	09 : 54 : 26	0	0149198420	308919343982	195801929	��־��	����A��	0102000002942038

///4�ɳ���
///ί��ʱ��	�ʽ��ʺ�	�ɶ�����	�ʺ����	֤ȯ����	֤ȯ����	������־	������־	ί������	ί�м۸�	ί������	ί�н��	ί�б��	�ɽ��۸�	�ɽ�����	�ѳ�����	���κ�	ժҪ	����������	������Ϣ


///�ʽ��ʺ�		�ɶ�����	�ɶ�����	�ʺ����	����ϯλ	������ȯ��ʶ	����ʽ�����ʶ	������Ϣ
///309619426390	0149182203	����		0			397517			0				0
///309619426390	A139336457	����		1			35614			0				0

/// <summary>
/// ������ͨ�����湦��,��ѯ������ʷ����
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Category">��ʾ��ѯ��Ϣ�����࣬0��ʷί��  1��ʷ�ɽ�   2���</param>
/// <param name="StartDate">��ʾ��ʼ���ڣ���ʽΪyyyyMMdd,����2014��3��1��Ϊ  20140301
/// <param name="EndDate">��ʾ�������ڣ���ʽΪyyyyMMdd,����2014��3��1��Ϊ  20140301
/// <param name="Result">ͬ��</param>
/// <param name="ErrInfo">ͬ��</param>
/// void  QueryHistoryData(int ClientID, int Category, char* StartDate, char* EndDate, char* Result, char* ErrInfo);
typedef void(__stdcall* QueryHistoryDataDelegate)(int ClientID, int Category, char* StartDate, char* EndDate, char* Result, char* ErrInfo);

//1��ʷ�ɽ�
//ί������			ί��ʱ��		ί�б��	֤ȯ����	֤ȯ����	������־	������־	ί������	ί�м۸�	ί������	�ɽ��۸�	�ɽ�����	�ɽ����		�ɽ�����		�ɽ�ʱ��	�ʺ����	�ɶ�����	�ʽ��ʺ�		�ͻ�����	�ɶ�����	����������	������Ϣ
//2018 - 04 - 20	09:36 : 45		NNZQSPEN	002236		�󻪹ɷ�		0					����		23.1000		400			23.1000			400			9240.00		2018 - 04 - 20	09 : 36 : 45	0		0149198420	308919343982	195801929	��־��		����A��
//2018 - 04 - 20	10 : 45 : 31	NNZQTJ8S	002456		ŷ�ƿƼ�		0					����		19.3600		500			19.3600			500			9680.00		2018 - 04 - 20	10 : 45 : 35	0		0149198420	308919343982	195801929	��־��		����A��

/// <summary>
/// ������ͨ�����湦��,������ѯ���ֽ�������,�����鴫��ÿ��ί�еĲ����������i��Ԫ�ر�ʾ��i����ѯ����Ӧ����
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Category">��Ϣ�����������, ��i��Ԫ�ر�ʾ��i����ѯ����Ϣ���࣬0�ʽ�  1�ɷ�   2����ί��  3���ճɽ�     4�ɳ���   5�ɶ�����  6�������   7��ȯ���  8����֤ȯ</param>
/// <param name="Count">��ѯ�ĸ�����������ĳ���</param>
/// <param name="Result">�������ݵ�����, ��i��Ԫ�ر�ʾ��i��ί�еķ�����Ϣ. ��APIִ�з��غ�Result[i]����ͬ�ϡ�</param>
/// <param name="ErrInfo">������Ϣ�����飬��i��Ԫ�ر�ʾ��i��ί�еĴ�����Ϣ. ��APIִ�з��غ�ErrInfo[i]����ͬ�ϡ�</param>
/// void  QueryDatas(int ClientID, int Category[], int Count, char* Result[], char* ErrInfo[]);
typedef void(__stdcall* QueryDatasDelegate)(int ClientID, int Category[], int Count, char* Result[], char* ErrInfo[]);


/// <summary>
/// ��ί�н���֤ȯ
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Category">��ʾί�е����࣬0���� 1����  2��������  3��ȯ����   4��ȯ��ȯ   5��ȯ����  6��ȯ��ȯ</param>
/// <param name="PriceType">��ʾ���۷�ʽ 0�Ϻ��޼�ί�� �����޼�ί�� 1(�м�ί��)���ڶԷ����ż۸�  2(�м�ί��)���ڱ������ż۸�  3(�м�ί��)���ڼ�ʱ�ɽ�ʣ�೷��  4(�м�ί��)�Ϻ��嵵����ʣ�� �����嵵����ʣ�� 5(�м�ί��)����ȫ��ɽ����� 6(�м�ί��)�Ϻ��嵵����ת�޼�
/// <param name="Gddm">�ɶ�����, �����Ϻ���Ʊ���Ϻ��Ĺɶ����룻�������ڵĹ�Ʊ�������ڵĹɶ�����</param>
/// <param name="Zqdm">֤ȯ����</param>
/// <param name="Price">ί�м۸�</param>
/// <param name="Quantity">ί������</param>
/// <param name="Result">ͬ��,���к���ί�б������</param>
/// <param name="ErrInfo">ͬ��</param>
/// void  SendOrder(int ClientID, int Category ,int PriceType,  char* Gddm,  char* Zqdm , float Price, int Quantity,  char* Result, char* ErrInfo);
typedef void(__stdcall* SendOrderDelegate)(int ClientID, int Category, int PriceType, char* Gddm, char* Zqdm, float Price, int Quantity, char* Result, char* ErrInfo);



/// <summary>
/// ������ͨ�����湦��,������ί�н���֤ȯ�������鴫��ÿ��ί�еĲ����������i��Ԫ�ر�ʾ��i��ί�е���Ӧ����
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Category">ί����������飬��i��Ԫ�ر�ʾ��i��ί�е����࣬0���� 1����  2��������  3��ȯ����   4��ȯ��ȯ   5��ȯ����  6��ȯ��ȯ</param>
/// <param name="PriceType">��ʾ���۷�ʽ������,  ��i��Ԫ�ر�ʾ��i��ί�еı��۷�ʽ, 0�Ϻ��޼�ί�� �����޼�ί�� 1(�м�ί��)���ڶԷ����ż۸�  2(�м�ί��)���ڱ������ż۸�  3(�м�ί��)���ڼ�ʱ�ɽ�ʣ�೷��  4(�м�ί��)�Ϻ��嵵����ʣ�� �����嵵����ʣ�� 5(�м�ί��)����ȫ��ɽ����� 6(�м�ί��)�Ϻ��嵵����ת�޼�
/// <param name="Gddm">�ɶ��������飬��i��Ԫ�ر�ʾ��i��ί�еĹɶ����룬�����Ϻ���Ʊ���Ϻ��Ĺɶ����룻�������ڵĹ�Ʊ�������ڵĹɶ�����</param>
/// <param name="Zqdm">֤ȯ�������飬��i��Ԫ�ر�ʾ��i��ί�е�֤ȯ����</param>
/// <param name="Price">ί�м۸����飬��i��Ԫ�ر�ʾ��i��ί�е�ί�м۸�</param>
/// <param name="Quantity">ί���������飬��i��Ԫ�ر�ʾ��i��ί�е�ί������</param>
/// <param name="Count">ί�еĸ�����������ĳ���</param>
/// <param name="Result">ͬ��</param>
/// <param name="ErrInfo">ͬ��</param>
/// void  SendOrders(int ClientID, int Category[] , int PriceType[], char* Gddm[],  char* Zqdm[] , float Price[], int Quantity[],  int Count, char* Result[], char* ErrInfo[]);
typedef void(__stdcall* SendOrdersDelegate)(int ClientID, int Category[], int PriceType[], char* Gddm[], char* Zqdm[], float Price[], int Quantity[], int Count, char* Result[], char* ErrInfo[]);



/// <summary>
/// ��ί��
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="ExchangeID">������ID�� �Ϻ�1������0(����֤ȯ��ͨ�˻�������2)</param>
/// <param name="hth">��ʾҪ����Ŀ��ί�еı��</param>
/// <param name="Result">ͬ��</param>
/// <param name="ErrInfo">ͬ��</param>
/// void  CancelOrder(int ClientID, char* ExchangeID, char* hth, char* Result, char* ErrInfo);
typedef void(__stdcall* CancelOrderDelegate)(int ClientID, char* ExchangeID, char* hth, char* Result, char* ErrInfo);




/// <summary>
/// ������ͨ�����湦��,������ί��, �����鴫��ÿ��ί�еĲ����������i��Ԫ�ر�ʾ��i����ί�е���Ӧ����
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
// <param name="ExchangeID">������ID�� �Ϻ�1������0(����֤ȯ��ͨ�˻�������2)</param>
/// <param name="hth">��ʾҪ����Ŀ��ί�еı��</param>
/// <param name="Count">��ί�еĸ�����������ĳ���</param>
/// <param name="Result">ͬ��</param>
/// <param name="ErrInfo">ͬ��</param>
/// void  CancelOrders(int ClientID, char* ExchangeID[], char* hth[], int Count, char* Result[], char* ErrInfo[]);
typedef void(__stdcall* CancelOrdersDelegate)(int ClientID, char* ExchangeID[], char* hth[], int Count, char* Result[], char* ErrInfo[]);




/// <summary>
/// ��ȡ֤ȯ��ʵʱ�嵵����
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Zqdm">֤ȯ����</param>
/// <param name="Result">ͬ��</param>
/// <param name="ErrInfo">ͬ��</param>
/// void  GetQuote(int ClientID, char* Zqdm, char* Result, char* ErrInfo);
typedef void(__stdcall* GetQuoteDelegate)(int ClientID, char* Zqdm, char* Result, char* ErrInfo);
///֤ȯ����	֤ȯ����	���ռ�	�񿪼�	��ծ��Ϣ	��ǰ��	��һ��	�����	������	���ļ�	�����	��һ��	�����	������	������	������	��һ��	������	������	���ļ�	�����	��һ��	������	������	������	������	����������	��С���׹���	��С����䶯��λ	��С�����䶯��λ	�ʺ����	����	��ծ��ʶ	��ͣ�۸�	��ͣ�۸�	ȡ��̨����	������Ϣ
///300003	����ҽ��	33.70	33.35	0.00		33.37	33.37	33.36	33.35	33.34	33.33	475		61		28		14		264		33.38	33.39	33.40	33.41	33.42	347		19		236		216		20			0			100				0.01				0.01				0			0	255

/// <summary>
/// ������ͨ�����湦��,������ȡ֤ȯ��ʵʱ�嵵����
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Zqdm">֤ȯ����</param>
/// <param name="Result">ͬ��</param>
/// <param name="ErrInfo">ͬ��</param>
/// void  GetQuotes(int ClientID, char* Zqdm[], int Count, char* Result[], char* ErrInfo[]);
typedef void(__stdcall* GetQuotesDelegate)(int ClientID, char* Zqdm[], int Count, char* Result[], char* ErrInfo[]);



/// <summary>
/// ������ȯֱ�ӻ���
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Amount">������</param>
/// <param name="Result">ͬ��</param>
/// <param name="ErrInfo">ͬ��</param>
/// void  Repay(int ClientID, char* Amount, char* Result, char* ErrInfo);
typedef void(__stdcall* RepayDelegate)(int ClientID, char* Amount, char* Result, char* ErrInfo);

#define ACC_PINAN
//#define ACC_PINAN2
//#define ACC_ZAHOSHANG

#define RUN_LOG 1

class TradeApi
{
protected:
	char *m_dll_ver = "6.40";
#ifdef ACC_PINAN//ƽ������
	char *m_ip = "202.69.19.56";
	int m_port = 7708;
	char *m_accout = "309619426390";
	char *m_pwd = "781813";
	CString m_dll_lib = _T("Trade_pa.dll");
#endif
#ifdef ACC_PINAN2//ƽ������
	char *m_ip = "202.69.19.56";
	int m_port = 7708;
    char *m_accout = "308919343982";
    char *m_pwd = "201637";
	CString m_dll_lib = _T("Trade_pa2.dll");
#endif
#ifdef ACC_ZAHOSHANG ////���̽���
	char *m_ip = "58.251.16.167";
	int m_port = 443;
	 char *m_accout = "97720035";
	 char *m_pwd = "781813";
	 CString m_dll_lib = _T("Trade_zs.dll");
#endif
public:
	TradeApi();
	~TradeApi();

	int log_num = 0;
	
	OpenTdxDelegate OpenTdx;
	CloseTdxDelegate CloseTdx;
	LogonDelegate Logon;
	LogoffDelegate Logoff;
	QueryDataDelegate QueryData;
	SendOrderDelegate SendOrder;
	CancelOrderDelegate CancelOrder;
	GetQuoteDelegate GetQuote;
	RepayDelegate Repay;


	//��������ͨ�����湦�ܺ���
	QueryDatasDelegate QueryDatas;
	QueryHistoryDataDelegate QueryHistoryData;
	SendOrdersDelegate SendOrders;
	CancelOrdersDelegate CancelOrders;
	GetQuotesDelegate GetQuotes;

	HMODULE HMODULE1;
	int m_ClientID;
	void Login();
	void Logout();

	float m_defProfitRate = 6.0;
	float m_defDownRate = 1.0;
	float m_defaultLossRate = 7.0;

	float m_shRate, m_szRate;

	IREPORT m_report[MAX_STOCK_NUM];
	int m_stock_num = 0;

	WREPORT cj_report[MAX_STOCK_NUM];
	int m_cj_num = 0;

	ZYZSREPORT zz_report[MAX_STOCK_NUM];
	int m_zyzs_num = 0;

	JKREPORT  jk_report[MAX_STOCK_NUM];
	int m_jk_num = 0;

	CString m_gddm_str[ITEM_NUM];
	GDLIST m_gddm_list[ITEM_NUM];
	int m_gddm_num = 0;
	//IREPORT m_PreReport[MAX_STOCK_NUM];

	void BuyStock(CString Zqdm, CString Price, CString Quantity);
	void SellStock(CString Zqdm, CString Price, CString Quantity);
	void GetGddm(void);

	void SellByTodayZyRate();
	void SellByTotalZyRate();
	void SellByZhiSunRate();

	void BuyByDownRate();

	void GetChicangInfo();

	CString GetZqGddm(CString Zqdm);
	REPORT GetZqInfo(CString Zqdm);



	void GetChengjiaoInfo();

	CString ReturnPath(void);
	void writeIni(CString name, CString strValue);
	CString readIni(CString name);
	BOOL isIniExist();
	void initIni();
	void readZyZsIni();
	void writeZyZsIni();
	void readJKIni();
	void writeJKIni();

	void writeIniLog(CString name, CString strValue);

	ZJREPORT m_zjInfo;
	void GetZqZj();
	CString GetZqZjStr();
	CString GetZjzhStr();
	CString GetShszStr();

	REPORT GetZqInfoSina(CString Zqdm);

	CString GetDateTimeInfo();
	CString GetDateInfo();

};



