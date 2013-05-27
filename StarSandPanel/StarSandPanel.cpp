#include "ssEditor.h"

extern ClassDesc2* GetEditorDesc();

HINSTANCE g_editorInstance;

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved)
{
	if( fdwReason == DLL_PROCESS_ATTACH )
	{
		g_editorInstance = hinstDLL;
		DisableThreadLibraryCalls(g_editorInstance);
	}

	return (TRUE);
}

extern "C" SSPANEL_EI const TCHAR* LibDescription()
{
	return _TEXT("Neptune starsand@ editor plugin.");
}

extern "C" SSPANEL_EI int LibNumberClasses()
{
   return 1;
}

extern "C" SSPANEL_EI ClassDesc* LibClassDesc(int i)
{
	switch(i)
	{
	case 0:
		return GetEditorDesc();
	default:
		return 0;
	}
}

extern "C" SSPANEL_EI ULONG LibVersion()
{
	return VERSION_3DSMAX;
}