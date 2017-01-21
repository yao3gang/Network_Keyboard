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

#include "atomic.h"
#include "c_lock.h"
#include "crwlock.h"
#include "ccircular_buffer.h"
#include "cthread.h"
#include "ctimer.h"
#include "ctrlprotocol.h"
#include "net.h"

#include "biz.h"
#include "biz_net.h"
#include "biz_msg_type.h"
#include "biz_types.h"
#include "biz_config.h"
#include "biz_remote_stream.h"
#include "biz_device.h"


//声明流管理者
class CMediaStreamManager;


/*******************************************************************
CMediaStream 声明
*******************************************************************/

class CMediaStream : public CObject
{
	friend class CMediaStreamManager;
	
public:
	CMediaStream();
	~CMediaStream();
	
	int Init();
	
private:
    CMediaStream(CMediaStream &)
	{
		
	}
	void FreeSrc();//释放资源
	void threadRcv(uint param);//接收服务器数据

private:
	C_Lock *plock4param;//mutex
	//指向流上层结构
	CObject *m_obj;
	PDEAL_FRAME m_deal_frame_cb;//流注册的帧数据处理函数
	PDEAL_STATUS m_deal_status_cb;//流注册的状态处理函数

	//流内部数据
	EM_DEV_TYPE dev_type;//服务器类型
	u32 dev_ip;//服务器IP
	u32 stream_id;//关键，系统唯一
	s32 stream_errno;//错误码
	EM_STREAM_STATUS_TYPE status;//流状态
	ifly_TCP_Stream_Req req;//流请求数据结构
	
	//流接收
	C_Lock *plock4thread_rcv;//mutex
	VD_BOOL b_thread_running;//接收线程运行标志
	VD_BOOL b_thread_exit;//接收线程退出标志
	s32 sock_stream;
	Threadlet m_threadlet_rcv;
	CSemaphore sem_exit;//等待threadRcv退出信号量
};

/**************************************************************/


/**************************************************************
CMediaStream 定义
创建: 上层BizStreamReqxxxx 调用
销毁: 上层BizStreamReqStop 后
		下层出错上层传递EM_STREAM_MSG_STOP 消息
		自身threadRcv 出错传递EM_STREAM_MSG_STOP 消息
*******************************************************************/
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
, status(EM_STREAM_STATUS_INIT) //流状态

//流接收
, plock4thread_rcv(NULL)
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

	plock4thread_rcv = new CMutex;
	if (NULL == plock4thread_rcv)
	{
		ERR_PRINT("new plock4thread_rcv failed\n");
		goto fail;
	}
	
	if (plock4thread_rcv->Create())//FALSE
	{
		ERR_PRINT("create plock4thread_rcv failed\n");
		goto fail;
	}
	return SUCCESS;
	
fail:

	FreeSrc();
	return -FAILURE;
}

