#ifndef __BIZ_REMOTE_STREAM_H__
#define __BIZ_REMOTE_STREAM_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <string.h>

//流状态
typedef enum
{
	EM_STREAM_STATUS_DISCONNECT,	//未连接，初始状态
	EM_STREAM_STATUS_RUNNING,		//已连接，正在运行
	EM_STREAM_STATUS_WAIT_DEL,		//等待删除，stream_errno 指示是否出错
	EM_STREAM_STATUS_STOP,			//停止，本模块内流无此状态
} EM_STREAM_STATUS_TYPE;


#include "types.h"
#include "ctrlprotocol.h"
#include "biz_device.h"
#include "biz_msg_type.h"


#define MAX_FRAME_SIZE (1 << 20) // 1MB




//帧接收头
typedef struct
{
    u8     m_byMediaType; //媒体类型
    u8    *m_pData;       //数据缓冲
	u32    m_dwPreBufSize;//m_pData缓冲前预留了多少空间，用于加
	// RTP option的时候偏移指针一般为12+4+12
	// (FIXED HEADER + Extence option + Extence bit)
    u32    m_dwDataSize;  //m_pData指向的实际缓冲大小
    u8     m_byFrameRate; //发送帧率,用于接收端
	u32    m_dwFrameID;   //帧标识，用于接收端
	u32    m_dwTimeStamp; //时间戳, 用于接收端
    union
    {
        struct{
			u32		m_bKeyFrame;    //频帧类型(I or P)
			u16		m_wVideoWidth;  //视频帧宽
			u16		m_wVideoHeight; //视频帧宽
			u32		m_wBitRate;
		}m_tVideoParam;
        u8    m_byAudioMode;//音频模式
    };
}FRAMEHDR,*PFRAMEHDR;


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
	u32 dev_ip,
	char *file_name,
	u32 offset,
	CObject *obj,
	PDEAL_FRAME deal_frame_cb,
	PDEAL_STATUS deal_status_cb,
	u32 *pstream_id );

//pstream_id 返回流ID
//成功返回并不表示连接成功，只是写入了消息列表，之后在消息线程连接
int BizStreamReqPlaybackByTime (
	EM_DEV_TYPE dev_type,
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

