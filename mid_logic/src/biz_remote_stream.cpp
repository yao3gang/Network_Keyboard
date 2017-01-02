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
, m_deal_frame_cb(NULL)	//流注册的帧数据处理函数
, m_deal_status_cb(NULL)	//流注册的状态处理函数

//流内部数据
, dev_type(EM_DEV_TYPE_NONE)	//服务器类型
, dev_ip(INADDR_NONE)			//服务器IP
, stream_id(INVALID_VALUE)		//关键，系统唯一
, stream_errno(SUCCESS)				//错误码
, status(EM_STREAM_STATUS_DISCONNECT) //流状态

//流接收
, b_thread_running(FALSE)//接收线程运行标志
, b_thread_exit(FALSE)//接收线程退出标志
, sock_stream(INVALID_SOCKET)
, sem_exit(0)
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

void CMediaStream::threadRcv(uint param)//接收服务器数据
{
	
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
	int ReqStreamStart(
			EM_DEV_TYPE dev_type,
			u32 dev_ip,
			ifly_TCP_Stream_Req *preq,
			CObject *obj,
			PDEAL_FRAME deal_frame_cb,
			PDEAL_STATUS deal_status_cb,
			u32 *pstream_id );

	int ReqStreamStop(u32 stream_id);
	
	// 写消息to stream manager
	int WriteMsg(SBizMsg_t *pmsg, u32 msg_len);
	
private: //function
	CMediaStreamManager();
	CMediaStreamManager(CMediaStreamManager &)
	{

	}
	void FreeSrc();//释放资源
	int dealStreamConnect(u32 stream_id);
	int dealStreamDel(u32 stream_id);
	int dealStreamMsgStop(u32 stream_id, s32 stream_errno);
	int dealMsg(SBizMsg_t *pmsg, u32 msg_len);
	void threadMsg(uint param);//消息线程执行函数
	//外部加锁
	u32 _createStreamID();//得到一个新的流ID
	
private: //data member
	C_Lock *plock4param;//mutex

	VD_BOOL b_inited;
	MAP_ID_PSTREAM map_pstream;
	u32 round_stream_no;//[0, MAX_STREAM_NO-1] 轮转
	u32 stream_cnt;
	
	//消息队列数据
	pthread_cond_t mq_cond;		//写flash 的条件变量
	pthread_mutex_t mq_mutex;	//条件变量互斥锁
	u32 mq_msg_count;			//条件，pmsg_queue 中消息数目
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

	//启动消息队列读取线程
	mq_msg_threadlet.run("BizStreamManager-mq", this, (ASYNPROC)&CMediaStreamManager::threadMsg, 0, 0);

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

int CMediaStreamManager::WriteMsg(SBizMsg_t *pmsg, u32 msg_len)
{
	int ret = SUCCESS;

	if (!b_inited)
	{
		ERR_PRINT("module not init\n");
		return -FAILURE;
	}

	if (NULL == pmsg)
	{
		ERR_PRINT("NULL == pmsg\n");
		
		return -EPARAM;
	}

	if (sizeof(SBizMsg_t) != msg_len)
	{
		ERR_PRINT("sizeof(SBizMsg_t)(%d) != msg_len(%d)\n",
			sizeof(SBizMsg_t), msg_len);
		
		return -EPARAM;
	}

	ret = pthread_mutex_lock(&mq_mutex);
	if (ret)
	{
		ERR_PRINT("pthread_mutex_lock failed, err(%d, %s)\n", ret, strerror(ret));
		
		return -FAILURE;
	}

	ret = pmq_ccbuf->Put((u8 *)pmsg, msg_len);
	if (ret)
	{
		ERR_PRINT("pmsg_queue Put msg failed\n");

		pmq_ccbuf->PutRst();
		goto fail;
	} 

	mq_msg_count++;

	ret = pthread_cond_signal(&mq_cond);
	if (ret)
	{
		ERR_PRINT("pthread_cond_signal failed, err(%d, %s)\n", ret, strerror(ret));
		goto fail;
	}

	ret = pthread_mutex_unlock(&mq_mutex);
	if (ret)
	{
		ERR_PRINT("pthread_mutex_unlock failed, err(%d, %s)\n", ret, strerror(ret));
		
		return -FAILURE;
	}
	
	return SUCCESS;

fail:
	pthread_mutex_unlock(&mq_mutex);
	return ret;
}

int CMediaStreamManager::dealStreamConnect(u32 stream_id)
{
	int ret = SUCCESS;
	s32 sock_stream = INVALID_VALUE;
	struct in_addr in;
	EM_DEV_TYPE dev_type;//服务器类型
	u32 dev_ip = INADDR_NONE;//服务器IP
	ifly_TCP_Stream_Req req;//流请求数据结构
	CMediaStream *pstream = NULL;
	MAP_ID_PSTREAM::iterator map_iter;
	SBizMsg_t msg;
	
	memset(&msg, 0, sizeof(SBizMsg_t));
	msg.msg_type = EM_STREAM_MSG_CONNECT_FALIURE;
	
	plock4param->Lock();
	
	map_iter = map_pstream.find(stream_id);
	if (map_iter == map_pstream.end())
	{
		ERR_PRINT("stream_id(%d) not found in map\n", stream_id);

		plock4param->Unlock();
		return -EPARAM;
	}
	
	pstream = map_iter->second;
	if (NULL == pstream)
	{
		ERR_PRINT("stream_id(%d) NULL == pstream\n", stream_id);

		plock4param->Unlock();
		return -EPARAM;
	}
	
	pstream->plock4param->Lock();

	dev_type = pstream->dev_type;
	dev_ip = pstream->dev_ip;
	memcpy(&req, &pstream->req, sizeof(ifly_TCP_Stream_Req));

	in.s_addr = dev_ip;

	if (EM_STREAM_STATUS_DISCONNECT != pstream->status)
	{
		ERR_PRINT("dev ip: %s, EM_STREAM_STATUS_DISCONNECT != pstream->status(%d)",
			inet_ntoa(in), pstream->status);

		pstream->plock4param->Unlock();
		plock4param->Unlock();
		return -EPARAM;
	}
	
	ret = BizDevReqStreamStart(dev_type, dev_ip, stream_id, &req, &sock_stream);
	if (ret)//failed
	{				
		ERR_PRINT("dev ip: %s, stream_id: %d, req cmd: %d, BizReqStreamStart failed, ret: %d\n",
			inet_ntoa(in), stream_id, req.command, ret);

		//内部数据
		pstream->stream_errno = ret;

		//发消息
		msg.stream_err.stream_id = stream_id;
		msg.stream_err.stream_errno = ret;
	}
	else//success
	{
		DBG_PRINT("dev ip: %s, stream_id: %d, req cmd: %d, BizReqStreamStart success\n",
			inet_ntoa(in), stream_id, req.command);

		//内部数据
		pstream->sock_stream = sock_stream;
		pstream->status = EM_STREAM_STATUS_RUNNING;
		pstream->stream_errno = SUCCESS;
		
		//启动流读取线程
		pstream->b_thread_running = TRUE;
		pstream->m_threadlet_rcv.run("BizStream", pstream, (ASYNPROC)&CMediaStream::threadRcv, 0, 0);

		//发消息
		msg.msg_type = EM_STREAM_MSG_CONNECT_SUCCESS;
		msg.stream_id = stream_id;
	}
	
	//状态回调，上层实现建议用消息传递，
	//否则可能锁死在pstream->plock4param
	if (pstream->m_obj && pstream->m_deal_status_cb)
		(pstream->m_obj->*(pstream->m_deal_status_cb))(&msg, sizeof(SBizMsg_t));
	
	pstream->plock4param->Unlock();
	plock4param->Unlock();

	return ret;
}

int CMediaStreamManager::dealStreamDel(u32 stream_id)
{
	int ret = SUCCESS;
	struct in_addr in;
	EM_DEV_TYPE dev_type;//服务器类型
	u32 dev_ip = INADDR_NONE;//服务器IP
	CMediaStream *pstream = NULL;
	MAP_ID_PSTREAM::iterator map_iter;
	u8 req_cmd = INVALID_VALUE;
	VD_BOOL b_thread_running = FALSE;
	CObject *obj = NULL;
	PDEAL_STATUS deal_status_cb = NULL;
	SBizMsg_t msg;
	memset(&msg, 0, sizeof(SBizMsg_t));
	msg.msg_type = EM_STREAM_MSG_DEL_FALIURE;
	
	plock4param->Lock();
	
	map_iter = map_pstream.find(stream_id);
	if (map_iter == map_pstream.end())
	{
		ERR_PRINT("stream_id(%d) not found in map\n", stream_id);

		plock4param->Unlock();
		return -EPARAM;
	}
	
	pstream = map_iter->second;	
	if (NULL == pstream)
	{
		ERR_PRINT("stream_id(%d) NULL == pstream\n", stream_id);

		plock4param->Unlock();
		return -EPARAM;
	}
	
	pstream->plock4param->Lock();

	dev_type = pstream->dev_type;
	dev_ip = pstream->dev_ip;
	req_cmd = pstream->req.command;
	
	pstream->b_thread_exit = TRUE;
	b_thread_running = pstream->b_thread_running;
	obj = pstream->m_obj;
	deal_status_cb = pstream->m_deal_status_cb;

	in.s_addr = dev_ip;
	
	if (EM_STREAM_STATUS_WAIT_DEL != pstream->status)
	{
		ERR_PRINT("dev ip: %s, stream_id: %d, EM_STREAM_STATUS_WAIT_DEL != pstream->status(%d)\n",
			inet_ntoa(in), stream_id, pstream->status);

		pstream->plock4param->Unlock();
		plock4param->Unlock();
		return -EPARAM;
	}

	pstream->plock4param->Unlock();

	ret = BizDevReqStreamStop(dev_type, dev_ip, stream_id);
	if (ret)
	{
		ERR_PRINT("dev ip: %s, stream_id: %d, BizDevReqStreamStop failed, ret: %d\n",
			inet_ntoa(in), ret, stream_id);

		//发消息
		msg.stream_err.stream_id = stream_id;
		msg.stream_err.stream_errno = ret;

		if (obj && deal_status_cb)
			(obj->*deal_status_cb)(&msg, sizeof(SBizMsg_t));

		plock4param->Unlock();
		return ret;
	}
	
	DBG_PRINT("dev ip: %s, stream_id: %d, BizDevReqStreamStop success\n", inet_ntoa(in), stream_id);

	//发消息
	msg.msg_type = EM_STREAM_MSG_DEL_SUCCESS;
	msg.stream_id = stream_id;

	if (b_thread_running)
	{
		pstream->sem_exit.TimedPend(5);
		
		DBG_PRINT("dev ip: %s, stream_id: %d, req cmd: %d, stream thread exit!!!\n",
			inet_ntoa(in), stream_id, req_cmd);
	}

	//安全了
	map_pstream.erase(stream_id);//移除
	
	delete pstream;//释放
	pstream = NULL;


	if (obj && deal_status_cb)
		(obj->*deal_status_cb)(&msg, sizeof(SBizMsg_t));

	plock4param->Unlock();

	return SUCCESS;
}

//下层传递上来的流关闭，可能是设备掉线，也可能是层的正常关闭
//所以在map find的时候找不到是可能的，因为上层已经erase
int CMediaStreamManager::dealStreamMsgStop(u32 stream_id, s32 stream_errno)
{	
	CMediaStream *pstream = NULL;
	MAP_ID_PSTREAM::iterator map_iter;
	SBizMsg_t msg;
	memset(&msg, 0, sizeof(SBizMsg_t));
	msg.msg_type = EM_STREAM_MSG_ERR;

	plock4param->Lock();
	
	map_iter = map_pstream.find(stream_id);
	if (map_iter == map_pstream.end())
	{
		ERR_PRINT("stream_id(%d) not found in map\n", stream_id);

		plock4param->Unlock();
		return -EPARAM; 
	}
	
	pstream = map_iter->second;
	if (NULL == pstream)
	{
		ERR_PRINT("stream_id(%d) NULL == pstream\n", stream_id);

		plock4param->Unlock();
		return -EPARAM;
	}

	pstream->plock4param->Lock();
	
	//状态回调，上层实现建议用消息传递，
	//否则可能锁死在pstream->plock4param
	msg.stream_err.stream_id = stream_id;
	msg.stream_err.stream_errno = stream_errno;
	
	if (pstream->m_obj && pstream->m_deal_status_cb)
		(pstream->m_obj->*(pstream->m_deal_status_cb))(&msg, sizeof(SBizMsg_t));
	
	pstream->plock4param->Unlock();
	plock4param->Unlock();

	return SUCCESS;
}

int CMediaStreamManager::dealMsg(SBizMsg_t *pmsg, u32 msg_len)
{
	int ret = SUCCESS;

	if (!b_inited)
	{
		ERR_PRINT("module not init\n");
		return -FAILURE;
	}

	if (NULL == pmsg)
	{
		ERR_PRINT("NULL == pmsg\n");
		
		return -EPARAM;
	}

	DBG_PRINT("msg type: %d\n", pmsg->msg_type);

	if (sizeof(SBizMsg_t) != msg_len)
	{
		ERR_PRINT("sizeof(SBizMsg_t)(%d) != msg_len(%d)\n",
			sizeof(SBizMsg_t), msg_len);
		
		return -EPARAM;
	}
	
	s32 msg_type = pmsg->msg_type;
	
	switch (msg_type)
	{
		//消息
	#if 0 //阻塞调用，返回值便知成功与否
		case EM_STREAM_MSG_CONNECT_SUCCESS:	//连接成功
		{
		} break;
		
		case EM_STREAM_MSG_CONNECT_FALIURE:	//连接失败
		{
		} break;
	#endif
		case EM_STREAM_MSG_ERR:		//流出错
		{
		} break;
		
		case EM_STREAM_MSG_STOP:			//biz_dev 层上传的流关闭，可能设备掉线
		{
			u32 stream_id = pmsg->stream_err.stream_id;
			s32 stream_errno = pmsg->stream_err.stream_errno;

			ret = dealStreamMsgStop(stream_id, stream_errno);
			
		} break;
		
		case EM_STREAM_MSG_PROGRESS:		//文件回放/下载进度
		{
		} break;
		
		case EM_STREAM_MSG_FINISH:		//文件下载完成
		{
		} break;
		
		
		//CMediaStreamManager 内部命令
		case EM_STREAM_CMD_CONNECT:	//连接流
		{
			ret = dealStreamConnect(pmsg->stream_id);
			
		} break;

		case EM_STREAM_CMD_DEL:	//删除流
		{
			ret = dealStreamDel(pmsg->stream_id);
			
		} break;

		default:
			ERR_PRINT("msg_type: %d, not support\n", msg_type);
	}

	return ret;
}

void CMediaStreamManager::threadMsg(uint param)//读消息
{
	VD_BOOL b_process = FALSE;
	int ret = SUCCESS;
	SBizMsg_t msg;
	
	while (1)
	{
		ret = SUCCESS;
		memset(&msg, 0, sizeof(SBizMsg_t));
		b_process = FALSE;
		
		ret = pthread_mutex_lock(&mq_mutex);
		if (ret)
		{
			ERR_PRINT("pthread_mutex_lock failed, err(%d, %s)\n", ret, strerror(ret));
			
			break;
		}
		
		if (mq_msg_count)	//有消息
		{
			ret = pmq_ccbuf->Get((u8 *)&msg, sizeof(SBizMsg_t));
			if (ret)
			{
				ERR_PRINT("msg_queue get msg fail(%d), reset it\n", ret);

				pmq_ccbuf->Reset();
				mq_msg_count = 0;
			}
			else
			{
				mq_msg_count--;
				b_process = TRUE;
			}
		}
		else	//无消息
		{
			ret = pthread_cond_wait(&mq_cond, &mq_mutex);
			if (ret)
			{
				ERR_PRINT("pthread_cond_wait failed, err(%d, %s)\n", ret, strerror(ret));

				pthread_mutex_unlock(&mq_mutex);
				break;
			}
		}
		
		ret = pthread_mutex_unlock(&mq_mutex);
		if (ret)
		{
			ERR_PRINT("pthread_cond_wait failed, err(%d, %s)\n", ret, strerror(ret));
			
			break;
		}
		
		if (b_process)
		{
			dealMsg(&msg, sizeof(SBizMsg_t));
		}
	}

thread_exit:
	
	ERR_PRINT("CMediaStreamManager::threadMsg exit, inconceivable\n");
}


int CMediaStreamManager::ReqStreamStart(
	EM_DEV_TYPE dev_type,
	u32 dev_ip,
	ifly_TCP_Stream_Req *preq,
	CObject *obj,
	PDEAL_FRAME deal_frame_cb,
	PDEAL_STATUS deal_status_cb,
	u32 *pstream_id )
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
		ERR_PRINT("dev ip: %s, cmd: %d, CMediaStream init failed, ret: %d\n", inet_ntoa(in), preq->command, ret);

		return ret;
	}

	pstream->dev_type = dev_type;
	pstream->dev_ip = dev_ip;
	memcpy(&pstream->req, preq, sizeof(ifly_TCP_Stream_Req));
	pstream->m_obj = obj;
	pstream->m_deal_frame_cb = deal_frame_cb;//流注册的帧数据处理函数
	pstream->m_deal_status_cb = deal_status_cb;//流注册的状态处理函数
	
	plock4param->Lock();

	pstream->stream_id = _createStreamID();
	*pstream_id = pstream->stream_id;
	
	//insert map
	if (!map_pstream.insert(std::make_pair(pstream->stream_id, pstream)).second)
	{
		ERR_PRINT("dev ip: %s, cmd: %d, map insert failed\n", inet_ntoa(in), preq->command);

		plock4param->Unlock();
		goto fail;
	}

	plock4param->Unlock();
	
	//write msg to threadmsg, in threadmsg connect
	SBizMsg_t msg;
	memset(&msg, 0, sizeof(SBizMsg_t));
	msg.msg_type = EM_STREAM_CMD_CONNECT;
	msg.stream_id = *pstream_id;

	ret = WriteMsg(&msg, sizeof(SBizMsg_t));
	if (ret)
	{
		ERR_PRINT("dev ip: %s, stream req cmd: %d, WriteMsg failed, ret: %d\n", inet_ntoa(in), preq->command, ret);

		goto fail2;
	}	

	return SUCCESS;