void CMediaStream::FreeSrc()//释放资源
{
	if (plock4thread_rcv)
	{
		delete plock4thread_rcv;
		plock4thread_rcv = NULL;
	}

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
	int ret = SUCCESS;
	int sock_fd = INVALID_SOCKET;
	FRAMEHDR frame_hdr;
	ifly_MediaFRAMEHDR_t hdr;
	char *pframe_data = NULL;
	u8 req_cmd = INVALID_VALUE;
	u32 _stream_id = INVALID_VALUE;
	int inside_err = SUCCESS;//标识线程内部是否出错
	CObject *obj = NULL;
	PDEAL_FRAME deal_frame_cb = NULL;
	//u32 rcv_cnt = 0;
		
	u64 ms64 = 0;
	
	fd_set rset;
	struct timeval timeout;
	struct in_addr in;
	SBizMsg_t msg;

	plock4param->Lock();

	obj = m_obj;
	deal_frame_cb = m_deal_frame_cb;

	sock_fd = sock_stream;
	in.s_addr = dev_ip;
	req_cmd = req.command;
	_stream_id = stream_id;
	
	plock4param->Unlock();

	if (INVALID_SOCKET == sock_fd)
	{
		ERR_PRINT("dev IP: %s, stream_id: %d, req_cmd: %d, INVALID_SOCKET == sock_stream\n",
			inet_ntoa(in), _stream_id, req_cmd);

		inside_err = -EPARAM;
		goto done;
	}
	
	pframe_data = new char[MAX_FRAME_SIZE];
	if (NULL == pframe_data)
	{
		ERR_PRINT("dev IP: %s, stream_id: %d, req_cmd: %d, malloc frame buffer failed\n",
			inet_ntoa(in), _stream_id, req_cmd);

		inside_err = -ESPACE;
		goto done;
	}

	DBG_PRINT("dev IP: %s, stream_id: %d, req_cmd: %d, threadRcv running\n",
			inet_ntoa(in), _stream_id, req_cmd);

	while (1)
	{		
		plock4thread_rcv->Lock();
		
		if (b_thread_exit)
		{
			//此时inside_err 肯定= SUCCESS，否则上次循环已经退出了
			//线程退出时用以标识是外部请求退出还是内部出错退出
			//内部退出则发消息
			ms64 = SystemGetMSCount64();
			DBG_PRINT("exit 1: %llu\n", ms64);
			
			plock4thread_rcv->Unlock();	
			break;
		}
		
		plock4thread_rcv->Unlock();

		FD_ZERO(&rset);
		FD_SET(sock_stream, &rset);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		ret = select(sock_stream+1, &rset, NULL, NULL, &timeout);
		if (ret < 0)
		{
			ERR_PRINT("dev IP: %s, stream_id: %d, req_cmd: %d, select failed, ret: %d, %s\n",
				inet_ntoa(in), _stream_id, req_cmd, ret, strerror(ret));
			
			inside_err = -ERECV;
			break;
		}
		else if (ret == 0)//timeout
		{
			continue;
		}

		if (FD_ISSET(sock_stream, &rset))
		{
			memset(&frame_hdr, 0, sizeof(FRAMEHDR));

			//0：预览 1：文件回放 2：时间回放 3：文件下载 4：升级 
			//5 VOIP 6 文件按帧下载 7 时间按帧下载 8 透明通道
			//9 远程格式化硬盘 10 主机端抓图 11 多路按时间回放 12 按时间下载文件
			
			if (req_cmd < 3)
			{
				ret = looprecv(sock_stream, (char *)&hdr, sizeof(ifly_MediaFRAMEHDR_t));
				if (ret)
				{
					ERR_PRINT("dev IP: %s, stream_id: %d, req_cmd: %d, recv ifly_MediaFRAMEHDR_t failed\n",
						inet_ntoa(in), _stream_id, req_cmd);

					inside_err = -ERECV;
					break;
				}
				
				hdr.m_dwDataSize = ntohl(hdr.m_dwDataSize);
				hdr.m_dwFrameID = ntohl(hdr.m_dwFrameID);
				hdr.m_dwTimeStamp = ntohl(hdr.m_dwTimeStamp);
				hdr.m_nVideoHeight = ntohl(hdr.m_nVideoHeight);
				hdr.m_nVideoWidth = ntohl(hdr.m_nVideoWidth);

				if (hdr.m_dwDataSize > MAX_FRAME_SIZE)
				{
					ERR_PRINT("dev IP: %s, stream_id: %d, req_cmd: %d, recv DataSize(%d) > MAX_FRAME_SIZE(%d) failed\n",
						inet_ntoa(in), _stream_id, req_cmd, hdr.m_dwDataSize, MAX_FRAME_SIZE);

					inside_err = -ERECV;
					break;
				}
				
				ret = looprecv(sock_stream, pframe_data, hdr.m_dwDataSize);
				if (ret)
				{
					ERR_PRINT("dev IP: %s, stream_id: %d, req_cmd: %d, recv one frame failed\n",
						inet_ntoa(in), _stream_id, req_cmd);

					inside_err = -ERECV;
					break;
				}
				
				frame_hdr.m_byMediaType = hdr.m_byMediaType;
				frame_hdr.m_byFrameRate = hdr.m_byFrameRate;
				frame_hdr.m_tVideoParam.m_bKeyFrame = hdr.m_bKeyFrame;
				//printf("m_bKeyFrame: %d\n", hdr.m_bKeyFrame);
				frame_hdr.m_tVideoParam.m_wVideoWidth = hdr.m_nVideoWidth;
				//frame.m_tVideoParam.m_wVideoHeight = hdr.m_nVideoHeight;
				frame_hdr.m_tVideoParam.m_wVideoHeight = hdr.m_nVideoHeight & 0x7fff;			//csp add 20091116
				frame_hdr.m_tVideoParam.m_wBitRate = ((hdr.m_nVideoHeight & 0x8000)?1:0) << 31;	//csp add 20091116
				frame_hdr.m_dwDataSize = hdr.m_dwDataSize;
				frame_hdr.m_dwFrameID = hdr.m_dwFrameID;
				frame_hdr.m_dwTimeStamp = hdr.m_dwTimeStamp;
				frame_hdr.m_pData = (u8 *)pframe_data;
				
#if 0
				if (MEDIA_TYPE_H264 == frame_hdr.m_byMediaType)
				{
					ms64 = SystemGetMSCount64();
					DBG_PRINT("m_dwFrameID: %u, m_dwDataSize: %d, m_dwTimeStamp: %u, ms64: %llu\n", 
						hdr.m_dwFrameID, hdr.m_dwDataSize, hdr.m_dwTimeStamp, ms64);
				}

#endif
#if 0


				if ((MEDIA_TYPE_H264 == frame_hdr.m_byMediaType) && hdr.m_bKeyFrame)
				{
					DBG_PRINT("dev IP: %s, stream_id: %d, req_cmd: %d, recv one frame, m_byMediaType: %d, m_bKeyFrame: %d, m_dwFrameID: %u, m_dwDataSize: %d\n", 
						inet_ntoa(in), _stream_id, req_cmd, hdr.m_byMediaType, hdr.m_bKeyFrame, hdr.m_dwFrameID, hdr.m_dwDataSize);
				}
#endif				
				//帧回调，当前只处理视频
				if ((MEDIA_TYPE_H264 == frame_hdr.m_byMediaType) 
						&& obj && deal_frame_cb)
				{
					(obj->*deal_frame_cb)(_stream_id, &frame_hdr);
				}
				
			}
			else if (req_cmd == 3)	//文件下载
			{
				ret = recv(sock_stream, pframe_data, MAX_FRAME_SIZE, 0);
				if (ret < 0)
				{					
					ERR_PRINT("dev IP: %s, stream_id: %d, req_cmd: %d, recv failed, ret: %d, errno(%d, %s)\n",
						inet_ntoa(in), _stream_id, req_cmd, ret, errno, strerror(errno));					
					
					inside_err = -ERECV;
					break;
				}
				else if (ret == 0)//对方关闭socket，可能下载完成
				{
					ERR_PRINT("dev IP: %s, stream_id: %d, req_cmd: %d, svr shutdown!!!\n",
						inet_ntoa(in), _stream_id, req_cmd);
					
					inside_err = -EPEER;
					break;
				}
				
				//仅仅一个数据包，与帧无关，只是借用frame_hdr 传递数据
				frame_hdr.m_pData = (u8 *)pframe_data;
				frame_hdr.m_dwDataSize = ret;

				//rcv_cnt += ret;
				
			#if 1
				//帧回调
				if (obj && deal_frame_cb)
				{
					(obj->*deal_frame_cb)(_stream_id, &frame_hdr);
				}
			#else
				DBG_PRINT("download file m_dwDataSize: %d, \n", ret);
			#endif
			
			}
			else if (req_cmd == 4)	//升级
			{
				ret = recv(sock_stream, pframe_data, MAX_FRAME_SIZE, 0);
				if (ret <= 0)
				{					
					ERR_PRINT("dev IP: %s, stream_id: %d, req_cmd: %d, recv failed\n",
						inet_ntoa(in), _stream_id, req_cmd);					
					
					inside_err = -ERECV;
					break;
				}
				//仅仅一个数据包，与帧无关，只是借用frame_hdr 传递数据
				frame_hdr.m_pData = (u8 *)pframe_data;
				frame_hdr.m_dwDataSize = ret;

				//帧回调
				if (obj && deal_frame_cb)
				{
					(obj->*deal_frame_cb)(_stream_id, &frame_hdr);
				}
			}
			else
			{
				DBG_PRINT("dev IP: %s, stream_id: %d, req command(%d) not supported\n",
					inet_ntoa(in), _stream_id, req_cmd);
			}
		}		
	}

done:

	if (pframe_data)
	{
		delete[] pframe_data;
		pframe_data = NULL;
	}
	
	//只在模块内部出错退出时发消息
	if (inside_err) 
	{
		memset(&msg, 0, sizeof(SBizMsg_t));
		msg.un_part_chn.stream_id = _stream_id;
	#if 0	
		if (-EPEER == inside_err)
		{
			msg.msg_type = EM_STREAM_MSG_SVR_SHUTDOWM;
		}
		else
		{
			msg.msg_type = EM_STREAM_MSG_STOP;
			msg.un_part_data.stream_errno = inside_err;
		}
	#else
		msg.msg_type = EM_STREAM_MSG_STOP;
		msg.un_part_data.stream_errno = inside_err;// -ERECV or -EREER
	#endif
		ret = BizSendMsg2StreamManager(&msg, sizeof(SBizMsg_t));
		if (ret)
		{
			ERR_PRINT("BizSendMsg2StreamManager failed, ret: %d, stream_id: %d, msg_type: %d\n",
				ret, msg.un_part_chn.stream_id, msg.msg_type);
		}		
	}

	ms64 = SystemGetMSCount64();
	
	DBG_PRINT("dev IP: %s, req_cmd: %d, threadRcv exit\n",
				inet_ntoa(in), req_cmd);

	sem_exit.Post();
	
}




