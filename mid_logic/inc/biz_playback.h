#ifndef __BIZ_PREVIEW_H__
#define __BIZ_PREVIEW_H__

#include "types.h"
#include "ctrlprotocol.h"


//外部接口
#ifdef __cplusplus
extern "C" {
#endif

//模块初始化
int BizPreviewInit(void);

//bmain 是否主码流
int BizPreviewStart(EM_DEV_TYPE _dev_type, u32 _dev_ip, u8 chn, u8 bmain);

//预览是否已经处于进行中
VD_BOOL BizPreviewIsStarted();

int BizPreviewStop();

//biz_device 调用
int BizPreviewNotify(u32 dev_ip, u8 chn, u8 stream_state);

//云台控制
int BizPtzControl(s32 handle, const ifly_PtzCtrl_t *p_para);


#ifdef __cplusplus
}
#endif



#endif