fail2:
	plock4param->Lock();

	map_pstream.erase(*pstream_id);
		
	plock4param->Unlock();

fail:
	if (pstream)
	{
		delete pstream;
		pstream = NULL;
	}

	*pstream_id = INVALID_VALUE;
	
	return -FAILURE;
}

int CMediaStreamManager::ReqStreamStop(u32 stream_id)
{
	int ret = SUCCESS;
	CMediaStream *pstream = NULL;
	MAP_ID_PSTREAM::iterator map_iter;
	struct in_addr in;
	
	if (!b_inited)
	{
		ERR_PRINT("module not init\n");
		return -FAILURE;
	}
	
	plock4param->Lock();
	
	map_iter = map_pstream.find(stream_id);
	if (map_iter == map_pstream.end())
	{
		ERR_PRINT("stream_id(%d) not found in map\n", stream_id);
		
		plock4param->Unlock();
		return -EPARAM; 
	}
	
	pstream = map_iter->second;
	if (NULL == pstream)
	{
		ERR_PRINT("stream_id(%d) NULL == pstream\n", stream_id);

		plock4param->Unlock();
		return -EPARAM;
	}

	pstream->plock4param->Lock();

	in.s_addr = pstream->dev_ip;
	pstream->status = EM_STREAM_STATUS_WAIT_DEL;
	
	//write msg to threadmsg, in threadmsg delete
	SBizMsg_t msg;
	memset(&msg, 0, sizeof(SBizMsg_t));
	msg.msg_type = EM_STREAM_CMD_DEL;
	msg.stream_id = pstream->stream_id;

	ret = WriteMsg(&msg, sizeof(SBizMsg_t));
	if (ret)
	{
		ERR_PRINT("dev ip: %s, msg type: %d, WriteMsg failed, ret: %d\n", inet_ntoa(in), msg.msg_type, ret);

		pstream->plock4param->Unlock();
		plock4param->Unlock();
		return -FAILURE;
	}

	pstream->plock4param->Unlock();
	plock4param->Unlock();
	
	return SUCCESS;
}