/*******************************************************************
CMediaStreamManager 声明
*******************************************************************/

#define g_biz_stream_manager (*CMediaStreamManager::instance())
#define MAX_STREAM_NO (1<<20)//注意必须是2的次幂

//u32:stream_id
typedef std::map<u32, CMediaStream*> MAP_ID_PSTREAM;


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
	//上层调用的流关闭，可能是正常关闭，也可能是上层出错后关闭
	//之后在内部threadMsg  线程中删除流
	int ReqStreamStop(u32 stream_id, s32 stop_reason=SUCCESS);
	// 写消息to stream manager
	int WriteMsg(SBizMsg_t *pmsg, u32 msg_len);
	
private: //function
	CMediaStreamManager();
	CMediaStreamManager(CMediaStreamManager &)
	{

	}
	void FreeSrc();//释放资源
	int dealStreamConnect(u32 stream_id);
	int dealStreamDel(u32 stream_id, u32 stop_reason=SUCCESS);
	int dealStreamMsgStop(u32 stream_id, s32 stream_errno);
	int dealStreamMsgProgess(u32 stream_id, u32 cur_pos, u32 total_size);
	int dealStreamMsgFinish(u32 stream_id);
	int dealStreamMsgSvrShutdown(u32 stream_id);
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


/*******************************************************************
CMediaStreamManager 声明
*******************************************************************/
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

	DBG_PRINT("stream_id: %u\n", stream_id);
	
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
		
		pthread_mutex_unlock(&mq_mutex);
		return ret;
	} 

	mq_msg_count++;

	ret = pthread_cond_signal(&mq_cond);
	if (ret)
	{
		ERR_PRINT("pthread_cond_signal failed, err(%d, %s)\n", ret, strerror(ret));

		pthread_mutex_unlock(&mq_mutex);
		return ret;
	}

	ret = pthread_mutex_unlock(&mq_mutex);
	if (ret)
	{
		ERR_PRINT("pthread_mutex_unlock failed, err(%d, %s)\n", ret, strerror(ret));
		
		return -FAILURE;
	}
	
	return SUCCESS;
}

