#ifndef __BIZ_SYS_COMPLEX_H__
#define __BIZ_SYS_COMPLEX_H__
#include <time.h>
#include "types.h"

typedef struct
{
	u16 nYear;
	u8 nMonth;
	u8 nDay;
	u8 nHour;
	u8 nMinute;
	u8 nSecode;
	u8 nWday;
} SDateTime;

//function  declaration
#ifdef __cplusplus
extern "C" {
#endif

int BizSystemComplexInit();
int BizGetTime(SDateTime *pdt);
int BizSetTime(SDateTime *pdt);
int BizNtpSyncTime(const char *ntpser_ip);



#ifdef __cplusplus
}
#endif

#endif

