#ifndef __BIZ_MSG_TYPE_H__
#define __BIZ_MSG_TYPE_H__

typedef enum
{
	
	//CMediaStream
	EM_STREAM_MSG_CONNECT_SUCCESS,	//连接成功
	EM_STREAM_MSG_CONNECT_FALIURE,	//连接失败
	EM_STREAM_MSG_STOP,			//流关闭，可能出错
	EM_STREAM_MSG_PROGRESS,		//文件回放/下载进度
	EM_STREAM_MSG_FINISH,		//文件下载完成
	
	//CMediaStreamManager 内部命令
	EM_STREAM_CMD_CONNECT,	//连接流
	EM_STREAM_CMD_DEL,		//删除流

	//playback
	EM_PLAYBACK_MSG_CONNECT_SUCCESS,	//连接成功
	EM_PLAYBACK_MSG_CONNECT_FALIURE,	//连接失败
	EM_PLAYBACK_MSG_STOP,			//流关闭，可能出错
	EM_PLAYBACK_MSG_PROGRESS,		//文件回放/下载进度
	EM_PLAYBACK_MSG_FINISH,		//文件回放完成

	//preview
	EM_PREVIEW_MSG_CONNECT_SUCCESS,	//连接成功
	EM_PREVIEW_MSG_CONNECT_FALIURE,	//连接失败
	EM_PREVIEW_MSG_STOP,			//流关闭，可能出错
	EM_PREVIEW_MSG_PROGRESS,		//文件回放/下载进度

	//file download
	EM_FILE_DOWNLOAD_MSG_CONNECT_SUCCESS,	//连接成功
	EM_FILE_DOWNLOAD_MSG_CONNECT_FALIURE,	//连接失败
	EM_FILE_DOWNLOAD_MSG_STOP,			//流关闭，可能出错
	EM_FILE_DOWNLOAD_MSG_PROGRESS,		//文件回放由NVR 端发出，下载和升级进度由本地接收进程发出
	EM_FILE_DOWNLOAD_MSG_FINISH,		//文件下载完成
	EM_FILE_CONVERT_MSG_FINISH,		//文件转换AVI 完成
} EM_BIZ_MSG_TYPE;



//CMediaStreamManager 消息结构

typedef struct 
{
	s32 msg_type;

	union UN_PART_CHN_T
	{
		u32 stream_id;//关键，系统唯一
		u32 playback_chn;
		u32 preview_chn;
	} un_part_chn;
		
	union UN_PART_DATA_T
	{
	//流相关数据结构
		
		//流错误码
		s32 stream_errno;// EM_STREAM_MSG_STOP
		
		//回放、文件下载进度
		struct
		{
			u32 cur_pos;
			u32 total_size;
		} stream_progress;
		
	} un_part_data;
}SBizMsg_t;	



#endif

