#ifndef __BIZ_REMOTE_STREAM_H__
#define __BIZ_REMOTE_STREAM_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "string.h"

#include "types.h"
#include "ctrlprotocol.h"
#include "biz_device.h"
#include "biz_msg_type.h"


#define MAX_FRAME_SIZE (1 << 20) // 1MB

//流状态
typedef enum
{
	EM_STREAM_STATUS_DISCONNECT,	//未连接，初始状态
	EM_STREAM_STATUS_RUNNING,		//已连接，正在运行
	EM_STREAM_STATUS_STOP,			//停止，stream_errno 指示是否出错
	EM_STREAM_STATUS_WAIT_DEL,		//等待删除
} EM_STREAM_STATUS_TYPE;

#if 0
//流错误码
typedef enum
{
	EM_STREAM_ERR_SEND,				//发送出错
	EM_STREAM_ERR_RECV,				//接收出错
	EM_STREAM_ERR_TIMEOUT,			//流超时
	EM_STREAM_ERR_DEV_OFFLINE,		//设备掉线
} EM_STREAM_ERR_TYPE;
#endif


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

//声明流管理者
class CMediaStreamManager;


/************************************************************
媒体数据请求类
做为预览、回放、文件下载的逻辑层结构
************************************************************/
//流注册的帧数据处理函数
typedef void (CObject:: *PDEAL_FRAME)(u32 stream_id, FRAMEHDR *pframe_hdr);

//流注册的状态处理函数
typedef void (CObject:: *PDEAL_STATUS)(SBizMsg_t *pmsg, u32 len);

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
	VD_BOOL b_thread_running;//接收线程运行标志
	VD_BOOL b_thread_exit;//接收线程退出标志
	s32 sock_stream;
	Threadlet m_threadlet_rcv;
	CSemaphore sem_exit;//等待threadRcv退出信号量
};

#if 0
typedef struct _SDev_StearmRcv_t
{
	VD_BOOL b_connect;
	int	sock_fd;	
	u32 link_id;
	ifly_TCP_Stream_Req req;
	CMediaStream* pstream; //指向具体的流结构，预览、回放、文件备份

	_SDev_StearmRcv_t()
	: b_connect(FALSE)
	, sock_fd(INVALID_SOCKET)
	, link_id(INVALID_VALUE)
	, pstream(NULL)
	{
		memset(&req, 0, sizeof(ifly_TCP_Stream_Req));
	}
} SDev_StearmRcv_t;
#endif


//extern  API
int BizStreamInit(void);

int BizSendMsg2StreamManager(SBizMsg_t *pmsg, u32 msg_len);

//pstream_id 返回流ID
int BizStreamReqPlaybackByFile (
	EM_DEV_TYPE dev_type,
	u32 dev_ip,
	char *file_name,
	u32 offset,
	CObject *obj,
	PDEAL_FRAME deal_frame_cb,
	PDEAL_STATUS deal_status_cb,
	u32 *pstream_id );

int BizStreamReqStop(u32 stream_id);



#endif

