#ifndef __BIZ_H__
#define __BIZ_H__

#include "types.h"
#include "biz_msg_type.h"
#include "biz_types.h"
#include "biz_config.h"

#include "ctrlprotocol.h"


/*******************************************
被上层调用的接口都在下面申明
由bond 模块调用
*******************************************/
#ifdef __cplusplus
extern "C" {
#endif

int BizSendMsg2BizManager(SBizMsg_t *pmsg, u32 msg_len);


//查询gui 是否准备好接收通知消息
VD_BOOL BizGuiIsReady();

int BizEventCB(SBizEventPara* pSBizEventPara);
int BizDataCB(SBizDataPara* SBizDataPara);


//处理网络客户机命令(回应客户机命令)
u16 BizDealClientCmd(
	CPHandle 	cph,
	u16 		event,
	char*		pbyMsgBuf,
	int 		msgLen,
	char*		pbyAckBuf,
	int*		pAckLen );

//处理网络客户机数据连接
int BizDealClientDataLink(
	int sock, 
	ifly_TCP_Stream_Req *preq_hdr, 
	struct sockaddr_in *pcli_addr_in);

#if 0 // 暂时未用
//处理网络服务器事件通知
void BizDealSvrNotify(u32 dev_ip, u16 event, s8 *pbyMsgBuf, int msgLen);
#endif


#ifdef __cplusplus
}
#endif

#endif

