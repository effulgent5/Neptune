#include "kiMemObject.h"
#include "kiMemoryPool.h"

//
void* kiMemAlloc(size_t size, int flag, unsigned int tick, unsigned int usrData)
{
	void* p = kiMemoryPool::get()->alloc(size, 0);
	return p;
}

void kiMemFree(void* p)
{
	if(p != NULL)
		kiMemoryPool::get()->free(p);
}

///<******************************************************************
void* kiMemObject::operator new(size_t size, const std::nothrow_t& nt, int flag, unsigned int tick, unsigned int usrData)
{
	void* p = kiMemoryPool::get()->alloc(size, 0);
	return p;
}

void kiMemObject::operator delete(void* p)
{
	if(p != NULL)
		kiMemoryPool::get()->free(p);
}

void* kiMemObject::operator new[](size_t size, const std::nothrow_t& nt, int flag, unsigned int tick, unsigned int usrData)
{
	void* p = kiMemoryPool::get()->alloc(size, 0);
	return p;
}

void kiMemObject::operator delete[](void* p)
{
	if(p != NULL)
		kiMemoryPool::get()->free(p);
}
