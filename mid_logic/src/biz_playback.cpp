#include "api.h"
#include "singleton.h"
#include "object.h"
#include "glb_error_num.h"
#include "biz.h"
#include "biz_types.h"
#include "biz_config.h"
#include "biz_net.h"
#include "biz_device.h"
#include "biz_playback.h"
#include "biz_msg_type.h"
#include "biz_remote_stream.h"

#include "hisi_sys.h"
#include "atomic.h"
#include "c_lock.h"
#include "crwlock.h"
#include "ccircular_buffer.h"
#include "public.h"
#include "flash.h"
#include "cthread.h"
#include "ctimer.h"
#include "ctrlprotocol.h"
#include "net.h"


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


#include <map>
#include <set>
//#include <bitset>
#include <algorithm>
#include <utility>

//本模块::playback_id 就是来自biz_remote_stream::stream_id

class CBizPlaybackManager；


/*******************************************************************
CBizPlayback 声明
*******************************************************************/
class CBizPlayback : public CObject
{	
	friend class CBizPlaybackManager;
	
public:
	~CBizPlayback();
	
	int Init(void);
	int StartByFile(u32 dev_ip, ifly_recfileinfo_t *pfile_info, u32 *pstream_id);
	int StartByTime(u32 dev_ip, u8 chn, u32 start_time, u32 end_time, u32 *pstream_id);
	//预览是否已经处于进行中
	VD_BOOL IsStarted();
	int Stop();
	int PlaybackCtrl(SPlayback_Ctrl_t *pb_ctl);
	
	void dealFrameFunc(u32 stream_id, FRAMEHDR *pframe_hdr);
	void dealStateFunc(SBizMsg_t *pmsg, u32 len);
	
	
private:
	CBizPlayback();
	CBizPlayback(CBizPlayback &)
	{

	}
	void FreeSrc();//释放资源
	
	
private:
	VD_BOOL b_inited;
	C_Lock *plock4param;//mutex
	int playback_type;
	u32 stream_id;//来自biz_remote_stream
	ifly_recfileinfo_t file_info;
	SPlayback_Time_Info_t time_info;
	
};


/*******************************************************************
CBizPlayback 定义
*******************************************************************/

int CBizPlayback::dealFrameFunc(u32 stream_id, FRAMEHDR *pframe_hdr)
{
	if (!b_inited)
	{
		ERR_PRINT("module not inited\n");
		return -FAILURE;
	}
	
	int rtn = 0;
	vdec_stream_s in_stream;
	in_stream.rsv = 0;
	in_stream.pts = pframe_hdr->m_dwTimeStamp;
	in_stream.pts *= 1000;
	in_stream.data = pframe_hdr->m_pData;
	in_stream.len = pframe_hdr->m_dwDataSize;

#if 0	
	if (pframe_hdr->m_tVideoParam.m_bKeyFrame)
	{
		DBG_PRINT("m_dwFrameID: %d\n", pframe_hdr->m_dwFrameID);
	}
#endif

	rtn = nvr_preview_vdec_write(0, &in_stream);
	if (rtn < 0)
	{
		DBG_PRINT("nvr_preview_vdec_write failed\n");
		return FAILURE;
	}
	
	return SUCCESS;
}

int CBizPlayback::dealStateFunc(SBizMsg_t *pmsg, u32 len)
{
	if (!b_inited)
	{
		ERR_PRINT("module not inited\n");
		return -FAILURE;
	}
	
	DBG_PRINT("status: %d\n", status);

	SBizEventPara para;

	if (EM_STREAM_CONNECTED == status)
	{
		para.emType = EM_BIZ_EVENT_PLAYBACK_START; //回放开始
		para.playback_para.dev_ip = dev_ip;
	}
	else if (EM_STREAM_RCV_ERR == status)
	{
		para.emType = EM_BIZ_EVENT_PLAYBACK_NETWORK_ERR; //回放时发生网络错误
		para.playback_para.dev_ip = dev_ip;
	}
	else if (EM_STREAM_STOP == status)
	{
		para.emType = EM_BIZ_EVENT_PLAYBACK_DONE; //回放结束
		para.playback_para.dev_ip = dev_ip;
	}
	else
	{
		ERR_PRINT("status: %d not support\n", status);
		return -EPARAM;
	}

	BizEventCB(&para);
	
	return SUCCESS;
}

