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

//窗口显示初始化
int BizPreviewDisplayInit(void);

//bmain 是否主码流
int BizPreviewStart(EM_DEV_TYPE _dev_type, u32 _dev_ip, u8 chn, u8 bmain);

int BizPreviewStop();

//biz_device 调用
int BizPreviewNotify(u32 dev_ip, u8 chn, u8 stream_state);

//云台控制
int BizPtzControl(s32 handle, const ifly_PtzCtrl_t *p_para);


#ifdef __cplusplus
}
#endif



#endif

