#pragma once



#include <windows.h>
#include <winbase.h>
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

// 行情刷新数据结构 CStock::dataReport
typedef struct report_t {
	char	m_szCode[STKLIB_MAX_CODE];		// 证券代码,以'\0'结尾
	char	m_szName[STKLIB_MAX_NAME];		// 证券名称,以'\0'结尾

	float	m_fLast;					// 昨收（元）
	float	m_fOpen;					// 今开（元）
	float   m_fZai;                     //国债利息
	float	m_fNew;						// 最新（元）

	float	m_fBuyPrice[5];				// 申买价1,2,3,4（元）
	int	m_fBuyVolume[5];			// 申买量1,2,3,4（股）
	float	m_fSellPrice[5];			// 申卖价1,2,3,4（元）
	int	m_fSellVolume[5];			// 申卖量1,2,3,4（股）

	int m_jysCode;
	int m_minNum;
	float m_minBuyUnit;
	float m_minSellUnit;
	int m_accountType;
	int m_biType;
	int m_zaiMark;
	float m_zhangTing;
	float m_dieTing;
	int   m_isGetHq;
	char m_revInfo[STKLIB_MAX_NAME];

} REPORT, *PREPORT;

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