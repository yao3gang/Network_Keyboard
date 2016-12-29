/***********************************************
注意：无锁版本只适合单生产者和单消费者
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

#include "ccircular_buffer_lockfree.h"

CcircularBufferLockfree::CcircularBufferLockfree(uint buf_size)
: CcircularBuffer(buf_size)
{
	m_pbuf = NULL;
	//atomic_set(&m_read_p, 0);
	//atomic_set(&m_write_p, 0);
	m_read_p = m_write_p = 0;
	DBG_PRINT("m_buf_size: %d\n", m_buf_size);
}

CcircularBufferLockfree::~CcircularBufferLockfree()
{	
	if (m_pbuf)
	{		
		delete[] m_pbuf;
		m_pbuf = NULL;
	}
	m_read_p = m_write_p = 0;
}
void CcircularBufferLockfree::Reset()
{
	//atomic_set(&m_read_p, 0);
	//atomic_set(&m_write_p, 0);
	m_read_p = m_write_p = 0;
}

int CcircularBufferLockfree::Put(u8 *pbuf, u32 len)
{
	if (NULL == pbuf)
		return -EPARAM;
	
	if (len > m_buf_size-(m_write_p - m_read_p))
	{
		ERR_PRINT("buffer is not enough space, len(%d)-rp(%d)-wp(%d)-size(%d)\n", len, m_read_p, m_write_p, m_buf_size);
		return -ESPACE;
	}
	
	__sync_synchronize();	
	
	/* first put the data starting from fifo->in to buffer end */
	uint l = std::min(len, m_buf_size - (m_write_p & (m_buf_size - 1)));
	memcpy(m_pbuf + (m_write_p & (m_buf_size - 1)), pbuf, l);
	
	/* then put the rest (if any) at the beginning of the buffer */
	memcpy(m_pbuf, pbuf + l, len - l);
	
	__sync_synchronize();
	
	m_write_p += len;

	return SUCCESS;
}

int CcircularBufferLockfree::Get(u8 *pbuf, u32 len)
{
	if (NULL == pbuf)
		return -EPARAM;
	
	if (len > m_write_p - m_read_p)
	{
		//ERR_PRINT("buffer is not enough valid data, len(%d)-rp(%d)-wp(%d)-size(%d)\n", len, m_read_p, m_write_p, m_buf_size);
		return -EDATA;
	}
	
	__sync_synchronize();
	
	/* first get the data from fifo->out until the end of the buffer */
	uint l = std::min(len, m_buf_size - (m_read_p & (m_buf_size - 1)));
	memcpy(pbuf, m_pbuf + (m_read_p & (m_buf_size - 1)), l);
	
	/* then get the rest (if any) from the beginning of the buffer */
	memcpy(pbuf + l, m_pbuf, len - l);
	
	__sync_synchronize();
	
	m_read_p += len;
	
	return SUCCESS;
}


