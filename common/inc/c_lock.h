#ifndef __C_LOCK_H__
#define __C_LOCK_H__

#include <stdlib.h>
#include "types.h"
#include "glb_error_num.h"
/************************************************
 抽象基类 申明一套接口
************************************************/

class C_Lock
{
public:
	explicit C_Lock()
	{
		m_plock = NULL;
	}
	
	virtual ~C_Lock()
	{
		
	}
	
	virtual int Create()//初始化操作在此，返回值告知外部类是否初始化成功
	{
		return -FAILURE;
	}
	
	virtual int Lock()
	{
		return -FAILURE;
	}
	
	virtual int Unlock()
	{
		return -FAILURE;
	}

	virtual int Rdlock()
	{
		return -FAILURE;
	}

	virtual int Wrlock()
	{
		return -FAILURE;
	}
	
	virtual int Enter()
	{
		return Lock();
	}
	
	virtual int Leave()
	{
		return Unlock();
	}
	
protected:
	VD_HANDLE m_plock;
	
private:
	C_Lock(const C_Lock&)
	{
		
	}
};

#endif //__C_LOCK_H__