int CBizPlayback::StartByFile(u32 dev_ip, ifly_recfileinfo_t *pfile_info, u32 *pstream_id)
{
	if (!b_inited)
	{
		ERR_PRINT("module not inited\n");
		return -FAILURE;
	}
	
	int ret = SUCCESS;
	
	plock4param->Lock();

	if (b_connect)//已经开启预览
	{
		DBG_PRINT("module has been started, stop before\n");
		
		ret = Stop();
		if (ret)
		{
			ERR_PRINT("Stop failed, ret: %d\n", ret);
			
			plock4param->Unlock();
			return ret;
		}
	}

	playback_type = EM_PLAYBACK_FILE;
	file_info = *pfile_info;

	//base class
	dev_type = EM_NVR;
	dev_ip = _dev_ip;
	memset(&req, 0, sizeof(ifly_TCP_Stream_Req));
	//0：预览 1：文件回放 2：时间回放 3：文件下载 4：升级 
	//5 VOIP 6 文件按帧下载 7 时间按帧下载 8 透明通道
	//9 远程格式化硬盘 10 主机端抓图 11 多路按时间回放 12 按时间下载文件
	req.command = 1;
	strcpy(req.FilePlayBack_t.filename, pfile_info->filename);
	req.FilePlayBack_t.offset = htonl(pfile_info->offset);
	
	ret = Start();
	if (ret)
	{
		ERR_PRINT("Start failed, ret: %d\n", ret);
		plock4param->Unlock();
		return ret;
	}

	//进度
	ret = _StreamProgress(TRUE);//接收进度信息
	if (ret)
	{
		ERR_PRINT("BizDevStreamProgress failed, ret: %d\n", ret);

		if (Stop())
		{
			ERR_PRINT("Stop failed\n");
		}
		
		plock4param->Unlock();
		return ret;
	}
	
	plock4param->Unlock();
	
	return SUCCESS;
}

int CBizPlayback::StartByTime(u32 dev_ip, u8 chn, u32 start_time, u32 end_time, u32 *pstream_id)
{
	if (!b_inited)
	{
		ERR_PRINT("module not inited\n");
		return -FAILURE;
	}
	
	int ret = SUCCESS;
	
	plock4param->Lock();

	if (b_connect)//已经开启预览
	{
		DBG_PRINT("module has been started, stop before\n");
		
		ret = Stop();
		if (ret)
		{
			ERR_PRINT("Stop failed, ret: %d\n", ret);
			
			plock4param->Unlock();
			return ret;
		}
	}
	
	playback_type = EM_PLAYBACK_TIME;
	time_info.chn = chn;
	time_info.type = 0xff;//所有文件类型
	time_info.start_time = start_time;
	time_info.end_time = end_time;

	//base class
	dev_type = EM_NVR;
	dev_ip = _dev_ip;
	memset(&req, 0, sizeof(ifly_TCP_Stream_Req));
	//0：预览 1：文件回放 2：时间回放 3：文件下载 4：升级 
	//5 VOIP 6 文件按帧下载 7 时间按帧下载 8 透明通道
	//9 远程格式化硬盘 10 主机端抓图 11 多路按时间回放 12 按时间下载文件
	req.command = 2;
	req.TimePlayBack_t.channel = time_info.chn;
	req.TimePlayBack_t.type = htons(time_info.type);
	req.TimePlayBack_t.start_time = htonl(time_info.start_time);
	req.TimePlayBack_t.end_time = htonl(time_info.end_time);	
	
	ret = Start();
	if (ret)
	{
		ERR_PRINT("Start failed, ret: %d\n", ret);
		plock4param->Unlock();
		return ret;
	}
	
	plock4param->Unlock();
	
	return SUCCESS;
}


//是否已经处于进行中
VD_BOOL CBizPlayback::IsStarted()
{	
	if (!b_inited)
	{
		ERR_PRINT("module not inited\n");
		return -FAILURE;
	}
	
	plock4param->Lock();

	VD_BOOL b = b_connect;
		
	plock4param->Unlock();

	return b;
}

