#include "ssEditor.h"
#include "iExporter.h"

ssEditor ssEditor::ms_editor;

//extern "C" 
//{ 
//	int Exporter( void ); 
//	typedef int(*CallBackExporter)( const TCHAR *name, ExpInterface *ei, Interface *i,
//		BOOL suppressPrompts, DWORD options ); 
//} 
//
//int IGameExporter::DoExport( const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options ) 
//{ 
//	HMODULE hModule; 
//	hModule = ::LoadLibraryEx( "plugins/BMLExporter.dll", NULL, 0 ); 
//	if( hModule == NULL ) 
//	{ 
//		hModule = ::LoadLibraryEx( "BMLExporter.dll", NULL, 0 ); 
//		if( hModule == NULL ) 
//		{ 
//			MessageBox( NULL, "加载模块BMLExporter.dll失败.", "", 0 ); 
//			return 0; 
//		} 
//	}
//	CallBackExporter pfnExporter = NULL; 
//	pfnExporter = (CallBackExporter)GetProcAddress( hModule, "Exporter" ); 
//	if( pfnExporter == NULL ) 
//		return FALSE; 
//	int nRet = pfnExporter( name, ei, i, suppressPrompts, options ); 
//	::FreeLibrary( hModule ); 
//	return nRet; 
//}

typedef void* (*func_exporter)(void);

BOOL DialogUILogic(HWND wnd, UINT ctrlID, ssEditor* edt)
{
	BOOL chked = FALSE;
	switch(ctrlID)
	{
	case IDCANCEL:
		EndDialog(wnd, TRUE);
		return TRUE;
		break;
	case BTN_LOADDLL:
		{
			//TCHAR buf[MAX_PATH];
			//GetWindowText(GetDlgItem(wnd, EDT_FILEPATH), buf, MAX_PATH);
			//kiMeshUtility::exportGeo(buf, exp->mFileName, exp);
			if(edt->mExporterDll != NULL)
			{
				::FreeLibrary(edt->mExporterDll);
				SetWindowText(GetDlgItem(wnd, BTN_LOADDLL), TEXT("Load DLL"));
				edt->mExporterDll = NULL;
			}
			else
			{
				TCHAR path[MAX_PATH];
				::GetModuleFileName(g_editorInstance, path, MAX_PATH*sizeof(TCHAR));
				//::MessageBox(NULL, path, TEXT("Error"), MB_ICONERROR);
				TCHAR* lastSlash = NULL;
				lastSlash = strrchr(path, TEXT('\\'));
				if(lastSlash != NULL)
				{
					*lastSlash = 0;
					///< load d3dcompiler_43
					strcat_s(path, MAX_PATH, TEXT("\\StarSandExporter.dll"));
					edt->mExporterDll = LoadLibraryEx(path, NULL, 0); 
					if(edt->mExporterDll != NULL) 
					{ 
						//::MessageBox(NULL, TEXT("LoadLibraryEx Error"), TEXT("Error"), MB_ICONERROR);
						func_exporter createFunc = (func_exporter)GetProcAddress(edt->mExporterDll, TEXT("createExporter"));
						if(createFunc != NULL)
						{
							iExporter* exporter = (iExporter*)createFunc();
							if(exporter != NULL)
							{
								exporter->exportObj(NULL);
								SetWindowText(GetDlgItem(wnd, BTN_LOADDLL), TEXT("UnLoad DLL"));
							}
						}
						else
						{
							::MessageBox(NULL, TEXT("GetProcAddress Error"), TEXT("Error"), MB_ICONERROR);
						}
					}
					else
					{
						::MessageBox(NULL, TEXT("LoadLibraryEx Error"), TEXT("Error"), MB_ICONERROR);
					}
				}
				///<
			}			
			return TRUE;
		}
		break;
	case CHK_BLENDWEIGHT:
		chked = IsDlgButtonChecked(wnd, CHK_BLENDWEIGHT);
		CheckDlgButton(wnd, CHK_BLENDWEIGHT, !chked);
		//exp->mExpBlendWeight = !chked;
		break;
	case CHK_NORMAL:
		chked = IsDlgButtonChecked(wnd, CHK_NORMAL);
		CheckDlgButton(wnd, CHK_NORMAL, !chked);
		//exp->mExpNormal = !chked;
		break;
	case CHK_BLENDINDICES:
		chked = IsDlgButtonChecked(wnd, CHK_BLENDINDICES);
		CheckDlgButton(wnd, CHK_BLENDINDICES, !chked);
		//exp->mExpBlendIndices = !chked;
		break;
	case CHK_TEXCOORD0:
		chked = IsDlgButtonChecked(wnd, CHK_TEXCOORD0);
		CheckDlgButton(wnd, CHK_TEXCOORD0, !chked);
		//exp->mExpTexcoord0 = !chked;
		break;
	case CHK_TEXCOORD1:
		chked = IsDlgButtonChecked(wnd, CHK_TEXCOORD1);
		CheckDlgButton(wnd, CHK_TEXCOORD1, !chked);
		//exp->mExpTexcoord1 = !chked;
		break;
	default:
		return FALSE;
		break;
	}
	return TRUE;
}

