//
// TCGraphicsContext_OGL.cpp
// This file will define the implementation of a graphics context for Open GL
//

//
// Includes
//

#include "TCGraphicsContext_OGL.h"
#if TC_PLATFORM_WIN32 && TC_GRAPHICS_PLATFORM_OGL4

//
// Defines
//

TCGraphicsContext_OGL::TCGraphicsContext_OGL()
{

}

TCGraphicsContext_OGL::~TCGraphicsContext_OGL()
{

}

TCResult TCGraphicsContext_OGL::Initialize( Description& desc )
{
	return Failure_NotImplemented;
}

TCResult TCGraphicsContext_OGL::Destroy()
{
	return Failure_NotImplemented;
}

TCResult TCGraphicsContext_OGL::BeginFrame()
{
	return Failure_NotImplemented;
}

TCResult TCGraphicsContext_OGL::EndFrame()
{
	return Failure_NotImplemented;
}

TCResult TCGraphicsContext_OGL::SwapBuffers()
{
	return Failure_NotImplemented;
}
#endif // TC_PLATFORM_WIN32