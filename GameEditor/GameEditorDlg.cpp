
// GameEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameEditor.h"
#include "GameEditorDlg.h"
#include "afxdialogex.h"
#include "kiMainViewer.h"
#include "kiGameScene.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGameEditorDlg dialog



CGameEditorDlg::CGameEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGameEditorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGameEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGameEditorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	//ON_CONTROL()
	{MAIN_VIEWER_MSG, 0, 0, 0, AfxSig_u_u_u, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< UINT (AFX_MSG_CALL CWnd::*)(UINT, UINT) > ( &CGameEditorDlg::CtrlMessage))},
	ON_COMMAND(ID_MAINPOP_BRUSH_SOLID, &CGameEditorDlg::BrushSelectSolid)
	ON_COMMAND(ID_MAINPOP_BRUSH_ROCK, &CGameEditorDlg::BrushSelectRock)
	ON_COMMAND(ID_MAINPOP_BRUSH_IRON, &CGameEditorDlg::BrushSelectIron)
	ON_COMMAND(ID_MAINPOP_BRUSH_ICE, &CGameEditorDlg::BrushSelectIce)
	ON_COMMAND(ID_MAINPOP_BRUSH_MOVE, &CGameEditorDlg::SelectMove)
	ON_COMMAND(ID_NEW_SCENE, &CGameEditorDlg::OnNewScene)
END_MESSAGE_MAP()


// CGameEditorDlg message handlers

BOOL CGameEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	initEditor();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGameEditorDlg::initEditor()
{
	mCameraSpeed = 5.f;
	mLeftBtnDown = FALSE;
	mRightBtnDown = FALSE;
	HWND mainHwnd = this->GetSafeHwnd();
	mEditor.init(mainHwnd);
	mScene = NULL;

	mSelMat = SCENE_MAT_SOLID;

	mMainPopMenu.CreatePopupMenu();	
	mMainPopMenu.AppendMenu(MF_STRING, ID_MAINPOP_BRUSH_SOLID, "土块(&S)");
	mMainPopMenu.AppendMenu(MF_STRING, ID_MAINPOP_BRUSH_ROCK, "石头(&R)");
	mMainPopMenu.AppendMenu(MF_STRING, ID_MAINPOP_BRUSH_IRON, "钢铁(&W)");
	mMainPopMenu.AppendMenu(MF_STRING, ID_MAINPOP_BRUSH_ICE, "冰块(&C)");
	mMainPopMenu.AppendMenu(MF_STRING, ID_MAINPOP_BRUSH_MOVE, "移动(&V)");	
}

VOID CGameEditorDlg::destroyEditor()
{
	if(mScene != NULL)
	{
		mEditor.closeScene(mScene);
		mScene = NULL;
	}

	mCameraSpeed = 5.f;
	mLeftBtnDown = FALSE;
	mRightBtnDown = FALSE;
	mSelMat = SCENE_MAT_SOLID;
	mEditor.finl();
	mMainPopMenu.DestroyMenu();
}

void CGameEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGameEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGameEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGameEditorDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	destroyEditor();

	CDialogEx::OnClose();
}


void CGameEditorDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnLButtonDown(nFlags, point);
}

