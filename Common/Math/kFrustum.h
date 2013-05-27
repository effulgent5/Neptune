#pragma once

#include "Memory/kiMemObject.h"
#include "kMath.h"
#include "systemCommon.h"
#include "commonDefines.h"

class COMMON_EI kFrustum : public kiMemObject
{
public:
	kFrustum(bool ortho = false)
		: mOrtho(ortho)
	{

	}
	kFrustum(float l, float r, float t, float b, float n, float f, bool ortho = false)
		: mLeft(l), mRight(r), mTop(t), mBottom(b), mNear(n), mFar(f), mOrtho(ortho)
	{

	}


	float mLeft, mRight, mTop, mBottom, mNear, mFar;
	bool mOrtho;
};
