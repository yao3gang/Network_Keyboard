#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "types.h"
#include <fcntl.h>
#include <time.h>



uint SystemGetMSCount(void)
{
	unsigned int t = 0;
	
#ifdef WIN32
//	t = GetTickCount();
#else
	#if 0// yzw modify 20111123
	t = (time(NULL) & 0xffffffff) * 10;
	#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	t = (uint)((((uint64)tv.tv_sec * 1000) + tv.tv_usec / 1000) & 0xffffffff);
	#endif
#endif
	
	return t;
}

uint64 SystemGetMSCount64(void)
{

	#if 0 // yzw modify 20111123
	
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (((uint64)tv.tv_sec * 1000) + tv.tv_usec / 1000) & 0xffffffff;
	
	#else
	uint64 val;
	struct timespec time;
	
	clock_gettime(CLOCK_MONOTONIC, &time);

	val = time.tv_sec;
	val = val*1000 + time.tv_nsec/(1000*1000);

	return val;
	#endif
} 

void SystemSleep(uint ms)
{
#ifdef WIN32
	//Sleep(ms);
#else
	usleep(ms * 1000);
#endif
	
}

u32 copyFile(int fd_dst, int fd_src)
{
	if ((fd_dst < 0) || (fd_src < 0))
	{
		return -EPARAM;
	}

	char buf[1024];
	int n = 0;

	while ((n=read(fd_src, buf, 1024)) > 0)
	{
		if (write(fd_dst, buf, n) != n)
		{
			ERR_PRINT("write file failed\n");
			return -FAILURE;
		}
	}

	if (n < 0)
	{
		ERR_PRINT("read file failed\n");
		return FAILURE;
	}

	return SUCCESS;
}

//设置fd 非阻塞 标记
int fdSetNonblockFlag(s32 fd)
{
	int fd_flag;

	fd_flag = fcntl(fd, F_GETFL, 0);
	if (fd_flag < 0)
	{
		DBG_PRINT("F_GETFL failed, %s\n", strerror(errno));
		return -FAILURE;
	}
	
	if (fcntl(fd, F_SETFL, fd_flag|O_NONBLOCK) < 0)
	{
		DBG_PRINT("F_SETFL failed, %s\n", strerror(errno));
		return -FAILURE;
	}
	
	return SUCCESS;
}

//去除fd 非阻塞 标记
int fdClrNonblockFlag(s32 fd)
{
	int fd_flag;
	
	fd_flag = fcntl(fd, F_GETFL, 0);
	if (fd_flag < 0)
	{
		ERR_PRINT("F_GETFL failed, %s\n", strerror(errno));
		return -FAILURE; 
	}
	if (fcntl(fd, F_SETFL, fd_flag&(~O_NONBLOCK)) < 0)
	{
		ERR_PRINT("F_SETFL failed, %s\n", strerror(errno));
		return -FAILURE;
	}

	return SUCCESS;
}



