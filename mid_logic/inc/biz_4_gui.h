#ifndef __BIZ_4_GUI_H__
#define __BIZ_4_GUI_H__

#include "types.h"
#include "ctrlprotocol.h"

#include "biz_config.h"


#ifdef __cplusplus
extern "C" {
#endif

int BizFirstInit(void);
int BizSecondInit(void);
//设置参数
int BizSetNetParam(SConfigNetParam &snet_param);

void BizEnterPreview(void);
void BizLeavePreview(void);

void BizEnterPlayback(void);
void BizLeavePlayback(void);

int BizPlaybackStartByFile(u32 playback_chn, u32 dev_ip, ifly_recfileinfo_t *pfile_info);
int BizPlaybackStartByTime(u32 playback_chn, u32 _dev_ip, u8 chn, u32 start_time, u32 end_time);
int BizPlaybackStop(u32 playback_chn);


//回放
int BizPlaybackPause(u32 playback_chn);
int BizPlaybackResume(u32 playback_chn);
int BizPlaybackStep(u32 playback_chn);//帧进
int BizPlaybackRate(u32 playback_chn, s32 rate);//{-8, -4, -2, 1, 2, 4, 8}
int BizPlaybackSeek(u32 playback_chn, u32 time);

//文件下载
int BizDownloadByFile(u32 dev_ip, ifly_recfileinfo_t *pfile_info);
int BizDownloadByTime(u32 _dev_ip, u8 chn, u32 start_time, u32 end_time);
int BizDownloadStop();
int BizDownloadCancel();



#ifdef __cplusplus
}
#endif


#endif

