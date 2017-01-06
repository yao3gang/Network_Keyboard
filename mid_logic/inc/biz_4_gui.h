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
//…Ë÷√≤Œ ˝
int BizSetNetParam(SConfigNetParam &snet_param);

void BizEnterPreview(void);
void BizLeavePreview(void);

void BizEnterPlayback(void);
void BizLeavePlayback(void);

int BizPlaybackStartByFile(u32 playback_chn, u32 dev_ip, ifly_recfileinfo_t *pfile_info);
int BizPlaybackStartByTime(u32 playback_chn, u32 _dev_ip, u8 chn, u32 start_time, u32 end_time);
int BizPlaybackStop(u32 playback_chn);





#ifdef __cplusplus
}
#endif


#endif

