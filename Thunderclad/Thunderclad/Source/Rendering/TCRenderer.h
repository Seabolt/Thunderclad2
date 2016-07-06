//
// TCRenderer.h
// This file will define the logic for rendering scenes.
//

#ifndef __TC_RENDERER_H__
#define __TC_RENDERER_H__

//
// Includes
//

#include "TCGraphicsContext.h"
#include "TCPlatformPrecompilerSymbols.h"

//
// Defines
//

//
// Forward Declarations
//

class TCScene;

//
// Class Declaration
//

class TCRenderer
{
	public:		// Members
	public:		// Methods
								TCRenderer( TCGraphicsContext* graphicsContext );
		virtual					~TCRenderer();

		virtual TCResult		Initialize() = 0;
		virtual TCResult		DrawScene( TCScene* scene );
		virtual void			Destroy();
	protected:	// Members
		TCGraphicsContext* mGraphicsContext;

	protected:	// Methods
};

#endif // __TC_RENDERER_H__