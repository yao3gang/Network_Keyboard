#ifndef __BIZ_REMOTE_STREAM_DEFINE_H__
#define __BIZ_REMOTE_STREAM_DEFINE_H__

#include "string.h"

#include "types.h"
#include "ctrlprotocol.h"
#include "biz_device.h"


#define MAX_FRAME_SIZE (768 << 10)
//流接收头
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

typedef enum
{
	EM_STREAM_CONNECTED,
	EM_STREAM_RCV_ERR,
	EM_STREAM_PROGRESS,	//文件下载进度
	EM_STREAM_END,	 	//文件下载完成
} EM_STREAM_STATE_TYPE;

/*
媒体数据请求类
做为预览、回放、文件下载的基类
*/
class CMediaStream : public CObject
{
public:
	int Start(CMediaStream *pstream);
	int Stop();
	virtual int dealFrameFunc(FRAMEHDR *pframe_hdr)
	{
		return TRUE;
	}
	virtual int dealStateFunc(EM_STREAM_STATE_TYPE state, int param = 0)//param: 文件下载进度值
	{
		return TRUE;
	}

	CMediaStream()
	: bstart(0)
	, dev_ip(0)
	, dev_idx(-1)
	, stream_idx(-1)
	{
		memset(&req, 0, sizeof(ifly_TCP_Stream_Req));
	}

	~CMediaStream()
	{
	}
	
protected:
	u8	bstart;
	u32 dev_ip;
	s32 dev_idx;
	s32 stream_idx;
	ifly_TCP_Stream_Req req;

private:
    CMediaStream(CMediaStream &)
	{
		
	}
};

typedef struct _SDev_StearmRcv_t
{
	u8 bstart;
	int	sockfd;	
	u32 linkid;
	ifly_TCP_Stream_Req req;
	CMediaStream* pstream; //指向具体的流结构，预览、回放、文件备份

	_SDev_StearmRcv_t()
	: bstart(0)
	, sockfd(INVALID_SOCKET)
	, linkid(-1)
	, pstream(NULL)
	{
		memset(&req, 0, sizeof(ifly_TCP_Stream_Req));
	}
} SDev_StearmRcv_t;





#endif

