#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <stdlib.h>
#include "types.h"

typedef enum
{
	//to biz_device
	EM_MQ_MSG_DEV_NET_REQ=0x10, 	//ÍøÂçÃüÁîÇëÇó
	EM_MQ_MSG_DEV_STREAM_STATUS,		//Á÷×´Ì¬Í¨Öª
	
	//to stream
	EM_MQ_MSG_STREAM_STATUS=0x20,		//Á÷×´Ì¬Í¨Öª
} EM_MQ_MSG_TYPE;

typedef struct
{
	u32 msg_type;
	u32 msg_len;	//²»°üº¬Í·
} SMQHdr_t;	//ÏûÏ¢¶ÓÁÐÍ·


class CcircularBuffer
{
public:
	explicit CcircularBuffer(uint buf_size = 0);
	virtual ~CcircularBuffer();	
	virtual int CreateBuffer(); //ç”³è¯·çŽ¯å½¢ç¼“å†²ç©ºé—´	
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
	uint m_buf_size;//å¿…é¡»æ˜¯2çš„Næ¬¡æ–¹ï¼Œ
	uint m_read_p;//å†™ç´¢å¼•,è¯»ç´¢å¼•
	uint m_pre_read_p;
	uint m_write_p;
	uint m_pre_write_p;
};

#endif
	
	