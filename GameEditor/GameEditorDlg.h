
// GameEditorDlg.h : header file
//

#pragma once

#include "kiEditor.h"

#define ID_MAINPOP_BRUSH_SOLID	7000
#define ID_MAINPOP_BRUSH_ROCK	7001
#define ID_MAINPOP_BRUSH_IRON	7002
#define ID_MAINPOP_BRUSH_ICE	7003
#define ID_MAINPOP_BRUSH_MOVE	7004

// CGameEditorDlg dialog
class CGameEditorDlg : public CDialogEx
{
// Construction
public:
	CGameEditorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GAMEEDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	VOID initEditor();
	VOID destroyEditor();

	kiEditor	mEditor;
	CMenu		mMainPopMenu;
	kiGameScene* mScene;

	float		mCameraSpeed;
	BOOL		mLeftBtnDown;
	BOOL		mRightBtnDown;
	CPoint		mPreviousPos;
	float		mOldMouseWorldX;
	float		mOldMouseWorldY;
	float		mOldCameraX;
	float		mOldCameraY;
	CHAR		mSelMat;
public:
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg UINT CtrlMessage(UINT p0, UINT p1);
	afx_msg void BrushSelectSolid();
	afx_msg void BrushSelectIron();
	afx_msg void BrushSelectIce();
	afx_msg void BrushSelectRock();
	afx_msg void SelectMove();
	afx_msg void OnNewScene();
};
