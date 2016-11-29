

#include "api.h"
#include "singleton.h"
#include "object.h"
#include "glb_error_num.h"
#include "biz.h"
#include "biz_types.h"
#include "biz_config.h"
#include "biz_net.h"
#include "biz_device.h"
#include "atomic.h"
#include "c_lock.h"
#include "crwlock.h"
#include "ccircular_buffer.h"
#include "public.h"
#include "flash.h"
#include "cthread.h"
#include "ctimer.h"
#include "ctrlprotocol.h"
#include "net.h"
#include "bond.h"


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


#include <map>
#include <set>
//#include <bitset>
#include <algorithm>
#include <utility>

#define SendBuf (1024)
#define RcvBuf (4096)
#define DIALOGUE_TIMEOUT	(3) //一次命令回话超时
#define RECONNECT_TIMEOUT	(5) //秒
#define KEEP_ALIVE_INTERVAL	(15) //秒

#ifdef __cplusplus
extern "C" {
#endif

//搜索回调
void SearchDevice_CB(ifly_DeviceInfo_t dev, void* pContext);

#ifdef __cplusplus
}
#endif


#define g_biz_device_manager (*CBizDeviceManager::instance())


//设备IP地址和设备类指针
//MAP_IP_IDX 中IP地址需要ntohl  操作，好排序
//<u32 IP_le, u32 pool_index> 设备池中的索引
typedef std::map<u32, s32> MAP_IP_IDX;

//<s32 socket fd, u32 pool_index>
typedef std::map<s32, s32> MAP_FD_IDX;


class CBizDeviceManager : public CObject
{
	friend class CBizDevice;
	
public:
	PATTERN_SINGLETON_DECLARE(CBizDeviceManager);
	~CBizDeviceManager();
	int GetDevIPList(EM_DEV_TYPE dev_type, std::list<u32> &dev_ip_list);//网络字节序
    int GetDevIdx(EM_DEV_TYPE dev_type, u32 dev_ip);
	int GetDevInfo(EM_DEV_TYPE dev_type, u32 dev_ip, SGuiDev *pdev);
	int StartNotifyDevInfo();//使能通知。设备层将信息通知给上层
	int AddDev(EM_DEV_TYPE dev_type, u32 dev_ip);
	int DelDev(EM_DEV_TYPE dev_type, u32 dev_ip);	
	int ReqStreamStart(s32 dev_idx, ifly_TCP_Stream_Req *preq, CMediaStream *pstream);
	int ReqStreamStop(s32 dev_idx, s32 stream_idx);
	//底层数据交互
	int NetDialogue(int sock, u16 event, const void *content, int length, void* ackbuf, int ackbuflen, int *realacklen);
	int Init();
	VD_BOOL isInited();
	
private:
	CBizDeviceManager();
	CBizDeviceManager(CBizDeviceManager &)
	{

	}
	void FreeSrc();//释放资源
	int _AddDev2Map(EM_DEV_TYPE dev_type, u32 dev_ip);//外部加锁
	void threadKeepAlive(uint param);//服务器保活
	void timerFuncReconnect(uint param);//服务器重连
	void threadRcv(uint param);// 1.接收服务器回应---2. 接收服务器报警
	//按是否在线, 分离添加的设备
	void _SplitDevFromMap(EM_DEV_TYPE dev_type, 
								std::list<s32> &list_devs_online, 
								std::list<s32> &list_devs_offline);
	int _UnlinkAllDevs();
	//add and del  map_fd_idx 需要接收网络信息的表
	int _AddMapRcv(s32 sock_fd, s32 dev_idx);
	int _DelMapRcv(s32 sock_fd);
	//device
	int _CleanDevSock(s32 dev_idx);
	u16 _IncAndGetSyncNum();
	u16 _GetSyncNum();
	
private:
	VD_BOOL b_inited;
	//VD_BOOL b_notify_devinfo;

	//设备管理
	//内层
	C_Lock *plock_dev_pool;//设备池
	C_Lock **pplock_dev;
	CBizDevice **ppcdev;
	std::vector<bool> pool_bits;//指示设备池中索引idx位置有没有被占用
	u32 pool_cur_idx;
	s32 pool_size;// max_link_devs *2 圆整为2的次幂	
	u32 pool_dev_count;

	//外层
	C_Lock *plock4param;//rwlock	
	u32 max_link_devs; //最大可连接设备数目(nvr + dec)
	std::set<u32> set_dev_ip; //保存设备IP的ntohl后的值，查询设备是否存在
	MAP_IP_IDX map_nvr; //已添加NVR
	MAP_IP_IDX map_patrol_dec; //已添加轮巡型DEC
	MAP_IP_IDX map_switch_dec; //已添加切换型DEC
	
	CTimer *m_reconnect_timer; //重连设备
	CSemaphore reconnect_sem; //threadKeepAlive 与timerFuncReconnect 同步
	Threadlet m_threadlet_keep_alive; // 1 处理待删除设备-- 2 处理已添加设备
	Threadlet m_threadlet_rcv; // 1 接收服务器回应-- 2 接收服务器报警-- 3 最终删除设备
	
	//待删除设备
	C_Lock *plock_list_del;//Mutex
	std::list<s32> list_dev_del;//待删除的设备列表	

	//集中接收设备消息
	C_Lock *plock_map_fd_idx;
	MAP_FD_IDX map_fd_idx;//m_threadlet_rcv 管理接收的设备socket 和index

	C_Lock *plock4sync_num;
	u16 sync_num;//网络回话序列号
	C_Lock *plock4sock;//Mutex 同时只允许跟一个服务器通信
	CSemaphore sync_sem; //socket 发送方同步等待接收服务器回应
	char *send_buf;
	char *sync_buf; //存储服务器回应数据
	u32 sync_ack_len; // 服务器回应数据长度
};

PATTERN_SINGLETON_IMPLEMENT(CBizDeviceManager);

/********************************************************************
							Device  Manager
********************************************************************/
CBizDeviceManager::CBizDeviceManager()
: b_inited(FALSE)
//, b_notify_devinfo(FALSE)
, plock_dev_pool(NULL)
, pplock_dev(NULL)
, ppcdev(NULL)
, pool_cur_idx(0)
, pool_size(0)
, pool_dev_count(0)
, plock4param(NULL)
, max_link_devs(0)
, m_reconnect_timer(NULL)
, reconnect_sem(0)
, plock_list_del(NULL)
, plock_map_fd_idx(NULL)
, plock4sync_num(NULL)
, sync_num(0)
, plock4sock(NULL)
, sync_sem(0)
, send_buf(NULL)
, sync_buf(NULL)
, sync_ack_len(0)
{
	
}

CBizDeviceManager::~CBizDeviceManager()
{
	FreeSrc();
}

int CBizDeviceManager::Init(void)
{
	int ret;
	std::vector<u32> dev_list;
	std::vector<u32>::iterator iter;
	SConfigDvrProperty sdev_property;
	u32 size = 0;
	u32 maxuint = 0;
	u32 andv = 0;
	s32 i;

	//最多可连接设备数目上限(nvr + dec)
	ret = BizConfigGetDvrProperty(sdev_property);
	if (ret)
	{
		ERR_PRINT("BizConfigGetDvrProperty failed\n");
		goto fail;
	}
	
	max_link_devs = sdev_property.nNvr_nums + sdev_property.nDec_nums;
	
#if 0 //debug
	struct in_addr in;
	MAP_IP_IDX::iterator map_iter;
#endif

	//pool_size = max_link_devs *2 圆整为2的次幂
	size = max_link_devs * 2;
	if((size & (size-1)) == 0)//是否为2的次幂
	{
		pool_size = size;
	}
	else
	{
		maxuint = (u32)((u32)~0);
		andv = ~(maxuint&(maxuint>>1));
		
		while((andv & size) == 0)
			andv = andv>>1;
		
		pool_size = (int)andv<<1;
	}

	plock_dev_pool = new CMutex;
	if (NULL == plock_dev_pool)
	{
		ERR_PRINT("new plock_dev_pool failed\n");
		goto fail;
	}

	if (plock_dev_pool->Create())
	{
		ERR_PRINT("create plock_dev_pool failed\n");
		goto fail;
	}
	
	pplock_dev = new C_Lock*[pool_size];
	if (NULL == pplock_dev)
	{
		ERR_PRINT("new pplock_dev failed\n");
		goto fail;
	}
	memset((void *)pplock_dev, 0, pool_size*sizeof(C_Lock *));

	pool_bits.resize(pool_size);
	
	for(i=0; i < pool_size; i++)
	{
		pplock_dev[i] = new CMutex;
		if (NULL == pplock_dev[i])
		{
			ERR_PRINT("new pplock_dev[%d] failed\n", i);
			goto fail;
		}
		
		if (pplock_dev[i]->Create())
		{
			ERR_PRINT("create pplock_dev[%d] failed\n", i);
			goto fail;
		}

		pool_bits[i] = 0;//set all bits to 0
	}

	ppcdev = new CBizDevice*[pool_size];
	if (NULL == ppcdev)
	{
		ERR_PRINT("new ppcdev failed\n");
		goto fail;
	}
	memset((void *)ppcdev, 0, pool_size*sizeof(CBizDevice *));	
	
	//rw lock
	plock4param = new CRWlock;
	if (NULL == plock4param)
	{
		ERR_PRINT("new plock4param failed\n");
		goto fail;
	}

	if (plock4param->Create())
	{
		ERR_PRINT("create plock4param failed\n");
		goto fail;
	}
	
	//填充DEV_MAP NVR
	dev_list.clear();
	ret = BizConfigGetNvrList(dev_list);
	if (ret)
	{
		ERR_PRINT("BizConfigGetNvrList failed\n");
		goto fail;
	}

	for (iter = dev_list.begin(); 
		iter != dev_list.end();
		++iter)
	{
		if (_AddDev2Map(EM_NVR, *iter))
		{
			ERR_PRINT("fillDevMap EM_NVR failed\n");
			goto fail;
		}
	}

#if 0 //debug
	if (!map_nvr.empty())
	{
		for (map_iter = map_nvr.begin();	\
		 	 map_iter != map_nvr.end();	\
			 ++map_iter)
		{
			in.s_addr = htonl(map_iter->first);
			printf("map nvr : %s\n", inet_ntoa(in));
		}
	}
#endif	

	//填充DEV_MAP PATROL_DEC
	dev_list.clear();
	ret = BizConfigGetPatrolDecList(dev_list);
	if (ret)
	{
		ERR_PRINT("BizConfigGetNvrList failed\n");
		goto fail;
	}

	for (iter = dev_list.begin();
		iter != dev_list.end();
		++iter)
	{
		if (_AddDev2Map(EM_PATROL_DEC, *iter))
		{
			ERR_PRINT("fillDevMap EM_PATROL_DEC failed\n");
			goto fail;
		}
	}

	//填充DEV_MAP SWITCH_DEC
	dev_list.clear();
	ret = BizConfigGetSwitchDecList(dev_list);
	if (ret)
	{
		ERR_PRINT("BizConfigGetNvrList failed\n");
		goto fail;
	}

	for (iter = dev_list.begin();
		iter != dev_list.end();
		++iter)
	{
		if (_AddDev2Map(EM_SWITCH_DEC, *iter))
		{
			ERR_PRINT("fillDevMap EM_SWITCH_DEC failed\n");
			goto fail;
		}
	}

	m_reconnect_timer = new CTimer("CBizDeviceManager-reconnect_timer");
	if (NULL == m_reconnect_timer)
	{
		ERR_PRINT("BizConfigGetNvrList failed\n");
		goto fail;
	}

	//待删除设备
	plock_list_del= new CMutex;
	if (NULL == plock_list_del)
	{
		ERR_PRINT("new plock_list_del failed\n");
		goto fail;
	}

	if (plock_list_del->Create())
	{
		ERR_PRINT("create plock_list_del failed\n");
		goto fail;
	}
	
	//集中接收设备消息
	plock_map_fd_idx = new CMutex;
	if (NULL == plock_map_fd_idx)
	{
		ERR_PRINT("new plock_map_fd_idx failed\n");
		goto fail;
	}

	if (plock_map_fd_idx->Create())
	{
		ERR_PRINT("create plock_map_fd_idx failed\n");
		goto fail;
	}
	
	plock4sync_num = new CMutex;
	if (NULL == plock4sync_num)
	{
		ERR_PRINT("new plock4sync_num failed\n");
		goto fail;
	}

	if (plock4sync_num->Create())
	{
		ERR_PRINT("create plock4sync_num failed\n");
		goto fail;
	}
	
	plock4sock = new CMutex;
	if (NULL == plock4sock)
	{
		ERR_PRINT("new plock4sock failed\n");
		goto fail;
	}

	if (plock4sock->Create())
	{
		ERR_PRINT("create plock4sock failed\n");
		goto fail;
	}

	send_buf = new char[SendBuf];
	if (NULL == send_buf)
	{
		ERR_PRINT("new send_buf failed\n");
		goto fail;
	}
	
	sync_buf = new char[RcvBuf];
	if (NULL == sync_buf)
	{
		ERR_PRINT("new sync_buf failed\n");
		goto fail;
	}
	
	//启动服务器接收线程	
	m_threadlet_rcv.run("BizDeviceManager-rcv", this, (ASYNPROC)&CBizDeviceManager::threadRcv, 0, 0);

	//启动线程: 保活
	m_threadlet_keep_alive.run("BizDeviceManager-keep_alive", this, (ASYNPROC)&CBizDeviceManager::threadKeepAlive, 0, 0);
	
	b_inited = TRUE;

	return SUCCESS;
	
fail:

	FreeSrc();
	return -FAILURE;
}

