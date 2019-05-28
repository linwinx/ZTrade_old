#include "stdafx.h"
#include "TradeApi.h"

void TradeApi::Login()
{
	//开始交易
	char* ErrInfo = new char[256];

	OpenTdx();//打开通达信,OpenTdx在整个应用程序中只能被调用一次,不能多次调用

	//登录帐号
	m_ClientID = Logon(m_ip, m_port, m_dll_ver, 0, m_accout, m_accout, m_pwd, "", ErrInfo); //平安交易

	//登录第二个帐号
	//int ClientID2 = Logon("111.111.111.111", 7708, "4.20", 0, "33333333","33333333",  "333", "", ErrInfo);
	if (m_ClientID == -1)
	{
		TRACE(ErrInfo);
		TRACE("\n");
	}
	else
	{
		TRACE("Login m_ClientID = %d\n", m_ClientID);
	}

	delete[] ErrInfo;
}

TradeApi::TradeApi()
{
	//载入dll, 所有4个dll都要复制到debug和release目录下,必须采用多字节字符集编程设置
	HMODULE1 = LoadLibrary(m_dll_lib); //pingan

	//获取api函数
	 OpenTdx = (OpenTdxDelegate)GetProcAddress(HMODULE1, "OpenTdx");
	 CloseTdx = (CloseTdxDelegate)GetProcAddress(HMODULE1, "CloseTdx");
	 Logon = (LogonDelegate)GetProcAddress(HMODULE1, "Logon");
	 Logoff = (LogoffDelegate)GetProcAddress(HMODULE1, "Logoff");
	 QueryData = (QueryDataDelegate)GetProcAddress(HMODULE1, "QueryData");
	 SendOrder = (SendOrderDelegate)GetProcAddress(HMODULE1, "SendOrder");
	 CancelOrder = (CancelOrderDelegate)GetProcAddress(HMODULE1, "CancelOrder");
	 GetQuote = (GetQuoteDelegate)GetProcAddress(HMODULE1, "GetQuote");
	 Repay = (RepayDelegate)GetProcAddress(HMODULE1, "Repay");


	//以下是普通批量版功能函数
	 QueryDatas = (QueryDatasDelegate)GetProcAddress(HMODULE1, "QueryDatas");
	 QueryHistoryData = (QueryHistoryDataDelegate)GetProcAddress(HMODULE1, "QueryHistoryData");
	 SendOrders = (SendOrdersDelegate)GetProcAddress(HMODULE1, "SendOrders");
	 CancelOrders = (CancelOrdersDelegate)GetProcAddress(HMODULE1, "CancelOrders");
	 GetQuotes = (GetQuotesDelegate)GetProcAddress(HMODULE1, "GetQuotes");

	//以下是高级批量版功能函数
	//	QueryMultiAccountsDatasDelegate QueryMultiAccountsDatas = (QueryMultiAccountsDatasDelegate)GetProcAddress(HMODULE1, "QueryMultiAccountsDatas");
	//	SendMultiAccountsOrdersDelegate SendMultiAccountsOrders = (SendMultiAccountsOrdersDelegate)GetProcAddress(HMODULE1, "SendMultiAccountsOrders");
	//	CancelMultiAccountsOrdersDelegate CancelMultiAccountsOrders = (CancelMultiAccountsOrdersDelegate)GetProcAddress(HMODULE1, "CancelMultiAccountsOrders");
	//	GetMultiAccountsQuotesDelegate GetMultiAccountsQuotes = (GetMultiAccountsQuotesDelegate)GetProcAddress(HMODULE1, "GetMultiAccountsQuotes");
	// Login();
}

void TradeApi::Logout()
{
	//TRACE("Logout m_ClientID = %d\n", m_ClientID);
	Logoff(m_ClientID);//注销
	CloseTdx();//关闭通达信,CloseTdx在整个应用程序中只能被调用一次,不能多次调用
}

TradeApi::~TradeApi()
{
	//Logout();
	FreeLibrary(HMODULE1);
}
CString TradeApi::GetZqZjStr()
{
	CString t_zjinfo;
	//t_zjinfo.Format("可用：%s", m_zjInfo.zKyzj);
	t_zjinfo.Format("总: %s  |  可用: %s", m_zjInfo.zTotalzj, m_zjInfo.zKyzj);
	return t_zjinfo;
}

CString TradeApi::GetShszStr()
{
	REPORT t_shInfo, t_szInfo;
	CString t_zjinfo;
	
	t_shInfo = GetZqInfoSina(_T("999999"));
	t_szInfo = GetZqInfoSina(_T("399001"));

	m_shRate = (atof(t_shInfo.zNew) / atof(t_shInfo.zLast) - 1.0) * 100.0;
	m_szRate = (atof(t_szInfo.zNew) / atof(t_szInfo.zLast) - 1.0) * 100.0;

	//TRACE("GetShszStr %.2f    %.2f\n", m_shRate, m_szRate);

	t_zjinfo.Format("上证 %.2f  %.2f%%  |  深证 %.2f  %.2f%%", atof(t_shInfo.zNew), m_shRate, atof(t_szInfo.zNew), m_szRate);
	return t_zjinfo;
}

CString TradeApi::GetZjzhStr()
{
	ZJREPORT zjInfo;
	CString t_zjinfo;
	t_zjinfo.Format("帐号: %s", m_zjInfo.zZjzh);
	return t_zjinfo;
}

