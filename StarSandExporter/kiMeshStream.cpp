#include "kiMeshStream.h"
#include "io.h"

kiMeshStream::kiMeshStream()
{
	mFile = NULL;
	mFileName = "";
}

kiMeshStream::~kiMeshStream()
{

}

BOOL kiMeshStream::open(const char* fileName, BOOL isExist)
{
	if(isExist)
	{
		if(_access(fileName, 0) < 0)
			return FALSE;
		else
			return TRUE;
	}

	errno_t result = fopen_s(&mFile, fileName, "wb+");
	if(result == EINVAL)
		return FALSE;

	mFileName = fileName;

	return TRUE;
}

VOID kiMeshStream::close()
{
	if(mFile != NULL)
	{
		fclose(mFile);
		mFile = NULL;
		mFileName = "";
	}
}

UINT kiMeshStream::write(void* buffer, UINT size)
{
	UINT writed = 0;
	if(mFile != NULL)
		writed = (UINT)fwrite(buffer, 1, size, mFile);
	return writed;
}

UINT kiMeshStream::read(void* buffer, UINT bufferSize, UINT readNum)
{
	UINT readBytes = 0;
	if(mFile != NULL)
		readBytes = (UINT)fread_s(buffer, bufferSize, 1, readNum, mFile);
	return readBytes;
}

BOOL kiMeshStream::movePointer(UINT absOffset)
{
	if(mFile == NULL)
		return FALSE;

	UINT curPos = getPointer();

	int i = fseek(mFile, absOffset, SEEK_SET);

	if(i == 0)
		return TRUE;
	else
		return FALSE;
}

UINT kiMeshStream::getPointer()
{
	UINT pt = 0;
	if(mFile != NULL)
		pt = ftell(mFile);

	return pt;
}

UINT kiMeshStream::getSize()
{
	UINT fileSize = 0;
	if(mFile != NULL)
	{
		int i = fseek(mFile, 0L, SEEK_END);
		if(i == 0)
			fileSize = ftell(mFile);
	}
	return fileSize;
}

BOOL kiMeshStream::deleteFile()
{
	if(mFile != NULL)
	{
		string str = mFileName;
		close();
		if(DeleteFile(str.c_str()) != 0)
			return TRUE;
	}

	return FALSE;
}