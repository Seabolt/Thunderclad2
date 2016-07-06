//
// TCRenderer.cpp
// This file will define the renderer interface.
//

//
// Includes
//

#include "TCRenderer.h"

//
// Defines
//

//
// Constructor
//		- Will initialize this object to a safe state. Must provide a valid graphics context.
// Inputs:
//		- TCGraphicsContext*: The context to use when rendering.
// Outputs:
//		- None.
//

TCRenderer::TCRenderer( TCGraphicsContext* graphicsContext )
{
	TC_ASSERT( graphicsContext );
	mGraphicsContext = graphicsContext;
}

//
// Destructor
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCRenderer::~TCRenderer()
{
	Destroy();
}

//
// Initialize
//		- Will initialize this object.
// Inputs:
//		- None.
// Outputs:
//		- The result of the operation.
//

TCResult TCRenderer::Initialize()
{
	return Success;
}

//
// DrawScene
//		- Will render a scene.
// Inputs:
//		- TCScene* scene: The scene to render. Must not be null.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCRenderer::DrawScene( TCScene* scene )
{
	return Failure_NotImplemented;
}

//
// Destroy
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCRenderer::Destroy()
{
	
}