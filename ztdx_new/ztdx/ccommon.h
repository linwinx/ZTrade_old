#ifndef _CCOMMON_H_
#define _CCOMMON_H_
#include <winsock2.h>

enum ZT_TCP_TYPE{
	ZT_BUY = 1,//1 买入指定股票
	ZT_SELL, //2 卖出指定股票
	ZT_QUERY_STOCK,//3 查询指定股票的信息
	ZT_QUERY_REVER,//4 查询指定股票的可撤消信息
	ZT_QUERY_MONEY,//5 查询账户资产
	ZT_REVER,//6 撤消指定股票买卖单
	ZT_SELL_CLEAR = 9,//9 清仓

	ZT_START = 12,//指定股票启动自动交易
	ZT_STOP = 13,//指定股票停止自动交易
	ZT_REFRESH = 14,//刷新指定股票持仓

	ZT_LOGTXT = 16,//16 把数值写到当前日志文件里，方便调试
	ZT_MSGBOX = 17,//17 把数值以消息框的形式弹出来，方便调试

	ACCOUNT_LOGIN = 90,

	TCP_PKG_TYPE_MAX = 99,
};

enum QUERY_STOCK_TYPE{//3 查询指定股票的信息
	ZQ_CHICANG,//查询持仓数量
	ZQ_KEMAIN, //查询可卖数量
	ZQ_CHENGBEN, //查询成本价
	ZQ_YINGLI_MONEY,//查询盈利金额
	ZQ_YINGLI_PERCENT,//查询盈利百分比
	ZQ_CHIANG_MONEY,//查询持仓市值
	ZQ_XIADAN_DAYS,//下单天数
	ZQ_CHICANG_DAYS,//持仓天数
	ZQ_PRE_BUY_PRICE,上次买入价
};

enum QUERY_REVER_TYPE{//4 查询指定股票的可撤消信息
	ZQ_REVER_BUY,//查询可撤买单
	ZQ_REVER_BUY_TIME,//查询可撤买单时间
	ZQ_REVER_SELL,//查询可撤卖单
	ZQ_REVER_SELL_TIME,//查询可撤卖单时间
};

enum QUERY_MONEY_TYPE{//5 查询账户资产
	ZQ_ALL_MONEY,//账户总资产
	ZQ_VALID_MONEY,//账户可用资金
	ZQ_CHICANG_MONEY,//账户持仓市值
	ZQ_ALL_YINGLI,//账户总盈利
	ZQ_CHICANG_NUM,//持仓个数
};

enum ZT_REVER_TYPE{//6 撤消指定股票买卖单
	Z_CANCEL_BUY = 1,//1，撤买单；
	Z_CANCEL_SELL,//2，撤卖单
};

enum ZT_SELL_CLEAR{//9 清仓比例
	Z_SELL_CLEAR_2 = 2,//20%
	Z_SELL_CLEAR_3 = 3,//30%
	Z_SELL_CLEAR_5 = 5,//50%
	Z_SELL_CLEAR_8 = 8,//80%
	Z_SELL_CLEAR_10 = 10,//100%
};

enum ZT_TCP_STATUS{
	Z_FAIL = 0,//失败
	Z_OK = 1,//成功
};

//Account Struct
#define MAX_ACCOUNT_NUM 10
typedef struct _AccountInfo_
{
	short        accountId; //账号序列
	char         accountName[32]; //真实股票账号
	char         accountPwd[32];//真实股票账号密码
}AccountInfo;

//Socket list Struct
#define MAX_SOCKET_NUM 64
typedef struct _SocketList_
{
	SOCKET       clientSocket; //绑定的socket
	LONG         clientAddr;
	struct _SocketList_    *next;
}SocketList;

//TCP Stock pkg Struct
typedef struct _StockInfo_
{
	char         stockId[8]; //股票编码
	short        stockQuantity; //股票数量
	float        stockPrice; //股票价格
	short        query_value; //返回查询值
}StockInfo;

//Stock Struct
typedef struct _tcp_pkg_
{
	ZT_TCP_TYPE  tcpCmdType; //命令类型
	AccountInfo account;
	StockInfo stock;
	ZT_TCP_STATUS status;
}TcpPkgInfo;

#endif