void TradeApi::GetZqZj()
{
	char* Result = new char[1024 * 1024];
	char* ErrInfo = new char[256];

	int i;
	CString t_errInfo;
	//登录成功
	QueryData(m_ClientID, Q_ZI_JING, Result, ErrInfo);//查询资金
	
	t_errInfo.Format(ErrInfo);
	if (!t_errInfo.IsEmpty())
	{
		TRACE("Get GetZqZj error");
		return;
	}
	CString str;
	str.Format(_T("%s"), Result);
	//TRACE("\n%s\n",Result);

	CString output;
	AfxExtractSubString(output, (LPCTSTR)str, 1, '\n');
	AfxExtractSubString(m_zjInfo.zZjzh, (LPCTSTR)output, 0, '\t');
	if (m_zjInfo.zZjzh.IsEmpty())
		AfxExtractSubString(output, (LPCTSTR)str, 2, '\n');
	//TRACE("\n%s\n", output);

	AfxExtractSubString(m_zjInfo.zZjzh, (LPCTSTR)output, 0, '\t');
	//TRACE("\n%s\n", zjInfo.zZjzh);
	AfxExtractSubString(m_zjInfo.zKyzj, (LPCTSTR)output, 3, '\t');
	//TRACE("\n%s\n", zjInfo.zKyzj);
	AfxExtractSubString(m_zjInfo.zTotalzj, (LPCTSTR)output, 8, '\t');
	//TRACE("\n%s\n", zjInfo.zTotalzj);

	delete[] Result;
	delete[] ErrInfo;
}

void TradeApi::GetChicangInfo()
{
	char* Result = new char[1024 * 1024];
	char* ErrInfo = new char[256];

	//登录成功
	QueryData(m_ClientID, Q_GU_FEN, Result, ErrInfo);//查询资金
	//TRACE("%s\n",Result);


	CString str;
	str.Format(_T("%s"), Result);
	CString output;
	//TRACE("%s\n", str);
	CString tmp_num_str;
	int tmp_num;
	m_stock_num = 0;
	int t_stock_num = 0;
	CString m_report_str[MAX_STOCK_NUM];

	int i;
	for (i = 0; i <MAX_STOCK_NUM; i++)
	{
		AfxExtractSubString(output, (LPCTSTR)str, i, '\n');
		if (i != 0) {
			if (output.IsEmpty())
				break;
			m_report_str[t_stock_num] = output;
			t_stock_num += 1;
			// TRACE("%s\n", output);
		}
	}

	for (i = 0; i < t_stock_num; i++)
	{
		AfxExtractSubString(tmp_num_str, (LPCTSTR)m_report_str[i], 2, '\t');
		tmp_num = atoi(tmp_num_str);
		if (tmp_num % 100 == 0 && tmp_num != 0)
		{
			AfxExtractSubString(m_report[m_stock_num].zCode, (LPCTSTR)m_report_str[i], 0, '\t');
			AfxExtractSubString(m_report[m_stock_num].zName, (LPCTSTR)m_report_str[i], 1, '\t');
			AfxExtractSubString(m_report[m_stock_num].zAllNum, (LPCTSTR)m_report_str[i], 2, '\t');
			AfxExtractSubString(m_report[m_stock_num].zValidNum, (LPCTSTR)m_report_str[i], 3, '\t');
			AfxExtractSubString(m_report[m_stock_num].zBuyPrice, (LPCTSTR)m_report_str[i], 4, '\t');
			AfxExtractSubString(m_report[m_stock_num].zNewPrice, (LPCTSTR)m_report_str[i], 5, '\t');
			AfxExtractSubString(m_report[m_stock_num].zNowValue, (LPCTSTR)m_report_str[i], 6, '\t');
			AfxExtractSubString(m_report[m_stock_num].zProfit, (LPCTSTR)m_report_str[i], 7, '\t');
			AfxExtractSubString(m_report[m_stock_num].zProfitRate, (LPCTSTR)m_report_str[i], 8, '\t');
			AfxExtractSubString(m_report[m_stock_num].zGddm, (LPCTSTR)m_report_str[i], 13, '\t');

			if (m_report[m_stock_num].zNewPrice > m_report[i].zHighPrice) m_report[i].zHighPrice = m_report[i].zNewPrice;
			if (m_report[m_stock_num].zNewPrice < m_report[i].zLowPrice) m_report[i].zLowPrice = m_report[i].zNewPrice;
			m_stock_num++;
		}
	}
	//i = 2;
	//TRACE("%s %s %s %s %s %s %s\n", m_report[i].zCode, m_report[i].zName, m_report[i].zAllNum, m_report[i].zValidNum, m_report[i].zBuyPrice, m_report[i].zCurrentPrice, m_report[i].zNowValue);

delete[] Result;
delete[] ErrInfo;

}

