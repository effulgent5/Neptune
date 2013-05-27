#pragma once

#ifndef RENDERER_IMPORT
	#ifdef RENDERER_EXPORT
		#define RENDERER_EI __declspec(dllexport)
	#else
		#define RENDERER_EI __declspec(dllimport)
	#endif
#endif

#define kiAlloc1(size)	kiMemAlloc(size, 0, 0, 0)
#define kiFree1(p)		kiMemFree(p)

#define kiNew1			new(std::nothrow, 0, 0, 0)
#define kiDelete1		delete

#define RELEASE_APP_RES(ref)	if(ref != NULL) {ref->Release(); ref = NULL;}
#define FINL_APP_PT(pt)	if(pt != NULL) {pt->finl(); kiDelete1 pt; pt = NULL;}
#define DEL_APP_PT(pt)	if(pt != NULL) {kiDelete1 pt; pt = NULL;}