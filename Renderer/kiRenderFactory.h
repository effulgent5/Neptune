#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"

class kiRenderBase;

class kiRenderFactory : public kiMemObject
{
public:
	kiRenderFactory(){}
	~kiRenderFactory(){}

	BOOL			init();
	VOID			finl();
	kiRenderBase*	create(const tstring& className, void* initData, UINT dataSize);
	UINT			obtain(kiRenderBase* obj);
	UINT			free(kiRenderBase* obj);
	UINT			getRefCount(kiRenderBase* obj);

	UINT			getResourceCount();

protected:
	kiLock						mDBLock;
	map<kiRenderBase*, UINT>	mObjDB;
};

extern kiRenderFactory g_rendererDB;