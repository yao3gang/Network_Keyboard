#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#endif

#include "types.h"
#include "public.h" 
#include "flash.h" 

int main(void)
{
	s32 tmp = 0;
	
	if(0 != PublicIniGetValue("/mnt/NetworkKeyboad/DevCustom.ini", \
		"DevCfg", \
		"CfgWay", \
		&tmp))
	{
		return -1;
	}
	DBG_PRINT("tmp: %d\n", tmp);
	
	s8 str[100];
	if(0 != PublicIniGetString("/mnt/NetworkKeyboad/DevCustom.ini", \
		"DevCfg", \
		"CfgFlashDev", \
		str,
		100))
	{
		return -1;
	}
	DBG_PRINT("CfgFlashDev: %s\n", str);
	
	s8 strarray[5][100];
	s8 *pstra[5];
	int i=0;
	for (i=0; i<5; ++i)
		pstra[i] = strarray[i];
	
	int ntab = 5;
	int realtab = 0;
	if(0 != PublicIniGetArrayStrings("/mnt/NetworkKeyboad/DevCustom.ini", \
		"DevCfg", \
		"CfgPathDefault", \
		pstra,
		&realtab,
		5,
		100))
	{
		return -1;
	}
	DBG_PRINT("realtab: %d\n", realtab);
	
	//for (i=0; i<realtab; ++i)
		DBG_PRINT("CfgPathDefault: %s\n", pstra[0]);
		DBG_PRINT("CfgPathDefault: %s\n", pstra[1]);

	ReadFlashToFile("/mnt/NetworkKeyboad/123", "/dev/mtd2", 0, 0x40000);
	
	return 0;
}



