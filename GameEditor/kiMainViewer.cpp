// kiMainViewer.cpp : implementation file
//

#include "stdafx.h"
#include "GameEditor.h"
#include "kiMainViewer.h"
#include "kiGameScene.h"

// kiMainViewer

IMPLEMENT_DYNAMIC(kiMainViewer, CWnd)

kiMainViewer::kiMainViewer()
{
	mViewCenterX = 0.f;
	mViewCenterY = 0.f;
	mFrustumY = 25.f;
	mFrustumX = 25.f;
	mSceneMat = NULL;
}

kiMainViewer::~kiMainViewer()
{
}


BEGIN_MESSAGE_MAP(kiMainViewer, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_KEYUP()
END_MESSAGE_MAP()



// kiMainViewer message handlers

VOID kiMainViewer::setScene(kiGameScene* scene)
{
	mScene = scene;
	if(mScene != NULL && mScene->getSceneBit() != NULL)
	{
		UCHAR* bits = mScene->getSceneBit();
		int w = (int)mScene->getWidth();
		int h = (int)mScene->getHeight();

		///< 移动相机
		mViewCenterX = (float)w / 2.f;
		mViewCenterY = (float)h / 2.f;

		CRect rc;
		GetClientRect(&rc);
		if(rc.Width() >= rc.Height())
		{
			mFrustumY = (float)h / 2.f;
			float ratio = mScreenW / mScreenH;
			mFrustumX = ratio * mFrustumY;
		}
		else
		{
			mFrustumY = (float)w / 2.f;
			float ratio = mScreenH / mScreenW;
			mFrustumX = ratio * mFrustumX;
		}

		CClientDC clientDC(this);
		mSwapDC.DeleteDC();
		mSwapBuffer.DeleteObject();

		mSwapDC.CreateCompatibleDC(&clientDC);
		mSwapBuffer.CreateCompatibleBitmap(&clientDC, SCENE_MAT_PIXEL*(int)mScene->getWidth(), \
			SCENE_MAT_PIXEL*(int)mScene->getHeight());
		mSwapDC.SelectObject(&mSwapBuffer);
	}
}

void kiMainViewer::OnPaint()
{
	CClientDC dc(this);

	drawMem(mMemDC);

	CRect rect;
	GetClientRect(&rect);
	CBrush brBg(RGB(127, 127, 127));
	mMemDC.FillRect(&rect, &brBg);


	if(mScene != NULL)
	{
		int swapWidth = (int)mScene->getWidth()*SCENE_MAT_PIXEL;
		int swapHeight = (int)mScene->getHeight()*SCENE_MAT_PIXEL;

		float worldMinX = 0.f, worldMinY = 0.f, worldMaxX = 0.f, worldMaxY = 0.f;
		float screenMinX = 0.f, screenMinY = 0.f, screenMaxX = 0.f, screenMaxY = 0.f;
		worldToScreen(0.f, 0.f, screenMinX, screenMinY);
		worldToScreen(mScene->getWidth(), mScene->getHeight(), screenMaxX, screenMaxY);
		mMemDC.StretchBlt((int)screenMinX, (int)screenMaxY, int(screenMaxX - screenMinX), int(screenMinY - screenMaxY), \
			&mSwapDC, 0, 0, swapWidth, swapHeight, SRCCOPY);
	}

	dc.BitBlt(0, 0, (int)mScreenW, (int)mScreenH, &mMemDC, 0, 0, SRCCOPY);

	brBg.DeleteObject();
}

VOID kiMainViewer::drawMem(CDC& memDC)
{
	CRect rect;
	GetClientRect(&rect);
	CBrush brBlack(RGB(0, 0, 0));

	float screenW = (float)rect.Width();
	float screenH = (float)rect.Height();

	if(mScene != NULL && mScene->getSceneBit() != NULL)
	{
		UCHAR* bits = mScene->getSceneBit();
		int width = (int)mScene->getWidth();
		int height = (int)mScene->getHeight();

		UCHAR bit = SCENE_MAT_EMPTY;
		float worldMinX = 0.f, worldMinY = 0.f, worldMaxX = 0.f, worldMaxY = 0.f;
		float screenMinX = 0.f, screenMinY = 0.f, screenMaxX = 0.f, screenMaxY = 0.f;
		for(int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				worldMinX = (float)w;
				worldMinY = (float)h;
				worldMaxX = (float)(w+1);
				worldMaxY = (float)(h+1);

				worldToScreen(worldMinX, worldMinY, screenMinX, screenMinY);
				worldToScreen(worldMaxX, worldMaxY, screenMaxX, screenMaxY);

				///< 相机裁剪
				if(screenMaxX < 0.f || screenMinX > screenW || screenMinY < 0.f || screenMaxY > screenH)
					continue;

				bit = bits[h*width+w];

				int originX = w * SCENE_MAT_PIXEL;
				int originY = (height - h - 1) * SCENE_MAT_PIXEL;

				int yyy = bit / 8;
				int xxx = bit % 8;
				mSwapDC.BitBlt(originX, originY, SCENE_MAT_PIXEL, SCENE_MAT_PIXEL, &mMatDC, xxx*SCENE_MAT_PIXEL, yyy*SCENE_MAT_PIXEL, SRCCOPY);
			}
		}
		///<
	}

	brBlack.DeleteObject();
}


