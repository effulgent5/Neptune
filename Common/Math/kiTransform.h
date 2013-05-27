#include "Memory/kiMemObject.h"
#include "systemCommon.h"
#include "commonDefines.h"
#include "kMath.h"
#include "kVec.h"

class COMMON_EI kiTransform : public kiMemObject
{
public:


	kMatrix33 mRot;
	kVec3 mScale;
	kVec3 mPos;
}