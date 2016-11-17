#ifndef __LOW_THREAD_H__
#define __LOW_THREAD_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif
/****************************************************************
phThread: 线程句柄
pStartAddress: 线程函数
pParameter: 线程函数的参数
nPriority: 优先级
dwStatckSize: 堆栈大小
****************************************************************/
int ThreadCreate(VD_HANDLE *phThread,
				 void(*pStartAddress)(void *p),
				 void *pParameter,
				 int nPriority,
				 uint dwStatckSize,
				 const char*pName);
				 
				 
int ThreadDestory(VD_HANDLE hThread);

void ThreadExit(void);

int ThreadGetID(void);

#ifdef __cplusplus
}
#endif

#endif

