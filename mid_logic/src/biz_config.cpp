//system & linux
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//C++
#include <algorithm>

//local
#include "api.h"
#include "singleton.h"
#include "object.h"
#include "glb_error_num.h"
#include "biz.h"
#include "biz_types.h"
#include "biz_config.h"
#include "atomic.h"
#include "c_lock.h"
#include "crwlock.h"
#include "ccircular_buffer.h"
#include "ctrlprotocol.h"
#include "public.h"
#include "flash.h"
#include "cthread.h"
#include "net.h"

void ToUpperString(std::string &str)  
{  
    std::transform(str.begin(), str.end(), str.begin(), (int (*)(int))toupper);  
}  
void ToLowerString(std::string &str)  
{  
    std::transform(str.begin(), str.end(), str.begin(), (int (*)(int))tolower);  
}

#define g_biz_config (*CBizConfig::instance())

/*****************************************************
模块内部实现
*****************************************************/

//设置与获取参数配置及其他所有模块相关

//类声明
class CBizConfig : public CObject
{
public:
	PATTERN_SINGLETON_DECLARE(CBizConfig);
	~CBizConfig();
	int Init(void);
	//获取默认参数
	int GetDefaultTimeParam(SConfigTimeParam &stimme_param);
	int GetDefaultNetParam(SConfigNetParam &snet_param);
	//获取参数
	int GetDvrProperty(SConfigDvrProperty &sdev_property);
	int GetTimeParam(SConfigTimeParam &stimme_param);
	int GetNetParam(SConfigNetParam &snet_param);

	int GetTvWallList(std::vector<SConfigTvWallParam> &vtvwall_list);
	int GetDevList(EM_DEV_TYPE dev_type, std::vector<uint> &vdev_list);\
		
	int GetAlmLinkList(std::vector<SConfigAlmLinkParam> &valm_link_list);

	//设置参数
	int SetTimeParam(SConfigTimeParam &stimme_param);
	int SetNetParam(SConfigNetParam &snet_param);

	//添加、删除一个或多个
	//修改只能是一个
	int AddTvWallParamList(std::vector<SConfigTvWallParam> &vtvwall_list);
	int DelTvWallParamList(std::vector<SConfigTvWallParam> &vtvwall_list);
	int DelAllTvWallParamList();
	int ModifyTvWallParam(u32 index, SConfigTvWallParam &stvwall_param);

	int AddAlmLinkParamList(std::vector<SConfigAlmLinkParam> &valm_link_list);
	int DelAlmLinkParamList(std::vector<SConfigAlmLinkParam> &valm_link_list);
	int DelAllAlmLinkParamList();
	int ModifyAlmLinkParam(u32 index, SConfigAlmLinkParam &salm_link_param);

	int AddDevList(EM_DEV_TYPE dev_type, std::vector<uint> &vdev_list);//dev IP list
	int DelDevList(EM_DEV_TYPE dev_type, std::vector<uint> &vdev_list);//dev IP list
	int DelAllDevList(EM_DEV_TYPE dev_type);
	
private:
	CBizConfig();
	CBizConfig(CBizConfig &)
	{

	}
	
	void FreeSrc();//释放资源
	void threadWriteFlash(uint param);//写flash 线程
	int InitConfigInfo();//读取DevCustom.ini : DevCfg
	int InitDevProperty();//读取DevCustom.ini : DevProperty
	int LoadUserCfgFileFromFlash();//从/dev/mtd2 加载到 /tmp/data/cfg/UserCfg.ini
	int UpgradeUserCfgFile2Flash();//写 /tmp/data/cfg/UserCfg.ini 到/dev/mtd2 
	//加载默认配置文件到参数
	int LoadUserCfgParamFromFile(SConfigAllParam *psall_param, const char *file_name);
	//从配置文件加载时间参数
	int GetTimeParamFromFile(SConfigTimeParam &stime_param, const char *file_name);
	int SetTimeParam2File(SConfigTimeParam &stime_param);//外部加锁
	//从配置文件加载网络参数
	int GetNetParamFromFile(SConfigNetParam &snet_param, const char *file_name);
	int SetNetParam2File(SConfigNetParam &snet_param);//外部加锁
	//从配置文件加载电视墙参数
	int GetTvWallParamFromFile(std::vector<SConfigTvWallParam> &vtvwall_list, const char *file_name);
	int SetTvWallParam2File(std::vector<SConfigTvWallParam> &vtvwall_list);//外部加锁
	//从配置文件加载NVR\patrol dec\switch dec 列表
	int GetDevListFromFile(EM_DEV_TYPE dev_type, std::vector<uint> &vdev_list, const char *file_name);
	int SetDevList2File(EM_DEV_TYPE dev_type, std::vector<uint> &vdev_list);//外部加锁
#if 0	
	VD_BOOL GetNvrListFromFile(std::vector<uint> &vnvr_list, const char *file_name);
	//从配置文件加载patrol dec 列表
	VD_BOOL GetPatrolDecListFromFile(std::vector<uint> &vpatrol_dec_list, const char *file_name);
	//从配置文件加载switch dec 列表
	VD_BOOL GetSwitchDecListFromFile(std::vector<uint> &vswitch_dec_list, const char *file_name);
#endif
	//从配置文件加载报警联动列表
	int GetAlmLinkParamListFromFile(std::vector<SConfigAlmLinkParam> &valm_link_list, const char *file_name);
	int SetAlmLinkParamList2File(std::vector<SConfigAlmLinkParam> &valm_link_list);


	//给threadWriteFlash 发消息
	int WriteMsg2Thread(EM_MSG_TYPE msg_type);
	
	
private:
	VD_BOOL b_inited;
	VD_BOOL b_pause;	//因为升级或其他原因需要暂停写flash
	
	//写flash 线程相关数据，线程函数在上面
	Threadlet m_threadlet;
	pthread_cond_t cond;	//写flash 的条件变量
	pthread_mutex_t cond_mutex;	//条件变量互斥锁
	int cond_msg_count;		//条件，pmsg_queue 中消息数目
	CcircularBuffer *pmsg_queue;	//把改变的配置传递给thread_write_flash
	
	C_Lock *plock4param;	//读写配置参数的互斥锁(优先使用读写锁)	
	SConfigDevInfo *pdev_info;
	SConfigDvrProperty *pdev_property;
	SConfigAllParam *psall_param;//所有配置参数
	SConfigAllParam *psall_param_default;//所有默认配置参数
};

CBizConfig::CBizConfig()
: b_inited(FALSE)
, b_pause(FALSE)
, cond_msg_count(0)
, pmsg_queue(NULL)
, plock4param(NULL)
, pdev_info(NULL)
, pdev_property(NULL)
, psall_param(NULL)
, psall_param_default(NULL)
{
	
}

PATTERN_SINGLETON_IMPLEMENT(CBizConfig);

//类实现
int CBizConfig::Init()
{
	//rwlock
	plock4param = new CRWlock;
	if (NULL == plock4param)
	{
		ERR_PRINT("new CRWlock failed\n");
		goto fail;
	}

	if (plock4param->Create())//FALSE
	{
		ERR_PRINT("create CRWlock failed\n");
		goto fail;
	}
	
	//创建线程及相关数据
	if (pthread_cond_init(&cond, NULL))
	{
		ERR_PRINT("init cond failed\n");
		goto fail;
	}
	
	if (pthread_mutex_init(&cond_mutex, NULL))
	{
		ERR_PRINT("init cond_mutex failed\n");
		goto fail;
	}

	pmsg_queue = new CcircularBuffer(1024);
	if (NULL == pmsg_queue)
	{
		ERR_PRINT("new CcircularBuffer failed\n");
		goto fail;
	}

	if (pmsg_queue->CreateBuffer())
	{
		ERR_PRINT("CreateBuffer CcircularBuffer failed\n");
		goto fail;
	}
	
	m_threadlet.run("BizConfig-writeFlash", this, (ASYNPROC)&CBizConfig::threadWriteFlash, 0, 0);
	
	//读文件，加载参数
	pdev_info = new SConfigDevInfo;
	if (NULL == pdev_info)
	{
		ERR_PRINT("new SConfigDevInfo failed\n");
		goto fail;
	}
	//memset(pdev_info, 0, sizeof(SConfigDevInfo));//有string 成员，不能memset
	
	if (InitConfigInfo())
	{
		ERR_PRINT("InitConfigInfo() failed\n");
		goto fail;
	}

	pdev_property = new SConfigDvrProperty;
	if (NULL == pdev_info)
	{
		ERR_PRINT("new SConfigDvrProperty failed\n");
		goto fail;
	}
	//memset(pdev_property, 0, sizeof(SConfigDvrProperty));
	
	if (InitDevProperty())
	{
		ERR_PRINT("InitDevProperty() failed\n");
		goto fail;
	}

#if 1
	//加载用户配置文件
	//从/dev/mtd2 加载到 /tmp/data/cfg/UserCfg.ini
	if (LoadUserCfgFileFromFlash())
	{
		ERR_PRINT("LoadUserCfgFileFromFlash() failed\n");
		goto fail;
	}
#endif

	//加载用户配置文件到参数
	psall_param = new SConfigAllParam;
	if (NULL == psall_param)
	{
		ERR_PRINT("new SConfigAllParam failed\n");
		goto fail;
	}

	if (LoadUserCfgParamFromFile(psall_param, pdev_info->nFilePath.c_str()))
	{
		DBG_PRINT("Load psall_param from %s failed, load default file\n", \
			pdev_info->nFilePath.c_str());

		//配置文件失败后从默认文件加载参数
		if (LoadUserCfgParamFromFile(psall_param, pdev_info->nDefaultConfPath.c_str()))
		{
			ERR_PRINT("Load psall_param from %s failed\n", \
				pdev_info->nDefaultConfPath.c_str());
			goto fail;	
		}
		
		//给threadWriteFlash 发消息，保存配置到flash
	}

	//加载默认配置文件到参数
	psall_param_default= new SConfigAllParam;
	if (NULL == psall_param_default)
	{
		ERR_PRINT("new SConfigAllParam default failed\n");
		goto fail;
	}
	
	if (LoadUserCfgParamFromFile(psall_param_default, pdev_info->nDefaultConfPath.c_str()))
	{
		ERR_PRINT("Load psall_param_default from %s failed\n", \
			pdev_info->nDefaultConfPath.c_str());
		goto fail;
	}

	//finish
	b_inited = TRUE;
	return SUCCESS;

fail:
	FreeSrc();
	return -FAILURE;
}

void CBizConfig::FreeSrc()
{
	b_inited = FALSE;

	if (plock4param)
	{
		delete plock4param;
		plock4param = NULL;
	}

	if (psall_param_default)
	{
		delete psall_param_default;
		psall_param_default = NULL;
	}

	if (psall_param)
	{
		delete psall_param;
		psall_param = NULL;
	}

	if (pdev_property)
	{
		delete pdev_property;
		pdev_property = NULL;
	}
	
	if (pdev_info)
	{
		delete pdev_info;
		pdev_info = NULL;
	}

	if (pmsg_queue)
	{
		delete pmsg_queue;
		pmsg_queue = NULL;
	}
	
	pthread_mutex_destroy(&cond_mutex);
	pthread_cond_destroy(&cond);
}

CBizConfig:: ~CBizConfig()
{
	FreeSrc();
}