int CMediaStreamManager::dealStreamConnect(u32 stream_id)
{
	int ret = SUCCESS;
	s32 sock_stream = INVALID_SOCKET;
	struct in_addr in;
	EM_DEV_TYPE dev_type;//服务器类型
	u32 dev_ip = INADDR_NONE;//服务器IP
	ifly_TCP_Stream_Req req;//流请求数据结构
	CMediaStream *pstream = NULL;
	MAP_ID_PSTREAM::iterator map_iter;
	CObject *obj = NULL;
	PDEAL_STATUS deal_status_cb = NULL;
	SBizMsg_t msg;
	memset(&msg, 0, sizeof(SBizMsg_t));
	
	//DBG_PRINT("manager lock\n");
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

	//DBG_PRINT("lock\n");
	pstream->plock4param->Lock();

	obj = pstream->m_obj;
	deal_status_cb = pstream->m_deal_status_cb;
	
	dev_type = pstream->dev_type;
	dev_ip = pstream->dev_ip;
	req = pstream->req;
	
	in.s_addr = dev_ip;

	if (EM_STREAM_STATUS_INIT != pstream->status)
	{
		ERR_PRINT("dev ip: %s, EM_STREAM_STATUS_INIT != pstream->status(%d)",
			inet_ntoa(in), pstream->status);

		pstream->plock4param->Unlock();
		plock4param->Unlock();
		return -EPARAM;
	}

	pstream->plock4param->Unlock();
	plock4param->Unlock();
	
	ret = BizDevReqStreamStart(dev_type, dev_ip, stream_id, &req, &sock_stream);
	
#if 0 //放入回放层操作	
	if (SUCCESS == ret)
	{
		
		ret = BizDevStreamProgress(dev_type, dev_ip, stream_id, TRUE);//接收回放进度信息
		if (ret)
		{
			BizDevReqStreamStop(dev_type, dev_ip, stream_id, ret);
		}
		
	}
#endif

	plock4param->Lock();
	pstream->plock4param->Lock();
		
	if (ret)//failed
	{				
		ERR_PRINT("dev ip: %s, stream_id: %d, req cmd: %d, BizReqStreamStart failed, ret: %d\n",
			inet_ntoa(in), stream_id, req.command, ret);

		//内部数据
		pstream->status = EM_STREAM_STATUS_STOP;
		if (SUCCESS == pstream->stream_errno)
		{
			pstream->stream_errno = ret;
		}
	#if 0
		//删除
		map_pstream.erase(stream_id);//移除
		
		delete pstream;//释放
		pstream = NULL;
	#endif
	
		//发消息
		msg.msg_type = EM_STREAM_MSG_CONNECT_FALIURE;
		msg.un_part_chn.stream_id = stream_id;
		msg.un_part_data.stream_errno = ret;
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
		pstream->b_thread_exit = FALSE;
		pstream->m_threadlet_rcv.run("BizStream", pstream, (ASYNPROC)&CMediaStream::threadRcv, 0, 0);

		//发消息
		msg.msg_type = EM_STREAM_MSG_CONNECT_SUCCESS;
		msg.un_part_chn.stream_id = stream_id;
	}

	pstream->plock4param->Unlock();
	plock4param->Unlock();
	
	if (obj && deal_status_cb)
	{
		ret = (obj->*deal_status_cb)(&msg, sizeof(SBizMsg_t));
		if (ret)
		{
			ERR_PRINT("deal_status_cb failed, ret: %d, stream_id: %d, msg_type: %d\n",
				ret, stream_id, msg.msg_type);
		}
	}
	
	return ret;
}

int CMediaStreamManager::dealStreamDel(u32 stream_id, u32 stop_reason)
{
	int ret = SUCCESS;
	struct in_addr in;
	EM_DEV_TYPE dev_type = EM_DEV_TYPE_NONE;//服务器类型
	u32 dev_ip = INADDR_NONE;//服务器IP
	CMediaStream *pstream = NULL;
	MAP_ID_PSTREAM::iterator map_iter;
	u8 req_cmd = INVALID_VALUE;

	//DBG_PRINT("manager lock\n");
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
	
	//DBG_PRINT("lock\n");
	pstream->plock4param->Lock();

	dev_type = pstream->dev_type;
	dev_ip = pstream->dev_ip;
	req_cmd = pstream->req.command;

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
	plock4param->Unlock();

	DBG_PRINT("dev ip: %s, stream_id: %d, req cmd: %d\n",
			inet_ntoa(in), stream_id, req_cmd);

	//先关
	ret = BizDevReqStreamStop(dev_type, dev_ip, stream_id, stop_reason);
	if (ret)
	{
		ERR_PRINT("dev ip: %s, stream_id: %d, req cmd: %d, BizDevReqStreamStop failed, ret: %d\n",
			inet_ntoa(in), stream_id, req_cmd, ret);
	}
	
	plock4param->Lock();
	pstream->plock4param->Lock();
	
	DBG_PRINT("wait threadRcv exit, b_thread_running: %d\n", pstream->b_thread_running);
	if (pstream->b_thread_running)
	{
		pstream->plock4thread_rcv->Lock();
		
		pstream->b_thread_exit = TRUE;

		pstream->plock4thread_rcv->Unlock();
		
		ret = pstream->sem_exit.Pend();
		DBG_PRINT("dev ip: %s, stream_id: %d, req cmd: %d, stream thread exit!!!, sem_exit pend ret: %d\n",
			inet_ntoa(in), stream_id, req_cmd, ret);

		if (INVALID_SOCKET != pstream->sock_stream)
		{
			close(pstream->sock_stream);
			pstream->sock_stream = INVALID_SOCKET;
		}
	}

	pstream->plock4param->Unlock();

	//安全了	
	map_pstream.erase(stream_id);//移除
	
	delete pstream;//释放
	pstream = NULL;

	plock4param->Unlock();
	
	DBG_PRINT("dev ip: %s, stream_id: %d, req cmd: %d, BizDevReqStreamStop success\n",
		inet_ntoa(in), stream_id, req_cmd);

	return SUCCESS;
}

