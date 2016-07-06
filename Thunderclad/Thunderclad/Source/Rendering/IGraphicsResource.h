//
// IGraphicsResource
// This file will define the interface for interacting with a graphics resource.
//

#ifndef __I_GRAPHICS_RESOURCE_H__
#define __I_GRAPHICS_RESOURCE_H__

//
// Includes
//

#include "TCResultCode.h"

//
// Forward Declaration
//

class TCGraphicsContext;
class TCBufferContext;

//
// Defines
//

#define TC_GRAPHICS_RESOURCE_SAFE_RELEASE( x ) { if( x ){ x->Release(); x = NULL; } }

//
// Class Declaration
//

class IGraphicsResource
{
	public:		// Members
	public:		// Methods
										IGraphicsResource( TCGraphicsContext* graphicsContext );
										IGraphicsResource( const IGraphicsResource& inRef );
		virtual	IGraphicsResource&		operator=( const IGraphicsResource& inRef );
		virtual							~IGraphicsResource();

		virtual TCResult				Release() = 0;

	protected:	// Members
		TCGraphicsContext*				mGraphicsContext;
		void*							mResourceBlob;

	protected:	// Methods
		virtual void					Clone( const IGraphicsResource& graphicsResource );

		virtual void					SetGraphicsResources( void* resources ) { mResourceBlob = resources; }
		virtual void*					GetGraphicsResources()					{ return mResourceBlob; }

		friend class TCGraphicsContext;
		friend class TCGraphicsContext_DX11;
};

#endif // __I_GRAPHICS_RESOURCE_H__