int CBizConfig::InitConfigInfo()
{
	int itmp = 0;
	char stmp[100];
	
	if(0 != PublicIniGetValue(DVR_CUSTOM_PATH, \
		DVR_CONFIG_INFO, \
		DVR_CONFIG_INFO_SAVETYPE, \
		&itmp))
	{
		ERR_PRINT("PublicIniGetValue DVR_CONFIG_INFO_SAVETYPE failed\n");
		return -FAILURE;
	}
	pdev_info->nConfSaveType = itmp;
	DBG_PRINT("nConfSaveType: %d\n", pdev_info->nConfSaveType);

	if(0 != PublicIniGetValue(DVR_CUSTOM_PATH, \
		DVR_CONFIG_INFO, \
		DVR_CONFIG_INFO_FORMAT, \
		&itmp))
	{
		ERR_PRINT("PublicIniGetValue DVR_CONFIG_INFO_FORMAT failed\n");
		return -FAILURE;
	}
	pdev_info->nConfFormat = itmp;
	DBG_PRINT("nConfFormat: %d\n", pdev_info->nConfFormat);

	if(0 != PublicIniGetString(DVR_CUSTOM_PATH, \
		DVR_CONFIG_INFO, \
		DVR_CONFIG_INFO_FLASHDEV, \
		stmp,
		sizeof(stmp)))
	{
		ERR_PRINT("PublicIniGetValue DVR_CONFIG_INFO_FLASHDEV failed\n");
		return -FAILURE;
	}
	pdev_info->nDevPath = stmp;
	DBG_PRINT("nDevPath: %s\n", pdev_info->nDevPath.c_str());

	if(0 != PublicIniGetString(DVR_CUSTOM_PATH, \
		DVR_CONFIG_INFO, \
		DVR_CONFIG_INFO_DEFAULTPATH, \
		stmp,
		sizeof(stmp)))
	{
		ERR_PRINT("PublicIniGetValue DVR_CONFIG_INFO_DEFAULTPATH failed\n");
		return -FAILURE;
	}
	pdev_info->nDefaultConfPath = stmp;
	DBG_PRINT("nDefaultConfPath: %s\n", pdev_info->nDefaultConfPath.c_str());
	
	if(0 != PublicIniGetString(DVR_CUSTOM_PATH, \
		DVR_CONFIG_INFO, \
		DVR_CONFIG_INFO_FILEPATH, \
		stmp,
		sizeof(stmp)))
	{
		ERR_PRINT("PublicIniGetValue DVR_CONFIG_INFO_FILEPATH failed\n");
		return -FAILURE;
	}
	pdev_info->nFilePath = stmp;
	DBG_PRINT("nFilePath: %s\n", pdev_info->nFilePath.c_str());
	
	if(0 != PublicIniGetValue(DVR_CUSTOM_PATH, \
		DVR_CONFIG_INFO, \
		DVR_CONFIG_INFO_ZIP, \
		&itmp))
	{
		ERR_PRINT("PublicIniGetValue DVR_CONFIG_INFO_ZIP failed\n");
		return -FAILURE;
	}
	pdev_info->nConfZip = itmp;
	DBG_PRINT("nConfZip: %d\n", pdev_info->nConfZip);

	if(0 != PublicIniGetValue(DVR_CUSTOM_PATH, \
		DVR_CONFIG_INFO, \
		DVR_CONFIG_INFO_OFFSET, \
		&itmp))
	{
		ERR_PRINT("PublicIniGetValue DVR_CONFIG_INFO_OFFSET failed\n");
		return -FAILURE;
	}
	pdev_info->nFlashOffset = itmp;
	DBG_PRINT("nFlashOffset: %d\n", pdev_info->nFlashOffset);

	if(0 != PublicIniGetValue(DVR_CUSTOM_PATH, \
		DVR_CONFIG_INFO, \
		DVR_CONFIG_INFO_LENGTH, \
		&itmp))
	{
		ERR_PRINT("PublicIniGetValue DVR_CONFIG_INFO_LENGTH failed\n");
		return -FAILURE;
	}
	pdev_info->nFlashSizeLimit = itmp;
	DBG_PRINT("nFlashSizeLimit: %d\n", pdev_info->nFlashSizeLimit);
	
	return SUCCESS;
}

int CBizConfig::InitDevProperty()
{
	int itmp = 0;
	char stmp[100];

	if(0 != PublicIniGetString(DVR_CUSTOM_PATH, \
		DVR_PROPERTY, \
		DVR_PROPERTY_VERSION, \
		stmp,
		sizeof(stmp)))
	{
		ERR_PRINT("PublicIniGetValue DVR_PROPERTY_VERSION failed\n");
		return -FAILURE;
	}
	pdev_property->nVersion = stmp;
	DBG_PRINT("nVersion: %s\n", pdev_property->nVersion.c_str());

	if(0 != PublicIniGetString(DVR_CUSTOM_PATH, \
		DVR_PROPERTY, \
		DVR_PROPERTY_MODEL, \
		stmp,
		sizeof(stmp)))
	{
		ERR_PRINT("PublicIniGetValue DVR_PROPERTY_MODEL failed\n");
		return -FAILURE;
	}
	pdev_property->nModel = stmp;
	DBG_PRINT("nModel: %s\n", pdev_property->nModel.c_str());

	if(0 != PublicIniGetString(DVR_CUSTOM_PATH, \
		DVR_PROPERTY, \
		DVR_PROPERTY_PRODUCTNUMBER, \
		stmp,
		sizeof(stmp)))
	{
		ERR_PRINT("PublicIniGetValue DVR_PROPERTY_PRODUCTNUMBER failed\n");
		return -FAILURE;
	}
	pdev_property->nproductnumber = stmp;
	DBG_PRINT("nproductnumber: %s\n", pdev_property->nproductnumber.c_str());

	if(0 != PublicIniGetValue(DVR_CUSTOM_PATH, \
		DVR_PROPERTY, \
		DVR_PROPERTY_PREVIEWNUMS, \
		&itmp))
	{
		ERR_PRINT("PublicIniGetValue DVR_PROPERTY_PREVIEWNUMS failed\n");
		return -FAILURE;
	}
	pdev_property->nPreview_nums = itmp;
	DBG_PRINT("nPreview_nums: %d\n", pdev_property->nPreview_nums);

	if(0 != PublicIniGetValue(DVR_CUSTOM_PATH, \
		DVR_PROPERTY, \
		DVR_PROPERTY_SVRCONNECTNUMS, \
		&itmp))
	{
		ERR_PRINT("PublicIniGetValue DVR_PROPERTY_SVRCONNECTNUMS failed\n");
		return -FAILURE;
	}
	pdev_property->nsvr_connect_nums= itmp;
	DBG_PRINT("nsvr_connect_nums: %d\n", pdev_property->nsvr_connect_nums);

	if(0 != PublicIniGetValue(DVR_CUSTOM_PATH, \
		DVR_PROPERTY, \
		DVR_PROPERTY_TVWALLNUMS, \
		&itmp))
	{
		ERR_PRINT("PublicIniGetValue DVR_PROPERTY_TVWALLNUMS failed\n");
		return -FAILURE;
	}
	pdev_property->nTvwall_nums = itmp;
	DBG_PRINT("nTvwall_nums: %d\n", pdev_property->nTvwall_nums);
	
	if(0 != PublicIniGetValue(DVR_CUSTOM_PATH, \
		DVR_PROPERTY, \
		DVR_PROPERTY_TVWALLDEVSMAX, \
		&itmp))
	{
		ERR_PRINT("PublicIniGetValue DVR_PROPERTY_TVWALLDEVSMAX failed\n");
		return -FAILURE;
	}
	pdev_property->nTvwall_devs_max = itmp;
	DBG_PRINT("nTvwall_devs_max: %d\n", pdev_property->nTvwall_devs_max);

	if(0 != PublicIniGetValue(DVR_CUSTOM_PATH, \
		DVR_PROPERTY, \
		DVR_PROPERTY_TVWALLLINE, \
		&itmp))
	{
		ERR_PRINT("PublicIniGetValue DVR_PROPERTY_TVWALLLINE failed\n");
		return -FAILURE;
	}
	pdev_property->nTvwall_devs_per_line = itmp;
	DBG_PRINT("nTvwall_devs_per_line: %d\n", pdev_property->nTvwall_devs_per_line);

	if(0 != PublicIniGetValue(DVR_CUSTOM_PATH, \
		DVR_PROPERTY, \
		DVR_PROPERTY_TVWALLCOL, \
		&itmp))
	{
		ERR_PRINT("PublicIniGetValue DVR_PROPERTY_TVWALLCOL failed\n");
		return -FAILURE;
	}
	pdev_property->nTvwall_devs_per_col = itmp;
	DBG_PRINT("nTvwall_devs_per_col: %d\n", pdev_property->nTvwall_devs_per_col);

	if(0 != PublicIniGetValue(DVR_CUSTOM_PATH, \
		DVR_PROPERTY, \
		DVR_PROPERTY_NVRNUMS, \
		&itmp))
	{
		ERR_PRINT("PublicIniGetValue DVR_PROPERTY_NVRNUMS failed\n");
		return -FAILURE;
	}
	pdev_property->nNvr_nums = itmp;
	DBG_PRINT("nNvr_nums: %d\n", pdev_property->nNvr_nums);

	if(0 != PublicIniGetValue(DVR_CUSTOM_PATH, \
		DVR_PROPERTY, \
		DVR_PROPERTY_DECNUMS, \
		&itmp))
	{
		ERR_PRINT("PublicIniGetValue DVR_PROPERTY_DECNUMS failed\n");
		return -FAILURE;
	}
	pdev_property->nDec_nums = itmp;
	DBG_PRINT("nDec_nums: %d\n", pdev_property->nDec_nums);
	
	return SUCCESS;
}

//从/dev/mtd2 加载到 /tmp/data/cfg/UserCfg.ini
int CBizConfig::LoadUserCfgFileFromFlash()
{
	u8 flag_default = 0;//是否使用默认配置文件

	//ERR_PRINT("open nDefaultConfPath: %s\n", pdev_info->nDefaultConfPath.c_str());
	//ERR_PRINT("open nFilePath: %s\n", pdev_info->nFilePath.c_str());
		
	if (0 == pdev_info->nConfSaveType)//CfgWay : 配置保存方式 (0=Flash;1=File)
	{
		if (ReadFlashToFile(pdev_info->nFilePath.c_str(), pdev_info->nDevPath.c_str(), 0, pdev_info->nFlashSizeLimit>>1))//读配置到文件
		{
			ERR_PRINT("ReadFlashToFile nDevPath: %s failed, use backup\n", pdev_info->nDevPath.c_str());
			if (ReadFlashToFile(pdev_info->nFilePath.c_str(), pdev_info->nDevPath.c_str(), pdev_info->nFlashSizeLimit>>1, pdev_info->nFlashSizeLimit>>1))//读配置到文件
			{
				flag_default = 1;

				ERR_PRINT("ReadFlashToFile nDevPath backup: %s failed, use default\n", pdev_info->nDevPath.c_str());
			}
		}
	}
	else
	{
		ERR_PRINT("nConfSaveType: %d, nonsupport\n", pdev_info->nConfSaveType);
		return -FAILURE;
	}

	if (flag_default)//使用默认配置文件
	{
		int fd_src = 0;
		int fd_dst = 0;

		fd_src = open(pdev_info->nDefaultConfPath.c_str(), O_RDONLY);
		if (fd_src < 0)
		{
			ERR_PRINT("open nDefaultConfPath: %s failed\n", pdev_info->nDefaultConfPath.c_str());
			return -FAILURE;
		}
		
		fd_dst = open(pdev_info->nFilePath.c_str(), O_CREAT|O_WRONLY|O_TRUNC, 0644);
		if (fd_dst < 0)
		{
			ERR_PRINT("open nFilePath: %s failed\n", pdev_info->nFilePath.c_str());
			close(fd_src);
			return FALSE;
		}

		if (copyFile(fd_dst, fd_src))
		{
			ERR_PRINT("copyFile nDefaultConfPath2nFilePath failed\n");
			close(fd_src);
			close(fd_dst);
			return -FAILURE;
		}
		
		close(fd_src);
		close(fd_dst);

		//写消息保存文件到flash
		if (0 == pdev_info->nConfSaveType)//CfgWay : 配置保存方式 (0=Flash;1=File)
		{
			//pmsg_queue->Put(uchar * pbuf,uint len);
			if (WriteMsg2Thread(EM_BIZCONFIG_ALL_FILE))
			{
				ERR_PRINT("WriteMsg2Thread failed\n");
				return -FAILURE;
			}
		}
	}

	return SUCCESS;
}

