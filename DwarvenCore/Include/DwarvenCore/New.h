#pragma once

#define _CRTDBG_MAP_ALLOC

#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DFNew new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define DFDelete delete
#else
#define DFNew new
#define DFDelete delete
#endif