//
// TCIndexBuffer.cpp
// This file will define the functionality for a index buffer.
//

//
// Includes
//

#include "TCIndexBuffer.h"
#include "TCMemUtils.h"
#include "TCGraphicsContext.h"

//
// Defines
//

TCIndexBuffer::TCIndexBuffer( TCGraphicsContext* context )
	: IGraphicsBuffer( context )
{
	mNumIndices = 0;
	mIndexSize = 0;
	mType = Index;

	mBufferContext = context->GetBufferContext();
}

TCIndexBuffer::TCIndexBuffer( TCIndexBuffer& inRef )
	: IGraphicsBuffer( inRef )
{
	Clone( inRef );
}

TCIndexBuffer& TCIndexBuffer::operator=( TCIndexBuffer& inRef )
{
	Release();
	Clone( inRef );

	return *this;
}

TCIndexBuffer::~TCIndexBuffer()
{
	Release();
}

TCResult TCIndexBuffer::Initialize( IGraphicsBuffer::Description& inDesc )
{
	TCIndexBuffer::Description& desc = (TCIndexBuffer::Description&)inDesc;

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

	mNumIndices = desc.mNumIndices;
	mIndexSize = desc.mIndexSize;

	//
	// Create our buffer
	//

	result = mBufferContext->CreateIndexBuffer( &desc, this );
	if( TC_FAILED( result ) )
	{
		return result;
	}

	//
	// Fill our buffer.
	//

	if( desc.mData != NULL )
	{
		result = Fill( desc.mData, mIndexSize * mNumIndices );
		if( TC_FAILED( result ) )
		{
			return result;
		}
	}

	return result;
}

TCResult TCIndexBuffer::Release()
{
	if( mBufferContext == NULL )			return Success_Unhandled;
	TCResult result = mBufferContext->Release( this );

	if( TC_FAILED( result ) )
	{
		return result;
	}

	return IGraphicsBuffer::Release();
}

TCResult TCIndexBuffer::Bind( unsigned int offset )
{
	if( mBufferContext == NULL )			return Failure_InvalidOperation;
	
	//
	// Bind this buffer
	//

	return mBufferContext->BindIndexBuffer( this, offset );
}

TCResult TCIndexBuffer::Unbind()
{
	if( mBindIndex < 0 )					return Failure_InvalidOperation;
	if( mBufferContext == NULL )			return Failure_InvalidOperation;

	return mBufferContext->UnbindIndexBuffer( this );
}

TCResult TCIndexBuffer::Fill( void* data, unsigned int dataSize )
{
	if( data == NULL || dataSize <= 0 )		return Failure_InvalidParameter;
	if( mBufferContext == NULL )			return Failure_InvalidOperation;

	return mBufferContext->Fill( this, data, dataSize );
}

bool TCIndexBuffer::IsValidDescription( TCIndexBuffer::Description& desc )
{
	if( IGraphicsBuffer::IsValidDescription( (IGraphicsBuffer::Description&)desc ) == false )
		return false;

	if( desc.mNumIndices <= 0 )		return false;

	if( desc.mIndexSize != sizeof( short ) &&
		desc.mIndexSize != sizeof( int ) )	
		return false;

	return true;
}

void TCIndexBuffer::Clone( TCIndexBuffer& inRef )
{
	//
	// Call our base clone.
	//

	IGraphicsBuffer::Clone( inRef );

	//
	// Copy our resources.
	//

	mNumIndices = inRef.mNumIndices;
	mIndexSize = inRef.mIndexSize;

	//
	// Copy our graphics resources.
	//

	mBufferContext->Copy( &inRef, this );
}