//写 /tmp/data/cfg/UserCfg.ini 到/dev/mtd2 
int CBizConfig::UpgradeUserCfgFile2Flash()
{
	int dev_fd = 0;
	
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}
	

	FILE* fp = fopen(pdev_info->nFilePath.c_str(), "r");
	if(NULL == fp)
	{
		ERR_PRINT("open file: %s failed!\n", pdev_info->nFilePath.c_str());
		return -FAILURE;
	}

	dev_fd = open(pdev_info->nDevPath.c_str(), O_RDWR);
	if(dev_fd < 0)
	{
		ERR_PRINT("open flash dev file: %s failed!\n", pdev_info->nDevPath.c_str());
		return -FAILURE;
	}
	
	int rtn;
	
	//csp modify
	struct mtd_info_user mtd;
	memset(&mtd, 0, sizeof(struct mtd_info_user));
	rtn = ioctl(dev_fd, MEMGETINFO, &mtd);
	//printf("MEMGETINFO=%d,mtdsize=%d,erasesize=%d\n",rtn,mtd.size,mtd.erasesize);
	
	struct erase_info_user erase;
	erase.start = pdev_info->nFlashOffset;
	erase.length = pdev_info->nFlashSizeLimit >> 1;//g_sConfigDevInfo.nFlashSizeLimit;//g_sConfigDevInfo.nFlashSizeLimit >> 1;//csp modify
	rtn = ioctl(dev_fd, MEMERASE, (char*)&erase);
	if(rtn < 0)
	{
		perror("MEMERASE");
		printf("MEMERASE-1 failed <fd=%d,start=%d,length=%d> in function:%s\n", dev_fd, erase.start, erase.length, __FUNCTION__);
		fclose(fp);
		return -FAILURE;
	}
	
	#if 1
	fseek(fp,0,SEEK_END);
	s32 len = ftell(fp);
	if (len < 0)
	{
		printf("%s ftell failed, len: %d, errno: %d, %s\n", __func__, len, errno, strerror(errno));
		fclose(fp);
		return -FAILURE;
	}
	
	fseek(fp,0,SEEK_SET);
	s8* pData = (s8*)malloc(len);
	fread(pData,len,1,fp);
	#else
	s8* pData = calloc(1024, 50);
	u32 len = 0;
	int c;
	while((c = fgetc(fp)) != EOF)
	{
		if((len) && ((len%(1024*50)) == 0))
		{
			pData = realloc(pData, 1024*50*(len/(1024*50)+1));
		}
		*(pData+len) = c;
		len++;		
	}
	#endif
	
	s32 ret = SetZipConf(dev_fd, 0, pData, len, pdev_info->nFlashSizeLimit>>1);
	if (-1 == ret)
	{
		printf("SetZipConf-1 failed! function:%s\n", __FUNCTION__);
		free(pData);
		fclose(fp);
		return -FAILURE;
	}
	if(-2 == ret)
	{
		printf("Warning: backup config to flash [No Space]\n");
	}
	else
	{
		#if 1//csp modify 20140812
		erase.start = pdev_info->nFlashOffset + (pdev_info->nFlashSizeLimit >> 1);
		erase.length = pdev_info->nFlashSizeLimit >> 1;
		rtn = ioctl(dev_fd, MEMERASE, (char*)&erase);
		if(rtn < 0)
		{
			printf("MEMERASE-2 failed! function:%s\n", __FUNCTION__);
			fclose(fp);
			free(pData);
			return -FAILURE;
		}
		#endif
		
		if(0 != SetZipConf(dev_fd, pdev_info->nFlashSizeLimit>>1, pData, len, pdev_info->nFlashSizeLimit>>1))
		{
			printf("Warning: backup config to flash error!\n");
		}
	}
	
	free(pData);
	close(dev_fd);
	fclose(fp);
	
	return SUCCESS;
}


//从配置文件加载时间参数
int CBizConfig::GetTimeParamFromFile(SConfigTimeParam &stime_param, const char *file_name)
{
	int itmp;
	char stmp[100];

	//日期格式
	if(0 != PublicIniGetValue(file_name, \
		CONFIG_TIME_PARAM, \
		CONFIG_TIME_PARAM_DATA_FORMAT, \
		&itmp))
	{
		ERR_PRINT("CONFIG_TIME_PARAM_DATA_FORMAT failed\n");
		return -FAILURE;
	}
	stime_param.ndate_format = itmp;
	DBG_PRINT("ndate_format: %d\n", itmp);

	//时间格式
	if(0 != PublicIniGetValue(file_name, \
		CONFIG_TIME_PARAM, \
		CONFIG_TIME_PARAM_TIME_FORMAT, \
		&itmp))
	{
		ERR_PRINT("CONFIG_TIME_PARAM_TIME_FORMAT failed\n");
		return -FAILURE;
	}
	stime_param.ntime_format = itmp;
	DBG_PRINT("ntime_format: %d\n", itmp);

	//自动同步
	if(0 != PublicIniGetValue(file_name, \
		CONFIG_TIME_PARAM, \
		CONFIG_TIME_PARAM_TIME_SYNC, \
		&itmp))
	{
		ERR_PRINT("CONFIG_TIME_PARAM_TIME_SYNC failed\n");
		return -FAILURE;
	}
	stime_param.btime_sync = itmp;
	DBG_PRINT("btime_sync: %d\n", itmp);

	//时间服务器IP
	if(0 != PublicIniGetString(file_name, \
		CONFIG_TIME_PARAM, \
		CONFIG_TIME_PARAM_NTPSVR_IP, \
		stmp,
		sizeof(stmp)))
	{
		ERR_PRINT("CONFIG_TIME_PARAM_NTPSVR_IP failed\n");
		return -FAILURE;
	}
	stime_param.ntp_svr_ip = stmp;
	DBG_PRINT("ntp_svr_ip : %s\n", stmp);
	
	return SUCCESS;
}

int CBizConfig::SetTimeParam2File(SConfigTimeParam &stime_param)
{
	int itmp;
	char stmp[100];
	char file_name[100];

	strcpy(file_name, pdev_info->nFilePath.c_str());

	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}
	
	//日期格式
	itmp = stime_param.ndate_format;
	
	if(0 != PublicIniSetValue(file_name, \
		CONFIG_TIME_PARAM, \
		CONFIG_TIME_PARAM_DATA_FORMAT, \
		itmp))
	{
		ERR_PRINT("ndate_format %d failed\n", itmp);
		return -FAILURE;
	}
	DBG_PRINT("ndate_format %d\n", itmp);

	//时间格式
	itmp = stime_param.ntime_format;
	
	if(0 != PublicIniSetValue(file_name, \
		CONFIG_TIME_PARAM, \
		CONFIG_TIME_PARAM_TIME_FORMAT, \
		itmp))
	{
		ERR_PRINT("ntime_format %d failed\n", itmp);
		return -FAILURE;
	}
	DBG_PRINT("ntime_format %d\n", itmp);

	//自动同步
	itmp = stime_param.btime_sync;
	
	if(0 != PublicIniSetValue(file_name, \
		CONFIG_TIME_PARAM, \
		CONFIG_TIME_PARAM_TIME_SYNC, \
		itmp))
	{
		ERR_PRINT("btime_sync %d failed\n", itmp);
		return -FAILURE;
	}
	DBG_PRINT("btime_sync %d\n", itmp);

	//时间服务器IP
	if (stime_param.ntp_svr_ip.size())
		strcpy(stmp, stime_param.ntp_svr_ip.c_str());
	else
		strcpy(stmp, psall_param_default->stime_param.ntp_svr_ip.c_str());
		
	if(0 != PublicIniSetString(file_name, \
		CONFIG_TIME_PARAM, \
		CONFIG_TIME_PARAM_NTPSVR_IP, \
		stmp))
	{
		ERR_PRINT("ntp_svr_ip failed\n");
		return -FAILURE;
	}
	DBG_PRINT("ntp_svr_ip %s\n", stmp);
	
	return SUCCESS;
}

//从配置文件加载网络参数
int CBizConfig::GetNetParamFromFile(SConfigNetParam &snet_param, const char *file_name)
{
	int itmp;
	char stmp[100];
	struct in_addr in;

	//nhost_ip
	if(0 != PublicIniGetValue(file_name, \
		CONFIG_NET_PARAM, \
		CONFIG_NET_PARAM_HOSTIP, \
		&itmp))
	{
		ERR_PRINT("CONFIG_NET_PARAM_HOSTIP failed\n");
		return -FAILURE;
	}
	snet_param.nhost_ip = itmp;
	in.s_addr = itmp;
	DBG_PRINT("nhost_ip: %s\n", inet_ntoa(in));

	//nsubmask
	if(0 != PublicIniGetValue(file_name, \
		CONFIG_NET_PARAM, \
		CONFIG_NET_PARAM_SUBMASK, \
		&itmp))
	{
		ERR_PRINT("CONFIG_NET_PARAM_SUBMASK failed\n");
		return -FAILURE;
	}
	snet_param.nsubmask = itmp;
	in.s_addr = itmp;
	DBG_PRINT("nsubmask: %s\n", inet_ntoa(in));

	//ngateway
	if(0 != PublicIniGetValue(file_name, \
		CONFIG_NET_PARAM, \
		CONFIG_NET_PARAM_GATEWAY, \
		&itmp))
	{
		ERR_PRINT("CONFIG_NET_PARAM_GATEWAY failed\n");
		return -FAILURE;
	}
	snet_param.ngateway = itmp;
	in.s_addr = itmp;
	DBG_PRINT("ngateway: %s\n", inet_ntoa(in));

	//ndns1
	if(0 != PublicIniGetValue(file_name, \
		CONFIG_NET_PARAM, \
		CONFIG_NET_PARAM_DNS1, \
		&itmp))
	{
		ERR_PRINT("CONFIG_NET_PARAM_DNS1 failed\n");
		return -FAILURE;
	}
	snet_param.ndns1 = itmp;
	in.s_addr = itmp;
	DBG_PRINT("ndns1: %s\n", inet_ntoa(in));

	//ndns2
	if(0 != PublicIniGetValue(file_name, \
		CONFIG_NET_PARAM, \
		CONFIG_NET_PARAM_DNS2, \
		&itmp))
	{
		ERR_PRINT("CONFIG_NET_PARAM_DNS2 failed\n");
		return -FAILURE;
	}
	snet_param.ndns2 = itmp;
	in.s_addr = itmp;
	DBG_PRINT("ndns2: %s\n", inet_ntoa(in));

	//nsvr_port
	if(0 != PublicIniGetValue(file_name, \
		CONFIG_NET_PARAM, \
		CONFIG_NET_PARAM_SVRPORT, \
		&itmp))
	{
		ERR_PRINT("CONFIG_NET_PARAM_SVRPORT failed\n");
		return -FAILURE;
	}
	snet_param.nsvr_port = itmp;
	DBG_PRINT("nsvr_port: %d\n", itmp);

	//Mac
	if(0 != PublicIniGetString(file_name, \
		CONFIG_NET_PARAM, \
		CONFIG_NET_PARAM_MAC, \
		stmp,
		sizeof(stmp)))
	{
		ERR_PRINT("CONFIG_NET_PARAM_MAC failed\n");
		return -FAILURE;
	}
	snet_param.mac = stmp;
	ToUpperString(snet_param.mac);
	DBG_PRINT("mac : %s\n", stmp);
	
	return SUCCESS;
}

