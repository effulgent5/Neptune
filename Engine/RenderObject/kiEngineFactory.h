#pragma once
#include "rendererCommon.h"
#include "engineDefines.h"
class kiEngineBase;

class kiEngineFactory : public kiMemObject
{
public:
	kiEngineFactory(){}
	~kiEngineFactory(){}

	BOOL			init();
	VOID			finl();
	kiEngineBase*	create(const tstring& className, void* initData, UINT dataSize);
	UINT			obtain(kiEngineBase* obj);
	UINT			free(kiEngineBase* obj);
	UINT			getRefCount(kiEngineBase* obj);

	UINT			getResourceCount();

protected:
	kiLock						mDBLock;
	map<kiEngineBase*, UINT>	mObjDB;
};

extern kiEngineFactory g_enginDB;