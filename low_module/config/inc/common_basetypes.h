#ifndef _BASE_TYPES_H_
#define _BASE_TYPES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "types.h"

#ifndef TRUE
#define TRUE				1
#endif

#ifndef FALSE
#define FALSE				0
#endif

#ifndef BOOL
#define BOOL int
#endif

typedef struct
{
    u8 nR;
    u8 nG;
    u8 nB;
    u8 nTrans;
} SRgb;

#define PARAOUT
#ifdef WIN32
#define PACK_NO_PADDING
#else
#define PACK_NO_PADDING  __attribute__ ((__packed__))
#endif

#endif

