#ifndef __HISI_SYS_H__
#define __HISI_SYS_H__

typedef struct{
	int rsv;
	unsigned long long pts;
	unsigned int len;
	unsigned char *data;
}vdec_stream_s;


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* Begin of #ifdef __cplusplus */

int HisiSysInit(void);
int nvr_preview_vdec_write(int stream_chn, vdec_stream_s *pin_stream);
int hisi_chn_start(int stream_chn);
int hisi_chn_stop(int stream_chn);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* Begin of #ifdef __cplusplus */


#endif

