#pragma once
#include "commonExporter.h"

#define OBJ_NAME_LENGTH 16

struct secDesc
{
	char	name[OBJ_NAME_LENGTH];	///< mesh name
	UINT	flag;		///< 0bit(static or skin)
	UINT	v_start;
	UINT	v_size;
	UINT	i_start;
	UINT	i_size;
};

struct meshHeader
{
	UINT	flag;	///< nevi
	UINT	version;	///< 0x00000001
	UINT	secNum;
};

class kiMeshStream
{
public:
	kiMeshStream();
	~kiMeshStream();

	BOOL		open(const char* fileName, BOOL isExist = FALSE);
	VOID		close();

	UINT		write(void* buffer, UINT size);
	UINT		read(void* buffer, UINT bufferSize, UINT readNum);
	BOOL		movePointer(UINT absOffset);
	UINT		getPointer();
	UINT		getSize();
	BOOL		deleteFile();
protected:
	FILE*		mFile;
	string		mFileName;
};