void CBizDeviceManager::FreeSrc()//释放资源
{	
	MAP_IP_IDX::iterator map_iter;
	std::vector<CBizDevice *>::iterator iter;
	std::list<ifly_cp_t *>list_iter;
	s32 i = 0;
	
	b_inited = FALSE;
	
	if (plock4param)
	{
		delete plock4param;
		plock4param = NULL;
	}

	//设备管理	
	//内层
	if (ppcdev)
	{
		delete []ppcdev;
		ppcdev = NULL;
	}

	if (pplock_dev)
	{
		for(i=0; i < pool_size; i++)
		{
			if (pplock_dev[i])
			{
				delete pplock_dev[i];
				pplock_dev[i] = NULL;
			}
		}

		delete []pplock_dev;
		pplock_dev = NULL;
	}
	
	if (plock_dev_pool)
	{
		delete plock_dev_pool;
		plock_dev_pool = NULL;
	}
	//外层
	max_link_devs = 0;
	set_dev_ip.clear();
	
	map_nvr.clear();
	map_patrol_dec.clear();
	map_switch_dec.clear();
	

	if (m_reconnect_timer)
	{
		delete m_reconnect_timer;
		m_reconnect_timer = NULL;
	}

	//待删除设备
	if (plock_list_del)
	{
		delete plock_list_del;
		plock_list_del = NULL;
	}

	list_dev_del.clear();

	//集中接收设备消息
	if (plock_map_fd_idx)
	{
		delete plock_map_fd_idx;
		plock_map_fd_idx = NULL;
	}
	
	if (plock4sync_num)
	{
		delete plock4sync_num;
		plock4sync_num = NULL;
	}
	
	if (plock4sock)
	{
		delete plock4sock;
		plock4sock = NULL;
	}

	if (send_buf)
	{
		delete []send_buf;
		send_buf = NULL;
	}
	
	if (sync_buf)
	{
		delete []sync_buf;
		sync_buf = NULL;
	}	
}

VD_BOOL CBizDeviceManager::isInited()
{
	return b_inited;
}

int CBizDeviceManager::GetDevIPList(EM_DEV_TYPE dev_type, std::list<u32> &dev_ip_list)
{
	MAP_IP_IDX *pmap = NULL;
	MAP_IP_IDX::iterator map_iter;
	int ret = -FAILURE;

	if (!dev_ip_list.empty())//非空
	{
		ERR_PRINT("param vdev_list is not empty\n");
		return -FAILURE;
	}

	if (!b_inited)
	{
		ERR_PRINT("module not init\n");
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
			pmap = &map_nvr;
			break;
			
		case EM_PATROL_DEC:
			pmap = &map_patrol_dec;
			break;
			
		case EM_SWITCH_DEC:
			pmap = &map_switch_dec;
			break;

		default:
			ERR_PRINT("dev_type%d exception\n", dev_type);
			ret = -EPARAM;
			goto fail;
	}

	for (map_iter = pmap->begin();
	 	 map_iter != pmap->end();
		 ++map_iter)
	{				
		dev_ip_list.push_back(ntohl(map_iter->first));
	}

    plock4param->Unlock();
	return SUCCESS;

fail:
	
	plock4param->Unlock();
	return ret;
}

int CBizDeviceManager::GetDevIdx(EM_DEV_TYPE dev_type, u32 dev_ip)
{
	u32 ip_le = ntohl(dev_ip);
	MAP_IP_IDX *pmap = NULL;
	MAP_IP_IDX::iterator map_iter;
	int ret = -FAILURE;
	int dev_idx = -1;
	
	if (!b_inited)
	{
		ERR_PRINT("module not init\n");
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
			pmap = &map_nvr;
			break;
			
		case EM_PATROL_DEC:
			pmap = &map_patrol_dec;
			break;
			
		case EM_SWITCH_DEC:
			pmap = &map_switch_dec;
			break;

		default:
			ERR_PRINT("dev_type%d exception\n", dev_type);
			ret = -EPARAM;
			goto fail;
	}

	map_iter = pmap->find(ip_le);
	if (map_iter == pmap->end())
	{
		ret = -EDEV_NOTFOUND;
		goto fail;
	}

	dev_idx = map_iter->second;

	plock4param->Unlock();
	return dev_idx;
	
fail:
	
	plock4param->Unlock();
	return ret;
}

int CBizDeviceManager::GetDevInfo(EM_DEV_TYPE dev_type, u32 dev_ip, SGuiDev *pgdev)
{
	MAP_IP_IDX *pmap = NULL;
	MAP_IP_IDX::iterator map_iter;
	CBizDevice *pcdev = NULL;
	std::string str_dev_type;
	s32 dev_idx;
	int ret = -FAILURE;
	u32 ip_le = ntohl(dev_ip);
	struct in_addr in;
	in.s_addr = dev_ip;

	if (!b_inited)
	{
		ERR_PRINT("module not init\n");
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
			pmap = &map_nvr;
			str_dev_type = "EM_NVR";
			break;
			
		case EM_PATROL_DEC:
			pmap = &map_patrol_dec;
			str_dev_type = "EM_PATROL_DEC";
			break;
			
		case EM_SWITCH_DEC:
			pmap = &map_switch_dec;
			str_dev_type = "EM_SWITCH_DEC";
			break;

		default:
			ERR_PRINT("dev_type%d exception\n", dev_type);
			ret = -EPARAM;
			goto fail;
	}

	map_iter = pmap->find(ip_le);
	if (map_iter == pmap->end())
	{
		ERR_PRINT("%s map not found IP(%s) failed\n", str_dev_type.c_str(), inet_ntoa(in));
		ret = -EDEV_NOTFOUND;
		goto fail;
	}

	dev_idx = map_iter->second;
	if ((dev_idx < 0) || (dev_idx >= pool_size))
	{
		ERR_PRINT("%s dev(%s) idx(%d) out of range, pool_size: %d\n", 
			str_dev_type.c_str(), inet_ntoa(in), dev_idx, pool_size);
		
		ret = -EDEV_NOTFOUND;
		goto fail;
	}

	pplock_dev[dev_idx]->Lock();
	
	pcdev = ppcdev[dev_idx];
	if (NULL == pcdev)
	{	
		ERR_PRINT("IP(%s) pdev == NULL\n", inet_ntoa(in));
		
		pplock_dev[dev_idx]->Unlock();
		ret = -EDEV_NOTFOUND;
		goto fail;		
	}

	pgdev->b_alive = pcdev->b_alive;
	pgdev->devicetype = pcdev->dev_info.devicetype;
	pgdev->deviceIP= pcdev->dev_info.deviceIP;
	pgdev->maxChnNum= pcdev->dev_info.maxChnNum;
	pgdev->dev_idx= pcdev->dev_idx;	

	pplock_dev[dev_idx]->Unlock();	
	plock4param->Unlock();
	return SUCCESS;

fail:
	
	plock4param->Unlock();
	return ret;

}

int CBizDeviceManager::StartNotifyDevInfo()//使能通知。设备层将信息通知给上层
{
	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock failed\n");
		return -FAILURE;
	}
	
	//b_notify_devinfo = TRUE;

	plock4param->Unlock();

	return SUCCESS;
}

int CBizDeviceManager::AddDev(EM_DEV_TYPE dev_type, u32 dev_ip)
{
	int ret = -FAILURE;
	u32 ip_le = ntohl(dev_ip);
	struct in_addr in;

	in.s_addr = dev_ip;
	
	if (!b_inited)
	{
		ERR_PRINT("module not init\n");
		return -FAILURE;
	}
	
	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock Wrlock\n");
		return -FAILURE;
	}

	//检查IP 冲突
	if (set_dev_ip.find(ip_le) != set_dev_ip.end())
	{
		ERR_PRINT("set IP(%s) conflict\n", inet_ntoa(in));
		goto done;
	}

	//设备数目上限
	if (set_dev_ip.size() >= max_link_devs)
	{
		ERR_PRINT("devs too mang, cur(%d), limit(%d)\n", set_dev_ip.size(), max_link_devs);
		ret = -EDEVS_LIMIT;
		goto fail;
	}

	ret = _AddDev2Map(dev_type, dev_ip);
	if (ret)
	{
		DBG_PRINT("_AddDev2Map IP(%s) failed\n", inet_ntoa(in));
		goto fail;
	}

	switch (dev_type)
	{
		case EM_NVR:
			ret = BizConfigAddNvr(dev_ip);
			if (ret)
			{
				DBG_PRINT("BizConfigAddNvr IP(%s) failed\n", inet_ntoa(in));
				goto fail;
			}
			break;
			
		case EM_PATROL_DEC:
			ret = BizConfigAddPatrolDec(dev_ip);
			if (ret)
			{
				DBG_PRINT("BizConfigAddPatrolDec IP(%s) failed\n", inet_ntoa(in));
				goto fail;
			}
			break;
			
		case EM_SWITCH_DEC:
			ret = BizConfigAddSwitchDec(dev_ip);
			if (ret)
			{
				DBG_PRINT("BizConfigAddSwitchDec IP(%s) failed\n", inet_ntoa(in));
				goto fail;
			}
			break;

		default:
			ERR_PRINT("dev_type%d exception\n", dev_type);
			ret = -EPARAM;
			goto fail;
	}
	
done:
	
	plock4param->Unlock();
	return SUCCESS;

fail:
	
	plock4param->Unlock();
	return ret;
}