void TradeApi::GetChengjiaoInfo()
{
	char* Result = new char[1024 * 1024];
	char* ErrInfo = new char[256];
	CString t_CjSum00 = _T("0.00");
	CString tmp_CJsum;


	//char StartDate[32];
	//char EndDate[32];
	//sprintf_s(EndDate, "%s", GetDateInfo());

	//登录成功
	QueryData(m_ClientID, Q_TODAY_CHENGJIAO, Result, ErrInfo);//查询资金

	//QueryHistoryData(m_ClientID, QH_CHENGJIAO, StartDate, EndDate, Result, ErrInfo);
	//TRACE("%s\n",Result);

	CString str;
	str.Format(_T("%s"), Result);
	CString output;
	//TRACE("%s\n", str);
	int t_cj_num = 0;
	m_cj_num = 0;
	int i;

	CString cj_report_str[MAX_STOCK_NUM];

	for (i = 0; i <MAX_STOCK_NUM; i++)
	{

		AfxExtractSubString(output, (LPCTSTR)str, i, '\n');
		if (i != 0) {
			if (output.IsEmpty())
				break;
			cj_report_str[t_cj_num] = output;
			t_cj_num += 1;
			// TRACE("%s\n", output);
		}
	}

	for (i = 0; i < t_cj_num; i++)
	{
		AfxExtractSubString(tmp_CJsum, (LPCTSTR)cj_report_str[i], 11, '\t');
		if (t_CjSum00 != tmp_CJsum)
		{
			AfxExtractSubString(cj_report[m_cj_num].zWtdm, (LPCTSTR)cj_report_str[i], 1, '\t');
			AfxExtractSubString(cj_report[m_cj_num].zZqdm, (LPCTSTR)cj_report_str[i], 2, '\t');
			AfxExtractSubString(cj_report[m_cj_num].zName, (LPCTSTR)cj_report_str[i], 3, '\t');
			AfxExtractSubString(cj_report[m_cj_num].zCjFlag, (LPCTSTR)cj_report_str[i], 5, '\t');
			//TRACE("%s %s\n", cj_report[m_cj_num].zZqdm, cj_report[m_cj_num].zCjFlag);

			AfxExtractSubString(cj_report[m_cj_num].zCjPrice, (LPCTSTR)cj_report_str[i], 9, '\t');
			AfxExtractSubString(cj_report[m_cj_num].zCjNum, (LPCTSTR)cj_report_str[i], 10, '\t');
			AfxExtractSubString(cj_report[m_cj_num].zCjSum, (LPCTSTR)cj_report_str[i], 11, '\t');
			AfxExtractSubString(cj_report[m_cj_num].zCjTime, (LPCTSTR)cj_report_str[i], 12, '\t');
			m_cj_num++;


			//TRACE("%s %s %s %s %s %s %s %s\n", cj_report[i].zWtdm, cj_report[i].zZqdm, cj_report[i].zName, cj_report[i].zCjFlag, cj_report[i].zCjPrice, cj_report[i].zCjNum, cj_report[i].zCjSum, cj_report[i].zCjTime);
	   }
	}

	delete[] Result;
	delete[] ErrInfo;

}


CString TradeApi::GetZqGddm(CString Zqdm)
{

	char* Result = new char[1024 * 1024];
	char* ErrInfo = new char[256];

	CString gddm;
	int i;
	REPORT gpInfo;
	char t_Zqdm[32];
	sprintf_s(t_Zqdm, "%s", Zqdm);
	//登录成功
	GetQuote(m_ClientID, t_Zqdm, Result, ErrInfo);//查询资金
	
	CString str;
	str.Format(_T("%s"), Result);
	CString output;

	AfxExtractSubString(output, (LPCTSTR)str, 1, '\n');

	AfxExtractSubString(gpInfo.zSiType, (LPCTSTR)output, 30, '\t');
	//TRACE("%s\n", gpInfo.zSiType);
	AfxExtractSubString(gpInfo.zName, (LPCTSTR)output, 1, '\t');
	//TRACE("%s\n", gpInfo.zName);

	for (i = 0; i < ITEM_NUM; i++)
	{
		if (m_gddm_list[i].zSiType == gpInfo.zSiType)
		{
			gddm = m_gddm_list[i].zGddm;
			//TRACE("%s %s\n", gpInfo.zName, gddm);
			break;
		}
	}

	delete[] Result;
	delete[] ErrInfo;

	return gddm;
}

REPORT TradeApi::GetZqInfo(CString Zqdm)
{

	char* Result = new char[1024 * 1024];
	char* ErrInfo = new char[256];
	CString t_errInfo;
	int i;
	REPORT gpInfo;
	char t_Zqdm[32];
	sprintf_s(t_Zqdm, "%s", Zqdm);
	//登录成功
	GetQuote(m_ClientID, t_Zqdm, Result, ErrInfo);//查询资金

	t_errInfo.Format(ErrInfo);
	if (!t_errInfo.IsEmpty())
	{
		TRACE("Get GetZqInfo error");
		return gpInfo;
	}
	//TRACE("\n%s\n", Result);

	CString str;
	str.Format(_T("%s"), Result);
	CString output;

	AfxExtractSubString(output, (LPCTSTR)str, 1, '\n');

	AfxExtractSubString(gpInfo.zCode, (LPCTSTR)output, 0, '\t');
	AfxExtractSubString(gpInfo.zName, (LPCTSTR)output, 1, '\t');
	AfxExtractSubString(gpInfo.zLast, (LPCTSTR)output, 2, '\t');
	AfxExtractSubString(gpInfo.zOpen, (LPCTSTR)output, 3, '\t');
	AfxExtractSubString(gpInfo.zNew, (LPCTSTR)output, 5, '\t');

	AfxExtractSubString(gpInfo.zFisrtBuyPrice, (LPCTSTR)output, 6, '\t');
	AfxExtractSubString(gpInfo.zFisrtSellPrice, (LPCTSTR)output, 16, '\t');

	AfxExtractSubString(gpInfo.zSiType, (LPCTSTR)output, 30, '\t');

	for (i = 0; i < ITEM_NUM; i++)
	{
		if (m_gddm_list[i].zSiType == gpInfo.zSiType)
		{
			gpInfo.zGddm = m_gddm_list[i].zGddm;
			//TRACE("%s %s\n", gpInfo.zName, gpInfo.zGddm);
			break;
		}
	}

	if (gpInfo.zNew > gpInfo.zHighPrice) gpInfo.zHighPrice = gpInfo.zNew;
	if (gpInfo.zNew < gpInfo.zLowPrice) gpInfo.zLowPrice = gpInfo.zNew;

	//TRACE("%s\n", gpInfo.zCode,gpInfo.zName,gpInfo.zLast,gpInfo.zOpen,gpInfo.zNew,gpInfo.zFisrtBuyPrice,gpInfo.zFisrtSellPrice);

	delete[] Result;
	delete[] ErrInfo;

	return gpInfo;
}

