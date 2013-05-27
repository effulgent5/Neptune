#include "kiRenderFactory.h"
#include "kiRenderBase.h"

kiRenderFactory g_rendererDB;

BOOL kiRenderFactory::init()
{
	return TRUE;
}

UINT kiRenderFactory::getResourceCount()
{
	UINT count = 0;
	mDBLock.lock();
	count = (UINT)mObjDB.size();
	mDBLock.unlock();
	return count;
}

void kiRenderFactory::finl()
{
	mDBLock.lock();
	map<kiRenderBase*, UINT>::iterator iter = mObjDB.begin();
	for ( ; iter != mObjDB.end(); ++iter)
	{
		if(iter->first != NULL)
			kiDelete1 iter->first;
	}
	mObjDB.clear();
	mDBLock.unlock();
}

kiRenderBase* kiRenderFactory::create(const tstring& className, void* initData, UINT dataSize)
{
	kiRenderBase* object = kiRenderBase::factoryCreateExactly(className, initData, dataSize);

	mDBLock.lock();
	if(object != NULL)
	{
		mObjDB[object] = 1;
	}
	mDBLock.unlock();

	return object;
}

UINT kiRenderFactory::obtain(kiRenderBase* obj)
{
	UINT count = 0;
	mDBLock.lock();

	map<kiRenderBase*, UINT>::iterator iter = mObjDB.find(obj);
	if(iter != mObjDB.end())
	{
		++(iter->second);
		count = iter->second;
	}

	mDBLock.unlock();

	return count;
}

UINT kiRenderFactory::free(kiRenderBase* obj)
{
	UINT count = 0;
	mDBLock.lock();

	map<kiRenderBase*, UINT>::iterator iter = mObjDB.find(obj);
	if(iter != mObjDB.end())
	{
		if(iter->second == 1)
		{
			iter->first->finl();
			kiDelete1 iter->first;
			mObjDB.erase(iter);
		}
		else
		{
			--(iter->second);
			count = iter->second;
		}
	}

	mDBLock.unlock();

	return count;
}

UINT kiRenderFactory::getRefCount(kiRenderBase* obj)
{
	UINT count = 0;
	mDBLock.lock();

	map<kiRenderBase*, UINT>::iterator iter = mObjDB.find(obj);
	if(iter != mObjDB.end())
		count = iter->second;

	mDBLock.unlock();
	return count;
}