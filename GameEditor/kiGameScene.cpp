#include "stdafx.h"
#include "kiGameScene.h"


kiGameScene::kiGameScene(void)
: mWidth(DEFUALT_SCENE_WIDTH)
, mHeight(DEFUALT_SCENE_HEIGHT)
, mUnitSize(DEFUALT_SCENE_UNITSIZE)
, mSceneBit(NULL)
{
}


kiGameScene::~kiGameScene(void)
{
}


VOID kiGameScene::setSceneMat(float worldX, float worldY, CHAR mat)
{
	if(worldX >= 0.f && worldX <= mWidth && worldY >= 0.f && worldY <= mHeight \
		&& mat >= SCENE_MAT_EMPTY && mat < SCENE_MAT_TOTAL)
	{
		mSceneBit[(int)mWidth*(int)worldY+(int)worldX] = mat;
	}
}

BOOL kiGameScene::newScene(float width, float height, float unit)
{
	mWidth = width;
	mHeight = height;
	mUnitSize = unit;

	//int sceneBitWidth = int(mWidth / unit);
	//int sceneBitHeight = int(mHeight / unit);

	int bitNum = int(mWidth*mHeight);
	mSceneBit = (UCHAR*)malloc(bitNum);
	memset(mSceneBit, SCENE_MAT_EMPTY, bitNum);

	for (int h = 0; h < (int)mHeight; ++h)
	{
		for (int w = 0; w < (int)mWidth; ++w)
		{			
			mSceneBit[(int)mWidth*h+w] = SCENE_MAT_EMPTY/*((h*(int)mWidth+w)%SCENE_MAT_POLE) + 1*/;
		}
	}

	return TRUE;
}

BOOL kiGameScene::loadScene(const string& sceneFile)
{
	return TRUE;
}

VOID kiGameScene::unloadScene()
{
	if(mSceneBit != NULL)
	{
		free(mSceneBit);
		mSceneBit = NULL;
	}

	mWidth = DEFUALT_SCENE_WIDTH;
	mHeight = DEFUALT_SCENE_HEIGHT;
	mUnitSize = DEFUALT_SCENE_UNITSIZE;

	mBackground = "";
}