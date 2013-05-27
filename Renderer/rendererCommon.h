#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include "commonCommon.h"
#include "rendererDefines.h"
#include "kiTimer.h"
#include "IRenderer.h"
#include "IStaticSolid.h"
#include "IBanal.h"
#include "IMetal.h"
#include "IImage.h"
//#include "ITimer.h"

extern "C" IRenderer* createRenderer(HWND wnd, UINT w, UINT h);
extern "C" UINT inspectRendererResource();