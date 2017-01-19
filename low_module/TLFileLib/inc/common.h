#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <time.h>


#include "types.h"


#define PRI_RECMSGQ				THREAD_PRI_HIGH

#define THREAD_STKSIZE_DEFAULT	(8<<20)

#define STKSIZE_RECMSGQ			(256<<10)

typedef struct _MSGQueue
{
	u32 dwReadID;
	u32 dwWriteID;
	u32 dwMsgNumber;
	u32 dwMsgLength;
}MSGQueue,* MSGQHANDLE;

typedef struct
{
	u16 sender;
	u16 event;
	u32 arg;
}ifly_msg_t;

#define MAX_CHN_NUM 16

#define NO_WAIT			0
#define WAIT_FOREVER    -1

#ifndef WIN32
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#endif

#define CLEAR(x) memset (&(x), 0, sizeof (x))

#define CONVERT_ENDIAN(x)  ((x)<<24 | (x)>>24 | ((x)&0x00FF0000)>>8 | ((x)&0x0000FF00)<<8)

#ifndef WIN32
/* Enables or disables debug output */
#ifdef __DEBUG
#define DBG(fmt, args...) fprintf(stderr, "Debug: " fmt, ## args)
#else
#define DBG(fmt, args...)
#endif

#define ERR(fmt, args...) fprintf(stderr, "Error: " fmt, ## args)
#endif

#define  THREADAFFMASK	1

//FTP错误类型
#define FTP_SUCCESS						0//成功
#define FTP_ERROR_PARAM					1//参数错误
#define FTP_ERROR_SERVER				2//服务器不存在
#define FTP_ERROR_FILE					3//文件不存在
#define FTP_ERROR_USER					4//用户名不存在
#define FTP_ERROR_PASSWD				5//密码错误

#ifdef __cplusplus
extern "C" {
#endif

MSGQHANDLE IFly_CreateMsgQueue(u32 dwMsgNumber, u32 dwMsgLength);
void IFly_CloseMsgQueue(MSGQHANDLE hMsgQ);

int IFly_SndMsg(MSGQHANDLE hMsgQ, char *pchMsgBuf, u32 dwLen, int nTimeout);
int IFly_RcvMsg(MSGQHANDLE hMsgQ, char *pchMsgBuf, u32 dwLen, int nTimeout);

#ifdef WIN32
THREADHANDLE IFly_CreateThread(void* pvTaskEntry, char* szName, u8 byPriority, u32 dwStacksize, u32 dwParam, u16 wFlag, u32 *pdwTaskID);
#else
THREADHANDLE IFly_CreateThread(LINUXFUNC pvTaskEntry, char* szName, u8 byPriority, u32 dwStacksize, u32 dwParam, u16 wFlag, u32 *pdwTaskID);
#endif
BOOL IFly_ThreadExit();
BOOL IFly_ThreadTerminate(THREADHANDLE hTask);

int OpenDev(char *Dev);
int set_Parity(int fd, int databits, int stopbits, int parity);
int set_speed(int fd, int speed);

time_t read_rtc(int utc);
void write_rtc(time_t t, int utc);
int show_clock(int utc);

u32 GetLocalIp();
int SetLocalIp(u32 dwIp);
int GetHWAddr(char *pBuf);
u32 GetNetMask();
int SetNetMask(u32 dwIp);
u32 GetDefaultGateway();
int SetDefaultGateway(u32 dwIp);
u32 GetDNSServer();
int	SetDNSServer(u32 dwIp);

int ftpget(char *serverip,char *localfile,char *remotefile);
int ftpput(char *serverip,char *remotefile,char *localfile);

int mount_user(char *mounted_path,char *user_path);
int umount_user(char *user_path);

#ifdef __cplusplus
}
#endif

#endif