REPORT TradeApi::GetZqInfoSina(CString Zqdm)
{
	CString t_url;
	REPORT t_zqinfo;
	CInternetSession c;
	CHttpFile * p = NULL;
	CString str;
	CString info = "";

	int i;
	CString output;
	CString t_HighPrice, t_LowPrice;
	bool is_get_zhishu = false;

	t_zqinfo = GetZqInfo(Zqdm);
	if (Zqdm == _T("999999"))
		t_url.Format("http://hq.sinajs.cn/list=sh%s", _T("000001"));
    else if (atoi(t_zqinfo.zSiType) == 0)
		t_url.Format("http://hq.sinajs.cn/list=sz%s", Zqdm);
	else
		t_url.Format("http://hq.sinajs.cn/list=sh%s", Zqdm);

	p = (CHttpFile*)c.OpenURL((LPCTSTR)t_url);   //打开一个URL
	while (p->ReadString(str))   //读取网页数据 
		info += str;

	if (info == _T("FAILED")) return t_zqinfo;

	if (Zqdm == _T("999999") || Zqdm == _T("399001"))
	{
		is_get_zhishu = TRUE;
	}

	//TRACE("\n%s\n", info);

	AfxExtractSubString(output, (LPCTSTR)info, 1, '"');

	AfxExtractSubString(t_HighPrice, (LPCTSTR)output, 4, ',');
	AfxExtractSubString(t_LowPrice, (LPCTSTR)output, 5, ',');
	if (atof(t_HighPrice) > atof(t_zqinfo.zHighPrice)) t_zqinfo.zHighPrice = t_HighPrice;
	if (atof(t_LowPrice) > atof(t_zqinfo.zLowPrice)) t_zqinfo.zLowPrice = t_LowPrice;

	if (is_get_zhishu) //用于获取指数
	{
		AfxExtractSubString(t_zqinfo.zName, (LPCTSTR)output, 0, ',');
		AfxExtractSubString(t_zqinfo.zOpen, (LPCTSTR)output, 1, ',');
		AfxExtractSubString(t_zqinfo.zLast, (LPCTSTR)output, 2, ',');
		AfxExtractSubString(t_zqinfo.zNew, (LPCTSTR)output, 3, ',');
		AfxExtractSubString(t_zqinfo.zFisrtBuyPrice, (LPCTSTR)output, 6, ',');
		AfxExtractSubString(t_zqinfo.zFisrtSellPrice, (LPCTSTR)output, 7, ',');
	}

	//TRACE("%s\n", gpInfo.zCode,gpInfo.zName,gpInfo.zLast,gpInfo.zOpen,gpInfo.zNew,gpInfo.zFisrtBuyPrice,gpInfo.zFisrtSellPrice);
	p->Close();
	delete  p;
	return t_zqinfo;
}

void TradeApi::GetGddm()
{   
	char* Result = new char[1024 * 1024];
	char* ErrInfo = new char[256];

	//登录成功
	QueryData(m_ClientID, Q_GUDONG_CODE, Result, ErrInfo);//查询资金

	CString str;
	str.Format(_T("%s"), Result);
	CString output;
	//TRACE("%s\n", str);
	m_gddm_num = 0;
	int i;
	for (i = 0; i <ITEM_NUM; i++)
	{

		AfxExtractSubString(output, (LPCTSTR)str, i, '\n');
		if (i != 0) {
			if (output.IsEmpty())
				break;
			m_gddm_str[m_gddm_num] = output;
			m_gddm_num += 1;
			// TRACE("%s\n", output);
		}
	}

	for (i = 0; i < m_gddm_num; i++)
	{
		AfxExtractSubString(m_gddm_list[i].zZjzh, (LPCTSTR)m_gddm_str[i], 0, '\t');
		AfxExtractSubString(m_gddm_list[i].zGddm, (LPCTSTR)m_gddm_str[i], 1, '\t');
		AfxExtractSubString(m_gddm_list[i].zGdname, (LPCTSTR)m_gddm_str[i], 2, '\t');
		AfxExtractSubString(m_gddm_list[i].zSiType, (LPCTSTR)m_gddm_str[i], 3, '\t');
		TRACE("%s %s\n", m_gddm_list[i].zGddm, m_gddm_list[i].zSiType);
	}

	delete[] Result;
	delete[] ErrInfo;
}

CString TradeApi::GetDateTimeInfo()
{
	CTime time = CTime::GetCurrentTime();
	CString t = time.Format("%H:%M:%S");
	CString s;
	s.Format("%d-%02d-%02d ", time.GetYear(), time.GetMonth(), time.GetDay());
	return (s + t);
}