//不是真正删除，只是放入待删除列表vdev_del_list
//真正的删除在threadKeepAlive 线程
int CBizDeviceManager::DelDev(EM_DEV_TYPE dev_type, u32 dev_ip)
{
	MAP_IP_IDX *pmap = NULL;
	MAP_IP_IDX::iterator map_iter;
	u32 ip_le = ntohl(dev_ip);
	struct in_addr in;
	int ret = -FAILURE;
	std::string str_dev_type;

	in.s_addr = dev_ip;
	
	if (!b_inited)
	{
		ERR_PRINT("module not init\n");
		return -FAILURE;
	}
	
	if (plock4param->Wrlock())
	{
		ERR_PRINT("Wrlock Wrlock\n");
		return -FAILURE;
	}

	//IP 不在set
	if (set_dev_ip.find(ip_le) == set_dev_ip.end())
	{
		DBG_PRINT("set IP(%s) not found\n", inet_ntoa(in));
		ret = -EDEV_NOTFOUND;
		goto done;
	}

	if (set_dev_ip.erase(ip_le) != 1)
	{
		ERR_PRINT("set erase IP(%s) failed\n", inet_ntoa(in));
		ret = -FAILURE;
		goto fail;
	}
	
	switch (dev_type)
	{
		case EM_NVR:
			pmap = &map_nvr;
			str_dev_type = "EM_NVR";
			break;
			
		case EM_PATROL_DEC:
			pmap = &map_patrol_dec;
			str_dev_type = "EM_PATROL_DEC";
			break;
			
		case EM_SWITCH_DEC:
			pmap = &map_switch_dec;
			str_dev_type = "EM_SWITCH_DEC";
			break;

		default:
			ERR_PRINT("dev_type%d exception\n", dev_type);
			ret = -EPARAM;
			goto fail;
	}

	map_iter = pmap->find(ip_le);
	if (map_iter == pmap->end())
	{
		ERR_PRINT("%s map not found IP(%s) failed\n", str_dev_type.c_str(), inet_ntoa(in));
		ret = -EDEV_NOTFOUND;
		goto fail;
	}

	//move dev to map_dev_del
	plock_list_del->Lock();
	list_dev_del.push_back(map_iter->second);
	plock_list_del->Unlock();
	
	pmap->erase(map_iter);
	
done:
	DBG_PRINT("%s map erase dev SUCCESS, IP(%s)\n", str_dev_type.c_str(), inet_ntoa(in));
	plock4param->Unlock();

	switch (dev_type)
	{
		case EM_NVR:
			ret = BizConfigDelNvr(dev_ip);
			if (ret)
			{
				DBG_PRINT("BizConfigDelNvr IP(%s) failed\n", inet_ntoa(in));
			}
			break;
			
		case EM_PATROL_DEC:
			ret = BizConfigDelPatrolDec(dev_ip);
			if (ret)
			{
				DBG_PRINT("BizConfigDelPatrolDec IP(%s) failed\n", inet_ntoa(in));
			}
			break;
			
		case EM_SWITCH_DEC:
			ret = BizConfigDelSwitchDec(dev_ip);
			if (ret)
			{
				DBG_PRINT("BizConfigDelSwitchDec IP(%s) failed\n", inet_ntoa(in));
			}
			break;

		default:
			ERR_PRINT("dev_type%d exception\n", dev_type);
	}
	
	return SUCCESS;

fail:
	plock4param->Unlock();
	return ret;
}

//成功返回stearm_rcv[MaxMediaLinks] 下标stream_idx
int CBizDeviceManager::ReqStreamStart(s32 dev_idx, ifly_TCP_Stream_Req *preq, CMediaStream *pstream)
{
	CBizDevice *pcdev = NULL;
	int stream_idx = -1;
	
	if ((dev_idx < 0) || (dev_idx >= pool_size))
	{
		DBG_PRINT("dev_idx(%d) out of range, pool_size: %d\n", dev_idx, pool_size);
		
		return -EPARAM;
	}		
	
	pplock_dev[dev_idx]->Lock();

	pcdev = ppcdev[dev_idx];
	if (NULL == pcdev)
	{	
		ERR_PRINT("pdev == NULL\n");

		pplock_dev[dev_idx]->Unlock();
		return -EDEV_NOTFOUND;
	}

	stream_idx = pcdev->StreamStart(preq, pstream);
	if (stream_idx < 0)
	{
		pplock_dev[dev_idx]->Unlock();
		return stream_idx;
	}

	//success
	pplock_dev[dev_idx]->Unlock();
	return stream_idx;
}

int CBizDeviceManager::ReqStreamStop(s32 dev_idx, s32 stream_idx)
{
	CBizDevice *pcdev = NULL;
	
	if ((dev_idx < 0) || (dev_idx >= pool_size))
	{
		DBG_PRINT("dev_idx(%d) out of range, pool_size: %d\n", dev_idx, pool_size);
		
		return -EPARAM;
	}		
	
	pplock_dev[dev_idx]->Lock();

	pcdev = ppcdev[dev_idx];
	if (NULL == pcdev)
	{	
		ERR_PRINT("pdev == NULL\n");

		pplock_dev[dev_idx]->Unlock();
		return -EDEV_NOTFOUND;
	}

	pcdev->StreamStop(stream_idx);
	pplock_dev[dev_idx]->Unlock();
	
	return SUCCESS;
}


//底层数据交互
int CBizDeviceManager::NetDialogue(int sock, u16 event, const void *content, int length, void* ackbuf, int ackbuflen, int *realacklen)
{
	//u8 abyBuf[1024];
	int ret = SUCCESS;
	int msglen = 0;
	int rcvlen = 0;
	u16 number = 0;
	ifly_cp_header_t cp_head_snd;
	ifly_cp_header_t cp_head_rcv;
	
	if(sock < 0)
	{
		DBG_PRINT("sock invalid\n");
		return -EPARAM;
	}
	
	if(length < 0)
	{
		DBG_PRINT("length: %d invalid\n", length);
		return -EPARAM;
	}
	if(NULL == ackbuf)
	{
		DBG_PRINT("ackbuf invalid\n");
		return -EPARAM;
	}
	if(NULL == realacklen)
	{
		DBG_PRINT("realacklen invalid\n");
		return -EPARAM;
	}
	//全局回话锁
	plock4sock->Lock();

	//发送
	msglen = sizeof(cp_head_snd)+length;
	memset(&cp_head_snd, 0, sizeof(cp_head_snd));
	cp_head_snd.length = htonl(msglen);
	cp_head_snd.type = htons(CTRL_COMMAND);
	cp_head_snd.event = htons(event);
	
	number = _IncAndGetSyncNum();
	//printf("%s snd number1: %d\n", , number);
	
	cp_head_snd.number = htons(number);
	cp_head_snd.version = htons(CTRL_VERSION);
	if(msglen > SendBuf)
	{
		DBG_PRINT("cp_head_snd.length(%d) > SendBuf, failed\n", msglen);
		
		plock4sock->Unlock();
		return -EPARAM;
	}
	
	memcpy(send_buf, &cp_head_snd, sizeof(cp_head_snd));

	if (content && length)
	{
		memcpy(send_buf+sizeof(cp_head_snd), content, length);
	}

	sync_sem.TryPend();//防止上一条消息的应答

	ret = loopsend(sock, (char *)send_buf, msglen);
	if(ret < 0)
	{
		ERR_PRINT("loopsend failed\n");

		plock4sock->Unlock();
		return -FAILURE;
	}
	//printf("%s snd number2: %d\n", , number);
	
	//等待接收返回
	ret = sync_sem.TimedPend(DIALOGUE_TIMEOUT);		
	if (ret)//timeout or failed
	{
		ERR_PRINT("sem_timedwait failed, error:[%d,%s], failed!\n", 
					errno, strerror(errno));
		
		plock4sock->Unlock();
		return -FAILURE;
	}

	if (sync_ack_len < (int)sizeof(ifly_cp_header_t))
	{
		ERR_PRINT("sync_ack_len < sizeof(ifly_cp_header_t), failed\n");
		
		plock4sock->Unlock();
		return -FAILURE;
	}

	memcpy(&cp_head_rcv, sync_buf, sizeof(ifly_cp_header_t));
	if (cp_head_rcv.event != CTRL_SUCCESS)
	{
		ERR_PRINT("cp_head_rcv.event: %d, failed\n", cp_head_rcv.event);
		
		plock4sock->Unlock();
		return -FAILURE;
	}

	rcvlen = sync_ack_len - sizeof(ifly_cp_header_t);
	
	if(realacklen != NULL) 
	{
		*realacklen = rcvlen;
	}
	
	if(rcvlen > 0)
	{
		if (ackbuf == NULL)
		{
			ERR_PRINT("ackbuf == NULL, failed\n");
		
			plock4sock->Unlock();
			return -FAILURE;
		}
		
		if(ackbuflen < rcvlen)
		{
			ERR_PRINT("ackbuflen(%d) < rcvlen(%d), failed\n", ackbuflen, rcvlen);
			
			plock4sock->Unlock();
			return -FAILURE;
		}
		
		memcpy(ackbuf, sync_buf+sizeof(ifly_cp_header_t), rcvlen);
	}

	plock4sock->Unlock();
	return SUCCESS;
}


//外部加锁
int CBizDeviceManager::_AddDev2Map(EM_DEV_TYPE dev_type, u32 dev_ip)
{
	MAP_IP_IDX *pmap = NULL;
	CBizDevice *pcdev = NULL;
	std::string str_dev_type;
	int ret = -FAILURE;
	u32 ip_le = ntohl(dev_ip);	//ntohl(ip) 小端格式，map 好排序
	struct in_addr in;
	s32 i = 0;
	u32 tmp_idx = 0;
	u32 pre_idx = 0;
	
	in.s_addr = dev_ip;
	
	//检查IP 冲突
	if (set_dev_ip.find(ip_le) != set_dev_ip.end())
	{
		ERR_PRINT("set IP(%s) conflict\n", inet_ntoa(in));
		return SUCCESS;
	}

	//init CBizDevice
	pcdev = NULL;
	pcdev = new CBizDevice;
	if (NULL == pcdev)
	{
		ERR_PRINT("%s new CBizDevice failed\n", str_dev_type.c_str());
		return -FAILURE;
	}

	if (pcdev->Init())
	{
		ERR_PRINT("%s CBizDevice init failed\n", str_dev_type.c_str());
		ret = -FAILURE;
		goto fail;
	}

	pcdev->dev_info.devicetype = dev_type;
	pcdev->dev_info.deviceIP = dev_ip;
	pcdev->dev_info.devicePort = CTRL_PROTOCOL_SERVERPORT;

	switch (dev_type)
	{
		case EM_NVR:
			pmap = &map_nvr;
			str_dev_type = "EM_NVR";
			break;
			
		case EM_PATROL_DEC:
			pmap = &map_patrol_dec;
			str_dev_type = "EM_PATROL_DEC";
			break;
			
		case EM_SWITCH_DEC:
			pmap = &map_switch_dec;
			str_dev_type = "EM_SWITCH_DEC";
			break;

		default:
			ERR_PRINT("dev_type%d exception\n", dev_type);
			ret = -EPARAM;
			goto fail;
	}
	
	//加入设备池
	plock_dev_pool->Lock();
	pre_idx = pool_cur_idx;
	
	//从当前位置开始找到下一个为使用的索引
	for (i = pool_cur_idx+1;
		tmp_idx = i & (pool_size-1), tmp_idx != pool_cur_idx;
		++i)
	{
		if (!pool_bits[tmp_idx])//false 当前位置未使用
		{
			break;
		}
	}

	if (tmp_idx == pool_cur_idx)
	{
		ERR_PRINT("%s dev pool full, count: %d\n", str_dev_type.c_str(), pool_dev_count);
		
		plock_dev_pool->Lock();
		ret = -FAILURE;
		goto fail;
	}

	pcdev->dev_idx = tmp_idx;//设备在设备池中的索引

	pplock_dev[tmp_idx]->Lock();
	ppcdev[tmp_idx] = pcdev;
	pplock_dev[tmp_idx]->Unlock();
	
	pool_bits[tmp_idx] = 1;
	pool_cur_idx = tmp_idx;
	++pool_dev_count;
	
	plock_dev_pool->Unlock();
	
	
	//insert map
	if (!pmap->insert(std::make_pair(ip_le, tmp_idx)).second)
	{
		ERR_PRINT("%s map insert failed, IP(%s) conflict\n", str_dev_type.c_str(), inet_ntoa(in));
		ret = -FAILURE;
		goto fail2;
	}

	//insert set
	if (!set_dev_ip.insert(ip_le).second)
	{
		ERR_PRINT("%s set insert failed, IP(%s) conflict\n", str_dev_type.c_str(), inet_ntoa(in));
		pmap->erase(ip_le);
		
		ret = -FAILURE;
		goto fail2;
	}

	DBG_PRINT("svr(%s) add success, dev_idx: %d\n", inet_ntoa(in), tmp_idx);

	return SUCCESS;
	
fail2:
	plock_dev_pool->Lock();
	
	pplock_dev[tmp_idx]->Lock();
	ppcdev[tmp_idx] = NULL;
	pplock_dev[tmp_idx]->Unlock();
	
	pool_bits[tmp_idx] = 0;
	pool_cur_idx = pre_idx;
	--pool_dev_count;
	
	plock_dev_pool->Unlock();
	
fail:
	
	delete pcdev;
	pcdev = NULL;
	
	return ret;
}

