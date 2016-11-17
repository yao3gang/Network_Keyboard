#ifndef _THREAD_H_
#define _THREAD_H_

#include "thread.h"
#include "Object.h"
#include "cguard.h"
#include "cmsgque.h"
#include "singleton.h"
#include <string>

typedef void (CObject:: * ASYNPROC)(uint wParam);

class CThread : public CObject
{
	friend void ThreadBody(void *pdat);
	friend class CThreadManager;

public:
	static int num;
	CThread(const char*pName, int nPriority, int nMsgQueSize = 0, uint dwStackSize = 0);
	virtual ~CThread();
	int CreateThread();
	int DestroyThread(VD_BOOL bWaited = FALSE);
	VD_BOOL IsThreadOver();
	int	GetThreadID();
	int VD_SendMessage (uint msg, uint wpa = 0, uint lpa = 0, uint priority = 0);
	int VD_RecvMessage (VD_MSG *pMsg, VD_BOOL wait = TRUE);
	void QuitMessage ();
	void ClearMessage();
	uint GetMessageCount();
	virtual void ThreadProc() = 0;
	int SetThreadName(const char*pName);
	int ShareSocket(int nSocket);
	void SetTimeout(uint milliSeconds);
	VD_BOOL IsTimeout();

	volatile VD_BOOL	m_bLoop;
protected:
	
	volatile VD_BOOL	m_bWaitThreadExit;
private:
	int		m_nPriority;
	uint	m_dwStackSize;
	VD_HANDLE	m_hThread;
	int		m_ID;
	char	m_Name[64];
	CMsgQue* m_pMsgQue;
	CThread* m_pPrev;		//é?ò?????3ì
	CThread* m_pNext;		//??ò?????3ì
	CSemaphore m_cSemaphore;	// ??D?o?á?ó?à′·à?1í?ò??????óμ???3ìí?ê±±?′′?¨?à′?￡?
	uint	m_expectedTime;	//?¤???′DD?áê?ê±μ?ê±??￡?0±íê?2??¤??
	CSemaphore m_desSemaphore;
	
};

class Threadlet;

class PooledThread : public CThread
{
	friend class CThreadManager;
	friend class Threadlet;

public:
	PooledThread();
	~PooledThread();
	void ThreadProc();

private:
	CObject *m_obj;
	ASYNPROC m_asynFunc;
	uint m_param;
	CSemaphore m_semaphore;
	PooledThread* m_nextPooled;
	Threadlet* m_caller;
};

class Threadlet
{
	friend class CThreadManager;
	friend class PooledThread;

public:
	Threadlet();
	~Threadlet();
	VD_BOOL run(VD_PCSTR name, CObject * obj, ASYNPROC asynProc, uint param, uint timeout);
	VD_BOOL isRunning();

private:
	PooledThread* m_thread;
	static CMutex m_mutex;	///< ±￡?¤ThreadletoíPooledThreadμ?1?áa1??μ
};

class CThreadManager : public CObject
{
	friend class CThread;
	friend class PooledThread;
	friend class Threadlet;
	friend void ThreadBody(void *pdat);

public:
	PATTERN_SINGLETON_DECLARE(CThreadManager);
	CThreadManager();
	~CThreadManager();

	void RegisterMainThread(int id);
	VD_BOOL HasThread(CThread *pThread);
	void DumpThreads();
	VD_BOOL GetTimeOutThread(std::string  &threadName);

private:
	VD_BOOL AddThread(CThread *pThread);
	VD_BOOL RemoveThread(CThread *pThread);
	PooledThread * GetPooledThread();
	void ReleasePooledThread(PooledThread* pThread);

private:
	CThread* m_pHead;//工作线程列表，已分配任务
	PooledThread* m_headPooled;//空闲线程列表
	CMutex m_Mutex;
	int m_mainThreadID;
};

#define g_ThreadManager (*CThreadManager::instance())

#endif //_THREAD_H_
