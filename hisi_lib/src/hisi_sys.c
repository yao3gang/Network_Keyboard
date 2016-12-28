#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>

#include "sample_comm.h"

#include <linux/fb.h>

#include "types.h"
#include "hifb.h"
#include "loadbmp.h"
#include "hi_math.h"

#include "hi_tde_api.h"
#include "hi_tde_type.h"
#include "hi_tde_errcode.h"
#include "mpi_vo.h"

//yaogang modify
#include "hisi_sys.h"

#define ALIGN_BACK(x, a)		 ((a) * (((x) / (a))))

static pthread_mutex_t hisi_lock = PTHREAD_MUTEX_INITIALIZER; 

static VO_DEV VoDev = 0;//Vo设备和Vo图形层都是0
static VO_LAYER VoLayer = 0;

static struct fb_bitfield s_a32 = {24,8,0};
static struct fb_bitfield s_r32 = {16,8,0};
static struct fb_bitfield s_g32 = {8,8,0};
static struct fb_bitfield s_b32 = {0,8,0};

int hisi_chn_start(int stream_chn)
{
	HI_S32 s32Ret = HI_SUCCESS;
	
	pthread_mutex_lock(&hisi_lock);

	s32Ret = HI_MPI_VO_EnableChn(VoLayer, stream_chn);
	if(HI_SUCCESS != s32Ret)
	{
		ERR_PRINT("HI_MPI_VO_EnableChn failed, ret: 0x%x\n", s32Ret);
	}
	
	s32Ret = HI_MPI_VDEC_StartRecvStream(stream_chn);//yaogang modify 20150306
	if(HI_SUCCESS != s32Ret)
	{
		ERR_PRINT("HI_MPI_VDEC_StartRecvStream failed, ret: 0x%x\n", s32Ret);
	}
								
	pthread_mutex_unlock(&hisi_lock);

	return 0;
}

int hisi_chn_stop(int stream_chn)
{
	HI_S32 s32Ret = HI_SUCCESS;
	
	pthread_mutex_lock(&hisi_lock);
	
	s32Ret = HI_MPI_VDEC_StopRecvStream(stream_chn);
	if(HI_SUCCESS != s32Ret)
	{
		ERR_PRINT("HI_MPI_VDEC_StopRecvStream failed, ret: 0x%x\n", s32Ret);
	}
	
	s32Ret = HI_MPI_VDEC_ResetChn(stream_chn);
	if(HI_SUCCESS != s32Ret)
	{
		ERR_PRINT("HI_MPI_VDEC_ResetChn failed, ret: 0x%x\n", s32Ret);
	}

	s32Ret = HI_MPI_VPSS_ResetGrp(stream_chn);
	if(HI_SUCCESS != s32Ret)
	{
		ERR_PRINT("HI_MPI_VPSS_ResetGrp(vdch=%d) failed, ret: 0x%x\n", stream_chn, s32Ret);
		//SAMPLE_COMM_VPSS_Stop(chn, 1, VPSS_MAX_CHN_NUM);
		//SampleWaitDestroyVdecChn(chn);
		return 0;
	}
	
	s32Ret = HI_MPI_VO_ClearChnBuffer(VoLayer, stream_chn, HI_TRUE);
	if(HI_SUCCESS != s32Ret)
	{
		ERR_PRINT("HI_MPI_VO_ClearChnBuffer failed, ret: 0x%x\n", s32Ret);
	}
	//yaogang modify 20150323
	//HI_MPI_VO_DisableChn + HI_MPI_VO_EnableChn是为了解决删除IPC通道后，
	//双击该通道区域会有一帧画面残留，逻辑上应该是黑屏
	s32Ret = HI_MPI_VO_DisableChn(VoLayer, stream_chn);
	if(HI_SUCCESS != s32Ret)
	{
		ERR_PRINT("HI_MPI_VO_DisableChn failed, ret: 0x%x\n", s32Ret);
	}
	
	pthread_mutex_unlock(&hisi_lock);

	return 0;
}