//按是否在线, 分离添加的设备
void CBizDeviceManager::_SplitDevFromMap(EM_DEV_TYPE dev_type, 
							std::list<s32> &list_devs_online, 
							std::list<s32> &list_devs_offline)
{
	MAP_IP_IDX *pmap = NULL;
	MAP_IP_IDX::iterator map_iter;
	CBizDevice *pcdev = NULL;
	std::string str_dev_type;
	s32 dev_idx;
	
	struct in_addr in;
	
	switch (dev_type)
	{
		case EM_NVR:
			pmap = &map_nvr;
			str_dev_type = "EM_NVR";
			break;
			
		case EM_PATROL_DEC:
			pmap = &map_patrol_dec;
			str_dev_type = "EM_PATROL_DEC";
			break;
			
		case EM_SWITCH_DEC:
			pmap = &map_switch_dec;
			str_dev_type = "EM_SWITCH_DEC";
			break;

		default:
			ERR_PRINT("dev_type%d exception\n", dev_type);
			return;
	}
	
	plock4param->Rdlock();
	
	for (map_iter = pmap->begin();
		map_iter != pmap->end();
		++map_iter)
	{
		in.s_addr = htonl(map_iter->first);
		dev_idx = map_iter->second;

		if ((dev_idx < 0) || (dev_idx >= pool_size))
		{
			ERR_PRINT("%s dev(%s) idx(%d) out of range, pool_size: %d\n", 
				str_dev_type.c_str(), inet_ntoa(in), dev_idx, pool_size);
			
			continue;
		}		
		
		pplock_dev[dev_idx]->Lock();

		pcdev = ppcdev[dev_idx];
		if (pcdev)
		{	
			if (pcdev->b_alive)
			{
				//DBG_PRINT("device online, IP(%s)\n", inet_ntoa(in));
				list_devs_online.push_back(dev_idx);
			}
			else
			{
				//DBG_PRINT("device offline, IP(%s)\n", inet_ntoa(in));
				list_devs_offline.push_back(dev_idx);
			}
		}
		else
		{
			ERR_PRINT("IP(%s) pdev == NULL\n", inet_ntoa(in));
		}

		pplock_dev[dev_idx]->Unlock();		
	}
	
	plock4param->Unlock();
}

int CBizDeviceManager::_UnlinkAllDevs()
{
	return SUCCESS;
}

//add and del  map_fd_idx 需要接收网络信息的表
int CBizDeviceManager::_AddMapRcv(s32 sock_fd, s32 dev_idx)
{
	plock_map_fd_idx->Lock();
	if (!map_fd_idx.insert(std::make_pair(sock_fd, dev_idx)).second)
	{
		ERR_PRINT("map_fd_idx insert failed, fd_tmp(%d) conflict\n", sock_fd);
		plock_map_fd_idx->Unlock();
		
		return -FAILURE;
	}
	
	plock_map_fd_idx->Unlock();
	return SUCCESS;
}

int CBizDeviceManager::_DelMapRcv(s32 sock_fd)
{
	MAP_FD_IDX::iterator map_iter;
	
	plock_map_fd_idx->Lock();
	
	map_iter = map_fd_idx.find(sock_fd);
	if (map_iter == map_fd_idx.end())
	{
		plock_map_fd_idx->Unlock();
		
		return -FAILURE;
	}

	map_fd_idx.erase(map_iter);
	
	plock_map_fd_idx->Unlock();
	return SUCCESS;
}


// 1 接收服务器回应
// 2 接收服务器报警
// 3 最终删除设备
void CBizDeviceManager::threadRcv(uint param)
{
	std::list<s32> list_fd;
	std::list<s32>::iterator list_iter;
	MAP_FD_IDX::iterator map_iter;
	CBizDevice *pcdev = NULL;
	s32 dev_idx;
	u32 deviceIP;
	
	fd_set rset;
	int fd_max = INVALID_SOCKET;
	int fd_tmp = INVALID_SOCKET;
	struct timeval timeout;
	//struct in_addr in;

	char *rcv_buf = NULL;
	int ret = -FAILURE;
	int msglen = 0;
	ifly_cp_header_t cprcvhead;
	
	DBG_PRINT("CBizDeviceManager::threadRcv running\n");

	rcv_buf = new char[RcvBuf];
	if (NULL == rcv_buf)
	{
		ERR_PRINT("new recv buffer failed\n");
		goto fail;
	}
	
	while(1)
	{
		list_fd.clear();		
		FD_ZERO(&rset);
		fd_max = INVALID_SOCKET;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		
		plock_map_fd_idx->Lock();
		
		for (map_iter = map_fd_idx.begin();
			map_iter != map_fd_idx.end();
			++map_iter)
		{
			fd_tmp = map_iter->first;

			if (INVALID_SOCKET != fd_tmp)
			{
				FD_SET(fd_tmp, &rset);
				fd_max = MAX(fd_tmp, fd_max);
				list_fd.push_back(fd_tmp);
			}
		}
				
		plock_map_fd_idx->Unlock();

		if (INVALID_SOCKET == fd_max)
		{
			sleep(1);
			continue;
		}

		ret = select(fd_max+1, &rset, NULL, NULL, &timeout);
		if (ret < 0)
		{
			ERR_PRINT("select failed\n");
			sleep(1);
			continue;
		}
		else if (ret == 0)//timeout
		{
			continue;
		}

		for (list_iter = list_fd.begin();
			list_iter != list_fd.end();
			++list_iter)
		{
			fd_tmp = *list_iter;			

			if ((INVALID_SOCKET != fd_tmp)
				&& FD_ISSET(fd_tmp, &rset))
			{
				ret = looprecv(fd_tmp, rcv_buf, sizeof(ifly_cp_header_t));
				if (ret != SUCCESS)
				{
					DBG_PRINT("recv ifly_cp_header_t failed\n");

					plock_map_fd_idx->Lock();
					
					map_iter = map_fd_idx.find(fd_tmp);
					if (map_iter != map_fd_idx.end())
					{
						//移除接收map_fd_idx
						dev_idx = map_iter->second;
						map_fd_idx.erase(map_iter);
						
						plock_map_fd_idx->Unlock();
						
						_CleanDevSock(dev_idx);
					}
					else
					{
						plock_map_fd_idx->Unlock();
					}					
					
					continue;
				}

				memcpy(&cprcvhead, rcv_buf, sizeof(cprcvhead));
				cprcvhead.length	= ntohl(cprcvhead.length);
				cprcvhead.type		= ntohs(cprcvhead.type);
				cprcvhead.version	= ntohs(cprcvhead.version);
				cprcvhead.number	= ntohs(cprcvhead.number);
				cprcvhead.event		= ntohs(cprcvhead.event);
/*
				printf("recv msg: \n");
				printf("\t type: %d\n", cprcvhead.type);
				printf("\t event: %d\n", cprcvhead.event);
				printf("\t length: %d\n", cprcvhead.length);
	*/				
				if(cprcvhead.length < sizeof(ifly_cp_header_t) || cprcvhead.type > CTRL_ACK || cprcvhead.version != CTRL_VERSION)
				{
					DBG_PRINT("sock_cli ifly_cp_header_t param error, len: %d, type: %d, version: %d\n", 
						cprcvhead.length, cprcvhead.type, cprcvhead.version);					
					
					continue;
				}

				//有消息，目前只接收服务器回应
				if(cprcvhead.length > sizeof(ifly_cp_header_t))
				{
					if (cprcvhead.length > RcvBuf)//出错，超出缓存区
					{
						ERR_PRINT("recv msg too long(%d) OUTOFMEMORY\n", cprcvhead.length);

						int data_size = cprcvhead.length > sizeof(ifly_cp_header_t);
						int rcv_len = RcvBuf - (int)sizeof(ifly_cp_header_t);
						while (data_size > 0)
						{
							ret = looprecv(fd_tmp, rcv_buf+sizeof(ifly_cp_header_t), rcv_len);
							if (ret != SUCCESS)
							{
								ERR_PRINT("recv looprecv error:[%d,%s]!\n", errno, strerror(errno));
								break;
							}
							
							data_size -= rcv_len;
							rcv_len = data_size>(RcvBuf-(int)sizeof(ifly_cp_header_t)) ? (RcvBuf-(int)sizeof(ifly_cp_header_t)) : data_size;					
						}

						if (data_size == 0)//接收完成
						{
							DBG_PRINT("OUTOFMEMORY looprecv finish!\n");
						}
						else	//接收出错
						{
							plock_map_fd_idx->Lock();
							
							map_iter = map_fd_idx.find(fd_tmp);
							if (map_iter != map_fd_idx.end())
							{
								//移除接收map_fd_idx
								dev_idx = map_iter->second;
								map_fd_idx.erase(map_iter);
								
								plock_map_fd_idx->Unlock();
								
								_CleanDevSock(dev_idx);
							}
							else
							{
								plock_map_fd_idx->Unlock();
							}							
						}
						
						continue;
					}
					
					msglen = cprcvhead.length - sizeof(ifly_cp_header_t);

					ret = looprecv(fd_tmp, rcv_buf+sizeof(ifly_cp_header_t), msglen);
					if (ret != SUCCESS)
					{
						ERR_PRINT("recv failed\n");

						plock_map_fd_idx->Lock();
						
						map_iter = map_fd_idx.find(fd_tmp);
						if (map_iter != map_fd_idx.end())
						{
							//移除接收map_fd_idx
							dev_idx = map_iter->second;
							map_fd_idx.erase(map_iter);
								
							plock_map_fd_idx->Unlock();
							
							_CleanDevSock(dev_idx);
						}
						else
						{
							plock_map_fd_idx->Unlock();
						}
						
						continue;
					}
				}

				if (cprcvhead.type == CTRL_ACK)//收到服务器回应消息
				{
					if(cprcvhead.number == _GetSyncNum())
					{
						memcpy(sync_buf, rcv_buf, cprcvhead.length);
						sync_ack_len = cprcvhead.length;

						sync_sem.Post();
					}					
				}
				else if (cprcvhead.type == CTRL_NOTIFY)//服务器通知消息(报警、处理进度等)
				{
					//from fd >>> dev index >>> IP
					plock_map_fd_idx->Lock();
						
					map_iter = map_fd_idx.find(fd_tmp);
					if (map_iter == map_fd_idx.end())
					{
						plock_map_fd_idx->Unlock();
						continue;
					}
				
					dev_idx = map_iter->second;
					plock_map_fd_idx->Unlock();

					if ((dev_idx < 0) || (dev_idx >= pool_size))
					{
						DBG_PRINT("dev_idx(%d) out of range, pool_size: %d\n", dev_idx, pool_size);

						continue;
					}
					
					pplock_dev[dev_idx]->Lock();

					pcdev = ppcdev[dev_idx];
					if (NULL == pcdev)
					{
						pplock_dev[dev_idx]->Unlock();

						DBG_PRINT("dev_idx(%d) pcdev == NULL\n", dev_idx);
						continue;
					}

					deviceIP = pcdev->dev_info.deviceIP;
					
					pplock_dev[dev_idx]->Unlock();

					BizDealSvrNotify(deviceIP, 
							cprcvhead.event,
							rcv_buf + sizeof(ifly_cp_header_t), 
							cprcvhead.length - sizeof(ifly_cp_header_t));																				
				}
				else
				{
					ERR_PRINT("svr msg not support, msg type: %d, event: %d\n", 
								cprcvhead.type,
								cprcvhead.event);
				}
			}
		}
	}

fail:
	ERR_PRINT("CBizDeviceManager::threadRcv exit, inconceivable\n");

	if (rcv_buf)
	{
		delete []rcv_buf;
		rcv_buf = NULL;
	}
}

