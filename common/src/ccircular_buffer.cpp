/*****************************************************
基础版本，需要外部锁或条件变量使用
*****************************************************/

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
#include "ccircular_buffer.h"

CcircularBuffer::CcircularBuffer(uint buf_size)
: m_pbuf(NULL)
, m_read_p(0)
, m_write_p(0)
{
	//至少分配1024
	if (buf_size < (1<<10))
		buf_size = 1<<10;
		
	//缓冲区空间的大小，圆整为2的次幂	
	//m_buf_size = roundup_power_of_two(buf_size);
	
	if((buf_size & (buf_size-1)) == 0)//是否为2的次幂
	{
		m_buf_size = buf_size;
	}
	else
	{
		uint maxuint = (uint)((uint)~0);
		uint andv = ~(maxuint&(maxuint>>1));
		
		while((andv & buf_size) == 0)
			andv = andv>>1;
		
		m_buf_size = andv<<1;
	}
	DBG_PRINT("m_buf_size: %d\n", m_buf_size);
}

CcircularBuffer::~CcircularBuffer()
{	
	if (m_pbuf)
	{		
		delete[] m_pbuf;
		m_pbuf = NULL;
	}
	m_read_p = m_write_p = 0;
}

int CcircularBuffer::CreateBuffer()
{
	if (NULL == m_pbuf)
	{
		m_pbuf = new uchar[m_buf_size];
	}
	
	if (NULL == m_pbuf)
	{
		ERR_PRINT("m_pbuf new except\n");
		return -FAILURE;
	}
	
	return SUCCESS;
}
void CcircularBuffer::Reset()
{
	m_pre_read_p = m_read_p = m_pre_write_p = m_write_p = 0;
}

void CcircularBuffer::PutRst()
{
	m_write_p = m_pre_write_p;
}

int CcircularBuffer::Put(u8 *pbuf, u32 len)
{
	if (NULL == pbuf)
		return -EPARAM;
	
	if (len > m_buf_size-(m_write_p - m_read_p))
	{
		ERR_PRINT("buffer is not enough space, len(%d)-rp(%d)-wp(%d)-size(%d)\n", len, m_read_p, m_write_p, m_buf_size);
		return -ESPACE;
	}
	
	//__sync_synchronize();	
	
	/* first put the data starting from fifo->in to buffer end */
	uint l = std::min(len, m_buf_size - (m_write_p & (m_buf_size - 1)));
	memcpy(m_pbuf + (m_write_p & (m_buf_size - 1)), pbuf, l);
	
	/* then put the rest (if any) at the beginning of the buffer */
	memcpy(m_pbuf, pbuf + l, len - l);
	
	//__sync_synchronize();
	m_pre_write_p = m_write_p;
	m_write_p += len;

	return SUCCESS;
}

void CcircularBuffer::GetRst()
{
	m_read_p = m_pre_read_p;
}

int CcircularBuffer::Get(u8 *pbuf, u32 len)
{
	if (NULL == pbuf)
		return -EPARAM;
	
	if (len > m_write_p - m_read_p)
	{
		//ERR_PRINT("buffer is not enough valid data, len(%d)-rp(%d)-wp(%d)-size(%d)\n", len, m_read_p, m_write_p, m_buf_size);
		return -EDATA;
	}
	
	//__sync_synchronize();
	
	/* first get the data from fifo->out until the end of the buffer */
	uint l = std::min(len, m_buf_size - (m_read_p & (m_buf_size - 1)));
	memcpy(pbuf, m_pbuf + (m_read_p & (m_buf_size - 1)), l);
	
	/* then get the rest (if any) from the beginning of the buffer */
	memcpy(pbuf + l, m_pbuf, len - l);
	
	//__sync_synchronize();
	m_pre_read_p = m_read_p;
	m_read_p += len;
	
	return SUCCESS;
}
