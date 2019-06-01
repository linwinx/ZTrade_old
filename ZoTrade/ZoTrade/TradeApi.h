#pragma once
#include <windows.h>
#include <winbase.h>
#include <afxinet.h>
#include <iostream>

using namespace std;

// some fixed length
#define	STKLIB_MAX_CODE			8
#define	STKLIB_MAX_CODE2		16	// 兼容较长股票代码
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

//查询行情
enum QUERY_HISTORY_TYPE  {
	QH_WEITUO = 0,       //0历史委托 
	QH_CHENGJIAO,        //1历史成交
	QH_JIAOGE,          //2交割单
};

enum QUERY_TYPE  {
	Q_ZI_JING = 0,      //0资金 
	Q_GU_FEN,           //1股份
	Q_TODAY_WEITUO,     //2当日委托
	Q_TODAY_CHENGJIAO,  //3当日成交
	Q_CAN_CANCEL,       //4可撤单
	Q_GUDONG_CODE,      //5股东代码
	Q_RONGZI_YUER,      //6融资余额
	Q_RONGQUAN_YUER,    //7融券余额
	Q_CAN_RONGQUANG,    //8可融证券
};

///0资金
///资金帐号	   币种	资金余额	可用资金	可取资金	在途资金	冻结资金	最新市值	总资产	  操作标志	保留信息
///308919343982	0	18210.06	31511.74	18210.06	13301.68	0.000	     7833.50	39345.24	0
typedef struct zjreport_t {
	CString	zZjzh;		// 资金帐号
	CString zKyzj;                     //可用资金
	CString zTotalzj;                      //总资产
} ZJREPORT;

///证券代码	证券名称	昨收价	今开价	国债利息	当前价	买一价	买二价	买三价	买四价	买五价	买一量	买二量	买三量	买四量	买五量	卖一价	卖二价	卖三价	卖四价	卖五价	卖一量	卖二量	卖三量	卖四量	卖五量	交易所代码	最小交易股数	最小买入变动价位	最小卖出变动价位	帐号类别	币种	国债标识	涨停价格	跌停价格	取柜台行情	保留信息
///300003	乐普医疗	33.70	33.35	0.00		33.37	33.37	33.36	33.35	33.34	33.33	475		61		28		14		264		33.38	33.39	33.40	33.41	33.42	347		19		236		216		20			0			100				0.01				0.01				0			0	255

// 行情数据结构 CStock::dataReport
typedef struct report_t {
	CString	zCode;		// 证券代码
	CString	zName;		// 证券名称

	CString	zLast;					// 昨收（元）
	CString	zOpen;					// 今开（元）
	CString	zNew;						// 最新（元）

	CString	zFisrtBuyPrice;            // 申买价1
	CString	zFisrtSellPrice;			// 申卖价1

	CString zSiType;
	CString zGddm;

	CString	zHighPrice;						// 最高价
	CString	zLowPrice;						// 最低价

} REPORT;

///证券代码 证券名称 股份余额 可用股份  成本价 当前价   最新市值    浮动盈亏   盈亏比例(%) 交易冻结数量 异常冻结 在途股份 当前拥股   股东代码	帐号类别 交易所代码	  资金帐号	   客户代码	  交易所名称	交易席位	产品标志	备注	保留信息
///001965	招商公路	1500	1500	11.573	9.26	13890.000	-3469.430	-19.99	       0	       0	   0	    1500	0149182203	   0	     0	     309619426390	194546902	深圳A股	     397517	

// 持仓数据结构 
typedef struct ireport_t {
	CString	zCode;		// 证券代码
	CString	zName;		// 证券名称
	CString	zAllNum;					// 股份余额
	CString	zValidNum;					// 可卖数量
	CString zBuyPrice;                     //成本价
	CString	zNewPrice;						// 当前价
	CString	zNowValue;					// 最新市值
	CString zProfit;                     //浮动盈亏
	CString	zProfitRate;						// 盈亏比例

	//CString zZhiYingPrice;                     //止盈价格
	//CString	zZhiSunPrice;						// 止损价格
	//CString zZhiYingRate;                     //止盈比例
	//CString	zZhiSunRate;						// 止损比例

	CString	zGddm;                          //股东代码
	CString	zHighPrice;						// 最高价
	CString	zLowPrice;						// 最低价
} IREPORT;

