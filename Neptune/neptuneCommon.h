#pragma once
#include "engineCommon.h"

#define kiAlloc4(size)	kiMemAlloc(size, 0, 0, 0)
#define kiFree4(p)		kiMemFree(p)
