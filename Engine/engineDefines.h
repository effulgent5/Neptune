#pragma once

#ifndef ENGINE_IMPORT
	#ifdef ENGINE_EXPORT
		#define ENGINE_EI __declspec(dllexport)
	#else
		#define ENGINE_EI __declspec(dllimport)
	#endif
#endif

#define kiAlloc2(size)	kiMemAlloc(size, 0, 0, 0)
#define kiFree2(p)		kiMemFree(p)

#define kiNew2			new(std::nothrow, 0, 0, 0)
#define kiDelete2		delete