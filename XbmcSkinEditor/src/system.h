#pragma once
//stdstring include windows.h from wtype.h so we need to include the afx headers before to be able to compile
#include "stdafx.h"
#include "xbmc/lib/StdString.h"
#define SAFE_RELEASE(p)      do { if(p) { (p)->Release(); (p)=NULL; } } while (0)