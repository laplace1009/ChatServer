#pragma once

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#pragma comment(lib, "Debug\\ChatLib.lib")
#else
#pragma comment(lib, "Release\\ChatLib.lib")
#endif

#include "CorePch.h"