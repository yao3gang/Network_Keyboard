#ifndef __BIZ_REMOTE_STREAM_H__
#define __BIZ_REMOTE_STREAM_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <string.h>

#include "types.h"
#include "ctrlprotocol.h"
#include "biz_types.h"
#include "biz_msg_type.h"

//流状态
typedef enum
{
	EM_STREAM_STATUS_INIT,			//初始状态
	EM_STREAM_STATUS_RUNNING,		//已连接，正在运行
	EM_STREAM_STATUS_WAIT_DEL,		//等待删除，stream_errno 指示是否出错
	EM_STREAM_STATUS_STOP,			//停止，stream_errno 指示是否出错
} EM_STREAM_STATUS_TYPE;


#define MAX_FRAME_SIZE (1 << 20) // 1MB


/************************************************************
媒体数据请求类
做为预览、回放、文件下载的逻辑层结构
************************************************************/
//流注册的帧数据处理函数
typedef int (CObject:: *PDEAL_FRAME)(u32 stream_id, FRAMEHDR *pframe_hdr);

//流注册的状态处理函数
typedef int (CObject:: *PDEAL_STATUS)(SBizMsg_t *pmsg, u32 len);


//外部接口
#ifdef __cplusplus
extern "C" {
#endif

//extern  API
int BizStreamInit(void);

int BizSendMsg2StreamManager(SBizMsg_t *pmsg, u32 msg_len);

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
	u32 *pstream_id );

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
	u32 *pstream_id );


int BizStreamReqStop(u32 stream_id, s32 stop_reason=SUCCESS);

#ifdef __cplusplus
}
#endif


#endif

