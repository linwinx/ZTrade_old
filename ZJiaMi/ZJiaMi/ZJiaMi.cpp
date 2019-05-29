// ZJiaMi.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


#define ACCOUNT_OFFSET 0x1148a9
#define ACCOUNT_NUM 0xF


void changAccount()
{
	char accuout[ACCOUNT_NUM];
	char new_accuout[ACCOUNT_NUM];
	char *dll_file = "tdx.dll";
	char *dll_new_file = "tdx_new.dll";

	FILE *fp = fopen(dll_file, "rb"); //按照filename的路径打开文件
	if (fp == 0){
		printf("file open fail\n");
		return;
	}

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	printf("size = 0x%x \n", size);
	//fclose(fp);

	fseek(fp, 0, 0);
	char *pre_buffer = (char *)malloc(ACCOUNT_OFFSET);
	fread(pre_buffer, ACCOUNT_OFFSET, 1, fp);

	fseek(fp, ACCOUNT_OFFSET, 0);
	fread(accuout, ACCOUNT_NUM, 1, fp); //读取头文件信息

	fseek(fp, ACCOUNT_OFFSET + ACCOUNT_NUM, 0);
	char *end_buffer = (char *)malloc(size - ACCOUNT_OFFSET - ACCOUNT_NUM);
	fread(end_buffer, size - ACCOUNT_OFFSET - ACCOUNT_NUM, 1, fp);


	FILE *fp_new = fopen(dll_new_file, "wb");
	if (fp_new == 0){
		printf("file open fail\n");
	}

	strcpy(new_accuout, "ABCDEFGHIJKL");

	fseek(fp_new, 0, 0);
	fwrite(pre_buffer, ACCOUNT_OFFSET, 1, fp_new);

	fseek(fp_new, ACCOUNT_OFFSET, 0);
	fwrite(new_accuout, ACCOUNT_NUM, 1, fp_new);

	fseek(fp_new, ACCOUNT_OFFSET + ACCOUNT_NUM, 0);
	fwrite(end_buffer, size - ACCOUNT_OFFSET - ACCOUNT_NUM, 1, fp_new);

	fclose(fp_new);
	fclose(fp);
	free(pre_buffer);
	free(end_buffer);
	printf("\n change account end \n");

}

char* getOddAccount(char* odd_account, char* org_account)
{
	char* odd = odd_account;
	while ((*org_account++) != '\0')
	{
		if (i % 2 == 0)
		{
			odd[j] = org_account[i];
			j++;
		}
			
	}
	odd_account[j] = '\0';


	printf("%s\n", odd_account);
	return odd_account;
}


int _tmain(int argc, _TCHAR* argv[])
{
#if 1
	unsigned short a3 = 0x55E;
	char* gpdm = "3245";
	char result[ACCOUNT_NUM];
	int st = 0;
	for (int i = 0; i < strlen(gpdm); i++)
	{
		bool next = true;
		int a = gpdm[i];
		int b = a3 >> 0x8;
		int c = a ^ b;
		a3 = (unsigned short)(0x207F * (a3 + c) - 0x523D);
		char t1;
		char t2;
		for (int j = 65; j < 91 && next; j++)
		{
			for (int k = 90; k >= 65 && next; k--)
			{
				int temp = (1755 + c - k);
				if (temp % 26 == 0 && temp / 26 == j)
				{
					result[st++] = j;
					result[st++] = k;
					next = false;
				}
			}
		}
	}
#endif
	result[st] = '\0';
	printf("%s\n", result);

//	changAccount();

	//getAccount();

	char *org_account = "309619426390";
	printf("%s\n", getAccount(org_account));

	while (1) {};
	return 0;
}