UINT CGameEditorDlg::CtrlMessage(UINT p0, UINT p1)
{
	switch (p0)
	{
	case MSG_RB_DOWN:
		{
			kiMouseBtnInfo* info = reinterpret_cast<kiMouseBtnInfo*>(p1);
			mRightBtnDown = TRUE;
			mPreviousPos = info->point;

			kiMainViewer* mainViewer = mEditor.getMainViewer();
			mainViewer->screenToWorld((float)info->point.x, (float)info->point.y, mOldMouseWorldX, mOldMouseWorldY);

			mOldCameraX = mainViewer->getCameraX();
			mOldCameraY = mainViewer->getCameraY();
		}
		break;
	case MSG_RB_UP:
		{
			kiMouseBtnInfo* info = reinterpret_cast<kiMouseBtnInfo*>(p1);
			mRightBtnDown = FALSE;
		}
		break;
	case MSG_LB_DOWN:
		{
			kiMouseBtnInfo* info = reinterpret_cast<kiMouseBtnInfo*>(p1);
			kiMainViewer* mainViewer = mEditor.getMainViewer();

			mLeftBtnDown = TRUE;

			if(mScene != NULL)
			{
				float mouseWorldX, mouseWorldY;
				mainViewer->screenToWorld((float)info->point.x, (float)info->point.y, mouseWorldX, mouseWorldY);
				mScene->setSceneMat(mouseWorldX, mouseWorldY, mSelMat);
				mainViewer->tick();
			}
			//ClientToScreen(&(info->point));	///< TrackPopupMenu需要相对于屏幕的位置
			//mMainPopMenu.TrackPopupMenu(TPM_LEFTALIGN, info->point.x, info->point.y, this);
		}
		break;
	case MSG_LB_UP:
		{
			mLeftBtnDown = FALSE;
		}
		break;
	case MSG_SEL_MAT:
		{
			kiMouseBtnInfo* info = reinterpret_cast<kiMouseBtnInfo*>(p1);
			switch (info->flag)
			{
			case '1': mSelMat = SCENE_MAT_SOLID; break;
			case '2': mSelMat = SCENE_MAT_ROCK; break;
			case '3': mSelMat = SCENE_MAT_IRON; break;
			case '4': mSelMat = SCENE_MAT_ICE; break;
			case '5': mSelMat = SCENE_MAT_LADDER; break;
			case '6': mSelMat = SCENE_MAT_POLE; break;
			case '7': mSelMat = SCENE_MAT_GOLD; break;
			case '8': mSelMat = SCENE_MAT_PLAYER; break;
			case '9': mSelMat = SCENE_MAT_ENEMY; break;
			case '0': mSelMat = SCENE_MAT_EMPTY; break;
			default:
				break;
			}
		}
		break;
	case MSG_MMOVE:
		{
			if(mRightBtnDown)
			{
				kiMouseBtnInfo* info = reinterpret_cast<kiMouseBtnInfo*>(p1);
				CPoint delta = info->point - mPreviousPos;

				float newWorldX, newWorldY;
				kiMainViewer* mainViewer = mEditor.getMainViewer();

				mainViewer->setCameraX(mOldCameraX);
				mainViewer->setCameraY(mOldCameraY);
				mainViewer->screenToWorld((float)info->point.x, (float)info->point.y, newWorldX, newWorldY);

				float deltaX = newWorldX - mOldMouseWorldX;
				float deltaY = newWorldY - mOldMouseWorldY;
				float newX = mOldCameraX - deltaX;
				float newY = mOldCameraY - deltaY;
				mainViewer->setCameraX(newX);
				mainViewer->setCameraY(newY);
				
				mEditor.tick();
			}
			else if(mLeftBtnDown)
			{
				kiMouseBtnInfo* info = reinterpret_cast<kiMouseBtnInfo*>(p1);
				kiMainViewer* mainViewer = mEditor.getMainViewer();

				if(mScene != NULL)
				{
					float mouseWorldX, mouseWorldY;
					mainViewer->screenToWorld((float)info->point.x, (float)info->point.y, mouseWorldX, mouseWorldY);
					mScene->setSceneMat(mouseWorldX, mouseWorldY, mSelMat);
					mainViewer->tick();
				}
			}
		}
		break;
	case MSG_MWHEEL:
		{
			if(!mRightBtnDown)
			{
				kiMouseWheelInfo* info = reinterpret_cast<kiMouseWheelInfo*>(p1);
				if(info->delta != 0)
				{
					float mouseWorldX, mouseWorldY, cameraWorldX, cameraWorldY;
					kiMainViewer* mainViewer = mEditor.getMainViewer();
					mainViewer->screenToWorld((float)info->point.x, (float)info->point.y, mouseWorldX, mouseWorldY);
					cameraWorldX = mainViewer->getCameraX();
					cameraWorldY = mainViewer->getCameraY();
					float oldFx = mainViewer->getFrustumX();
					float oldFy = mainViewer->getFrustumY();

					//float ratioX = ((float)info->point.x + 0.5f) / mainViewer->getScreenW();
					//float ratioY = ((float)info->point.y + 0.5f) / mainViewer->getScreenH();

					//ratioX = (ratioX - 0.5f)*2.f;
					//ratioY = 2.f*(0.5f - ratioY);

					float ratio = mainViewer->getScreenW() / mainViewer->getScreenH();
					float newFy = oldFy - (float)info->delta*0.01f;
					float newFx = newFy*ratio;
					if(newFx >= 1.f || newFy >= 1.f)
					{
						mainViewer->setFrustumX(newFx);
						mainViewer->setFrustumY(newFy);

						mainViewer->setCameraX(mouseWorldX);
						mainViewer->setCameraY(mouseWorldY);

						float x, y;
						mainViewer->screenToWorld((float)info->point.x, (float)info->point.y, x, y);

						mainViewer->setCameraX(mouseWorldX+mouseWorldX-x);
						mainViewer->setCameraY(mouseWorldY+mouseWorldY-y);

						//float distX, distY;
						//mainViewer->pixelNumToWorldDist(x-(float)info->point.x, y-(float)info->point.y, distX, distY);

						//mainViewer->setCameraX(cameraWorldX - distX);
						//mainViewer->setCameraY(cameraWorldY - distY);

						mEditor.tick();
					}
				}
			}
			///<
		}
		break;
	default:
		break;
	}

	return 3;
	// TODO: Add your control notification handler code here
}


void CGameEditorDlg::BrushSelectSolid()
{
	// TODO: Add your command handler code here
}

void CGameEditorDlg::BrushSelectIron()
{
	// TODO: Add your command handler code here
}

void CGameEditorDlg::BrushSelectIce()
{
	// TODO: Add your command handler code here
}

void CGameEditorDlg::BrushSelectRock()
{
	// TODO: Add your command handler code here
}

void CGameEditorDlg::SelectMove()
{
	// TODO: Add your command handler code here
}

void CGameEditorDlg::OnNewScene()
{
	// TODO: Add your command handler code here
	mScene = mEditor.newScene(50.f, 50.f, 1.0f);
}
