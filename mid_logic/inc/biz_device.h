#ifndef __BIZ_DEVICE_H__
#define __BIZ_DEVICE_H__

#include <map>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "types.h"
#include "C_Lock.h"
#include "object.h"
#include "csemaphore.h"
#include "cthread.h"
#include "gui_dev.h"
#include "ctrlprotocol.h"
#include "glb_error_num.h"

#include "biz_config.h"
#include "biz_msg_type.h"


//设备信息
typedef struct DeviceInfo_t
{
	u8	maxChnNum;		//最大通道数
	u8 	devicetype;		//EM_DEV_TYPE
	u16	devicePort;		//设备端口 
	u32	deviceIP; 		//设备IP  
	bool operator==(const DeviceInfo_t &a) const
	{
		return deviceIP == a.deviceIP;
	}

	bool operator!=(const DeviceInfo_t &a) const
	{
		return deviceIP != a.deviceIP;
	}

	bool operator<(const DeviceInfo_t &a) const
	{
		return ntohl(deviceIP) < ntohl(a.deviceIP);
	}

	DeviceInfo_t& operator=(const DeviceInfo_t &a)
	{
		maxChnNum = a.maxChnNum;
		devicetype = a.devicetype;
		devicePort = a.devicePort;
		deviceIP = a.deviceIP;
		return *this;
	}
	
	DeviceInfo_t()
	: maxChnNum(0)
	, devicetype(0)
	, devicePort(0)
	, deviceIP(INADDR_NONE)
	{
		
	}

	~DeviceInfo_t()
	{
		
	}
} SBiz_DeviceInfo_t;



//外部接口
#ifdef __cplusplus
extern "C" {
#endif

int BizDeviceFirstInit(void);

int BizDeviceSecondInit(void);



//设备搜索
int BizDevSearch(std::vector<SBiz_DeviceInfo_t> *pvnvr_list, 
							std::vector<SBiz_DeviceInfo_t> *pvpatrol_dec_list, 
							std::vector<SBiz_DeviceInfo_t> *pvswitch_dec_list);

int BizSendMsg2DevManager(SBizMsg_t *pmsg, u32 msg_len);


int BizAddDev(EM_DEV_TYPE dev_type, u32 dev_ip);
int BizDelDev(EM_DEV_TYPE dev_type, u32 dev_ip);
int BizGetDevIPList(EM_DEV_TYPE dev_type, std::list<u32> &dev_ip_list);//网络字节序
int BizGetDevIdx(EM_DEV_TYPE dev_type, u32 dev_ip);
int BizGetDevInfo(EM_DEV_TYPE dev_type, u32 dev_ip, SGuiDev *pdev);
//获取所有通道的IPC信息
int BizGetDevChnIPCInfo(EM_DEV_TYPE dev_type, u32 dev_ip, ifly_ipc_info_t * pipc_info, u32 size);
//只支持NVR，得到NVR通道名(osd info)
int BizGetDevChnName(EM_DEV_TYPE dev_type, u32 dev_ip, u8 chn, char *pbuf, u32 size);
//设置解码器通道对应的NVR 通道
int BizSetDevChnIpc(EM_DEV_TYPE dev_type, u32 dec_ip , u8 dec_chn, u32 nvr_ip, u8 nvr_chn);
//删除通道IPC
int BizDelDevChnIpc(EM_DEV_TYPE dev_type, u32 dec_ip , u8 dec_chn);
//获取设备轮巡参数
int BizDevGetPatrolPara(EM_DEV_TYPE dev_type, u32 dec_ip, ifly_patrol_para_t *para, u32 *pbuf_size);
//NVR 录像搜索
int BizDevRecFilesSearch(u32 nvr_ip, ifly_recsearch_param_t *psearch_para, ifly_search_file_result_t *psearch_result);













int BizStartNotifyDevInfo();//使能通知。设备层将信息通知给上层


//成功返回stearm_rcv[MaxMediaLinks] 下标stream_idx
int BizDevReqStreamStart(EM_DEV_TYPE dev_type, u32 dev_ip, u32 stream_id, ifly_TCP_Stream_Req *preq, s32 *psock_stream);
int BizDevReqStreamStop(EM_DEV_TYPE dev_type, u32 dev_ip, u32 stream_id, s32 stop_reason=SUCCESS);//GLB_ERROR_NUM
int BizDevStreamProgress(EM_DEV_TYPE dev_type, u32 dev_ip, u32 stream_id, VD_BOOL b);//接收回放进度信息




#ifdef __cplusplus
}
#endif





#endif

