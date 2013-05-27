#include "kiMeshParse.h"

kiMeshParse::kiMeshParse(VOID* buffer, UINT size)
{
	mBuffer = buffer;
	mSize = size;
}

kiMeshParse::~kiMeshParse()
{

}

UINT kiMeshParse::getVersion()
{
	if(mBuffer == NULL)
		return 0;

	meshHeader* header = (meshHeader*)mBuffer;
	if(header->flag == 'iven')
		return header->version;
	else
		return 0;
}

UINT kiMeshParse::getMeshCount()
{
	if(mBuffer == NULL)
		return 0;

	meshHeader* header = (meshHeader*)mBuffer;
	if(header->flag == 'iven')
		return header->secNum;
	else
		return 0;
}

BOOL kiMeshParse::getMeshBias(UINT index, UINT& v_start, UINT& v_size, UINT& i_start, UINT& i_size)
{
	UINT meshCount = getMeshCount();
	if(meshCount == 0 || index >= meshCount)
		return FALSE;

	secDesc* secs = (secDesc*)((unsigned char*)mBuffer+sizeof(meshHeader));
	v_start = secs[index].v_start;
	v_size = secs[index].v_size;
	i_start = secs[index].i_start;
	i_size = secs[index].i_size;
	return TRUE;
}

BOOL kiMeshParse::getMeshBias(const char* name, UINT& v_start, UINT& v_size, UINT& i_start, UINT& i_size)
{
	UINT meshCount = getMeshCount();
	if(meshCount == 0)
		return FALSE;

	secDesc* secs = (secDesc*)((unsigned char*)mBuffer+sizeof(meshHeader));
	for(UINT i = 0; i < meshCount; ++i)
	{
		if(strcmp(secs[i].name, name) == 0)
		{
			v_start = secs[i].v_start;
			v_size = secs[i].v_size;
			i_start = secs[i].i_start;
			i_size = secs[i].i_size;
			return TRUE;
		}
	}

	return FALSE;
}