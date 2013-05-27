#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"
#include "Memory/kiMemObject.h"
//
////-----------------------------------------------------------------------
//class kiTimer : public kiMemObject
//{
//public:
//	kiTimer();
//	~kiTimer();
//	
//public:
//	void				tick();
//	double				frameTime() { return mFrameTime; }
//	double				totalTime() { return mTotalTime; }
//	unsigned int		frameCount() { return mFrameCount; }
//
//public:
//	double				mFrameTime;	
//	double				mTotalTime;
//	unsigned int		mFrameCount;
//
//private:
//	LARGE_INTEGER		mStartTime;
//	LARGE_INTEGER		mFrequency;
//	LONGLONG 			m_llLastElapsedTime;
//
//	LONGLONG			mLastTime;
//	DWORD				mStartTick;
//	HANDLE				mProc;
//	DWORD				mProcMask;
//	DWORD				mSysMask;
//	HANDLE				mThread;
//};
struct	FrameTime{
	float	fTimeDelta;
	double	fTotalTime;
	U64		uiFrameIndex;
};


class RENDERER_EI kiTimer : public kiMemObject
{
public:
	kiTimer();

	void	tick();

	inline	float	frameTime(){return	m_FrameTime.fTimeDelta;};
	inline	double	totalTime(){return	m_FrameTime.fTotalTime;};
	inline	U64		frameCount(){return	m_FrameTime.uiFrameIndex;};

	double			m_Freq;
	LARGE_INTEGER	m_LastTime;
	FrameTime		m_FrameTime;
};


extern kiTimer*			g_timer;