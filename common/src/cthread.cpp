#include <algorithm>


#include "ctimer.h"
#include "thread.h"
#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#endif

void ThreadBody(void *pdat)
{
	//printf("$$$$$$$$$$$$$$$$$$ThreadBody id:%d\n",getpid());
	
	//pthread_detach(pthread_self());//csp modify 20121018
	
	CThread *pThread = (CThread *)pdat;
	
	/************************************************************************
		??3¨¬¨ª¡§¨®???¦̡o¡¥¨ºy¡魊		1?¡罿¦ʿ??3¨¬ID¡轊		2?¡翡ツ¨¬??¡§???¨®¦ʿ??3¨¬??¦̡o¡¥¨ºy¡轊		3?¡駌¡(¬??¡§???¨®¦ʿ??3¨¬??¦̡o¡¥¨ºy?¡ツ¨ª¨º¡�¡迡὿???3¨¬¡䨮??3¨¬1¨¹¨¤¨ª?¨®¨¢D?D¨¦?3y¡轊		4?¡騺¨ª¡¤?¨ª?2?D?o?¨¢?¡轊		5?¡颣???3¨¬??¡(²???a??¡轊	************************************************************************/
	pThread->m_ID = ThreadGetID();
	
	//printf("create gui thread id: %d\n", syscall(__NR_gettid));
	
	g_ThreadManager.AddThread(pThread);
	//printf("ThreadBody Enter id = %d, handle = 0x%x, stack = 0x%0x\n",pThread->m_ID, (int)pThread->m_hThread, &pThread);
	pThread->ThreadProc();
	//printf("ThreadBody Leave id = %d, handle = 0x%x\n",pThread->m_ID, (int)pThread->m_hThread);
	pThread->ClearMessage();
	if (pThread->m_bWaitThreadExit)
	{
		pThread->m_desSemaphore.Post();
	}
	pThread->m_cSemaphore.Post();
	g_ThreadManager.RemoveThread(pThread);
	
	//printf("thread exit id: %d\n", syscall(__NR_gettid));
	
	ThreadExit();
}

int CThread::num = 0;

CThread::CThread(const char*pName, int nPriority, int nMsgQueSize /* = 0 */, uint dwStackSize /* = 0 */)
: m_cSemaphore(1)
, m_desSemaphore(0)
{
	m_nPriority = nPriority;
	m_dwStackSize = dwStackSize;
	if(pName)
	{
		strcpy(m_Name, pName);
	}
	else
	{
		strcpy(m_Name, "Nonamed");
	}
	m_hThread = NULL;
	m_ID = -1;
	m_bLoop = FALSE;
	if(nMsgQueSize)
	{
		m_pMsgQue = new CMsgQue(nMsgQueSize);
	}
	else
	{
		m_pMsgQue = NULL;
	}
	m_expectedTime = 0;

	m_bWaitThreadExit = FALSE;
}

CThread::~CThread()
{
	if (m_hThread)
	{
		DestroyThread();
	}
	
	if(m_pMsgQue)
	{
		delete m_pMsgQue;
	}
}

int CThread::SetThreadName(const char*pName)
{
	if(pName)
	{
		size_t len = std::min(sizeof(m_Name)-1, strlen(pName));
		memset(m_Name, 0, sizeof(m_Name));
		strncpy(m_Name, pName, len);
	}
	else
	{
		strcpy(m_Name, "Nonamed");
	}

	return 0;
}

