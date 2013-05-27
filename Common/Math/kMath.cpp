#include "kMath.h"

float kFastATan2(float fY, float fX)
{
	// Poly approximation valid for |z| <= 1.  To compute ATAN(z)
	// for z > 1, use ATAN(z) = PI/2 - ATAN(1/z).  For z < -1, use
	// ATAN(z) = -PI/2 - ATAN(1/z).

	if (fX == 0.0f && fY == 0.0f)
		return 0.0f;

	float fOffset = 0.0f;
	float fZ;
	if (kAbs(fY) > kAbs(fX))
	{
		//  |fY/fX| > 1 so use 1/z identities.
		fZ = fX / fY;
		if (fZ > 0.0f)
		{
			fOffset = K_HALF_PI;
		}
		else if (fZ < 0.0f)
		{
			fOffset = -K_HALF_PI;
		}
		else // fZ == 0.0f
		{
			// special case for 90deg and -90deg
			return (fY > 0.0f) ? K_HALF_PI : -K_HALF_PI;
		}
	}
	else
	{
		fZ = fY / fX;

		if (fZ == 0.0f)
		{
			// special case for 0deg and 180deg
			return (fX > 0.0f) ? 0.0f : K_PI;
		}
	}

	float fZ2 = fZ * fZ;

	// Polynomial approximation of degree 9, P(z).
	// |ATAN(z)-P(z)| <= 1e-05

	float fResult;
	fResult = 0.0208351f;
	fResult *= fZ2;
	fResult -= 0.0851330f;
	fResult *= fZ2;
	fResult += 0.1801410f;
	fResult *= fZ2;
	fResult -= 0.3302995f;
	fResult *= fZ2;
	fResult += 0.9998660f;
	fResult *= fZ;

	if (fOffset)
		fResult = fOffset - fResult;

	// find proper solution to two arg arctan
	if (fY < 0.0f && fX < 0.0f)  // quadrant IV
		fResult -= K_PI;
	if (fY > 0.0f && fX < 0.0f)  // quadrant II
		fResult += K_PI;

	// NIASSERT(NiAbs(fResult - NiATan2(fY, fX)) <= 1e-05);

	return fResult;
}
