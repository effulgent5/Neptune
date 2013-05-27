#include "kiRenderBase.h"
#include "kiRenderFactory.h"

stTypeClass* stTypeClass::ms_typeClassHead = NULL;
stTypeClass kiRenderBase::class_kiRenderBase = { TEXT("kiRenderBase"), sizeof(kiRenderBase), kiRenderBase::createObject, NULL, NULL };
static _LINK_DYNAMIC _link_kikiGraphicBase(&kiRenderBase::class_kiRenderBase);

stTypeClass* kiRenderBase::getTypeClass() const
{
	return &class_kiRenderBase;
}

kiRenderBase* kiRenderBase::createObject()
{
	return kiNew1 kiRenderBase;
}

kiRenderBase* kiRenderBase::factoryCreateExactly(const tstring& className, void* initData, UINT size)
{
	kiRenderBase* object = NULL;

	stTypeClass* classType = stTypeClass::ms_typeClassHead;
	while(classType != NULL)
	{
		if(classType->className == className)
		{
			object = classType->funcCreateObject();
			if(object != NULL)
			{
				BOOL result = object->init(initData, size);
				if(!result)
				{
					object->finl();
					kiDelete1 object;
					object = NULL;
				}
			}
			break;
		}
		classType = classType->nextClass;
	}

	return object;
}

kiRenderBase::kiRenderBase()
{

}

kiRenderBase::~kiRenderBase()
{

}

UINT kiRenderBase::free()
{
	return g_rendererDB.free(this);
}

UINT kiRenderBase::obtain()
{
	return g_rendererDB.obtain(this);
}

UINT kiRenderBase::getRefCount()
{
	return g_rendererDB.getRefCount(this);
}

BOOL kiRenderBase::init(VOID* initData, UINT size)
{
	return FALSE;
}

VOID kiRenderBase::finl()
{

}