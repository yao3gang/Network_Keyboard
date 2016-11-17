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

//stream_type: 0 主码流，1 子码流
//handle: 成功后返回的句柄
int BizPreviewStart(u32 svr_ip, u8 chn, u8 bmain);

int BizPreviewStop();

//biz_device 调用
int BizPreviewNotify(u32 dev_ip, u8 chn, u8 stream_state);

//云台控制
int BizPtzControl(s32 handle, const ifly_PtzCtrl_t *p_para);


#ifdef __cplusplus
}
#endif



#endif

