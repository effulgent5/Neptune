#pragma once
//
#ifndef COMMON_IMPORT
	#ifdef COMMON_EXPORT
		#define COMMON_EI __declspec(dllexport)
	#else
		#define COMMON_EI __declspec(dllimport)
	#endif
#endif

#define KICALL_TYPE			__stdcall

typedef unsigned long long	U64;
typedef unsigned int        UINT;

#define NULL 0

#define kiNew0			new(std::nothrow, 0, 0, 0)
#define kiDelete0		delete

#define kiAlloc0(size)	kiMemAlloc(size, 0, 0, 0)
#define kiFree0(p)		kiMemFree(p)

#ifdef _UNICODE
typedef wstring tstring;
#define tfopen	_wfsopen
#define tchrrfind wcsrchr
#define tchrfind wcschr
#define tstrcat wcscat_s
#define tstrcpy wcscpy_s
#define tstrlen wcslen
#define tstrcmp wcscmp
#else
typedef string tstring;
#define tfopen	_fsopen
#define tchrrfind strrchr
#define tchrfind strchr
#define tstrcat strcat_s
#define tstrcpy	strcpy_s
#define tstrlen strlen
#define tstrcmp strcmp
#endif