//extern API
//流控制API
//pstream_id 返回流ID
int BizStreamReqPlaybackByFile (
	EM_DEV_TYPE dev_type,
	u32 dev_ip,
	char *file_name,
	u32 offset,
	CObject *obj,
	PDEAL_FRAME deal_frame_cb,
	PDEAL_STATUS deal_status_cb,
	u32 *pstream_id )
{
	ifly_TCP_Stream_Req req;

	memset(&req, 0, sizeof(ifly_TCP_Stream_Req));

	if (NULL == file_name || NULL == pstream_id)
	{
		return -EPARAM;
	}

	if (strlen(file_name) >= sizeof(req.FilePlayBack_t.filename))
	{
		ERR_PRINT("file_name len(%d) too long\n", strlen(file_name));
		return -EPARAM;
	}

	//0：预览 1：文件回放 2：时间回放 3：文件下载 4：升级 
	//5 VOIP 6 文件按帧下载 7 时间按帧下载 8 透明通道
	//9 远程格式化硬盘 10 主机端抓图 11 多路按时间回放 12 按时间下载文件
	req.command = 1;
	strcpy(req.FilePlayBack_t.filename, file_name);
	req.FilePlayBack_t.offset = offset;
	
	return g_biz_stream_manager.ReqStreamStart(dev_type, dev_ip, &req, obj, deal_frame_cb, deal_status_cb, pstream_id);
}

//流只能由上层删除
int BizStreamReqStop(u32 stream_id)
{
	return g_biz_stream_manager.ReqStreamStop(stream_id);
}


int BizSendMsg2StreamManager(SBizMsg_t *pmsg, u32 msg_len)
{
	return g_biz_stream_manager.WriteMsg(pmsg, msg_len);
}

