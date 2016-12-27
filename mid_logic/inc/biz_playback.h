#ifndef __BIZ_PLAYBACK_H__
#define __BIZ_PLAYBACK_H__

#include "types.h"
#include "ctrlprotocol.h"

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

//外部接口
#ifdef __cplusplus
extern "C" {
#endif

//模块初始化
int BizPlaybackInit(void);

int BizPlaybackStartByFile(u32 _dev_ip, ifly_recfileinfo_t *pfile_info);
int BizPlaybackStartByTime(u32 _dev_ip, u8 chn, u32 start_time, u32 end_time);


//是否已经处于进行中
VD_BOOL BizPlaybackIsStarted();

int BizPlaybackStop();


//播放控制
int BizPlaybackCtl();


#ifdef __cplusplus
}
#endif



#endif