//止盈止损 Zot.ini
typedef struct zzreport_t {
	CString	zCode;		// 证券代码
	CString zZhiYingRate;                     //止盈比例
	CString zZyHlRate;                      //盈达回落比例
	CString	zZhiSunRate;						// 止损比例
	int m_index;
} ZYZSREPORT;

//监控 Zot.ini
typedef struct jkreport_t {
	CString	zCode;		// 证券代码
	CString	zName;		// 证券名称
	CString	zBuyNum;      //买入数量
	CString zXiaDieRate;                     //下跌比例
	CString zXdHtRate;                      //下跌回调比例
} JKREPORT;

///3当日成交
///委托时间	委托编号	证券代码	证券名称	买卖标志	买卖标志	委托类型	委托价格	委托数量	成交价格	成交数量	成交金额	成交时间	帐号类别	股东代码	资金帐号	客户代码	股东姓名	交易所名称	成交编号	保留信息
///09:35:29	NNZPPACZ	000933		神火股份					1		撤卖		7.4600		1800		7.4600		- 1800		0.00		09 : 36 : 43	0	0149198420	308919343982	195801929	徐志娥	深圳A股	0102000001304887
///09:53:49	NNZPPMSR	000933		神火股份					1		卖出		7.4000		1800		7.4000		1800		13320.00	09 : 54 : 26	0	0149198420	308919343982	195801929	徐志娥	深圳A股	0102000002942038

typedef struct wreport_t {
	CString	zWtdm;      //委托编号
	CString	zZqdm;		// 证券代码
	CString	zName;		// 证券名称

	CString zCjFlag;     //买卖类型
	CString	zCjPrice;	// 成交价格
	CString	zCjNum;		// 成交数量
	CString zCjSum;      //成交金额
	CString	zCjTime;	// 成交时间

} WREPORT;

//交易
enum SEND_TYPE  {
	S_BUY = 0,          //0买入
	S_SELL,             //1卖出
	S_RONGZI_BUY,       //2融资买入
	S_RONGZI_SELL,      //3融券卖出
	S_BUYQUAN_HUANQUAN, //4买券还券
	S_SELLQUAN_HUANQUAN,//5卖券还款
	S_XIANQUAN_HUANQUAN,//6现券还券
};

///资金帐号		股东代码	股东姓名	帐号类别	交易席位	融资融券标识	开放式基金标识	保留信息
///309619426390	0149182203	张林		0			397517			0				0
///309619426390	A139336457	张林		1			35614			0				0
typedef struct gdlist_t {
	CString	zZjzh;		// 资金帐号
	CString	zGddm;		// 股东代码
	CString	zGdname;		// 股东姓名
	CString	zSiType;		// 帐号类别
} GDLIST;

//开发文档
//

//1.交易API均是Trade.dll文件的导出函数，包括以下函数：
//基本版的9个函数：
// void  OpenTdx();//打开通达信
// void  CloseTdx();//关闭通达信
//  int  Logon(char* IP, short Port, char* Version, short YybID, char* AccountNo,char* TradeAccount, char* JyPassword,   char* TxPassword, char* ErrInfo);//登录帐号
// void  Logoff(int ClientID);//注销
// void  QueryData(int ClientID, int Category, char* Result, char* ErrInfo);//查询各类交易数据
// void  SendOrder(int ClientID, int Category ,int PriceType,  char* Gddm,  char* Zqdm , float Price, int Quantity,  char* Result, char* ErrInfo);//下单
// void  CancelOrder(int ClientID, char* ExchangeID, char* hth, char* Result, char* ErrInfo);//撤单
// void  GetQuote(int ClientID, char* Zqdm, char* Result, char* ErrInfo);//获取五档报价
// void  Repay(int ClientID, char* Amount, char* Result, char* ErrInfo);//融资融券账户直接还款


