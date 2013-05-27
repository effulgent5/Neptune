#include "kiMemoryPool.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	//HANDLE heapHandle = GetProcessHeap();
	//HANDLE handle = HeapAlloc(heapHandle,0, 0xfffff);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		kiMemoryPool::create();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		kiMemoryPool::destroy();
		break;
	}
	return TRUE;
}

