#include "kiTimer.h"

kiTimer* g_timer = NULL;

//kiTimer::kiTimer()
//{ 	
//	mProc = GetCurrentProcess();
//	mThread = GetCurrentThread();
//
//	GetProcessAffinityMask(mProc, &mProcMask, &mSysMask);
//	SetThreadAffinityMask(mThread, 1);
//
//	QueryPerformanceFrequency( &mFrequency );
//
//	LARGE_INTEGER mStartTime;
//	QueryPerformanceCounter( &mStartTime );
//	mStartTick = GetTickCount();
//	mLastTime = 0;
//	mFrameTime = 0.0;
//	mTotalTime = 0.0;
//
//	SetThreadAffinityMask(mThread, mProcMask);
//
//	mFrameCount = 0;
//}
//
//kiTimer::~kiTimer(void)
//{
//}
//
//void kiTimer::tick()
//{	
//	SetThreadAffinityMask(mThread, 1);
//	LARGE_INTEGER curTime;
//	QueryPerformanceFrequency( &mFrequency );
//	QueryPerformanceCounter(&curTime);
//	SetThreadAffinityMask(mThread, mProcMask);
//
//	LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;
//	
//	double newTicks =  ((double)newTime / mFrequency.QuadPart);
//
//	// detect and compensate for performance counter leaps
//	// (surprisingly common, see Microsoft KB: Q274323)
//	//unsigned long check = GetTickCount() - mStartTick;
//	//signed long msecOff = (signed long)(newTicks - check);
//	//if (msecOff < -100 || msecOff > 100)
//	//{
//	//	// We must keep the timer running forward :)
//	//	LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
//	//	mStartTime.QuadPart += adjust;
//	//	newTime -= adjust;
//
//	//	// Re-calculate milliseconds
//	//	newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
//	//}
//	
//	mFrameTime =  newTicks;
//	mTotalTime += mFrameTime;
//	mStartTime.QuadPart = curTime.QuadPart;
//	++mFrameCount;
//}
//

kiTimer::kiTimer()
{
	LARGE_INTEGER	FREQ;
	QueryPerformanceFrequency(&FREQ);
	m_Freq	=	(double)FREQ.QuadPart	/	(double)1000000;
	QueryPerformanceCounter(&m_LastTime);

	m_FrameTime.fTimeDelta		=	0.0f;
	m_FrameTime.fTotalTime		=	0;
	m_FrameTime.uiFrameIndex	=	0;
}

void kiTimer::tick()
{
	LARGE_INTEGER	currenttime;
	QueryPerformanceCounter(&currenttime);

	double	dTimeDelta	=	0.000001*((currenttime.QuadPart	-	m_LastTime.QuadPart)/m_Freq);
	m_FrameTime.fTimeDelta	=	(float)dTimeDelta;
	m_FrameTime.fTotalTime	+=	dTimeDelta;
	m_FrameTime.uiFrameIndex++;

	m_LastTime.QuadPart	=	currenttime.QuadPart;
}

//COMMON_EXPORT	Timer& GetTimer()
//{
//	static Timer	g_Timer;
//	return	g_Timer;
//}

