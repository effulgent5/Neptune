#include "kiEngineFactory.h"
#include "kiEngineBase.h"

kiEngineFactory g_enginDB;

BOOL kiEngineFactory::init()
{
	return TRUE;
}

UINT kiEngineFactory::getResourceCount()
{
	UINT count = 0;
	mDBLock.lock();
	count = (UINT)mObjDB.size();
	mDBLock.unlock();
	return count;
}

void kiEngineFactory::finl()
{
	mDBLock.lock();
	map<kiEngineBase*, UINT>::iterator iter = mObjDB.begin();
	for ( ; iter != mObjDB.end(); ++iter)
	{
		if(iter->first != NULL)
			kiDelete2 iter->first;
	}
	mObjDB.clear();
	mDBLock.unlock();
}

kiEngineBase* kiEngineFactory::create(const tstring& className, void* initData, UINT dataSize)
{
	kiEngineBase* object = kiEngineBase::factoryCreateExactly(className, initData, dataSize);

	mDBLock.lock();
	if(object != NULL)
	{
		mObjDB[object] = 1;
	}
	mDBLock.unlock();

	return object;
}

UINT kiEngineFactory::obtain(kiEngineBase* obj)
{
	UINT count = 0;
	mDBLock.lock();

	map<kiEngineBase*, UINT>::iterator iter = mObjDB.find(obj);
	if(iter != mObjDB.end())
	{
		++(iter->second);
		count = iter->second;
	}

	mDBLock.unlock();

	return count;
}

UINT kiEngineFactory::free(kiEngineBase* obj)
{
	UINT count = 0;
	mDBLock.lock();

	map<kiEngineBase*, UINT>::iterator iter = mObjDB.find(obj);
	if(iter != mObjDB.end())
	{
		if(iter->second == 1)
		{
			iter->first->finl();
			kiDelete2 iter->first;
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

UINT kiEngineFactory::getRefCount(kiEngineBase* obj)
{
	UINT count = 0;
	mDBLock.lock();

	map<kiEngineBase*, UINT>::iterator iter = mObjDB.find(obj);
	if(iter != mObjDB.end())
		count = iter->second;

	mDBLock.unlock();
	return count;
}