// TradeLin.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TradeLin.h"

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
///����ɹ������ַ���Result�����˽������,��ʽΪ�������ݣ�������֮��ͨ��\n�ַ��ָ������֮��ͨ��\t�ָ���
///Result��\n��\t�ָ��������ַ����������ѯ�ɶ�����ʱ���صĽ���ַ������� 

///"�ɶ�����\t�ɶ�����\t�ʺ����\t������Ϣ\n
///0000064567\t\t0\t\nA000064567\t\t1\t\n
///2000064567\t\t2\t\nB000064567\t\t3\t"

///��ô�����֮��ͨ���ָ��ַ����� ���Իָ�Ϊ���м��еı�����ʽ������



//2.APIʹ������Ϊ: Ӧ�ó����ȵ���OpenTdx��ͨ����ʵ����һ��ʵ���¿���ͬʱ��¼��������˻���ÿ�������˻���֮ΪClientID.
//ͨ������Logon���ClientID��Ȼ����Ե�������API���������ClientID���в�ѯ���µ�; Ӧ�ó����˳�ʱӦ����Logoffע��ClientID, ������CloseTdx�ر�ͨ����ʵ��. 
//OpenTdx��CloseTdx������Ӧ�ó�����ֻ�ܱ�����һ��.API���ж����Զ��������ܣ�Ӧ�ó���ֻ�����API�������صĳ�����Ϣ�����ʵ����������ɡ�


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
/// <param name="TradeAccount">�����˺ţ�һ�����¼�ʺ���ͬ. ���¼ȯ��ͨ������������ѯ�ɶ��б����ɶ��б��ڵ��ʽ��ʺž��ǽ����ʺ�, �����ѯ���������վ���ȵ��ʴ���Ŀ</param>
/// <param name="JyPassword">��������</param>
/// <param name="TxPassword">ͨѶ����</param>
/// <param name="ErrInfo">��APIִ�з��غ���������������˴�����Ϣ˵����һ��Ҫ����256�ֽڵĿռ䡣û����ʱΪ���ַ�����</param>
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
/// <param name="Result">��APIִ�з��غ�Result�ڱ����˷��صĲ�ѯ����, ��ʽΪ�������ݣ�������֮��ͨ��\n�ַ��ָ������֮��ͨ��\t�ָ���һ��Ҫ����1024*1024�ֽڵĿռ䡣����ʱΪ���ַ�����</param>
/// <param name="ErrInfo">ͬLogon������ErrInfo˵��</param>
/// void  QueryData(int ClientID, int Category, char* Result, char* ErrInfo);
typedef void(__stdcall* QueryDataDelegate)(int ClientID, int Category, char* Result, char* ErrInfo);


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





