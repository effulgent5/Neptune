#include "kiRenderParam.h"

IMPL_DYNAMIC(kiRenderParam, kiRenderBase)

kiRenderParam::kiRenderParam()
: mMaterial(NULL)
{

}

kiRenderParam::~kiRenderParam()
{

}

BOOL kiRenderParam::init(VOID* initData, UINT size)
{
	return TRUE;
}

VOID kiRenderParam::finl()
{

}