//普通批量版新增的5个函数：(有些券商对批量操作进行了限速，最大批量操作数目请咨询券商)
// void  QueryHistoryData(int ClientID, int Category, char* StartDate, char* EndDate, char* Result, char* ErrInfo);//查询各类历史数据
// void  QueryDatas(int ClientID, int Category[], int Count, char* Result[], char* ErrInfo[]);//单账户批量查询各类交易数据
// void  SendOrders(int ClientID, int Category[] , int PriceType[], char* Gddm[],  char* Zqdm[] , float Price[], int Quantity[],  int Count, char* Result[], char* ErrInfo[]);//单账户批量下单
// void  CancelOrders(int ClientID, char* ExchangeID[], char* hth[], int Count, char* Result[], char* ErrInfo[]);//单账户批量撤单
// void  GetQuotes(int ClientID, char* Zqdm[], int Count, char* Result[], char* ErrInfo[]);//单账户批量获取五档报价


///交易接口执行后，如果失败，则字符串ErrInfo保存了出错信息中文说明；
///如果成功，则字符串Result保存了结果数据,形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。
///Result是\n，\t分隔的中文字符串，比如查询股东代码时返回的结果字符串就是 

///"股东代码\t股东名称\t帐号类别\t保留信息\n
///0000064567\t\t0\t\nA000064567\t\t1\t\n
///2000064567\t\t2\t\nB000064567\t\t3\t"

///查得此数据之后，通过分割字符串， 可以恢复为几行几列的表格形式的数据



//2.API使用流程为: 应用程序先调用OpenTdx打开通达信实例，一个实例下可以同时登录多个交易账户，每个交易账户称之为ClientID.
//通过调用Logon获得ClientID，然后可以调用其他API函数向各个ClientID进行查询或下单; 应用程序退出时应调用Logoff注销ClientID, 最后调用CloseTdx关闭通达信实例. 
//OpenTdx和CloseTdx在整个应用程序中只能被调用一次.API带有断线自动重连功能，应用程序只需根据API函数返回的出错信息进行适当错误处理即可。


//3. 各个函数功能说明

/// <summary>
/// 打开通达信实例
/// </summary>
///void   OpenTdx();
typedef void(__stdcall* OpenTdxDelegate)();


/// <summary>
/// 关闭通达信实例
/// </summary>
///void   CloseTdx();
typedef void(__stdcall* CloseTdxDelegate)();


/// <summary>
/// 交易账户登录
/// </summary>
/// <param name="IP">券商交易服务器IP</param>
/// <param name="Port">券商交易服务器端口</param>
/// <param name="Version">设置通达信客户端的版本号</param>
/// <param name="YybID">营业部代码，请到网址 http://www.chaoguwaigua.com/downloads/qszl.htm 查询</param>
/// <param name="AccountNo">完整的登录账号，券商一般使用资金帐户或客户号</param>
/// <param name="TradeAccount">交易账号，一般与登录帐号相同. 请登录券商通达信软件，查询股东列表，股东列表内的资金帐号就是交易帐号, 具体查询方法请见网站“热点问答”栏目</param>
/// <param name="JyPassword">交易密码</param>
/// <param name="TxPassword">通讯密码</param>
/// <param name="ErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
/// <returns>客户端ID，失败时返回-1</returns>
///  int  Logon(char* IP, short Port, char* Version,short YybID,  char* AccountNo,char* TradeAccount, char* JyPassword,   char* TxPassword, char* ErrInfo);
typedef int(__stdcall* LogonDelegate)(char* IP, short Port, char* Version, short YybID, char* AccountNo, char* TradeAccount, char* JyPassword, char* TxPassword, char* ErrInfo);

/// <summary>
/// 交易账户注销
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// void  Logoff(int ClientID);
typedef void(__stdcall* LogoffDelegate)(int ClientID);

/// <summary>
/// 查询各种交易数据
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Category">表示查询信息的种类，0资金  1股份   2当日委托  3当日成交     4可撤单   5股东代码  6融资余额   7融券余额  8可融证券</param>
/// <param name="Result">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
/// <param name="ErrInfo">同Logon函数的ErrInfo说明</param>
/// void  QueryData(int ClientID, int Category, char* Result, char* ErrInfo);
typedef void(__stdcall* QueryDataDelegate)(int ClientID, int Category, char* Result, char* ErrInfo);

