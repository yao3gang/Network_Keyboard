
#ifndef __GUARD_H__
#define __GUARD_H__

#include "c_lock.h"

class CGuard
{
public:
	inline explicit CGuard(C_Lock& lock)
		:m_lock(lock)
	{
		m_lock.Lock();
	};
	
	inline ~CGuard()
	{
		m_lock.Unlock();
	};
protected:
private:
	C_Lock &m_lock;
};

#endif //__GUARD_H__
//
//
