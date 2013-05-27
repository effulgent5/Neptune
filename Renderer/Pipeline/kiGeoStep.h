#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"
#include "kiStepBase.h"

class kiSolid;
class kiTransform;
class kiGeoStep : public kiStepBase
{
public:
	kiGeoStep(UINT ID);
	virtual ~kiGeoStep();

	virtual VOID updatePerStepCB();
	VOID insertSolid(kiSolid* s, kiTransform* w, kiRenderParam* p);
};