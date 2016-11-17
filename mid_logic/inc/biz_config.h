#ifndef __BIZ_CONFIG_H__
#define __BIZ_CONFIG_H__

#include<vector>
#include<string>
#include<algorithm>

#include "types.h"
#include "biz_types.h"
#include "devcustom.h"
#include "ctrlprotocol.h"


//获取参数相关
typedef enum
{
	//设备类型参数
	EM_DEV_TYPE_NONE,
	EM_NVR,
	EM_PATROL_DEC,
	EM_SWITCH_DEC,
	EM_DEV_TYPE_MAX,
} EM_DEV_TYPE;


typedef struct
{
	u8 nConfSaveType;			//0:flash 1;file
	u8 nConfFormat; 			//0=Struct;1=Ini;2=XML
	std::string nDevPath;			//flash设备路径
	std::string nDefaultConfPath;	//默认配置文件路径
	std::string nFilePath;			//配置文件路径
	u8 nConfZip;				//0:不压缩 1:压缩
	u32 nFlashOffset;			//保存区域的偏移地址
	u32 nFlashSizeLimit;		//保存区域的大小
} SConfigDevInfo;

typedef struct
{
	std::string nVersion;		//版本号
	std::string nModel;			//型号
	std::string nproductnumber;	//产品号
	u32 nPreview_nums;			//预览通道数
	u32 nsvr_connect_nums;	//作为服务器，tcp 最大连接数
	u32 nTvwall_nums;		//电视墙配置表数量限制
	u32 nTvwall_devs_max;	//电视墙上设备数量的最大值
	u32 nTvwall_devs_per_line;	//电视墙每行设备数量限制
	u32 nTvwall_devs_per_col;	//电视墙每列设备数量限制
	u32 nNvr_nums;			//连接NVR数量限制
	u32 nDec_nums;			//连接DEC数量限制
} SConfigDvrProperty;

//时间配置参数
typedef struct ConfigTimeParam
{
	u8 ndate_format;	//日期格式
	u8 ntime_format;	//时间格式
	u8 btime_sync;		//自动同步
	std::string ntp_svr_ip;	//时间服务器IP
	bool operator==(const ConfigTimeParam &a)
	{
		return ((a.ndate_format == ndate_format) \
			&&(a.ntime_format == ntime_format) \
			&&(a.btime_sync == btime_sync) \
			&&(a.ntp_svr_ip == ntp_svr_ip));
	}
	bool operator!=(const ConfigTimeParam &a)
	{
		return ((a.ndate_format != ndate_format) \
			||(a.ntime_format != ntime_format) \
			||(a.btime_sync != btime_sync) \
			||(a.ntp_svr_ip != ntp_svr_ip));
	}
} SConfigTimeParam;

//网络配置参数
typedef struct ConfigNetParam
{
	u32 nhost_ip;
	u32 nsubmask;
	u32 ngateway;
	u32 ndns1;
	u32 ndns2;
	u32 nsvr_port;
	std::string  mac;
	bool operator==(const ConfigNetParam &a)
	{
		return ((a.nhost_ip == nhost_ip) \
			&&(a.nsubmask == nsubmask) \
			&&(a.ngateway == ngateway) \
			&&(a.ndns1 == ndns1) \
			&&(a.ndns2 == ndns2) \
			&&(a.nsvr_port == nsvr_port) \
			&&(a.mac == mac));
	}
	bool operator!=(const ConfigNetParam &a)
	{
		return ((a.nhost_ip != nhost_ip) \
			||(a.nsubmask != nsubmask) \
			||(a.ngateway != ngateway) \
			||(a.ndns1 != ndns1) \
			||(a.ndns2 != ndns2) \
			||(a.nsvr_port != nsvr_port) \
			||(a.mac != mac));
	}
} SConfigNetParam;

//电视墙配置参数
typedef struct ConfigTvWallParam
{
	u32 ndevs_per_line;	//电视墙行屏幕数	
	u32 ndevs_per_col;	//电视墙列屏幕数
	std::vector<uint> devlist;	//二维电视墙转化为一维设备列表，屏和设备一一对应，空格间隔，0表示该屏未设置解码器
	bool operator==(const ConfigTvWallParam &a)
	{
		return ((a.ndevs_per_line == ndevs_per_line) \
			&&(a.ndevs_per_col == ndevs_per_col) \
			&&(a.devlist == devlist));
	}
	bool operator!=(const ConfigTvWallParam &a)
	{
		return ((a.ndevs_per_line != ndevs_per_line) \
			||(a.ndevs_per_col != ndevs_per_col) \
			||(a.devlist != devlist));
	}
} SConfigTvWallParam;

typedef struct ConfigAlmLinkParam

