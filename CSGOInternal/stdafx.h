// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#pragma warning(disable: 4530)

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN   
#ifdef _AMD64_
#define _PTR_MAX_VALUE ((PVOID)0x000F000000000000)
#else
#define _PTR_MAX_VALUE ((PVOID)0xFFE00000)
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_WARNINGS // let me use freopen u fawk

#define _CRT_SECURE_NO_DEPRECATE
#define CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#define PAD(SIZE) BYTE MACRO_CONCAT(_pad, __COUNTER__)[SIZE];
#define POINTERCHK( pointer ) ( pointer  && pointer !=0  && HIWORD( pointer ) )
#define VALID( x ) ( x != NULL && HIWORD( x ) )// Exclude rarely-used stuff from Windows headers
// Windows Header Files:

#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>
//#include "detours.h"
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <iostream>
#include <d3dx9.h>
#include <Dwmapi.h> 
#include <TlHelp32.h>
#include <tchar.h>
#include <strsafe.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <hash_map>
#include <time.h>



// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "Dwmapi.lib")
//#pragma comment (lib, "detours.lib")

#include "Math.h"
#include "CSGOStructs.h"
#include "Definitions.h"
#include "CEngine.h"
#include "CVMTHooking.h"
#include "CDrawing.h"
#include "Menu.h"
// TODO: reference additional headers your program requires here
