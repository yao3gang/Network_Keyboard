#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>


#include "ctimer.h"
#include "cthread.h"

#include "biz.h"
#include "biz_types.h"
#include "biz_config.h"
#include "biz_net.h"
#include "biz_device.h"
#include "biz_preview.h"
#include "biz_system_complex.h"

#include "ctrlprotocol.h"
#include "net.h"


static VD_BOOL biz_inited = 0;




/*******************************************
bond 层接口声明，用以改变gui 控件
*******************************************/
#include "bond.h"

/*
gui起来之后要设置显示窗口信息

typedef struct hiRECT_S 
{ 
	 HI_S32 s32X; 
	 HI_S32 s32Y; 
	 HI_U32 u32Width; 
	 HI_U32 u32Height; 
}RECT_S;
*/
int BizInit(void)
{
	biz_inited = 0;
	
	g_ThreadManager.RegisterMainThread(ThreadGetID());
	g_TimerManager.Start();
	
	if (BizConfigInit())
	{
		ERR_PRINT("BizConfigInit failed\n");
	}
	
	if (BizNetInit())
	{
		ERR_PRINT("BizNetInit failed\n");
	}
	
	if (BizDeviceInit())
	{
		ERR_PRINT("BizDeviceInit failed\n");
	}

	if (BizPreviewInit())
	{
		ERR_PRINT("BizDeviceInit failed\n");
	}
	/*
	if (BizPreviewStart((u32)4244744384, 1, 1))
	{
		ERR_PRINT("BizPreviewStart failed\n");
	}
	*/
	if (BizSystemComplexInit())
	{
		ERR_PRINT("BizSystemComplexInit failed\n");
	}

	biz_inited = 1;
	
	return SUCCESS;
}

//设置参数
int BizSetNetParam(SConfigNetParam &snet_param)
{
	return BizNetSetNetParam(snet_param);
}


int BizEventCB(SBizEventPara* pSBizEventPara)
{
	return 0;
}

//处理网络客户机命令(回应客户机命令)
u16 BizDealClientCmd(
	CPHandle 	cph,
	u16 		event,
	char*		pbyMsgBuf,
	int 		msgLen,
	char*		pbyAckBuf,
	int*		pAckLen )
{
	DBG_PRINT("client cmd: %d\n", event);

	if(pAckLen)
	{
		*pAckLen = 0;
	}
	
	switch (event)
	{
		case CTRL_CMD_GETDEVICEINFO:
			ifly_DeviceInfo_t dev_info;
					
			if (BizConfigGetDevInfo(&dev_info))
			{
				ERR_PRINT("BizConfigGetDevInfo failed\n");
				return CTRL_FAILED_RESOURCE;
			}
			else
			{
				if(pAckLen)
				{
					*pAckLen = sizeof(ifly_DeviceInfo_t);
				}

				dev_info.devicePort = htons(dev_info.devicePort);
				memcpy(pbyAckBuf, &dev_info, sizeof(ifly_DeviceInfo_t));
			}
			
			break;
		default:
			ERR_PRINT("client cmd: %d nonsupport\n", event);
			return CTRL_FAILED_COMMAND;
			
	}
	
	
	return CTRL_SUCCESS;
}

//处理网络客户机数据连接
int BizDealClientDataLink(
	int sock, 
	ifly_TCP_Stream_Req *preq_hdr, 
	struct sockaddr_in *pcli_addr_in)
{
	
	return SUCCESS;
}

//处理网络服务器事件通知
void BizDealSvrNotify(u32 svr_ip, u16 event, s8 *pbyMsgBuf, int msgLen)
{
	u32 ip_le = ntohl(svr_ip);
	struct in_addr in;
	in.s_addr = svr_ip;
		
	DBG_PRINT("svr IP: %s, event: %d\n", inet_ntoa(in), event);
}