{
	u32 nvrip;	//触发报警的NVR ip
	u32 alm_type;	//报警源类型(0: sensor, 1: ipc)
	u32 alm_chn;	//报警源所属通道
	u32 decip;		//联动的DEC
	u32 dec_chn;	//DEC 单通道放大
	bool operator==(const ConfigAlmLinkParam &a)
	{
		return ((a.nvrip == nvrip) \
			&&(a.alm_type == alm_type) \
			&&(a.alm_chn == alm_chn) \
			&&(a.decip == decip) \
			&&(a.dec_chn == dec_chn));
	}
	bool operator!=(const ConfigAlmLinkParam &a)
	{
		return ((a.nvrip != nvrip) \
			||(a.alm_type != alm_type) \
			||(a.alm_chn != alm_chn) \
			||(a.decip != decip) \
			||(a.dec_chn != dec_chn));
	}
} SConfigAlmLinkParam;

//所有配置参数集合
typedef struct
{
	SConfigTimeParam stime_param;	//时间配置参数
	SConfigNetParam snet_param;	//网络配置参数
	std::vector<SConfigTvWallParam> vtvwall_list;
	std::vector<uint> vnvr_list;
	std::vector<uint> vpatrol_dec_list; //轮巡型解码器列表
	std::vector<uint> vswitch_dec_list; //切换型解码器列表
	std::vector<SConfigAlmLinkParam> valm_link_list; //报警联动列表
} SConfigAllParam;

#ifdef __cplusplus
extern "C" {
#endif

int BizConfigInit(void);

//获取默认参数
int BizConfigGetDefaultTimeParam(SConfigTimeParam &stimme_param);
int BizConfigGetDefaultNetParam(SConfigNetParam &snet_param);

//获取参数
int BizConfigGetDevInfo(ifly_DeviceInfo_t *pdev_info);
int BizConfigGetDvrProperty(SConfigDvrProperty &sdev_property);
int BizConfigGetTimeParam(SConfigTimeParam &stimme_param);
int BizConfigGetNetParam(SConfigNetParam &snet_param);

int BizConfigGetTvWallList(std::vector<SConfigTvWallParam> &vtvwall_list);
int BizConfigGetNvrList(std::vector<uint> &vdev_list);
int BizConfigGetPatrolDecList(std::vector<uint> &vdev_list);
int BizConfigGetSwitchDecList(std::vector<uint> &vdev_list);
int BizConfigGetAlmLinkList(std::vector<SConfigAlmLinkParam> &valm_link_list);

//设置参数
int BizConfigSetTimeParam(SConfigTimeParam &stimme_param);
int BizConfigSetNetParam(SConfigNetParam &snet_param);


//添加、删除一个或多个
//修改只能是一个
int BizConfigAddTvWallParam(SConfigTvWallParam &tvwall_param);
int BizConfigAddTvWallParamList(std::vector<SConfigTvWallParam> &vtvwall_list);
int BizConfigDelTvWallParam(SConfigTvWallParam &tvwall_param);
int BizConfigDelTvWallParamList(std::vector<SConfigTvWallParam> &vtvwall_list);
int BizConfigDelAllTvWallParamList();
int BizConfigModifyTvWallParam(u32 index, SConfigTvWallParam &stvwall_param);

int BizConfigAddAlmLinkParam(SConfigAlmLinkParam &alm_link_param);
int BizConfigAddAlmLinkParamList(std::vector<SConfigAlmLinkParam> &valm_link_list);
int BizConfigDelAlmLinkParam(SConfigAlmLinkParam &alm_link_param);
int BizConfigDelAlmLinkParamList(std::vector<SConfigAlmLinkParam> &valm_link_list);
int BizConfigDelAllAlmLinkParamList();
int BizConfigModifyAlmLinkParam(u32 index, SConfigAlmLinkParam &salm_link_param);


int BizConfigAddNvr(uint dev_ip);
int BizConfigDelNvr(uint dev_ip);
int BizConfigAddNvrList(std::vector<uint> &vdev_list);//nvr IP list
int BizConfigDelNvrList(std::vector<uint> &vdev_list);
int BizConfigDelAllNvrList();

int BizConfigAddPatrolDec(uint dev_ip);
int BizConfigDelPatrolDec(uint dev_ip);
int BizConfigAddPatrolDecList(std::vector<uint> &vdev_list);
int BizConfigDelPatrolDecList(std::vector<uint> &vdev_list);
int BizConfigDelAllPatrolDecList();

int BizConfigAddSwitchDec(uint dev_ip);
int BizConfigDelSwitchDec(uint dev_ip);
int BizConfigAddSwitchDecList(std::vector<uint> &vdev_list);
int BizConfigDelSwitchDecList(std::vector<uint> &vdev_list);
int BizConfigDelAllSwitchDecList(std::vector<uint> &vdev_list);


#ifdef __cplusplus
}
#endif

#endif
