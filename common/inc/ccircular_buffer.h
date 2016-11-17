#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <stdlib.h>
#include "types.h"

class CcircularBuffer
{
public:
	explicit CcircularBuffer(uint buf_size = 0);
	virtual ~CcircularBuffer();	
	virtual int CreateBuffer(); //申请环形缓冲空间	
	virtual void Reset();	
	virtual int Put(uchar *pbuf, uint len);
	virtual int Get(uchar *pbuf, uint len);
	
private:
	CcircularBuffer(const CcircularBuffer &)
	{
		
	}
	
protected:
	uchar *m_pbuf;
	uint m_buf_size;//必须是2的N次方，
	uint m_read_p;//写索引,读索引
	uint m_write_p;
};

#endif
	
	