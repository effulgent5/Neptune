#pragma once

class kiMainViewer;
class kiGameScene;

#define MAIN_VIEWER_WIDTH 1400
#define MAIN_VIEWER_HEIGHT 1000

class kiEditor
{
public:
	kiEditor(void);
	~kiEditor(void);

	BOOL init(HWND parentWnd);
	VOID finl();

	kiGameScene* newScene(float w, float h, float unitSize);
	kiGameScene* loadScene(const string& filePath);
	BOOL saveScene(kiGameScene* scene);
	BOOL closeScene(kiGameScene* scene);

	kiMainViewer* getMainViewer() { return mMainViewer; }

	VOID tick();

protected:
	kiMainViewer*		mMainViewer;
};

