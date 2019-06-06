#ifndef _CCOMMON_H_
#define _CCOMMON_H_
#include <winsock2.h>

enum ZT_TCP_TYPE{
	ZT_BUY = 1,//1 ����ָ����Ʊ
	ZT_SELL, //2 ����ָ����Ʊ
	ZT_QUERY_STOCK,//3 ��ѯָ����Ʊ����Ϣ
	ZT_QUERY_REVER,//4 ��ѯָ����Ʊ�Ŀɳ�����Ϣ
	ZT_QUERY_MONEY,//5 ��ѯ�˻��ʲ�
	ZT_REVER,//6 ����ָ����Ʊ������
	ZT_SELL_CLEAR = 9,//9 ���

	ZT_START = 12,//ָ����Ʊ�����Զ�����
	ZT_STOP = 13,//ָ����Ʊֹͣ�Զ�����
	ZT_REFRESH = 14,//ˢ��ָ����Ʊ�ֲ�

	ZT_LOGTXT = 16,//16 ����ֵд����ǰ��־�ļ���������
	ZT_MSGBOX = 17,//17 ����ֵ����Ϣ�����ʽ���������������

	ACCOUNT_LOGIN = 90,

	TCP_PKG_TYPE_MAX = 99,
};

enum QUERY_STOCK_TYPE{//3 ��ѯָ����Ʊ����Ϣ
	ZQ_CHICANG,//��ѯ�ֲ�����
	ZQ_KEMAIN, //��ѯ��������
	ZQ_CHENGBEN, //��ѯ�ɱ���
	ZQ_YINGLI_MONEY,//��ѯӯ�����
	ZQ_YINGLI_PERCENT,//��ѯӯ���ٷֱ�
	ZQ_CHIANG_MONEY,//��ѯ�ֲ���ֵ
	ZQ_XIADAN_DAYS,//�µ�����
	ZQ_CHICANG_DAYS,//�ֲ�����
	ZQ_PRE_BUY_PRICE,�ϴ������
};

enum QUERY_REVER_TYPE{//4 ��ѯָ����Ʊ�Ŀɳ�����Ϣ
	ZQ_REVER_BUY,//��ѯ�ɳ���
	ZQ_REVER_BUY_TIME,//��ѯ�ɳ���ʱ��
	ZQ_REVER_SELL,//��ѯ�ɳ�����
	ZQ_REVER_SELL_TIME,//��ѯ�ɳ�����ʱ��
};

enum QUERY_MONEY_TYPE{//5 ��ѯ�˻��ʲ�
	ZQ_ALL_MONEY,//�˻����ʲ�
	ZQ_VALID_MONEY,//�˻������ʽ�
	ZQ_CHICANG_MONEY,//�˻��ֲ���ֵ
	ZQ_ALL_YINGLI,//�˻���ӯ��
	ZQ_CHICANG_NUM,//�ֲָ���
};

enum ZT_REVER_TYPE{//6 ����ָ����Ʊ������
	Z_CANCEL_BUY = 1,//1�����򵥣�
	Z_CANCEL_SELL,//2��������
};

enum ZT_SELL_CLEAR{//9 ��ֱ���
	Z_SELL_CLEAR_2 = 2,//20%
	Z_SELL_CLEAR_3 = 3,//30%
	Z_SELL_CLEAR_5 = 5,//50%
	Z_SELL_CLEAR_8 = 8,//80%
	Z_SELL_CLEAR_10 = 10,//100%
};

enum ZT_TCP_STATUS{
	Z_FAIL = 0,//ʧ��
	Z_OK = 1,//�ɹ�
};

//Account Struct
#define MAX_ACCOUNT_NUM 10
typedef struct _AccountInfo_
{
	short        accountId; //�˺�����
	char         accountName[32]; //��ʵ��Ʊ�˺�
	char         accountPwd[32];//��ʵ��Ʊ�˺�����
}AccountInfo;

//Socket list Struct
#define MAX_SOCKET_NUM 64
typedef struct _SocketList_
{
	SOCKET       clientSocket; //�󶨵�socket
	LONG         clientAddr;
	struct _SocketList_    *next;
}SocketList;

//TCP Stock pkg Struct
typedef struct _StockInfo_
{
	char         stockId[8]; //��Ʊ����
	short        stockQuantity; //��Ʊ����
	float        stockPrice; //��Ʊ�۸�
	short        query_value; //���ز�ѯֵ
}StockInfo;

//Stock Struct
typedef struct _tcp_pkg_
{
	ZT_TCP_TYPE  tcpCmdType; //��������
	AccountInfo account;
	StockInfo stock;
	ZT_TCP_STATUS status;
}TcpPkgInfo;

#endif