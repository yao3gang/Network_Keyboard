#ifndef __BIZ_TYPES_H__
#define __BIZ_TYPES_H__

#include "types.h"


//图像编码类型
#define  MEDIA_TYPE_H264		(BYTE)98//H.264//可能是109?
#define  MEDIA_TYPE_MP4			(BYTE)97//MPEG-4
#define  MEDIA_TYPE_H261		(BYTE)31//H.261
#define  MEDIA_TYPE_H263		(BYTE)34//H.263
#define  MEDIA_TYPE_MJPEG		(BYTE)26//Motion JPEG
#define  MEDIA_TYPE_MP2			(BYTE)33//MPEG2 video

//语音编码类型
#define	 MEDIA_TYPE_MP3			(BYTE)96//mp3
#define  MEDIA_TYPE_PCMU		(BYTE)0//G.711 ulaw
#define  MEDIA_TYPE_PCMA		(BYTE)8//G.711 Alaw
#define	 MEDIA_TYPE_G7231		(BYTE)4//G.7231
#define	 MEDIA_TYPE_G722		(BYTE)9//G.722
#define	 MEDIA_TYPE_G728		(BYTE)15//G.728
#define	 MEDIA_TYPE_G729		(BYTE)18//G.729
#define	 MEDIA_TYPE_RAWAUDIO	(BYTE)19//raw audio
#define  MEDIA_TYPE_ADPCM		(BYTE)20//adpcm
#define  MEDIA_TYPE_ADPCM_HISI	(BYTE)21//adpcm-hisi
// #endif//MEDIA_TYPE


//业务层消息类型
typedef enum
{
	//保存相应的参数
	//BIZ CONFIG
	EM_BIZCONFIG_TIME_DATE_PARAM,
	EM_BIZCONFIG_NET_PARAM,
	EM_BIZCONFIG_TVWALL_LIST,
	EM_BIZCONFIG_NVR_LIST,
	EM_BIZCONFIG_PATROL_DEC_LIST,
	EM_BIZCONFIG_SWITCH_DEC_LIST,
	EM_BIZCONFIG_ALARM_LINK_LIST,
	EM_BIZCONFIG_ALL_FILE,//整个文件
	
	//网络参数改变
	EM_NET_PARAM_CHANGE,
	
	//升级或外部需要暂停的情况
	EM_THREAD_PAUSE,
	EM_THREAD_RESUME,
	EM_MSG_TYPE_MAX,
} EM_MSG_TYPE;



