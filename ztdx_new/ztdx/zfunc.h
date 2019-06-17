#ifndef _ZFUNC_H_
#define _ZFUNC_H_
#pragma pack(push,1) 

//����(���ݸ���,���,����a,����b,����c)
typedef void(*pPluginFUNC)(int, float*, float*, float*, float*);

typedef struct tagPluginTCalcFuncInfo
{
	unsigned short		nFuncMark;//�������
	pPluginFUNC			pCallFunc;//������ַ
}PluginTCalcFuncInfo;

typedef BOOL(*pRegisterPluginFUNC)(PluginTCalcFuncInfo**);

#pragma pack(pop)
#endif