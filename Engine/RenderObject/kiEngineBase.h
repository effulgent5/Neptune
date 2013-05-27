#pragma once
#include "rendererCommon.h"
#include "engineDefines.h"

///< rtti and dynamic creation support
class kiEngineBase;
struct stTypeClass
{
	tstring			className;
	unsigned int	classSize;
	kiEngineBase*	(*funcCreateObject)();
	stTypeClass*	baseClass;
	stTypeClass*	nextClass;
	static stTypeClass* ms_typeClassHead;
};

#define _DECLARE_DYNAMIC_(theClass)			\
	static stTypeClass class_##theClass;	\
	virtual stTypeClass* getTypeClass() const; \
	static kiEngineBase* createObject();	
	//static const char* getClassName();

struct _LINK_DYNAMIC
{
	_LINK_DYNAMIC(stTypeClass* newClassType)
	{
		newClassType->nextClass = stTypeClass::ms_typeClassHead;
		stTypeClass::ms_typeClassHead = newClassType;
	}
};

#define IMPL_DYNAMIC(theClass, parentClass)	\
	stTypeClass theClass::class_##theClass = { TEXT(#theClass), sizeof(theClass), theClass::createObject, &parentClass::class_##parentClass, NULL }; \
	static _LINK_DYNAMIC _link_##theClass(&theClass::class_##theClass); \
	stTypeClass* theClass::getTypeClass() const { return &theClass::class_##theClass; } \
	kiEngineBase* theClass::createObject() { return kiNew2 theClass; }					

class kiEngineBase : public kiMemObject
{
protected:
	kiEngineBase();
public:
	///< interface impl	
	virtual ~kiEngineBase();
	///< ICommon Interface
	virtual UINT KICALL_TYPE	free();
	virtual UINT KICALL_TYPE	obtain();
	virtual UINT KICALL_TYPE	getRefCount();

	///< IRenderBase Interface
	virtual BOOL KICALL_TYPE	init(VOID* initData, UINT size);
	virtual VOID KICALL_TYPE	finl();

	///< 必须放在所有接口之后
	_DECLARE_DYNAMIC_(kiEngineBase)

	friend class kiEngineFactory;
private:
	static kiEngineBase* factoryCreateExactly(const tstring& className, void* initData, UINT size);
};