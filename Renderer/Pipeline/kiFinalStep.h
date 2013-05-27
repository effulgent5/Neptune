#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"
#include "kiStepBase.h"

class kiFinalStep : public kiStepBase
{
public:
	kiFinalStep(UINT ID);
	virtual ~kiFinalStep();

	virtual VOID updatePerStepCB();
};