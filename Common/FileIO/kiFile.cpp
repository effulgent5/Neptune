#include "kiFile.h"

kiFile::kiFile()
	: mFile(NULL)
{

}

kiFile::~kiFile()
{
	closeFile();
}


BOOL kiFile::openFile(const tstring& fileName, const tstring& mode)
{
	mFile = tfopen(fileName.c_str(), mode.c_str(), _SH_DENYNO);
	if(mFile == NULL)
		return FALSE;
	else
		return TRUE;
}

VOID kiFile::closeFile()
{
	if(mFile != 0)
	{
		fclose(mFile);
		mFile = NULL;
	}
}

BOOL kiFile::getStatus()
{
	return mFile != NULL;
}

UINT kiFile::getSize()
{
	long curFP = ftell(mFile);
	fseek(mFile, 0, SEEK_END);
	UINT size = ftell(mFile);
	fseek(mFile, curFP, SEEK_SET);
	return size;
}

UINT kiFile::read(void* buffer, unsigned int sizeToRead)
{
	size_t st = fread(buffer, 1, sizeToRead, mFile);
	return (UINT)st;
}

UINT kiFile::write(void* buffer, unsigned int sizeToWrite)
{
	size_t st = fwrite(buffer, sizeToWrite, 1, mFile);
	return (UINT)st;
}

INT kiFile::setFPOffset(INT offset)
{
	return fseek(mFile, offset, SEEK_CUR);
}

UINT kiFile::getFP()
{
	return (unsigned int)ftell(mFile);
}

VOID kiFile::setFP2Begin()
{
	fseek(mFile, 0, SEEK_SET);
}

VOID kiFile::setFP2End()
{
	fseek(mFile, 0, SEEK_END);
}