int CBizConfig::SetNetParam2File(SConfigNetParam &snet_param)
{
	uint itmp;
	char stmp[100];
	struct in_addr in;
	char file_name[100];

	strcpy(file_name, pdev_info->nFilePath.c_str());

	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}
	
	//nhost_ip
	itmp = snet_param.nhost_ip;
	in.s_addr = itmp;
	
	if(0 != PublicIniSetValue(file_name, \
		CONFIG_NET_PARAM, \
		CONFIG_NET_PARAM_HOSTIP, \
		itmp))
	{		
		ERR_PRINT("nhost_ip: %s, failed\n", inet_ntoa(in));
		return -FAILURE;
	}
	DBG_PRINT("nhost_ip: %s\n", inet_ntoa(in));

	//nsubmask
	itmp = snet_param.nsubmask;
	in.s_addr = itmp;
	
	if(0 != PublicIniSetValue(file_name, \
		CONFIG_NET_PARAM, \
		CONFIG_NET_PARAM_SUBMASK, \
		itmp))
	{
		ERR_PRINT("submask: %s, failed\n", inet_ntoa(in));
		return -FAILURE;
	}
	DBG_PRINT("submask: %s\n", inet_ntoa(in));

	//ngateway
	itmp = snet_param.ngateway;
	in.s_addr = itmp;
	
	if(0 != PublicIniSetValue(file_name, \
		CONFIG_NET_PARAM, \
		CONFIG_NET_PARAM_GATEWAY, \
		itmp))
	{
		ERR_PRINT("gateway: %s, failed\n", inet_ntoa(in));
		return -FAILURE;
	}
	DBG_PRINT("gateway: %s\n", inet_ntoa(in));

	//ndns1
	itmp = snet_param.ndns1;
	in.s_addr = itmp;
	
	if(0 != PublicIniSetValue(file_name, \
		CONFIG_NET_PARAM, \
		CONFIG_NET_PARAM_DNS1, \
		itmp))
	{
		ERR_PRINT("dns1: %s, failed\n", inet_ntoa(in));
		return -FAILURE;
	}
	DBG_PRINT("dns1: %s\n", inet_ntoa(in));

	//ndns2
	itmp = snet_param.ndns2;
	in.s_addr = itmp;
	
	if(0 != PublicIniSetValue(file_name, \
		CONFIG_NET_PARAM, \
		CONFIG_NET_PARAM_DNS2, \
		itmp))
	{
		ERR_PRINT("dns2: %s, failed\n", inet_ntoa(in));
		return -FAILURE;
	}
	DBG_PRINT("dns2: %s\n", inet_ntoa(in));

	//nsvr_port
	itmp = snet_param.nsvr_port;
	
	if (0 != PublicIniSetValue(file_name, \
		CONFIG_NET_PARAM, \
		CONFIG_NET_PARAM_SVRPORT, \
		itmp))
	{
		ERR_PRINT("svr_port: %d, failed\n", itmp);
		return -FAILURE;
	}
	DBG_PRINT("svr_port: %d\n", itmp);

	//Mac	
	if (snet_param.mac.size())
	{
		ToUpperString(snet_param.mac);
		strcpy(stmp, snet_param.mac.c_str());
	}
	else
	{
		strcpy(stmp, psall_param_default->snet_param.mac.c_str());
	}
	
	if (0 != PublicIniSetString(file_name, \
		CONFIG_NET_PARAM, \
		CONFIG_NET_PARAM_MAC, \
		stmp))
	{
		ERR_PRINT("mac %s, failed\n", stmp);
		return -FAILURE;
	}
	DBG_PRINT("mac: %s\n", stmp);
	
	return SUCCESS;
}


//从配置文件加载电视墙参数
int CBizConfig::GetTvWallParamFromFile(std::vector<SConfigTvWallParam> &vtvwall_list, const char *file_name)
{
	int itmp;
	char str_item[100];
	uint i, j, list_num;
	struct in_addr in;
	SConfigTvWallParam tvwall_param;
	
	i = j = list_num = 0;
	vtvwall_list.clear();
	
	//nums
	if(0 != PublicIniGetValue(file_name, \
		CONFIG_TVWALL_PARAM, \
		CONFIG_TVWALL_PARAM_NUMS, \
		&itmp))
	{
		ERR_PRINT("tvwall nums failed\n");
		return -FAILURE;
	}
	list_num = itmp;
	DBG_PRINT("tvwall nums: %d\n", list_num);

	for (i=0; i<list_num; ++i)
	{
		memset(&tvwall_param, 0, sizeof(SConfigTvWallParam));
		
		//ndevs_per_line
		sprintf(str_item, "%s%d", CONFIG_TVWALL_PARAM_DEVS_PER_LINE, i);
		if(0 != PublicIniGetValue(file_name, \
			CONFIG_TVWALL_PARAM, \
			str_item, \
			&itmp))
		{
			ERR_PRINT("tvwall %s failed\n", str_item);
			goto fail;
		}
		tvwall_param.ndevs_per_line = itmp;
		DBG_PRINT("%s: %d\n", str_item, itmp);

		//ndevs_per_col
		sprintf(str_item, "%s%d", CONFIG_TVWALL_PARAM_DEVS_PER_COL, i);
		if(0 != PublicIniGetValue(file_name, \
			CONFIG_TVWALL_PARAM, \
			str_item, \
			&itmp))
		{
			ERR_PRINT("tvwall %s failed\n", str_item);
			goto fail;
		}
		tvwall_param.ndevs_per_col = itmp;
		DBG_PRINT("%s: %d\n", str_item, itmp);

		//devlist
		sprintf(str_item, "%s%d", CONFIG_TVWALL_PARAM_DEVLIST, i);
		for (j=0; j<tvwall_param.ndevs_per_line * tvwall_param.ndevs_per_col; ++j)
		{
			if(0 != PublicIniGetArrayValue(file_name, \
				CONFIG_TVWALL_PARAM, \
				str_item, \
				j,
				&itmp))
			{
				ERR_PRINT("devlist%d-%d failed\n", i, j);
				tvwall_param.devlist.clear();
				goto fail;
			}
			tvwall_param.devlist.push_back(itmp);
			in.s_addr = itmp;
			DBG_PRINT("devlist%d-%d: %s\n", i, j, inet_ntoa(in));
		}
		
		vtvwall_list.push_back(tvwall_param);
	}
	
	return SUCCESS;

fail:
	
	vtvwall_list.clear();
	return -FAILURE;
}

int CBizConfig::SetTvWallParam2File(std::vector<SConfigTvWallParam> &vtvwall_list)
{
	uint itmp;
	char str_item[100];
	struct in_addr in;
	char file_name[100];
	uint i, j, list_num;

	strcpy(file_name, pdev_info->nFilePath.c_str());

	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	i = j = 0;
	list_num = vtvwall_list.size();

	if (list_num > pdev_property->nTvwall_nums)
	{
		ERR_PRINT("too mang tvwall lists(%d), limit: %d\n", \
			list_num, \
			pdev_property->nTvwall_nums);
		return -FAILURE;
	}
	
	//nums
	itmp = list_num;
	
	if(0 != PublicIniSetValue(file_name, \
		CONFIG_TVWALL_PARAM, \
		CONFIG_TVWALL_PARAM_NUMS, \
		itmp))
	{
		ERR_PRINT("tvwall nums failed\n");
		return -FAILURE;
	}
	DBG_PRINT("tvwall nums: %d\n", itmp);

	for (i=0; i<list_num; ++i)
	{		
		//ndevs_per_line
		itmp = vtvwall_list[i].ndevs_per_line;
		
		sprintf(str_item, "%s%d", CONFIG_TVWALL_PARAM_DEVS_PER_LINE, i);
		if(0 != PublicIniSetValue(file_name, \
			CONFIG_TVWALL_PARAM, \
			str_item, \
			itmp))
		{
			ERR_PRINT("tvwall %s failed\n", str_item);
			return -FAILURE;
		}
		DBG_PRINT("%s: %d\n", str_item, itmp);

		//ndevs_per_col
		itmp = vtvwall_list[i].ndevs_per_col;
		
		sprintf(str_item, "%s%d", CONFIG_TVWALL_PARAM_DEVS_PER_COL, i);
		if(0 != PublicIniSetValue(file_name, \
			CONFIG_TVWALL_PARAM, \
			str_item, \
			itmp))
		{
			ERR_PRINT("tvwall %s failed\n", str_item);
			return -FAILURE;
		}
		DBG_PRINT("%s: %d\n", str_item, itmp);

		//devlist
		sprintf(str_item, "%s%d", CONFIG_TVWALL_PARAM_DEVLIST, i);
		for (j=0; j<vtvwall_list[i].ndevs_per_line * vtvwall_list[i].ndevs_per_col; ++j)
		{
			itmp = vtvwall_list[i].devlist[j];
			in.s_addr = itmp;
			if(0 != PublicIniSetArrayValue(file_name, \
				CONFIG_TVWALL_PARAM, \
				str_item, \
				j,
				itmp))
			{
				ERR_PRINT("tvwall %s%d-%d: %s failed\n", str_item, i, j, inet_ntoa(in));
				return -FAILURE;
			}
			DBG_PRINT("tvwall %s%d-%d: %s\n", str_item, i, j, inet_ntoa(in));
		}
	}
	
	return SUCCESS;
}


//从配置文件加载NVR 列表
int CBizConfig::GetDevListFromFile(EM_DEV_TYPE dev_type, std::vector<uint> &vdev_list, const char *file_name)
{
	int itmp;
	uint i, dev_nums;
	struct in_addr in;
	char str_dev_type[20];
	char str_section[20];
	
	i = dev_nums = 0;
	
	switch (dev_type)
	{
		case EM_NVR:
			strcpy(str_dev_type, "NVR");
			strcpy(str_section, CONFIG_NVRLIST_PARAM);
			break;
			
		case EM_PATROL_DEC:
			strcpy(str_dev_type, "Patrol DEC");
			strcpy(str_section, CONFIG_PATROL_DECLIST_PARAM);
			break;
			
		case EM_SWITCH_DEC:
			strcpy(str_dev_type, "Switch DEC");
			strcpy(str_section, CONFIG_SWITCH_DECLIST_PARAM);
			break;

		default:
			ERR_PRINT("dev_type%d exception\n", dev_type);
			return -FAILURE;
	}
	
	vdev_list.clear();
	
	//nums
	if(0 != PublicIniGetValue(file_name, \
		str_section, \
		CONFIG_DEVLIST_PARAM_NUMS, \
		&itmp))
	{
		ERR_PRINT("get %s list nums failed\n", str_dev_type);
		return -FAILURE;
	}
	dev_nums = itmp;
	DBG_PRINT("%s list dev nums: %d\n", str_dev_type, dev_nums);

	for (i=0; i<dev_nums; ++i)
	{
		if(0 != PublicIniGetArrayValue(file_name, \
			str_section, \
			CONFIG_DEVLIST_PARAM_DEVLIST, \
			i,
			&itmp))
		{
			ERR_PRINT("%s list dev%d failed\n", str_dev_type, i);
			vdev_list.clear();
			return -FAILURE;
		}
		vdev_list.push_back(itmp);
		in.s_addr = itmp;
		DBG_PRINT("%s list dev%d: %s\n", str_dev_type, i, inet_ntoa(in));
	}
	
	return SUCCESS;
}

int CBizConfig::SetDevList2File(EM_DEV_TYPE dev_type, std::vector<uint> &vdev_list)
{
	int itmp;
	uint i, dev_nums;
	struct in_addr in;
	char str_dev_type[20];
	char str_section[20];
	char file_name[200];
	strcpy(file_name, pdev_info->nFilePath.c_str());

	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	dev_nums = vdev_list.size();
	switch (dev_type)
	{
		case EM_NVR:
			strcpy(str_dev_type, "NVR");
			strcpy(str_section, CONFIG_NVRLIST_PARAM);
			if (dev_nums > pdev_property->nNvr_nums)
			{
				ERR_PRINT("too mang devs(%d), limit: %d\n", \
					dev_nums, \
					pdev_property->nNvr_nums);
				return -FAILURE;
			}
			break;
			
		case EM_PATROL_DEC:
			strcpy(str_dev_type, "Patrol DEC");
			strcpy(str_section, CONFIG_PATROL_DECLIST_PARAM);
			if (dev_nums > pdev_property->nDec_nums)
			{
				ERR_PRINT("too mang devs(%d), limit: %d\n", \
					dev_nums, \
					pdev_property->nDec_nums);
				return -FAILURE;
			}
			break;
			
		case EM_SWITCH_DEC:
			strcpy(str_dev_type, "Switch DEC");
			strcpy(str_section, CONFIG_SWITCH_DECLIST_PARAM);
			if (dev_nums > pdev_property->nDec_nums)
			{
				ERR_PRINT("too mang devs(%d), limit: %d\n", \
					dev_nums, \
					pdev_property->nDec_nums);
				return -FAILURE;
			}
			break;

		default:
			ERR_PRINT("dev_type%d exception\n", dev_type);
			return -FAILURE;
	}
	
	//nums
	itmp = dev_nums;
	
	if(0 != PublicIniSetValue(file_name, \
		str_section, \
		CONFIG_DEVLIST_PARAM_NUMS, \
		itmp))
	{
		ERR_PRINT("%s list dev nums%d failed\n", str_dev_type, itmp);
		return -FAILURE;
	}
	DBG_PRINT("%s list dev nums: %d\n", str_dev_type, itmp);

	//dev list
	for (i=0; i<dev_nums; ++i)
	{
		itmp = vdev_list[i];
		in.s_addr = itmp;
		
		if(0 != PublicIniSetArrayValue(file_name, \
			str_section, \
			CONFIG_DEVLIST_PARAM_DEVLIST, \
			i,
			itmp))
		{
			ERR_PRINT("%s list dev%d: %s failed\n", str_dev_type, i, inet_ntoa(in));
			vdev_list.clear();
			return -FAILURE;
		}
		DBG_PRINT("%s list dev%d: %s\n", str_dev_type, i, inet_ntoa(in));
	}
	
	return SUCCESS;
}


