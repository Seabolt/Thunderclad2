//
// TCShaderProgram.cpp
// This file will implement the delegate class for a Shader Program.
//

//
// Includes
//

#include "TCShaderProgram.h"
#include "TCGraphicsContext.h"
#include "TCLogger.h"

//
// Defines
//

#define TC_SHADER_PROGRAM_LOG_ERROR(x) (TCLogger::GetInstance()->LogError(TCString("[TCShaderProgram] ") + x))

//
// TCShaderProgram -- Default Constructor
//		- This will initialize the shader program to a safe default state.
// Inputs:
//		- TCGraphicsContext* context: The context where we can pull the shader context from.
// Outputs:
//		- None
//

TCShaderProgram::TCShaderProgram( TCGraphicsContext* context )
	: IGraphicsResource( context )
{
	if( context == NULL )
	{
		TC_SHADER_PROGRAM_LOG_ERROR("Tried to initialize a shader program without a valid TCGraphicsContext!");
	}

	mContext = context->GetShaderContext();
	mIsBound = false;
	mVertexFormat = NULL;
	mVertexShader = NULL;
	mPixelShader = NULL;
}

//
// TCShaderProgram -- Copy Constructor
//		- This will initialize this program to be a copy of another program.
// Inputs:
//		- const TCShaderProgram& toCopy
// Outputs:
//		- None.
//

TCShaderProgram::TCShaderProgram( const TCShaderProgram& toCopy )
	: IGraphicsResource( toCopy )
{
	Clone( toCopy );
}

//
// TCShaderProgram -- Destructor
//		- This will release all resources associated with this program
// Inputs:
//		- None
// Outputs:
//		- None.
//

TCShaderProgram::~TCShaderProgram()
{
	Release();
}

//
// TCShaderProgram -- Assignment Operator
//		- This will set this instance of a shader program to be a copy of another program.
// Inputs:
//		- const TCShaderProgram& toCopy
// Outputs:
//		- TCShaderProgram&: The reference to this instance after it's been changed.
//

TCShaderProgram& TCShaderProgram::operator=( const TCShaderProgram& toCopy )
{
	Release();
	Clone( toCopy );

	return *this;
}

//
// Initialize
//		- This will initialize this program so it can be used.
// Inputs:
//		- TCShaderProgramDescription* description: The information needed to initialize this object.
// Outputs:
//		- TCResult: The result of the operation.
//			- Failure_InvalidParameter: The description is NULL, or one of the shader are NULL.
//			- Failure_InvalidOperation: The vertex shader and the vertex format are incompatible.
//			- Success: The operation completed successfully.
//

TCResult TCShaderProgram::Initialize( TCShaderProgramDescription* description )
{
	if( description == NULL )
		return Failure_InvalidParameter;

	if( description->mVertexShader == NULL )
	{
		TC_SHADER_PROGRAM_LOG_ERROR( "Failed to initialize shader program: a NULL vertex shader was provided." );
		return Failure_InvalidParameter;
	}

	if( description->mPixelShader == NULL )
	{
		TC_SHADER_PROGRAM_LOG_ERROR( "Failed to initialize shader program: a NULL pixel shader was provided." );
		return Failure_InvalidParameter;
	}

	//
	// TODO: Need to test if the vertex format matches the vertex shader.
	//

	mVertexShader = description->mVertexShader;
	mPixelShader = description->mPixelShader;
	mVertexFormat = description->mVertexFormat;

	return Success;
}

//
// Release
//		- This method will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderProgram::Release()
{
	if( mGraphicsContext == NULL || mGraphicsContext->GetShaderContext() == NULL )
	{
		return Failure_InvalidState;
	}

	TCResult result = mGraphicsContext->GetShaderContext()->ReleaseShaderProgram( this );
	if( TC_FAILED( result ) )
	{
		return result;
	}

	return IGraphicsResource::Release();
}

//
// Bind
//		- This method will bind this program for use.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderProgram::Bind()
{
	//
	// Check to see if our context valid.
	//

	if( mGraphicsContext == NULL || mGraphicsContext->GetShaderContext() == NULL )
	{
		return Failure_InvalidState;
	}

	//
	// Check to see if we're already bound.
	//

	if( mIsBound )
	{
		return Success_Cached;
	}

	//
	// Bind the shader program.
	//

	TCResult result = mGraphicsContext->GetShaderContext()->BindShaderProgram( this );
	if( TC_SUCCEEDED( result ) )
	{
		mIsBound = true;
	}

	return result;
}

//
// Unbind
//		- THis method will unbind this program from use.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderProgram::Unbind()
{
	//
	// Check to see if we're in a valid state.
	//

	if( mGraphicsContext == NULL || mGraphicsContext->GetShaderContext() == NULL )
	{
		return Failure_InvalidState;
	}

	//
	// Check to see if we're bound.
	//

	if( !mIsBound )
	{
		return Success_Cached;
	}

	//
	// Call the platform bind.
	//

	TCResult result = mGraphicsContext->GetShaderContext()->BindShaderProgram( this );
	if( TC_SUCCEEDED( result ) )
	{
		mIsBound = false;
	}

	return result;
}

//
// Clone
//		- This method will copy another instance into this instance.
// Inputs:
//		- const TCShaderProgram& toCopy: The reference to copy.
// Outputs:
//		- None.
//

void TCShaderProgram::Clone( const TCShaderProgram& toCopy )
{
	IGraphicsResource::Clone( toCopy );

	mVertexShader		= toCopy.mVertexShader;
	mPixelShader		= toCopy.mPixelShader;
	mVertexFormat		= toCopy.mVertexFormat;
	mIsBound			= toCopy.mIsBound;
	mGraphicsContext	= toCopy.mGraphicsContext;
	mProgramName		= toCopy.mProgramName;
}