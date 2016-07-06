//
// TCConstantBuffer.cpp
// This file will define the functionality of the platform agnostic layer for a constant buffer.
//

//
// Includes
//

#include "TCConstantBuffer.h"
#include "TCLogger.h"
#include "TCShaderUniform.h"
#include "TCShader.h"

//
// Defines
//

#define TC_CONSTANT_BUFFER_LOG_ERROR( x ) (TCLogger::GetInstance()->LogError( TCString("[TCConstantBuffer] ") + x))

//
// TCConstantBuffer::Description -- Default Constructor
//		- This function will initialize the constant buffer description to the appropriate default values.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCConstantBuffer::Description::Description()
{
	name = "";
	bufferRegister = kInvalidRegister;
	bufferSize = 0;
	shaderType = TCShader::kShaderTypeUnknown;
}

//
// TCConstantBuffer -- Default Constructor
//		- This function will initialize the constant buffer to a safe state.
// Inputs:
//		- TCGraphicsContext* context: The context to use.
// Outputs:
//		- None
//

TCConstantBuffer::TCConstantBuffer( TCGraphicsContext* context )
	: IGraphicsResource( context )
{
	if( context == NULL )
	{
		TC_CONSTANT_BUFFER_LOG_ERROR( "Failed to create TCConstantBuffer: NULL context provided." );
		return;
	}

	mContext		= context->GetShaderContext();
	mRegister		= kInvalidRegister;
	mName			= "";
	mIsBound		= false;
	mShaderType		= TCShader::kShaderTypeUnknown;
}

//
// TCConstantBuffer -- Copy Constructor
//		- This function will initialize the constant buffer to be a copy of another constant buffer.
// Inputs:
//		- const TCConstantBuffer& toCopy
// Outputs:
//		- None.
//

TCConstantBuffer::TCConstantBuffer( const TCConstantBuffer& toCopy )
	: IGraphicsResource( toCopy )
{
	Clone( toCopy );
}

//
// TCConstantBuffer -- Destructor
//		- This function will release all reasources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCConstantBuffer::~TCConstantBuffer()
{
	Release();
}

//
// TCConstantBuffer -- Assignment Operator
//		- This function will set this instance to be equal to another instance.
// Inputs:
//		- const TCConstantBuffer& toCopy: The instance to copy.
// Outputs:
//		- TCConstantBuffer&: The reference to this instance after it's been set to be a copy of the input.
//

TCConstantBuffer& TCConstantBuffer::operator=( const TCConstantBuffer& toCopy )
{
	Release();
	Clone( toCopy );

	return *this;
}

//
// Initialize
//		- Will initialize this instance so that it can be used.
// Inputs:
//		- Description* description: The information necessary to initialize correctly.
// Outputs:
//		- TCResult: The result of the operation:
//			- Failure_InvalidParameter:
//				- The description is NULL
//				- The buffer register is < 0
//				- The name is empty.
//			- Failure_InvalidState:
//				- The context is invalid.
//			- Success
//				- The operation completed successfully.
//

TCResult TCConstantBuffer::Initialize( TCConstantBuffer::Description* description )
{
	Release();

	//
	// Check for valid data.
	//

	if( mContext == NULL )
	{
		TC_CONSTANT_BUFFER_LOG_ERROR( "Failed to initialize TCConstantBuffer: Context was invalid.");
		return Failure_InvalidState;
	}

	if( description == NULL )
	{
		TC_CONSTANT_BUFFER_LOG_ERROR( "Failed to initialize TCConstantBuffer: Description was NULL." );
		return Failure_InvalidParameter;
	}

	if( description->name.IsEmpty() )
	{
		TC_CONSTANT_BUFFER_LOG_ERROR( "Failed to initialize TCConstantBuffer: Name was empty. All constant buffers must be named." );
		return Failure_InvalidParameter;
	}

	if( description->bufferRegister < 0 )
	{
		TC_CONSTANT_BUFFER_LOG_ERROR( "Failed to initialize TCConstantBuffer: Buffer Register was invalid." );
		return Failure_InvalidParameter;
	}

	//
	// Copy over the necessary data.
	//

	mName		= description->name;
	mRegister	= description->bufferRegister;

	//
	// Perform platform initialization.
	//

	return mContext->CreateConstantBuffer( (void*)description, this );
}

