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
#include "biz_remote_stream_define.h"
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

#define g_biz_playback (*CBizPlayback::instance())

class CBizPlayback : public CMediaStream
{	
public:
	PATTERN_SINGLETON_DECLARE(CBizPlayback);
	~CBizPlayback();
	
	int Init(void);
	int PlaybackStartByFile(u32 _dev_ip, ifly_recfileinfo_t *pfile_info);
	int PlaybackStartByTime(u32 _dev_ip, u8 chn, u32 start_time, u32 end_time);
	//预览是否已经处于进行中
	VD_BOOL PlaybackIsStarted();
	int PlaybackStop();
	virtual int dealFrameFunc(FRAMEHDR *pframe_hdr);
	virtual int dealStateFunc(EM_STREAM_STATE_TYPE state, u32 param = 0);//param: 文件下载进度值
	
	
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
	ifly_recfileinfo_t file_info;
	SPlayback_Time_Info_t time_info;
	
};

PATTERN_SINGLETON_IMPLEMENT(CBizPlayback);

CBizPlayback::CBizPlayback()
: b_inited(FALSE)
, plock4param(NULL)
, playback_type(EM_PLAYBACK_NONOE)
{
	memset(&file_info, 0, sizeof(ifly_recfileinfo_t));
	memset(&time_info, 0, sizeof(SPlayback_Time_Info_t));
}

CBizPlayback::~CBizPlayback()
{
	FreeSrc();
}

int CBizPlayback::Init(void)
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

	b_inited = TRUE;
	return SUCCESS;

fail:
	
	FreeSrc();
	return -FAILURE;
}

void CBizPlayback::FreeSrc()
{
	if (plock4param)
	{
		delete plock4param;
		plock4param = NULL;
	}

	b_inited = FALSE;
}

int CBizPlayback::PlaybackStartByFile(u32 _dev_ip, ifly_recfileinfo_t *pfile_info)
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
	if (ret < 0)
	{
		ERR_PRINT("Start failed, ret: %d\n", ret);
		plock4param->Unlock();
		return ret;
	}
	
	plock4param->Unlock();
	
	return SUCCESS;
}

int CBizPlayback::PlaybackStartByTime(u32 _dev_ip, u8 chn, u32 start_time, u32 end_time)
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
VD_BOOL CBizPlayback::PlaybackIsStarted()
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

int CBizPlayback::PlaybackStop()
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

int CBizPlayback::dealFrameFunc(FRAMEHDR *pframe_hdr)
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

//外部获得plock4stream
int CBizPlayback::dealStateFunc(EM_STREAM_STATE_TYPE status, u32 param)//param: 文件下载进度
{
	if (!b_inited)
	{
		ERR_PRINT("module not inited\n");
		return -FAILURE;
	}
	
	DBG_PRINT("status: %d\n", status);

	SBizEventPara para;

	if (EM_STREAM_RCV_ERR == status)
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







/*****************************************************
			外部接口实现
*****************************************************/
int BizPlaybackInit(void)
{
	return g_biz_playback.Init();
}

int BizPlaybackStartByFile(u32 _dev_ip, ifly_recfileinfo_t *pfile_info)
{
	return g_biz_playback.PlaybackStartByFile(_dev_ip, pfile_info);
}

int BizPlaybackStartByTime(u32 _dev_ip, u8 chn, u32 start_time, u32 end_time)
{
	return g_biz_playback.PlaybackStartByTime(_dev_ip, chn, start_time, end_time);
}

//是否已经处于进行中
VD_BOOL BizPlaybackIsStarted()
{
	return g_biz_playback.PlaybackIsStarted();
}

int BizPlaybackStop()
{
	return g_biz_playback.PlaybackStop();
}


//播放控制
int BizPlaybackCtl()
{
	return SUCCESS;
}




