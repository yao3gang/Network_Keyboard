#ifndef _TIMER_H_
#define _TIMER_H_

#include "types.h"
#include "Object.h"
#include "cthread.h"
#include "cmutex.h"
#include "cguard.h"


typedef void (CObject:: * VD_TIMERPROC)(uint wParam);

class CTimerManager;
class Threadlet;

class CTimer : public CObject
{
	friend class CTimerManager;
public:
	CTimer(VD_PCSTR pName = "NoName");
	virtual ~CTimer();
	void Start(CObject * pObj, VD_TIMERPROC pTimerFun, uint dwDueTime, uint dwPriod, uint param = 0, uint timeout = 0);
	void Stop(VD_BOOL bCallNow = FALSE);
	void run();
	VD_PCSTR GetName();
	void SetName(VD_PCSTR);
	VD_BOOL IsStarted();
	VD_BOOL IsCalled();

private:
	uint64 m_CallTime;
	uint m_Priod;
	uint m_timeout;
	CObject *m_pObj;
	VD_TIMERPROC m_pTimerFun;
	uint m_param;
	char  m_Name[64];
	VD_BOOL  m_Started;
	VD_BOOL  m_Called;
	static CMutex m_Mutex;
	CTimer* m_pPrev;		//��?��????������?��
	CTimer* m_pNext;		//??��????������?��
	Threadlet m_threadlet;
};

class CTimerManager : public CThread
{
	friend class CTimer;
public:
	PATTERN_SINGLETON_DECLARE(CTimerManager);
	CTimerManager();
	~CTimerManager();
	void Start();
	VD_BOOL AddTimer(CTimer * pTimer);
	VD_BOOL RemoveTimer(CTimer * pTimer);
	void DumpTimers();
	void ThreadProc();

protected:
private:
	uint64 m_CurTime;	//o��????��y
	CTimer* m_pHead;
};

#define g_TimerManager (*CTimerManager::instance())

#endif