///0资金
///资金帐号	   币种	资金余额	可用资金	可取资金	在途资金	冻结资金	最新市值	总资产	  操作标志	保留信息
///308919343982	0	18210.06	31511.74	18210.06	13301.68	0.000	     7833.50	39345.24	0

//1股份
///证券代码 证券名称 股份余额 可用股份  成本价 当前价   最新市值    浮动盈亏   盈亏比例(%) 交易冻结数量 异常冻结 在途股份 当前拥股   股东代码	帐号类别 交易所代码	  资金帐号	   客户代码	  交易所名称	交易席位	产品标志	备注	保留信息
///001965	招商公路	1500	1500	11.573	9.26	13890.000	-3469.430	-19.99	       0	       0	   0	    1500	0149182203	   0	     0	     309619426390	194546902	深圳A股	     397517	

///2当日委托
///委托时间	委托编号	证券代码	证券名称	买卖标志	买卖标志	委托类型	委托价格	委托数量	委托金额	成交价格	成交数量	成交金额	已撤数量	撤单标志	帐号类别	股东代码	交易所名称	客户代码		资金帐号	股东姓名	合法标志	合法信息	可撤单标志	交易所代码	保留信息
///09:53:49	NNZPPMSR	000933		神火股份					1		卖出		7.4000		1800		13320.00	7.400		1800		13320.00	0			已成交		0			0149198420	深圳A股		195801929	308919343982	徐志娥		合法		委托合法		0		00
///09:35:29	NNZPPACZ	000933		神火股份					1		卖出		7.4600		1800		13428.00	0.000		0			0.00		1800		已撤		0			0149198420	深圳A股		195801929	308919343982	徐志娥		合法		委托合法		0		00

///3当日成交
///委托时间	委托编号	证券代码	证券名称	买卖标志	买卖标志	委托类型	委托价格	委托数量	成交价格	成交数量	成交金额	成交时间	帐号类别	股东代码	资金帐号	客户代码	股东姓名	交易所名称	成交编号	保留信息
///09:35:29	NNZPPACZ	000933		神火股份					1		撤卖		7.4600		1800		7.4600		- 1800		0.00		09 : 36 : 43	0	0149198420	308919343982	195801929	徐志娥	深圳A股	0102000001304887
///09:53:49	NNZPPMSR	000933		神火股份					1		卖出		7.4000		1800		7.4000		1800		13320.00	09 : 54 : 26	0	0149198420	308919343982	195801929	徐志娥	深圳A股	0102000002942038

///4可撤单
///委托时间	资金帐号	股东代码	帐号类别	证券代码	证券名称	买卖标志	买卖标志	委托类型	委托价格	委托数量	委托金额	委托编号	成交价格	成交数量	已撤数量	批次号	摘要	交易所代码	保留信息


///资金帐号		股东代码	股东姓名	帐号类别	交易席位	融资融券标识	开放式基金标识	保留信息
///309619426390	0149182203	张林		0			397517			0				0
///309619426390	A139336457	张林		1			35614			0				0

/// <summary>
/// 属于普通批量版功能,查询各种历史数据
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Category">表示查询信息的种类，0历史委托  1历史成交   2交割单</param>
/// <param name="StartDate">表示开始日期，格式为yyyyMMdd,比如2014年3月1日为  20140301
/// <param name="EndDate">表示结束日期，格式为yyyyMMdd,比如2014年3月1日为  20140301
/// <param name="Result">同上</param>
/// <param name="ErrInfo">同上</param>
/// void  QueryHistoryData(int ClientID, int Category, char* StartDate, char* EndDate, char* Result, char* ErrInfo);
typedef void(__stdcall* QueryHistoryDataDelegate)(int ClientID, int Category, char* StartDate, char* EndDate, char* Result, char* ErrInfo);

//1历史成交
//委托日期			委托时间		委托编号	证券代码	证券名称	买卖标志	买卖标志	委托类型	委托价格	委托数量	成交价格	成交数量	成交金额		成交日期		成交时间	帐号类别	股东代码	资金帐号		客户代码	股东姓名	交易所名称	保留信息
//2018 - 04 - 20	09:36 : 45		NNZQSPEN	002236		大华股份		0					买入		23.1000		400			23.1000			400			9240.00		2018 - 04 - 20	09 : 36 : 45	0		0149198420	308919343982	195801929	徐志娥		深圳A股
//2018 - 04 - 20	10 : 45 : 31	NNZQTJ8S	002456		欧菲科技		0					买入		19.3600		500			19.3600			500			9680.00		2018 - 04 - 20	10 : 45 : 35	0		0149198420	308919343982	195801929	徐志娥		深圳A股

