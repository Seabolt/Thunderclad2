//
// TCGraphicsContext_OGL.h
// This file will declare the Win32 interface for a graphics context.
//

#ifndef __TC_GRAPHICS_CONTEXT_OGL_H__
#define __TC_GRAPHICS_CONTEXT_OGL_H__

//
// Includes
//

#include "TCGraphicsContext.h"
#include "TCPlatformPrecompilerSymbols.h"

#if TC_PLATFORM_WIN32 && TC_GRAPHICS_PLATFORM_OGL4

#include <Windows.h>

//
// Defines
//

#define WGL_DRAW_TO_WINDOW_ARB			(0x2001)
#define WGL_ACCELERATION_ARB			(0x2003)
#define WGL_SWAP_METHOD_ARB				(0x2007)
#define WGL_SUPPORT_OPENGL_ARB			(0x2010)
#define WGL_DOUBLE_BUFFER_ARB			(0x2011)
#define WGL_PIXEL_TYPE_ARB				(0x2013)
#define WGL_COLOR_BITS_ARB				(0x2014)
#define WGL_DEPTH_BITS_ARB				(0x2022)
#define WGL_STENCIL_BITS_ARB			(0x2023)
#define WGL_FULL_ACCELERATION_ARB		(0x2027)
#define WGL_SWAP_EXCHANGE_ARB			(0x2028)
#define WGL_TYPE_RGBA_ARB				(0x202B)
#define WGL_CONTEXT_MAJOR_VERSION_ARB	(0x2091)
#define WGL_CONTEXT_MINOR_VERSION_ARB	(0x2092)
#define GL_ARRAY_BUFFER					(0x8892)
#define GL_STATIC_DRAW					(0x88E4)
#define GL_FRAGMENT_SHADER				(0x8B30)
#define GL_VERTEX_SHADER				(0x8B31)
#define GL_COMPILE_STATUS				(0x8B81)
#define GL_LINK_STATUS					(0x8B82)
#define GL_INFO_LOG_LENGTH				(0x8B84)
#define GL_TEXTURE0						(0x84C0)
#define GL_BGRA							(0x80E1)
#define GL_ELEMENT_ARRAY_BUFFER			(0x8893)

//
// Class Declaration
//

class TCGraphicsContext_OGL : public TCGraphicsContext
{
	public:		// Members
	public:		// Methods
							TCGraphicsContext_OGL();
		virtual				~TCGraphicsContext_OGL();

		virtual	TCResult	Initialize( Description& desc );
		virtual TCResult	Destroy();
		virtual TCResult	BeginFrame();
		virtual TCResult	EndFrame();
		virtual TCResult	SwapBuffers();

	protected:	// Members
		static const unsigned int kMaxGLAttributes				= 32;

		HGLRC		mGlResourceContext;
		HDC			mDrawingContext;
		TCWindow*	mWindow;
		int			mGLAttributes[ kMaxGLAttributes ];

	protected:	// Methods
				TCResult	LoadGLExtensions();
				TCResult	InitializeGL3_XContext();
				TCResult	InitializeGL4_XContext();
				TCResult	GetPixelFormat( PIXELFORMATDESCRIPTOR& pixelFormatDescriptor );
		
};
#endif // TC_PLATFORM_WIN32

#endif // __TC_GRAPHICS_CONTEXT_OGL_H__