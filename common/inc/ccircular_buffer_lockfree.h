/***********************************************
注意：无锁版本只适合单生产者和单消费者
************************************************/

#ifndef __CIRCULAR_BUFFER_LOCKFREE_H__
#define __CIRCULAR_BUFFER_LOCKFREE_H__

#include "ccircular_buffer.h"

class CcircularBufferLockfree: public CcircularBuffer
{
public:
	explicit CcircularBufferLockfree(uint buf_size);	
	virtual ~CcircularBufferLockfree();
	//virtual VD_BOOL CreateBuffer();使用父类的
	virtual void Reset();
	virtual int Put(u8 *pbuf, u32 len);
	virtual int Get(u8 *pbuf, u32 len);

private:
	CcircularBufferLockfree(const CcircularBufferLockfree& )
	{
		
	}

#if 0	
private:
	atomic_t m_read_p;
	atomic_t m_write_p;
#endif

};

#endif

