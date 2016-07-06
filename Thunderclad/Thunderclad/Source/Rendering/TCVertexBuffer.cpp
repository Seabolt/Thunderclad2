//
// TCVertexBuffer.cpp
// This file will define the functionality for a vertex buffer.
//

//
// Includes
//

#include "TCVertexBuffer.h"
#include "TCMemUtils.h"
#include "TCGraphicsContext.h"

//
// Defines
//

TCVertexBuffer::TCVertexBuffer( TCGraphicsContext* context )
	: IGraphicsBuffer( context )
{
	mVertexFormat = NULL;
	mNumVertices = 0;
	mVertices = NULL;
	mType = Vertex;

	mBufferContext = context->GetBufferContext();
}

TCVertexBuffer::TCVertexBuffer( const TCVertexBuffer& inRef )
	: IGraphicsBuffer( inRef )
{
	Clone( inRef );
}

TCVertexBuffer& TCVertexBuffer::operator=( const TCVertexBuffer& inRef )
{
	Release();
	Clone( inRef );

	return *this;
}

TCVertexBuffer::~TCVertexBuffer()
{
	Release();
}

TCResult TCVertexBuffer::Initialize( IGraphicsBuffer::Description& inDesc )
{
	TCVertexBuffer::Description& desc = (TCVertexBuffer::Description&)inDesc;

	//
	// Call our base initialize
	//

	TCResult result = IGraphicsBuffer::Initialize( desc );
	if( TC_FAILED( result ) )
	{
		return result;
	}

	//
	// Copy our resources from the description.
	//

	mNumVertices = desc.mNumVertices;
	mVertexFormat = desc.mVertexFormat;		TC_ASSERT( mVertexFormat != NULL );

	//
	// Copy our vertices if necessary.
	//

	unsigned int dataSize = mNumVertices * mVertexFormat->Size();
	if( !desc.mDiscardVerts )
	{
		mVertices = new unsigned char[ dataSize ];
		TCMemoryUtils::MemCopy( mVertices, desc.mData, dataSize );
	}

	//
	// Create our buffer
	//

	result = mBufferContext->CreateVertexBuffer( &desc, this );
	if( TC_FAILED( result ) )
	{
		return result;
	}

	//
	// Fill our buffer.
	//

	if( desc.mData != NULL )
	{
		result = Fill( desc.mData, dataSize );
		if( TC_FAILED( result ) )
		{
			return result;
		}
	}

	return result;
}

TCResult TCVertexBuffer::Release()
{
	if( mBufferContext == NULL )			return Success_Unhandled;
	TC_SAFE_DELETE_ARRAY( mVertices );

	TCResult result = mBufferContext->Release( this );
	if( TC_FAILED( result ) )
	{
		return result;
	}

	return IGraphicsBuffer::Release();
}

TCResult TCVertexBuffer::Bind( unsigned int index )
{
	if( mBufferContext == NULL )			return Failure_InvalidOperation;
	
	//
	// Bind this buffer
	//

	return mBufferContext->BindVertexBuffer( this, index );
}

TCResult TCVertexBuffer::Unbind()
{
	if( mBindIndex < 0 )					return Failure_InvalidOperation;
	if( mBufferContext == NULL )			return Failure_InvalidOperation;

	return mBufferContext->UnbindVertexBuffer( this );
}

TCResult TCVertexBuffer::Fill( void* data, unsigned int dataSize )
{
	if( data == NULL || dataSize <= 0 )		return Failure_InvalidParameter;
	if( mBufferContext == NULL )			return Failure_InvalidOperation;

	return mBufferContext->Fill( this, data, dataSize );
}

bool TCVertexBuffer::IsValidDescription( TCVertexBuffer::Description& desc )
{
	if( IGraphicsBuffer::IsValidDescription( (IGraphicsBuffer::Description&)desc ) == false )
		return false;

	if( desc.mNumVertices <= 0 )		return false;
	if( desc.mVertexFormat == NULL )	return false;

	return true;
}

void TCVertexBuffer::Clone( const TCVertexBuffer& inRef )
{
	//
	// Call our base clone.
	//

	IGraphicsBuffer::Clone( inRef );

	//
	// Copy our resources.
	//

	mNumVertices = inRef.mNumVertices;
	mVertexFormat = inRef.mVertexFormat;

	if( mVertexFormat == NULL )
	{
		TC_ASSERT( 0 );
	}

	unsigned int dataSize = mNumVertices * mVertexFormat->Size();
	mVertices = new unsigned char[ dataSize ];
	TCMemoryUtils::MemCopy( mVertices, inRef.mVertices, dataSize );
}