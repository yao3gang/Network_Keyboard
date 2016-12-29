#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <stdlib.h>
#include "types.h"

typedef struct
{
	u32 msg_type;
	u32 msg_len;//������ͷ
} SMQHdr_t;//��Ϣ����ͷ

class CcircularBuffer
{
public:
	explicit CcircularBuffer(uint buf_size = 0);
	virtual ~CcircularBuffer();	
	virtual int CreateBuffer(); //申请环形缓冲空间	
	virtual void Reset();	
	virtual void PutRst();
	virtual int Put(u8 *pbuf, u32 len);
	virtual void GetRst();
	virtual int Get(u8 *pbuf, u32 len);
	
private:
	CcircularBuffer(const CcircularBuffer &)
	{
		
	}
	
protected:
	uchar *m_pbuf;
	uint m_buf_size;//必须是2的N次方，
	uint m_read_p;//写索引,读索引
	uint m_pre_read_p;
	uint m_write_p;
	uint m_pre_write_p;
};

#endif
	
	