int CBizPlayback::Stop()
{
	if (!b_inited)
	{
		ERR_PRINT("module not inited\n");
		return -FAILURE;
	}
	
	int ret = SUCCESS;
	
	plock4param->Lock();

	if (b_connect)
	{
		ret = Stop();
		if (ret)
		{
			ERR_PRINT("PlaybackStop failed, ret: %d\n", ret);
			
			plock4param->Unlock();
			return ret;
		}

		b_connect = FALSE;
		dev_type = EM_DEV_TYPE_NONE;
		dev_ip = INADDR_NONE;
		stream_idx = INVALID_VALUE;
		memset(&req, 0, sizeof(ifly_TCP_Stream_Req));
	}	
	
	plock4param->Unlock();
	
	return SUCCESS;
}

int CBizPlayback::PlaybackCtrl(SPlayback_Ctrl_t *pb_ctl)
{
	
}















/*******************************************************************
CBizPlaybackManager 声明
*******************************************************************/

#define g_biz_playback_manager (*CBizPlaybackManager::instance())

//playback_chn, stream_id
typedef std::map<u32, u32> MAP_PBChn_SID;

//u32:stream_id
typedef std::map<u32, CBizPlayback*> MAP_ID_PCPLAYBACK;


class CBizPlaybackManager : public CObject
{	
public:
	PATTERN_SINGLETON_DECLARE(CBizPlaybackManager);
	~CBizPlaybackManager();
	
	int Init(void);
	// 写消息to stream manager
	int WriteMsg(SBizMsg_t *pmsg, u32 msg_len);
	int PlaybackStartByFile(u32 playback_chn, u32 dev_ip, ifly_recfileinfo_t *pfile_info);
	int PlaybackStartByTime(u32 playback_chn, u32 dev_ip, u8 chn, u32 start_time, u32 end_time);
	//预览是否已经处于进行中
	VD_BOOL PlaybackIsStarted(u32 playback_chn);
	int PlaybackStop(u32 playback_chn);
	int PlaybackCtrl(u32 playback_chn, SPlayback_Ctrl_t *pb_ctl);
	
private:
	CBizPlaybackManager();
	CBizPlaybackManager(CBizPlaybackManager &)
	{

	}
	void FreeSrc();//释放资源
	void threadMsg(uint param);//消息线程执行函数
	
private:
	VD_BOOL b_inited;
	C_Lock *plock4param;//mutex
	MAP_PBChn_SID map_pbchn_sid;
	MAP_ID_PCPLAYBACK map_pcplayback;
	u32 cplayback_cnt;
	
	//消息队列数据
	pthread_cond_t mq_cond;		//写flash 的条件变量
	pthread_mutex_t mq_mutex;	//条件变量互斥锁
	u32 mq_msg_count;			//条件，pmsg_queue 中消息数目
	CcircularBuffer *pmq_ccbuf;	//环形缓冲区
	Threadlet mq_msg_threadlet;	//消息线程
	
};

PATTERN_SINGLETON_IMPLEMENT(CBizPlaybackManager);


/*******************************************************************
CBizPlaybackManager 定义
*******************************************************************/
CBizPlaybackManager::CBizPlaybackManager()
: b_inited(FALSE)
, plock4param(NULL)
, cplayback_cnt(0)

//消息队列数据
, mq_msg_count(0)
, pmq_ccbuf(NULL)
{
	
}

CBizPlaybackManager::~CBizPlaybackManager()
{
	FreeSrc();
}

int CBizPlaybackManager::Init(void)
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
	mq_msg_threadlet.run("BizPlaybackManager-mq", this, (ASYNPROC)&CBizPlaybackManager::threadMsg, 0, 0);

	b_inited = TRUE;
	return SUCCESS;
	
fail:

	FreeSrc();
	return -FAILURE;
}

void CBizPlaybackManager::FreeSrc()
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

// 写消息to Playback manager
int CBizPlaybackManager::WriteMsg(SBizMsg_t *pmsg, u32 msg_len)
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

