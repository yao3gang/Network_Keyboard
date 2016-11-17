#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#endif

#include <assert.h>

#include "types.h"
#include "crwlock.h"

int CRWlock::Create()
{
	pthread_rwlock_t *rwptr = NULL;
	int ret = 0;
	
	//已经创建过，返回
	if (m_plock)
		return SUCCESS;
	
	rwptr = new pthread_rwlock_t;
	if (NULL == rwptr)
	{
		ERR_PRINT("new CRWlock except\n");
		return -FAILURE;
	}

	ret = pthread_rwlock_init(rwptr, NULL);
	if (ret)
	{
		ERR_PRINT("pthread_rwlock_init\n");
		delete rwptr;
		rwptr = NULL;
		
		return -FAILURE;
	}
	
	m_plock = (VD_HANDLE)rwptr;
	
	return SUCCESS;
}

CRWlock::~CRWlock()
{
	if (m_plock)
	{	
		pthread_rwlock_destroy((pthread_rwlock_t *)m_plock);
		delete (pthread_rwlock_t *)m_plock;
		m_plock = NULL;
	}
	else
	{
		ERR_PRINT("m_plock == NULL\n");
	}
}

int CRWlock::Rdlock()
{
	if (NULL == m_plock)
	{
		ERR_PRINT("m_plock == NULL\n");
		return -FAILURE;
	}
	
	return pthread_rwlock_rdlock((pthread_rwlock_t *)m_plock);
}

int CRWlock::Wrlock()
{
	if (NULL == m_plock)
	{
		ERR_PRINT("m_plock == NULL\n");
		return -FAILURE;
	}
	
	return pthread_rwlock_wrlock((pthread_rwlock_t *)m_plock);
}

int CRWlock::Unlock()
{
	if (NULL == m_plock)
	{
		ERR_PRINT("m_plock == NULL\n");
		return -FAILURE;
	}
	
	return pthread_rwlock_unlock((pthread_rwlock_t *)m_plock);
}

