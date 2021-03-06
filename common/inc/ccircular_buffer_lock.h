/***********************************************
注意：有锁版本只适合多生产者或多消费者
************************************************/

#ifndef __CIRCULAR_BUFFER_LOCK_H__
#define __CIRCULAR_BUFFER_LOCK_H__

#include "ccircular_buffer.h"
#include "c_lock.h"

class CcircularBufferLock: public CcircularBuffer
{
public:
	explicit CcircularBufferLock(uint buf_size);	
	virtual ~CcircularBufferLock();
	virtual int CreateBuffer();
	virtual void Reset();
	virtual int Put(u8 *pbuf, u32 len);
	virtual int Get(u8 *pbuf, u32 len);

private:
	CcircularBufferLock(const CcircularBufferLock& )
	{
		
	}

private:
	C_Lock* m_plock;

};

#endif

