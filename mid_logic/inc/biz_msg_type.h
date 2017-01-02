#ifndef __BIZ_MSG_TYPE_H__
#define __BIZ_MSG_TYPE_H__

typedef enum
{
	
	//流相关消息
	EM_STREAM_MSG_CONNECT_SUCCESS,	//连接成功
	EM_STREAM_MSG_CONNECT_FALIURE,	//连接失败
	EM_STREAM_MSG_DEL_SUCCESS,		//删除流成功
	EM_STREAM_MSG_DEL_FALIURE,		//删除流失败
	EM_STREAM_MSG_ERR,		//流出错
	EM_STREAM_MSG_STOP,			//流关闭
	EM_STREAM_MSG_PROGRESS,		//文件回放/下载进度
	EM_STREAM_MSG_FINISH,		//文件下载完成
		//CMediaStreamManager 内部命令
	EM_STREAM_CMD_CONNECT,	//连接流
	EM_STREAM_CMD_DEL,		//删除流
} EM_BIZ_MSG_TYPE;



//CMediaStreamManager 消息结构

typedef struct 
{
	s32 msg_type;
		
	union		//72byte
	{
	//流相关数据结构
		//
		u32 stream_id;//关键，系统唯一
		
		//流错误码
		struct
		{
			u32 stream_id;//关键，系统唯一
			s32 stream_errno;//GLB_ERROR_NUM
		} stream_err;
		
		//回放、文件下载进度
		struct
		{
			u32 stream_id;//关键，系统唯一
			u32 cur_pos;
			u32 total_size;
		} stream_progress;						
		
		
	};
}SBizMsg_t;	



#endif

