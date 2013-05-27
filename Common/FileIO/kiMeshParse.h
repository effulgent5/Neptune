#pragma once
#include "Memory/kiMemObject.h"
#include "systemCommon.h"
#include "commonDefines.h"
#include "kiCommonFileStruct.h"

class COMMON_EI kiMeshParse : public kiMemObject
{
public:
	kiMeshParse(VOID* buffer, UINT size);
	~kiMeshParse();

	UINT getVersion();
	UINT getMeshCount();
	BOOL getMeshBias(UINT index, UINT& v_start, UINT& v_size, UINT& i_start, UINT& i_size);
	BOOL getMeshBias(const char* name, UINT& v_start, UINT& v_size, UINT& i_start, UINT& i_size);

protected:
	VOID*	mBuffer;
	UINT	mSize;
};