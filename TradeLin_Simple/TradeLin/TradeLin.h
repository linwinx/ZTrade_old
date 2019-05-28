#pragma once



#include <windows.h>
#include <winbase.h>
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

// ����ˢ�����ݽṹ CStock::dataReport
typedef struct report_t {
	char	m_szCode[STKLIB_MAX_CODE];		// ֤ȯ����,��'\0'��β
	char	m_szName[STKLIB_MAX_NAME];		// ֤ȯ����,��'\0'��β

	float	m_fLast;					// ���գ�Ԫ��
	float	m_fOpen;					// �񿪣�Ԫ��
	float   m_fZai;                     //��ծ��Ϣ
	float	m_fNew;						// ���£�Ԫ��

	float	m_fBuyPrice[5];				// �����1,2,3,4��Ԫ��
	int	m_fBuyVolume[5];			// ������1,2,3,4���ɣ�
	float	m_fSellPrice[5];			// ������1,2,3,4��Ԫ��
	int	m_fSellVolume[5];			// ������1,2,3,4���ɣ�

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