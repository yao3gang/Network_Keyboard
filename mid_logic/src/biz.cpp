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
#include <signal.h>

#include "ctimer.h"
#include "cthread.h"

#include "biz.h"
#include "biz_types.h"
#include "biz_config.h"
#include "biz_net.h"
#include "biz_device.h"
#include "biz_remote_stream.h"
#include "biz_preview.h"
#include "biz_playback.h"

#include "biz_system_complex.h"
#include "hisi_sys.h"

#include "ctrlprotocol.h"
#include "net.h"

#include "bond.h"



static VD_BOOL b_inited = FALSE;
static C_Lock *plock4param = NULL;//mutex
static VD_BOOL b_preview = FALSE;
static u32 preview_chn_mask = 0;
static VD_BOOL b_playback = FALSE;
static u32 playback_chn_mask = 0;
static VD_BOOL b_upgrade = FALSE;


//internal API ***********************************************************************
int BizManagerInit()
{
	plock4param = new CMutex(MUTEX_RECURSIVE);
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
	if (plock4param)
	{
		delete plock4param;
		plock4param = NULL;
	}

	return -FAILURE;
}





//extern other biz module API ***********************************************************

//查询gui 是否准备好接收通知消息
VD_BOOL BizGuiIsReady()
{
	return notifyIsReady();
}

int _checkPlayback(u32 playback_chn)
{
	plock4param->Lock();

	if (!b_playback)
	{
		ERR_PRINT("b_playback FALSE\n");

		plock4param->Unlock();
		return -ESYS_MODE;
	}
	
	if ( !(playback_chn_mask & (1<<playback_chn)) )
	{
		ERR_PRINT("playback_chn(%d) unused\n", playback_chn);

		plock4param->Unlock();
		return -EPARAM;
	}
	
	plock4param->Unlock();

	return SUCCESS;
}