//biz_dev 层上传的流关闭
// 1、设备掉线或网络通信出错调用，
// 2、GUI层删除设备调用
//需要在本层继续上报
int CMediaStreamManager::dealStreamMsgStop(u32 stream_id, s32 stream_errno)
{	
	// 1、如果出错关闭，就上报
	// 2、发消息EM_STREAM_CMD_DEL，延后删除流
	int ret = SUCCESS;
	CMediaStream *pstream = NULL;
	MAP_ID_PSTREAM::iterator map_iter;
	
	CObject *obj = NULL;
	PDEAL_STATUS deal_status_cb = NULL;
	s32 stop_reason = SUCCESS;
	SBizMsg_t msg;
	
	//DBG_PRINT("manager lock\n");
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
	//DBG_PRINT("lock\n");
	pstream->plock4param->Lock();

	obj = pstream->m_obj;
	deal_status_cb = pstream->m_deal_status_cb;
	
	if (SUCCESS == pstream->stream_errno
			&& SUCCESS != stream_errno)
	{
		 pstream->stream_errno = stream_errno;
	}

	stop_reason = pstream->stream_errno;
		
	pstream->b_thread_exit = TRUE;
	//pstream->status = EM_STREAM_STATUS_WAIT_DEL;//设为待删除状态
	pstream->status = EM_STREAM_STATUS_STOP;

	pstream->plock4param->Unlock();
	plock4param->Unlock();
#if 0
	//给自己发消息，延后删除流
	memset(&msg, 0, sizeof(SBizMsg_t));
	msg.msg_type = EM_STREAM_CMD_DEL;//删除流
	msg.un_part_chn.stream_id = stream_id;
	msg.un_part_data.stream_errno = stop_reason;
	ret = BizSendMsg2StreamManager(&msg, sizeof(SBizMsg_t));
	if (ret)
	{
		ERR_PRINT("BizSendMsg2StreamManager failed, ret: %d, stream_id: %d, msg_type: %d\n",
			ret, msg.un_part_chn.stream_id, msg.msg_type);
	}
#endif	
	//上报
	//状态回调，上层实现建议用消息传递，
	//否则可能锁死在pstream->plock4param
	memset(&msg, 0, sizeof(SBizMsg_t));
	msg.msg_type = EM_STREAM_MSG_STOP;
	msg.un_part_chn.stream_id = stream_id;
	msg.un_part_data.stream_errno = stop_reason;
	
	if (obj && deal_status_cb)
	{
		ret = (obj->*deal_status_cb)(&msg, sizeof(SBizMsg_t));
		if (ret)
		{
			ERR_PRINT("deal_status_cb failed, ret: %d, stream_id: %d, msg_type: %d\n",
				ret, stream_id, msg.msg_type);
		}
	}	

	return SUCCESS;
}

int CMediaStreamManager::dealStreamMsgProgess(u32 stream_id, u32 cur_pos, u32 total_size)
{
	int ret = SUCCESS;
	struct in_addr in;
	CMediaStream *pstream = NULL;
	MAP_ID_PSTREAM::iterator map_iter;
	EM_DEV_TYPE dev_type;//服务器类型
	u32 dev_ip = INADDR_NONE;//服务器IP
	u8 req_cmd = INVALID_VALUE;
	CObject *obj = NULL;
	PDEAL_STATUS deal_status_cb = NULL;
	SBizMsg_t msg;
	
	//DBG_PRINT("manager lock\n");
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

	//DBG_PRINT("lock\n");
	pstream->plock4param->Lock();

	obj = pstream->m_obj;
	deal_status_cb = pstream->m_deal_status_cb;
		
	dev_type = pstream->dev_type;
	dev_ip = pstream->dev_ip;
	req_cmd = pstream->req.command;
	in.s_addr = dev_ip;

	pstream->plock4param->Unlock();
	
	plock4param->Unlock();

	

	memset(&msg, 0, sizeof(SBizMsg_t));
	msg.msg_type = EM_STREAM_MSG_PROGRESS;
	msg.un_part_chn.stream_id = stream_id;
	msg.un_part_data.stream_progress.cur_pos = cur_pos;
	msg.un_part_data.stream_progress.total_size = total_size;

	if (obj && deal_status_cb)
	{
		ret = (obj->*deal_status_cb)(&msg, sizeof(SBizMsg_t));
		if (ret)
		{
			ERR_PRINT("deal_status_cb failed, ret: %d, stream_id: %d, msg_type: %d\n",
				ret, stream_id, msg.msg_type);
		}
	}
	
#if 0
	DBG_PRINT("dev ip: %s, stream_id: %d, req cmd: %d, cur_pos: %u, total_size: %u\n",
			inet_ntoa(in), stream_id, req_cmd, cur_pos, total_size);
#endif	
	
	return SUCCESS;
}

