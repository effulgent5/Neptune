#include "kiCamera.h"

IMPL_DYNAMIC(kiCamera, kiRenderBase)

kiCamera::kiCamera()
{

}

kiCamera::~kiCamera()
{

}

BOOL kiCamera::init(VOID* initData, UINT size)
{
	return TRUE;
}

VOID kiCamera::finl()
{

}

const kVec3& kiCamera::getUp()
{
	return mUp;
}

const kVec3& kiCamera::getRight()
{
	return mRight;
}

const kVec3& kiCamera::getFront()
{
	return mFront;
}

const kVec3& kiCamera::getPosition()
{
	return mPosition;
}

const kFrustum& kiCamera::getFrustum()
{
	return mFrustum;
}

void kiCamera::setUp(const kVec3& val)
{

}

void kiCamera::setRight(const kVec3& val)
{

}

void kiCamera::setFront(const kVec3& val)
{

}

void kiCamera::setPosition(const kVec3& val)
{

}

void kiCamera::setFrustum(const kFrustum& val)
{

}