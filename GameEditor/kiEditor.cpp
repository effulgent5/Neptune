#include "stdafx.h"
#include "kiEditor.h"
#include "kiMainViewer.h"
#include "kiGameScene.h"

kiEditor::kiEditor(void)
{
}


kiEditor::~kiEditor(void)
{
}

BOOL kiEditor::init(HWND parentWnd)
{
	///< 设置主窗口大小
	RECT oldClientRect;
	RECT oldWindowRect;
	GetClientRect(parentWnd, &oldClientRect);
	GetWindowRect(parentWnd, &oldWindowRect);
	INT widthBias = (oldWindowRect.right - oldWindowRect.left) - (oldClientRect.right - oldClientRect.left);
	INT heightBias = (oldWindowRect.bottom - oldWindowRect.top) - (oldClientRect.bottom - oldClientRect.top);

	MoveWindow(parentWnd, 0, 0, MAIN_VIEWER_WIDTH+widthBias, MAIN_VIEWER_HEIGHT+heightBias, TRUE);

	RECT parentClient;
	GetClientRect(parentWnd, &parentClient);

	mMainViewer = new kiMainViewer();
	LPCTSTR className = AfxRegisterWndClass(NULL, NULL, NULL, NULL);
	BOOL result = mMainViewer->CreateEx(0, className, "mainViewer", WS_CHILD, \
										parentClient.left, parentClient.top, \
										parentClient.right - parentClient.left, \
										parentClient.bottom - parentClient.top, \
										parentWnd, NULL, NULL);
	mMainViewer->ShowWindow(SW_SHOW);

	return TRUE;
}

VOID kiEditor::finl()
{
	delete mMainViewer;
}

VOID kiEditor::tick()
{
	if(mMainViewer != NULL)
		mMainViewer->tick();
}

kiGameScene* kiEditor::newScene(float w, float h, float unitSize)
{
	kiGameScene* scene = new kiGameScene();
	scene->newScene(w, h, unitSize);
	mMainViewer->setScene(scene);
	return scene;
}

kiGameScene* kiEditor::loadScene(const string& filePath)
{
	return NULL;
}

BOOL kiEditor::saveScene(kiGameScene* scene)
{
	return TRUE;
}

BOOL kiEditor::closeScene(kiGameScene* scene)
{
	if(scene != NULL)
	{
		scene->unloadScene();
		delete scene;
	}
	return TRUE;
}