//业务层回调
//param used by SBizEventPara
typedef enum
{
    EM_BIZ_EVENT_UNKNOW = -1,
    EM_BIZ_EVENT_RECORD = 0,  //录像状态及类型
    EM_BIZ_EVENT_ALARM_SENSOR, //传感器报警状态
    EM_BIZ_EVENT_ALARM_VMOTION, //移动侦测状态
    EM_BIZ_EVENT_ALARM_VBLIND,  //遮挡状态
    EM_BIZ_EVENT_ALARM_VLOSS, //视频丢失状态
    EM_BIZ_EVENT_ALARM_OUT, //报警输出状态
    EM_BIZ_EVENT_ALARM_BUZZ,  //蜂鸣器状态
    EM_BIZ_EVENT_ALARM_IPCEXT,//IPC外部传感器报警事件
    EM_BIZ_EVENT_ALARM_IPCCOVER,
    EM_BIZ_EVENT_ALARM_DISK_LOST,
    EM_BIZ_EVENT_ALARM_DISK_ERR,
    EM_BIZ_EVENT_ALARM_DISK_NONE,
    EM_BIZ_EVENT_ALARM_485EXT,
    

    EM_BIZ_EVENT_LOCK = 50,
    EM_BIZ_EVENT_RESTART,		//系统将在nDelay后重起
    EM_BIZ_EVENT_POWEROFF,		//系统将在nDelay后关机
    EM_BIZ_EVENT_POWEROFF_MANUAL,	//提示可以手动关机了

    EM_BIZ_EVENT_TIMETICK = 90,
    EM_BIZ_EVENT_DATETIME_YMD,
	
    EM_BIZ_EVENT_FORMAT_INIT = 100,  //格式化初始化
    EM_BIZ_EVENT_FORMAT_RUN, //格式化中
    EM_BIZ_EVENT_FORMAT_DONE, //格式化结束

    EM_BIZ_EVENT_PLAYBACK_INIT = 150,  //回放初始化
    EM_BIZ_EVENT_PLAYBACK_START, //回放开始
    EM_BIZ_EVENT_PLAYBACK_RUN, //回放中
    EM_BIZ_EVENT_PLAYBACK_DONE, //回放结束
    EM_BIZ_EVENT_PLAYBACK_NETWORK_ERR, //回放时发生网络错误

    EM_BIZ_EVENT_BACKUP_INIT = 200,  //备份初始化
    EM_BIZ_EVENT_BACKUP_RUN, //备份中
    EM_BIZ_EVENT_BACKUP_DONE, //备份结束

    
    EM_BIZ_EVENT_UPGRADE_INIT = 250,  //升级初始化
    EM_BIZ_EVENT_UPGRADE_RUN, //升级中
    EM_BIZ_EVENT_UPGRADE_DONE, //升级结束
    EM_BIZ_EVENT_REMOTEUP_START,//cw_remote
    EM_BIZ_EVENT_GETDMINFO,
    EM_BIZ_EVENT_SATARELOAD,//
    EM_BIZ_EVENT_DISKCHANGED,//

    EM_BIZ_EVENT_PREVIEW_REFRESH = 300,
    EM_BIZ_EVENT_LIVE_REFRESH = 301,
    EM_BIZ_EVENT_SHOWTIME_REFRESH = 302,

	EM_BIZ_EVENT_ENCODE_GETRASTER = 350,

	EM_BIZ_EVENT_NET_STATE_DHCP = 400,
	EM_BIZ_EVENT_NET_STATE_PPPOE,
	EM_BIZ_EVENT_NET_STATE_MAIL,
	EM_BIZ_EVENT_NET_STATE_DDNS,
	EM_BIZ_EVENT_NET_STATE_CONN,
	EM_BIZ_EVENT_NET_STATE_UPDATEMAINBOARDSTART,
	EM_BIZ_EVENT_NET_STATE_UPDATEPANNELSTART,
	EM_BIZ_EVENT_NET_STATE_UPDATESTARTLOGOSTART,
	EM_BIZ_EVENT_NET_STATE_UPDATEAPPLOGOSTART,
	EM_BIZ_EVENT_NET_STATE_FORMATSTART,

	EM_BIZ_EVENT_NET_STATE_DHCP_STOP = 410,
	EM_BIZ_EVENT_NET_STATE_PPPOE_STOP,
	EM_BIZ_EVENT_NET_STATE_MAIL_STOP,
	EM_BIZ_EVENT_NET_STATE_DDNS_STOP,
	EM_BIZ_EVENT_NET_STATE_CONN_STOP,

	EM_BIZ_EVENT_NET_STATE_SGUPLOAD,
	
	EM_BIZ_EVENT_NET_CHANGEPARA_RESET,
	
}EMBIZEVENT;

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


//param used by BizEventCB
typedef struct
{
	EMBIZEVENT type;
	
	union
	{
		u32 stream_id;//关键，系统唯一
		u32 playback_chn;
		u32 preview_chn;
	} un_part_chn;
	
	union 
	{
		s32 stream_errno; //GLB_ERROR_NUM

		//回放、文件下载进度
		struct
		{
			u32 cur_pos;
			u32 total_size;
		} stream_progress;
		
	} un_part_data;   
} SBizEventPara;

//param used by  SBizInitPara.pfnBizEventCb
//typedef void (* FNBIZEVENTCB)(SBizEventPara* sBizEventPara); //业务层事件回调函数
typedef int (* FNBIZEVENTCB)(SBizEventPara* sBizEventPara); //业务层事件回调函数


//param used by BizDataCB
typedef enum
{
    EM_BIZ_DATA_UNKNOW = -1,
		
    EM_BIZ_DATA_PREVIEW = 10,	//预览数据
    EM_BIZ_DATA_PLAYBACK,  		//回放数据
    EM_BIZ_DATA_DOWNLOAD,		//下载
    EM_BIZ_DATA_UPGRADE,		//远程升级数据
    
} EMBIZDATA;


typedef struct
{
	EMBIZDATA type;
	
	union
	{
		u32 stream_id;//关键，系统唯一
		u32 playback_chn;
		u32 preview_chn;
	} un_part_chn;
	
	union
	{
		FRAMEHDR *pframe_hdr;
		
	} un_part_data;   
} SBizDataPara;


#endif