//从配置文件加载报警联动列表
int CBizConfig::GetAlmLinkParamListFromFile(std::vector<SConfigAlmLinkParam> &valm_link_list, const char *file_name)
{
	int itmp;
	char str_item[10];
	uint i, j, item_nums;
	struct in_addr in;
	SConfigAlmLinkParam alm_link_param;

	i = j = item_nums = 0;
	valm_link_list.clear();

	//nums
	if(0 != PublicIniGetValue(file_name, \
		CONFIG_ALARM_LINK_LIST_PARAM, \
		CONFIG_ALARM_LINK_LIST_PARAM_NUMS, \
		&itmp))
	{
		ERR_PRINT("nums failed\n");
		return -FAILURE;
	}
	item_nums = itmp;
	DBG_PRINT("alm_link_list item nums: %d\n", itmp);

	if (item_nums > pdev_property->nNvr_nums)
	{
		ERR_PRINT("too mang almlink items(%d), limit: %d\n", \
			item_nums, \
			pdev_property->nNvr_nums);
		return -FAILURE;
	}

	for (i=0; i<item_nums; ++i)
	{
		memset(&alm_link_param, 0, sizeof(SConfigAlmLinkParam));
		sprintf(str_item, "%s%d", CONFIG_ALARM_LINK_LIST_PARAM_DEVLIST, i);
		
		//nvr ip
		if(0 != PublicIniGetArrayValue(file_name, \
			CONFIG_ALARM_LINK_LIST_PARAM, \
			str_item, \
			EM_CONFIG_ALARM_LINK_LIST_PARAM_NVRIP_U32,
			&itmp))
		{
			ERR_PRINT("alm_link_list item%d nvr ip failed\n", i);
			goto fail;
		}
		alm_link_param.nvrip = itmp;
		in.s_addr = itmp;
		DBG_PRINT("alm_link_list item%d nvr ip: %s\n", i, inet_ntoa(in));

		//nvr alm src type
		if(0 != PublicIniGetArrayValue(file_name, \
			CONFIG_ALARM_LINK_LIST_PARAM, \
			str_item, \
			EM_CONFIG_ALARM_LINK_LIST_PARAM_ALM_TYPE_U32,
			&itmp))
		{
			ERR_PRINT("alm_link_list item%d alm_type failed\n", i);
			goto fail;
		}
		alm_link_param.alm_type = itmp;
		DBG_PRINT("alm_link_list item%d alm_type: %s\n", i, itmp==0 ? "Sensor":"Ipc");

		//nvr alm chn
		if(0 != PublicIniGetArrayValue(file_name, \
			CONFIG_ALARM_LINK_LIST_PARAM, \
			str_item, \
			EM_CONFIG_ALARM_LINK_LIST_PARAM_ALM_CHN_U32,
			&itmp))
		{
			ERR_PRINT("alm_link_list item%d alm_chn failed\n", i);
			goto fail;
		}
		alm_link_param.alm_chn = itmp;
		DBG_PRINT("alm_link_list item%d alm_chn: %d\n", i, itmp);

		//dec ip
		if(0 != PublicIniGetArrayValue(file_name, \
			CONFIG_ALARM_LINK_LIST_PARAM, \
			str_item, \
			EM_CONFIG_ALARM_LINK_LIST_PARAM_DECIP_U32,
			&itmp))
		{
			ERR_PRINT("alm_link_list item%d dec ip failed\n", i);
			goto fail;
		}
		alm_link_param.decip = itmp;
		in.s_addr = itmp;
		DBG_PRINT("alm_link_list item%d dec ip: %s\n", i, inet_ntoa(in));

		//dec chn
		if(0 != PublicIniGetArrayValue(file_name, \
			CONFIG_ALARM_LINK_LIST_PARAM, \
			str_item, \
			EM_CONFIG_ALARM_LINK_LIST_PARAM_DEC_CHN_U32,
			&itmp))
		{
			ERR_PRINT("alm_link_list item%d dec_chn failed\n", i);
			goto fail;
		}
		alm_link_param.dec_chn = itmp;
		DBG_PRINT("alm_link_list item%d dec_chn: %d\n", i, itmp);
		
		valm_link_list.push_back(alm_link_param);
	}
	
	return SUCCESS;

fail:

	valm_link_list.clear();
	return -FAILURE;
}

int CBizConfig::SetAlmLinkParamList2File(std::vector<SConfigAlmLinkParam> &valm_link_list)
{
	int itmp;
	char str_item[10];
	uint i, j, item_nums;
	struct in_addr in;
	char file_name[200];

	strcpy(file_name, pdev_info->nFilePath.c_str());

	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	i = j = 0;
	item_nums = valm_link_list.size();
	if (item_nums > pdev_property->nNvr_nums)
	{
		ERR_PRINT("too mang almlink items(%d), limit: %d\n", \
			item_nums, \
			pdev_property->nNvr_nums);
		return -FAILURE;
	}

	//nums
	itmp = item_nums;
	
	if(0 != PublicIniSetValue(file_name, \
		CONFIG_ALARM_LINK_LIST_PARAM, \
		CONFIG_ALARM_LINK_LIST_PARAM_NUMS, \
		itmp))
	{
		ERR_PRINT("alm_link_list item nums: %d failed\n", itmp);
		return -FAILURE;
	}
	DBG_PRINT("alm_link_list item nums: %d\n", itmp);

	for (i=0; i<item_nums; ++i)
	{
		sprintf(str_item, "%s%d", CONFIG_ALARM_LINK_LIST_PARAM_DEVLIST, i);
		
		//nvr ip
		itmp = valm_link_list[i].nvrip;
		in.s_addr = itmp;
		
		if(0 != PublicIniSetArrayValue(file_name, \
			CONFIG_ALARM_LINK_LIST_PARAM, \
			str_item, \
			EM_CONFIG_ALARM_LINK_LIST_PARAM_NVRIP_U32,
			itmp))
		{
			ERR_PRINT("alm_link_list item%d nvr ip: %s failed\n", i, inet_ntoa(in));
			return -FAILURE;
		}
		DBG_PRINT("alm_link_list item%d nvr ip: %s\n", i, inet_ntoa(in));

		//nvr alm src type
		itmp = valm_link_list[i].alm_type;
		
		if(0 != PublicIniSetArrayValue(file_name, \
			CONFIG_ALARM_LINK_LIST_PARAM, \
			str_item, \
			EM_CONFIG_ALARM_LINK_LIST_PARAM_ALM_TYPE_U32,
			itmp))
		{
			ERR_PRINT("alm_link_list item%d alm_type: %s failed\n", i, itmp==0 ? "Sensor":"Ipc");
			return -FAILURE;
		}		
		DBG_PRINT("alm_link_list item%d alm_type: %s\n", i, itmp==0 ? "Sensor":"Ipc");

		//nvr alm chn
		itmp = valm_link_list[i].alm_chn;
		
		if(0 != PublicIniSetArrayValue(file_name, \
			CONFIG_ALARM_LINK_LIST_PARAM, \
			str_item, \
			EM_CONFIG_ALARM_LINK_LIST_PARAM_ALM_CHN_U32,
			itmp))
		{
			ERR_PRINT("alm_link_list item%d alm_chn: %d failed\n", i, itmp);
			return -FAILURE;
		}
		DBG_PRINT("alm_link_list item%d alm_chn: %d\n", i, itmp);

		//dec ip
		itmp = valm_link_list[i].decip;
		in.s_addr = itmp;
		
		if(0 != PublicIniSetArrayValue(file_name, \
			CONFIG_ALARM_LINK_LIST_PARAM, \
			str_item, \
			EM_CONFIG_ALARM_LINK_LIST_PARAM_DECIP_U32,
			itmp))
		{
			ERR_PRINT("alm_link_list item%d dec ip: %s failed\n", i, inet_ntoa(in));
			return -FAILURE;
		}
		DBG_PRINT("alm_link_list item%d dec ip: %s\n", i, inet_ntoa(in));

		//dec chn
		itmp = valm_link_list[i].dec_chn;
		
		if(0 != PublicIniSetArrayValue(file_name, \
			CONFIG_ALARM_LINK_LIST_PARAM, \
			str_item, \
			EM_CONFIG_ALARM_LINK_LIST_PARAM_DEC_CHN_U32,
			itmp))
		{
			ERR_PRINT("alm_link_list item%d dec_chn: %d failed\n", i, itmp);
			return -FAILURE;
		}
		DBG_PRINT("alm_link_list item%d dec_chn: %d\n", i, itmp);
	}
	
	return SUCCESS;
}

////加载配置文件到参数
int CBizConfig::LoadUserCfgParamFromFile(SConfigAllParam *psall_param, const char *file_name)
{
	DBG_PRINT("Load psall_param from %s\n", file_name);
	
	//从配置文件加载时间参数
	if (GetTimeParamFromFile(psall_param->stime_param, file_name))
	{
		ERR_PRINT("GetTimeParamFromFile failed\n");
		return -FAILURE;
	}
	
	//从配置文件加载网络参数
	if (GetNetParamFromFile(psall_param->snet_param, file_name))
	{
		ERR_PRINT("GetNetParamFromFile failed\n");
		return -FAILURE;
	}
	
	//从配置文件加载电视墙参数
	if (GetTvWallParamFromFile(psall_param->vtvwall_list, file_name))
	{
		ERR_PRINT("GetTvWallParamFromFile failed\n");
		return -FAILURE;
	}
	
	//从配置文件加载NVR 列表
	if (GetDevListFromFile(EM_NVR, psall_param->vnvr_list, file_name))
	{
		ERR_PRINT("GetNvrListFromFile failed\n");
		return -FAILURE;
	}
	
	//从配置文件加载patrol dec 列表
	if (GetDevListFromFile(EM_PATROL_DEC, psall_param->vpatrol_dec_list, file_name))
	{
		ERR_PRINT("GetPatrolDecListFromFile failed\n");
		return -FAILURE;
	}
	
	//从配置文件加载switch dec 列表
	if (GetDevListFromFile(EM_SWITCH_DEC, psall_param->vswitch_dec_list, file_name))
	{
		ERR_PRINT("GetSwitchDecListFromFile failed\n");
		return -FAILURE;
	}
	
	//从配置文件加载报警联动列表
	if (GetAlmLinkParamListFromFile(psall_param->valm_link_list, file_name))
	{
		ERR_PRINT("GetAlmLinkParamListFromFile failed\n");
		return -FAILURE;
	}
	
	return SUCCESS;
}

//给threadWriteFlash 发消息
int CBizConfig::WriteMsg2Thread(EM_MSG_TYPE msg_type)
{
	/*
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}
	*/
	if (msg_type >=  EM_MSG_TYPE_MAX)
	{
		ERR_PRINT("msg_type(%d) >=  EM_MSG_TYPE_MAX(%d)\n", msg_type, EM_MSG_TYPE_MAX);
		return -FAILURE;
	}

	if (pthread_mutex_lock(&cond_mutex))
	{
		ERR_PRINT("cond_mutex lock failed\n");
		return -FAILURE;
	}

	if (pmsg_queue->Put((uchar *)&msg_type, sizeof(EM_MSG_TYPE)))
	{
		ERR_PRINT("pmsg_queue Put failed\n");
		goto fail;
	}

	cond_msg_count++;

	if (pthread_cond_signal(&cond))
	{
		ERR_PRINT("pthread_cond_broadcast failed\n");
		goto fail;
	}

	pthread_mutex_unlock(&cond_mutex);
	return SUCCESS;

fail:
	pthread_mutex_unlock(&cond_mutex);
	return -FAILURE;
}

