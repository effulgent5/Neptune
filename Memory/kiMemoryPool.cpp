#include "kiMemoryPool.h"
#include <map>
using std::map;

kiMemoryPool* kiMemoryPool::msPool = NULL;
map<void*, unsigned int> g_memRecord;

kiMemoryPool::kiMemoryPool()
{

}

kiMemoryPool::~kiMemoryPool()
{

}

kiMemoryPool* kiMemoryPool::get()
{
	if(msPool == NULL)
	{
		kiMemoryPool* pool = new kiMemoryPool();
		msPool = pool;
	}

	return msPool;
}

void kiMemoryPool::create()
{
	kiMemoryPool* pool = kiMemoryPool::get();
	if(pool == NULL)
		__asm int 3;
}

void kiMemoryPool::destroy()
{
	if(msPool != NULL)
	{
		if(g_memRecord.size() > 0)
			::MessageBox(NULL, TEXT("Memory leak! fuck man!"), TEXT("Memory Leak Error!"), MB_ICONERROR);

		delete msPool;
		msPool = NULL;
	}
}

void* kiMemoryPool::alloc(UINT size, UINT flag)
{
	void* p = ::malloc(size);

	if(p != NULL)
		g_memRecord[p] = (unsigned int)size;

	return p;
}

void kiMemoryPool::free(void* p)
{
	if(p == NULL)
	{
		__asm int 3;
		return;
	}

	map<void*, unsigned int>::iterator iter = g_memRecord.find(p);
	if(iter != g_memRecord.end())
		g_memRecord.erase(iter);
	else
		__asm int 3;

	::free(p);
}