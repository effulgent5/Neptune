#pragma once

#define MAIN_VIEWER_MSG WM_USER + 7

#define MSG_LB_DOWN		0
#define MSG_LB_UP		1
#define MSG_RB_DOWN		2
#define MSG_RB_UP		3
#define MSG_MMOVE		4
#define MSG_MWHEEL		5
#define MSG_SEL_MAT		6
// kiMainViewer

struct kiMouseBtnInfo
{
	UINT		flag;
	CPoint		point;
};

struct kiMouseWheelInfo
{
	UINT		flag;
	short		delta;
	CPoint		point;
};

class kiGameScene;

class kiMainViewer : public CWnd
{
	DECLARE_DYNAMIC(kiMainViewer)

public:
	kiMainViewer();
	virtual ~kiMainViewer();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();

	VOID	tick();
	VOID	setScene(kiGameScene* scene);

	float	getCameraX() { return mViewCenterX; }
	float	getCameraY() { return mViewCenterY; }
	VOID	setCameraX(float val) { mViewCenterX = val; }
	VOID	setCameraY(float val) { mViewCenterY = val; }

	float	getFrustumY() { return mFrustumY; }
	float	getFrustumX() { return mFrustumX; }
	VOID	setFrustumY(float val) 
	{ 
		if(val > 1.0f)
			mFrustumY = val;
		else
			mFrustumY = 1.f;
	}

	VOID	setFrustumX(float val)
	{ 
		if(val > 1.0f)
			mFrustumX = val;
		else
			mFrustumX = 1.f;
	}

	float	getScreenW() { return mScreenW; }
	float	getScreenH() { return mScreenH; }

	VOID	worldToScreen(float worldX, float worldY, float& screenX, float& screenY);
	VOID	screenToWorld(float screenX, float screenY, float& worldX, float& worldY);
	VOID	worldDistToPixelNum(float worldDist, float& pixelNum);
	VOID	pixelNumToWorldDist(float pixelX, float pixelY, float& worldX, float& worldY);

protected:
	VOID	loadMat();
	UINT	NotifyMsg(UINT p0, UINT p1);
	VOID	drawMem(CDC& memDC);

	CDC		mMemDC;				///< impl dual display buffer
	CDC		mSwapDC;
	CDC		mMatDC;
	CBitmap mMemBuffer;
	CBitmap	mSwapBuffer;

	kiGameScene* mScene;

	float	mViewCenterX, mViewCenterY;
	float	mFrustumY, mFrustumX;
	float	mScreenW, mScreenH;

	CBitmap* mSceneMat;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};