//重连
void CBizDeviceManager::timerFuncReconnect(uint param)
{
	std::list<s32> *plist_devs_offline = (std::list<s32> *)param;
	std::list<s32>::iterator list_iter;
	MAP_FD_IDX map_fd_dev;
	MAP_FD_IDX::iterator map_iter;	
	CBizDevice *pcdev = NULL;
	GuiDev gdev;
	s32 dev_idx;
	
	struct sockaddr_in svr_addr;
	struct in_addr in;
	int fd_tmp = INVALID_SOCKET;
	int ret = 0;

	fd_set wset, set_raw;
	int fd_max = INVALID_SOCKET;
	struct timeval timeout;

	u64 pre_tick, cur_tick;

	FD_ZERO(&wset);
	FD_ZERO(&set_raw);

	DBG_PRINT("CBizDeviceManager::timerFuncReconnect running\n");
	
	//对离线设备重连
	for (list_iter = plist_devs_offline->begin();
		list_iter != plist_devs_offline->end();
		++list_iter)
	{
		dev_idx = *list_iter;
		if ((dev_idx < 0) || (dev_idx >= pool_size))
		{
			DBG_PRINT("dev_idx(%d) out of range, pool_size: %d\n", dev_idx, pool_size);

			continue;
		}
	
		pplock_dev[dev_idx]->Lock();
		
		pcdev = ppcdev[dev_idx];
		if (NULL == pcdev)
		{
			pplock_dev[dev_idx]->Unlock();
			DBG_PRINT("dev_idx(%d) pcdev == NULL\n", dev_idx);

			continue;
		}

		in.s_addr = pcdev->dev_info.deviceIP;
		if (pcdev->b_alive)
		{
			DBG_PRINT("svr IP(%s) is alive, inconceivable\n", inet_ntoa(in));
			pplock_dev[dev_idx]->Unlock();
			
			continue;
		}		
		
		memset(&svr_addr, 0, sizeof(svr_addr));
		svr_addr.sin_family = AF_INET;
		svr_addr.sin_addr.s_addr = pcdev->dev_info.deviceIP;
		svr_addr.sin_port = htons(pcdev->dev_info.devicePort);		
		
		pplock_dev[dev_idx]->Unlock();

		fd_tmp = socket(AF_INET, SOCK_STREAM, 0);
		if (fd_tmp < 0)
		{
			DBG_PRINT("svr IP(%s) create socket failed, %s\n", inet_ntoa(in), strerror(errno));
			continue;
		}

		//设置套接口非阻塞 
		//多个服务器发起connect 后由 select 统一监听
		ret = fdSetNonblockFlag(fd_tmp);
		if (ret)
		{
			DBG_PRINT("svr IP(%s) fdSetNonblockFlag\n", inet_ntoa(in));
			close(fd_tmp);
			continue;
		}
		
		ret = connect(fd_tmp, (struct sockaddr *)&svr_addr, sizeof(struct sockaddr_in));
	#if 0	
		if (ret == 0)//成功
		{
			//去除O_NONBLOCK
			if (fdClrNonblockFlag(fd_tmp))
			{
				DBG_PRINT("svr IP(%s) fdClrNonblockFlag\n", inet_ntoa(in));
				close(fd_tmp);
				continue;
			}

			if (SetSockLinkOpt(fd_tmp))
			{				
				ERR_PRINT("svr IP(%s) SetSockLinkOpt failed, %s\n", inet_ntoa(in), strerror(errno));
				close(fd_tmp);
				continue;
			}
		/*
			if (SetSockIOTimeout(fd_tmp, CTRL_SOCKET_IO_TIMEOUT))
			{
				ERR_PRINT("svr IP(%s) SetSockIOTimeout failed, %s\n", inet_ntoa(in), strerror(errno));
				close(fd_tmp);
				continue;
			}		
		*/
			//加入接收map_fd_idx
			if (_AddMapRcv(fd_tmp, dev_idx))
			{
				ERR_PRINT("_AddMapRcv failed, fd_tmp(%d) conflict\n", fd_tmp);
				close(fd_tmp);
				continue;
			}			

			pplock_dev[dev_idx]->Lock();
			pcdev->sock_cmd = fd_tmp;

			if (pcdev->DevConnect())//连接服务器失败
			{
				pcdev->CleanSock();//close fd
				pplock_dev[dev_idx]->Unlock();
				
				//移除接收map_fd_idx
				_DelMapRcv(fd_tmp);
			}
			else //连接服务器成功
			{
				pcdev->b_alive = TRUE;
				pplock_dev[dev_idx]->Unlock();
				
				//通知设备在线
			}
			continue;
		}
	#endif
		//非阻塞调用没有立即返回成功，可能失败，可能正在处理
		//继续判断
		//if (errno != EINPROGRESS)//EINPROGRESS 表示系统正在处理连接
		if (ret < 0 && (errno != EINPROGRESS))
		{
			close(fd_tmp);
			continue;
		}

		//收集fd, 后面交由select 轮询connect 结果
		//去除O_NONBLOCK
		if (fdClrNonblockFlag(fd_tmp))
		{
			DBG_PRINT("svr IP(%s) fdClrNonblockFlag\n", inet_ntoa(in));
			close(fd_tmp);
			continue;
		}
		
		//insert map，用于在之后处理连接超时的设备
		if (!map_fd_dev.insert(std::make_pair(fd_tmp, dev_idx)).second)
		{
			ERR_PRINT("map_fd_dev insert failed, IP(%s) conflict\n", inet_ntoa(in));
			close(fd_tmp);
			continue;
		}

		FD_SET(fd_tmp, &set_raw);
		fd_max = MAX(fd_tmp, fd_max);
	}

	pre_tick = SystemGetMSCount64();
	do
	{
		cur_tick = SystemGetMSCount64();

		if (map_fd_dev.empty()) //所有服务器连接完成|| 没有服务器需要重连
		{
			DBG_PRINT("map_fd_dev empty, work done\n");
			break;
		}

		if (cur_tick - pre_tick > RECONNECT_TIMEOUT*1000) //5s
		{
			DBG_PRINT("timeout\n");
			break;
		}	

		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		wset = set_raw;

		ret = select(fd_max+1, NULL, &wset, NULL, NULL);
		if (ret < 0)
		{
			sleep(1);
			continue;
		}
		else if (ret == 0)
		{
			//timeout
			continue;
		}

		for (map_iter = map_fd_dev.begin();
			map_iter != map_fd_dev.end();
			/* 空 */)
		{
			fd_tmp = map_iter->first;
			dev_idx = map_iter->second;
			
			if ((INVALID_SOCKET != fd_tmp) && FD_ISSET(fd_tmp, &wset))
			{
				if (isSockIOErr(fd_tmp)) //socket 出错
				{
					close(fd_tmp);
					goto remove;
				}
				
				if (SetSockLinkOpt(fd_tmp))
				{				
					ERR_PRINT("SetSockLinkOpt failed, %s\n", strerror(errno));
					close(fd_tmp);
					goto remove;
				}
			#if 0
				if (SetSockIOTimeout(fd_tmp, CTRL_SOCKET_IO_TIMEOUT))
				{
					ERR_PRINT("SetSockIOTimeout failed, %s\n", strerror(errno));
					close(fd_tmp);
					goto remove;
				}
			#endif
				//加入接收map_fd_idx				
				if ((dev_idx < 0) || (dev_idx >= pool_size))
				{
					DBG_PRINT("dev_idx(%d) out of range, pool_size: %d\n", dev_idx, pool_size);
					
					close(fd_tmp);
					goto remove;
				}
				
				pcdev = ppcdev[dev_idx];
				if (NULL == pcdev)
				{
					DBG_PRINT("dev_idx(%d) pcdev == NULLs\n", dev_idx);

					close(fd_tmp);
					goto remove;
				}

				if (_AddMapRcv(fd_tmp, dev_idx))
				{
					ERR_PRINT("_AddMapRcv failed, fd_tmp(%d) conflict\n", fd_tmp);
					close(fd_tmp);
					goto remove;
				}
				
				pplock_dev[dev_idx]->Lock();
				
				pcdev->sock_cmd = fd_tmp;
				in.s_addr = pcdev->dev_info.deviceIP;

				if (pcdev->DevConnect())//连接服务器失败
				{
					pcdev->CleanSock();
					pplock_dev[dev_idx]->Unlock();
					
					//移除接收map_fd_idx
					_DelMapRcv(fd_tmp);

					DBG_PRINT("svr(%s) connect failed\n", inet_ntoa(in));
				}
				else //连接服务器成功
				{
					pcdev->b_alive = TRUE;
					pplock_dev[dev_idx]->Unlock();

					DBG_PRINT("svr(%s) connect success\n", inet_ntoa(in));
					
					//通知设备在线
					//if (b_notify_devinfo)
					{
						memset(&gdev, 0, sizeof(gdev));
						gdev.b_alive = pcdev->b_alive;
						gdev.devicetype = pcdev->dev_info.devicetype;
						gdev.deviceIP= pcdev->dev_info.deviceIP;
						gdev.maxChnNum= pcdev->dev_info.maxChnNum;
						gdev.dev_idx= pcdev->dev_idx;
						
						notifyDevInfo(&gdev);
					}
				}
				
remove:
				//成功或出错，都移除，剩下的即是超时的
				FD_CLR(fd_tmp, &set_raw);
				map_fd_dev.erase(map_iter++);
			}
			else
			{
				++map_iter;
			}
		}		
	} while(1);

	//还在map 中表示服务器连接超时
	if (!map_fd_dev.empty())
	{
		for (map_iter = map_fd_dev.begin();
			map_iter != map_fd_dev.end();
			++map_iter)
		{
			close(map_iter->first);
		}
	}

	reconnect_sem.Post();
}

