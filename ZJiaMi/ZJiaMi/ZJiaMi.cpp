// ZJiaMi.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned short a3 = 0x55E;
	char* gpdm = "3245";
	char sss[30];
	char* result = sss;
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
					char p, q;
					_itoa(j, &p, 10);
					_itoa(k, &q, 10);
					//p = (char)j;
					//q = (char)k;
					//result += p + "" + q;
					//strcat(result, &p);
					//strcat(result, &q);
					printf("%s %s ", &p, &q);
					next = false;
				}
			}
		}
	}

	//printf("%s\n", result);

	while (1) {};
	return 0;
}