int CMediaStreamManager::dealStreamMsgFinish(u32 stream_id)
{
	int ret = SUCCESS;
	struct in_addr in;
	CMediaStream *pstream = NULL;
	MAP_ID_PSTREAM::iterator map_iter;
	EM_DEV_TYPE dev_type;//服务器类型
	u32 dev_ip = INADDR_NONE;//服务器IP
	u8 req_cmd = INVALID_VALUE;
	CObject *obj = NULL;
	PDEAL_STATUS deal_status_cb = NULL;
	SBizMsg_t msg;
	
	DBG_PRINT("manager lock\n");
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
	
	DBG_PRINT("lock\n");
	pstream->plock4param->Lock();
	
	obj = pstream->m_obj;
	deal_status_cb = pstream->m_deal_status_cb;
		
	dev_type = pstream->dev_type;
	dev_ip = pstream->dev_ip;
	req_cmd = pstream->req.command;
	in.s_addr = dev_ip;

	pstream->plock4param->Unlock();
	
	plock4param->Unlock();
	
	

	memset(&msg, 0, sizeof(SBizMsg_t));
	msg.msg_type = EM_STREAM_MSG_FINISH;
	msg.un_part_chn.stream_id = stream_id;
	if (obj && deal_status_cb)
	{
		ret = (obj->*deal_status_cb)(&msg, sizeof(SBizMsg_t));
		if (ret)
		{
			ERR_PRINT("deal_status_cb failed, ret: %d, stream_id: %d, msg_type: %d\n",
				ret, stream_id, msg.msg_type);
		}
	}
	
	

	DBG_PRINT("dev ip: %s, stream_id: %d, req cmd: %d\n",
			inet_ntoa(in), stream_id, req_cmd);
	
	return SUCCESS;
}

#if 0
int CMediaStreamManager::dealStreamMsgSvrShutdown(u32 stream_id)
{
	int ret = SUCCESS;
	struct in_addr in;
	CMediaStream *pstream = NULL;
	MAP_ID_PSTREAM::iterator map_iter;
	EM_DEV_TYPE dev_type;//服务器类型
	u32 dev_ip = INADDR_NONE;//服务器IP
	u8 req_cmd = INVALID_VALUE;
	CObject *obj = NULL;
	PDEAL_STATUS deal_status_cb = NULL;
	SBizMsg_t msg;
	
	//DBG_PRINT("manager lock\n");
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
	
	//DBG_PRINT("lock\n");
	pstream->plock4param->Lock();

	//内部数据
	pstream->status = EM_STREAM_STATUS_FAILURE;
	pstream->stream_errno = -EPEER;
	
	//其他
	in.s_addr = pstream->dev_ip;
	req_cmd = pstream->req.command;	

	obj = pstream->m_obj;
	deal_status_cb = pstream->m_deal_status_cb;

	pstream->plock4param->Unlock();
	plock4param->Unlock();

	DBG_PRINT("dev ip: %s, stream_id: %d, req cmd: %d\n",
			inet_ntoa(in), stream_id, req_cmd);

	memset(&msg, 0, sizeof(SBizMsg_t));
	msg.msg_type = EM_STREAM_MSG_SVR_SHUTDOWM;
	msg.un_part_chn.stream_id = stream_id;
	if (obj && deal_status_cb)
	{
		ret = (obj->*deal_status_cb)(&msg, sizeof(SBizMsg_t));
		if (ret)
		{
			ERR_PRINT("deal_status_cb failed, ret: %d, stream_id: %d, msg_type: %d\n",
				ret, stream_id, msg.msg_type);
		}
	}
	
	return SUCCESS;
}
#endif