//写flash 线程
void CBizConfig::threadWriteFlash(uint param)
{
	EM_MSG_TYPE msg_type = EM_MSG_TYPE_MAX;
	int ret = -FAILURE;
	VD_BOOL b_process = FALSE;
	
	DBG_PRINT("CBizConfig::threadWriteFlash running\n");

	sleep(2);
	
	while (1)
	{
		b_process = FALSE;
		
		pthread_mutex_lock(&cond_mutex);
		if (cond_msg_count)	//有消息
		{
			ret = pmsg_queue->Get((uchar *)&msg_type, sizeof(EM_MSG_TYPE));
			if (ret != SUCCESS)
			{
				if (ret == -EDATA)
					ERR_PRINT("msg_queue is not enough valid data\n");
				else
					ERR_PRINT("msg_queue get data fail, ret: %d\n", ret);

				pthread_mutex_unlock(&cond_mutex);
				break;
			}

			cond_msg_count--;
			b_process = TRUE;			
		}
		else	//无消息
		{
			pthread_cond_wait(&cond, &cond_mutex);			
		}		
		pthread_mutex_unlock(&cond_mutex);		
		
		if (b_process)
		{
			//升级或外部需要暂停的情况
			if (EM_THREAD_PAUSE == msg_type)
			{
				b_pause = TRUE;
				b_process = FALSE;
				
				ERR_PRINT("get msg be pause*****************\n");
				
			}
			else if (EM_THREAD_RESUME == msg_type)
			{
				b_pause = FALSE;
				b_process = FALSE;
				
				ERR_PRINT("get msg be resume*****************\n");
			}
			
			if (b_pause)
			{
				continue;
			}
			
			//处理
			if (plock4param->Wrlock())
			{
				ERR_PRINT("Wrlock failed\n");
				break;
			}
			
			switch (msg_type)
			{
				case EM_BIZCONFIG_TIME_DATE_PARAM:
					{
						printf("%s process EM_BIZCONFIG_TIME_DATE_PARAM\n", __func__);
						if (SetTimeParam2File(psall_param->stime_param))//失败
						{
							SetTimeParam2File(psall_param_default->stime_param);
						}
						
					} break;
				case EM_BIZCONFIG_NET_PARAM:
					{
						printf("%s process EM_BIZCONFIG_NET_PARAM\n", __func__);
						if (SetNetParam2File(psall_param->snet_param))//失败
						{
							SetNetParam2File(psall_param_default->snet_param);
						}
						
					} break;
				case EM_BIZCONFIG_TVWALL_LIST:
					{
						printf("%s process EM_BIZCONFIG_TVWALL_LIST\n", __func__);
						if (SetTvWallParam2File(psall_param->vtvwall_list))//失败
						{
							SetTvWallParam2File(psall_param_default->vtvwall_list);
						}
						
					} break;
				case EM_BIZCONFIG_NVR_LIST:
					{
						printf("%s process EM_BIZCONFIG_NVR_LIST\n", __func__);
						if (SetDevList2File(EM_NVR, psall_param->vnvr_list))//失败
						{
							SetDevList2File(EM_NVR, psall_param_default->vnvr_list);
						}
					} break;
				case EM_BIZCONFIG_PATROL_DEC_LIST:
					{
						printf("%s process EM_BIZCONFIG_PATROL_DEC_LIST\n", __func__);
						if (SetDevList2File(EM_PATROL_DEC, psall_param->vpatrol_dec_list))//失败
						{
							SetDevList2File(EM_PATROL_DEC, psall_param_default->vpatrol_dec_list);
						}
						
					} break;
				case EM_BIZCONFIG_SWITCH_DEC_LIST:
					{
						printf("%s process EM_BIZCONFIG_SWITCH_DEC_LIST\n", __func__);
						if (SetDevList2File(EM_SWITCH_DEC, psall_param->vswitch_dec_list))//失败
						{
							SetDevList2File(EM_SWITCH_DEC, psall_param_default->vswitch_dec_list);
						}
						
					} break;
				case EM_BIZCONFIG_ALARM_LINK_LIST:
					{
						printf("%s process EM_BIZCONFIG_ALARM_LINK_LIST\n", __func__);
						if (SetAlmLinkParamList2File(psall_param->valm_link_list))//失败
						{
							SetAlmLinkParamList2File(psall_param_default->valm_link_list);
						}
					} break;
				case EM_BIZCONFIG_ALL_FILE:
					{
						printf("%s process EM_BIZCONFIG_ALL_FILE\n", __func__);
						
					} break;
				
				//上面已经处理
				case EM_THREAD_PAUSE:
				case EM_THREAD_RESUME:
					break;
				default:
					ERR_PRINT("msg type%d, not support*****************\n", msg_type);
			}

			if (plock4param->Unlock())
			{
				ERR_PRINT("Unlock failed\n");
				break;
			}

			if (b_process) // write file to flash
			{
				if (UpgradeUserCfgFile2Flash())
				{
					ERR_PRINT("UpgradeUserCfgFile2Flash failed\n");
					break;
				}
			}
		}
	}

	ERR_PRINT("CBizConfig::threadWriteFlash exit**********************\n");
}
//获取默认参数
int CBizConfig::GetDefaultTimeParam(SConfigTimeParam &stimme_param)
{
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (plock4param->Rdlock())
	{
		ERR_PRINT("Rdlock failed\n");
		return -FAILURE;
	}

	stimme_param = psall_param_default->stime_param;

	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}
	
	return SUCCESS;
}

int CBizConfig::GetDefaultNetParam(SConfigNetParam &snet_param)
{
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (plock4param->Rdlock())
	{
		ERR_PRINT("Rdlock failed\n");
		return -FAILURE;
	}
	
	snet_param = psall_param_default->snet_param;

	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}
	
	return SUCCESS;
}	

//获取参数
int CBizConfig::GetDvrProperty(SConfigDvrProperty &sdev_property)
{
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}
	
	if (plock4param->Rdlock())
	{
		ERR_PRINT("Rdlock failed\n");
		return -FAILURE;
	}
	
	sdev_property = *pdev_property;

	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}
	
	return SUCCESS;
}
	
int CBizConfig::GetTimeParam(SConfigTimeParam &stimme_param)
{
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (plock4param->Rdlock())
	{
		ERR_PRINT("Rdlock failed\n");
		return -FAILURE;
	}
	
	stimme_param = psall_param->stime_param;

	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}
	
	return SUCCESS;
}
	
int CBizConfig::GetNetParam(SConfigNetParam &snet_param)
{
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (plock4param->Rdlock())
	{
		ERR_PRINT("Rdlock failed\n");
		return -FAILURE;
	}

	snet_param = psall_param->snet_param;

	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}
	
	return SUCCESS;
}
	

int CBizConfig::GetTvWallList(std::vector<SConfigTvWallParam> &vtvwall_list)
{
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (plock4param->Rdlock())
	{
		ERR_PRINT("Rdlock failed\n");
		return -FAILURE;
	}

	vtvwall_list = psall_param->vtvwall_list;

	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}
	
	return SUCCESS;
}

int CBizConfig::GetDevList(EM_DEV_TYPE dev_type, std::vector<uint> &vdev_list)
{
	std::vector<uint> *pvdev_list = NULL;
	
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (plock4param->Rdlock())
	{
		ERR_PRINT("Rdlock failed\n");
		return -FAILURE;
	}

	switch (dev_type)
	{
		case EM_NVR:
			pvdev_list = &psall_param->vnvr_list;
			break;
			
		case EM_PATROL_DEC:
			pvdev_list = &psall_param->vpatrol_dec_list;
			break;
			
		case EM_SWITCH_DEC:
			pvdev_list = &psall_param->vswitch_dec_list;
			break;

		default:
			ERR_PRINT("dev_type%d exception\n", dev_type);
			goto fail;
	}
	
	vdev_list = *pvdev_list;

	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}
	
	return SUCCESS;
fail:
	
	plock4param->Unlock();
	return -FAILURE;	
}

int CBizConfig::GetAlmLinkList(std::vector<SConfigAlmLinkParam> &valm_link_list)
{
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (plock4param->Rdlock())
	{
		ERR_PRINT("Rdlock failed\n");
		return -FAILURE;
	}

	valm_link_list = psall_param->valm_link_list;

	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}
	
	return SUCCESS;
}

//设置参数
int CBizConfig::SetTimeParam(SConfigTimeParam &stimme_param)
{
	VD_BOOL b_save = FALSE;
	
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}
	
	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock failed\n");
		return -FAILURE;
	}

	if (psall_param->stime_param != stimme_param)
	{
		psall_param->stime_param = stimme_param;
		b_save = TRUE;
	}
	
	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}

	if (b_save)
	{
		if (WriteMsg2Thread(EM_BIZCONFIG_TIME_DATE_PARAM))
		{
			ERR_PRINT("WriteMsg2Thread failed\n");
			return -FAILURE;
		}
	}
	
	return SUCCESS;
}

int CBizConfig::SetNetParam(SConfigNetParam &snet_param)
{
	VD_BOOL b_save = FALSE;
	
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock failed\n");
		return -FAILURE;
	}

	if (psall_param->snet_param != snet_param)
	{
		psall_param->snet_param = snet_param;
		b_save = TRUE;		
	}
	
	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}

	if (b_save)
	{
		if (WriteMsg2Thread(EM_BIZCONFIG_NET_PARAM))
		{
			ERR_PRINT("WriteMsg2Thread failed\n");
			return -FAILURE;
		}
	}
	
	return SUCCESS;
}

//添加、删除、修改
//删除时的参数是被删除项的索引
int CBizConfig::AddTvWallParamList(std::vector<SConfigTvWallParam> &vtvwall_list)
{
	VD_BOOL b_save = FALSE;
	std::vector<SConfigTvWallParam>::iterator iter;
	
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (vtvwall_list.empty())
	{
		ERR_PRINT("param list empty\n");
		return -FAILURE;
	}

	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock failed\n");
		return -FAILURE;
	}
	
	if (psall_param->vtvwall_list.size() + vtvwall_list.size() > pdev_property->nTvwall_nums)
	{
		ERR_PRINT("too mang tvwall lists(%d+%d), limit: %d\n", \
			psall_param->vtvwall_list.size(), \
			vtvwall_list.size(), \
			pdev_property->nTvwall_nums);
		
		goto fail;
	}
	
	for (iter = vtvwall_list.begin(); \
		iter != vtvwall_list.end();	\
		++iter)
	{
		if (iter->devlist.size() != iter->ndevs_per_line * iter->ndevs_per_col)
		{
			ERR_PRINT("devlist size(%d) != ndevs_per_line(%d) * ndevs_per_col(%d)\n", \
				iter->devlist.size(), \
				iter->ndevs_per_line, \
				iter->ndevs_per_col);
			
			goto fail;
		}
		
		if (iter->ndevs_per_line > pdev_property->nTvwall_devs_per_line)
		{
			ERR_PRINT("too mang devs per line %d, limit: %d\n", \
				iter->ndevs_per_line, \
				pdev_property->nTvwall_devs_per_line);
			
			goto fail;
		}
		
		if (iter->ndevs_per_col > pdev_property->nTvwall_devs_per_col)
		{
			ERR_PRINT("too mang devs per col %d, limit: %d\n", \
				iter->ndevs_per_col, \
				pdev_property->nTvwall_devs_per_col);
			
			goto fail;
		}
		
		if (iter->devlist.size() > pdev_property->nTvwall_devs_max)
		{
			ERR_PRINT("tvwall has too mang devs %d, limit: %d\n", \
				iter->devlist.size(), \
				pdev_property->nTvwall_devs_max);
			
			goto fail;
		}

		//当前不在列表内
		if (psall_param->vtvwall_list.end() == \
			std::find(psall_param->vtvwall_list.begin(), psall_param->vtvwall_list.end(), *iter))
		{
			psall_param->vtvwall_list.push_back(*iter);
			b_save = TRUE;
		}
	}	
	
	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return FALSE;
	}

	if (b_save)
	{
		if (WriteMsg2Thread(EM_BIZCONFIG_TVWALL_LIST))
		{
			ERR_PRINT("WriteMsg2Thread failed\n");
			return -FAILURE;
		}
	}
	
	return SUCCESS;

fail:
	
	plock4param->Unlock();
	return -FAILURE;
}

