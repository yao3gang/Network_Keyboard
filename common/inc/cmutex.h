#ifndef __CMUTEX_H__
#define __CMUTEX_H__

#include "c_lock.h"
#include "types.h"

enum mutex_type
{
	MUTEX_FAST = 0,
	MUTEX_RECURSIVE,
};

class CMutex : public C_Lock
{
public:
	explicit CMutex(int nType = MUTEX_FAST):type(nType)
	{
		m_plock = NULL;
		//DBG_PRINT("\n");
	}
	
	~CMutex();
	virtual int Create();
	virtual int Lock();
	virtual int Unlock();
	//VD_BOOL Enter();
	//VD_BOOL Leave();

private:
	CMutex(const CMutex&)
	{
		
	}
	
private:
	int type;
};

#endif //__CMUTEX_H__

