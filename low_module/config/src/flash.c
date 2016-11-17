#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>

#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#endif

#include "zlib.h"
#include "flash.h"
#include "glb_error_num.h"

#define ZIP_BUF_LEN ((64 << 10) - 100)

s32 ReadFromEEPROM(int fd,unsigned int offset,char *buffer,unsigned int len, u32 flashSizeLimit)
{
	if(fd == -1 || buffer == NULL)
	{
		return -1;
	}
	if(offset + len > flashSizeLimit)
	{
		return -1;
	}

	off_t ret1 = lseek(fd,0,SEEK_END);
	if(ret1 == -1 || offset + len > ret1)
	{
		return -1;
	}
	off_t ret2 = lseek(fd,offset,SEEK_SET);
	if(ret2 == -1)
	{
		return -1;
	}
	ssize_t size = read(fd,buffer,len);
	if(size != len)
	{
		return -1;
	}
	return 0;
}

s32 ReadZipConf(const s8 *pDevPath, u32 offset, s8** pData, u32* length, u32 flashSizeLimit)
{
	if(NULL == pDevPath)
	{
		ERR_PRINT("param pDevPath failed\n");
		return -EPARAM;
	}

	if(NULL == pData)
	{
		ERR_PRINT("param pData failed\n");
		return -EPARAM;
	}
	
	if(NULL == length)
	{
		ERR_PRINT("param length failed\n");
		return -EPARAM;
	}
	
	int fd_dev = open(pDevPath, O_RDONLY);
	if (fd_dev < 0)
	{
		ERR_PRINT("open pDevPath failed\n");
		return -FAILURE;
	}
	
	SConfigFlashHead head;
	if(0 != ReadFromEEPROM(fd_dev, offset, (char *)&head, sizeof(head), flashSizeLimit))
	{
		ERR_PRINT("ReadFromEEPROM failed\n");
		goto fail1;
	}
	
	head.nConfSize = CF_LE_L(head.nConfSize);
	head.nFileSize = CF_LE_L(head.nFileSize);
	head.nCheckSum= CF_LE_L(head.nCheckSum);
	DBG_PRINT("config head info: size[%u][%u], checksum[0x%08x]\n", head.nConfSize, head.nFileSize, head.nCheckSum);
	
	if((0!=head.nConfSize)&&(0!=head.nFileSize)&&(head.nConfSize<=ZIP_BUF_LEN))
	{
		char zip_head_buf[ZIP_BUF_LEN];
		u32 unc_size = head.nFileSize;
		if(0 != ReadFromEEPROM(fd_dev, offset+64, zip_head_buf, head.nConfSize, flashSizeLimit))
		{
			ERR_PRINT("ReadFromEEPROM failed\n");
			goto fail1;
		}
		*length = head.nFileSize;
		*pData = (char*)calloc(head.nFileSize, 1);
		if (NULL == *pData)
		{
			ERR_PRINT("malloc failed!\n");
			goto fail1;
		}

		int rec = 0;
		rec = uncompress((char *)*pData,(unsigned long *)&unc_size,zip_head_buf,(unsigned long)(head.nConfSize));
		if(rec!= Z_OK)
		{
			ERR_PRINT("uncompress Error! \n");
			goto fail1;
		}
		
		u32 tmp = 0;
		u32 nNumOfZero = 0;
		u32 i;
		for(i = 0; i < head.nFileSize; i++)
		{
			tmp += (*(*pData + i));
			if((*(*pData + i) == 0) || (*(*pData + i) == '0'))
			{
				nNumOfZero++;
			}
		}
		tmp += (head.nFileSize - head.nConfSize);
		tmp |= (nNumOfZero << 16);
		
		if(tmp != head.nCheckSum)
		{
			ERR_PRINT("flash checksum failed!\n");
			goto fail1;
		}
		//printf("flash checksum OK! [0x%08x]\n", tmp);
		close(fd_dev);
		return SUCCESS;
	}
	
	//printf("head.nConfSize Error!\n");
	

fail1:
	close(fd_dev);
	return -FAILURE;
}

