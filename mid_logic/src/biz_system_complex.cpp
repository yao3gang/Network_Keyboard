#include "api.h"
#include "types.h"
#include "singleton.h"
#include "object.h"
#include "glb_error_num.h"
#include "biz.h"
#include "biz_types.h"
#include "biz_config.h"
#include "biz_net.h"
#include "biz_device.h"
#include "biz_system_complex.h"
#include "bond.h"
#include "atomic.h"
#include "c_lock.h"
#include "crwlock.h"
#include "ccircular_buffer.h"
#include "public.h"
#include "flash.h"
#include "cthread.h"
#include "ctimer.h"
#include "ctrlprotocol.h"
#include "net.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>


#include <map>
#include <set>
//#include <bitset>
#include <algorithm>
#include <utility>

#include "hi_rtc.h"

#define RTC_DEVNAME "/dev/hi_rtc"

static int b_inited = 0;
static int fd_rtc = INVALID_FD;
static pthread_mutex_t mutex_rtc = PTHREAD_MUTEX_INITIALIZER;
static pthread_t ptid = -1;

//读取后要加时区偏移
static int read_rtc(time_t *pt)//utc time
{
	struct tm tm_now;
	rtc_time_t hitm;
	
	memset(&hitm, 0, sizeof(hitm));
	memset(&tm_now, 0, sizeof(tm_now));

	if (fd_rtc < 0)
	{
		DBG_PRINT("fd_rtc == INVALID_FD\n");
		return -1;
	}

	pthread_mutex_lock(&mutex_rtc);
	
	int ret = ioctl(fd_rtc, HI_RTC_RD_TIME, &hitm);
	if(ret < 0)
	{
		DBG_PRINT("ioctl: HI_RTC_RD_TIME failed\n");
		pthread_mutex_unlock(&mutex_rtc);
		return -1;
	}
	/*
	printf("HI_RTC_RD_TIME:\n");
	printf("year:%d\n", hitm.year);
	printf("month:%d\n",hitm.month);
	printf("date:%d\n", hitm.date);
	printf("hour:%d\n", hitm.hour);
	printf("minute:%d\n", hitm.minute);
	printf("second:%d\n", hitm.second);
	printf("weekday:%d\n", hitm.weekday);
	*/
	if (hitm.year <= 2000)//20130101 00:00:00 星期二
	{
		printf("%s RTC exception --- year <= 2000, set to 20130101 08:00:00\n", __func__);
		tm_now.tm_year = 2013 - 1900;
		tm_now.tm_mon = 1 - 1;
		tm_now.tm_mday = 1;
		tm_now.tm_hour = 0;
		tm_now.tm_min = 0;
		tm_now.tm_sec = 0;
		tm_now.tm_wday = 2;
	}
	else
	{
		tm_now.tm_year = hitm.year - 1900;
		tm_now.tm_mon = hitm.month - 1;
		tm_now.tm_mday = hitm.date;
		tm_now.tm_hour = hitm.hour;
		tm_now.tm_min = hitm.minute;
		tm_now.tm_sec = hitm.second;
		tm_now.tm_wday = hitm.weekday;
	}
	
	tm_now.tm_isdst = -1; // not known
	*pt = mktime(&tm_now);

	pthread_mutex_unlock(&mutex_rtc);
	return 0;
}

static int write_rtc(time_t t)
{
	rtc_time_t hitm;
	struct tm tm;
	
	gmtime_r(&t, &tm);
	tm.tm_isdst = 0;	
	
	hitm.year   = tm.tm_year + 1900;
	hitm.month  = tm.tm_mon + 1;
	hitm.date   = tm.tm_mday;
	hitm.hour   = tm.tm_hour;
	hitm.minute = tm.tm_min;
	hitm.second = tm.tm_sec;
	hitm.weekday = 0;

	pthread_mutex_lock(&mutex_rtc);
	
	int ret = ioctl(fd_rtc, HI_RTC_SET_TIME, &hitm);
	if(ret < 0)
	{
		DBG_PRINT("ioctl: HI_RTC_SET_TIME failed\n");
		pthread_mutex_unlock(&mutex_rtc);
		return -1;
	}
	//printf("HI_RTC_SET_TIME:\n");
	//printf("year:%d\n", hitm.year);
	//printf("month:%d\n",hitm.month);
	//printf("date:%d\n", hitm.date);
	//printf("hour:%d\n", hitm.hour);
	//printf("minute:%d\n", hitm.minute);
	//printf("second:%d\n", hitm.second);
	pthread_mutex_unlock(&mutex_rtc);
	return 0;
}