int _tmain(int argc, _TCHAR* argv[])
{
	//����dll, ����4��dll��Ҫ���Ƶ�debug��releaseĿ¼��,������ö��ֽ��ַ����������
	//HMODULE HMODULE1 = LoadLibrary("Trade_zs.dll");//zhaoshang
	HMODULE HMODULE1 = LoadLibrary("Trade_pa.dll"); //pingan

	//��ȡapi����
	OpenTdxDelegate OpenTdx = (OpenTdxDelegate)GetProcAddress(HMODULE1, "OpenTdx");
	CloseTdxDelegate CloseTdx = (CloseTdxDelegate)GetProcAddress(HMODULE1, "CloseTdx");
	LogonDelegate Logon = (LogonDelegate)GetProcAddress(HMODULE1, "Logon");
	LogoffDelegate Logoff = (LogoffDelegate)GetProcAddress(HMODULE1, "Logoff");
	QueryDataDelegate QueryData = (QueryDataDelegate)GetProcAddress(HMODULE1, "QueryData");
	SendOrderDelegate SendOrder = (SendOrderDelegate)GetProcAddress(HMODULE1, "SendOrder");
	CancelOrderDelegate CancelOrder = (CancelOrderDelegate)GetProcAddress(HMODULE1, "CancelOrder");
	GetQuoteDelegate GetQuote = (GetQuoteDelegate)GetProcAddress(HMODULE1, "GetQuote");
	RepayDelegate Repay = (RepayDelegate)GetProcAddress(HMODULE1, "Repay");


	//��������ͨ�����湦�ܺ���
	QueryDatasDelegate QueryDatas = (QueryDatasDelegate)GetProcAddress(HMODULE1, "QueryDatas");
	QueryHistoryDataDelegate QueryHistoryData = (QueryHistoryDataDelegate)GetProcAddress(HMODULE1, "QueryHistoryData");
	SendOrdersDelegate SendOrders = (SendOrdersDelegate)GetProcAddress(HMODULE1, "SendOrders");
	CancelOrdersDelegate CancelOrders = (CancelOrdersDelegate)GetProcAddress(HMODULE1, "CancelOrders");
	GetQuotesDelegate GetQuotes = (GetQuotesDelegate)GetProcAddress(HMODULE1, "GetQuotes");

	//�����Ǹ߼������湦�ܺ���
//	QueryMultiAccountsDatasDelegate QueryMultiAccountsDatas = (QueryMultiAccountsDatasDelegate)GetProcAddress(HMODULE1, "QueryMultiAccountsDatas");
//	SendMultiAccountsOrdersDelegate SendMultiAccountsOrders = (SendMultiAccountsOrdersDelegate)GetProcAddress(HMODULE1, "SendMultiAccountsOrders");
//	CancelMultiAccountsOrdersDelegate CancelMultiAccountsOrders = (CancelMultiAccountsOrdersDelegate)GetProcAddress(HMODULE1, "CancelMultiAccountsOrders");
//	GetMultiAccountsQuotesDelegate GetMultiAccountsQuotes = (GetMultiAccountsQuotesDelegate)GetProcAddress(HMODULE1, "GetMultiAccountsQuotes");


	//��ʼ����
	char* Result = new char[1024 * 1024];
	char* ErrInfo = new char[256];

	int Categorys[4] = {1,1,1,1};
	char Results[4] ;
	char ErrInfos[4] ;



	OpenTdx();//��ͨ����,OpenTdx������Ӧ�ó�����ֻ�ܱ�����һ��,���ܶ�ε���


	//��¼�ʺ�
	//int ClientID = Logon("58.249.119.236", 7709, "6.20", 0, "97720035", "97720035", "781813", "", ErrInfo); //��������
	//int ClientID = Logon("58.251.16.167", 443, "6.20", 0, "97720035", "97720035", "781813", "", ErrInfo); //���̽���
	int ClientID = Logon("202.69.19.56", 7708, "6.20", 0, "309619426390", "309619426390", "781813", "", ErrInfo); //ƽ������

	//��¼�ڶ����ʺ�
	//int ClientID2 = Logon("111.111.111.111", 7708, "4.20", 0, "33333333","33333333",  "333", "", ErrInfo);
	if (ClientID == -1)
	{
		cout << ErrInfo << endl;//��¼ʧ��
	}
	else
	{
		//��¼�ɹ�
	QueryData(ClientID, 1, Result, ErrInfo);//��ѯ�ʽ�
		//QueryData(ClientID2, 0, Result, ErrInfo);//�ڶ����ʺ�,��ѯ�ʽ�
		cout << Result << "| " << ErrInfo;

		//QueryDatas(ClientID, Categorys, 5, Results, ErrInfos);//��ѯ�ʽ�
		//QueryData(ClientID2, 0, Result, ErrInfo);//�ڶ����ʺ�,��ѯ�ʽ�
		//cout << Result << "| " << ErrInfo;

		//GetQuote(ClientID,"002245", Result, ErrInfo);
		//cout << Result << "| " << ErrInfo;

		/*
		SendOrder(ClientID, 0, 0, "A111111", "601988", 2.7f, 100, Result, ErrInfo);//�����Ʊ
		cout << "�����µ����:" << Result << " " << ErrInfo;

		//��ͨ������, ������50����
		const int Count = 50;

		int Category[Count];
		int PriceType[Count];
		char* Gddm[Count];
		char* Zqdm[Count];
		float Price[Count];
		int Quantity[Count];

		char* Result1[Count];
		char* ErrInfo1[Count];

		for (int i = 0; i < Count; i++)
		{
			Category[i] = 0;//��i���������
			PriceType[i] = 0;//��i�����ı��۷�ʽ
			Gddm[i] = "A11111";//��i�����Ĺɶ�����
			Zqdm[i] = "601988";//��i������֤ȯ����
			Price[i] = 2.27f;//��i�����ļ۸�
			Quantity[i] = 100;//��i����������
			Result1[i] = new char[1024];//�����i�������µ����
			ErrInfo1[i] = new char[256];//�����i�������µ�������Ϣ
		}

		SendOrders(ClientID, Category, PriceType, Gddm, Zqdm, Price, Quantity, Count, Result1, ErrInfo1);


		for (int i = 0; i < Count; i++)
		{
			cout << "��" << i << "�����:" << Result1[i] << " " << ErrInfo1[i];

			delete[] Result1[i];
			delete[] ErrInfo1[i];
		}

		*/

		Logoff(ClientID);//ע��
	}


	CloseTdx();//�ر�ͨ����,CloseTdx������Ӧ�ó�����ֻ�ܱ�����һ��,���ܶ�ε���

	delete[] Result;
	delete[] ErrInfo;



	FreeLibrary(HMODULE1);
	while (1) {};

	return 0;
}


