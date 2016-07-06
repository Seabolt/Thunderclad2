//
// TCIndexBuffer.h
// This file will define the interface for a vertex buffer.
//

#ifndef __TC_INDEX_BUFFER_H__
#define __TC_INDEX_BUFFER_H__

//
// Includes
//

#include "IGraphicsBuffer.h"
#include "TCGraphicsContext.h"

//
// Forward Declaration
//

//
// Defines
//

//
// Class Declaration
//

class TCIndexBuffer :
	public IGraphicsBuffer
{
	public:		// Members
		struct Description : public IGraphicsBuffer::Description
		{
			unsigned int					mNumIndices;
			unsigned int					mIndexSize;
		};

	public:		// Methods
									TCIndexBuffer( TCGraphicsContext* graphicsContext );
									TCIndexBuffer( TCIndexBuffer& inRef );
		virtual	TCIndexBuffer&		operator=( TCIndexBuffer& inRef );
		virtual						~TCIndexBuffer();

		virtual TCResult			Initialize( IGraphicsBuffer::Description& desc );
		virtual TCResult			Release();
		virtual TCResult			Bind( unsigned int offet = 0 );
		virtual TCResult			Unbind();
		virtual TCResult			Fill( void* data, unsigned int dataSize );
		virtual TCResult			Lock( void** vertices );
		virtual TCResult			Unlock( void* vertices );

				unsigned int		GetNumIndices()	{ return mNumIndices; }
				unsigned int		GetIndexSize()	{ return mIndexSize; }

	protected:	// Members
		unsigned int								mNumIndices;
		unsigned int								mIndexSize;
		TCGraphicsContext::TCBufferContext*			mBufferContext;

	protected:	// Methods
		virtual	bool				IsValidDescription( Description& desc );
		virtual void				Clone( TCIndexBuffer& inRef );

		//
		// Friend class glue...
		//

#if TC_GRAPHICS_PLATFORM_OGL4
		friend class TCGraphicsContext_OGL4
#elif TC_GRAPHICS_PLATFORM_DX11
		friend class TCGraphicsContext_DX11;
		friend class TCBufferContext_DX11;
#endif

};

#endif // __TC_VERTEX_BUFFER_H__