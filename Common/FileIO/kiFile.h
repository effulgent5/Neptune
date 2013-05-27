#pragma once
#include "Memory/kiMemObject.h"
#include "systemCommon.h"
#include "commonDefines.h"

class COMMON_EI kiFile : public kiMemObject
{
public:
	kiFile();
	~kiFile();

	BOOL			openFile(const tstring& fileName, const tstring& mode);
	VOID			closeFile();
	BOOL			getStatus();
	UINT			getSize();

	UINT			read(void* buffer, unsigned int sizeToRead);
	UINT			write(void* buffer, unsigned int sizeToWrite);
	INT				setFPOffset(int offset);
	UINT			getFP();
	VOID			setFP2Begin();
	VOID			setFP2End();
private:
	FILE*			mFile;
};