int HisiMPPInit(SIZE_S *pstSize)
{
	VB_CONF_S stVbConf;
	HI_S32 s32Ret = HI_SUCCESS;
	HI_U32 u32AlignWidth = 16;
	MPP_SYS_CONF_S stSysConf = {0};
	int i = 0;

	HI_MPI_SYS_Exit();
    for(i=0;i<VB_MAX_USER;i++)
    {
         HI_MPI_VB_ExitModCommPool(i);
    }
	for(i=0; i<VB_MAX_POOLS; i++)
    {
         HI_MPI_VB_DestroyPool(i);
    }
    HI_MPI_VB_Exit();
	
	/******************************************
	step  1: init global  variable 
	******************************************/
	memset(&stVbConf,0,sizeof(VB_CONF_S));
	stVbConf.u32MaxPoolCnt = 2;
	stVbConf.astCommPool[0].u32BlkSize = (CEILING_2_POWER(pstSize->u32Width, SAMPLE_SYS_ALIGN_WIDTH) \
		* CEILING_2_POWER(pstSize->u32Height, SAMPLE_SYS_ALIGN_WIDTH) * 3) >> 1;
	stVbConf.astCommPool[0].u32BlkCnt = 20;
	stVbConf.astCommPool[1].u32BlkSize = (CEILING_2_POWER(704, SAMPLE_SYS_ALIGN_WIDTH) \
		* CEILING_2_POWER(576, SAMPLE_SYS_ALIGN_WIDTH)*3)>>2;
	stVbConf.astCommPool[1].u32BlkCnt = 20;

	s32Ret = HI_MPI_VB_SetConf(&stVbConf);
	if (HI_SUCCESS != s32Ret)
	{
	    printf("HI_MPI_VB_SetConf failed!, 0x%x\n", s32Ret);
	    return HI_FAILURE;
	}
	
	s32Ret = HI_MPI_VB_Init();
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VB_Init failed!, 0x%x\n", s32Ret);
        return HI_FAILURE;
    }

    stSysConf.u32AlignWidth = SAMPLE_SYS_ALIGN_WIDTH;
    s32Ret = HI_MPI_SYS_SetConf(&stSysConf);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_SetConf failed!, 0x%x\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_SYS_Init();
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_Init failed!, 0x%x\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

int HisiMPPExit()
{
	HI_S32 i;

    HI_MPI_SYS_Exit();
    for(i=0;i<VB_MAX_USER;i++)
    {
         HI_MPI_VB_ExitModCommPool(i);
    }
    for(i=0; i<VB_MAX_POOLS; i++)
    {
         HI_MPI_VB_DestroyPool(i);
    }	
    HI_MPI_VB_Exit();
	
	return HI_SUCCESS;
}

int HisiVdecInit(SIZE_S *pstSize)
{	
	HI_S32 s32Ret = HI_SUCCESS;
	VDEC_CHN_ATTR_S stAttr;
	VDEC_PRTCL_PARAM_S stPrtclParam;
	
	memset(&stAttr, 0, sizeof(VDEC_CHN_ATTR_S));
	stAttr.enType = PT_H264;
	stAttr.u32BufSize = pstSize->u32Height * pstSize->u32Width * 3 / 4;
	stAttr.u32Priority = 1;//此处必须大于0
	stAttr.u32PicWidth = pstSize->u32Width;
	stAttr.u32PicHeight = pstSize->u32Height;
	stAttr.stVdecVideoAttr.u32RefFrameNum = 2;//1;//csp modify 20140307
	stAttr.stVdecVideoAttr.enMode = VIDEO_MODE_FRAME;//VIDEO_MODE_FRAME; VIDEO_MODE_STREAM
	stAttr.stVdecVideoAttr.s32SupportBFrame = 0;//0;

	//就一个通道
	s32Ret = HI_MPI_VDEC_CreateChn(0, &stAttr);
	if(HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_VDEC_CreateChn[%d] failed errno 0x%x (wxh=%dx%d)\n", 0, s32Ret, pstSize->u32Width, pstSize->u32Height);
		return s32Ret;
	}

	s32Ret = HI_MPI_VDEC_GetProtocolParam(0, &stPrtclParam);
	if(HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_VDEC_GetPrtclParam failed errno 0x%x\n", s32Ret);
		HI_MPI_VDEC_DestroyChn(0);//csp modify 20150110
		return s32Ret;
	}

	stPrtclParam.s32MaxSpsNum = 1;
	stPrtclParam.s32MaxPpsNum = 1;
	stPrtclParam.s32MaxSliceNum = 16;
	stPrtclParam.s32DisplayFrameNum = 1;//2;//1;
	s32Ret = HI_MPI_VDEC_SetProtocolParam(0, &stPrtclParam);
	if(HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_VDEC_SetPrtclParam failed errno 0x%x\n", s32Ret);
		HI_MPI_VDEC_DestroyChn(0);//csp modify 20150110
		return s32Ret;
	}

	s32Ret = HI_MPI_VDEC_StartRecvStream(0);
	if(HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_VDEC_StartRecvStream failed errno 0x%x\n", s32Ret);
		HI_MPI_VDEC_DestroyChn(0);//csp modify 20150110
		return s32Ret;
	}

	return HI_SUCCESS;
}