int BizEventCB(SBizEventPara* pSBizEventPara)
{
	EMBIZEVENT type = pSBizEventPara->type;

	//DBG_PRINT("type: %d\n", type);

	switch (type)
	{
		case EM_BIZ_EVENT_PLAYBACK_START: //回放开始
		{
			u32 playback_chn = pSBizEventPara->un_part_chn.playback_chn;

			s32 ret = _checkPlayback(playback_chn);
			if (ret)
			{
				DBG_PRINT("_checkPlayback(%d) failed, event type: %d, ret: %d\n", playback_chn, type, ret);

				return ret;
			}

			//hisi process
			if (playback_chn < 0x10)//回放
			{
				hisi_chn_start(playback_chn);
			}

			//notify gui
			SPlaybackNotify_t para;
			memset(&para, 0, sizeof(SPlaybackNotify_t));
			
			para.type = EM_BIZ_EVENT_PLAYBACK_START;
			para.playback_chn = playback_chn;

			DBG_PRINT("playback_chn: %u start\n", playback_chn);
		
			notifyPlaybackInfo(&para);
			
		} break;

		case EM_BIZ_EVENT_PLAYBACK_RUN: //回放中，进度信息
		{
			u32 playback_chn = pSBizEventPara->un_part_chn.playback_chn;
			u32 cur_pos = pSBizEventPara->un_part_data.stream_progress.cur_pos;
			u32 total_size = pSBizEventPara->un_part_data.stream_progress.total_size;
			
			s32 ret = _checkPlayback(playback_chn);
			if (ret)
			{
				DBG_PRINT("_checkPlayback(%d) failed, event type: %d, ret: %d\n", playback_chn, type, ret);

				return ret;
			}
			
			//notify gui
			SPlaybackNotify_t para;
			memset(&para, 0, sizeof(SPlaybackNotify_t));
			
			para.type = EM_BIZ_EVENT_PLAYBACK_RUN;
			para.playback_chn = playback_chn;
			para.stream_progress.cur_pos = cur_pos;
			para.stream_progress.total_size = total_size;

		#if 0
			DBG_PRINT("cur_pos: %u, total_size: %u\n", cur_pos, total_size);
		#endif
		
			notifyPlaybackInfo(&para);
			
		} break;
		
		case EM_BIZ_EVENT_PLAYBACK_DONE: //回放结束
		{
			u32 playback_chn = pSBizEventPara->un_part_chn.playback_chn;
			
			plock4param->Lock();

			if (!b_playback)
			{
				ERR_PRINT("b_playback FALSE\n");

				plock4param->Unlock();
				return -ESYS_MODE;
			}
			
			if ( (playback_chn_mask & (1<<playback_chn)) == FALSE)
			{
				ERR_PRINT("playback_chn(%d) unused\n", playback_chn);

				plock4param->Unlock();
				return -EPARAM;
			}
			
			playback_chn_mask &= ~(1<<playback_chn);
			
			plock4param->Unlock();

			//hisi process
			hisi_chn_stop(playback_chn);
			
			//notify gui
			SPlaybackNotify_t para;
			memset(&para, 0, sizeof(SPlaybackNotify_t));

			para.type = EM_BIZ_EVENT_PLAYBACK_DONE;
			para.playback_chn = playback_chn;
			
			notifyPlaybackInfo(&para);			

		} break;
		
		case EM_BIZ_EVENT_PLAYBACK_NETWORK_ERR: //回放时发生网络错误
		{
			u32 playback_chn = pSBizEventPara->un_part_chn.playback_chn;
			u32 stream_errno = pSBizEventPara->un_part_data.stream_errno;
			
			plock4param->Lock();

			if (!b_playback)
			{
				ERR_PRINT("b_playback FALSE\n");

				plock4param->Unlock();
				return -ESYS_MODE;
			}
			
			if ( (playback_chn_mask & (1<<playback_chn)) == FALSE)
			{
				ERR_PRINT("playback_chn(%d) unused\n", playback_chn);

				plock4param->Unlock();
				return -EPARAM;
			}

			playback_chn_mask &= ~(1<<playback_chn);

			plock4param->Unlock();

			//hisi process
			hisi_chn_stop(playback_chn);
			
			//notify gui
			SPlaybackNotify_t para;
			memset(&para, 0, sizeof(SPlaybackNotify_t));

			para.type = EM_BIZ_EVENT_PLAYBACK_NETWORK_ERR;
			para.playback_chn = playback_chn;
			para.stream_errno = stream_errno;
			
			notifyPlaybackInfo(&para);
			
		} break;	

		default:
			ERR_PRINT("type: %d not support\n", type);
			break;
	}
	
	return SUCCESS;
}

