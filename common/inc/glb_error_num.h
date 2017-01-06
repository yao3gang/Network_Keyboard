#ifndef __GLB_ERROR_NUM_H__
#define __GLB_ERROR_NUM_H__

enum GLB_ERROR_NUM
{
	SUCCESS = 0,
	FAILURE,/* 无需或无法描述的错误 */
	EPARAM,	/* 参数有误 */
	/* 数据通信 */
	ESTREAM_ID_CONFLICT, /* stream ID 冲突 */
	ETIMEOUT,
	ESEND,
	ERECV,
	
	/* CcircularBuffer */
	ESPACE,	/* 剩余空间不足 */
	EDATA,	/* 有效数据不足 */

	/* biz_device */
	EIP_CONFLICT, /* IP 冲突 */
	EDEV_NOTFOUND,	/* 设备未找到 */
	EDEV_OFFLINE,	/* 设备离线 */
	EDEVS_LIMIT, /* 设备数目到达上限 */

	ESYS_MODE,
};


#endif

