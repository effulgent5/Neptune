#include "kiEngineScene.h"
#include "kiEngineBase.h"
#include "kiSceneItem.h"
#include "kiEngine.h"
#include "kiEngineFactory.h"

IMPL_DYNAMIC(kiEngineScene, kiEngineBase)

kiEngineScene::kiEngineScene()
{

}

kiEngineScene::~kiEngineScene()
{

}

//UINT kiEngineScene::free()
//{
//	return g_engienDB.free(this);
//}
//
//UINT kiEngineScene::obtain()
//{
//	return g_engienDB.obtain(this);
//}
//
//UINT kiEngineScene::getRefCount()
//{
//	return g_engienDB.getRefCount(this);
//}

BOOL kiEngineScene::init(VOID* initData, UINT size)
{
	return TRUE;
}

VOID kiEngineScene::finl()
{

}

IRenderItem* kiEngineScene::createItem(const tstring& className, void* initData, UINT size)
{
	kiSceneItem* item = (kiSceneItem*)g_enginDB.create(className, initData, size);
	IRenderItem* inter = reinterpret_cast<IRenderItem*>(item);
	return inter;
}

void kiEngineScene::appendItem(IRenderItem* item)
{

}