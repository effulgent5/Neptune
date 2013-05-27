#include "kiStepBase.h"

kiStepBase::kiStepBase(UINT ID)
	: mStepID(ID)
{
}

kiStepBase::~kiStepBase()
{

}

BOOL kiStepBase::init()
{
	return TRUE;
}

VOID kiStepBase::finl()
{

}

VOID kiStepBase::draw()
{

}

VOID kiStepBase::clear()
{
	mRenderSequence.clear();
}

VOID kiStepBase::insert(kiSolid* s, kiTransform* w, kiRenderParam* p)
{
	mRenderSequence.push_back(stRenderObj(s, w, p));
}

VOID kiStepBase::updatePerStepCB()
{

}