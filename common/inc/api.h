#ifndef __API_H__
#define __API_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

uint SystemGetMSCount(void);
//需要开发一个驱动，获取jiffies

uint64 SystemGetMSCount64(void);

void SystemSleep(uint ms);

u32 copyFile(int fd_dst, int fd_src);

//设置fd 非阻塞 标记
int fdSetNonblockFlag(s32 fd);

//去除fd 非阻塞 标记
int fdClrNonblockFlag(s32 fd);

#ifdef __cplusplus
}
#endif


#endif
