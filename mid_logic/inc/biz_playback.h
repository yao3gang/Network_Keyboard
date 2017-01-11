#ifndef __BIZ_PLAYBACK_H__
#define __BIZ_PLAYBACK_H__

#include "types.h"
#include "ctrlprotocol.h"
#include "biz_msg_type.h"

typedef enum
{
	EM_PLAYBACK_NONOE,
	EM_PLAYBACK_FILE,
	EM_PLAYBACK_TIME,
} EM_PLAYBACK_TYPE;

typedef struct
{
	u8		chn;				//通道数
	u16		type;				//类型
	u32		start_time;			//开始时间
	u32		end_time;			//终止时间
} SPlayback_Time_Info_t;

typedef struct
{
	
} SPlayback_Ctrl_t;


//外部接口
#ifdef __cplusplus
extern "C" {
#endif

//模块初始化
int BizPlaybackInit(void);
int BizSendMsg2PlaybackManager(SBizMsg_t *pmsg, u32 msg_len);


//playback_chn 上层传递
int BizModulePlaybackStartByFile(u32 playback_chn, u32 _dev_ip, ifly_recfileinfo_t *pfile_info);
int BizModulePlaybackStartByTime(u32 playback_chn, u32 _dev_ip, u8 chn, u32 start_time, u32 end_time);


//是否已经处于进行中
VD_BOOL BizModulePlaybackIsStarted(u32 playback_chn);

int BizModulePlaybackStop(u32 playback_chn);

//回放控制
int BizModulePlaybackPause(u32 playback_chn);
int BizModulePlaybackResume(u32 playback_chn);
int BizModulePlaybackStep(u32 playback_chn);//帧进
int BizModulePlaybackRate(u32 playback_chn, s32 rate);//{-8, -4, -2, 1, 2, 4, 8}
int BizModulePlaybackSeek(u32 playback_chn, u32 time);




#ifdef __cplusplus
}
#endif



#endif

