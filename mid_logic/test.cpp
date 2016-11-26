#include <new>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#endif

#include "types.h"
#include "ctimer.h"
#include "cthread.h"

#include "biz.h"
#include "biz_config.h"
#include "biz_net.h"
#include "biz_device.h"
#include "biz_preview.h"

#if 1
int main(void)
{
	VD_BOOL b_ret = FALSE;
	struct in_addr in;
	std::vector<SBiz_DeviceInfo_t> vnvr_list;
	std::vector<SBiz_DeviceInfo_t> vpatrol_dec_list;
	std::vector<SBiz_DeviceInfo_t> vswitch_dec_list;
	std::vector<SBiz_DeviceInfo_t>::iterator iter;
	
	BizInit();

	BizDelDev(EM_NVR, 4244744384);
	sleep(3);
	//printf("\n\n\n\n");
	//BizPreviewInit();
	//BizPreviewStart((u32)4244744384, 1, 1);//chn1 main-stream

	//sleep(3);

	//BizPreviewStop();

	while (1)
	{
		sleep(1);
	}
/*
	BizDevSearch(&vnvr_list, &vpatrol_dec_list, &vswitch_dec_list);

	sleep(3);
	for (iter = vnvr_list.begin();
		iter != vnvr_list.end();
		++iter)
	{
		BizAddDev((EM_DEV_TYPE)iter->devicetype, iter->deviceIP);
	}

	for (iter = vpatrol_dec_list.begin();
		iter != vpatrol_dec_list.end();
		++iter)
	{
		BizAddDev((EM_DEV_TYPE)iter->devicetype, iter->deviceIP);
	}
		
	for (iter = vswitch_dec_list.begin();
		iter != vswitch_dec_list.end();
		++iter)
	{
		BizAddDev((EM_DEV_TYPE)iter->devicetype, iter->deviceIP);
	}
	
	while (1)
	{
		sleep(1);
	}
*/	
#if 0
	SConfigTimeParam stimme_param;
	b_ret = BizGetTimeParam(stimme_param);
	if (!b_ret)
	{
		ERR_PRINT("BizGetTimeParam failed\n");
		return 1;
	}

	DBG_PRINT("ndata_format: %d\n", stimme_param.ndata_format);
	DBG_PRINT("ntime_format: %d\n", stimme_param.ntime_format);
	DBG_PRINT("btime_sync: %d\n", stimme_param.btime_sync);
	DBG_PRINT("ntp_svr_ip: %s\n", stimme_param.ntp_svr_ip.c_str());

	stimme_param.ntime_format = 1;
	b_ret = BizSetTimeParam(stimme_param);
	if (!b_ret)
	{
		ERR_PRINT("BizSetTimeParam failed\n");
		return 1;
	}

#endif
#if 0

	SConfigNetParam snet_param;
	b_ret = BizGetNetParam(snet_param);
	if (!b_ret)
	{
		ERR_PRINT("BizGetNetParam failed\n");
		return 1;
	}
	
	in.s_addr = snet_param.nhost_ip;
	DBG_PRINT("nhost_ip: %s\n", inet_ntoa(in));
	in.s_addr = snet_param.nsubmask;
	DBG_PRINT("nsubmask: %s\n", inet_ntoa(in));
	in.s_addr = snet_param.ngateway;
	DBG_PRINT("ngateway: %s\n", inet_ntoa(in));
	in.s_addr = snet_param.ndns1;
	DBG_PRINT("ndns1: %s\n", inet_ntoa(in));
	in.s_addr = snet_param.ndns2;
	DBG_PRINT("ndns2: %s\n", inet_ntoa(in));
	;
	DBG_PRINT("nsvr_port: %d\n", snet_param.nsvr_port);
	
	DBG_PRINT("mac: %s\n", snet_param.mac.c_str());

	snet_param.nhost_ip = 1929488576;
	b_ret = BizSetNetParam(snet_param);
	if (!b_ret)
	{
		ERR_PRINT("BizSetNetParam failed\n");
		return 1;
	}
#endif
#if 0
	std::vector<uint> vnvr_list;
	u32 i, dev_nums;
	
	b_ret = BizConfigGetNvrList(vnvr_list);
	if (!b_ret)
	{
		ERR_PRINT("BizGetNvrList failed\n");
		return 1;
	}
	dev_nums = vnvr_list.size();

	for (i=0; i<dev_nums; ++i)
	{
		in.s_addr = vnvr_list[i];
		DBG_PRINT("dev list %d: %s\n", i, inet_ntoa(in));
	}

	vnvr_list.clear();
	vnvr_list.push_back(100772032);
	vnvr_list.push_back(83994816);
	
	
	b_ret = BizConfigAddNvrList(vnvr_list);
	if (!b_ret)
	{
		ERR_PRINT("BizGetNvrList failed\n");
		return 1;
	}

#endif	
#if 0
	std::vector<SConfigTvWallParam> vtvwall_list;
	int i, j, list_num;
	u32 itmp;
	
	b_ret = BizGetTvWallList(vtvwall_list);
	if (!b_ret)
	{
		ERR_PRINT("BizGetTvWallList failed\n");
		return 1;
	}

	list_num = vtvwall_list.size();
	DBG_PRINT("tvwall nums: %d\n", list_num);

	for (i=0; i<list_num; ++i)
	{		
		//ndevs_per_line
		itmp = vtvwall_list[i].ndevs_per_line;
		
		DBG_PRINT("ndevs_per_line: %d\n", itmp);

		//ndevs_per_col
		itmp = vtvwall_list[i].ndevs_per_col;
		
		DBG_PRINT("ndevs_per_col: %d\n", itmp);
		
		for (j=0; j<vtvwall_list[i].ndevs_per_line * vtvwall_list[i].ndevs_per_col; ++j)
		{
			itmp = vtvwall_list[i].devlist[j];
			in.s_addr = itmp;
			
			DBG_PRINT("tvwall devlist%d-%d: %s\n", i, j, inet_ntoa(in));
		}
	}

	SConfigTvWallParam tvwall;
	/*
	tvwall.ndevs_per_col = 1;
	tvwall.ndevs_per_line =2;
	tvwall.devlist.push_back(117549248);
	tvwall.devlist.push_back(134326464);

	vtvwall_list.clear();
	vtvwall_list.push_back(tvwall);

	b_ret = BizAddTvWallParamList(vtvwall_list);
	*/
	/*
	tvwall = vtvwall_list[1];
	tvwall.ndevs_per_col = 2;
	tvwall.ndevs_per_line = 1;
	b_ret = BizModifyTvWallParam(1, tvwall);
	*/
	b_ret = BizAddTvWallParamList(vtvwall_list);
	if (!b_ret)
	{
		ERR_PRINT("BizAddTvWallParamList failed\n");
		return 1;
	}
#endif

#if 0

	//Éè±¸ËÑË÷
	int cnt = 0;
	std::vector<SBiz_DeviceInfo_t> vnvr_list;
	std::vector<SBiz_DeviceInfo_t> vpatrol_dec_list;
	std::vector<SBiz_DeviceInfo_t> vswitch_dec_list;
	while (cnt++ < 3)
	{
		BizDevSearch(&vnvr_list, &vpatrol_dec_list, &vswitch_dec_list);
		vnvr_list.clear();
		vpatrol_dec_list.clear();
		vswitch_dec_list.clear();
		sleep(2);
	}
#endif

	DBG_PRINT("main end\n");
	return 0;
}
#endif 

#if 0
int main(int argc, char **argv)
{
	struct in_addr in;
	
	Biz_DeviceInfo_t a, b;
	
	a.deviceIP = inet_addr(argv[1]);
	b.deviceIP = inet_addr(argv[2]);
	
	printf("a ip: %s, u32: %u\n", argv[1], a.deviceIP);
	printf("b ip: %s, u32: %u\n", argv[2], b.deviceIP);
	
	printf("a %c b\n", a<b?'<':'>');
	
	
	return 0;
}
#endif

