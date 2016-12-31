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
#include "biz_remote_stream.h"
#include "atomic.h"
#include "c_lock.h"
#include "crwlock.h"
#include "ccircular_buffer.h"
#include "cthread.h"
#include "ctimer.h"
#include "ctrlprotocol.h"
#include "net.h"

//CMediaStream **************************************************************
CMediaStream::CMediaStream()
: plock4param(NULL)
//指向流上层结构
, m_obj(NULL)
, m_deal_frame(NULL)	//流注册的帧数据处理函数
, m_deal_status(NULL)	//流注册的状态处理函数

//流内部数据
, dev_type(EM_DEV_TYPE_NONE)	//服务器类型
, dev_ip(INADDR_NONE)			//服务器IP
, stream_id(INVALID_VALUE)		//关键，系统唯一
, err_code(SUCCESS)				//错误码
, status(EM_STREAM_STATUS_DISCONNECT) //流状态

//流接收
, b_thread_exit(FALSE);//接收线程退出标志
, s32 sock_stream(INVALID_SOCKET)
{
	memset(&req, 0, sizeof(ifly_TCP_Stream_Req)); //流请求数据结构
}

int CMediaStream::Init()
{
	plock4param = new CMutex;
	if (NULL == plock4param)
	{
		ERR_PRINT("new plock4param failed\n");
		goto fail;
	}
	
	if (plock4param->Create())//FALSE
	{
		ERR_PRINT("create plock4param failed\n");
		goto fail;
	}
	
	return SUCCESS;
	
fail:

	FreeSrc();
	return -FAILURE;
}

void CMediaStream::FreeSrc()//释放资源
{
	if (plock4param)
	{
		delete plock4param;
		plock4param = NULL;
	}
}

CMediaStream::~CMediaStream()
{
	FreeSrc();
}




//CMediaStreamManager *******************************************************
//u32:stream_id
typedef std::map<u32, CMediaStream*> MAP_ID_PSTREAM;
#define g_biz_stream_manager (*CMediaStreamManager::instance())
#define MAX_STREAM_NO (1<<20)//注意必须是2的次幂

class CMediaStreamManager : public CObject
{
	friend class CMediaStream;
	
public:
	PATTERN_SINGLETON_DECLARE(CMediaStreamManager);
	~CMediaStreamManager();

	int Init();
	// 写消息to stream manager
	int WriteMsg(u32 msg_type, u8 *pmsg, u32 msg_len);

	int ReqStreamStart(EM_DEV_TYPE dev_type, u32 dev_ip, ifly_TCP_Stream_Req *preq, u32 *pstream_id);
	
private: //function
	CMediaStreamManager();
	CMediaStreamManager(CMediaStreamManager &)
	{

	}
	void FreeSrc();//释放资源
	void threadMsg(uint param);//消息线程执行函数
	//外部加锁
	u32 _createStreamID();//得到一个新的流ID
	
private: //data member
	C_Lock *plock4param;//mutex

	VD_BOOL b_inited;
	MAP_ID_PSTREAM map_pstream;
	u32 round_stream_no;//[0, MAX_STREAM_NO-1] 轮转
	u32 stream_cnt;
	std::list<u32> list_delete;//待删除流的ID 列表
	
	//消息队列数据
	pthread_cond_t mq_cond;		//写flash 的条件变量
	pthread_mutex_t mq_mutex;	//条件变量互斥锁
	int mq_msg_count;			//条件，pmsg_queue 中消息数目
	CcircularBuffer *pmq_ccbuf;	//环形缓冲区
	Threadlet mq_msg_threadlet;	//消息线程
};


PATTERN_SINGLETON_IMPLEMENT(CMediaStreamManager);

CMediaStreamManager::CMediaStreamManager()
: plock4param(NULL)
, b_inited(FALSE)
, round_stream_no(0) //[0, MAX_STREAM_NO-1] 轮转
, stream_cnt(0)

//消息队列数据
, mq_msg_count(0)
, pmq_ccbuf(NULL)
{
	
}

int CMediaStreamManager::Init()
{
	plock4param = new CMutex;
	if (NULL == plock4param)
	{
		ERR_PRINT("new plock4param failed\n");
		goto fail;
	}
	
	if (plock4param->Create())//FALSE
	{
		ERR_PRINT("create plock4param failed\n");
		goto fail;
	}	


	
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

void CMediaStreamManager::FreeSrc()//释放资源
{		
	b_inited = FALSE;

	if (plock4param)
	{
		delete plock4param;
		plock4param = NULL;
	}

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

//外部加锁
u32 CMediaStreamManager::_createStreamID()//得到一个新的流ID
{
	u32 stream_id = round_stream_no;

	round_stream_no = (round_stream_no+1) & (MAX_STREAM_NO-1);//[0, MAX_STREAM_NO-1] 轮转

	return stream_id;
}
int CMediaStreamManager::ReqStreamStart(EM_DEV_TYPE dev_type, u32 dev_ip, ifly_TCP_Stream_Req *preq, u32 *pstream_id)
{
	int ret = SUCCESS;
	CMediaStream *pstream = NULL;
	struct in_addr in;
	in.s_addr = dev_ip;

	if (NULL == preq || NULL == pstream_id)
	{
		ERR_PRINT("param invalid\n");
		return -EPARAM;
	}
	
	if (!b_inited)
	{
		ERR_PRINT("module not init\n");
		return -FAILURE;
	}

	pstream = new CMediaStream;
	ret = pstream->Init();
	if (ret)
	{
		ERR_PRINT("CMediaStream init failed, ret: %d\n", ret);

		return ret;
	}

	memcpy(&pstream->req, preq, sizeof(ifly_TCP_Stream_Req));
	
	plock4param->Lock();

	pstream->stream_id = _createStreamID();
	
	//insert map
	if (!map_pstream.insert(std::make_pair(pstream->stream_id, pstream)).second)
	{
		ERR_PRINT("dev ip: %s, cmd: %d, map insert failed\n", inet_ntoa(in), preq.command);

		plock4param->Unlock();
		goto fail;
	}

	plock4param->Unlock();
	
	//write msg to threadmsg, in threadmsg connect
	

	return SUCCESS;

fail:
	if (pstream)
	{
		delete pstream;
		pstream = NULL;
	}

	return -FAILURE;		
}


//extern API
//流控制API
//pstream_id 返回流ID
int BizReqPlaybackByFile(EM_DEV_TYPE dev_type, u32 dev_ip, char *file_name, u32 offset, u32 *pstream_id)
{
	ifly_TCP_Stream_Req req;

	memset(&req, 0, sizeof(ifly_TCP_Stream_Req));

	if (NULL == file_name || NULL == pstream_id)
	{
		return -EPARAM;
	}

	if (strlen(file_name) >= sizeof(req.FilePlayBack_t.filename))
	{
		ERR_PRINT("file_name len(%d) too long\n", strlen(file_name))
		return -EPARAM;
	}

	//0：预览 1：文件回放 2：时间回放 3：文件下载 4：升级 
	//5 VOIP 6 文件按帧下载 7 时间按帧下载 8 透明通道
	//9 远程格式化硬盘 10 主机端抓图 11 多路按时间回放 12 按时间下载文件
	req.command = 1;
	strcpy(req.FilePlayBack_t.filename, file_name);
	req.FilePlayBack_t.offset = offset;
	
	return g_biz_stream_manager.ReqStreamStart(dev_type, dev_ip, &req, pstream_id);
}



