#ifndef _ZFUNC_H_
#define _ZFUNC_H_
#pragma pack(push,1) 

//函数(数据个数,输出,输入a,输入b,输入c)
typedef void(*pPluginFUNC)(int, float*, float*, float*, float*);

typedef struct tagPluginTCalcFuncInfo
{
	unsigned short		nFuncMark;//函数编号
	pPluginFUNC			pCallFunc;//函数地址
}PluginTCalcFuncInfo;

typedef BOOL(*pRegisterPluginFUNC)(PluginTCalcFuncInfo**);

#pragma pack(pop)
#endif