int HisiVdecExit()
{
	HI_S32 s32Ret = HI_SUCCESS;
	
	s32Ret = HI_MPI_VDEC_StopRecvStream(0);
	if(HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_VDEC_StopRecvStream failed errno 0x%x\n", s32Ret);
		return s32Ret;
	}
	
	s32Ret = HI_MPI_VDEC_DestroyChn(0);
	if(HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_VDEC_DestroyChn failed errno 0x%x\n", s32Ret);
		return s32Ret;
	}

	return HI_SUCCESS;
}

int HisiVpssInit(SIZE_S *pstSize)
{	
	HI_S32 s32Ret = HI_SUCCESS;
	VPSS_CHN VpssChn;
	VPSS_GRP_ATTR_S stGrpAttr = {0};
    VPSS_CHN_ATTR_S stChnAttr = {0};
    VPSS_GRP_PARAM_S stVpssParam = {0};
	HI_S32 i = 0;

	stGrpAttr.u32MaxW = pstSize->u32Width;
	stGrpAttr.u32MaxH = pstSize->u32Height;
	
	stGrpAttr.bDciEn = HI_FALSE;
	
	stGrpAttr.bIeEn = HI_FALSE;
	stGrpAttr.bNrEn = HI_FALSE;//HI_TRUE;//HI_FALSE;//很关键,可以解决Hi3535主板VGA分辨率800x600下HDMI显示闪烁的问题//csp modify 20150110
	stGrpAttr.bHistEn = HI_FALSE;
	stGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;//VPSS_DIE_MODE_AUTO;
	stGrpAttr.enPixFmt = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

	//就一个通道
	s32Ret = HI_MPI_VPSS_CreateGrp(0, &stGrpAttr);
	if (s32Ret != HI_SUCCESS)
	{
		printf("HI_MPI_VPSS_CreateGrp failed with %#x!\n", s32Ret);
		return HI_FAILURE;
	}

	s32Ret = HI_MPI_VPSS_GetGrpParam(0, &stVpssParam);
    if (s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_VPSS_GetGrpParam failed with %#x!\n", s32Ret);
        goto fail;
    }
    
    stVpssParam.u32IeStrength = 0;
    s32Ret = HI_MPI_VPSS_SetGrpParam(0, &stVpssParam);
    if (s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_VPSS_SetGrpParam failed with %#x!\n", s32Ret);
        goto fail;
    }

	/* Set Vpss Chn attr */
	stChnAttr.bSpEn = HI_FALSE;//csp modify 20150110
	stChnAttr.bBorderEn = HI_FALSE;//HI_TRUE;
	stChnAttr.stBorder.u32Color = 0xff00;
	stChnAttr.stBorder.u32LeftWidth = 2;
	stChnAttr.stBorder.u32RightWidth = 2;
	stChnAttr.stBorder.u32TopWidth = 2;
	stChnAttr.stBorder.u32BottomWidth = 2;

	for(i=0; i<3; i++)
	{
        VpssChn = i;        
        
        s32Ret = HI_MPI_VPSS_SetChnAttr(0, VpssChn, &stChnAttr);
        if (s32Ret != HI_SUCCESS)
        {
            printf("HI_MPI_VPSS_SetChnAttr failed with %#x\n", s32Ret);
            goto fail2;
        }

        s32Ret = HI_MPI_VPSS_EnableChn(0, VpssChn);
        if (s32Ret != HI_SUCCESS)
        {
            printf("HI_MPI_VPSS_EnableChn failed with %#x\n", s32Ret);
            goto fail2;
        }
    }

	s32Ret = HI_MPI_VPSS_StartGrp(0);
	if (s32Ret != HI_SUCCESS)
	{
		printf("HI_MPI_VPSS_StartGrp failed with %#x\n", s32Ret);
		goto fail2;
	}

	return HI_SUCCESS;

fail2:
	for(i=0; i<3; i++)
	{
        VpssChn = i;       
       
        HI_MPI_VPSS_DisableChn(0, VpssChn);
    }
	
fail:
	HI_MPI_VPSS_DestroyGrp(0);

	return s32Ret;
}

