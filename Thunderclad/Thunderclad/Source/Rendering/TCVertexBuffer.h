//
// TCVertexBuffer.h
// This file will define the interface for a vertex buffer.
//

#ifndef __TC_VERTEX_BUFFER_H__
#define __TC_VERTEX_BUFFER_H__

//
// Includes
//

#include "IGraphicsBuffer.h"
#include "TCVertexFormat.h"
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

class TCVertexBuffer :
	public IGraphicsBuffer
{
	public:		// Members
		struct Description : public IGraphicsBuffer::Description
		{
			TCVertexFormat*					mVertexFormat;	// Refactor this to graphics buffer.
			unsigned int					mNumVertices;
			bool							mDiscardVerts;
		};

	public:		// Methods
									TCVertexBuffer( TCGraphicsContext* graphicsContext );
									TCVertexBuffer( const TCVertexBuffer& inRef );
		virtual	TCVertexBuffer&		operator=( const TCVertexBuffer& inRef );
		virtual						~TCVertexBuffer();

		virtual TCResult			Initialize( IGraphicsBuffer::Description& desc );
		virtual TCResult			Release();
		virtual TCResult			Bind( unsigned int index );
		virtual TCResult			Unbind();
		virtual TCResult			Fill( void* data, unsigned int dataSize );
		virtual TCResult			Lock( void** vertices );
		virtual TCResult			Unlock( void* vertices );

		virtual TCVertexFormat*		GetVertexFormat()	{ return mVertexFormat; }
				unsigned int		GetNumVertices()	{ return mNumVertices; }

	protected:	// Members
		TCVertexFormat*								mVertexFormat;
		void*										mVertices;
		unsigned int								mNumVertices;
		TCGraphicsContext::TCBufferContext*			mBufferContext;

	protected:	// Methods
		virtual	bool				IsValidDescription( Description& desc );
		virtual void				Clone( const TCVertexBuffer& inRef );

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