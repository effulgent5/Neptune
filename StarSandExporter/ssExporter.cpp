#include "ssExporter.h"
#include "kiMeshUtility.h"

ssExporter::ssExporter()
{

}

ssExporter::~ssExporter()
{

}

BOOL ssExporter::exportObj(const TCHAR* fileName)
{
	BOOL result = kiMeshUtility::exportGeo(TEXT("D:\\test\\"), TEXT("first"), this);
	if(result)
		::MessageBox(NULL, TEXT("export success!"), TEXT("Error"), MB_OK);
	else
		::MessageBox(NULL, TEXT("export failed!"), TEXT("Error"), MB_ICONERROR);
	return result;
}