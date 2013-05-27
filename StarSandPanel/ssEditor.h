#pragma once
#include "commonPanel.h"

///< gen with gencid.exe
#define STARSANDEDITOR_CLASS_ID	Class_ID(0x751720f5, 0x28666764)

extern HINSTANCE g_editorInstance;

class ssEditor : public UtilityObj
{
public:
	ssEditor();
	virtual ~ssEditor();
	virtual void BeginEditParams(Interface* ip, IUtil* iu);
	virtual void EndEditParams(Interface* ip, IUtil* iu);
	virtual void SelectionSetChanged(Interface* ip, IUtil* iu);
	virtual void DeleteThis();
	virtual void SetStartupParam(MSTR param);
private:
	HWND			mCtrlPanel;			///< ¿ØÖÆ´°¿Ú¾ä±ú
	IUtil*			mUtility;			///< Utility interface pointer
	Interface*		mMaxInterface;		///< MAX interface pointer
protected:
	static ssEditor ms_editor;
public:
	static ssEditor* getInstance()
	{
		return &ms_editor;
	}

	HMODULE			mExporterDll;
};

class ssEditorClassDesc : public ClassDesc2
{
public:
	int				IsPublic()						{ return TRUE; }
	void*			Create(BOOL loading = FALSE)	{ return ssEditor::getInstance(); }
	const TCHAR*	ClassName()						{ return _TEXT("StarSand@Editor"); }
	SClass_ID		SuperClassID()					{ return UTILITY_CLASS_ID; }
	Class_ID		ClassID()						{ return STARSANDEDITOR_CLASS_ID; }
	const TCHAR*	Category()						{ return _TEXT("Utility Category"); }
	const TCHAR*	InternalName()					{ return _TEXT("StarSand@Editor"); }
	HINSTANCE		HInstance()						{ return g_editorInstance; }
};