LRESULT CALLBACK DialogProc(HWND hWnd,UINT uMsg, WPARAM wParam,LPARAM lParam)       
{ 
	//std::stringstream ss;
	//std::string tmp="";
	//char tmpc[100];
	ssEditor* edt = (ssEditor*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	switch(uMsg)    
	{    
	case WM_INITDIALOG:
		edt = (ssEditor*)lParam;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, lParam);

		//SetWindowPos(hWnd,NULL,500,200,0,0,SWP_NOSIZE);
		///< 设置几何导出部分界面选项
		//CheckDlgButton(hWnd, CHK_BLENDWEIGHT, exp->mExpBlendWeight);
		//CheckDlgButton(hWnd, CHK_NORMAL, exp->mExpNormal);
		//CheckDlgButton(hWnd, CHK_BLENDINDICES, exp->mExpBlendIndices);
		//CheckDlgButton(hWnd, CHK_TEXCOORD0, exp->mExpTexcoord0);
		//CheckDlgButton(hWnd, CHK_TEXCOORD1, exp->mExpTexcoord1);

		///< 设置路径
		//SetWindowText(GetDlgItem(hWnd, EDT_FILEPATH), exp->mPath);

		return TRUE;
	case WM_COMMAND:
		return DialogUILogic(hWnd, LOWORD(wParam), edt);		
		break;
	}
	return FALSE;
}

ssEditor::ssEditor()
{
	mExporterDll = NULL;
}

ssEditor::~ssEditor()
{

}

void ssEditor::BeginEditParams(Interface* ip, IUtil* iu)
{
	mUtility		= iu;
	mMaxInterface	= ip;
	mCtrlPanel		= ip->AddRollupPage(	g_editorInstance, MAKEINTRESOURCE(IDD_EXPORTER_MAIN), \
							(DLGPROC)DialogProc, TEXT("StarSand@ Ctrl Panel"), (LPARAM)this);
}

void ssEditor::EndEditParams(Interface* ip, IUtil* iu)
{
	mUtility = NULL;
	mMaxInterface = NULL;
	ip->DeleteRollupPage(mCtrlPanel);
	mCtrlPanel = NULL;
}

void ssEditor::SelectionSetChanged(Interface* ip, IUtil* iu)
{
	UNUSED_PARAM(ip);
	UNUSED_PARAM(iu);
}
void ssEditor::DeleteThis()
{

}

void ssEditor::SetStartupParam(MSTR param)
{

}


///< *****************************************************
static ssEditorClassDesc	editorDesc;

ClassDesc2* GetEditorDesc()
{
	return &editorDesc;
}