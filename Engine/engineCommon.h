#pragma once
#include "rendererCommon.h"
#include "engineDefines.h"
#include "IEngine.h"
#include "IEngineBase.h"
#include "ISceneItem.h"
#include "IEngineScene.h"
#include "IStaticItem.h"

extern "C" IEngine* createEngine(HWND wnd, UINT w, UINT h);
extern "C" UINT inspectEngineResource();