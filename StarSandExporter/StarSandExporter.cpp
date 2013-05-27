#include "commonExporter.h"
#include "ssExporter.h"

BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


extern "C" SSEXPORTER_EI iExporter* createExporter()
{
	iExporter* exporter = new ssExporter();
	if(exporter != NULL)
		return exporter;
	else
		return NULL;
}