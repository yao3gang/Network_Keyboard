#include <new>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#endif

#include "types.h"
#include "cmutex.h"

/*
CMutex::CMutex(int nType): type(nType)
{
	
}
*/

CMutex::~CMutex()
{
	//DBG_PRINT("\n");
	if (m_plock)
	{
		pthread_mutex_destroy((pthread_mutex_t *)m_plock);
		delete (pthread_mutex_t *)m_plock;
		m_plock = NULL;
	}
	else
	{
		ERR_PRINT("m_plock == NULL\n");
	}	
}

int CMutex::Create()
{
	//DBG_PRINT("1 \n");
	pthread_mutex_t *lock = NULL;
	int ret = 0;
	
	//已经创建过，返回
	if (m_plock)
		return SUCCESS;
	
	//DBG_PRINT("2 \n");	
	lock = new pthread_mutex_t;
	if (NULL == lock)
	{
		ERR_PRINT("new CMutex except\n");
		goto fail;
	}
	
	if (MUTEX_FAST == type)
	{
		ret = pthread_mutex_init(lock, NULL);
		if (ret)
		{
			ERR_PRINT("pthread_mutex_init\n");
			goto fail;
		}
	}
	else //MUTEX_RECURSIVE
	{
		pthread_mutexattr_t attr;
		ret = pthread_mutexattr_init(&attr);
		if (ret)
		{
			ERR_PRINT("pthread_mutexattr_init\n");
			goto fail;
		}
    	
    	ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
    	if (ret)
		{
			ERR_PRINT("pthread_mutexattr_settype\n");
			goto fail;
		}
		
		ret = pthread_mutex_init(lock, &attr);
		if (ret)
		{
			ERR_PRINT("pthread_mutex_init\n");
			goto fail;
		}
	}
	//DBG_PRINT("3 \n");
	m_plock = (VD_HANDLE)lock;
	return SUCCESS;
	
	
fail:
	if (lock)
	{
		delete lock;
		lock = NULL;
	}
	
	m_plock = NULL;
	return -FAILURE;
}

int CMutex::Lock()
{
	if (NULL == m_plock)
	{
		ERR_PRINT("m_plock == NULL\n");
		return -FAILURE;
	}
	//DBG_PRINT("\n");
	return pthread_mutex_lock((pthread_mutex_t *)m_plock);
}

int CMutex::Unlock()
{
	if (NULL == m_plock)
	{
		ERR_PRINT("m_plock == NULL\n");
		return -FAILURE;
	}
	//DBG_PRINT("\n");
	return pthread_mutex_unlock((pthread_mutex_t *)m_plock);
}

#if 0
VD_BOOL CMutex::Enter()
{
	return Lock();
}

VD_BOOL CMutex::Leave()
{
	return Unlock();
}
#endif

