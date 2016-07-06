//
// PlatformPrecompilerSymbols.h
// This file is where all platform related pre compiler symbols should be added.
//

#ifndef __TC_PLATFORM_PRECOMPILER_SYMBOLS_H__
#define __TC_PLATFORM_PRECOMPILER_SYMBOLS_H__

#define TC_PLATFORM_WIN32		1
#define TC_PLATFORM_MAC			0
#define TC_PLATFORM_LINUX		0
#define TC_PLATFORM_ANDROID		0
#define TC_PLATFORM_IPHONE		0

#define TC_GRAPHICS_PLATFORM_DX11	1
#define TC_GRAPHICS_PLATFORM_OGL4	0

#define TC_BUILD_CONFIGURATION_DEBUG	_DEBUG

#if TC_PLATFORM_WIN32
	#include <assert.h>
	#define TC_ASSERT( x ) assert( x )
	
	#ifndef NULL
		#define NULL 0
	#endif // NULL
#endif

#endif // __TC_PLATFORM_PRECOMPILER_SYMBOLS_H__