int CThread::CreateThread()
{
/************************************************************************
	¡䢤?¡§??3¨¬¡魊	1?¡騦¨¨??D?o?¨¢?o¨ª??3¨¬??¦̡o¡¥¨ºy?D¦ʿ?-?¡¤¡(º??¡轊	2?¡駌¡¨®?¡䢤?¡§??3¨¬API¡轊	3?¡騨?1???3¨¬¡䢤?¡§3¨¦1|¡迡὿???3¨¬???¨®?¨®¨¨?¦ʿ??3¨¬1¨¹¨¤¨ª?¨®¨¢D¡轲¡顤¦ʿ?TURE¡轊	   ¡¤??¨°¡¤¦ʿ?FALSE¡轊************************************************************************/
	//printf("1^^^^^^^%s,%d\n",__func__,__LINE__);//cw_test
	m_cSemaphore.Pend();
	//printf("2^^^^^^^%s,%d\n",__func__,__LINE__);//cw_test
	m_bLoop = TRUE;

	int ret = ThreadCreate(&m_hThread, ThreadBody, this, m_nPriority, m_dwStackSize, m_Name) == 0 ? TRUE : FALSE;
	//return ThreadCreate(&m_hThread, ThreadBody, this, m_nPriority, m_dwStackSize, m_Name) == 0 ? TRUE : FALSE;
	num++;
	printf("%s return: %d, num: %d\n", __func__, ret?1:0, num);
	return ret;
}

int CThread::DestroyThread(VD_BOOL bWaited)
{
/************************************************************************
	?¨¢¨º???3¨¬¡魊	1?¡翡�¨®¡Ῠ®?¨®¨²?????¡?-?¡¤¦ʿ¡�¨¢????aFALSE¡迨¨??¨¹¨ª?3??¡?-?¡¤¡轊************************************************************************/
	if (!IsThreadOver())
	{
		m_bLoop = FALSE;
		VD_SendMessage(XM_QUIT);//¨¨?1?¡¤¡翨ª3?¡䨪¡轿¦ʿ¡?¨®¨¢D¨°??¨²¡迡⿨º¡�_bLoop?¨¦¨°?¡!迡潿3¨¬?-?¡¤¨ª?3?

		// 2?¨º?¡�?o1?¡�?o¦ʿ¨º¡�¨°2?2¨¦¨®?¡h¨¨¨?¨º?
		if (m_ID != ThreadGetID())
		{
			m_bWaitThreadExit = bWaited;
			if (m_bWaitThreadExit)
			{
				m_desSemaphore.Pend();
			}
		}
	}
	return SUCCESS;
}

VD_BOOL CThread::IsThreadOver()
{
/************************************************************************
	?D????3¨¬??¦̡o¡¥¨ºy¨º?¡¤?¨°??-¨ª¡轱¡魊	1?¡騨?1?¨°??-¨ª¡轱¡轿¨°¡¤¦ʿ?TRUE¡迡¤??¨°¡¤¦ʿ?FALSE¡轊************************************************************************/
	return !m_bLoop;
	
}

int CThread::GetThreadID()
{
	return m_ID;
}

#if 0
int CThread::ShareSocket(int nSocket)
{
	return ThreadShareSocket(m_hThread, nSocket);
}
#endif

int CThread::VD_SendMessage(uint msg, uint wpa /* = 0 */, uint lpa /* = 0 */, uint priority /* = 0 */)
{
/************************************************************************
	¡¤¡翨ª???¡颪o
	1?¡羄??¡�??3¨¬¨º?¡¤?¨®D???¡翨®¨¢D¡轊	2?¡騨?1?¨®D¡轿¨°¦̡¨®????¡翨®¨¢D???¨®¦ʿSendMessageo¡¥¨ºy¡¤¡翨ª???¡颪?
	   ¡¤??¨°?¡�¨®¡¤¦ʿ?FALSE¡轊************************************************************************/
	if(m_pMsgQue)
	{
		return m_pMsgQue->SendMessage(msg, wpa, lpa, priority);
	}
	
	return -FAILURE;
}

