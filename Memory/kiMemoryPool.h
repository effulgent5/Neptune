#pragma once
#include <windows.h>

#ifndef MEM_IMPORT
	#ifdef MEM_EXPORT
		#define MEM_EI __declspec(dllexport)
	#else
		#define MEM_EI __declspec(dllimport)
	#endif
#endif

class MEM_EI kiMemoryPool
{
public:
	~kiMemoryPool();
	static kiMemoryPool* get();

	static void create();
	static void destroy();

	void* alloc(UINT size, UINT flag);
	void  free(void*);
protected:
	kiMemoryPool();

	static kiMemoryPool* msPool;

private:

};