//
// Release
//		- This function will release all resources associated with an object.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The operation completed successfully.
//

TCResult TCConstantBuffer::Release()
{
	if( mContext == NULL )
		return Failure_InvalidState;

	//
	// Constant Buffer assume ownership of all shader uniforms, since on DX11 they don't cross buffer boundaries.
	//

	for( int currentShaderUniform = 0; currentShaderUniform < mShaderUniforms.Count(); ++currentShaderUniform )
	{
		mShaderUniforms[ currentShaderUniform ]->Release();
	}
	mShaderUniforms.Clear();
	
	//
	// Perform the platform release.
	//

	TCResult result = mContext->ReleaseConstantBuffer(this);
	if( TC_FAILED( result ) )
	{
		return result;
	}

	return IGraphicsResource::Release();
}

//
// Bind
//		- This function will bind this constant buffer for use.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation. See the platform mContext->BindConstantBuffer()'s documentation.
//

TCResult TCConstantBuffer::Bind()
{
	//
	// Check to see if we're already bound.
	//

	if( mIsBound )
	{
		return Success_Cached;
	}

	//
	// Make sure we have a valid context.
	//

	if( mContext == NULL )
	{
		TC_CONSTANT_BUFFER_LOG_ERROR( "Failed to bind TCConstantBuffer, invalid context" );
		return Failure_InvalidState;
	}

	//
	// Call the platform Bind.
	//

	TCResult result = mContext->BindConstantBuffer( this );
	if( TC_SUCCEEDED( result ) )
	{
		mIsBound = true;
	}

	return result;
}

//
// Unbind
//		- This function will unbind this constant buffer from use.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//			- Check the platform documentation for this method.
//

TCResult TCConstantBuffer::Unbind()
{
	//
	// Check to see if we are bound.
	//

	if( mIsBound == false )
	{
		return Failure_InvalidState;
	}

	//
	// Call the platform Unbind.
	//

	TCResult result = mContext->UnbindConstantBuffer( this );
	if( TC_SUCCEEDED( result ) )
	{
		mIsBound = false;
	}

	return result;
}

//
// UpdateShaderUniform
//		- This will update a specific shader uniform that is managed by this constant buffer.
// Inputs:
//		- TCShaderUniform* uniform: The uniform that should be updated.
// Outputs:
//		- TCResult: The result of the operation.
//			- Failed_InvalidState: The context is invalid.
//			- Failed_InvalidParameter: The specified shader uniform isn't managed by this constant buffer, or the shader uniform provided is null. 
//			- For more information check the platform layer information.
//

TCResult TCConstantBuffer::UpdateShaderUniform( TCShaderUniform* uniform )
{
	//
	// Make sure we have a valid context.
	//

	if( mContext == NULL )
	{
		TC_CONSTANT_BUFFER_LOG_ERROR( "Failed to update shader uniform: Context is NULL" );
		return Failure_InvalidState;
	}

	//
	// Make sure the uniform is not NULL.
	//

	if( uniform == NULL )
	{
		TC_CONSTANT_BUFFER_LOG_ERROR( "Failed to update shader uniform: Provided uniform is NULL" );
		return Failure_InvalidParameter;
	}

	//
	// Make sure the uniform is owned by us
	//

	if( GetShaderUniform( uniform->GetName().Data() ) == NULL )
	{
		TC_CONSTANT_BUFFER_LOG_ERROR( "Failed to update shader uniform: Uniform isn't managed by this constant buffer." );
		return Failure_InvalidParameter;
	}

	//
	// Call the platform update.
	//

	TCResult result = mContext->SetConstantBufferUniform( this, uniform );
	if( TC_FAILED( result ) )
	{
		TC_CONSTANT_BUFFER_LOG_ERROR( "Failed to update constant buffer: " + result );
		return result;
	}

	return result;
}

