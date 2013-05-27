#pragma once
#include "systemCommon.h"
#include "commonDefines.h"
#include <map>
#include <new>
using std::map;


//extern void* operator new(size_t size, const std::nothrow_t& nt, int i);
//extern void operator delete(void* p);
//
//extern void* operator new[](size_t size, const std::nothrow_t& nt, int i);
//extern void operator delete[](void* p);
extern COMMON_EI void*	kiMemAlloc(size_t size, int flag, unsigned int tick, unsigned int usrData);
extern COMMON_EI void	kiMemFree(void*);
//#error aaaaaaaaaaaaaaaaaaaaaaa
//static_assert(false, "abcd");
//#pragma warning(push)
//#pragma warning(disable : 4100)
class COMMON_EI kiMemObject
{
public:
	static void* operator new(size_t size, const std::nothrow_t& nt, int flag, unsigned int tick, unsigned int usrData);
	static void operator delete(void* p);
	static void operator delete(void* p, const std::nothrow_t& nt, int flag, unsigned int tick, unsigned int usrData){}

	static void* operator new[](size_t size, const std::nothrow_t& nt, int flag, unsigned int tick, unsigned int usrData);
	static void operator delete[](void* p);
	static void operator delete[](void* p, const std::nothrow_t& nt, int flag, unsigned int tick, unsigned int usrData){}
};
//#pragma warning(pop)