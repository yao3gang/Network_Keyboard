#ifndef __CRW_LOCK_H__
#define __CRW_LOCK_H__

#include "c_lock.h"
#include "types.h"

class CRWlock : public C_Lock
{
public:
	explicit CRWlock()
	{
		m_plock = NULL;
	}
	
	~CRWlock();
	
	int Create();
	int Unlock();
	int Rdlock();
	int Wrlock();

private:
	CRWlock(const CRWlock&)
	{
		
	}
};

#endif //__RW_LOCK_H__
