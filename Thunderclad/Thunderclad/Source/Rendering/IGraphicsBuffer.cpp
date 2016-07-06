//
// IGraphicsBuffer.cpp
// This class will define the common functionality for a graphics buffer.
//

//
// Includes
//

#include "IGraphicsBuffer.h"

//
// Defines
//

//
// Default Constructor
//		- Will initialize the object to a safe state.
// Inputs:
//		- TCGraphicsContext* context: The context associated with this object.
// Outputs:
//		- None.
//

IGraphicsBuffer::IGraphicsBuffer( TCGraphicsContext* context )
	: IGraphicsResource( context )
{
	mUsage			= Static;
	mBindIndex		= -1;
	mDataSize		= 0;
	mLockState		= Unlocked;
}

//
// Copy Constructor
//		- Will initialize the object to a copy of another buffer.
// Inputs:
//		- const IGraphicsBuffer& buffer: The buffer to copy.
// Outputs:
//		- None.
//

IGraphicsBuffer::IGraphicsBuffer( const IGraphicsBuffer& buffer )
	: IGraphicsResource( buffer )
{
	Clone( buffer );
}

//
// Assignment Operator
//		- Will initialize this instance to another.
// Inputs:
//		- const IGraphicsBuffer& buffer: The buffer to copy.
// Outputs:
//		- IGraphicsBuffer&: The graphics buffer after being initialized.
//

IGraphicsBuffer& IGraphicsBuffer::operator=( const IGraphicsBuffer& buffer )
{
	Release();
	Clone( buffer );

	return *this;
}

//
// Destructor
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

IGraphicsBuffer::~IGraphicsBuffer()
{
	Release();
}

//
// Initialize
//		- Will set this object up correctly.
// Inputs:
//		- Description& desc: The description to copy from.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult IGraphicsBuffer::Initialize( Description& desc )
{
	if( !IsValidDescription( desc ) )
	{
		return Failure_InvalidParameter;
	}

	mUsage = desc.mUsage;
	mDataSize = desc.mDataSize;

	return Success;
}

//
// Release
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult IGraphicsBuffer::Release()
{
	return IGraphicsResource::Release();
}

//
// Clone
//		- Will copy another IGraphicsBuffer
// Inputs:
//		- const IGraphicsBuffer& inRef: The reference to copy.
// Outputs:
//		- None.
//

void IGraphicsBuffer::Clone( const IGraphicsBuffer& inRef )
{
	IGraphicsResource::Clone( inRef );

	mUsage			= inRef.mUsage;
	mBindIndex		= inRef.mBindIndex;
	mDataSize		= inRef.mDataSize;
	mLockState		= inRef.mLockState;
}