void CMediaStreamManager::threadMsg(uint param)//读消息
{
	VD_BOOL b_process = FALSE;
	int ret = SUCCESS;
	SBizMsg_t msg;
	
	while (1)
	{
		ret = SUCCESS;
		b_process = FALSE;
		
		ret = pthread_mutex_lock(&mq_mutex);
		if (ret)
		{
			ERR_PRINT("pthread_mutex_lock failed, err(%d, %s)\n", ret, strerror(ret));
			
			break;
		}
		
		if (mq_msg_count)	//有消息
		{
			memset(&msg, 0, sizeof(SBizMsg_t));
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
			ret = SUCCESS;
			s32 msg_type = msg.msg_type;
			DBG_PRINT("msg type: %d\n", msg_type);		
			
			switch (msg_type)
			{
				//消息
			#if 0 //阻塞调用，返回值便知成功与否
				case EM_STREAM_MSG_CONNECT_SUCCESS:	//连接成功
				{
					u32 stream_id = msg.stream_id;

					ret = dealStreamMsgConnectSuccess(stream_id);
				} break;
				
				case EM_STREAM_MSG_CONNECT_FALIURE:	//连接失败
				{
					u32 stream_id = msg.stream_err.stream_id;
					s32 stream_errno = msg.stream_err.stream_errno;

					ret = dealStreamMsgConnectFail(stream_id, stream_errno);
				} break;
			#endif

				//biz_dev 层上传的流关闭
				// 1、设备掉线或网络通信出错调用，
				// 2、上层删除设备调用
				case EM_STREAM_MSG_STOP:
				{
					u32 stream_id = msg.un_part_chn.stream_id;
					s32 stream_errno = msg.un_part_data.stream_errno;

					ret = dealStreamMsgStop(stream_id, stream_errno);
					
				} break;
				
				case EM_STREAM_MSG_PROGRESS:		//文件回放/下载进度，biz_dev 层上传
				{
					ret = dealStreamMsgProgess(msg.un_part_chn.stream_id,
												msg.un_part_data.stream_progress.cur_pos,
												msg.un_part_data.stream_progress.total_size);
				} break;
				
				case EM_STREAM_MSG_FINISH:		//文件下载完成，上层传递下来
				{
					ret = dealStreamMsgFinish(msg.un_part_chn.stream_id);
				} break;
			#if 0
				case EM_STREAM_MSG_SVR_SHUTDOWM:	//连接对端关闭
				{
					ret = dealStreamMsgSvrShutdown(msg.un_part_chn.stream_id);
				} break;
			#endif	
				//CMediaStreamManager 内部命令
				case EM_STREAM_CMD_CONNECT:	//连接流
				{
					ret = dealStreamConnect(msg.un_part_chn.stream_id);
					
				} break;

				//上层调用的流关闭，可能是正常关闭，也可能是上层出错后关闭
				case EM_STREAM_CMD_DEL:	//删除流
				{
					u32 stream_id = msg.un_part_chn.stream_id;
					s32 stream_errno = msg.un_part_data.stream_errno;
					ret = dealStreamDel(stream_id, stream_errno);
					
				} break;

				default:
					ERR_PRINT("msg_type: %d, not support\n", msg_type);
			}
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
	struct in_addr in;
	in.s_addr = dev_ip;

	if (NULL == preq 		
		|| NULL == obj
		|| NULL == deal_frame_cb
		|| NULL == deal_status_cb
		|| NULL == pstream_id)
	{
		ERR_PRINT("param invalid\n");
		
		return -EPARAM;
	}
	//DBG_PRINT("start\n");
	
	if (!b_inited)
	{
		ERR_PRINT("module not init\n");
		return -FAILURE;
	}

	CMediaStream *pstream = NULL;
	pstream = new CMediaStream;
	if (NULL == pstream)
	{
		ERR_PRINT("new CMediaStream failed\n");
		
		return -ESPACE;
	}
	
	ret = pstream->Init();
	if (ret)
	{
		ERR_PRINT("dev ip: %s, cmd: %d, CMediaStream init failed, ret: %d\n", inet_ntoa(in), preq->command, ret);

		goto fail;
	}

	pstream->dev_type = dev_type;
	pstream->dev_ip = dev_ip;
	pstream->req = *preq;
	pstream->m_obj = obj;
	pstream->m_deal_frame_cb = deal_frame_cb;//流注册的帧数据处理函数
	pstream->m_deal_status_cb = deal_status_cb;//流注册的状态处理函数

	//DBG_PRINT("manager lock\n");
	plock4param->Lock();

	pstream->stream_id = _createStreamID();
	*pstream_id = pstream->stream_id;
	
	//insert map
	if (!map_pstream.insert(std::make_pair(pstream->stream_id, pstream)).second)
	{
		ERR_PRINT("dev ip: %s, cmd: %d, map insert failed\n", inet_ntoa(in), preq->command);

		ret = -FAILURE;
		goto fail2;
	}
	
	plock4param->Unlock();
	
	//write msg to threadmsg, in threadmsg connect
	SBizMsg_t msg;
	memset(&msg, 0, sizeof(SBizMsg_t));
	msg.msg_type = EM_STREAM_CMD_CONNECT;
	msg.un_part_chn.stream_id = *pstream_id;

	ret = WriteMsg(&msg, sizeof(SBizMsg_t));
	if (ret)
	{
		ERR_PRINT("dev ip: %s, stream req cmd: %d, WriteMsg failed, ret: %d\n", inet_ntoa(in), preq->command, ret);

		ret = -FAILURE;
		goto fail3;
	}	

	//DBG_PRINT("end\n");
	return SUCCESS;

fail3:
	
	plock4param->Lock();
	map_pstream.erase(*pstream_id);

fail2:

	plock4param->Unlock();

fail:
	
	if (pstream)
	{
		delete pstream;
		pstream = NULL;
	}

	*pstream_id = INVALID_VALUE;
	
	return ret;
}

//上层调用的流关闭，可能是正常关闭，也可能是上层出错后关闭
//之后在内部threadMsg  线程中删除流
int CMediaStreamManager::ReqStreamStop(u32 stream_id, s32 stop_reason)
{
	int ret = SUCCESS;
	struct in_addr in;
	CMediaStream *pstream = NULL;
	MAP_ID_PSTREAM::iterator map_iter;
	SBizMsg_t msg;

	//DBG_PRINT("start\n");
		
	if (!b_inited)
	{
		ERR_PRINT("module not init\n");
		return -FAILURE;
	}

#if 1
	//DBG_PRINT("manager lock\n");
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

	//DBG_PRINT("lock\n");
	pstream->plock4param->Lock();

	//内部数据
	pstream->status = EM_STREAM_STATUS_WAIT_DEL;//设为待删除状态
	if (SUCCESS == pstream->stream_errno
		&& SUCCESS != stop_reason)
	{
		 pstream->stream_errno = stop_reason;
	}

	//其他
	in.s_addr = pstream->dev_ip;

	pstream->plock4param->Unlock();
	plock4param->Unlock();

	DBG_PRINT("dev ip: %s, stream id: %d, req cmd: %d\n",
			inet_ntoa(in), pstream->stream_id, pstream->req.command);

	//write msg to threadmsg, in threadmsg delete
	memset(&msg, 0, sizeof(SBizMsg_t));
	msg.msg_type = EM_STREAM_CMD_DEL;//删除流
	msg.un_part_chn.stream_id = pstream->stream_id;
	msg.un_part_data.stream_errno = stop_reason;
	ret = WriteMsg(&msg, sizeof(SBizMsg_t));
	if (ret)
	{
		ERR_PRINT("WriteMsg failed, ret: %d, stream_id: %d, msg_type: %d\n",
			ret, msg.un_part_chn.stream_id, msg.msg_type);
		
		return -FAILURE;
	}

#else
	//write msg to threadmsg, in threadmsg delete
	SBizMsg_t msg;
	memset(&msg, 0, sizeof(SBizMsg_t));
	msg.msg_type = EM_STREAM_MSG_STOP;
	msg.stream_err.stream_id = stream_id;
	msg.stream_err.stream_errno = stop_reason;

	ret = BizSendMsg2StreamManager(&msg, sizeof(SBizMsg_t));
	if (ret)
	{
		ERR_PRINT("BizSendMsg2StreamManager failed, ret: %d, stream_id: %d, msg_type: %d\n",
			ret, msg.stream_err.stream_id, msg.msg_type);
	}
#endif

	//DBG_PRINT("end\n");

	return ret;
}










//extern API
int BizStreamInit(void)
{
	return g_biz_stream_manager.Init();
}


//流控制API
int BizSendMsg2StreamManager(SBizMsg_t *pmsg, u32 msg_len)
{
	return g_biz_stream_manager.WriteMsg(pmsg, msg_len);
}

//pstream_id 返回流ID
//成功返回并不表示连接成功，只是写入了消息列表，之后在消息线程连接
int BizStreamReqPlaybackByFile (
	EM_DEV_TYPE dev_type,
	int connect_type,/* 0 回放 1 下载*/
	u32 dev_ip,
	char *file_name,
	u32 offset,
	u32 size,
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
		ERR_PRINT("file_name len(%d) >= sizeof(req.FilePlayBack_t.filename)(%d), too long\n",
			strlen(file_name), sizeof(req.FilePlayBack_t.filename));
		
		return -EPARAM;
	}

	//0：预览 1：文件回放 2：时间回放 3：文件下载 4：升级 
	//5 VOIP 6 文件按帧下载 7 时间按帧下载 8 透明通道
	//9 远程格式化硬盘 10 主机端抓图 11 多路按时间回放 12 按时间下载文件
	if (connect_type == 0)
	{
		req.command = 1;
		strcpy(req.FilePlayBack_t.filename, file_name);
		req.FilePlayBack_t.offset = offset;
	}
	else if (connect_type == 1)
	{
		req.command = 3;
		strcpy(req.FileDownLoad_t.filename, file_name);
		req.FileDownLoad_t.offset = offset;
		req.FileDownLoad_t.size = size;
	}
	else
	{
		ERR_PRINT("connect_type invalid, 0: playback, 1: download\n");
		return -EPARAM;
	}
	
	return g_biz_stream_manager.ReqStreamStart(dev_type, dev_ip, &req, obj, deal_frame_cb, deal_status_cb, pstream_id);
}

//pstream_id 返回流ID
//成功返回并不表示连接成功，只是写入了消息列表，之后在消息线程连接
int BizStreamReqPlaybackByTime (
	EM_DEV_TYPE dev_type,
	int connect_type,/* 0 回放 1 下载*/
	u32 dev_ip,
	u8 chn, 
	u32 start_time, 
	u32 end_time,
	CObject *obj,
	PDEAL_FRAME deal_frame_cb,
	PDEAL_STATUS deal_status_cb,
	u32 *pstream_id )
{
	ifly_TCP_Stream_Req req;

	memset(&req, 0, sizeof(ifly_TCP_Stream_Req));

	//0：预览 1：文件回放 2：时间回放 3：文件下载 4：升级 
	//5 VOIP 6 文件按帧下载 7 时间按帧下载 8 透明通道
	//9 远程格式化硬盘 10 主机端抓图 11 多路按时间回放 12 按时间下载文件
	req.command = 2;
	req.TimePlayBack_t.channel = chn;
	req.TimePlayBack_t.type = 0xff;//全部类型
	req.TimePlayBack_t.start_time = start_time;
	req.TimePlayBack_t.end_time = end_time;
	
	return g_biz_stream_manager.ReqStreamStart(dev_type, dev_ip, &req, obj, deal_frame_cb, deal_status_cb, pstream_id);
}

//流只能由上层删除
int BizStreamReqStop(u32 stream_id, s32 stop_reason)
{
	return g_biz_stream_manager.ReqStreamStop(stream_id, stop_reason);
}