int CThread::VD_RecvMessage(VD_MSG *pMsg, VD_BOOL wait /* = TRUE */)
{
/************************************************************************
	?¨®¨º????¡颪o
	1?¡羄??¡�??3¨¬¨º?¡¤?¨®D???¡翨®¨¢D¡轊	2?¡騨?1?¨®D¡轿¨°¦̡¨®????¡翨®¨¢D???¨®¦ʿRecvMessageo¡¥¨ºy?¨®¨º????¡颪?
	   ¡¤??¨°?¡�¨®¡¤¦ʿ?FALSE¡轊************************************************************************/
	if(m_pMsgQue)
	{
		return m_pMsgQue->RecvMessage(pMsg, wait);
	}
	return FALSE;
}

void CThread::QuitMessage()
{
/************************************************************************
	¨ª¡轱???¡颪o
	1?¡羄??¡�??3¨¬¨º?¡¤?¨®D???¡翨®¨¢D¡轊	2?¡騨?1?¨®D¡轿¨°¦̡¨®????¡翨®¨¢D???¨®¦ʿQuitMessageo¡¥¨ºy¨ª¡轱???¡颪?
	   ¡¤??¨°?¡�¨®¡¤¦ʿ?FALSE¡轊************************************************************************/
	if(m_pMsgQue)
	{
		m_pMsgQue->QuitMessage();
	}
}

void CThread::ClearMessage()
{
	if(m_pMsgQue)
	{
		m_pMsgQue->ClearMessage();
	}
}

uint CThread::GetMessageCount()
{
	if(m_pMsgQue)
	{
		return m_pMsgQue->GetMessageCount();
	}
	return 0;
}

void CThread::SetTimeout(uint milliSeconds)
{
	if(milliSeconds == 0) // ?????¡濨²¨º¡�?
	{
		m_expectedTime = 0;
	}
	else
	{
		m_expectedTime = SystemGetMSCount() + milliSeconds;

		if(m_expectedTime < milliSeconds) // ??¨ºy¨°?3?¡轠?Y¨º¡�?¨¦¨¨???¡濨²¨º¡(º¡�?
		{
			m_expectedTime = 0;
		}
	}
}

VD_BOOL CThread::IsTimeout()
{
	return (m_expectedTime != 0 && m_expectedTime < SystemGetMSCount());
}

////////////////////////////////////////////////////////////////////////////////////
// PooledThread
////////////////////////////////////////////////////////////////////////////////////
PooledThread::PooledThread() : CThread("Pooled", TP_DEFAULT)
{
}

PooledThread::~PooledThread()
{
}

void PooledThread::ThreadProc()
{	
	while(m_bLoop)
	{
		m_semaphore.Pend();
		(m_obj->*m_asynFunc)(m_param); 
		SetThreadName("Pooled");
		SetTimeout(0);
		Threadlet::m_mutex.Enter();
		if(m_caller)
		{
			m_caller->m_thread = NULL;
			m_caller = NULL;
		}
		Threadlet::m_mutex.Leave();
		g_ThreadManager.ReleasePooledThread(this);
	}
}

////////////////////////////////////////////////////////////////////////////////////
// Threadlet
////////////////////////////////////////////////////////////////////////////////////
CMutex Threadlet::m_mutex;

Threadlet::Threadlet()
{
	m_thread = NULL;
}

Threadlet::~Threadlet()
{
	CGuard guard(m_mutex);	

	if(m_thread)
	{
		m_thread->m_caller = NULL;
	}
}

VD_BOOL Threadlet::run(VD_PCSTR name, CObject * obj, ASYNPROC asynProc, uint param, uint timeout)
{
	CGuard guard(m_mutex);

	if(m_thread) //��������
	{
		return FALSE;
	}

	m_thread = g_ThreadManager.GetPooledThread();
	m_thread->SetThreadName(name);
	m_thread->SetTimeout(timeout);
	m_thread->m_obj = obj;
	m_thread->m_asynFunc = asynProc;
	m_thread->m_param = param;
	m_thread->m_caller = this;
	m_thread->m_semaphore.Post();

	return TRUE;
}

VD_BOOL Threadlet::isRunning()
{	
	CGuard guard(m_mutex);
	
	return (m_thread != NULL);
}