//
// UpdateShaderUniform
//		- This will update a specific shader uniform that is managed by this constant buffer.
// Inputs:
//		- const char* uniformName: The name of the uniform to update.
//		- void* uniformData: The data to update the uniform with.
//		- unsigned int uniformDataSize: The size of the data in the uniform.
// Outputs:
//		- TCResult: The result of the operation.
//			- Failed_InvalidState: The context is invalid.
//			- Failed_InvalidParameter: The specified shader uniform isn't managed by this constant buffer, or the shader uniform provided is null.
//				- If the uniformData is NULL.
//				- If the size of the uniform data doesn't match the uniform specified's data size.
//			- For more information check the platform layer information.
//

TCResult TCConstantBuffer::UpdateShaderUniform( const char8* uniformName, void* uniformData, unsigned int uniformDataSize )
{	
	//
	// Get the shader uniform specified. 
	//

	TCShaderUniform* uniform = GetShaderUniform( uniformName );
	if( uniform == NULL )
	{
		TC_CONSTANT_BUFFER_LOG_ERROR( "Failed to update shader uniform: Uniform isn't managed by this constant buffer." );
		return Failure_InvalidParameter;
	}

	//
	// Make sure the uniform size and the data size provided are a match.
	//

	if( uniform->GetDataSize() != uniformDataSize )
	{
		TC_CONSTANT_BUFFER_LOG_ERROR( "Failed to update shader uniform: Uniform data size provided doesn't match the uniform's data size." );
		return Failure_InvalidParameter;
	}

	//
	// Make sure the uniform data provided is valid.
	//

	if( uniformData == NULL )
	{
		TC_CONSTANT_BUFFER_LOG_ERROR( "Failed to update shader uniform: Uniform data was NULL." );
		return Failure_InvalidParameter;
	}

	//
	// Update the shader uniform, this will call back into this class as part of the standard update flow.
	//

	return uniform->SetValue( uniformData, uniformDataSize );
}

//
// GetShaderUniform
//		- This function will return a uniform with the name specified if managed by this constant buffer.
// Inputs:
//		- const char* uniformName: The name of the uniform.
// Outputs:
//		- TCShaderUniform* uniform: The uniform found, NULL if not found.
//

TCShaderUniform* TCConstantBuffer::GetShaderUniform( const char8* uniformName )
{
	//
	// Search our list of shader uniforms for a uniform with the name provided.
	//

	for( int currentShaderUniform = 0; currentShaderUniform < mShaderUniforms.Count(); ++currentShaderUniform )
	{
		if( mShaderUniforms[ currentShaderUniform ] != NULL && mShaderUniforms[ currentShaderUniform ]->GetName().Equal( uniformName ) )
		{
			return mShaderUniforms[ currentShaderUniform ];
		}
	}

	return NULL;
}

//
// GetShaderUniform
//		- This function will return a uniform with the name specified if managed by this constant buffer.
// Inputs:
//		- unsigned int shaderUniformIndex: The index of the uniform.
// Outputs:
//		- TCShaderUniform* uniform: The uniform found, NULL if not found.
//

TCShaderUniform* TCConstantBuffer::GetShaderUniform( unsigned int uniformIndex )
{
	if( uniformIndex > (unsigned int)mShaderUniforms.Count() ) return NULL;

	return mShaderUniforms[ uniformIndex ];
}

//
// Clone
//		- This function will make this instance equal to another instance.
// Inputs:
//		- TCConstantBuffer& toCopy: The constant buffer to copy.
// Outputs:
//		- None.
//

void TCConstantBuffer::Clone( const TCConstantBuffer& toCopy )
{
	IGraphicsResource::Clone( toCopy );

	mShaderUniforms	= toCopy.mShaderUniforms;
	mContext		= toCopy.mContext;
	mIsBound		= toCopy.mIsBound;
	mName			= toCopy.mName;
	mRegister		= toCopy.mRegister;
	mShaderType		= toCopy.mShaderType;
}
