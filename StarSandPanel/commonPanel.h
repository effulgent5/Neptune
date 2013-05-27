#pragma once
// useful for #pragma message
#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define TODO(x) __FILE__ "(" STRING(__LINE__) "): TODO: "x

#include "max.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "IGame/IGame.h"
#include "IGame/IGameModifier.h"
#include "decomp.h"
#include "utilapi.h"

#include <windows.h>

///< std c library
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

///< std c++ library
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <deque>
#include <stdio.h>
#include <set>
#include <assert.h>
#include <algorithm>
using std::wstring;
using std::string;
using std::vector;
using std::map;
using std::queue;
using std::deque;
using std::set;

#ifdef _UNICODE
typedef wstring tstring;
#define tfopen	_wfsopen
#else
typedef string tstring;
#define tfopen	_fsopen
#endif

#pragma comment(lib, "core.lib")
#pragma comment(lib, "geom.lib")
#pragma comment(lib, "gfx.lib")
#pragma comment(lib, "mesh.lib")
#pragma comment(lib, "maxutil.lib")
#pragma comment(lib, "maxscrpt.lib")
#pragma comment(lib, "paramblk2.lib")
#pragma comment(lib, "msxml2.lib")
#pragma comment(lib, "igame.lib")

#include "resource.h"

extern HINSTANCE hInstance;

#ifndef SSPANEL_IMPORT
#ifdef SSPANEL_EXPORT
#define SSPANEL_EI __declspec(dllexport)
#else
#define SSPANEL_EI __declspec(dllimport)
#endif
#endif