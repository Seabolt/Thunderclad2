//
// TCGraphicsContext.cpp
// This file will define the platform agnostic interface for a graphics context.
//

//
// Includes
//

#include "TCGraphicsContext.h"
#include "TCPlatformPrecompilerSymbols.h"

//
// Defines
//

//
// Default Constructor
//		- Will initialize this object to a safe state.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCGraphicsContext::TCGraphicsContext()
{
	mWindow = NULL;
	mFileManager = NULL;
}

//
// Destructor
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCGraphicsContext::~TCGraphicsContext()
{
	
}

//
// TCPlatformData Constructor
//		- Will initialize a platform resource's data.
// Inputs:
//		- TCGraphicsContext* context: The context this resource belongs to.
// Outputs:
//		- None.
//

TCGraphicsContext::TCPlatformData::TCPlatformData( TCGraphicsContext* context )
{
	TC_ASSERT( context && "Tried to initialize platform data without a context." );
	mContext = context;
	mNumReferences = 1;
}

//
// Initialize
//		- Will allocate all resources associated with this object.
// Inputs:
//		- Description&: The description of how to initialize this object.
// Outputs:
//		- TCResult: The result of the operation.
//			- Failed_InvalidParameter: The description contains an invalid parameter.
//			- Failed_InvalidOperation: The description contains instructions to do something invalid.
//			- Success: The operation succeeded.
//

TCResult TCGraphicsContext::Initialize( TCGraphicsContext::Description& description )
{
	if( IsValidDescription( description ) == false )
	{
		return Failure_InvalidParameter;
	}

	mWindow = description.mWindow;
	mFileManager = description.mFileManager;

	return TCResult::Success;
}

//
// Destroy
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The operation succeeded.
//

TCResult TCGraphicsContext::Destroy()
{
	return TCResult::Success;
}

//
// BeginFrame
//		- Will prepare the context to render for an upcoming frame.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The operation succeeded.
//

TCResult TCGraphicsContext::BeginFrame()
{
	return TCResult::Success;
}

//
// EndFrame
//		- Will prepare the context to end a frame.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The operation succeeded.
//

TCResult TCGraphicsContext::EndFrame()
{
	return TCResult::Success;
}

//
// SwapBuffers
//		- Will swap the front buffer with the next buffer.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The operation succeeded.
//

TCResult TCGraphicsContext::SwapBuffers()
{
	return TCResult::Success;
}

//
// IsValidDescription
//		- Will determine if a description is valid for use.
// Inputs:
//		- Description: The structure to determine if it's valid.
// Outputs:
//		- bool: Was the description valid or not.
//

bool TCGraphicsContext::IsValidDescription( TCGraphicsContext::Description& desc )
{
	if( desc.mWindow == NULL )
	{
		return false;
	}

	if( desc.mFileManager == NULL )
	{
		return false;
	}

	if( desc.mNumBuffers <= 0 )
	{
		return false;
	}

	return true;
}