void CBizPlaybackManager::threadMsg(uint param)//读消息
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
			ret = SUCCESS;
			s32 msg_type = msg.msg_type;
			DBG_PRINT("msg type: %d\n", msg_type);		
			
			switch (msg_type)
			{
				//消息
				case EM_STREAM_MSG_CONNECT_SUCCESS:	//连接成功
				{
				} break;
				
				case EM_STREAM_MSG_CONNECT_FALIURE:	//连接失败
				{
				} break;
				case EM_STREAM_MSG_ERR:		//流接收线程上传出错				
				case EM_STREAM_MSG_STOP:	//biz_dev 层上传的流关闭，可能设备掉线
				{
					u32 stream_id = msg.stream_err.stream_id;
					s32 stream_errno = msg.stream_err.stream_errno;

					ret = dealStreamMsgStop(stream_id, stream_errno);
					
				} break;
				
				case EM_STREAM_MSG_PROGRESS:		//文件回放/下载进度
				{
					ret = dealStreamMsgProgess(msg.stream_progress.stream_id,
												msg.stream_progress.cur_pos,
												msg.stream_progress.total_size);
				} break;
				
				case EM_STREAM_MSG_FINISH:		//文件下载完成
				{
					ret = dealStreamMsgFinish(msg.stream_progress.stream_id);
				} break;
				
				
				//CMediaStreamManager 内部命令
				case EM_STREAM_CMD_CONNECT:	//连接流
				{
					ret = dealStreamConnect(msg.stream_id);
					
				} break;

				case EM_STREAM_CMD_DEL:	//删除流
				{
					ret = dealStreamDel(msg.stream_id);
					
				} break;

				default:
					ERR_PRINT("msg_type: %d, not support\n", msg_type);
			}
		}
	}

thread_exit:
	
	ERR_PRINT("CMediaStreamManager::threadMsg exit, inconceivable\n");
}


int CBizPlaybackManager::PlaybackStartByFile(u32 playback_chn, u32 dev_ip, ifly_recfileinfo_t *pfile_info)
{
	int ret = SUCCESS;
	u32 stream_id = INVALID_VALUE;
	
	if (!b_inited)
	{
		ERR_PRINT("module not inited\n");
		return -FAILURE;
	}
		
	plock4param->Lock();

	if (b_connect)//已经开启预览
	{
		DBG_PRINT("module has been started, stop before\n");
		
		ret = Stop();
		if (ret)
		{
			ERR_PRINT("Stop failed, ret: %d\n", ret);
			
			plock4param->Unlock();
			return ret;
		}
	}

	playback_type = EM_PLAYBACK_FILE;
	file_info = *pfile_info;

	//base class
	dev_type = EM_NVR;
	dev_ip = dev_ip;
	memset(&req, 0, sizeof(ifly_TCP_Stream_Req));
	//0：预览 1：文件回放 2：时间回放 3：文件下载 4：升级 
	//5 VOIP 6 文件按帧下载 7 时间按帧下载 8 透明通道
	//9 远程格式化硬盘 10 主机端抓图 11 多路按时间回放 12 按时间下载文件
	req.command = 1;
	strcpy(req.FilePlayBack_t.filename, pfile_info->filename);
	req.FilePlayBack_t.offset = htonl(pfile_info->offset);
	
	ret = Start();
	if (ret)
	{
		ERR_PRINT("Start failed, ret: %d\n", ret);
		plock4param->Unlock();
		return ret;
	}

	//进度
	ret = _StreamProgress(TRUE);//接收进度信息
	if (ret)
	{
		ERR_PRINT("BizDevStreamProgress failed, ret: %d\n", ret);

		if (Stop())
		{
			ERR_PRINT("Stop failed\n");
		}
		
		plock4param->Unlock();
		return ret;
	}
	
	plock4param->Unlock();
	
	return SUCCESS;
}