int BizGetTime(SDateTime *pdt)
{
	struct tm tm_time;
	time_t currtime;
		
	if (!b_inited)
	{
		DBG_PRINT("mod not inited\n");
		return -1;
	}

	currtime = time(NULL);
	currtime += 8*3600;//时区偏移	
	
	gmtime_r(&currtime, &tm_time);
	
	pdt->nYear	= tm_time.tm_year + 1900;
	pdt->nMonth	= tm_time.tm_mon + 1;
	pdt->nDay	= tm_time.tm_mday;
	pdt->nHour	= tm_time.tm_hour;
	pdt->nMinute = tm_time.tm_min;
	pdt->nSecode = tm_time.tm_sec;
	pdt->nWday	= tm_time.tm_wday;

	return 0;
}

int BizSetTime(SDateTime *pdt)
{
	struct tm tm_time;
	time_t t;
	
	if (!b_inited)
	{
		DBG_PRINT("mod not inited\n");
		return -1;
	}

	tm_time.tm_year = pdt->nYear - 1900;
	tm_time.tm_mon 	= pdt->nMonth - 1;
	tm_time.tm_mday = pdt->nDay;
	
	tm_time.tm_hour = pdt->nHour;
	tm_time.tm_min 	= pdt->nMinute;
	tm_time.tm_sec 	= pdt->nSecode;
	
	printf("y:%d, m:%d, d:%d, h:%d, m:%d, s:%d\n", 
		pdt->nYear, pdt->nMonth, pdt->nDay, pdt->nHour, pdt->nMinute, pdt->nSecode);
	
	t = mktime(&tm_time);
	t -= 8*3600;//时区偏移
	
	stime(&t);//set system time
	if (write_rtc(t))//set rtc
	{
		DBG_PRINT("write_rtc failed\n");
		return -1;
	}

	return 0;
}

int BizNtpSyncTime(const char *ntpser_ip)
{
	struct timeval tv;
	time_t local_t;
	if(GetTimeFromNtpserver(ntpser_ip, 0, 5, &tv))
	{
		return -1;
	}

	local_t = time(NULL);

	if (tv.tv_sec != local_t)
	{
		local_t = tv.tv_sec;
		stime(&local_t);//set system time
		if (write_rtc(local_t))//set rtc
		{
			DBG_PRINT("write_rtc failed\n");
			return -1;
		}
	}
	
	return 0;
}

void* threadUpdateTime(void* pParam)
{
	int ret = 0;
	SDateTime dt;
	time_t pre_t, cur_t;
	u32 count = 0;
	SConfigTimeParam stime_param;
	
	pre_t = cur_t = time(NULL);
	
	ret = pthread_detach(pthread_self());
	if(ret < 0)
	{
		ERR_PRINT("pthread_detach");
		pthread_exit((void *)0);
	}
	else
	{
		DBG_PRINT("threadUpdateTime detach ok\n");
	}

	while(1)
	{
		cur_t = time(NULL);

		if (pre_t != cur_t)
		{
			pre_t = cur_t;
			
			if (BizGetTime(&dt) == 0)
			{
				notifyGuiUpdateTime(&dt);
			}

			if (count++ > 60*60*12)//每隔12小时自动同步时间服务器
			{
				count == 0;
				
		        if (BizConfigGetTimeParam(stime_param))
		        {
		            DBG_PRINT("BizConfigGetTimeParam failed\n");
		        }
		        else
		        {
					if (stime_param.btime_sync)
					{
						if (BizNtpSyncTime(stime_param.ntp_svr_ip.c_str()))
						{
							DBG_PRINT("BizNtpSyncTime failed\n");
						}
					}
		        }				
			}
		}

		usleep(100*1000);//delay 100ms
	}

	pthread_exit((void *)0);
}

int BizSystemComplexInit()
{
	int ret = SUCCESS;
	time_t cur;
	
	fd_rtc = open(RTC_DEVNAME, O_RDWR);
	if(fd_rtc < 0)
	{
		DBG_PRINT("open %s failed\n", RTC_DEVNAME);
		return -1;
	}
	
	ret = read_rtc(&cur);
	if (ret)
	{
		DBG_PRINT("read_rtc failed\n");
		return -1;
	}
	
	stime(&cur);
	DBG_PRINT("utc : system current date&time: %s\n", ctime(&cur));	

	ret = pthread_create(&ptid, NULL, threadUpdateTime, NULL);
	if (ret)
	{
		DBG_PRINT("pthread_create failed\n");
		return -1;
	}
	

	b_inited = 1;
	
	return 0;
}