//offset : 写入地址相对于flash 分区起始地址偏移
//flashSizeLimit : flash 分区大小限制
s32 ReadFlashToFile(const s8 *pFilePath, const s8 *pDevPath, u32 offset, u32 flashSizeLimit)
{
	if(NULL == pFilePath)
	{
		ERR_PRINT("param pFilePath failed\n");
		return -EPARAM;
	}
	
	if(NULL == pDevPath)
	{
		ERR_PRINT("param pDevPath failed\n");
		return -EPARAM;
	}
	
	s8* pData = NULL;
	u32 len = 0;
	if(0 != ReadZipConf(pDevPath, offset, &pData, &len, flashSizeLimit))
	{
		DBG_PRINT("Warnning: system will have to use config backuped!\n");
		if(0 != ReadZipConf(pDevPath, offset+(flashSizeLimit>>1), &pData, &len, flashSizeLimit))
		{
			ERR_PRINT("Warnning: read config backuped failed!\n");
			return -FAILURE;
		}
	}
	
	FILE* fp;
	fp = fopen(pFilePath, "w");
	if(NULL == fp)
	{
		ERR_PRINT("open file[%s] error!\n", pFilePath);
		return -FAILURE;
	}
	
	u32 i;
	for(i = 0; i < len; i++)
	{
		fputc(*(pData+i), fp);
	}
	
	fclose(fp);
	
	if (pData)
		free(pData);

	return 0;
}

s32 WriteToEEPROM(int fd,unsigned int offset,char *buffer,unsigned int len)
{
	s32 ret = 0;
	if(fd == -1 || buffer == NULL)
	{
		return -1;
	}
	//offset += g_sConfigDevInfo.nFlashOffset;
	
	off_t rett = lseek(fd,offset,SEEK_SET);
	if(rett == -1)
	{
		return -1;
	}
	ssize_t size = write(fd,buffer,len);
	if(size != len)
	{
		return -1;
	}
	
	//fdatasync(fd);
	fsync(fd);//csp modify
	//sync();//csp modify
	
	//usleep(1);
	
	return ret;
}

s32 SetZipConf(int dev_fd, u32 offset, s8* pData, u32 len, u32 limit)
{
	SConfigFlashHead head;
	head.nFileSize = len;
	char zip_head_buf[ZIP_BUF_LEN];
	u32 size_buf = ZIP_BUF_LEN;

	printf("%s offset: %u, zip_head_buf: %p, size_buf: %u, pdata: %p, head.nFileSize: %u\n", __func__, offset, zip_head_buf, size_buf, pData, (unsigned long)head.nFileSize);
	int rec = compress(zip_head_buf,(unsigned long*)&size_buf,(char *)pData,(unsigned long)head.nFileSize);
	if(rec!= Z_OK)
	{
		printf("compress Error! rec = %d, errno: %d, %s\n", rec, errno, strerror(errno));
		printf("%s offset: %u, zip_head_buf: %p, size_buf: %u, pdata: %p, head.nFileSize: %u\n", __func__, offset, zip_head_buf, size_buf, pData, (unsigned long)head.nFileSize);
		return -1;
	}
	head.nConfSize = CT_LE_L((u32)size_buf);
	head.nCheckSum = head.nFileSize - head.nConfSize;

	if (size_buf > limit)
	{
		printf("Warnning: will have no space to backup config(%u, %u)!\n", size_buf, limit);
		return -2;
	}
	
	u32 tmp = 0;
	u32 nNumOfZero = 0;
	u32 i;
	for(i = 0; i < len; i++)
	{
		tmp += pData[i];
		if((pData[i] == 0) || (pData[i] == '0'))
		{
			nNumOfZero++;
		}
	}
	head.nCheckSum += tmp;
	head.nCheckSum |= (nNumOfZero << 16);
	
	s32 ret = 0;
	ret = WriteToEEPROM(dev_fd,offset+64,zip_head_buf,size_buf);
	WriteToEEPROM(dev_fd,offset,(char *)&head,sizeof(head));
	
	return ret;
}







