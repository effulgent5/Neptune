#include "kiEngine.h"
#include "kiEngineScene.h"
#include "kiEngineFactory.h"

IRenderer* kiEngine::ms_renderer = NULL;
kiEngine* kiEngine::ms_engine = NULL;

IMPL_DYNAMIC(kiEngine, kiEngineBase)

struct stEngineData
{
	HWND wnd;
	UINT w;
	UINT h;
};

kiEngine::kiEngine()
{
	mCreateThread = ::GetCurrentThreadId();
}

kiEngine::~kiEngine()
{

}

BOOL kiEngine::init(VOID* initData, UINT size)
{
	if(size != sizeof(stEngineData) || initData == NULL)
		return FALSE;

	ms_engine = this;

	stEngineData* structData = (stEngineData*)initData;
	ms_renderer = createRenderer(structData->wnd, structData->w, structData->h);
	g_enginDB.init();

	if(ms_renderer != NULL)
		return TRUE;
	else
		return FALSE;
}

VOID kiEngine::finl()
{
	if(ms_renderer != NULL)
	{
		ms_renderer->free();
		ms_renderer = NULL;
	}

	UINT leakCount = inspectRendererResource();

	ms_engine = NULL;
}

IRenderScene* kiEngine::createScene()
{
	kiEngineScene* scene = (kiEngineScene*)g_enginDB.create(TEXT("kiEngineScene"), NULL, 0);
	IRenderScene* inter = reinterpret_cast<IRenderScene*>(scene);
	return inter;
}

void kiEngine::setRenderScene(IRenderScene* scene)
{
	mCurScene = (kiEngineScene*)scene;
}

void kiEngine::tick()
{
	ms_renderer->begin(NULL);

	ms_renderer->end();
}

IRenderer* kiEngine::getInstance()
{
	return ms_renderer;
}

kiEngine* kiEngine::getEngine()
{
	return ms_engine;
}

extern "C" ENGINE_EI IEngine* createEngine(HWND wnd, UINT w, UINT h)
{
	kiEngine* engine = kiEngine::getEngine();
	if(engine == NULL)
	{
		stEngineData ed = { wnd, w, h };
		engine = (kiEngine*)g_enginDB.create(TEXT("kiEngine"), &ed, sizeof(stEngineData));
	}

	IEngine* inter = reinterpret_cast<IEngine*>(engine);
	return inter;
}

extern "C" ENGINE_EI UINT inspectEngineResource()
{
	UINT leakCount = g_enginDB.getResourceCount();
	g_enginDB.finl();
	return leakCount;
}
