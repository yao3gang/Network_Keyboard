#ifndef GUI_DEV_H
#define GUI_DEV_H

#include <map>
#include "types.h"

typedef struct GuiDev
{
    u8	maxChnNum;
    u8 	devicetype;
    u8  b_alive;    //是否在线
    u32	deviceIP;
    s32 dev_idx; //dev pool index

    GuiDev()
    : maxChnNum(0)
    , devicetype(0)
    , b_alive(0)
    , deviceIP(0)
    , dev_idx(-1)
    {

    }
} SGuiDev;

typedef std::map<u32, SGuiDev *> MAP_IP_DEV;   //IP需要 ntohl，好排序

#endif // GUI_DEV_H
