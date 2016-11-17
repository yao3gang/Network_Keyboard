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
#include "cspin_lock.h"

CSpinLock::~CSpinLock()
{
	//DBG_PRINT("\n");
	if (m_plock)
	{
		pthread_spin_destroy((pthread_spinlock_t *)m_plock);
		delete (pthread_spinlock_t *)m_plock;
		m_plock = NULL;
	}
	else
	{
		ERR_PRINT("m_plock == NULL\n");
	}	
}

int CSpinLock::Create()
{
	pthread_spinlock_t *lock = NULL;
	int ret = 0;
	
	//已经创建过，返回
	if (m_plock)
		return SUCCESS;
	
	lock = new pthread_spinlock_t;
	if (NULL == lock)
	{
		ERR_PRINT("new CSpinLock except\n");
		return -FAILURE;
	}
	
	ret = pthread_spin_init(lock, PTHREAD_PROCESS_PRIVATE);//进程内所有线程共享
	if (ret)
	{
		ERR_PRINT("pthread_spin_init\n");
		delete lock;
		lock = NULL;
		
		return -FAILURE;
	}
	
	m_plock = (VD_HANDLE)lock;
	return SUCCESS;
}

int CSpinLock::Lock()
{
	if (NULL == m_plock)
	{
		ERR_PRINT("m_plock == NULL\n");
		return -FAILURE;
	}

	//DBG_PRINT("\n");
	
	return pthread_spin_lock((pthread_spinlock_t *)m_plock);
}

int CSpinLock::Unlock()
{
	if (NULL == m_plock)
	{
		ERR_PRINT("m_plock == NULL\n");
		return -FAILURE;
	}

	//DBG_PRINT("\n");
	
	return pthread_spin_unlock((pthread_spinlock_t *)m_plock);
}

