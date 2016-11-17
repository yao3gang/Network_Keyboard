#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "glb_error_num.h"

//////////////////////////////////////////////////////////////////////////
// base types definition
// compilers should be restricted to ensure the following equalities!
typedef char					schar;	///< sizeof(uchar) == sizeof(schar) == sizeof(char) == 1
typedef unsigned char			uchar;	
typedef unsigned int			uint;	///< sizeof(uint) == sizeof(int) == 4
typedef unsigned short			ushort;	///< sizeof(ushort) == sizeof(short) == 2
typedef unsigned long			ulong;	///< sizeof(ulong) == sizeof(long) == 4

typedef unsigned char			BYTE;
typedef unsigned char			UCHAR;
typedef unsigned short			WORD;
typedef int						LONG;
typedef unsigned int			PARAM;
typedef unsigned int			DWORD;


typedef char					s8;	///< sizeof(uchar) == sizeof(schar) == sizeof(char) == 1
typedef unsigned char			u8;
typedef short					s16;	///< sizeof(ushort) == sizeof(short) == 2
typedef unsigned short			u16;	///< sizeof(ushort) == sizeof(short) == 2
typedef int						s32;
typedef unsigned int			u32;	///< sizeof(uint) == sizeof(int) == 4


typedef void*	VD_HANDLE;
typedef void*	HANDLE;
typedef int		VD_BOOL;
typedef int		BOOL;
typedef const char* VD_PCSTR;


#ifdef WIN32
typedef __int64					int64;	///< sizeof(int64) == sizeof(uint64) == 8
typedef unsigned __int64		uint64;
typedef __int64					s64;	///< sizeof(int64) == sizeof(uint64) == 8
typedef unsigned __int64		u64;

#elif defined(__GNUC__)
typedef long long				int64;
typedef unsigned long long		uint64;
typedef long long				s64;
typedef unsigned long long		u64;

#elif defined(__TCS__)
typedef signed   long long int	int64;
typedef unsigned long long int	uint64;
typedef signed   long long int	s64;
typedef unsigned long long int	u64;
#endif

typedef pthread_t THREADHANDLE;
typedef void* (*LINUXFUNC)(void*);