int CBizPlaybackManager::PlaybackStartByTime(u32 playback_chn, u32 dev_ip, u8 chn, u32 start_time, u32 end_time)
{
	if (!b_inited)
	{
		ERR_PRINT("module not inited\n");
		return -FAILURE;
	}
	
	int ret = SUCCESS;
	
	plock4param->Lock();

	if (b_connect)//已经开启预览
	{
		DBG_PRINT("module has been started, stop before\n");
		
		ret = Stop();
		if (ret)
		{
			ERR_PRINT("Stop failed, ret: %d\n", ret);
			
			plock4param->Unlock();
			return ret;
		}
	}
	
	playback_type = EM_PLAYBACK_TIME;
	time_info.chn = chn;
	time_info.type = 0xff;//所有文件类型
	time_info.start_time = start_time;
	time_info.end_time = end_time;

	//base class
	dev_type = EM_NVR;
	dev_ip = dev_ip;
	memset(&req, 0, sizeof(ifly_TCP_Stream_Req));
	//0：预览 1：文件回放 2：时间回放 3：文件下载 4：升级 
	//5 VOIP 6 文件按帧下载 7 时间按帧下载 8 透明通道
	//9 远程格式化硬盘 10 主机端抓图 11 多路按时间回放 12 按时间下载文件
	req.command = 2;
	req.TimePlayBack_t.channel = time_info.chn;
	req.TimePlayBack_t.type = htons(time_info.type);
	req.TimePlayBack_t.start_time = htonl(time_info.start_time);
	req.TimePlayBack_t.end_time = htonl(time_info.end_time);	
	
	ret = Start();
	if (ret)
	{
		ERR_PRINT("Start failed, ret: %d\n", ret);
		plock4param->Unlock();
		return ret;
	}
	
	plock4param->Unlock();
	
	return SUCCESS;
}


//是否已经处于进行中
VD_BOOL CBizPlaybackManager::PlaybackIsStarted(u32 playback_chn)
{	
	if (!b_inited)
	{
		ERR_PRINT("module not inited\n");
		return -FAILURE;
	}
	
	plock4param->Lock();

	VD_BOOL b = b_connect;
		
	plock4param->Unlock();

	return b;
}

int CBizPlaybackManager::PlaybackStop(u32 playback_chn)
{
	if (!b_inited)
	{
		ERR_PRINT("module not inited\n");
		return -FAILURE;
	}
	
	int ret = SUCCESS;
	
	plock4param->Lock();

	if (b_connect)
	{
		ret = Stop();
		if (ret)
		{
			ERR_PRINT("PlaybackStop failed, ret: %d\n", ret);
			
			plock4param->Unlock();
			return ret;
		}

		b_connect = FALSE;
		dev_type = EM_DEV_TYPE_NONE;
		dev_ip = INADDR_NONE;
		stream_idx = INVALID_VALUE;
		memset(&req, 0, sizeof(ifly_TCP_Stream_Req));
	}	
	
	plock4param->Unlock();
	
	return SUCCESS;
}

int CBizPlaybackManager::PlaybackCtrl(u32 playback_chn, SPlayback_Ctrl_t *pb_ctl)
{
	
}










/*****************************************************
			外部接口实现
*****************************************************/
int BizPlaybackInit(void)
{
	return g_biz_playback_manager.Init();
}

int BizSendMsg2PlaybackManager(SBizMsg_t *pmsg, u32 msg_len)
{
	return g_biz_playback_manager.WriteMsg(pmsg, msg_len);
}


//启动时获取一个playback_id，后面操作都通过此ID
int BizModulePlaybackStartByFile(u32 playback_chn, u32 dev_ip, ifly_recfileinfo_t *pfile_info)
{
	return g_biz_playback_manager.PlaybackStartByFile(playback_chn, dev_ip, pfile_info);
}

int BizModulePlaybackStartByTime(u32 playback_chn, u32 dev_ip, u8 chn, u32 start_time, u32 end_time)
{
	return g_biz_playback_manager.PlaybackStartByTime(playback_chn, dev_ip, chn, start_time, end_time);
}

//是否已经处于进行中
VD_BOOL BizModulePlaybackIsStarted(u32 playback_chn)
{
	return g_biz_playback_manager.PlaybackIsStarted(playback_chn);
}

int BizModulePlaybackStop(u32 playback_chn)
{
	return g_biz_playback_manager.PlaybackStop(playback_chn);
}


//播放控制
int BizModulePlaybackCtrl(u32 playback_chn, SPlayback_Ctrl_t *pb_ctl)
{
	return g_biz_playback_manager.PlaybackCtrl(playback_chn, pb_ctl);
}




