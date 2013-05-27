#pragma once

#define DEFUALT_SCENE_WIDTH		50.f
#define DEFUALT_SCENE_HEIGHT	50.f
#define DEFUALT_SCENE_UNITSIZE	0.1f


#define SCENE_MAT_EMPTY		0
#define SCENE_MAT_SOLID		1
#define SCENE_MAT_ROCK		2
#define SCENE_MAT_IRON		3
#define SCENE_MAT_ICE		4
#define SCENE_MAT_LADDER	5
#define SCENE_MAT_POLE		6
#define SCENE_MAT_GOLD		7
#define SCENE_MAT_PLAYER	8
#define SCENE_MAT_ENEMY		9
#define SCENE_MAT_TOTAL		10

#define SCENE_MAT_PIXEL		32

class kiGameScene
{
public:
	kiGameScene(void);
	~kiGameScene(void);

	BOOL	newScene(float width, float height, float unit);
	BOOL	loadScene(const string& sceneFile);
	VOID	unloadScene();

	float	getWidth()		{ return mWidth; }
	float	getHeight()		{ return mHeight; }
	float	getUnitSize()	{ return mUnitSize; }
	const string& getBackground() { return mBackground; }
	UCHAR*	getSceneBit()	{ return mSceneBit; }

	VOID	setSceneMat(float worldX, float worldY, CHAR mat);
protected:

	float	mWidth;
	float	mHeight;
	float	mUnitSize;

	UCHAR*	mSceneBit;
	string	mBackground;

	///< 地图原点坐标在左下角
};