////////////////////////////////////////////////////////////////////////////////////
// CThreadManager
////////////////////////////////////////////////////////////////////////////////////

PATTERN_SINGLETON_IMPLEMENT(CThreadManager);

CThreadManager::CThreadManager() : m_Mutex(MUTEX_RECURSIVE)
{
	printf("CThreadManager::CThreadManager()>>>>>>>>>\n");
	m_pHead = NULL;
	m_headPooled = NULL;
	
	if (m_Mutex.Create())
	{
		ERR_PRINT("CThreadManager m_Mutex.Create()\n");
	}

	if (Threadlet::m_mutex.Create())
	{
		ERR_PRINT("Threadlet m_mutex.Create\n");
	}
}

CThreadManager::~CThreadManager()
{
}

void CThreadManager::RegisterMainThread(int id)
{
	m_mainThreadID = id;
}

VD_BOOL CThreadManager::AddThread(CThread *pThread)
{
	//DBG_PRINT("\n");
	CGuard guard(m_Mutex);

	pThread->m_pPrev = NULL;
	pThread->m_pNext = m_pHead;

	if(m_pHead)
	{
		m_pHead->m_pPrev = pThread;
	}

	m_pHead = pThread;

	return TRUE;
}

VD_BOOL CThreadManager::RemoveThread(CThread *pThread)
{
	CGuard guard(m_Mutex);

	if(pThread->m_pPrev == NULL)
	{
		m_pHead = pThread->m_pNext;
		if(m_pHead)
		{
			m_pHead->m_pPrev = NULL;
		}

		return TRUE;
	}

	pThread->m_pPrev->m_pNext = pThread->m_pNext;
	if(pThread->m_pNext != NULL)
		pThread->m_pNext->m_pPrev = pThread->m_pPrev;

	return TRUE;
}

VD_BOOL CThreadManager::HasThread(CThread *pThread)
{
	CGuard guard(m_Mutex);
	CThread * p = m_pHead;

	while (p)
	{
		if (p == pThread)
		{
			return TRUE;
		}
		p = p->m_pNext;
	}

	return FALSE;
}

#ifdef FUNC_THREAD_DETECT
VD_BOOL CThreadManager::GetTimeOutThread(std::string &threadName)
{
	CThread *p = m_pHead;
	VD_BOOL ret = FALSE;
	threadName.clear();

	while(p)
	{
		if (p->IsTimeout())
		{
			threadName += p->m_Name;		
			ret = TRUE;
		}

		p = p->m_pNext;
	}
	
	return ret;
}
#endif

PooledThread * CThreadManager::GetPooledThread()
{
	CGuard guard(m_Mutex);
	PooledThread * p = m_headPooled;
	VD_BOOL ret;
	
	if(p) 
	{
		m_headPooled = p->m_nextPooled;
	}
	else 
	{
		p = new PooledThread();
		ret = p->CreateThread();
		if (FALSE == ret)
		{
			printf("%s CreateThread failed\n", __func__);
		}
	}

	return p;
}

void CThreadManager::ReleasePooledThread(PooledThread* pThread)
{
	CGuard guard(m_Mutex);
	assert(pThread);

	pThread->m_nextPooled = m_headPooled;
	m_headPooled = pThread;
}

void CThreadManager::DumpThreads()
{
	CGuard guard(m_Mutex);
	CThread * p = m_pHead;

	printf("Threads:\n");
	printf("               Name            PID  Prior State\n");
	printf("_______________________________________________________\n");
	printf("%24s   %8d  %3d  %s\n", "Main", m_mainThreadID, TP_DEFAULT, "Normal");
	while (p)
	{
		printf("%24s   %8d  %3d  %s\n", p->m_Name, p->m_ID, p->m_nPriority, p->IsTimeout() ? "Timeout" : "Normal");
		p = p->m_pNext;
	}
}