/// <summary>
/// 属于普通批量版功能,批量查询各种交易数据,用数组传入每个委托的参数，数组第i个元素表示第i个查询的相应参数
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Category">信息的种类的数组, 第i个元素表示第i个查询的信息种类，0资金  1股份   2当日委托  3当日成交     4可撤单   5股东代码  6融资余额   7融券余额  8可融证券</param>
/// <param name="Count">查询的个数，即数组的长度</param>
/// <param name="Result">返回数据的数组, 第i个元素表示第i个委托的返回信息. 此API执行返回后，Result[i]含义同上。</param>
/// <param name="ErrInfo">错误信息的数组，第i个元素表示第i个委托的错误信息. 此API执行返回后，ErrInfo[i]含义同上。</param>
/// void  QueryDatas(int ClientID, int Category[], int Count, char* Result[], char* ErrInfo[]);
typedef void(__stdcall* QueryDatasDelegate)(int ClientID, int Category[], int Count, char* Result[], char* ErrInfo[]);


/// <summary>
/// 下委托交易证券
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Category">表示委托的种类，0买入 1卖出  2融资买入  3融券卖出   4买券还券   5卖券还款  6现券还券</param>
/// <param name="PriceType">表示报价方式 0上海限价委托 深圳限价委托 1(市价委托)深圳对方最优价格  2(市价委托)深圳本方最优价格  3(市价委托)深圳即时成交剩余撤销  4(市价委托)上海五档即成剩撤 深圳五档即成剩撤 5(市价委托)深圳全额成交或撤销 6(市价委托)上海五档即成转限价
/// <param name="Gddm">股东代码, 交易上海股票填上海的股东代码；交易深圳的股票填入深圳的股东代码</param>
/// <param name="Zqdm">证券代码</param>
/// <param name="Price">委托价格</param>
/// <param name="Quantity">委托数量</param>
/// <param name="Result">同上,其中含有委托编号数据</param>
/// <param name="ErrInfo">同上</param>
/// void  SendOrder(int ClientID, int Category ,int PriceType,  char* Gddm,  char* Zqdm , float Price, int Quantity,  char* Result, char* ErrInfo);
typedef void(__stdcall* SendOrderDelegate)(int ClientID, int Category, int PriceType, char* Gddm, char* Zqdm, float Price, int Quantity, char* Result, char* ErrInfo);



/// <summary>
/// 属于普通批量版功能,批量下委托交易证券，用数组传入每个委托的参数，数组第i个元素表示第i个委托的相应参数
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Category">委托种类的数组，第i个元素表示第i个委托的种类，0买入 1卖出  2融资买入  3融券卖出   4买券还券   5卖券还款  6现券还券</param>
/// <param name="PriceType">表示报价方式的数组,  第i个元素表示第i个委托的报价方式, 0上海限价委托 深圳限价委托 1(市价委托)深圳对方最优价格  2(市价委托)深圳本方最优价格  3(市价委托)深圳即时成交剩余撤销  4(市价委托)上海五档即成剩撤 深圳五档即成剩撤 5(市价委托)深圳全额成交或撤销 6(市价委托)上海五档即成转限价
/// <param name="Gddm">股东代码数组，第i个元素表示第i个委托的股东代码，交易上海股票填上海的股东代码；交易深圳的股票填入深圳的股东代码</param>
/// <param name="Zqdm">证券代码数组，第i个元素表示第i个委托的证券代码</param>
/// <param name="Price">委托价格数组，第i个元素表示第i个委托的委托价格</param>
/// <param name="Quantity">委托数量数组，第i个元素表示第i个委托的委托数量</param>
/// <param name="Count">委托的个数，即数组的长度</param>
/// <param name="Result">同上</param>
/// <param name="ErrInfo">同上</param>
/// void  SendOrders(int ClientID, int Category[] , int PriceType[], char* Gddm[],  char* Zqdm[] , float Price[], int Quantity[],  int Count, char* Result[], char* ErrInfo[]);
typedef void(__stdcall* SendOrdersDelegate)(int ClientID, int Category[], int PriceType[], char* Gddm[], char* Zqdm[], float Price[], int Quantity[], int Count, char* Result[], char* ErrInfo[]);