// 1 处理待删除设备
// 2 保活
void CBizDeviceManager::threadKeepAlive(uint param)
{
	//list dev_idx
	std::list<s32> list_devs_online;
	std::list<s32> list_devs_offline;
	std::list<s32>::iterator list_iter;
	s32 dev_idx;
	CBizDevice *pcdev = NULL;
	GuiDev gdev;
	
	ifly_DeviceInfo_t device_info;
	struct in_addr in;
	int state = 0;//网络状态机
	VD_BOOL b_network_ok = TRUE;
	VD_BOOL b_wait_timer = FALSE;
	VD_BOOL b_dev_offline = FALSE;
	u64 pre_tick = 0;
	u64 cur_tick = 0;
	u64 keep_alive_pre_tick = 0;
	int fd_tmp = INVALID_SOCKET;
	
	DBG_PRINT("CBizDeviceManager::threadKeepAlive running\n");
	
	while(1)
	{	
		pre_tick = SystemGetMSCount64();
		
		//网络状态机，检查网络是否正常
		b_network_ok = BizisNetworkOk();
		switch (state)
		{
			case 0:	//发现异常则去初始化
				if (!b_network_ok)
				{
					state = 1;
					_UnlinkAllDevs();
				}
				break;
			case 1:	//等待网络正常
				if (b_network_ok)	//网络正常
				{
					state = 0;
				}
				break;
			default:				
				ERR_PRINT("state%d, module not support*****************\n", state);				
		}

		if (state)
		{
			sleep(1);
			continue;
		}
		
		// 1、处理待删除设备
		do
		{
			plock_list_del->Lock();
		
			list_iter = list_dev_del.begin();
			if (list_iter == list_dev_del.end())//删除干净了
			{
				plock_list_del->Unlock();
				break;
			}

			dev_idx = *list_iter;
			list_dev_del.erase(list_iter);
			
			plock_list_del->Unlock();

			if ((dev_idx < 0) || (dev_idx >= pool_size))
			{
				DBG_PRINT("dev_idx(%d) out of range, pool_size: %d\n", dev_idx, pool_size);

				continue;
			}

			//删除设备	
			pplock_dev[dev_idx]->Lock();
			
			pcdev = ppcdev[dev_idx];			
			
			if (NULL == pcdev)
			{
				DBG_PRINT("dev_idx(%d) pcdev == NULLs\n", dev_idx);
				pplock_dev[dev_idx]->Unlock();

				continue;
			}

			fd_tmp = pcdev->sock_cmd;

			//断开流连接
			pcdev->ShutdownStreamAll();
			
			//注销logout
			pcdev->DevDisconnect();
			
			//关闭cmd socket
			pcdev->CleanSock();
			
			pplock_dev[dev_idx]->Unlock();

			//移除接收map_fd_idx
			_DelMapRcv(fd_tmp);

			//重置该索引
			plock_dev_pool->Lock();

			delete ppcdev[dev_idx];
			ppcdev[dev_idx] = NULL;			
			pool_bits[dev_idx] = 0;
			--pool_dev_count;
			
			plock_dev_pool->Unlock();
			
		} while(1);

		//处理已添加的设备
		// 1、在线: 保活
		// 2、离线: 重连，提交给定时器函数
		//按是否在线，分离添加的设备
		list_devs_online.clear();
		list_devs_offline.clear();
		
		_SplitDevFromMap(EM_NVR, list_devs_online, list_devs_offline);
		_SplitDevFromMap(EM_PATROL_DEC, list_devs_online, list_devs_offline);
		_SplitDevFromMap(EM_SWITCH_DEC, list_devs_online, list_devs_offline);

		//提交离线设备列表给定时器函数重连
		b_wait_timer = FALSE;
		if (!list_devs_offline.empty())
		{
			b_wait_timer = TRUE;
			m_reconnect_timer->Start(this, (VD_TIMERPROC)&CBizDeviceManager::timerFuncReconnect, 0, 0, (u32)&list_devs_offline);
		}

		//对在线设备保活
		cur_tick = SystemGetMSCount64();

		if ((keep_alive_pre_tick == 0) || (cur_tick - keep_alive_pre_tick > KEEP_ALIVE_INTERVAL*1000))
		{
			keep_alive_pre_tick = cur_tick;
			
			for (list_iter = list_devs_online.begin();
				list_iter != list_devs_online.end();
				++list_iter)
			{
				dev_idx = *list_iter;

				if (dev_idx >= pool_size)
				{
					DBG_PRINT("dev_idx(%d) out of range, pool_size: %d\n", dev_idx, pool_size);

					continue;
				}

				//设备保活
				pplock_dev[dev_idx]->Lock();
				
				pcdev = ppcdev[dev_idx];
				in.s_addr = pcdev->dev_info.deviceIP;
				
				if (NULL == pcdev)
				{
					DBG_PRINT("svr(%s) dev_idx(%d) dev not exist\n", inet_ntoa(in), dev_idx);
					pplock_dev[dev_idx]->Unlock();

					continue;
				}
				
				if (pcdev->GetDeviceInfo(&device_info))//出错
				{
					DBG_PRINT("svr(%s) dev_idx(%d) offline\n", inet_ntoa(in), dev_idx);

					fd_tmp = pcdev->sock_cmd;
					pcdev->CleanSock();
					//通知设备离线
					b_dev_offline = TRUE;

					//移除接收map_fd_idx
					_DelMapRcv(fd_tmp);
				}
				else //成功，设备在线，如果需要，对流连接重连
				{
					//DBG_PRINT("svr(%s) dev_idx(%d) keep alive success\n", inet_ntoa(in), dev_idx);
					pcdev->CheckAndReconnectStream();
				}

				pplock_dev[dev_idx]->Unlock();


				//通知设备离线
				if (b_dev_offline)
				{
					b_dev_offline = FALSE;

					//if (b_notify_devinfo)
					{
						memset(&gdev, 0, sizeof(gdev));
						gdev.b_alive = pcdev->b_alive;
						gdev.devicetype = pcdev->dev_info.devicetype;
						gdev.deviceIP= pcdev->dev_info.deviceIP;
						gdev.maxChnNum= pcdev->dev_info.maxChnNum;
						gdev.dev_idx= pcdev->dev_idx;
						
						notifyDevInfo(&gdev);
					}
				}
			}
		}
		
		//等待定时器函数结束
		if (b_wait_timer)
		{
			reconnect_sem.Pend();
		}

		cur_tick = SystemGetMSCount64();
		if (cur_tick - pre_tick < 3*1000)
		{
			sleep(1);
		}
	}

	ERR_PRINT("CBizDeviceManager::threadKeepAlive exit, inconceivable\n");
}

int CBizDeviceManager::_CleanDevSock(s32 dev_idx)
{
	CBizDevice *pcdev = NULL;
	
	if ((dev_idx < 0) || (dev_idx >= pool_size))
	{
		DBG_PRINT("dev_idx(%d) out of range, pool_size: %d\n", dev_idx, pool_size);

		return -EPARAM;
	}

	pplock_dev[dev_idx]->Lock();
	
	pcdev = ppcdev[dev_idx];
	if (NULL == pcdev)
	{
		DBG_PRINT("dev_idx(%d) dev not exist\n", dev_idx);
		pplock_dev[dev_idx]->Unlock();

		return -EPARAM;
	}

	pcdev->CleanSock();
	
	pplock_dev[dev_idx]->Unlock();
	
	return SUCCESS;
}

u16 CBizDeviceManager::_IncAndGetSyncNum()
{
	u16 no = 0;
	
	plock4sync_num->Lock();
	no = ++sync_num;
	plock4sync_num->Unlock();

	return no;
}

u16 CBizDeviceManager::_GetSyncNum()
{
	u16 no = 0;
	
	plock4sync_num->Lock();
	no = sync_num;	
	plock4sync_num->Unlock();

	return no;
}

/********************************************************************
							Device
********************************************************************/

CBizDevice::CBizDevice()
: plock4param(NULL)
, dev_idx(-1)
, sock_cmd(INVALID_SOCKET)
, b_alive(FALSE)
, cnt_err(0)
//stream
, plock4stream(NULL)
, bthread_stream_running(FALSE)
, bthread_stream_exit(FALSE)
, stream_cnt(0)
, idle_cnt(0)
, sem_exit(0)
{
	
}

CBizDevice::~CBizDevice()
{
	FreeSrc();
}

int CBizDevice::Init()
{
	//VD_BOOL b_ret = FALSE;
	
	plock4param = new CMutex;
	if (NULL == plock4param)
	{
		ERR_PRINT("new plock4param failed\n");
		goto fail;
	}
	
	if (plock4param->Create())//FALSE
	{
		ERR_PRINT("create plock4param failed\n");
		goto fail;
	}	

	plock4stream = new CMutex;
	if (NULL == plock4stream)
	{
		ERR_PRINT("new plock4stream failed\n");
		goto fail;
	}
	
	if (plock4stream->Create())//FALSE
	{
		ERR_PRINT("create plock4stream failed\n");
		goto fail;
	}
	return SUCCESS;

fail:
	FreeSrc();
	return -FAILURE;
}

void CBizDevice::FreeSrc()//释放资源
{	
	if (plock4param)
	{
		delete plock4param;
		plock4param = NULL;
	}

	if (plock4stream)
	{
		delete plock4stream;
		plock4stream = NULL;
	}
}

//外部加锁
void CBizDevice::CleanSock()
{
	if (INVALID_SOCKET != sock_cmd)
	{
		close(sock_cmd);
		sock_cmd = INVALID_SOCKET;

		b_alive = FALSE;
		cnt_err = 0;
	}
}

int CBizDevice::_DevLogin(ifly_loginpara_t *plogin)
{
	int realacklen = 0;
	int ret = SUCCESS;
	u8 buf[128];
    
	ret = g_biz_device_manager.NetDialogue(sock_cmd, CTRL_CMD_LOGIN, plogin, sizeof(ifly_loginpara_t), buf, sizeof(buf), &realacklen);
	if (SUCCESS != ret)
	{
		ERR_PRINT("NetDialogue CTRL_CMD_LOGIN invalid\n");
		return -FAILURE;
	}

	return SUCCESS;
}

int CBizDevice::_DevLogout(ifly_loginpara_t *plogin)
{
	int realacklen = 0;
	int ret = SUCCESS;
	u8 buf[128];
    
	ret = g_biz_device_manager.NetDialogue(sock_cmd, CTRL_CMD_LOGOFF, plogin, sizeof(ifly_loginpara_t), buf, sizeof(buf), &realacklen);
	if (SUCCESS != ret)
	{
		ERR_PRINT("NetDialogue CTRL_CMD_LOGOFF invalid\n");
		return -FAILURE;
	}

	return SUCCESS;
}

int CBizDevice::_DevSetAlarmUpload(u8 upload_enable)
{
	int realacklen = 0;
	int ret = SUCCESS;
	u8 buf[128];

	if (0 != upload_enable && 1 != upload_enable)
    {
        DBG_PRINT("upload_enable: %d, invalid\n", upload_enable);
        return -EPARAM;
    }
	
	ret = g_biz_device_manager.NetDialogue(sock_cmd, CTRL_CMD_ALARMUPLOADCENTER, &upload_enable,sizeof(upload_enable), buf, sizeof(buf), &realacklen);
	if (SUCCESS != ret)
	{
		ERR_PRINT("NetDialogue CTRL_CMD_LOGOFF invalid\n");
		return -FAILURE;
	}
	
	return SUCCESS;
}

int CBizDevice::GetDeviceInfo(ifly_DeviceInfo_t *pDeviceInfo)
{
	int realacklen = 0;
	int ret = SUCCESS;

    if (INVALID_SOCKET == sock_cmd || NULL == pDeviceInfo)
    {
        DBG_PRINT("param invalid\n");
        return -EPARAM;
    }
	
	ret = g_biz_device_manager.NetDialogue(sock_cmd, CTRL_CMD_GETDEVICEINFO, NULL, 0, pDeviceInfo, sizeof(ifly_DeviceInfo_t), &realacklen);
	if (ret)
	{
		ERR_PRINT("NetDialogue invalid\n");
		return -FAILURE;
	}

	//数据改变字节序
	pDeviceInfo->devicePort = ntohs(pDeviceInfo->devicePort);//只改一处

#ifdef NDEBUG
	struct in_addr in;
	in.s_addr = pDeviceInfo->deviceIP;
	
	printf("get device info deviceIP:%s, devicePort:%d, device_name:%s, device_mode:%s, MaxChn=%d, nNVROrDecoder:%d\n", \
		inet_ntoa(in),
		pDeviceInfo->devicePort,
		pDeviceInfo->device_name,
		pDeviceInfo->device_mode,
		pDeviceInfo->maxChnNum,
		pDeviceInfo->nNVROrDecoder);
#endif

	return ret;
}

