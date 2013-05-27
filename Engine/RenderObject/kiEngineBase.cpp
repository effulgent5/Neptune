#include "kiEngineBase.h"
#include "kiEngineFactory.h"

stTypeClass* stTypeClass::ms_typeClassHead = NULL;
stTypeClass kiEngineBase::class_kiEngineBase = { TEXT("kiEngineBase"), sizeof(kiEngineBase), kiEngineBase::createObject, NULL, NULL };
static _LINK_DYNAMIC _link_kiRenderBase(&kiEngineBase::class_kiEngineBase);

stTypeClass* kiEngineBase::getTypeClass() const
{
	return &class_kiEngineBase;
}

kiEngineBase* kiEngineBase::createObject()
{
	return kiNew2 kiEngineBase;
}

kiEngineBase* kiEngineBase::factoryCreateExactly(const tstring& className, void* initData, UINT size)
{
	kiEngineBase* object = NULL;

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
					kiDelete2 object;
					object = NULL;
				}
			}
			break;
		}
		classType = classType->nextClass;
	}

	return object;
}

kiEngineBase::kiEngineBase()
{

}

kiEngineBase::~kiEngineBase()
{

}

UINT kiEngineBase::free()
{
	return g_enginDB.free(this);
}

UINT kiEngineBase::obtain()
{
	return g_enginDB.obtain(this);
}

UINT kiEngineBase::getRefCount()
{
	return g_enginDB.getRefCount(this);
}

BOOL kiEngineBase::init(VOID* initData, UINT size)
{
	return FALSE;
}

VOID kiEngineBase::finl()
{

}