int CBizConfig::DelTvWallParamList(std::vector<SConfigTvWallParam> &vtvwall_list)
{
	VD_BOOL b_save = FALSE;
	std::vector<SConfigTvWallParam>::iterator iter, iter_del;
	
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (vtvwall_list.empty())
	{
		ERR_PRINT("param list empty\n");
		return SUCCESS;
	}

	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock failed\n");
		return -FAILURE;
	}
	
	for (iter = vtvwall_list.begin(); \
		iter != vtvwall_list.end();	\
		++iter)
	{
		if (iter->devlist.size() != iter->ndevs_per_line * iter->ndevs_per_col)
		{
			ERR_PRINT("devlist size(%d) != ndevs_per_line(%d) * ndevs_per_col(%d)\n", \
				iter->devlist.size(), \
				iter->ndevs_per_line, \
				iter->ndevs_per_col);
			
			goto fail;
		}
		
		iter_del = std::find(psall_param->vtvwall_list.begin(), psall_param->vtvwall_list.end(), *iter);
		if (psall_param->vtvwall_list.end() != iter_del)
		{
			psall_param->vtvwall_list.erase(iter_del);
			b_save = TRUE;
		}
	}	
	
	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}

	if (b_save)
	{
		if (WriteMsg2Thread(EM_BIZCONFIG_TVWALL_LIST))
		{
			ERR_PRINT("WriteMsg2Thread failed\n");
			return -FAILURE;
		}	
	}
	
	return SUCCESS;

fail:
	
	plock4param->Unlock();
	return -FAILURE;

}

int CBizConfig::DelAllTvWallParamList()
{
	VD_BOOL b_save = FALSE;
	
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}
	
	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock failed\n");
		return -FAILURE;
	}
	
	if (psall_param->vtvwall_list.empty())//空
	{
		DBG_PRINT("vtvwall_list empty\n");
	}
	else
	{
		psall_param->vtvwall_list.clear();
		b_save = TRUE;
	}
	
	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}

	if (b_save)
	{
		if (WriteMsg2Thread(EM_BIZCONFIG_TVWALL_LIST))
		{
			ERR_PRINT("WriteMsg2Thread failed\n");
			return -FAILURE;
		}	
	}
	
	return SUCCESS;
}


int CBizConfig::ModifyTvWallParam(u32 index, SConfigTvWallParam &stvwall_param)
{
	VD_BOOL b_save = FALSE;
	
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return FALSE;
	}

	if (stvwall_param.devlist.size() != stvwall_param.ndevs_per_line * stvwall_param.ndevs_per_col)
	{
		ERR_PRINT("devlist size(%d) != ndevs_per_line(%d) * ndevs_per_col(%d)\n", \
			stvwall_param.devlist.size(), \
			stvwall_param.ndevs_per_line, \
			stvwall_param.ndevs_per_col);
		
		return -FAILURE;
	}

	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock failed\n");
		return -FAILURE;
	}
	
	if (index >= pdev_property->nTvwall_nums)
	{
		ERR_PRINT("index %d >= limit: %d\n", \
			index, \
			pdev_property->nTvwall_nums);
		
		goto fail;
	}	
		
	if (stvwall_param.ndevs_per_line > pdev_property->nTvwall_devs_per_line)
	{
		ERR_PRINT("too mang devs per line %d, limit: %d\n", \
			stvwall_param.ndevs_per_line, \
			pdev_property->nTvwall_devs_per_line);
		
		goto fail;
	}
	
	if (stvwall_param.ndevs_per_col > pdev_property->nTvwall_devs_per_col)
	{
		ERR_PRINT("too mang devs per col %d, limit: %d\n", \
			stvwall_param.ndevs_per_col, \
			pdev_property->nTvwall_devs_per_col);
		
		goto fail;
	}
	
	if (stvwall_param.devlist.size() > pdev_property->nTvwall_devs_max)
	{
		ERR_PRINT("tvwall has too mang devs %d, limit: %d\n", \
			stvwall_param.devlist.size(), \
			pdev_property->nTvwall_devs_max);
		
		goto fail;
	}

	if (psall_param->vtvwall_list[index] != stvwall_param)
	{
		psall_param->vtvwall_list[index] = stvwall_param;
		b_save = TRUE;
	}
	else
	{
		DBG_PRINT("same index %d\n", index);
	}
	
	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}

	if (b_save)
	{
		if (WriteMsg2Thread(EM_BIZCONFIG_TVWALL_LIST))
		{
			ERR_PRINT("WriteMsg2Thread failed\n");
			return FALSE;
		}	
	}
	
	return SUCCESS;

fail:
	
	plock4param->Unlock();
	return -FAILURE;
}

int CBizConfig::AddAlmLinkParamList(std::vector<SConfigAlmLinkParam> &valm_link_list)
{
	VD_BOOL b_save = FALSE;
	std::vector<SConfigAlmLinkParam>::iterator iter;
	
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (valm_link_list.empty())
	{
		ERR_PRINT("param list empty\n");
		return -FAILURE;
	}

	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock failed\n");
		return -FAILURE;
	}

	//限制值是NVR 的数量
	if (psall_param->valm_link_list.size() + valm_link_list.size() > pdev_property->nNvr_nums)
	{
		ERR_PRINT("too mang almlink items(%d+%d), limit: %d\n", \
			psall_param->valm_link_list.size(), \
			valm_link_list.size(), \
			pdev_property->nNvr_nums);
		
		goto fail;
	}
		
	for (iter = valm_link_list.begin(); \
		iter != valm_link_list.end();	\
		++iter)
	{	
		//当前不在列表内
		if (psall_param->valm_link_list.end() == \
			std::find(psall_param->valm_link_list.begin(), psall_param->valm_link_list.end(), *iter))
		{
			psall_param->valm_link_list.push_back(*iter);
			b_save = TRUE;
		}
	}	
	
	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}

	if (b_save)
	{
		if (WriteMsg2Thread(EM_BIZCONFIG_ALARM_LINK_LIST))
		{
			ERR_PRINT("WriteMsg2Thread failed\n");
			return -FAILURE;
		}
	}
	
	return SUCCESS;

fail:
	
	plock4param->Unlock();
	return -FAILURE;
}

int CBizConfig::DelAlmLinkParamList(std::vector<SConfigAlmLinkParam> &valm_link_list)
{
	VD_BOOL b_save = FALSE;
	std::vector<SConfigAlmLinkParam>::iterator iter, iter_del;
	
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (valm_link_list.empty())
	{
		ERR_PRINT("param list empty\n");
		return -FAILURE;
	}

	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock failed\n");
		return -FAILURE;
	}
	
	for (iter = valm_link_list.begin(); \
		iter != valm_link_list.end();	\
		++iter)
	{
		iter_del = std::find(psall_param->valm_link_list.begin(), psall_param->valm_link_list.end(), *iter);
			
		if (psall_param->valm_link_list.end() != iter_del)
		{
			psall_param->valm_link_list.erase(iter_del);
			b_save = TRUE;
		}		
	}	
	
	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}

	if (b_save)
	{
		if (WriteMsg2Thread(EM_BIZCONFIG_ALARM_LINK_LIST))
		{
			ERR_PRINT("WriteMsg2Thread failed\n");
			return -FAILURE;
		}	
	}
	
	return SUCCESS;
}

int CBizConfig::DelAllAlmLinkParamList()
{
	VD_BOOL b_save = FALSE;
	
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}
	
	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock failed\n");
		return -FAILURE;
	}
	
	if (psall_param->valm_link_list.empty())//空
	{
		DBG_PRINT("vtvwall_list empty\n");
	}
	else
	{
		psall_param->valm_link_list.clear();
		b_save = TRUE;
	}
	
	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}

	if (b_save)
	{
		if (WriteMsg2Thread(EM_BIZCONFIG_ALARM_LINK_LIST))
		{
			ERR_PRINT("WriteMsg2Thread failed\n");
			return -FAILURE;
		}	
	}
	
	return SUCCESS;
}

int CBizConfig::ModifyAlmLinkParam(u32 index, SConfigAlmLinkParam &salm_link_param)
{
	VD_BOOL b_save = FALSE;
	
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock failed\n");
		return -FAILURE;
	}
	
	if (index >= pdev_property->nNvr_nums)
	{
		ERR_PRINT("index %d >= limit: %d\n", \
			index, \
			pdev_property->nNvr_nums);
		
		goto fail;
	}

	if (psall_param->valm_link_list[index] != salm_link_param)
	{
		psall_param->valm_link_list[index] = salm_link_param;
		b_save = TRUE;
	}
	
	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}

	if (b_save)
	{
		if (WriteMsg2Thread(EM_BIZCONFIG_ALARM_LINK_LIST))
		{
			ERR_PRINT("WriteMsg2Thread failed\n");
			return -FAILURE;
		}	
	}	
	
	return SUCCESS;

fail:
	
	plock4param->Unlock();
	return -FAILURE;
}

//按由小到大顺序插入
int CBizConfig::AddDevList(EM_DEV_TYPE dev_type, std::vector<uint> &vdev_list_add)//dev IP list
{
	std::vector<uint>::iterator iter, iter_add;
	std::vector<uint> *pvdev_list = NULL;
	struct in_addr in;
	EM_MSG_TYPE msg_type = EM_MSG_TYPE_MAX;
	VD_BOOL b_save = FALSE;
	
	
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (dev_type >= EM_DEV_TYPE_MAX)
	{
		ERR_PRINT("param dev_type%d >= EM_DEV_TYPE_MAX%d\n", \
			dev_type, EM_DEV_TYPE_MAX);
		
		return -FAILURE;
	}
	
	if (vdev_list_add.empty())
	{
		ERR_PRINT("param list empty\n");
		return SUCCESS;
	}

	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock failed\n");
		return -FAILURE;
	}

	switch (dev_type)
	{
		case EM_NVR:
			pvdev_list = &psall_param->vnvr_list;
			msg_type = EM_BIZCONFIG_NVR_LIST;
			break;
			
		case EM_PATROL_DEC:
			pvdev_list = &psall_param->vpatrol_dec_list;
			msg_type = EM_BIZCONFIG_PATROL_DEC_LIST;
			break;
			
		case EM_SWITCH_DEC:
			pvdev_list = &psall_param->vswitch_dec_list;
			msg_type = EM_BIZCONFIG_SWITCH_DEC_LIST;
			break;

		default:
			ERR_PRINT("dev_type%d exception\n", dev_type);
			pvdev_list = NULL;
			msg_type = EM_MSG_TYPE_MAX;
			goto fail;
	}

	for (iter_add = vdev_list_add.begin(); \
		iter_add != vdev_list_add.end();	\
		++iter_add)
	{
		//空
		if (pvdev_list->empty())
		{
			pvdev_list->push_back(*iter_add);
			b_save = TRUE;
			continue;
		}
		//已经存在
		if (pvdev_list->end() != std::find(pvdev_list->begin(), pvdev_list->end(), *iter_add))
		{
			in.s_addr = *iter_add;
			
			DBG_PRINT("dev ip: %s has been existing in list\n", inet_ntoa(in));
			continue;
		}
			
		for (iter = pvdev_list->begin(); \
			iter != pvdev_list->end();	\
			++iter)
		{
			if (lessIP_u32(*iter_add, *iter))//*iter_add < *iter  TRUE
				break;
		}

		pvdev_list->insert(iter, *iter_add);
		b_save = TRUE;
	}

	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}
	
	if (b_save)
	{
		if (WriteMsg2Thread(msg_type))
		{
			ERR_PRINT("WriteMsg2Thread failed\n");
			return -FAILURE;
		}	
	}
	
	return SUCCESS; 

fail:
	
	plock4param->Unlock();
	return -FAILURE;
}