//连接、登录服务器
int CBizDevice::DevConnect()
{
	int ret = 0;
	struct in_addr in;
	ifly_DeviceInfo_t DeviceInfo;

	in.s_addr = dev_info.deviceIP;

	//连接类型，0x1：控制信令；0x2：码流传输；0x3：广播搜索；0x4  轮巡同步(只对已经使能轮巡的设备有效)
	ret = SendTcpConnHead(sock_cmd, 0x1);
	if (ret < 0)
	{
		DBG_PRINT("svr IP: %s, SendTcpConnHead failed\n", inet_ntoa(in));
		
		goto fail;
	}
	DBG_PRINT("svr IP: %s, SendTcpConnHead success\n", inet_ntoa(in));

	//得到设备信息
	memset(&DeviceInfo, 0, sizeof(ifly_DeviceInfo_t));
	ret = GetDeviceInfo(&DeviceInfo);
	if (ret < 0)
	{
		DBG_PRINT("svr IP: %s, GetDeviceInfo failed\n", inet_ntoa(in));

		goto fail;
	}
	DBG_PRINT("svr IP: %s, GetDeviceInfo success\n", inet_ntoa(in));

	//类型不一致
#if 0	
	if (dev_info.devicetype != DeviceInfo.nNVROrDecoder+(u8)EM_DEV_TYPE_NONE)
	{
		DBG_PRINT("device type conflict, devicetype: %d, nNVROrDecoder: %d\n", 
			dev_info.devicetype, DeviceInfo.nNVROrDecoder);

		goto fail;
	}
#endif

	dev_info.maxChnNum = DeviceInfo.maxChnNum;

	//登录设备	
	memset(&login, 0, sizeof(ifly_loginpara_t));
	
	login.ipAddr = GetLocalIp();
	if (login.ipAddr == 0)
	{
		DBG_PRINT("GetLocalIp failed\n");
		goto fail;
	}
	
	strcpy(login.username, "admin");
	//strcpy(login.loginpass, pwd);
	strcpy(login.macAddr, "00:00:00:00:00:00");
	ret = _DevLogin(&login);
	if (ret < 0)
	{
		DBG_PRINT("svr IP: %s, DeviceLogin failed\n", inet_ntoa(in));

		goto fail;
	}
	DBG_PRINT("svr IP: %s, DeviceLogin success\n", inet_ntoa(in));

	//设置接收报警信息
	ret = _DevSetAlarmUpload(1);
	if (ret < 0)
	{
		DBG_PRINT("svr IP: %s, Device SetAlarmUpload failed\n", inet_ntoa(in));

		goto fail2;
	}
	DBG_PRINT("svr IP: %s, Device SetAlarmUpload success\n", inet_ntoa(in));
	
	b_alive = TRUE;
	return SUCCESS;

fail2:
	_DevLogout(&login);

fail:
	return -FAILURE;
}

//断开命令连接
int CBizDevice::DevDisconnect()
{
	int ret = 0;
	struct in_addr in;

	in.s_addr = dev_info.deviceIP;
	
	ret = _DevLogout(&login);
	if (ret < 0)
	{
		DBG_PRINT("svr IP: %s, DeviceLogout failed\n", inet_ntoa(in));
		
		goto fail;
	}
	DBG_PRINT("svr IP: %s, DeviceLogout success\n", inet_ntoa(in));

	//设置接收报警信息
	ret = _DevSetAlarmUpload(0);
	if (ret < 0)
	{
		DBG_PRINT("svr IP: %s, Device SetAlarmUpload failed\n", inet_ntoa(in));
		
		goto fail;
	}
	DBG_PRINT("svr IP: %s, Device SetAlarmUpload success\n", inet_ntoa(in));

	CleanSock();
	return SUCCESS;

fail:
	CleanSock();
	return -FAILURE;

}

//stream
//成功返回stearm_rcv[MaxMediaLinks] 下标stream_idx
int CBizDevice::StreamStart(ifly_TCP_Stream_Req *preq, CMediaStream *pstream)
{
	int i = 0;
	int ret = -FAILURE;
	int fd_tmp = INVALID_SOCKET;
	ifly_TCP_Stream_Ack stream_ack;
	struct sockaddr_in server;
	struct in_addr in;
	in.s_addr = dev_info.deviceIP;

	if (!b_alive)
	{
		DBG_PRINT("svr IP: %s, svr not alive\n", inet_ntoa(in));
		return -EDEV_OFFLINE;
	}
	
	plock4stream->Lock();

	for (i = 0; i < MaxMediaLinks; ++i)
	{
		if (!stearm_rcv[i].bstart)
		{
			break;
		}
	}

	if (i == MaxMediaLinks)
	{
		DBG_PRINT("svr IP: %s, media link num >= MaxMediaLinks\n", inet_ntoa(in));
		goto fail;
	}

	fd_tmp = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_tmp < 0)
	{
		DBG_PRINT("svr IP: %s, create socket failed, %s\n", inet_ntoa(in), strerror(errno));
		goto fail;
	}	

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = dev_info.deviceIP;
	server.sin_port = htons(dev_info.devicePort);

	ret = connect(fd_tmp, (struct sockaddr*)&server, sizeof(server));
	if (ret < 0)
	{
		DBG_PRINT("svr IP: %s, socket connect failed, %s\n", inet_ntoa(in), strerror(errno));

		goto fail2;
	}

	//连接类型，0x1：控制信令；0x2：码流传输；0x3：广播搜索；0x4  轮巡同步(只对已经使能轮巡的设备有效)
	ret = SendTcpConnHead(fd_tmp, 0x2);
	if (ret < 0)
	{
		DBG_PRINT("svr IP: %s, SendTcpConnHead 0x2 failed\n", inet_ntoa(in));
		
		goto fail2;
	}

	ret = loopsend(fd_tmp, (char *)preq, sizeof(ifly_TCP_Stream_Req));
	if (ret < 0)
	{
		DBG_PRINT("svr IP: %s, Send ifly_TCP_Stream_Req failed\n", inet_ntoa(in));
		goto fail2;
	}

	ret = looprecv(fd_tmp, (char *)&stream_ack, sizeof(ifly_TCP_Stream_Ack));
	if (ret < 0)
	{
		DBG_PRINT("svr IP: %s, recv ifly_TCP_Stream_Ack failed\n", inet_ntoa(in));
		goto fail2;
	}

	stream_ack.ackid = ntohl(stream_ack.ackid);
	stream_ack.errcode = ntohl(stream_ack.errcode);

	if (stream_ack.errcode != 0)
	{
		DBG_PRINT("svr IP: %s, stream req cmd: %d, ack errcode: %d\n", 
			inet_ntoa(in), preq->command, stream_ack.errcode);
		goto fail2;
	}

	//success
	if (!bthread_stream_running)
	{
		//启动服务器接收线程	
		m_threadlet_stream_rcv.run("BizDevice-streamrcv", this, (ASYNPROC)&CBizDevice::threadStreamRcv, 0, 0);
		bthread_stream_running = TRUE;
		bthread_stream_exit = FALSE;
	}
	
	stearm_rcv[i].sockfd = fd_tmp;
	stearm_rcv[i].linkid = stream_ack.ackid;
	stearm_rcv[i].req = *preq;
	stearm_rcv[i].pstream = pstream;
	stearm_rcv[i].bstart = 1;

	++stream_cnt;
	idle_cnt = 0;
	
	plock4stream->Unlock();

	DBG_PRINT("svr IP: %s, stream req cmd: %d success\n", 
			inet_ntoa(in), preq->command);
	return i;//success

fail2:
	close(fd_tmp);
	
fail:
	plock4stream->Unlock();

	return -FAILURE;
}

int CBizDevice::StreamStop(int stream_idx)
{
	int ret = -FAILURE;
	struct in_addr in;
	in.s_addr = dev_info.deviceIP;
	u32 id = 0;
	int realacklen = 0;
	char buf[128];
	
	if (stream_idx < 0 || stream_idx > MaxMediaLinks)
	{
		DBG_PRINT("svr IP: %s, stream_idx(%d) out of range[%d, %d)\n", 
			inet_ntoa(in), stream_idx, 0, MaxMediaLinks);
		return -EPARAM;
	}
	
	plock4stream->Lock();

	if (!stearm_rcv[stream_idx].bstart)
	{
		DBG_PRINT("svr IP: %s, stream_idx(%d) not start\n", 
			inet_ntoa(in), stream_idx);
		
		plock4stream->Unlock();
		return SUCCESS;
	}
	
	id = htonl(stearm_rcv[stream_idx].linkid);
	_CleanStream(stream_idx);
	
	plock4stream->Unlock();

	if (b_alive && (INVALID_SOCKET != sock_cmd))
	{
		ret = g_biz_device_manager.NetDialogue(sock_cmd, CTRL_CMD_STOPVIDEOMONITOR, &id, sizeof(id), buf, sizeof(buf), &realacklen);
		if (ret)
		{
			ERR_PRINT("NetDialogue CTRL_CMD_STOPVIDEOMONITOR failed\n");
		}
	}

	return SUCCESS;
}

void CBizDevice::_CleanStream(int stream_idx)
{
	//plock4stream->Lock();
	if (INVALID_SOCKET != stearm_rcv[stream_idx].sockfd)
	{
		close(stearm_rcv[stream_idx].sockfd);
		stearm_rcv[stream_idx].sockfd = INVALID_SOCKET;
	}
	
	stearm_rcv[stream_idx].linkid = -1;
	memset(&stearm_rcv[stream_idx].req, 0, sizeof(ifly_TCP_Stream_Req));
	stearm_rcv[stream_idx].pstream = NULL;
	stearm_rcv[stream_idx].bstart = 0;

	--stream_cnt;
	//plock4stream->Unlock();
}

//关闭所有数据流连接
int CBizDevice::ShutdownStreamAll()
{
	int i = 0;
	
	for (i = 0; i < MaxMediaLinks; ++i)
	{
		StreamStop(i);
	}
	
	return SUCCESS;
}

//重连部分数据流连接
int CBizDevice::CheckAndReconnectStream()
{
	return SUCCESS;
}

