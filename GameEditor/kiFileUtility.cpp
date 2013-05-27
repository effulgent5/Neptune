#include "stdafx.h"
#include "kiFileUtility.h"

BOOL kiFileUtility::makeReadOnly(const TCHAR* fileName)
{
	if(fileName != NULL)
	{
		DWORD attr = GetFileAttributes(fileName);
		if((attr != INVALID_FILE_ATTRIBUTES) && ((attr & (~FILE_ATTRIBUTE_DIRECTORY)) != 0))
		{
			if((attr & FILE_ATTRIBUTE_READONLY) == 0)
			{
				DWORD readOnlyAttr = attr | FILE_ATTRIBUTE_READONLY;
				BOOL result = SetFileAttributes(fileName, readOnlyAttr);
				return result;
			}
		}
	}

	return FALSE;
}

BOOL kiFileUtility::makeWriteable(const TCHAR* fileName)
{
	if(fileName != NULL)
	{
		DWORD attr = GetFileAttributes(fileName);
		if((attr != INVALID_FILE_ATTRIBUTES) && ((attr & (~FILE_ATTRIBUTE_DIRECTORY)) != 0))
		{
			if((attr & FILE_ATTRIBUTE_READONLY) != 0)
			{
				DWORD writeableAttr = attr & (~FILE_ATTRIBUTE_READONLY);
				BOOL result = SetFileAttributes(fileName, writeableAttr);
				return result;
			}
		}
	}

	return FALSE;
}

BOOL kiFileUtility::stdPath(const TCHAR* src, TCHAR* dst)
{
	///< change '/' to '\\'. 
	if(src != NULL && dst != NULL)
	{
		UINT len = (UINT)strlen(src);
		if(len > 0)
		{
			int slashCount = 0;
			while(*src != 0)
			{
				if(*src == TEXT('/') || *src == TEXT('\\'))
				{
					*dst = TEXT('\\');
					++slashCount;
				}
				else
				{
					*dst = *src;
					slashCount = 0;
				}

				if(slashCount <= 1)
					dst++;

				 src++;
			}
			*dst = (TCHAR)0;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL kiFileUtility::extractPath(const TCHAR* src, TCHAR* path, UINT size)
{
	///< 注意路徑要標準化
	const TCHAR* pos = strrchr(src, TEXT('\\'));
	if(pos != NULL)
	{
		UINT sizeCopy = (unsigned char*)pos - (unsigned char*)src + sizeof(TCHAR);
		memcpy_s(path, size*sizeof(TCHAR), src, sizeCopy);
		*((TCHAR*)((unsigned char*)path + sizeCopy)) = TCHAR(0);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL kiFileUtility::extractExt(const TCHAR* src, TCHAR* path, UINT size)
{
	///< 注意路徑要標準化
	const TCHAR* pos = strrchr(src, TEXT('.'));
	if(pos != NULL)
	{
		const TCHAR* end = src + strlen(src);
		UINT sizeCopy = (unsigned char*)end - (unsigned char*)pos;
		memcpy_s(path, size*sizeof(TCHAR), (unsigned char*)pos + sizeof(TCHAR), sizeCopy);
		//*((TCHAR*)((unsigned char*)path + sizeCopy + sizeof(TCHAR))) = TCHAR(0);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL kiFileUtility::extractName(const TCHAR* src, TCHAR* path, UINT size)
{
	///< 注意路徑要標準化
	const TCHAR* pos0 = strrchr(src, TEXT('.'));
	const TCHAR* pos1 = strrchr(src, TEXT('\\'));
	if(pos0 != NULL && pos1 != NULL && pos0 - pos1 > 1)
	{
		UINT sizeCopy = (unsigned char*)pos0 - (unsigned char*)pos1 - sizeof(TCHAR);
		memcpy_s(path, size*sizeof(TCHAR), ((unsigned char*)pos1 + sizeof(TCHAR)), sizeCopy);
		*((TCHAR*)((unsigned char*)path + sizeCopy)) = TCHAR(0);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL kiFileUtility::getRootDir(TCHAR* path, UINT size)
{
	///< 生成shader目录
	HMODULE handle = ::GetModuleHandle(NULL);
	::GetModuleFileName(handle, path, MAX_PATH);
	TCHAR* lastSlash = NULL;
	lastSlash = strrchr(path, TEXT('\\'));
	if(lastSlash != NULL)
	{
		*lastSlash = 0;
		lastSlash = strrchr(path, TEXT('\\'));
		if(lastSlash != NULL)
		{
			*(lastSlash+1) = 0;
			return TRUE;
		}
	}
	
	memset(path, 0, size*sizeof(TCHAR));
	return FALSE;
}

BOOL kiFileUtility::getBinDir(TCHAR* path, UINT size)
{
	///< 生成shader目录
	HMODULE handle = ::GetModuleHandle(NULL);
	::GetModuleFileName(handle, path, MAX_PATH);
	TCHAR* lastSlash = NULL;
	lastSlash = strrchr(path, TEXT('\\'));
	if(lastSlash != NULL)
	{
		*(lastSlash+1) = 0;		
		return TRUE;
	}	
	memset(path, 0, size*sizeof(TCHAR));
	return FALSE;	
}

UINT kiFileUtility::dirEnum(const TCHAR* path, BOOL enumSubDir, EnuCallBack callback, void* userData, UINT dataSize, int maxDepth)
{
	mTotalFileNum = 0;
	mMaxDepth = maxDepth;
	mCallback = callback;
	mUserData = userData;
	mDataSize = dataSize;
	mEnumSubDir = enumSubDir;

	TCHAR temp[MAX_PATH];
	kiFileUtility::stdPath(path, temp);
	dirFileEnum_r(temp, 0);

	return mTotalFileNum;
}

void kiFileUtility::dirFileEnum_r(TCHAR* path, int curDepth)
{
	if((mMaxDepth != 0) && ((++curDepth) > mMaxDepth))
		return;

	int len = strlen(path);
	if(path == NULL || len <= 0)
		return;

	if(path[len-1] != TEXT('\\'))
		strcat_s(path, MAX_PATH, TEXT("\\*"));
	else
		strcat_s(path, MAX_PATH, TEXT("*"));

	WIN32_FIND_DATA fd;
	HANDLE fileHandle = FindFirstFile(path, &fd);
	if(fileHandle == INVALID_HANDLE_VALUE)
	{
		FindClose(fileHandle);
		return;
	}

	BOOL isDir = FALSE;
	BOOL loop = TRUE;
	while(loop)
	{
		path[len] = (TCHAR)0;
		strcat_s(path, MAX_PATH, fd.cFileName);

		isDir = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

		if(!isDir)
		{
			++mTotalFileNum;
			BOOL result = mCallback(path, isDir, mUserData, mDataSize);
			if(!result)
			{
				FindClose(fileHandle);
				return;
			}
		}
		else
		{
			if(strcmp(fd.cFileName, TEXT(".")) != 0 && strcmp(fd.cFileName, TEXT("..")) != 0)
			{
				if(mEnumSubDir)
				{
					strcat_s(path, MAX_PATH, TEXT("\\"));
					dirFileEnum_r(path, curDepth);
				}
			}
		}

		loop = FindNextFile(fileHandle, &fd);
	}

	FindClose(fileHandle);
	///< 恢复字符串
	path[len] = (TCHAR)0;
}