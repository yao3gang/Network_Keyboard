/***********************************************
注意：有锁版本适合多生产者或多消费者
************************************************/

#include <new>
#include <algorithm>

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
#include "cguard.h"
#include "cspin_lock.h"
#include "ccircular_buffer_lock.h"

CcircularBufferLock::CcircularBufferLock(uint buf_size)
: CcircularBuffer(buf_size)
{
	m_pbuf = NULL;
	m_plock = NULL;
	m_read_p = m_write_p = 0;
	//DBG_PRINT("m_buf_size: %d\n", m_buf_size);
}

CcircularBufferLock::~CcircularBufferLock()
{	
	if (m_plock)
	{
		m_plock->Lock();
	}
	
	if (m_pbuf)
	{		
		delete[] m_pbuf;
		m_pbuf = NULL;
	}
	
	m_read_p = m_write_p = 0;

	if (m_plock)
	{
		m_plock->Unlock();
		delete m_plock;
		m_plock = NULL;
	}
}

int CcircularBufferLock::CreateBuffer()
{
	if (NULL == m_plock)
	{
		//DBG_PRINT("CreateBuffer 1\n");
		
		m_plock = new CSpinLock;
		if (NULL == m_plock)
		{
			return -FAILURE;
		}

		//DBG_PRINT("CreateBuffer 2\n");
		
		if (0 != m_plock->Create())
		{
			delete m_plock;
			m_plock = NULL;
			
			return -FAILURE;
		}

		//DBG_PRINT("CreateBuffer 3\n");
	}

	m_plock->Lock();
	
	if (NULL == m_pbuf)
	{
		m_pbuf = new uchar[m_buf_size];
		if (NULL == m_pbuf)
		{
			ERR_PRINT("m_pbuf new except\n");
			m_plock->Unlock();
			delete m_plock;
			m_plock = NULL;
			
			return -FAILURE;
		}
	}

	m_plock->Unlock();
	return SUCCESS;
}
void CcircularBufferLock::Reset()
{
	//atomic_set(&m_read_p, 0);
	//atomic_set(&m_write_p, 0);
	CGuard guard(*m_plock);
	
	m_read_p = m_write_p = 0;
}

int CcircularBufferLock::Put(uchar *pbuf, uint len)
{
	if (NULL == pbuf)
		return -EPARAM;
	
	CGuard guard(*m_plock);
	
	if (len > m_buf_size-(m_write_p - m_read_p))
	{
		ERR_PRINT("buffer is not enough space, len(%d)-rp(%d)-wp(%d)-size(%d)\n", len, m_read_p, m_write_p, m_buf_size);
		return -ESPACE;
	}
	
	//__sync_synchronize();	已加锁
	
	/* first put the data starting from fifo->in to buffer end */
	uint l = std::min(len, m_buf_size - (m_write_p & (m_buf_size - 1)));
	memcpy(m_pbuf + (m_write_p & (m_buf_size - 1)), pbuf, l);
	
	/* then put the rest (if any) at the beginning of the buffer */
	memcpy(m_pbuf, pbuf + l, len - l);
	
	//__sync_synchronize();
	
	m_write_p += len;

	return SUCCESS;
}

int CcircularBufferLock::Get(uchar *pbuf, uint len)
{
	if (NULL == pbuf)
		return -EPARAM;
	
	CGuard guard(*m_plock);
	
	if (len > m_write_p - m_read_p)
	{
		ERR_PRINT("buffer is not enough valid data, len(%d)-rp(%d)-wp(%d)-size(%d)\n", len, m_read_p, m_write_p, m_buf_size);
		return -EDATA;
	}
	
	//__sync_synchronize(); 已加锁
	
	/* first get the data from fifo->out until the end of the buffer */
	uint l = std::min(len, m_buf_size - (m_read_p & (m_buf_size - 1)));
	memcpy(pbuf, m_pbuf + (m_read_p & (m_buf_size - 1)), l);
	
	/* then get the rest (if any) from the beginning of the buffer */
	memcpy(pbuf + l, m_pbuf, len - l);
	
	//__sync_synchronize();
	
	m_read_p += len;
	
	return SUCCESS;
}