void CBizDevice::threadStreamRcv(uint param)
{
	struct in_addr in;
	in.s_addr = dev_info.deviceIP;
	int i = 0;	
	fd_set rset;
	struct timeval timeout;
	int fd_max = INVALID_SOCKET;
	int fd_tmp = INVALID_SOCKET;
	int ret = SUCCESS;
	FRAMEHDR frame_hdr;
	ifly_MediaFRAMEHDR_t hdr;
	char *pframe_data = NULL;
	u8 req_command = 0;
	CMediaStream* pstream = NULL;

	pframe_data = new char[MAX_FRAME_SIZE];
	if (NULL == pframe_data)
	{
		DBG_PRINT("svr IP: %s, CBizDevice::threadStreamRcv malloc frame buffer failed\n", inet_ntoa(in));
		return ;
	}

	DBG_PRINT("svr IP: %s, CBizDevice::threadStreamRcv running\n", inet_ntoa(in));

	while (1)
	{
		FD_ZERO(&rset);
		fd_max = INVALID_SOCKET;
		timeout.tv_sec = 2;
		timeout.tv_usec = 0;
		
		plock4stream->Lock();//plock4stream->Unlock();
	#if 0
		if (bthread_stream_exit)
		{
			bthread_stream_running = FALSE;
			break;
		}
	#endif
		for (i = 0; i < MaxMediaLinks; ++i)
		{
			fd_tmp = stearm_rcv[i].sockfd;
			if (stearm_rcv[i].bstart && (INVALID_SOCKET != fd_tmp))
			{
				FD_SET(fd_tmp, &rset);
				fd_max = MAX(fd_tmp, fd_max);
			}
		}

		if (INVALID_SOCKET == fd_max)
		{
			if (++idle_cnt > MaxIdleNum)
			{
				//空闲超时退出
				bthread_stream_running = FALSE;
				plock4stream->Unlock();
				break;
			}

			sleep(1);
			continue;
		}
		else
		{
			idle_cnt = 0;
		}
		
		plock4stream->Unlock();

		ret = select(fd_max+1, &rset, NULL, NULL, &timeout);
		if (ret < 0)
		{
			ERR_PRINT("select failed\n");
			sleep(1);
			continue;
		}
		else if (ret == 0)//timeout
		{
			continue;
		}

		//plock4stream->Lock();//plock4stream->Unlock();
		for (i = 0; i < MaxMediaLinks; ++i)
		{
			fd_tmp = stearm_rcv[i].sockfd;
			//0：预览 1：文件回放 2：时间回放 3：文件下载 4：升级 
			//5 VOIP 6 文件按帧下载 7 时间按帧下载 8 透明通道
			//9 远程格式化硬盘 10 主机端抓图 11 多路按时间回放 12 按时间下载文件
			req_command = stearm_rcv[i].req.command;
			pstream = stearm_rcv[i].pstream;
			
			if ((INVALID_SOCKET != fd_tmp) && FD_ISSET(fd_tmp, &rset))
			{				
				memset(&frame_hdr, 0, sizeof(FRAMEHDR));
				
				if (req_command == 0 || req_command == 1)
				{
					ret = looprecv(fd_tmp, (char *)&hdr, sizeof(ifly_MediaFRAMEHDR_t));
					if (ret)
					{
						DBG_PRINT("svr IP: %s, recv ifly_MediaFRAMEHDR_t failed\n", inet_ntoa(in));
						goto over;
					}
					
					hdr.m_dwDataSize = ntohl(hdr.m_dwDataSize);
					hdr.m_dwFrameID = ntohl(hdr.m_dwFrameID);
					hdr.m_dwTimeStamp = ntohl(hdr.m_dwTimeStamp);
					hdr.m_nVideoHeight = ntohl(hdr.m_nVideoHeight);
					hdr.m_nVideoWidth = ntohl(hdr.m_nVideoWidth);

					if (hdr.m_dwDataSize > MAX_FRAME_SIZE)
					{
						DBG_PRINT("svr IP: %s, recv DataSize(%d) > MAX_FRAME_SIZE(%d) failed\n",
							inet_ntoa(in), hdr.m_dwDataSize, MAX_FRAME_SIZE);
						goto over;
					}
					
					ret = looprecv(fd_tmp, pframe_data, hdr.m_dwDataSize);
					if (ret)
					{
						DBG_PRINT("svr IP: %s, recv one frame failed\n", inet_ntoa(in));
						goto over;
					}

					
					frame_hdr.m_byMediaType = hdr.m_byMediaType;
					frame_hdr.m_byFrameRate = hdr.m_byFrameRate;
					frame_hdr.m_tVideoParam.m_bKeyFrame = hdr.m_bKeyFrame;
					//printf("m_bKeyFrame: %d\n", hdr.m_bKeyFrame);
					frame_hdr.m_tVideoParam.m_wVideoWidth = hdr.m_nVideoWidth;
					//frame.m_tVideoParam.m_wVideoHeight = hdr.m_nVideoHeight;
					frame_hdr.m_tVideoParam.m_wVideoHeight = hdr.m_nVideoHeight & 0x7fff;			//csp add 20091116
					frame_hdr.m_tVideoParam.m_wBitRate = ((hdr.m_nVideoHeight & 0x8000)?1:0) << 31;	//csp add 20091116
					frame_hdr.m_dwDataSize = hdr.m_dwDataSize;
					frame_hdr.m_dwFrameID = hdr.m_dwFrameID;
					frame_hdr.m_dwTimeStamp = hdr.m_dwTimeStamp;
					frame_hdr.m_pData = (u8 *)pframe_data;

					if (hdr.m_bKeyFrame)
					{
						DBG_PRINT("svr IP: %s, recv one frame, command: %d, m_byMediaType: %d, m_bKeyFrame: %d, m_dwFrameID: %d, m_dwDataSize: %d\n", 
							inet_ntoa(in), req_command, hdr.m_byMediaType, hdr.m_bKeyFrame, hdr.m_dwFrameID, hdr.m_dwDataSize);
					}
					
					if (pstream != NULL)
					{
						pstream->dealFrameFunc(&frame_hdr);
					}
				}
				else if (req_command == 3)	//文件下载
				{
					ret = recv(fd_tmp, pframe_data, MAX_FRAME_SIZE, 0);
					if (ret <= 0)
					{
						DBG_PRINT("svr IP: %s, recv one frame failed\n", inet_ntoa(in));
						goto over;
					}
					frame_hdr.m_pData = (u8 *)pframe_data;
					frame_hdr.m_dwDataSize = ret;

					if (pstream != NULL)
					{
						pstream->dealFrameFunc(&frame_hdr);
					}
				}
				else
				{
					DBG_PRINT("svr IP: %s, stream req command(%d) not supported\n", inet_ntoa(in), req_command);
				}
			}
			continue;
			
over: //接收出错
			plock4stream->Lock();
			_CleanStream(i);
			plock4stream->Unlock();
			
			if (pstream != NULL)
			{
				pstream->dealStateFunc(EM_STREAM_RCV_ERR);//处理接收错误	
			}
		}		
	}

	delete [] pframe_data;
	pframe_data = NULL;
	DBG_PRINT("svr IP: %s, CBizDevice::threadStreamRcv exit\n", inet_ntoa(in));
}



/***********************************
 	               search device
***********************************/
static pthread_t g_ack_thread_id = 0;
static pthread_mutex_t g_search_lock = PTHREAD_MUTEX_INITIALIZER;
static unsigned char g_search_start = 0;
static std::vector<SBiz_DeviceInfo_t> *g_search_nvr_list;//搜索结果NVR列表
static std::vector<SBiz_DeviceInfo_t> *g_search_patrol_dec_list;//搜索结果patrol_dec 列表
static std::vector<SBiz_DeviceInfo_t> *g_search_switch_dec_list;//搜索结果switch_dec 列表


void SearchDevice_CB(ifly_DeviceInfo_t dev, void* pContext)
{
	SBiz_DeviceInfo_t dev_info;
	
	pthread_mutex_lock(&g_search_lock);

	if (g_search_start == 0)//未开启搜索
	{
		pthread_mutex_unlock(&g_search_lock);
		ERR_PRINT("Has not yet started\n");
		return;
	}

#if 1
	struct in_addr in;
	in.s_addr = dev.deviceIP;
	DBG_PRINT("dev name: %s, deviceIP: %s, port: %u, nNVROrDecoder: %d\n", \
		dev.device_name, inet_ntoa(in), dev.devicePort, dev.nNVROrDecoder);
#endif

	//得到本机IP，排除本机
	u32 localIP = GetLocalIp();
	if (localIP == 0)
	{
		pthread_mutex_unlock(&g_search_lock);
		ERR_PRINT("GetLocalIp failed\n");
		return;
	}

	if (localIP == dev.deviceIP)
	{
		pthread_mutex_unlock(&g_search_lock);
		//printf("%s search device is itself, return\n", );
		return;
	}

	//
	dev_info.maxChnNum 		= dev.maxChnNum;
	//nNVROrDecoder: 1 nvr，2 轮巡解码器，3 切换解码器，4 网络键盘
	//转换一下
	dev_info.devicetype		= (u8)EM_DEV_TYPE_NONE;
	dev_info.devicetype 	+= dev.nNVROrDecoder;
	dev_info.devicePort 	= dev.devicePort;
	dev_info.deviceIP 		= dev.deviceIP;

	switch (dev_info.devicetype)
	{
		case EM_NVR:
			if (NULL != g_search_nvr_list)
				g_search_nvr_list->push_back(dev_info);
			break;
		case EM_PATROL_DEC:
			if (NULL != g_search_patrol_dec_list)
				g_search_patrol_dec_list->push_back(dev_info);
			break;
		case EM_SWITCH_DEC:
			if (NULL != g_search_switch_dec_list)
				g_search_switch_dec_list->push_back(dev_info);
			break;
		default:
			ERR_PRINT("dev_type%d nonsupport\n", dev_info.devicetype);
	}
	
	pthread_mutex_unlock(&g_search_lock);
}

//设备搜索
int BizDevSearch(std::vector<SBiz_DeviceInfo_t> *pvnvr_list, 
							std::vector<SBiz_DeviceInfo_t> *pvpatrol_dec_list, 
							std::vector<SBiz_DeviceInfo_t> *pvswitch_dec_list)
{
	int ret = 0;

	if (!g_biz_device_manager.isInited())
	{
		ERR_PRINT("CBizDeviceManager not inited\n");
		return 0;
	}
	
	pthread_mutex_lock(&g_search_lock);

	if (g_search_start)//已经开始搜索
	{
		pthread_mutex_unlock(&g_search_lock);
		DBG_PRINT("has already started\n");
		return -FAILURE;
	}

	ret = CreateAckSearchProc(&g_ack_thread_id);
	if (ret)
	{
		pthread_mutex_unlock(&g_search_lock);
		DBG_PRINT("CreateAckSearchProc failed\n");
		return -FAILURE;
	}
	
	g_search_start = 1;
	g_search_nvr_list = pvnvr_list;//搜索结果NVR列表
	g_search_patrol_dec_list = pvpatrol_dec_list;//搜索结果patrol_dec 列表
	g_search_switch_dec_list = pvswitch_dec_list;//搜索结果switch_dec 列表

	pthread_mutex_unlock(&g_search_lock);

	ret = SearchDevice();
	if (ret)
	{
		ERR_PRINT("SearchDevice failed\n");
		return -FAILURE;
	}
	
	sleep(3);

	//取消回应线程
	pthread_mutex_lock(&g_search_lock);
	
	g_search_start = 0;
	
	if (g_ack_thread_id)
	{
		//取消回应线程
		ret = pthread_cancel(g_ack_thread_id);
		if (ret)
		{
			printf("%s pthread_cancel failed\n", __func__);
			return 0;
		}
		
		ret = pthread_join(g_ack_thread_id, NULL);
		if (ret)
		{
			printf("%s pthread_join failed\n", __func__);
			return 0;
		}

		g_ack_thread_id = 0;
	}

	if (NULL != g_search_nvr_list)
		std::sort(g_search_nvr_list->begin(), g_search_nvr_list->end());

	if (NULL != g_search_patrol_dec_list)
		std::sort(g_search_patrol_dec_list->begin(), g_search_patrol_dec_list->end());

	if (NULL != g_search_switch_dec_list)
		std::sort(g_search_switch_dec_list->begin(), g_search_switch_dec_list->end());

	
	pthread_mutex_unlock(&g_search_lock);

#if 0
	struct in_addr in;

	std::vector<SBiz_DeviceInfo_t>::iterator iter;
	for (iter = g_search_nvr_list->begin();
			iter != g_search_nvr_list->end();
			++iter)
	{		
		in.s_addr = iter->deviceIP;
		DBG_PRINT("nvr list deviceIP: %s\n", inet_ntoa(in));
	}

	for (iter = g_search_patrol_dec_list->begin();
			iter != g_search_patrol_dec_list->end();
			++iter)
	{		
		in.s_addr = iter->deviceIP;
		DBG_PRINT("patrol dec list deviceIP: %s\n", inet_ntoa(in));
	}

	for (iter = g_search_switch_dec_list->begin();
			iter != g_search_switch_dec_list->end();
			++iter)
	{		
		in.s_addr = iter->deviceIP;
		DBG_PRINT("switch dec list deviceIP: %s\n", inet_ntoa(in));
	}
#endif

	return SUCCESS;
}


/*****************************************************
			Device  Manager外部接口实现
*****************************************************/

int BizDeviceInit(void)
{
	
	//注册搜索回调函数
	SetFindDeviceCB(SearchDevice_CB, NULL);
	
	return g_biz_device_manager.Init();
}
int BizAddDev(EM_DEV_TYPE dev_type, u32 dev_ip)
{
	return g_biz_device_manager.AddDev(dev_type, dev_ip);
}

int BizDelDev(EM_DEV_TYPE dev_type, u32 dev_ip)
{
	return g_biz_device_manager.DelDev(dev_type, dev_ip);
}

int BizGetDevIPList(EM_DEV_TYPE dev_type, std::list<u32> &dev_ip_list)//网络字节序
{
	return g_biz_device_manager.GetDevIPList(dev_type, dev_ip_list);
}

int BizGetDevIdx(EM_DEV_TYPE dev_type, u32 dev_ip)
{
	return g_biz_device_manager.GetDevIdx(dev_type, dev_ip);
}

int BizGetDevInfo(EM_DEV_TYPE dev_type, u32 dev_ip, SGuiDev *pdev)
{
	return g_biz_device_manager.GetDevInfo(dev_type, dev_ip, pdev);
}

int BizStartNotifyDevInfo(void)	//使能通知。设备层将信息通知给上层
{
	return g_biz_device_manager.StartNotifyDevInfo();
}

int BizReqStreamStart(s32 dev_idx, ifly_TCP_Stream_Req *preq, CMediaStream *pstream)
{
	return g_biz_device_manager.ReqStreamStart(dev_idx, preq, pstream);
}

int BizReqStreamStop(s32 dev_idx, s32 stream_idx)
{
	return g_biz_device_manager.ReqStreamStop(dev_idx, stream_idx);
}