int BizDataCB(SBizDataPara* SBizDataPara)
{
	int rtn = 0;
	vdec_stream_s in_stream;
	in_stream.rsv = 0;
	in_stream.pts = SBizDataPara->un_part_data.pframe_hdr->m_dwTimeStamp;
	in_stream.pts *= 1000;
	in_stream.data = SBizDataPara->un_part_data.pframe_hdr->m_pData;
	in_stream.len = SBizDataPara->un_part_data.pframe_hdr->m_dwDataSize;

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


//处理网络客户机命令(回应客户机命令)
u16 BizDealClientCmd(
	CPHandle 	cph,
	u16 		event,
	char*		pbyMsgBuf,
	int 		msgLen,
	char*		pbyAckBuf,
	int*		pAckLen )
{
	DBG_PRINT("client cmd: %d\n", event);

	if(pAckLen)
	{
		*pAckLen = 0;
	}
	
	switch (event)
	{
		case CTRL_CMD_GETDEVICEINFO:
			ifly_DeviceInfo_t dev_info;
					
			if (BizConfigGetDevInfo(&dev_info))
			{
				ERR_PRINT("BizConfigGetDevInfo failed\n");
				return CTRL_FAILED_RESOURCE;
			}
			else
			{
				if(pAckLen)
				{
					*pAckLen = sizeof(ifly_DeviceInfo_t);
				}

				dev_info.devicePort = htons(dev_info.devicePort);
				memcpy(pbyAckBuf, &dev_info, sizeof(ifly_DeviceInfo_t));
			}
			
			break;
		default:
			ERR_PRINT("client cmd: %d nonsupport\n", event);
			return CTRL_FAILED_COMMAND;
			
	}
	
	
	return CTRL_SUCCESS;
}

//处理网络客户机数据连接
int BizDealClientDataLink(
	int sock, 
	ifly_TCP_Stream_Req *preq_hdr, 
	struct sockaddr_in *pcli_addr_in)
{
	
	return SUCCESS;
}

#if 0 //暂时未使用，使用的是BizEventCB
//处理网络服务器事件通知
void BizDealSvrNotify(u32 dev_ip, u16 event, s8 *pbyMsgBuf, int msgLen)
{
	struct in_addr in;
	in.s_addr = dev_ip;
		
	DBG_PRINT("svr IP: %s, event: %d\n", inet_ntoa(in), event);
}
#endif


//extern Gui API ************************************************************************
void term_exit(int signo)
{
	time_t cur;
	cur = time(NULL);
	cur += 3600*8;
	
	DBG_PRINT("!!!!!!recv signal(%d),SIGBUS=%d,SIGPIPE=%d,at %s", signo, SIGBUS, SIGPIPE, ctime(&cur));
	if(signo != 17)//子进程结束//SIGCHLD
	{
		//SIGINT=2;//SIGTSTP=20
		if(signo != SIGPIPE && signo != SIGINT && signo != SIGTSTP && signo != 21 && signo != SIGQUIT && signo != SIGWINCH)
		{
			//sleep(10);
			printf("process quit!!!\n");
			exit(-1);
		}
		else
		{
			//ignore "CTRL+C" "CTRL+Z"
			printf("???\n");
		}
	}
}

int BizFirstInit(void)
{
	b_inited = 0;

	if(signal(SIGPIPE, term_exit) == SIG_ERR)
	{
		DBG_PRINT("Register SIGPIPE handler failed, %s\n", strerror(errno));
	}

	//hisi_3535
	if (HisiSysInit())
    {
        ERR_PRINT("HisiSysInit failed\n");
        return -FAILURE;
    }
	DBG_PRINT("HisiSysInit success\n");
	
	g_ThreadManager.RegisterMainThread(ThreadGetID());
	g_TimerManager.Start();

	if (BizManagerInit())
	{
		ERR_PRINT("BizManagerInit failed\n");
		return -FAILURE;
	}
	DBG_PRINT("BizManagerInit success\n");
	
	if (BizConfigInit())
	{
		ERR_PRINT("BizConfigInit failed\n");
		return -FAILURE;
	}
	DBG_PRINT("BizConfigInit success\n");

	if (BizNetInit())
	{
		ERR_PRINT("BizNetInit failed\n");
		return -FAILURE;
	}
	DBG_PRINT("BizNetInit success\n");
	
	if (BizDeviceFirstInit())
	{
		ERR_PRINT("BizDeviceFirstInit failed\n");
		return -FAILURE;
	}
	DBG_PRINT("BizDeviceFirstInit success\n");
	
	return SUCCESS;
}

int BizSecondInit(void)
{
	if (BizDeviceSecondInit())
	{
		ERR_PRINT("BizDeviceSecondInit failed\n");
		return -FAILURE;
	}
	DBG_PRINT("BizDeviceSecondInit success\n");

	if (BizStreamInit())
	{
		ERR_PRINT("BizStreamInit failed\n");
		return -FAILURE;
	}
	DBG_PRINT("BizStreamInit success\n");
#if 0	
	if (BizPreviewInit())
	{
		ERR_PRINT("BizPreviewInit failed\n");
		return -FAILURE;
	}
	DBG_PRINT("BizPreviewInit success\n");
#endif
	if (BizPlaybackInit())
	{
		ERR_PRINT("BizPlaybackInit failed\n");
		return -FAILURE;
	}
	DBG_PRINT("BizPlaybackInit success\n");
	
	if (BizSystemComplexInit())
	{
		ERR_PRINT("BizSystemComplexInit failed\n");
		return -FAILURE;
	}
	DBG_PRINT("BizSystemComplexInit success\n");

	b_inited = 1;
	
	return SUCCESS;
}

void BizEnterPlayback(void)
{
	DBG_PRINT("\n");
	
	plock4param->Lock();

	if (b_preview)
	{
		ERR_PRINT("b_preview == TRUE\n");
	}

	if (b_upgrade)
	{
		ERR_PRINT("b_preview == TRUE\n");
	}

	if (playback_chn_mask)
	{
		ERR_PRINT("playback_chn_mask(0x%x) != 0\n", playback_chn_mask);
	}

	b_playback = TRUE;	
	
	plock4param->Unlock();
}

void BizLeavePlayback(void)
{
	u32 chn_mask = 0;
	s32 i = 0;

	DBG_PRINT("\n");
	
	plock4param->Lock();
	
	chn_mask = playback_chn_mask;
	
	plock4param->Unlock();
	
	if (chn_mask)
	{
		DBG_PRINT("playback_chn_mask(0x%x) != 0\n", chn_mask);

		for (i=0; i<32; ++i)
		{
			if (chn_mask & (1<<i))
			{
				BizPlaybackStop(i);
			}
		}
	}

	plock4param->Lock();
	
	playback_chn_mask = 0;
	b_playback = FALSE;
	
	plock4param->Unlock();
}

//playback_chn 上层传递
int BizPlaybackStartByFile(u32 playback_chn, u32 dev_ip, ifly_recfileinfo_t *pfile_info)
{
	u32 chn_mask = 0;
	int ret = SUCCESS;

	//DBG_PRINT("start\n");
	plock4param->Lock();

	if (!b_playback)
	{
		ERR_PRINT("b_playback FALSE\n");

		plock4param->Unlock();
		return -FAILURE;
	}

	chn_mask = playback_chn_mask;
	
	plock4param->Unlock();

	if (chn_mask & (1<<playback_chn))//已经启动，先关闭
	{
		
		DBG_PRINT("stop %d first\n", playback_chn);
		
		ret = BizModulePlaybackStop(playback_chn);
		if (ret)
		{
			ERR_PRINT("BizModulePlaybackStop failed, playback_chn: %d, ret: %d\n",
				playback_chn, ret);

			return ret;
		}

		plock4param->Lock();
		
		playback_chn_mask &= ~(1<<playback_chn);
		
		plock4param->Unlock();
	}
	
	ret = BizModulePlaybackStartByFile(playback_chn, dev_ip, pfile_info);
	if (ret)
	{
		ERR_PRINT("BizModulePlaybackStartByFile failed, playback_chn: %d, ret: %d\n",
			playback_chn, ret);

		return ret;
	}

	plock4param->Lock();
	
	playback_chn_mask |= 1<<playback_chn;

	plock4param->Unlock();

	//DBG_PRINT("end\n");
	return SUCCESS;
}

int BizPlaybackStartByTime(u32 playback_chn, u32 _dev_ip, u8 chn, u32 start_time, u32 end_time)
{
	//BizModulePlaybackStartByTime
	return SUCCESS;
}

//是否已经处于进行中
VD_BOOL BizPlaybackIsStarted(u32 playback_chn)
{
	u32 chn_mask = 0;

	plock4param->Lock();

	if (!b_playback)
	{
		ERR_PRINT("b_playback FALSE\n");

		plock4param->Unlock();
		return -FAILURE;
	}

	chn_mask = playback_chn_mask;
	
	plock4param->Unlock();

	return (chn_mask & (1<<playback_chn)) ? TRUE:FALSE;
}

int BizPlaybackStop(u32 playback_chn)
{	
	u32 chn_mask = 0;
	int ret = SUCCESS;

	plock4param->Lock();

	if (!b_playback)
	{
		ERR_PRINT("b_playback FALSE\n");

		plock4param->Unlock();
		return -FAILURE;
	}

	chn_mask = playback_chn_mask;
	
	plock4param->Unlock();

	//hisi process
	hisi_chn_stop(playback_chn);

	if (chn_mask & (1<<playback_chn))
	{
		ret = BizModulePlaybackStop(playback_chn);
		if (ret)
		{
			ERR_PRINT("BizModulePlaybackStop(%d) failed, ret: %d\n",
				playback_chn, ret);

			return ret;
		}
		
		plock4param->Lock();
			
		playback_chn_mask &= ~(1<<playback_chn);

		plock4param->Unlock();
	}
	
	return SUCCESS;
}


//播放控制

int BizPlaybackPause(u32 playback_chn)
{	
	u32 chn_mask = 0;
	int ret = SUCCESS;

	plock4param->Lock();

	if (!b_playback)
	{
		ERR_PRINT("b_playback FALSE\n");

		plock4param->Unlock();
		return -FAILURE;
	}

	chn_mask = playback_chn_mask;
	
	plock4param->Unlock();

	if (chn_mask & (1<<playback_chn))
	{
		ret = BizModulePlaybackPause(playback_chn);
		if (ret)
		{
			ERR_PRINT("BizModulePlayPause(%d) failed, ret: %d\n",
				playback_chn, ret);

			return ret;
		}
	}
	
	return SUCCESS;
}

int BizPlaybackResume(u32 playback_chn)
{	
	u32 chn_mask = 0;
	int ret = SUCCESS;

	plock4param->Lock();

	if (!b_playback)
	{
		ERR_PRINT("b_playback FALSE\n");

		plock4param->Unlock();
		return -FAILURE;
	}

	chn_mask = playback_chn_mask;
	
	plock4param->Unlock();

	if (chn_mask & (1<<playback_chn))
	{
		ret = BizModulePlaybackResume(playback_chn);
		if (ret)
		{
			ERR_PRINT("BizModulePlayResume(%d) failed, ret: %d\n",
				playback_chn, ret);

			return ret;
		}
	}
	
	return SUCCESS;
}

int BizPlaybackStep(u32 playback_chn)//帧进
{	
	u32 chn_mask = 0;
	int ret = SUCCESS;

	plock4param->Lock();

	if (!b_playback)
	{
		ERR_PRINT("b_playback FALSE\n");

		plock4param->Unlock();
		return -FAILURE;
	}

	chn_mask = playback_chn_mask;
	
	plock4param->Unlock();

	if (chn_mask & (1<<playback_chn))
	{
		ret = BizModulePlaybackStep(playback_chn);
		if (ret)
		{
			ERR_PRINT("BizModulePlayStep(%d) failed, ret: %d\n",
				playback_chn, ret);

			return ret;
		}
	}
	
	return SUCCESS;
}

int BizPlaybackRate(u32 playback_chn, s32 rate)//{-8, -4, -2, 1, 2, 4, 8}
{	
	u32 chn_mask = 0;
	int ret = SUCCESS;

	plock4param->Lock();

	if (!b_playback)
	{
		ERR_PRINT("b_playback FALSE\n");

		plock4param->Unlock();
		return -FAILURE;
	}

	chn_mask = playback_chn_mask;
	
	plock4param->Unlock();

	if (chn_mask & (1<<playback_chn))
	{
		ret = BizModulePlaybackRate(playback_chn, rate);
		if (ret)
		{
			ERR_PRINT("BizModulePlayStep(%d) failed, ret: %d\n",
				playback_chn, ret);

			return ret;
		}
	}
	
	return SUCCESS;
}

int BizPlaybackSeek(u32 playback_chn, u32 time)
{
	u32 chn_mask = 0;
	int ret = SUCCESS;

	plock4param->Lock();

	if (!b_playback)
	{
		ERR_PRINT("b_playback FALSE\n");

		plock4param->Unlock();
		return -FAILURE;
	}

	chn_mask = playback_chn_mask;
	
	plock4param->Unlock();

	if (chn_mask & (1<<playback_chn))
	{
		ret = BizModulePlaybackSeek(playback_chn, time);
		if (ret)
		{
			ERR_PRINT("BizModulePlaybackSeek(%d) failed, ret: %d\n",
				playback_chn, ret);

			return ret;
		}
	}
	
	return SUCCESS;
}

//文件下载
int BizDownloadByFile(u32 dev_ip, ifly_recfileinfo_t *pfile_info)
{
	return BizPlaybackStartByFile(0x10, dev_ip, pfile_info);// 0回放 0x10下载
}
int BizDownloadByTime(u32 dev_ip, u8 chn, u32 start_time, u32 end_time)
{
	return BizPlaybackStartByTime(0x10, dev_ip, chn, start_time, end_time);// 0回放 1下载
}

int BizDownloadStop()
{
	return BizPlaybackStop(0x10);
}

int BizDownloadCancel()
{
	return BizPlaybackStop(0x10);
}


