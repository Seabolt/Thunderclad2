//
// IGraphicsResource.cpp
// This file will define the interface for a graphics resource.
//

//
// Includes
//

#include "IGraphicsResource.h"
#include "TCGraphicsContext.h"

//
// Defines
//

//
// Default Constructor
//		- This will initialize the object to a safe state.
// Inputs:
//		- TCGraphicsContext* context: The context to associate this resource with.
// Outputs:
//		- None.
//

IGraphicsResource::IGraphicsResource( TCGraphicsContext* context )
{
	TC_ASSERT( context );
	mGraphicsContext	= context;
	mResourceBlob		= NULL; 
}

//
// Copy Constructor
//		- This will copy another resource into this one.
// Inputs:
//		- IGraphicsResource& inRef: The reference to copy from.
// Outputs:
//		- None.
//

IGraphicsResource::IGraphicsResource( const IGraphicsResource& inRef )
{
	Clone( inRef );
}

//
// Destructor
//		- This will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

IGraphicsResource::~IGraphicsResource()
{
	Release();
}

//
// Release
//		- This will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult IGraphicsResource::Release()
{
	return Success;
}

//
// Assignment Operator
//		- This will set this instance to another.
// Inputs:
//		- const IGraphicsResource& inRef: The reference to copy.
// Outputs:
//		- IGraphicsResource&: The reference to this.
//

IGraphicsResource& IGraphicsResource::operator=( const IGraphicsResource& inRef )
{
	Release();
	Clone( inRef );

	return *this;
}

//
// Clone
//		- This will set this reference to another.
// Inputs:
//		- const IGraphicsResource& inRef: The reference to copy.
// Outputs:
//		- None.
//

void IGraphicsResource::Clone( const IGraphicsResource& inRef )
{
	mGraphicsContext	= inRef.mGraphicsContext;
}