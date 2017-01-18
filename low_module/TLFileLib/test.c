#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "types.h"
#include "TLFileLib.h"

int main()
{
	char fly_file_path[128];
	char avi_file_path[128];
	
	strcpy(fly_file_path, "./udisk/chn220130415122615.ifv");
	strcpy(avi_file_path, fly_file_path);
	strcpy(avi_file_path+strlen(fly_file_path)-3, "avi");//ºó×ºÃûifv to avi
	
	DBG_PRINT("fly_file_path: %s, avi_file_path %s\n", fly_file_path, avi_file_path);
	
	TLFILE_t hfile = TL_OpenFile(fly_file_path, TL_FILE_READ);
	DBG_PRINT("VideoFrameNum: %d, AudioFrameNum: %d\n", 
		TL_FileVideoFrameNum(hfile), TL_FileAudioFrameNum(hfile));
	TL_CloseFile(hfile);
	
	return 0;
}