int HisiVpssExit()
{
	VPSS_CHN VpssChn;
	HI_S32 i = 0;
	
	for(i=0; i<3; i++)
	{
        VpssChn = i;       
       
        HI_MPI_VPSS_DisableChn(0, VpssChn);
    }
	
	HI_MPI_VPSS_DestroyGrp(0);

	return HI_SUCCESS;
}

static HI_VOID SAMPLE_COMM_VO_HdmiConvertSync(VO_INTF_SYNC_E enIntfSync,
    HI_HDMI_VIDEO_FMT_E *penVideoFmt)
{
	switch (enIntfSync)
	{
		case VO_OUTPUT_PAL:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_PAL;
			break;
		case VO_OUTPUT_NTSC:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_NTSC;
			break;
		case VO_OUTPUT_1080P24:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_24;
			break;
		case VO_OUTPUT_1080P25:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_25;
			break;
		case VO_OUTPUT_1080P30:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_30;
			break;
		case VO_OUTPUT_720P50:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_720P_50;
			break;
		case VO_OUTPUT_720P60:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_720P_60;
			break;
		case VO_OUTPUT_1080I50:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_1080i_50;
			break;
		case VO_OUTPUT_1080I60:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_1080i_60;
			break;
		case VO_OUTPUT_1080P50:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_50;
			break;
		case VO_OUTPUT_1080P60:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_60;
			break;
		case VO_OUTPUT_576P50:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_576P_50;
			break;
		case VO_OUTPUT_480P60:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_480P_60;
			break;
		case VO_OUTPUT_800x600_60:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_800X600_60;
			break;
		case VO_OUTPUT_1024x768_60:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1024X768_60;
			break;
		case VO_OUTPUT_1280x1024_60:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1280X1024_60;
			break;
		case VO_OUTPUT_1366x768_60:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1366X768_60;
			break;
		case VO_OUTPUT_1440x900_60:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1440X900_60;
			break;
		case VO_OUTPUT_1280x800_60:
			*penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1280X800_60;
			break;
		default:
			printf("Unkonw VO_INTF_SYNC_E value!\n");
			break;
	}
	
	return;
}

HI_S32 SAMPLE_COMM_VO_HdmiStart(VO_INTF_SYNC_E enIntfSync)
{
    HI_HDMI_INIT_PARA_S stHdmiPara;
    HI_HDMI_ATTR_S      stAttr;
    HI_HDMI_VIDEO_FMT_E enVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1920X1080_60;
	
    SAMPLE_COMM_VO_HdmiConvertSync(enIntfSync, &enVideoFmt);
	
    stHdmiPara.enForceMode = HI_HDMI_FORCE_HDMI;
    stHdmiPara.pCallBackArgs = NULL;
    stHdmiPara.pfnHdmiEventCallback = NULL;
    HI_MPI_HDMI_Init(&stHdmiPara);
	
    HI_MPI_HDMI_Open(HI_HDMI_ID_0);
	
    HI_MPI_HDMI_GetAttr(HI_HDMI_ID_0, &stAttr);
	
    stAttr.bEnableHdmi = HI_TRUE;
    
    stAttr.bEnableVideo = HI_TRUE;
    stAttr.enVideoFmt = enVideoFmt;
	
    stAttr.enVidOutMode = HI_HDMI_VIDEO_MODE_YCBCR444;
    stAttr.enDeepColorMode = HI_HDMI_DEEP_COLOR_OFF;
    stAttr.bxvYCCMode = HI_FALSE;
	
    stAttr.bEnableAudio = HI_FALSE;
    stAttr.enSoundIntf = HI_HDMI_SND_INTERFACE_I2S;
    stAttr.bIsMultiChannel = HI_FALSE;
	
    stAttr.enBitDepth = HI_HDMI_BIT_DEPTH_16;
	
    stAttr.bEnableAviInfoFrame = HI_TRUE;
    stAttr.bEnableAudInfoFrame = HI_TRUE;
    stAttr.bEnableSpdInfoFrame = HI_FALSE;
    stAttr.bEnableMpegInfoFrame = HI_FALSE;
	
    stAttr.bDebugFlag = HI_FALSE;          
    stAttr.bHDCPEnable = HI_FALSE;
	
    stAttr.b3DEnable = HI_FALSE;
    
    HI_MPI_HDMI_SetAttr(HI_HDMI_ID_0, &stAttr);
	
    HI_MPI_HDMI_Start(HI_HDMI_ID_0);
	
	#ifdef HDMI_HAS_AUDIO
	extern HI_S32 SAMPLE_COMM_AUDIO_StartHdmi();
	SAMPLE_COMM_AUDIO_StartHdmi();
	#endif
    
    printf("HDMI start success.\n");
	
    return HI_SUCCESS;
}

