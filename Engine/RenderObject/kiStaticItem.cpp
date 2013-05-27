#include "kiStaticItem.h"
#include "kiEngine.h"
#include "kiMeshParse.h"

IMPL_DYNAMIC(kiStaticItem, kiSceneItem)

kiStaticItem::kiStaticItem()
{
	mSolid = NULL;
}

kiStaticItem::~kiStaticItem()
{

}

BOOL kiStaticItem::init(VOID* initData, UINT size)
{
	unsigned char* byteData = (unsigned char*)initData;
	IRenderer* renderer = kiEngine::getInstance();
	if(renderer != NULL)
	{
		kiMeshParse meshParse(initData, size);
		UINT v_start = 0, v_size = 0, i_start = 0, i_size = 0;
		BOOL result = meshParse.getMeshBias((UINT)0, v_start, v_size, i_start, i_size);
		if(!result)
			return FALSE;

		struct paramData
		{
			VOID* v;
			UINT vs;
			VOID* i;
			UINT is;
		};
		paramData pd = { (VOID*)(byteData+v_start), v_size, (VOID*)(byteData+i_start), i_size };
		mSolid = (IStaticSolid*)renderer->createItem(TEXT("kiStaticSolid"),(VOID*)&pd, sizeof(paramData));
		if(mSolid != NULL)
			return TRUE;
	}

	return FALSE;
}

VOID kiStaticItem::finl()
{
	if(mSolid != NULL)
	{
		mSolid->free();
		mSolid = NULL;
	}
}

IStaticSolid* kiStaticItem::getSolid()
{
	return mSolid;
}

IRenderParam* kiStaticItem::getMatParam()
{
	return NULL;
}