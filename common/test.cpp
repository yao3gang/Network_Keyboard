#include <new>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#endif

#include "types.h"
#include "ccircular_buffer_lock.h"
#include "ccircular_buffer_lockfree.h"

#include "ctimer.h"
#include "cthread.h"

typedef struct
{
	uint len;
} MSG_T;

void tsk1(void *parg)
{
	CcircularBuffeLock* pcirbuf = (CcircularBuffeLock *)parg;
	uchar buf[100];
	char str[] = "tsk1 write msg";
	MSG_T msg;
	int cnt = 0;
	msg.len = strlen(str)+1;

	memcpy(buf, &msg, sizeof(MSG_T));
	memcpy(buf+sizeof(MSG_T), str, msg.len);

	while (1)
	{
		if (!pcirbuf->Put(buf, sizeof(MSG_T)+msg.len))
		{
			ERR_PRINT("tsk1 put msg failed\n");
		}
		usleep(200*10000);

		if (cnt++ > 40)
			break;
	}
	DBG_PRINT("tsk1 end\n");
	return;
}

void tsk2(void *parg)
{
	CcircularBuffeLock* pcirbuf = (CcircularBuffeLock *)parg;
	uchar buf[100];
	char str[] = "tsk2 write msg";
	MSG_T msg;
	int cnt = 0;

	msg.len = strlen(str)+1;

	memcpy(buf, &msg, sizeof(MSG_T));
	memcpy(buf+sizeof(MSG_T), str, msg.len);

	while (1)
	{
		//if (cnt > 30)
		{
			//msg.len = 5;

			//memcpy(buf, &msg, sizeof(MSG_T));
			//memcpy(buf+sizeof(MSG_T), "quit", msg.len);
		}
		
		if (!pcirbuf->Put(buf, sizeof(MSG_T)+msg.len))
		{
			//ERR_PRINT("tsk2 put msg failed\n");
		}
		usleep(2*10000);
		
		//if (cnt++ > 30)
		//	break;
	}
	DBG_PRINT("tsk2 end\n");
	return;

}

void tsk3(void *parg)
{
	CcircularBuffeLock* pcirbuf = (CcircularBuffeLock *)parg;
	uchar buf[100];
	MSG_T msg;

	while (1)
	{
		if (!pcirbuf->Get((uchar *)&msg, sizeof(MSG_T)))
		{
			//ERR_PRINT("tsk3 Get MSG_T failed\n");
		}
		else
		{
			if (!pcirbuf->Get(buf, msg.len))
			{
				ERR_PRINT("tsk3 Get msg failed\n");
			}
			else
			{
				DBG_PRINT("msg: %s\n", buf);
				if (strcmp((char *)buf, "quit")==0)
					break;
			}
		}
		usleep(1*10000);
	}

	DBG_PRINT("tsk3 end\n");
	return;

}

int main(void)
{
	VD_BOOL b_ret = FALSE;
	g_ThreadManager.RegisterMainThread(ThreadGetID());
	g_TimerManager.Start();

	
	CcircularBuffeLockfree ring_buf(2011);
	
	b_ret = ring_buf.CreateBuffer();
	if (!b_ret)
	{
		ERR_PRINT("CcircularBuffeLock create failed\n");
		return 1;
	}

	pthread_t pid1, pid2, pid3;
	//if (pthread_create(&pid1, NULL, (void* (*)(void *))tsk1, (void*)&ring_buf))
	{
	//	ERR_PRINT("create pid1 failed\n");
	}
	
	if (pthread_create(&pid2, NULL, (void* (*)(void *))tsk2, (void*)&ring_buf))
	{
		ERR_PRINT("create pid2 failed\n");
	}

	if (pthread_create(&pid3, NULL, (void* (*)(void *))tsk3, (void*)&ring_buf))
	{
		ERR_PRINT("create pid2 failed\n");
	}

	//pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
	pthread_join(pid3, NULL);

	
	DBG_PRINT("main end\n");
	return 0;
}