int HisiVoInit(SIZE_S *pstSize)
{
	VO_PUB_ATTR_S VoAttr;
	VO_PUB_ATTR_S stVoPubAttr; 
	VO_VIDEO_LAYER_ATTR_S stLayerAttr;
	VO_CHN_ATTR_S stChnAttr;	
	HI_S32 gs_u32ViFrmRate;
	HI_S32 s32Ret = HI_SUCCESS;
	HI_S32 s32X, s32Y;
	HI_U32 u32Width, u32Height;
	
	memset(&VoAttr, 0, sizeof(VO_PUB_ATTR_S));
	memset(&stLayerAttr, 0, sizeof(VO_VIDEO_LAYER_ATTR_S));

	gs_u32ViFrmRate = 25; //PAL
	VoDev = 0;//VO_DEVICE_HD;
	stVoPubAttr.enIntfSync = VO_OUTPUT_1024x768_60;
	stVoPubAttr.enIntfType = VO_INTF_VGA | VO_INTF_HDMI; // VGA & HDMI output
	stVoPubAttr.u32BgColor = 0x00000000;

	//VO StartDev
	s32Ret = HI_MPI_VO_SetPubAttr(VoDev, &stVoPubAttr);
    if (s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_VO_SetPubAttr failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VO_Enable(VoDev);
    if (s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_VO_Enable failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

	//VO StartLayer
	s32Ret = HI_MPI_VO_GetVideoLayerAttr(VoLayer, &stLayerAttr);
	if (s32Ret != HI_SUCCESS)
	{
	   printf("HI_MPI_VO_GetVideoLayerAttr failed with %#x!\n", s32Ret);
	   goto fail;
	}

	stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
	stLayerAttr.u32DispFrmRt = 25;//视频显示帧率
 	stLayerAttr.stDispRect.s32X		 = 0;
	stLayerAttr.stDispRect.s32Y		 = 0;
	stLayerAttr.stDispRect.u32Width	 = pstSize->u32Width;
	stLayerAttr.stDispRect.u32Height  = pstSize->u32Height;
	stLayerAttr.stImageSize.u32Width  = pstSize->u32Width;
	stLayerAttr.stImageSize.u32Height = pstSize->u32Height;

	s32Ret = HI_MPI_VO_SetVideoLayerAttr(VoLayer, &stLayerAttr);
	if (s32Ret != HI_SUCCESS)
	{
	   printf("HI_MPI_VO_SetVideoLayerAttr failed with %#x!\n", s32Ret);
	   goto fail;
	}
	
	s32Ret = HI_MPI_VO_EnableVideoLayer(VoLayer);
	if (s32Ret != HI_SUCCESS)
	{
	   printf("HI_MPI_VO_EnableVideoLayer failed with %#x!\n", s32Ret);
	   goto fail;
	}
	//yaogang modify 20150922
	s32Ret = HI_MPI_VO_SetPlayToleration(VoLayer, 300);//300ms
	if(HI_SUCCESS != s32Ret)
	{
		printf("%s HI_MPI_VO_SetPlayToleration() failed!\n", __func__);
		goto fail2;
	}

	//VO StartChn
	s32X = 200;//QT widget_left
	s32Y = 0;//QT widget_title
	u32Width  = pstSize->u32Width  - s32X;
	u32Height = pstSize->u32Height - s32Y;

	stChnAttr.stRect.s32X       = ALIGN_BACK(s32X, 2);
	stChnAttr.stRect.s32Y       = ALIGN_BACK(s32Y, 2);
	stChnAttr.stRect.u32Width   = ALIGN_BACK(u32Width, 2);
	stChnAttr.stRect.u32Height  = ALIGN_BACK(u32Height, 2);
	stChnAttr.u32Priority       = 0;
	stChnAttr.bDeflicker        = HI_TRUE;

	s32Ret = HI_MPI_VO_SetChnAttr(VoLayer, 0, &stChnAttr);
	if (s32Ret != HI_SUCCESS)
	{
		printf("%s(%d):failed with %#x!\n", __FUNCTION__, __LINE__,  s32Ret);
		goto fail2;
	}
	
	s32Ret = HI_MPI_VO_EnableChn(VoLayer, 0);
	if (s32Ret != HI_SUCCESS)
	{
		printf("failed with %#x!\n", s32Ret);
		goto fail2;
	}

	if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
	{
		s32Ret = SAMPLE_COMM_VO_HdmiStart(stVoPubAttr.enIntfSync);
		if (HI_SUCCESS != s32Ret)
		{
			printf("Start SAMPLE_COMM_VO_HdmiStart failed!\n");
			goto fail3;
		}
	}

	return HI_SUCCESS;

fail3:
	HI_MPI_VO_DisableChn(VoLayer, 0);
	
fail2:
	HI_MPI_VO_DisableVideoLayer(VoLayer);
	
fail:
	HI_MPI_VO_Disable(VoDev);

	return s32Ret;
}

int HisiVoExit()
{
	HI_MPI_VO_DisableChn(VoLayer, 0);
	HI_MPI_VO_DisableVideoLayer(VoLayer);
	HI_MPI_VO_Disable(VoDev);

	return HI_SUCCESS;
}

/******************************************************************************
* function : vdec group bind vpss chn
******************************************************************************/
HI_S32 SAMLE_COMM_VDEC_BindVpss(VDEC_CHN VdChn, VPSS_GRP VpssGrp)
{
	HI_S32 s32Ret = HI_SUCCESS;
	MPP_CHN_S stSrcChn;
	MPP_CHN_S stDestChn;
	
	stSrcChn.enModId = HI_ID_VDEC;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = VdChn;
	
	stDestChn.enModId = HI_ID_VPSS;
	stDestChn.s32DevId = VpssGrp;
	stDestChn.s32ChnId = 0;
	
	s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
	if(s32Ret != HI_SUCCESS)
	{
		printf("HI_MPI_SYS_Bind failed with %#x!\n",s32Ret);
		return HI_FAILURE;
	}
	
	return HI_SUCCESS;
}

/******************************************************************************
* function : vdec group unbind vpss chn
******************************************************************************/
HI_S32 SAMLE_COMM_VDEC_UnBindVpss(VDEC_CHN VdChn, VPSS_GRP VpssGrp)
{
	HI_S32 s32Ret = HI_SUCCESS;
	MPP_CHN_S stSrcChn;
	MPP_CHN_S stDestChn;
	
	stSrcChn.enModId = HI_ID_VDEC;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = VdChn;
	
	stDestChn.enModId = HI_ID_VPSS;
	stDestChn.s32DevId = VpssGrp;
	stDestChn.s32ChnId = 0;
	
	s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
	if(s32Ret != HI_SUCCESS)
	{
		printf("HI_MPI_SYS_UnBind failed with %#x!\n", s32Ret);
		return HI_FAILURE;
	}
	
	return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VO_BindVpss(VO_LAYER VoLayer,VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;

    stDestChn.enModId = HI_ID_VOU;
    stDestChn.s32DevId = VoLayer;
    stDestChn.s32ChnId = VoChn;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        printf("SAMPLE_COMM_VO_BindVpss failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return s32Ret;
}
HI_S32 SAMPLE_COMM_VO_UnBindVpss(VO_LAYER VoLayer,VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;

    stDestChn.enModId = HI_ID_VOU;
    stDestChn.s32DevId = VoLayer;
    stDestChn.s32ChnId = VoChn;

    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        printf("SAMPLE_COMM_VO_UnBindVpss failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
	
    return s32Ret;
}

int HisiFBInit(SIZE_S *pstSize)
{
	int fd = -1;
	HI_BOOL bShow;
	HIFB_POINT_S stPoint;
	HI_S32 s32Ret = HI_SUCCESS;

	/* 1. open framebuffer device overlay 0 */
	fd = open("/dev/fb0", O_RDWR, 0);
	if(fd < 0)
	{
		printf("open %s failed!\n", "/dev/fb0");

		return HI_FAILURE;	
	} 

	bShow = HI_FALSE;
	if (ioctl(fd, FBIOPUT_SHOW_HIFB, &bShow) < 0)
	{
		printf("FBIOPUT_SHOW_HIFB failed!\n");
		s32Ret = HI_FAILURE;
		goto fail;
	}

	/* 2. set the screen original position */
	//设置叠加层在屏幕上显示的起始坐标
	
	stPoint.s32XPos = 0;
	stPoint.s32YPos = 0;

	if (ioctl(fd, FBIOPUT_SCREEN_ORIGIN_HIFB, &stPoint) < 0)
	{
		printf("set screen original show position failed!\n");
		s32Ret = HI_FAILURE;
		goto fail;
	}

	/* 3. get the variable screen info */
	struct fb_var_screeninfo var;
	if (ioctl(fd, FBIOGET_VSCREENINFO, &var) < 0)
	{
		printf("Get variable screen info failed!\n");
		s32Ret = HI_FAILURE;
		goto fail;
	}
	
	/* 4. modify the variable screen info
		  the screen size: IMAGE_WIDTH*IMAGE_HEIGHT
		  the virtual screen size: VIR_SCREEN_WIDTH*VIR_SCREEN_HEIGHT
		  (which equals to VIR_SCREEN_WIDTH*(IMAGE_HEIGHT*2))
		  the pixel format: ARGB1555
	*/
	
	var.xres_virtual = pstSize->u32Width;
	var.yres_virtual = pstSize->u32Height;
	var.xres = pstSize->u32Width;
	var.yres = pstSize->u32Height;
	/*
	var.transp= s_a16;
	var.red = s_r16;
	var.green = s_g16;
	var.blue = s_b16;
	var.bits_per_pixel = 16;
	*/
	
	var.transp= s_a32;
	var.red = s_r32;
	var.green = s_g32;
	var.blue = s_b32;
	var.bits_per_pixel = 32;
	var.activate = FB_ACTIVATE_NOW;
	
	/* 5. set the variable screeninfo */
	if (ioctl(fd, FBIOPUT_VSCREENINFO, &var) < 0)
	{
		printf("Put variable screen info failed!\n");
		s32Ret = HI_FAILURE;
		goto fail;
	}

	HIFB_ALPHA_S stAlpha;
	if (ioctl(fd, FBIOGET_ALPHA_HIFB, &stAlpha) < 0)
	{
		printf("Get alpha failed!\n");
		s32Ret = HI_FAILURE;
		goto fail;
	}
	
	stAlpha.bAlphaEnable = HI_TRUE;
	stAlpha.bAlphaChannel = 0x0;
	stAlpha.u8Alpha0 = 0x0;
	stAlpha.u8Alpha1 = 0x0;
	stAlpha.u8GlobalAlpha = 0xff;
	
	if (ioctl(fd, FBIOPUT_ALPHA_HIFB, &stAlpha) < 0)
	{
		printf("Set alpha failed!\n");
		s32Ret = HI_FAILURE;
		goto fail;
	}

	/* time to play */
	bShow = HI_TRUE;
	if (ioctl(fd, FBIOPUT_SHOW_HIFB, &bShow) < 0)
	{
		printf("FBIOPUT_SHOW_HIFB failed!\n");
		s32Ret = HI_FAILURE;
		goto fail;
	}
	
	//close(fd);//成功后不能关闭
	return HI_SUCCESS;

fail:
	close(fd);

	return s32Ret;
}

int HisiSysInit()
{
	HI_S32 s32Ret = HI_SUCCESS;
	SIZE_S stSize_dec, stSize_disp;

	stSize_dec.u32Width = 1920;
	stSize_dec.u32Height = 1200;
	
	stSize_disp.u32Width = 1024;
	stSize_disp.u32Height = 768;	

	//MPP version
	MPP_VERSION_S stMppVersion;
	HI_MPI_SYS_GetVersion(&stMppVersion);
	DBG_PRINT("Mpp version: %s\n", stMppVersion.aVersion);
	
	s32Ret = HisiMPPInit(&stSize_dec);
	if (HI_SUCCESS != s32Ret)
    {
        ERR_PRINT("HisiMPPInit failed!\n");
        goto END1;
    }
	/************************************************
	step2:  init mod common VB  hisi3535 TL没有设置
	*************************************************/
	/*
	SAMPLE_COMM_VDEC_ModCommPoolConf(&stModVbConf, PT_H264, &stSize);	
	s32Ret = SAMPLE_COMM_VDEC_InitModCommVb(&stModVbConf);
	if(s32Ret != HI_SUCCESS)
	{			
		printf("init mod common vb fail for %#x!\n", s32Ret);
		goto END1;
	}
	*/
	/************************************************
	step3:  start VDEC
	*************************************************/
	s32Ret = HisiVdecInit(&stSize_dec);
	if (HI_SUCCESS != s32Ret)
	{
        ERR_PRINT("HisiVdecInit failed!\n");
        goto END1;
    }

	/************************************************
	step4:  start VPSS
	*************************************************/
	s32Ret = HisiVpssInit(&stSize_dec);
	if (HI_SUCCESS != s32Ret)
	{
        ERR_PRINT("HisiVpssInit failed!\n");
        goto END2;
    }

	/************************************************
	step5:  start VO
	*************************************************/	
	s32Ret = HisiVoInit(&stSize_disp);
	if (HI_SUCCESS != s32Ret)
	{
        ERR_PRINT("HisivVoInit failed!\n");
        goto END3;
    }

	/************************************************
	step6:  VDEC bind VPSS
	*************************************************/	
	s32Ret = SAMLE_COMM_VDEC_BindVpss(0, 0);
	if (HI_SUCCESS != s32Ret)
	{
		ERR_PRINT("SAMLE_COMM_VDEC_BindVpss failed!\n");
		goto END4;
	}

	/************************************************
	step7:  VPSS bind VO
	*************************************************/
	s32Ret = SAMPLE_COMM_VO_BindVpss(VoLayer, 0, 0, VPSS_BSTR_CHN);
	if (HI_SUCCESS != s32Ret)
	{
		ERR_PRINT("SAMLE_COMM_VDEC_BindVpss failed!\n");
		goto END5;
	}

	/******************************
	step8:  hifb
	*******************************/
	s32Ret = HisiFBInit(&stSize_disp);
	if (HI_SUCCESS != s32Ret)
	{
		ERR_PRINT("HisiFBInit failed!\n");
		goto END6;
	}	
	
	DBG_PRINT("vo & hifb ready!\n");
	
	return HI_SUCCESS;


END6:
	SAMPLE_COMM_VO_UnBindVpss(VoLayer, 0, 0, VPSS_BSTR_CHN);
	
END5:
	SAMLE_COMM_VDEC_UnBindVpss(0, 0);
		
END4:
	HisiVoExit();

END3:
	HisiVpssExit();
	
END2:
	HisiVdecExit();// HisiVdecInit()	
	
END1:
	HisiMPPExit();// HisiMPPInit()
	
	return s32Ret;
}

int nvr_preview_vdec_write(int stream_chn, vdec_stream_s *pin_stream)
{
	VDEC_STREAM_S stStream;
	HI_S32 s32ret;
	memset(&stStream, 0, sizeof(VDEC_STREAM_S));

	stStream.pu8Addr = pin_stream->data;
	stStream.u32Len = pin_stream->len;
	
	stStream.u64PTS = pin_stream->pts;
	stStream.bEndOfFrame  = HI_TRUE;
	stStream.bEndOfStream = HI_FALSE;
		

	s32ret = HI_MPI_VDEC_SendStream(stream_chn, &stStream, -1);
	
	if(HI_SUCCESS != s32ret)
	{
		printf("%s ***Error*** send to vdecchn%d failed, ret: 0x%x\n", \
			__func__, stream_chn, s32ret);
		return -1;
	}
	
	return pin_stream->len;
}




