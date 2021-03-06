#include "common_basetypes.h"
#include "kdvsys.h"
#include "public.h"

s32 PublicIniSetString(const s8* strFilePath, const s8* strSection, const s8* strItem, const s8* strNewValue)
{
	if((NULL == strFilePath) || (NULL == strSection) || (NULL == strItem) || (NULL == strNewValue))
	{
		fprintf(stderr, "Invalid para! function: %s\n", __FUNCTION__);
		return -1;
	}
	
	if(FALSE == SetRegKeyString(strFilePath, strSection, strItem, strNewValue))
	{
		fprintf(stderr, "Set String failed! function: %s\n", __FUNCTION__);
		return -1;
	}
	
	return 0;
}

s32 PublicIniSetValue(const s8* strFilePath, const s8* strSection, const s8* strItem, s32 nNewValue)
{
	if((NULL == strFilePath) || (NULL == strSection) || (NULL == strItem))
	{
		fprintf(stderr, "Invalid para! function: %s\n", __FUNCTION__);
		return -1;
	}
	
	if(FALSE == SetRegKeyInt(strFilePath, strSection, strItem, nNewValue))
	{
		fprintf(stderr, "Set Value failed! function: %s\n", __FUNCTION__);
		return -1;
	}
	
	return 0;
}

s32 PublicIniSetValueU64(const s8* strFilePath, const s8* strSection, const s8* strItem, u64 nNewValue)
{
	if((NULL == strFilePath) || (NULL == strSection) || (NULL == strItem))
	{
		fprintf(stderr, "Invalid para! function: %s\n", __FUNCTION__);
		return -1;
	}
	
	if(FALSE == SetRegKeyU64(strFilePath, strSection, strItem, nNewValue))
	{
		fprintf(stderr, "Set Value failed! function: %s\n", __FUNCTION__);
		return -1;
	}
	
	return 0;
}

s32 PublicIniSetArrayString(const s8* strFilePath, const s8* strSection, const s8* strItem, u16 nIndex, const s8* strNewtValue)
{
	if((NULL == strFilePath) || (NULL == strSection) || (NULL == strItem) || (NULL == strNewtValue))
	{
		fprintf(stderr, "Invalid para! function: %s\n", __FUNCTION__);
		return -1;
	}
	
	if(FALSE == SetStringToTableWithKey(strFilePath, strSection, strItem, nIndex+1, strNewtValue))
	{
		fprintf(stderr, "Set string failed! function: %s\n", __FUNCTION__);
		return -1;
	}
	
	return 0;
}

s32 PublicIniSetArrayValue(const s8* strFilePath, const s8* strSection, const s8* strItem, u16 nIndex, s32 nNewValue)
{
	if((NULL == strFilePath) || (NULL == strSection) || (NULL == strItem))
	{
		fprintf(stderr, "Invalid para! function: %s\n", __FUNCTION__);
		return -1;
	}
	
	if(FALSE == SetValueToTableWithKey(strFilePath, strSection, strItem, nIndex+1, nNewValue))
	{
		fprintf(stderr, "Set value failed! function: %s\n", __FUNCTION__);
		return -1;
	}
	
	return 0;
}

s32 PublicIniSetArrayValueU64(const s8* strFilePath, const s8* strSection, const s8* strItem, u16 nIndex, u64 nNewValue)
{
	if((NULL == strFilePath) || (NULL == strSection) || (NULL == strItem))
	{
		fprintf(stderr, "Invalid para! function: %s\n", __FUNCTION__);
		return -1;
	}
	
	if(FALSE == SetU64ValueToTableWithKey(strFilePath, strSection, strItem, nIndex+1, nNewValue))
	{
		fprintf(stderr, "Set value failed! function: %s\n", __FUNCTION__);
		return -1;
	}
	
	return 0;
}