/// <summary>
/// 撤委托
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="ExchangeID">交易所ID， 上海1，深圳0(招商证券普通账户深圳是2)</param>
/// <param name="hth">表示要撤的目标委托的编号</param>
/// <param name="Result">同上</param>
/// <param name="ErrInfo">同上</param>
/// void  CancelOrder(int ClientID, char* ExchangeID, char* hth, char* Result, char* ErrInfo);
typedef void(__stdcall* CancelOrderDelegate)(int ClientID, char* ExchangeID, char* hth, char* Result, char* ErrInfo);




/// <summary>
/// 属于普通批量版功能,批量撤委托, 用数组传入每个委托的参数，数组第i个元素表示第i个撤委托的相应参数
/// </summary>
/// <param name="ClientID">客户端ID</param>
// <param name="ExchangeID">交易所ID， 上海1，深圳0(招商证券普通账户深圳是2)</param>
/// <param name="hth">表示要撤的目标委托的编号</param>
/// <param name="Count">撤委托的个数，即数组的长度</param>
/// <param name="Result">同上</param>
/// <param name="ErrInfo">同上</param>
/// void  CancelOrders(int ClientID, char* ExchangeID[], char* hth[], int Count, char* Result[], char* ErrInfo[]);
typedef void(__stdcall* CancelOrdersDelegate)(int ClientID, char* ExchangeID[], char* hth[], int Count, char* Result[], char* ErrInfo[]);




/// <summary>
/// 获取证券的实时五档行情
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Zqdm">证券代码</param>
/// <param name="Result">同上</param>
/// <param name="ErrInfo">同上</param>
/// void  GetQuote(int ClientID, char* Zqdm, char* Result, char* ErrInfo);
typedef void(__stdcall* GetQuoteDelegate)(int ClientID, char* Zqdm, char* Result, char* ErrInfo);
///证券代码	证券名称	昨收价	今开价	国债利息	当前价	买一价	买二价	买三价	买四价	买五价	买一量	买二量	买三量	买四量	买五量	卖一价	卖二价	卖三价	卖四价	卖五价	卖一量	卖二量	卖三量	卖四量	卖五量	交易所代码	最小交易股数	最小买入变动价位	最小卖出变动价位	帐号类别	币种	国债标识	涨停价格	跌停价格	取柜台行情	保留信息
///300003	乐普医疗	33.70	33.35	0.00		33.37	33.37	33.36	33.35	33.34	33.33	475		61		28		14		264		33.38	33.39	33.40	33.41	33.42	347		19		236		216		20			0			100				0.01				0.01				0			0	255

/// <summary>
/// 属于普通批量版功能,批量获取证券的实时五档行情
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Zqdm">证券代码</param>
/// <param name="Result">同上</param>
/// <param name="ErrInfo">同上</param>
/// void  GetQuotes(int ClientID, char* Zqdm[], int Count, char* Result[], char* ErrInfo[]);
typedef void(__stdcall* GetQuotesDelegate)(int ClientID, char* Zqdm[], int Count, char* Result[], char* ErrInfo[]);



/// <summary>
/// 融资融券直接还款
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Amount">还款金额</param>
/// <param name="Result">同上</param>
/// <param name="ErrInfo">同上</param>
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
#ifdef ACC_PINAN//平安交易
	char *m_ip = "202.69.19.56";
	int m_port = 7708;
	char *m_accout = "309619426390";
	char *m_pwd = "781813";
	CString m_dll_lib = _T("Trade_pa.dll");
#endif
#ifdef ACC_PINAN2//平安交易
	char *m_ip = "202.69.19.56";
	int m_port = 7708;
    char *m_accout = "308919343982";
    char *m_pwd = "201637";
	CString m_dll_lib = _T("Trade_pa2.dll");
#endif
#ifdef ACC_ZAHOSHANG ////招商交易
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


	//以下是普通批量版功能函数
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



