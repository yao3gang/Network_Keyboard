#ifndef __BIZ_DEVICE_H__
#define __BIZ_DEVICE_H__

#include <map>
#include <vector>

#include <arpa/inet.h>


#include "types.h"
#include "C_Lock.h"
#include "object.h"
#include "csemaphore.h"
#include "cthread.h"
#include "biz_config.h"
#include "biz_remote_stream_define.h"
#include "gui_dev.h"


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
	, deviceIP(0)
	{
		
	}

} SBiz_DeviceInfo_t;


#define MaxMediaLinks (5)
#define MaxIdleNum (60)

class CBizDevice : public CObject {
	friend class CBizDeviceManager;
public:
	CBizDevice();
	~CBizDevice();
	VD_BOOL Init(void);
	void CleanSock();

	int GetDeviceInfo(ifly_DeviceInfo_t *pDeviceInfo);	
	//连接、登录服务器
	int DevConnect();
	//断开命令连接
	int DevDisconnect();

	//stream
	//成功返回stearm_rcv[MaxMediaLinks] 下标stream_idx
	int StreamStart(ifly_TCP_Stream_Req *preq, CMediaStream *pstream);
	int StreamStop(int stream_idx);
	void _CleanStream(int stream_idx);

	//关闭所有数据流连接
	int ShutdownStreamAll();
	
	//重连部分数据流连接
	int CheckAndReconnectStream();
	
	//获取所有通道的IPC信息
	int GetChnIPCInfo(ifly_ipc_info_t * pipc_info, s32 size);
	//只支持NVR，得到NVR通道名(osd info)
	int GetChnName(u8 chn, char *pbuf, u32 size);
	
	//设置解码器通道对应的NVR 通道
	int SetChnIpc(u8 dec_chn, u32 nvr_ip, u8 nvr_chn);
	//删除通道IPC
	int DelChnIpc(u8 dec_chn);


	
	
private:
    CBizDevice(CBizDevice &)
	{
		
	}

	void FreeSrc();//释放资源
	int _DevLogin(ifly_loginpara_t *plogin);
	int _DevLogout(ifly_loginpara_t *plogin);
	int _DevSetAlarmUpload(u8 upload_enable);

private:
	C_Lock *plock4param;//mutex
	SBiz_DeviceInfo_t dev_info;
	ifly_loginpara_t login;
	s32 dev_idx; //dev pool index
	int sock_cmd;		//命令sock，登录，接收报警
	VD_BOOL b_alive;	//是否在线
	int cnt_err;		//暂时未用。错误计数，累计2次，重新连接设备
						//_Add2SetFromMap 检测cnt_err ，close dev sock
	//stream					
	C_Lock *plock4stream;//mutex
	VD_BOOL bthread_stream_running;
	VD_BOOL bthread_stream_exit;//外部控制线程退出
	int stream_cnt;
	int idle_cnt;//空闲计数，线程空闲1分钟后退出
	CSemaphore sem_exit;//等待threadStreamRcv退出信号量
	SDev_StearmRcv_t stearm_rcv[MaxMediaLinks]; //数据流结构MaxMediaLinks
	void threadStreamRcv(uint param);
	Threadlet m_threadlet_stream_rcv;
};


//外部接口
#ifdef __cplusplus
extern "C" {
#endif

int	BizDeviceInit();

//设备搜索
int BizDevSearch(std::vector<SBiz_DeviceInfo_t> *pvnvr_list, 
							std::vector<SBiz_DeviceInfo_t> *pvpatrol_dec_list, 
							std::vector<SBiz_DeviceInfo_t> *pvswitch_dec_list);

int BizAddDev(EM_DEV_TYPE dev_type, u32 dev_ip);
int BizDelDev(EM_DEV_TYPE dev_type, u32 dev_ip);
int BizGetDevIPList(EM_DEV_TYPE dev_type, std::list<u32> &dev_ip_list);//网络字节序
int BizGetDevIdx(EM_DEV_TYPE dev_type, u32 dev_ip);
int BizGetDevInfo(EM_DEV_TYPE dev_type, u32 dev_ip, SGuiDev *pdev);
//获取所有通道的IPC信息
int BizGetDevChnIPCInfo(EM_DEV_TYPE dev_type, u32 dev_ip, ifly_ipc_info_t * pipc_info, s32 size);
//只支持NVR，得到NVR通道名(osd info)
int BizGetDevChnName(EM_DEV_TYPE dev_type, u32 dev_ip, u8 chn, char *pbuf, u32 size);
//设置解码器通道对应的NVR 通道
int BizSetDevChnIpc(EM_DEV_TYPE dev_type, u32 dec_ip , u8 dec_chn, u32 nvr_ip, u8 nvr_chn);
//删除通道IPC
int BizDelDevChnIpc(EM_DEV_TYPE dev_type, u32 dec_ip , u8 dec_chn);














int BizStartNotifyDevInfo();//使能通知。设备层将信息通知给上层



int BizReqStreamStart(s32 dev_idx, ifly_TCP_Stream_Req *preq, CMediaStream *pstream);
int BizReqStreamStop(s32 dev_idx, s32 stream_idx);





#ifdef __cplusplus
}
#endif





#endif

