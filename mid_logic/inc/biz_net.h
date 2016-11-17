#ifndef __BIZ_NET_H__
#define __BIZ_NET_H__

//system & linux
#include <arpa/inet.h>

//C++
#include <map>
#include <vector>

//local
#include "types.h"
#include "C_Lock.h"
#include "object.h"

//外部接口
#ifdef __cplusplus
extern "C" {
#endif

int BizNetInit(void);
int BizNetGetNetParam(SConfigNetParam &snet_param);
int BizNetSetNetParam(SConfigNetParam &snet_param);
VD_BOOL BizisNetworkOk(void);


//外部接口
#ifdef __cplusplus
}
#endif



#endif

