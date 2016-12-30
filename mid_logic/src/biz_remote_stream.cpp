#include <map>
#include <set>
//#include <bitset>
#include <algorithm>
#include <utility>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>

#include "api.h"
#include "singleton.h"
#include "object.h"
#include "glb_error_num.h"
#include "biz.h"
#include "biz_types.h"
#include "biz_config.h"
#include "biz_net.h"
#include "biz_device.h"
#include "biz_remote_stream_define.h"
#include "atomic.h"
#include "c_lock.h"
#include "crwlock.h"
#include "ccircular_buffer.h"
#include "cthread.h"
#include "ctimer.h"
#include "ctrlprotocol.h"
#include "net.h"

//CMediaStream **************************************************************
int CMediaStream::Start()
{
	int ret = SUCCESS;
	ret = BizReqStreamStart(dev_type, dev_ip, &req, this);//返回流ID
	if (ret < 0)
	{
		ERR_PRINT("BizReqStreamStop failed, ret: %d\n", ret);
	}
	else
	{
		stream_idx = ret;//返回流ID
		b_connect = TRUE;
		
		ret = SUCCESS;
	}
	
	return ret;
}

int CMediaStream::Stop()
{
	int ret = SUCCESS;
	
	ret = BizReqStreamStop(dev_type, dev_ip, stream_idx);
	if (ret)
	{
		ERR_PRINT("BizReqStreamStop failed, ret: %d\n", ret);
	}

	b_connect = FALSE;
		
	return ret;
}



//CMediaStreamManager *******************************************
//u64 ==> h32:htonl(dev_ip)   l32:linkid 
typedef std::map<u64, CMediaStream*> MAP_IPID_PSTREAM;

class CMediaStreamManager : public CObject
{
	friend class CMediaStream;
	
public:
	PATTERN_SINGLETON_DECLARE(CMediaStreamManager);
	CMediaStreamManager();

	int Init();
	// 写消息to stream manager
	int WriteMsg(u32 msg_type, u8 *pmsg, u32 msg_len);
	
private: //function
	CMediaStreamManager();
	CMediaStreamManager(CMediaStreamManager &)
	{

	}
	void FreeSrc();//释放资源	
	
private: //data member
	VD_BOOL b_inited;
	MAP_IPID_PSTREAM map_pstream;
	
	//消息队列数据
	pthread_cond_t mq_cond;		//写flash 的条件变量
	pthread_mutex_t mq_mutex;	//条件变量互斥锁
	int mq_msg_count;			//条件，pmsg_queue 中消息数目
	CcircularBuffer *pmq_ccbuf;	//环形缓冲区
	Threadlet mq_msg_threadlet;	//消息线程
};


PATTERN_SINGLETON_IMPLEMENT(CMediaStreamManager);

CMediaStreamManager::CMediaStreamManager()
: b_inited(FALSE)

//消息队列数据
, mq_msg_count(0)
, pmq_ccbuf(NULL)
{
	
}

int CMediaStreamManager::Init()
{



	
	//创建消息队列线程及相关数据
	if (pthread_cond_init(&mq_cond, NULL))
	{
		ERR_PRINT("init mq_cond failed\n");
		goto fail;
	}
	
	if (pthread_mutex_init(&mq_mutex, NULL))
	{
		ERR_PRINT("init mq_mutex failed\n");
		goto fail;
	}

	pmq_ccbuf = new CcircularBuffer(1024);
	if (NULL == pmq_ccbuf)
	{
		ERR_PRINT("new CcircularBuffer failed\n");
		goto fail;
	}

	if (pmq_ccbuf->CreateBuffer())
	{
		ERR_PRINT("CreateBuffer CcircularBuffer failed\n");
		goto fail;
	}

	b_inited = TRUE;
	return SUCCESS;
	
fail:

	FreeSrc();
	return -FAILURE;
}
}

void CMediaStreamManager::FreeSrc()//释放资源
{		
	b_inited = FALSE;


	if (pmq_ccbuf)
	{
		delete pmq_ccbuf;
		pmq_ccbuf = NULL;
	}
}

CMediaStreamManager::~CMediaStreamManager()
{
	FreeSrc();
}






