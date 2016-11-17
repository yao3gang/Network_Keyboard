#ifndef __DEVCUSTOM_H__
#define __DEVCUSTOM_H__

//#define DVR_CUSTOM_PATH  "/tmp/data/cfg/DevCustom.ini"
#define DVR_CUSTOM_PATH  "/mnt/NetworkKeyboad/DevCustom.ini"


//配置保存
#define DVR_CONFIG_INFO 			"DevCfg"			
#define DVR_CONFIG_INFO_SAVETYPE	"CfgWay"			//配置保存方式(0: flash, 1: file)
#define DVR_CONFIG_INFO_FORMAT		"CfgFormat"			//配置保存格式(0=Struct;1=Ini;2=XML)
#define DVR_CONFIG_INFO_FLASHDEV	"CfgFlashDev"		//flash设备
#define DVR_CONFIG_INFO_DEFAULTPATH	"CfgPathDefault"	//默认配置文件路径
#define DVR_CONFIG_INFO_FILEPATH	"CfgPath"			//配置文件路径
#define DVR_CONFIG_INFO_ZIP			"CfgZip"			//压缩方式(0: 不压缩, 1: zip压缩)
#define DVR_CONFIG_INFO_OFFSET		"Offset"			//在flash设备中的偏移地址
#define DVR_CONFIG_INFO_LENGTH		"Length"			//配置区域大小限制
//

//DVR属性
#define DVR_PROPERTY				"DevProperty"		
#define DVR_PROPERTY_VERSION		"Version"			//版本号
#define DVR_PROPERTY_MODEL			"Model"				//型号
#define DVR_PROPERTY_PRODUCTNUMBER	"Productnumber"		//产品号
#define DVR_PROPERTY_PREVIEWNUMS	"PreviewNums"		//预览通道数
#define DVR_PROPERTY_SVRCONNECTNUMS "SvrMaxConnectNums"	//作为服务器，tcp 最大连接数
#define DVR_PROPERTY_TVWALLNUMS		"TvWallNums"		//电视墙配置表数量限制
#define DVR_PROPERTY_TVWALLDEVSMAX	"TvwallDevsMax"		//电视墙上设备数量的最大值
#define DVR_PROPERTY_TVWALLLINE		"TvWallLine"		//电视墙每行设备数量限制
#define DVR_PROPERTY_TVWALLCOL		"TvWallCol"			//电视墙每列设备数量限制
#define DVR_PROPERTY_NVRNUMS		"NvrNums"			//连接NVR数量限制
#define DVR_PROPERTY_DECNUMS		"DecNums"			//连接DEC数量限制

//时间参数
#define CONFIG_TIME_PARAM				"TimeDate"		//
#define CONFIG_TIME_PARAM_DATA_FORMAT	"DateFormat"	//
#define CONFIG_TIME_PARAM_TIME_FORMAT	"TimeFormat"	//
#define CONFIG_TIME_PARAM_TIME_SYNC		"TimeSync"		//
#define CONFIG_TIME_PARAM_NTPSVR_IP		"NtpServerAdress"	//

//网络参数
#define CONFIG_NET_PARAM 			"NetWork"
#define CONFIG_NET_PARAM_HOSTIP		"HostIP"
#define CONFIG_NET_PARAM_SUBMASK	"Submask"
#define CONFIG_NET_PARAM_GATEWAY	"GateWayIP"
#define CONFIG_NET_PARAM_DNS1		"DNSIP1"
#define CONFIG_NET_PARAM_DNS2		"DNSIP2"
#define CONFIG_NET_PARAM_SVRPORT	"TCPPort"
#define CONFIG_NET_PARAM_MAC		"Mac"


//电视墙参数
#define CONFIG_TVWALL_PARAM					"TvWall"
#define CONFIG_TVWALL_PARAM_DEVS_PER_LINE	"DevsPerLine"
#define CONFIG_TVWALL_PARAM_DEVS_PER_COL	"DevsPerCol"
#define CONFIG_TVWALL_PARAM_DEVLIST			"DevList"		//设备列表
#define CONFIG_TVWALL_PARAM_NUMS			"Nums"			//当前记录了几份电视墙列表


#define CONFIG_DEVLIST_PARAM_NUMS		"Nums"
#define CONFIG_DEVLIST_PARAM_DEVLIST	"DevList"
//NVR 列表
#define CONFIG_NVRLIST_PARAM			"NvrList"

//轮巡型DEC 列表
#define CONFIG_PATROL_DECLIST_PARAM		"patrolDecList"

//切换型DEC 列表
#define CONFIG_SWITCH_DECLIST_PARAM		"SwitchDecList"



//报警联动列表
#define CONFIG_ALARM_LINK_LIST_PARAM			"AlmLinkList"
#define CONFIG_ALARM_LINK_LIST_PARAM_NUMS		"Nums"
#define CONFIG_ALARM_LINK_LIST_PARAM_DEVLIST	"Item"
enum
{
	EM_CONFIG_ALARM_LINK_LIST_PARAM_NVRIP_U32,
	EM_CONFIG_ALARM_LINK_LIST_PARAM_ALM_TYPE_U32,
	EM_CONFIG_ALARM_LINK_LIST_PARAM_ALM_CHN_U32,
	EM_CONFIG_ALARM_LINK_LIST_PARAM_DECIP_U32,
	EM_CONFIG_ALARM_LINK_LIST_PARAM_DEC_CHN_U32,
	EM_CONFIG_ALARM_LINK_LIST_PARAM_MAX,
};



#endif