int CBizConfig::DelDevList(EM_DEV_TYPE dev_type, std::vector<uint> &vdev_list_del)
{
	std::vector<uint>::iterator iter, iter_del, iter_find;
	std::vector<uint> *pvdev_list = NULL;
	
	EM_MSG_TYPE msg_type = EM_MSG_TYPE_MAX;
	VD_BOOL b_save = FALSE;
	struct in_addr in;
	
	
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (dev_type >= EM_DEV_TYPE_MAX)
	{
		ERR_PRINT("param dev_type%d >= EM_DEV_TYPE_MAX%d\n", \
			dev_type, EM_DEV_TYPE_MAX);
		
		return -FAILURE;
	}
	
	if (vdev_list_del.empty())
	{
		ERR_PRINT("param list empty\n");
		return SUCCESS;
	}

	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock failed\n");
		return -FAILURE;
	}

	switch (dev_type)
	{
		case EM_NVR:
			pvdev_list = &psall_param->vnvr_list;
			msg_type = EM_BIZCONFIG_NVR_LIST;
			break;
			
		case EM_PATROL_DEC:
			pvdev_list = &psall_param->vpatrol_dec_list;
			msg_type = EM_BIZCONFIG_PATROL_DEC_LIST;
			break;
			
		case EM_SWITCH_DEC:
			pvdev_list = &psall_param->vswitch_dec_list;
			msg_type = EM_BIZCONFIG_SWITCH_DEC_LIST;
			break;

		default:
			ERR_PRINT("dev_type%d exception\n", dev_type);
			pvdev_list = NULL;
			msg_type = EM_MSG_TYPE_MAX;
			goto fail;
	}

	//空
	if (pvdev_list->empty())
	{
		plock4param->Unlock();
		DBG_PRINT("dev_type:%d list empty\n", dev_type);
		return SUCCESS;
	}

	for (iter_del = vdev_list_del.begin(); \
		iter_del != vdev_list_del.end();	\
		++iter_del)
	{
		in.s_addr = *iter_del;
		iter_find = std::find(pvdev_list->begin(), pvdev_list->end(), *iter_del);
		
		//存在
		if (pvdev_list->end() != iter_find)
		{
			pvdev_list->erase(iter_find);
			b_save = TRUE;
			DBG_PRINT("list delete dev, ip: %s\n", inet_ntoa(in));			
		}
		else //不存在
		{
			DBG_PRINT("dev ip: %s not find in list\n", inet_ntoa(in));	
		}
	}

	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}
	
	if (b_save)
	{
		if (WriteMsg2Thread(msg_type))
		{
			ERR_PRINT("WriteMsg2Thread failed\n");
			return -FAILURE;
		}	
	}
	
	return SUCCESS; 

fail:
	
	plock4param->Unlock();
	return -FAILURE;
}

int CBizConfig::DelAllDevList(EM_DEV_TYPE dev_type)
{
	VD_BOOL b_save = FALSE;
	std::vector<uint> *pvdev_list = NULL;
	EM_MSG_TYPE msg_type = EM_MSG_TYPE_MAX;
	
	if (!b_inited)
	{
		ERR_PRINT("CBizConfig not init\n");
		return -FAILURE;
	}

	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock failed\n");
		return -FAILURE;
	}
	
	switch (dev_type)
	{
		case EM_NVR:
			pvdev_list = &psall_param->vnvr_list;
			msg_type = EM_BIZCONFIG_NVR_LIST;
			break;
			
		case EM_PATROL_DEC:
			pvdev_list = &psall_param->vpatrol_dec_list;
			msg_type = EM_BIZCONFIG_PATROL_DEC_LIST;
			break;
			
		case EM_SWITCH_DEC:
			pvdev_list = &psall_param->vswitch_dec_list;
			msg_type = EM_BIZCONFIG_SWITCH_DEC_LIST;
			break;

		default:
			ERR_PRINT("dev_type%d exception\n", dev_type);
			pvdev_list = NULL;
			msg_type = EM_MSG_TYPE_MAX;
			goto fail;
	}

	//空
	if (pvdev_list->empty())
	{
		plock4param->Unlock();
		DBG_PRINT("dev_type:%d list empty\n", dev_type);
		return SUCCESS;
	}
	//清空
	pvdev_list->clear();
	b_save = TRUE;

	if (plock4param->Unlock())
	{
		ERR_PRINT("Unlock failed\n");
		return -FAILURE;
	}
	
	if (b_save)
	{
		if (!WriteMsg2Thread(msg_type))
		{
			ERR_PRINT("WriteMsg2Thread failed\n");
			return -FAILURE;
		}	
	}

	return SUCCESS;

fail:
	
	plock4param->Unlock();
	return -FAILURE;

}


/*****************************************************
外部接口实现
*****************************************************/
int BizConfigInit(void)
{
	return g_biz_config.Init();
}

//获取默认参数
int BizConfigGetDefaultTimeParam(SConfigTimeParam &stimme_param)
{
	return g_biz_config.GetDefaultTimeParam(stimme_param);
}

int BizConfigGetDefaultNetParam(SConfigNetParam &snet_param)
{
	return g_biz_config.GetDefaultNetParam(snet_param);
}

//获取参数
int BizConfigGetDevInfo(ifly_DeviceInfo_t *pdev_info)
{
	SConfigNetParam snet_param;

	if (NULL == pdev_info)
	{
		ERR_PRINT("BizConfigGetDevInfo param failed\n");
		return -EPARAM;
	}

	//net init	
	if (BizConfigGetNetParam(snet_param))
	{
		ERR_PRINT("BizConfigGetNetParam failed\n");
		return -FAILURE;
	}

	memset(pdev_info, 0, sizeof(ifly_DeviceInfo_t));
	
	pdev_info->deviceIP = snet_param.nhost_ip;
	pdev_info->devicePort = snet_param.nsvr_port;
	
	strcpy(pdev_info->device_name, "NetworkKeyboard");
	pdev_info->nNVROrDecoder = 4;

	return SUCCESS;
}

int BizConfigGetDvrProperty(SConfigDvrProperty &sdev_property)
{
	return g_biz_config.GetDvrProperty(sdev_property);
}

int BizConfigGetTimeParam(SConfigTimeParam &stimme_param)
{
	return g_biz_config.GetTimeParam(stimme_param);
}

int BizConfigGetNetParam(SConfigNetParam &snet_param)
{
	return g_biz_config.GetNetParam(snet_param);
}

int BizConfigGetTvWallList(std::vector<SConfigTvWallParam> &vtvwall_list)
{
	return g_biz_config.GetTvWallList(vtvwall_list);
}

int BizConfigGetNvrList(std::vector<uint> &vdev_list)
{
	return g_biz_config.GetDevList(EM_NVR, vdev_list);
}

int BizConfigGetPatrolDecList(std::vector<uint> &vdev_list)
{
	return g_biz_config.GetDevList(EM_PATROL_DEC, vdev_list);
}

int BizConfigGetSwitchDecList(std::vector<uint> &vdev_list)
{
	return g_biz_config.GetDevList(EM_SWITCH_DEC, vdev_list);
}

int BizConfigGetAlmLinkList(std::vector<SConfigAlmLinkParam> &valm_link_list)
{
	return g_biz_config.GetAlmLinkList(valm_link_list);
}

//设置参数
int BizConfigSetTimeParam(SConfigTimeParam &stimme_param)
{
	return g_biz_config.SetTimeParam(stimme_param);
}

int BizConfigSetNetParam(SConfigNetParam &snet_param)
{
	return g_biz_config.SetNetParam(snet_param);
}

//添加、删除、修改
//删除时的参数是被删除项的索引
int BizConfigAddTvWallParam(SConfigTvWallParam &tvwall_param)
{
	std::vector<SConfigTvWallParam> vtvwall_list;
	vtvwall_list.push_back(tvwall_param);
	
	return g_biz_config.AddTvWallParamList(vtvwall_list);
}

int BizConfigAddTvWallParamList(std::vector<SConfigTvWallParam> &vtvwall_list)
{
	return g_biz_config.AddTvWallParamList(vtvwall_list);
}

int BizConfigDelTvWallParam(SConfigTvWallParam &tvwall_param)
{
	std::vector<SConfigTvWallParam> vtvwall_list;
	vtvwall_list.push_back(tvwall_param);
	
	return g_biz_config.DelTvWallParamList(vtvwall_list);
}

int BizConfigDelTvWallParamList(std::vector<SConfigTvWallParam> &vtvwall_list)//删除一个或多个表索引
{
	return g_biz_config.DelTvWallParamList(vtvwall_list);
}

int BizConfigDelAllTvWallParamList()
{
	return g_biz_config.DelAllTvWallParamList();
}

int BizConfigModifyTvWallParam(u32 index, SConfigTvWallParam &stvwall_param)	
{
	return g_biz_config.ModifyTvWallParam(index, stvwall_param);
}

int BizConfigAddAlmLinkParam(SConfigAlmLinkParam &alm_link_param)
{
	std::vector<SConfigAlmLinkParam> valm_link_list;
	valm_link_list.push_back(alm_link_param);

	return g_biz_config.AddAlmLinkParamList(valm_link_list);
}

int BizConfigAddAlmLinkParamList(std::vector<SConfigAlmLinkParam> &valm_link_list)	
{
	return g_biz_config.AddAlmLinkParamList(valm_link_list);
}

int BizConfigDelAlmLinkParam(SConfigAlmLinkParam &alm_link_param)
{
	std::vector<SConfigAlmLinkParam> valm_link_list;
	valm_link_list.push_back(alm_link_param);

	return g_biz_config.DelAlmLinkParamList(valm_link_list);
}

int BizConfigDelAlmLinkParamList(std::vector<SConfigAlmLinkParam> &valm_link_list)//删除一个或多个表索引	
{
	return g_biz_config.DelAlmLinkParamList(valm_link_list);
}

int BizConfigDelAllAlmLinkParamList()	
{
	return g_biz_config.DelAllAlmLinkParamList();
}

int BizConfigModifyAlmLinkParam(u32 index, SConfigAlmLinkParam &salm_link_param)	
{
	return g_biz_config.ModifyAlmLinkParam(index, salm_link_param);
}

int BizConfigAddNvr(uint dev_ip)//nvr IP list	
{
	std::vector<uint> vdev_list;
	vdev_list.push_back(dev_ip);
	
	return g_biz_config.AddDevList(EM_NVR, vdev_list);
}

int BizConfigAddNvrList(std::vector<uint> &vdev_list)//nvr IP list	
{
	return g_biz_config.AddDevList(EM_NVR, vdev_list);
}

int BizConfigDelNvr(uint dev_ip)//nvr IP list	
{
	std::vector<uint> vdev_list;
	vdev_list.push_back(dev_ip);
	
	return g_biz_config.DelDevList(EM_NVR, vdev_list);
}

int BizConfigDelNvrList(std::vector<uint> &vdev_list)
{
	return g_biz_config.DelDevList(EM_NVR, vdev_list);
}

int BizConfigDelAllNvrList()
{
	return g_biz_config.DelAllDevList(EM_NVR);
}

int BizConfigAddPatrolDec(uint dev_ip)//nvr IP list	
{
	std::vector<uint> vdev_list;
	vdev_list.push_back(dev_ip);
	
	return g_biz_config.AddDevList(EM_PATROL_DEC, vdev_list);
}

int BizConfigAddPatrolDecList(std::vector<uint> &vdev_list)	
{
	return g_biz_config.AddDevList(EM_PATROL_DEC, vdev_list);
}

int BizConfigDelPatrolDec(uint dev_ip)//nvr IP list	
{
	std::vector<uint> vdev_list;
	vdev_list.push_back(dev_ip);
	
	return g_biz_config.DelDevList(EM_PATROL_DEC, vdev_list);
}

int BizConfigDelPatrolDecList(std::vector<uint> &vdev_list)	
{
	return g_biz_config.DelDevList(EM_PATROL_DEC, vdev_list);
}

int BizConfigAllDelPatrolDecList()
{
	return g_biz_config.DelAllDevList(EM_PATROL_DEC);
}

int BizConfigAddSwitchDec(uint dev_ip)//nvr IP list	
{
	std::vector<uint> vdev_list;
	vdev_list.push_back(dev_ip);
	
	return g_biz_config.AddDevList(EM_SWITCH_DEC, vdev_list);
}

int BizConfigAddSwitchDecList(std::vector<uint> &vdev_list)	
{
	return g_biz_config.AddDevList(EM_SWITCH_DEC, vdev_list);
}

int BizConfigDelSwitchDec(uint dev_ip)//nvr IP list	
{
	std::vector<uint> vdev_list;
	vdev_list.push_back(dev_ip);
	
	return g_biz_config.DelDevList(EM_SWITCH_DEC, vdev_list);
}

int BizConfigDelSwitchDecList(std::vector<uint> &vdev_list)	
{
	return g_biz_config.DelDevList(EM_SWITCH_DEC, vdev_list);
}

int BizConfigDelAllSwitchDecList()
{
	return g_biz_config.DelAllDevList(EM_SWITCH_DEC);
}



