#include <semaphore.h>
#include <time.h>
#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int SemaphoreCreate(HANDLE *semId, DWORD cnt)
{
	sem_t *	sem;
	
	sem = (sem_t *)malloc(sizeof(sem_t));
	if (sem == NULL)
	{
		ERR_PRINT("SemaphoreCreate Malloc Fail\n");
		return -1;
	}
	if (sem_init(sem, 0, cnt) == 0)
	{
		*semId = (HANDLE)sem;
		return 0;
	}
	
	ERR_PRINT("Sem_init Fail\n");
	return -1;
} 


int SemaphoreDestory(HANDLE semId)
{
	if (sem_destroy((sem_t *)semId) == -1)
	{
		perror("sem destroy");
		return -errno;
	}
	else
	{
		free((void*)semId);
		return 0;
	}
} 


int SemaphorePost(HANDLE semId)
{
  return sem_post((sem_t *)semId);
} 


int SemaphorePend(HANDLE semId)
{
  return sem_wait((sem_t *)semId);
}

int SemaphoreTryPend(HANDLE semId)
{
  return sem_trywait((sem_t *)semId);
}

int SemaphoreTimedPend(HANDLE semId, u32 seconds)
{
	struct timespec tm;
	time_t cur_t;
	
	memset(&tm, 0, sizeof(tm));
	time(&cur_t);
	tm.tv_sec = cur_t+seconds;//Î´À´Ê±¼ä

	return sem_timedwait((sem_t *)semId, &tm);
}