#ifndef __CSPIN_LOCK_H__
#define __CSPIN_LOCK_H__

#include "c_lock.h"
#include "types.h"

class CSpinLock : public C_Lock
{
public:
	explicit CSpinLock()
	{
		m_plock = NULL;
		//DBG_PRINT("\n");
	}
	
	~CSpinLock();
	virtual int Create();
	virtual int Lock();
	virtual int Unlock();

private:
	CSpinLock(const CSpinLock&)
	{
		
	}
};

#endif //__CMUTEX_H__
