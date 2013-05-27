#pragma once
#include "systemCommon.h"
#include "commonDefines.h"

#define K_INFINITY FLT_MAX
#define K_PI       3.1415926535897932f
#define K_HALF_PI  1.5707963267948966f
#define K_TWO_PI   6.2831853071795865f
#define K_E        2.7182818284590452f

// 'float' wrappers for standard math functions
float kAbs(float fValue);
float kACos(float fValue);
float kASin(float fValue);
float kATan(float fValue);
float kATan2(float fNumerator, float fDenominator);
float kCos(float fRadians);
float kExp(float fValue);
float kFloor(float fValue);
float kFmod(float fNumerator, float fDenominator);
float kLog(float fValue);
float kPow(float fBase, float fExponent);
float kSign(float fValue);
float kSin(float fRadians);
void kSinCos(float fRadians, float& fSin, float& fCos);
float kSqr(float fValue);
float kSqrt(float fValue);

// min max clamp
float kMin(float fA, float fB);
float kMax(float fA, float fB);
int kMax(int iA, int iB);
float kClamp(float fValue, float fMin, float fMax);
int kMin(int iA, int iB);
int kMax(int iA, int iB);
int kClamp(int iValue, int iMin, int iMax);

// utility functions
bool kIsPowerOf2(unsigned int uiI);
#define NiLerp(T, START, FINISH) ((START) + ((T) * ((FINISH) - (START))))

// random number generation
void kSrand(unsigned int uiSeed);
int kRand();  // r in [0,RAND_MAX]
float kSymmetricRandom();  // r in [-1,1]
float kUnitRandom();  // r in [0,1]

// fast functions
unsigned int kFastLog(unsigned int uiNum);
float kFastInvSqrt(float fValue);
float kFastSqrt(float fValue);
float kFastATan2(float fY, float fX);


///<********************************************************************
inline float kAbs (float fValue)
{
	return float(fabs(fValue));
}
//---------------------------------------------------------------------------
inline float kACos (float fValue)
{
	if ( -1.0f < fValue )
	{
		if ( fValue < 1.0f )
			return float(acos(fValue));
		else
			return 0.0f;
	}
	else
	{
		return K_PI;
	}
}
//---------------------------------------------------------------------------
inline float kASin (float fValue)
{
	if ( -1.0f < fValue )
	{
		if ( fValue < 1.0f )
			return float(asin(fValue));
		else
			return K_HALF_PI;
	}
	else
	{
		return -K_HALF_PI;
	}
}
//---------------------------------------------------------------------------
inline float kATan (float fValue)
{
	return float(atan(fValue));
}
//---------------------------------------------------------------------------
inline float kATan2 (float fNumerator, float fDenominator)
{
	return float(atan2(fNumerator, fDenominator));
}
//---------------------------------------------------------------------------
inline float kCos (float fRadians)
{
	return float(cos(fRadians));
}
//---------------------------------------------------------------------------
inline float kExp (float fValue)
{
	return float(exp(fValue));
}
//---------------------------------------------------------------------------
inline float kFloor (float fValue)
{
	return float(floor(fValue));
}
//---------------------------------------------------------------------------
inline float kFmod (float fNumerator, float fDenominator)
{
	return float(fmod(fNumerator, fDenominator));
}
//---------------------------------------------------------------------------
inline float kLog (float fValue)
{
	return float(log(fValue));
}
//---------------------------------------------------------------------------
inline float kPow (float fBase, float fExponent)
{
	return float(pow(fBase, fExponent));
}
//---------------------------------------------------------------------------
inline float kSign (float fValue)
{
	return ( fValue > 0.0f ? +1.0f : ( fValue < 0.0f ? -1.0f : 0.0f ) );
}
//---------------------------------------------------------------------------
inline float kSin (float fRadians)
{
	return float(sin(fRadians));
}
//---------------------------------------------------------------------------
inline void kSinCos (float fRadians, float& fSin, float& fCos)
{
	float s, c;
	__asm fld fRadians
	__asm fsincos
	__asm fstp c
	__asm fstp s

	fSin = s;
	fCos = c;
}
//---------------------------------------------------------------------------
inline float kSqr (float fValue)
{
	return fValue*fValue;
}
//---------------------------------------------------------------------------
inline float kSqrt (float fValue)
{
	return float(sqrt(fValue));
}
//---------------------------------------------------------------------------
inline float kMin(float fA, float fB)
{
	return (fA < fB) ? fA : fB;
}
//---------------------------------------------------------------------------
inline float kMax(float fA, float fB)
{
	return (fA > fB) ? fA : fB;
}

//---------------------------------------------------------------------------
inline float kClamp(float fValue, float fMin, float fMax)
{
	fValue = kMax(fValue, fMin);
	fValue = kMin(fValue, fMax);
	return fValue;
}
//---------------------------------------------------------------------------
inline int kMin(int iA, int iB)
{
	return (iA < iB) ? iA : iB;
}
//---------------------------------------------------------------------------
inline int kMax(int iA, int iB)
{
	return (iA > iB) ? iA : iB;
}
//---------------------------------------------------------------------------
inline int kClamp(int iValue, int iMin, int iMax)
{
	iValue = kMax(iValue, iMin);
	iValue = kMin(iValue, iMax);
	return iValue;
}
//---------------------------------------------------------------------------
inline bool kIsPowerOf2(unsigned int uiI)
{
	if (!uiI)
		return false;

	return (uiI & (uiI - 1)) == 0;
}
//---------------------------------------------------------------------------
inline void kSrand (unsigned int uiSeed)
{
	srand(uiSeed);
}
//---------------------------------------------------------------------------
inline int kRand ()
{
	return rand();
}
//---------------------------------------------------------------------------
inline float kUnitRandom ()
{
	return float(kRand())/float(RAND_MAX);
}
//---------------------------------------------------------------------------
inline float kSymmetricRandom ()
{
	return -1.0f + 2.0f*float(kRand())/float(RAND_MAX);
}
//---------------------------------------------------------------------------
inline unsigned int kFastLog (unsigned int uiNum)
{
	unsigned int uiLog = 0;
	unsigned int uiTemp = uiNum >> 1;
	while ( uiTemp & 0xffffffff )
	{
		uiTemp >>= 1;
		uiLog++;
	}

	return uiLog;
}
//---------------------------------------------------------------------------
// Algorithm for fast inverse square root from Dave Eberly's WildMagic code.
// See http://www.wild-magic.com/Web/Documents/FastInverseSqrt.pdf for
// implementation discussion.
inline float kFastInvSqrt(float fValue)
{
	float fHalf = 0.5f*fValue;
	int i  = *(int*)&fValue;
	i = 0x5f3759df - (i >> 1);
	fValue = *(float*)&i;
	fValue = fValue*(1.5f - fHalf*fValue*fValue);
	return fValue;
}
//---------------------------------------------------------------------------
inline float kFastSqrt(float fValue)
{
	return kFastInvSqrt(fValue) * fValue;
}