int kiMainViewer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CClientDC clientDC(this);
	CRect rect;
	GetClientRect(&rect);

	mScreenW = (float)rect.Width();
	mScreenH = (float)rect.Height();

	///< 创建内存DC
	mMemDC.DeleteDC();
	mMemDC.CreateCompatibleDC(&clientDC);
	mMemBuffer.CreateCompatibleBitmap(&clientDC, rect.Width(), rect.Height());
	mMemDC.SelectObject(&mMemBuffer);

	mMatDC.DeleteDC();
	mMatDC.CreateCompatibleDC(&clientDC);
	///< 加载材质纹理
	loadMat();
	mMatDC.SelectObject(mSceneMat);

	return 0;
}

VOID kiMainViewer::loadMat()
{
	if(mSceneMat != NULL)
	{
		delete mSceneMat;
		mSceneMat = NULL;
	}

	mSceneMat = new CBitmap();

	HINSTANCE handle = ::AfxGetInstanceHandle();
	CHAR buffer[0x100];	
	kiFileUtility::getRootDir(buffer, 0x100);
	strcat_s(buffer, 0x100, "texture\\scene\\scene_mat0.bmp");
	HBITMAP matHandle = (HBITMAP)::LoadImage(handle, buffer, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if(matHandle == NULL)
	{
		//AfxMessageBox("bmp load failed!");
		__asm int 3;
	}
	else
	{
		BOOL result = mSceneMat->Attach(matHandle);
		int jj = 0;
	}

}

void kiMainViewer::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	mMemDC.DeleteDC();
	mMemBuffer.DeleteObject();
	mSceneMat->DeleteObject();
	mMatDC.DeleteDC();

	if(mSceneMat != NULL)
	{
		delete mSceneMat;
		mSceneMat = NULL;
	}

	CWnd::OnClose();
}

VOID kiMainViewer::worldToScreen(float worldX, float worldY, float& screenX, float& screenY)
{
	float cameraX = worldX - mViewCenterX;
	float cameraY = worldY - mViewCenterY;

	///< 投影变换
	float x = cameraX / mFrustumX;
	float y = cameraY / mFrustumY;

	float frustumX = 0.5f*(x + 1.f);
	float frustumY = 0.5f*(1.f - y);		///< 0 ~ 1

	screenX = frustumX * mScreenW;
	screenY = frustumY * mScreenH;

	//screenX = screenX >= mScreenW ? (mScreenW - 1) : screenX;
	//screenY = screenY >= mScreenH ? (mScreenH - 1) : screenY;

	//screenX = float(int(frustumX * mScreenW - 0.5f));
	//screenY = float(int(frustumY * mScreenH - 0.5f));
}

VOID kiMainViewer::screenToWorld(float screenX, float screenY, float& worldX, float& worldY)
{
	float frustumX = screenX / mScreenW;
	float frustumY = screenY / mScreenH;
	float x = frustumX * 2.f - 1.f;
	float y = 1.f - frustumY * 2.f;

	worldX = x*mFrustumX + mViewCenterX;
	worldY = y*mFrustumY + mViewCenterY;

	//char str[512];
	//sprintf_s(str,512,"%f %f %f %f move\n",mFrustumX,mFrustumY,mViewCenterX,mViewCenterY);
	//OutputDebugString(str);
}

VOID kiMainViewer::worldDistToPixelNum(float worldDist, float& pixelNum)
{

}

VOID kiMainViewer::pixelNumToWorldDist(float pixelX, float pixelY, float& worldX, float& worldY)
{
	worldX = mFrustumX / mScreenW * pixelX;
	worldY = mFrustumY / mScreenH * pixelY;
}

UINT kiMainViewer::NotifyMsg(UINT p0, UINT p1)
{
	CWnd* parentWnd = GetParent();
	UINT result = ::SendMessage(parentWnd->GetSafeHwnd(), MAIN_VIEWER_MSG, p0, p1);
	return result;
}

void kiMainViewer::OnLButtonDown(UINT nFlags, CPoint point)
{
	kiMouseBtnInfo msg;
	msg.flag = nFlags;
	msg.point = point;
	UINT result = NotifyMsg(MSG_LB_DOWN, reinterpret_cast<UINT>(&msg));
}


void kiMainViewer::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	kiMouseBtnInfo msg;
	msg.flag = nFlags;
	msg.point = point;
	UINT result = NotifyMsg(MSG_RB_DOWN, reinterpret_cast<UINT>(&msg));
}


void kiMainViewer::OnMouseMove(UINT nFlags, CPoint point)
{
	kiMouseBtnInfo msg;
	msg.flag = nFlags;
	msg.point = point;
	UINT result = NotifyMsg(MSG_MMOVE, reinterpret_cast<UINT>(&msg));
}

BOOL kiMainViewer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	ScreenToClient(&pt);
	kiMouseWheelInfo msg;
	msg.flag = nFlags;
	msg.point = pt;
	msg.delta = zDelta;
	UINT result = NotifyMsg(MSG_MWHEEL, reinterpret_cast<UINT>(&msg));

	return TRUE;
}

VOID kiMainViewer::tick()
{
	Invalidate();
}

void kiMainViewer::OnLButtonUp(UINT nFlags, CPoint point)
{
	kiMouseBtnInfo msg;
	msg.flag = nFlags;
	msg.point = point;
	UINT result = NotifyMsg(MSG_LB_UP, reinterpret_cast<UINT>(&msg));
}


void kiMainViewer::OnRButtonUp(UINT nFlags, CPoint point)
{
	kiMouseBtnInfo msg;
	msg.flag = nFlags;
	msg.point = point;
	UINT result = NotifyMsg(MSG_RB_UP, reinterpret_cast<UINT>(&msg));
}

void kiMainViewer::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	kiMouseBtnInfo msg;
	msg.flag = nChar;
	UINT result = NotifyMsg(MSG_SEL_MAT, reinterpret_cast<UINT>(&msg));
}
