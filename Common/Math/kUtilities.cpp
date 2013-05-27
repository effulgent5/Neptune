#include "kUtilities.h"
#include "commonDefines.h"

kUtilities::kUtilities()
{

}

kUtilities::~kUtilities()
{

}

kMesh* kUtilities::createUnitSphere(unsigned int longitude, unsigned int latitude)
{
	return NULL;
}

kMesh* kUtilities::createBox(float x, float y, float z)
{
	return NULL;
}
//
//void kUtilities::mouseToRay(float mouseX, float mouseY, float screenW, float screenH, const kiCamera& camera,  kRay& ray)
//{
//	float fUnitizedX = (mouseX / screenW) * 2.0f - 1.0f;
//	float fUnitizedY = ((screenH - mouseY) / screenH) * 2.0f - 1.0f;
//	fUnitizedX *= camera.getFrustum().mRight;
//	fUnitizedY *= camera.getFrustum().mTop;
//
//	kVec3 front = camera.GetFront();
//	kVec3 up = camera.GetUp();
//	kVec3 right = camera.GetRight();
//
//	if (camera.getFrustum().mOrtho)
//	{
//		ray.mOrigin = camera.GetPosition() + right * fUnitizedX + up * fUnitizedY;
//		ray.mDirection = front;
//	}
//	else
//	{
//		ray.mOrigin = camera.GetPosition();
//		ray.mDirection = front + up * fUnitizedY + right * fUnitizedX;
//		ray.mDirection.normalize();
//	}
//}

unsigned short kUtilities::float32ToFloat16(float f)
{
	unsigned int Result;

	unsigned int IValue = ((unsigned int *)(&f))[0];
	unsigned int Sign = (IValue & 0x80000000U) >> 16U;
	IValue = IValue & 0x7FFFFFFFU;      // Hack off the sign

	if (IValue > 0x47FFEFFFU)
	{
		// The number is too large to be represented as a half.  Saturate to infinity.
		Result = 0x7FFFU;
	}
	else
	{
		if (IValue < 0x38800000U)
		{
			// The number is too small to be represented as a normalized half.
			// Convert it to a denormalized value.
			unsigned int Shift = 113U - (IValue >> 23U);
			IValue = (0x800000U | (IValue & 0x7FFFFFU)) >> Shift;
		}
		else
		{
			// Rebias the exponent to represent the value as a normalized half.
			IValue += 0xC8000000U;
		}

		Result = ((IValue + 0x0FFFU + ((IValue >> 13U) & 1U)) >> 13U)&0x7FFFU; 
	}
	return (unsigned short)(Result|Sign);
}