#define ERR_PRINT(fmt, args ... ) \
	do {fprintf(stderr, "Error : FILE -> %s, FUNC: %s, LINE -> %d" ": "   fmt, \
		__FILE__, __func__, __LINE__, ## args);} while(0)

#ifdef NDEBUG

#define DBG_PRINT(fmt, args ... )

#else

#define DBG_PRINT(fmt, args ... ) \
	do {fprintf(stderr, "DBG : FILE -> %s, FUNC: %s, LINE -> %d" ": "   fmt, \
		__FILE__, __func__, __LINE__, ## args);} while(0)

#endif


#define TRUE	1
#define FALSE	0
#define INVALID_FD (-1)

#ifndef MAX
#define MAX(a,b)				(((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)				(((a) < (b)) ? (a) : (b))
#endif

#ifndef ABS//csp modify
#define ABS(x) ((x)>=0?(x):-(x))
#endif

#define CF_LE_W(v) (v)
#define CF_LE_L(v) (v)
#define CT_LE_W(v) (v)
#define CT_LE_L(v) (v)

//thread priority fifo 0-29
#define TP_COM			 1		//1
#define TP_TIMER		 2		//1
#define TP_ALARM		 3		//1
#define TP_PLAY			 4		//4

#define TP_REC			 6
#define TP_CAPTURE	20		//16
#define TP_DHFILE		24		//1
#define TP_GUI			25		//1
#define TP_PTZ			26

//thread priority other 30-99
#define TP_SMTP     30
#define WATCH_DOG   31

#define TP_BACKUP		35		//1
#define TP_SVR			37		//2
#define TP_NET			39		//16
#define TP_TEL			55		//1
#define TP_FRONTBOARD	56		//1
#define TP_MOUSE		57		//1
#define TP_FTP			58
#define TP_WEB          58
#define TP_DEFAULT		64
#define TP_RTSP			65
#define TP_PRE			 127

#define PID_TIMER				0x10
#define PID_MAIN_CONTROL		0x20
#define PID_SYSLOG				0x30
#define PID_PANEL				0x40
#define PID_EVENT				0x50

#define THREAD_PRI_LOW			100
#define THREAD_PRI_MID			80
#define THREAD_PRI_HIGH			60

#define PRI_TIMER				THREAD_PRI_MID
#define PRI_UI					THREAD_PRI_HIGH
#define PRI_CP					THREAD_PRI_HIGH
#define PRI_CPCHECK				THREAD_PRI_LOW
#define PRI_CAPTURE				THREAD_PRI_HIGH
#define PRI_DISPLAY				THREAD_PRI_HIGH
#define PRI_ENCODE				THREAD_PRI_HIGH
#define PRI_DECODE				THREAD_PRI_HIGH
#define PRI_RECORD				THREAD_PRI_HIGH
#define PRI_MEDIASND			THREAD_PRI_HIGH
#define PRI_MEDIARCV			THREAD_PRI_HIGH
#define PRI_AUDIO				THREAD_PRI_MID
#define PRI_ALARM				THREAD_PRI_MID
#define NTP_CLIENT				THREAD_PRI_MID

#define THREAD_STKSIZE_DEFAULT	(8<<20)

#define STKSIZE_TIMER			(256<<10)
#define STKSIZE_UI				(4<<20)
#define STKSIZE_CP				(512<<10)
#define STKSIZE_CPCHECK			(256<<10)
#define STKSIZE_CAPTURE			(1<<20)
#define STKSIZE_DISPLAY			(1<<20)
#define STKSIZE_ENCODE			(1<<20)
#define STKSIZE_DECODE			(1<<20)
#define STKSIZE_RECORD			(2<<20)
#define STKSIZE_MEDIASND		(512<<10)
#define STKSIZE_MEDIARCV		(512<<10)
#define STKSIZE_AUDIO			(512<<10)
#define STKSIZE_ALARM			(256<<10)
#define STKSIZE_NTPCLIENT		(256<<10)

enum xm_msg_t {
	XM_MSG_SYSTEM = 0x000000,
		XM_QUIT,

	XM_MSG_LOC = 0x010000,
		XM_DHFILE_CREATE,	XM_DHFILE_OPEN,		XM_DHFILE_CLOSE,	XM_DHFILE_PACK,	
		XM_DHFILE_READ,		XM_DHFILE_WRITE,	XM_DHFILE_DATA,		XM_DHFILE_SYNC,
		XM_DHFILE_WAKEUP,
		XM_CAP_START,		XM_CAP_STOP,		XM_CAP_DATA,		XM_CAP_FLUSH, 
		XM_PLY_START,		XM_PLY_STOP,		XM_PLY_DATA,		XM_PLY_SYNC, 
		XM_BKP_DATA,
		
	XM_MSG_GUI = 0x020000,
		XM_KEEPALIVE,
		XM_KEYDOWN,		XM_KEYUP,
		XM_MOUSEFOUND,  XM_MOUSELOST,
		XM_MOUSEMOVE,	XM_MOUSEWHEEL,
		XM_LBUTTONDOWN,	XM_LBUTTONUP,	XM_LBUTTONDBLCLK,
		XM_RBUTTONDOWN,	XM_RBUTTONUP,	XM_RBUTTONDBLCLK,
		XM_MBUTTONDOWN,	XM_MBUTTONUP,	XM_MBUTTONDBLCLK,
		XM_DRAW,		XM_SCROLL,		XM_LIST,
		XM_NUMPAD,		XM_CARET,		XM_CHAR,
		XM_TIMER,		XM_INPUTTYPE,	XM_ACTIVATE,
		XM_DEACTIVATE,
		XM_KEYDOWNDELAY,
		XM_TIMETICK,

	XM_MSG_NET = 0x030000,
		XM_NET_START, XM_NET_STOP, XM_NET_DATA, XM_NET_FLUSH,
		XM_NMN_START, XM_NMN_STOP, XM_NMN_DATA, XM_NMN_FLUSH,
		XM_NPY_START, XM_NPY_STOP, XM_NPY_DATA, XM_NPY_FLUSH,
		XM_NUSR_STOP,
	
	XM_SYSTEM_END
};

//typedef void *					VD_HANDLE;
//typedef int						VD_BOOL;

#endif// __DH_TYPES_H__
