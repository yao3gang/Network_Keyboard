#include "api.h"
#include "singleton.h"
#include "object.h"
#include "glb_error_num.h"
#include "biz.h"
#include "biz_types.h"
#include "biz_config.h"
#include "biz_net.h"
#include "biz_device.h"
#include "biz_preview.h"
#include "biz_remote_stream_define.h"
#include "hisi_sys.h"
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

#define g_biz_preview (*CBizPreview::instance())

class CBizPreview : public CMediaStream
{	
public:
	PATTERN_SINGLETON_DECLARE(CBizPreview);
	~CBizPreview();
	
	int Init(void);
	int PreviewStart(u32 svr_ip, u8 chn, u8 bmain);//bmain 是否主码流
	int PreviewStop();
	virtual int dealFrameFunc(FRAMEHDR *pframe_hdr);
	virtual int dealStateFunc(EM_STREAM_STATE_TYPE state, int param = 0);//param: 文件下载进度值
	
	
private:
	CBizPreview();
	CBizPreview(CBizPreview &)
	{

	}
	void FreeSrc();//释放资源
	
	
private:
	VD_BOOL b_inited;
	u8 b_start;
	u8 b_connect;
	C_Lock *plock4param;//mutex
	
};

PATTERN_SINGLETON_IMPLEMENT(CBizPreview);

CBizPreview::CBizPreview()
: b_inited(FALSE)
, b_start(0)
, b_connect(0)
, plock4param(NULL)
{
	
}

CBizPreview::~CBizPreview()
{
	FreeSrc();
}

int CBizPreview::Init(void)
{
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

	b_inited = TRUE;
	return SUCCESS;

fail:
	FreeSrc();
	return -FAILURE;
}

void CBizPreview::FreeSrc()
{
	if (plock4param)
	{
		delete plock4param;
		plock4param = NULL;
	}

	b_inited = FALSE;
}

int CBizPreview::PreviewStart(u32 svr_ip, u8 chn, u8 bmain)//bmain 是否主码流
{
	int ret = SUCCESS;
	
	plock4param->Lock();

	if (b_start)//已经开启预览
	{
		Stop();
		b_start = 0;
		b_connect = 0;
	}
	
	dev_ip = svr_ip;

	memset(&req, 0, sizeof(ifly_TCP_Stream_Req));
	req.command = 0;
	req.Monitor_t.chn = chn;
	req.Monitor_t.type = bmain ? 0:2;
	
	ret = BizGetDevIdx(EM_NVR, dev_ip);
	if (ret < 0)
	{
		ERR_PRINT("BizGetDevIdx failed, ret: %d\n", ret);
		plock4param->Unlock();
		return ret;
	}

	dev_idx = ret;
	
	ret = Start(this);
	if (ret < 0)
	{
		ERR_PRINT("PreviewStart failed, ret: %d\n", ret);
		plock4param->Unlock();
		return ret;
	}

	stream_idx = ret;

	b_start = 1;
	b_connect = 1;
	plock4param->Unlock();
	
	return SUCCESS;
}

int CBizPreview::PreviewStop()
{
	plock4param->Lock();

	if (b_start)
	{
		Stop();
		b_start = 0;
		b_connect = 0;
	}
	
	plock4param->Unlock();
	
	return SUCCESS;
}

int CBizPreview::dealFrameFunc(FRAMEHDR *pframe_hdr)
{
	int rtn = 0;
	vdec_stream_s in_stream;
	in_stream.rsv = 0;
	in_stream.pts = pframe_hdr->m_dwTimeStamp;
	in_stream.pts *= 1000;
	in_stream.data = pframe_hdr->m_pData;
	in_stream.len = pframe_hdr->m_dwDataSize;
	
	if (pframe_hdr->m_tVideoParam.m_bKeyFrame)
	{
		DBG_PRINT("m_dwFrameID: %d\n", pframe_hdr->m_dwFrameID);
	}
	
	rtn = nvr_preview_vdec_write(0, &in_stream);
	if (rtn < 0)
	{
		DBG_PRINT("nvr_preview_vdec_write failed\n");
		return FAILURE;
	}
	
	return SUCCESS;
}

int CBizPreview::dealStateFunc(EM_STREAM_STATE_TYPE state, int param)//param: 文件下载进度
{
	DBG_PRINT("\n");
	
	return SUCCESS;
}







/*****************************************************
			外部接口实现
*****************************************************/
int BizPreviewInit(void)
{
	return g_biz_preview.Init();
}

int BizPreviewStart(u32 svr_ip, u8 chn, u8 bmain)//bmain 是否主码流
{
	return g_biz_preview.PreviewStart(svr_ip, chn, bmain);
}

int BizPreviewStop()
{
	return g_biz_preview.PreviewStop();
}