CString TradeApi::GetDateInfo()
{
	CTime time = CTime::GetCurrentTime();
	CString s;
    s.Format("%d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	return s;
}

void TradeApi::BuyStock(CString Zqdm, CString Price, CString Quantity)
{
	char* Result = new char[1024 * 1024];
	char* ErrInfo = new char[256];

	char t_zqdm[32];
	char t_gddm[32];

	float t_price = 0.0;
	int t_quantity = 0;

	t_price = atof(Price);
	t_quantity = atoi(Quantity);

	sprintf_s(t_gddm, "%s", GetZqGddm(Zqdm));
	sprintf_s(t_zqdm, "%s", Zqdm);

	//SendOrder(m_ClientID, S_BUY, 1, t_gddm, t_zqdm, t_price, t_quantity, Result, ErrInfo);//买入股票
	TRACE(_T("%s %s %.2f %d\n"), t_gddm, t_zqdm, t_price, t_quantity);

	#ifdef RUN_LOG
		CString run_log;
		CString logname;
		logname.Format("Buy%d", log_num++);
		run_log.Format("%s %.2f %d [%s]", t_zqdm, t_price, t_quantity, GetDateTimeInfo());
		writeIniLog(logname, run_log);
	#endif

	delete[] Result;
	delete[] ErrInfo;
}

void TradeApi::SellStock(CString Zqdm, CString Price, CString Quantity)
{
	char* Result = new char[1024 * 1024];
	char* ErrInfo = new char[256];

	char t_zqdm[32];
	char t_gddm[32];

	float t_price = 0.0;
	int t_quantity = 0;

	t_price = atof(Price);
	t_quantity = atoi(Quantity);

	sprintf_s(t_gddm, "%s", GetZqGddm(Zqdm));
	sprintf_s(t_zqdm, "%s", Zqdm);

	//SendOrder(m_ClientID, S_SELL, 1, t_gddm, t_zqdm, t_price, t_quantity, Result, ErrInfo);//卖出股票
	TRACE(_T("%s %s %.2f %d\n"), t_gddm, t_zqdm, t_price, t_quantity);

	#ifdef RUN_LOG
		CString run_log;
		CString logname;
		logname.Format("Sell%d", log_num++);
		run_log.Format("%s %.2f %d [%s]", t_zqdm, t_price, t_quantity, GetDateTimeInfo());
		writeIniLog(logname, run_log);
	#endif

	delete[] Result;
	delete[] ErrInfo;
}

void TradeApi::SellByTodayZyRate()
{
	char* Result = new char[1024 * 1024];
	char* ErrInfo = new char[256];
	int i, j;
	bool s_flag = FALSE;
	bool has_zy_flag = FALSE;

	char t_gddm[32];
	char t_zqdm[32];

	float t_buyPrice = 0.0;
	float t_newPrice = 0.0;
	float t_highPrice = 0.0;
	float t_profitRate = 0.0;
	float t_downRate = 0.0;
	float t_lastPrice = 0.0;

	int t_quantity = 0;

	REPORT t_zqinfo, gpinfo;
	CString t_ffBuyPrice;
	float t_firstBuyPrice = 0.0;

	for (i = 0; i < m_stock_num; i++)
	{
		s_flag = FALSE;
		has_zy_flag = FALSE;
		sprintf_s(t_gddm, "%s", m_report[i].zGddm);
		sprintf_s(t_zqdm, "%s", m_report[i].zCode);

		t_quantity = atoi(m_report[i].zValidNum);
		t_buyPrice = atof(m_report[i].zBuyPrice);
		

		t_zqinfo = GetZqInfoSina(m_report[i].zCode);
		t_highPrice = atof(t_zqinfo.zHighPrice);
		t_lastPrice = atof(t_zqinfo.zLast);
		t_newPrice = atof(t_zqinfo.zNew);

		for (j = 0; j < m_zyzs_num; j++)
		{
			if (m_report[i].zCode == zz_report[j].zCode)
			{
				t_profitRate = atof(zz_report[j].zZhiYingRate);
				if (!zz_report[j].zZyHlRate.IsEmpty())
					t_downRate = atof(zz_report[j].zZyHlRate);
				else
					t_downRate = 0.0;
				has_zy_flag = TRUE;
				break;
			}
		}

		//if (!has_zy_flag) 
		//{
		//	t_profitRate = m_defProfitRate;
		//	t_downRate = m_defDownRate;
		//  has_zy_flag = TRUE;
		//}

		if (has_zy_flag)
		{
			if (t_highPrice > t_lastPrice*(1 + t_profitRate / 100.0))
			{
				if (t_newPrice <= t_lastPrice*(1 + (t_profitRate - t_downRate) / 100.0))
				{
					TRACE("Good!!\n");
					s_flag = TRUE;
				}

				if (t_highPrice > t_lastPrice*(1 + (t_profitRate + 2) / 100.0))
				{
					if (t_newPrice <= t_lastPrice*(1 + ((t_profitRate + 2) - t_downRate) / 100.0))
					{
						TRACE("Very Good!!\n");
						s_flag = TRUE;
					}
				}
			}

			if (s_flag && t_quantity != 0 && t_quantity % 100 == 0)
			{
				gpinfo = GetZqInfo(m_report[i].zCode);
				t_ffBuyPrice.Format("%.2f", atof(gpinfo.zFisrtBuyPrice));
				t_firstBuyPrice = atof(t_ffBuyPrice);

				//SendOrder(m_ClientID, S_SELL, 1, t_gddm, t_zqdm, t_firstBuyPrice, t_quantity, Result, ErrInfo);//卖出股票
				TRACE(_T("%s %s %.2f %d | %.2f %.2f\n"), t_gddm, t_zqdm, t_firstBuyPrice, t_quantity, t_highPrice, t_profitRate);

				#ifdef RUN_LOG
					CString run_log;
					CString logname;
					logname.Format("Sell%d", log_num++);
					run_log.Format("%s %.2f %d [%s]", t_zqdm, t_firstBuyPrice, t_quantity, GetDateTimeInfo());
					writeIniLog(logname, run_log);
				#endif
			}

		}
	}

	delete[] Result;
	delete[] ErrInfo;

}

void TradeApi::SellByTotalZyRate()
{
	char* Result = new char[1024 * 1024];
	char* ErrInfo = new char[256];
	int i, j;
	bool s_flag = FALSE;
	bool has_zy_flag = FALSE;

	char t_gddm[32];
	char t_zqdm[32];

	float t_buyPrice = 0.0;
	float t_newPrice = 0.0;
	float t_highPrice = 0.0;
	float t_profitRate = 0.0;
	float t_downRate = 0.0;

	int t_quantity = 0;

	REPORT t_zqinfo;
	CString t_ffBuyPrice;
	float t_firstBuyPrice = 0.0;

	for (i = 0; i < m_stock_num; i++)
	{
		s_flag = FALSE;
		has_zy_flag = FALSE;
		sprintf_s(t_gddm, "%s", m_report[i].zGddm);
		sprintf_s(t_zqdm, "%s", m_report[i].zCode);

		t_quantity = atoi(m_report[i].zValidNum);
		t_buyPrice = atof(m_report[i].zBuyPrice);
		t_newPrice = atof(m_report[i].zNewPrice);
		t_highPrice = atof(m_report[i].zHighPrice);

		for (j = 0; j < m_zyzs_num; j++)
		{
			if (m_report[i].zCode == zz_report[j].zCode)
			{
				t_profitRate = atof(zz_report[j].zZhiYingRate);
				if (!zz_report[j].zZyHlRate.IsEmpty())
					t_downRate = atof(zz_report[j].zZyHlRate);
				else
					t_downRate = 0.0;
				has_zy_flag = TRUE;
				break;
			}
		}

		//if (!has_zy_flag) 
		//{
		//	t_profitRate = m_defProfitRate;
		//	t_downRate = m_defDownRate;
		//  has_zy_flag = TRUE;
		//}

		if (has_zy_flag)
		{
			if (t_highPrice > t_buyPrice*(1 + t_profitRate / 100.0))
			{
				if (t_newPrice <= t_buyPrice*(1 + (t_profitRate - t_downRate) / 100.0))
				{
					TRACE("Good!!\n");
					s_flag = TRUE;
				}

				if (t_highPrice > t_buyPrice*(1 + t_profitRate * 2 / 100.0))
				{
					if (t_newPrice <= t_buyPrice*(1 + (t_profitRate * 2 - t_downRate) / 100.0))
					{
						TRACE("Very Good!!\n");
						s_flag = TRUE;
					}
				}
			}

			if (s_flag && t_quantity != 0 && t_quantity % 100 == 0)
			{
				t_zqinfo = GetZqInfo(m_report[i].zCode);
				t_ffBuyPrice.Format("%.2f", atof(t_zqinfo.zFisrtBuyPrice));
				t_firstBuyPrice = atof(t_ffBuyPrice);

				//SendOrder(m_ClientID, S_SELL, 1, t_gddm, t_zqdm, t_firstBuyPrice, t_quantity, Result, ErrInfo);//卖出股票
				TRACE(_T("%s %s %.2f %d | %.2f %.2f\n"), t_gddm, t_zqdm, t_firstBuyPrice, t_quantity, t_highPrice, t_profitRate);

				#ifdef RUN_LOG
					CString run_log;
					CString logname;
					logname.Format("Sell%d", log_num++);
					run_log.Format("%s %.2f %d [%s]", t_zqdm, t_firstBuyPrice, t_quantity, GetDateTimeInfo());
					writeIniLog(logname, run_log);
				#endif
			}

		}
	}

	delete[] Result;
	delete[] ErrInfo;

}

void TradeApi::SellByZhiSunRate()
{
	char* Result = new char[1024 * 1024];
	char* ErrInfo = new char[256];
	int i, j;
	bool s_flag = FALSE;
	bool has_zs_flag = FALSE;

	char t_gddm[32];
	char t_zqdm[32];

	float t_buyPrice = 0.0;
	float t_newPrice = 0.0;
	CString t_ffBuyPrice;
	float t_firstBuyPrice = 0.0;
	int t_quantity = 0;
	float t_lossRate = 0.0;

	REPORT t_zqinfo;

	for (i = 0; i < m_stock_num; i++)
	{
		s_flag = FALSE;
		has_zs_flag = FALSE;
		sprintf_s(t_gddm, "%s", m_report[i].zGddm);
		sprintf_s(t_zqdm, "%s", m_report[i].zCode);

		t_quantity = atoi(m_report[i].zValidNum);
		t_buyPrice = atof(m_report[i].zBuyPrice);
		t_newPrice = atof(m_report[i].zNewPrice);
		for (j = 0; j < m_zyzs_num; j++)
		{
			if (m_report[i].zCode == zz_report[j].zCode)
			{
				
				

				if (!zz_report[j].zZyHlRate.IsEmpty())
				{
					t_lossRate = atof(zz_report[j].zZhiSunRate);
					has_zs_flag = TRUE;
				}
				else
				{
					t_lossRate = 0.0;
				}				
				break;
			}
		}

		if (!has_zs_flag) t_lossRate = m_defaultLossRate; //如果没有设置止损比例，则强制设置止损为m_defaultLossRate
		
		if (t_newPrice <= t_buyPrice*(1 - t_lossRate/100.0))
		{
			TRACE("Cutted!!\n");
			s_flag = TRUE;
		}

		if (s_flag && t_quantity != 0 && t_quantity % 100 == 0)
		{
			t_zqinfo = GetZqInfo(m_report[i].zCode);
			t_ffBuyPrice.Format("%.2f", atof(t_zqinfo.zFisrtBuyPrice));
			t_firstBuyPrice = atof(t_ffBuyPrice);

			//SendOrder(m_ClientID, S_SELL, 1, t_gddm, t_zqdm, t_firstBuyPrice, t_quantity, Result, ErrInfo);//卖出股票
			TRACE(_T("%s %s %.2f %d | %.2f %.2f\n"), t_gddm, t_zqdm, t_firstBuyPrice, t_quantity, t_newPrice, t_lossRate);

			#ifdef RUN_LOG
				CString run_log;
				CString logname;
				logname.Format("Sell%d", log_num++);
				run_log.Format("%s %.2f %d [%s]", t_zqdm, t_firstBuyPrice, t_quantity, GetDateTimeInfo());
				writeIniLog(logname, run_log);
			#endif
		}
	}

	delete[] Result;
	delete[] ErrInfo;
}

void TradeApi::BuyByDownRate()
{
	char* Result = new char[1024 * 1024];
	char* ErrInfo = new char[256];
	int i;
	bool s_flag = FALSE;
	bool s_has_down = FALSE;
	REPORT t_zqinfo, gqinfo;

	char t_gddm[32];
	char t_zqdm[32];
	int t_quantity = 0;

	float t_lastPrice = 0.0;
	float t_openPrice = 0.0;
	float t_newPrice = 0.0;
	CString t_ffSellPrice;
	float t_firstSellPrice = 0.0;
	float t_lowPrice = 0.0;

	float t_downRate = 0.0;
	float t_backRate = 0.0;


	for (i = 0; i < m_jk_num; i++)
	{
		s_flag = FALSE;
		s_has_down = FALSE;

		if (!jk_report[i].zXiaDieRate.IsEmpty())
		{
			s_has_down = TRUE;
		}

		if (s_has_down)
		{
			t_zqinfo = GetZqInfoSina(jk_report[i].zCode);

			sprintf_s(t_gddm, "%s", t_zqinfo.zGddm);
			sprintf_s(t_zqdm, "%s", jk_report[i].zCode);
			t_quantity = atoi(jk_report[i].zBuyNum);
			t_lastPrice = atof(t_zqinfo.zLast);
			t_openPrice = atof(t_zqinfo.zOpen);
			t_newPrice = atof(t_zqinfo.zNew);
			t_lowPrice = atof(t_zqinfo.zLowPrice);
			t_downRate = atof(jk_report[i].zXiaDieRate);
			if (!jk_report[i].zXdHtRate.IsEmpty()) 
				t_backRate = atof(jk_report[i].zXdHtRate);
			else
				t_backRate = 0.0;


			if (t_lowPrice < t_lastPrice*(1 - t_downRate / 100.0))
			{
				if (t_newPrice >= t_lastPrice*(1 - (t_downRate + t_backRate) / 100.0))
				{
					TRACE("Buy!!\n");
					s_flag = TRUE;
				}

				if (t_lowPrice < t_lastPrice*(1 - t_downRate / 100.0 * 2))
				{
					if (t_newPrice >= t_lastPrice*(1 - (t_downRate * 2 + t_backRate) / 100.0))
					{
						TRACE("Very Buy!!\n");
						s_flag = TRUE;
					}
				}
			}

			if (s_flag && t_quantity != 0 && t_quantity % 100 == 0)
			{
				gqinfo = GetZqInfo(m_report[i].zCode);
				t_ffSellPrice.Format("%.2f", atof(gqinfo.zFisrtSellPrice));
				t_firstSellPrice = atof(t_ffSellPrice);
				//SendOrder(m_ClientID, S_SELL, 1, t_gddm, t_zqdm, t_firstSellPrice, t_quantity, Result, ErrInfo);//卖出股票
				TRACE(_T("%s %s %.2f %d\n"), t_gddm, t_zqdm, t_firstSellPrice, t_quantity);
				#ifdef RUN_LOG
					CString run_log;
					CString logname;
					logname.Format("Buy%d", log_num++);
					run_log.Format("%s %.2f %d [%s]", t_zqdm, t_firstSellPrice, t_quantity, GetDateTimeInfo());
					writeIniLog(logname, run_log);
				#endif
			}
		}
	 }
		delete[] Result;
		delete[] ErrInfo;
}

CString TradeApi::ReturnPath(void)

{
	CString    sPath;
	int    nPos;
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();

	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	return    sPath;
}

void TradeApi::writeIniLog(CString name, CString strValue)
{
	CString exePath = ReturnPath();
	CString sConfigName = _T("Log");
	CString desIniPath;
	desIniPath = exePath + "\\Log.ini";

	WritePrivateProfileString(sConfigName, name, strValue, desIniPath);
}

void TradeApi::writeIni(CString name, CString strValue)
{
	CString exePath = ReturnPath();
	CString sConfigName = _T("Setting");
	CString desIniPath;
	desIniPath = exePath + "\\Zot.ini";

	WritePrivateProfileString(sConfigName, name, strValue, desIniPath);
}

CString TradeApi::readIni(CString name)
{
	CString strValue;
	CString exePath = ReturnPath();
	CString sConfigName = _T("Setting");
	CString desIniPath;
	desIniPath = exePath + "\\Zot.ini";

	GetPrivateProfileString(sConfigName, name, _T(""), strValue.GetBuffer(MAX_PATH), MAX_PATH, desIniPath);
	strValue.ReleaseBuffer();

	return strValue;
}

BOOL TradeApi::isIniExist()
{
	CString exePath;
	CString desIniPath;
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
	BOOL ifFind;

	exePath = ReturnPath();
	desIniPath = exePath + "\\Zot.ini";

	ifFind = finder.FindFile(desIniPath);
	BOOL retValue = FALSE;
	if (ifFind)
	{
		retValue = TRUE;
	}

	return retValue;
}

void TradeApi::initIni()
{
	CString exePath = ReturnPath();
	CString sConfigName = _T("Setting");

	CString sIniFlag = _T("S");

	CString desIniPath;
	desIniPath = exePath + "\\Zot.ini";

	CString sZyZs_default = "002456,0.06,0.01,0.04;600507,0.06,0.01,0.04;";
	CString sJKList_default = "002456,欧菲科技,0.04,0.01;300124,汇川技术,0.04,0.01;";
	//TRACE("%s",desIniPath);

	if (!isIniExist())
	{
		TRACE(_T("Zot.ini is not exist! \n"));
		WritePrivateProfileString(sConfigName, _T("ZyZs"), sZyZs_default, desIniPath);
		WritePrivateProfileString(sConfigName, _T("JKList"), sJKList_default, desIniPath);
	}
	else
	{
		TRACE(_T("Zot.ini is ok! \n"));

		readZyZsIni();
		readJKIni();	
	}

}

void TradeApi::readZyZsIni()
{
	CString output;
	int i, j;
	CString tmp_str[MAX_STOCK_NUM];

	CString t_zcode;
	CString s_ZyZs;
	s_ZyZs = readIni(_T("ZyZs"));
	int t_zyzs_num = 0;
	m_zyzs_num = 0;

	for (i = 0; i <MAX_STOCK_NUM; i++)
	{
		AfxExtractSubString(output, (LPCTSTR)s_ZyZs, i, ';');
		if (output.IsEmpty())
			break;
		tmp_str[t_zyzs_num] = output;
		t_zyzs_num += 1;
		// TRACE("%s\n", output);
	}

	for (i = 0; i < t_zyzs_num; i++)
	{
		AfxExtractSubString(t_zcode, (LPCTSTR)tmp_str[i], 0, ',');
		for (j = 0; j < m_stock_num; j++)
		{
			if (t_zcode == m_report[j].zCode)
			{
				AfxExtractSubString(zz_report[m_zyzs_num].zCode, (LPCTSTR)tmp_str[i], 0, ',');
				AfxExtractSubString(zz_report[m_zyzs_num].zZhiYingRate, (LPCTSTR)tmp_str[i], 1, ',');
				AfxExtractSubString(zz_report[m_zyzs_num].zZyHlRate, (LPCTSTR)tmp_str[i], 2, ',');
				AfxExtractSubString(zz_report[m_zyzs_num].zZhiSunRate, (LPCTSTR)tmp_str[i], 3, ',');
				zz_report[m_zyzs_num].m_index = j;
				//TRACE("%d %s %s %s %s\n", m_zyzs_num, zz_report[m_zyzs_num].zCode, zz_report[m_zyzs_num].zZhiYingRate, zz_report[m_zyzs_num].zZyHlRate, zz_report[m_zyzs_num].zZhiSunRate);

				m_zyzs_num++;
			}
		}
	}
}

void TradeApi::writeZyZsIni()
{
	CString w_str;
	int i;
	for (i = 0; i < m_zyzs_num; i++)
	{
		if (zz_report[i].zCode.IsEmpty()) break;
		w_str.AppendFormat("%s,%s,%s,%s;", zz_report[i].zCode, zz_report[i].zZhiYingRate, zz_report[i].zZyHlRate, zz_report[i].zZhiSunRate);
		//TRACE("%s\n", w_str);
	}
	writeIni(_T("ZyZs"), w_str);
}

void TradeApi::readJKIni()
{
	CString output;
	int i;
	CString tmp_str[MAX_STOCK_NUM];

	CString s_JK;
	s_JK = readIni(_T("JKList"));

	m_jk_num = 0;
	//REPORT t_zqinfo;

	for (i = 0; i <MAX_STOCK_NUM; i++)
	{
		AfxExtractSubString(output, (LPCTSTR)s_JK, i, ';');
		if (output.IsEmpty())
			break;
		tmp_str[m_jk_num] = output;
		m_jk_num += 1;
		// TRACE("%s\n", output);
	}

	for (i = 0; i < m_jk_num; i++)
	{
		AfxExtractSubString(jk_report[i].zCode, (LPCTSTR)tmp_str[i], 0, ',');
		AfxExtractSubString(jk_report[i].zName, (LPCTSTR)tmp_str[i], 1, ',');
		AfxExtractSubString(jk_report[i].zBuyNum, (LPCTSTR)tmp_str[i], 2, ',');
		AfxExtractSubString(jk_report[i].zXiaDieRate, (LPCTSTR)tmp_str[i], 3, ',');
		AfxExtractSubString(jk_report[i].zXdHtRate, (LPCTSTR)tmp_str[i], 4, ',');

		//t_zqinfo = GetZqInfo(jk_report[i].zCode);
		//jk_report[i].zName = t_zqinfo.zName;

		//TRACE("%s %s %s %s %s\n", jk_report[i].zCode, jk_report[i].zName, jk_report[i].zBuyNum, jk_report[i].zXiaDieRate, jk_report[i].zXdHtRate);
	}
}

void TradeApi::writeJKIni()
{
	CString w_str;
	int i;
	for (i = 0; i < m_jk_num; i++)
	{
		if (jk_report[i].zCode.IsEmpty()) break;
		w_str.AppendFormat("%s,%s,%s,%s,%s;", jk_report[i].zCode, jk_report[i].zName, jk_report[i].zBuyNum, jk_report[i].zXiaDieRate, jk_report[i].zXdHtRate);
	}
	writeIni(_T("JKList"), w_str);
}
