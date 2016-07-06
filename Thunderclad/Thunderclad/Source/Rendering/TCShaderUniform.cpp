//
// TCShaderUniform.cpp
// This file will define the functionality for a shader uniform.
//

//
// Includes
//

#include "TCShaderUniform.h"
#include "TCLogger.h"
#include "TCMemUtils.h"

//
// Defines
//

#define TC_SHADER_UNIFORM_LOG_ERROR(x) (TCLogger::GetInstance()->LogError( TCString("[TCShaderUniform] ") + x ))

//
// TCShaderUniform::Description-- Destructor
//		- Will release all resources associated with this description.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCShaderUniform::Description::~Description()
{
	TC_SAFE_DELETE( defaultValue );
}

//
// TCShaderUniform -- Default Constructor
//		- This will initialize the shader uniform to a safe default state.
// Inputs:
//		- TCGraphicsContext* context: The context this shader uniform will be a child of.
// Outputs:
//		- None.
//

TCShaderUniform::TCShaderUniform( TCGraphicsContext* context )
	: IGraphicsResource( context )
{
	mContext				= NULL;
	mData					= NULL;
	mDefaultValue			= NULL;
	mParentConstantBuffer	= NULL;
	mName					= "";
	mType					= kShaderUniformTypeUnknown;
	mUsage					= kShaderUniformUsageUnknown;
	mRegister				= -1;

	if( context == NULL )
	{
		TC_SHADER_UNIFORM_LOG_ERROR( "Failed to create shader uniform: Invalid graphics context provided!" );
		return;
	}

	mContext = context->GetShaderContext();
}

//
// TCShaderUniform -- Copy Constructor
//		- This will initialize the shader uniform to a copy of another uniform.
// Inputs:
//		- const TCShaderUniform& toCopy: The shader uniform to copy.
// Outputs:
//		- None.
//

TCShaderUniform::TCShaderUniform( const TCShaderUniform& toCopy )
	: IGraphicsResource( toCopy )
{
	Clone( toCopy );
}

//
// TCShaderUniform -- Destructor
//		- This function will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCShaderUniform::~TCShaderUniform()
{
	Release();
}

//
// TCShaderUniform -- Assignment Operator
//		- This function will set this instance of a shader uniform to be a copy of another.
// Inputs:
//		- const TCShaderUniform& toCopy: The instance to copy.
// Outputs:
//		- TCShaderUniform&: The reference to this shader uniform after it's been changed.
//

TCShaderUniform& TCShaderUniform::operator=( const TCShaderUniform& toCopy )
{
	Release();
	Clone( toCopy );

	return *this;
}

//
// Initialize
//		- This function will initialize this ahder uniform for use.
// Inputs:
//		- Description* description: The information needed to initialize this uniform.
// Outputs:
//		- TCResult: The result of the operation.
//			- Failure_InvalidParameter: 
//				- Description is NULL
//				- Description.name is NULL or empty
//				- Descripiton.globalRegister is < 0 and Description.constantBufferName is empty.
//				- Descripiton.type is kShaderUniformTypeUnknown.
//			- Failure_InvalidState:
//				- mContext is NULL.
//			- Success
//				- The uniform initialized successfully.
//

TCResult TCShaderUniform::Initialize( TCShaderUniform::Description* description )
{
	if( description == NULL )
	{
		TC_SHADER_UNIFORM_LOG_ERROR( "Failed to initialize TCShaderUniform: NULL description provided." );
		return Failure_InvalidParameter;
	}

	if( description->name.IsEmpty() )
	{
		TC_SHADER_UNIFORM_LOG_ERROR( "Failed to initialize TCShaderUniform: Unnamed uniform provided." );
		return Failure_InvalidParameter;
	}

	if( description->globalRegister < 0 && description->constantBufferName.IsEmpty() )
	{
		TC_SHADER_UNIFORM_LOG_ERROR( "Failed to initialize TCShaderUniform: Uniform location is unknown." );
		return Failure_InvalidParameter;
	}

	if( description->type == kShaderUniformTypeUnknown )
	{
		TC_SHADER_UNIFORM_LOG_ERROR( "Failed to initialize TCShaderUniform: Uniform type is unknown." );
		return Failure_InvalidParameter;
	}

	if( description->defaultValue == NULL )
	{
		TC_SHADER_UNIFORM_LOG_ERROR( "Failed to initialize TCShaderUniform: Uniform default value is unknown." );
		return Failure_InvalidParameter;
	}

	if( mContext == NULL )
	{
		return Failure_InvalidState;
	}

	//
	// Set our type and name.
	//

	mType = description->type;
	mName = description->name;
	mRegister = description->constantBufferName.IsEmpty() ? description->globalRegister : description->constantBufferOffset;
	
	//
	// Get our constant buffer parent if applicable.
	//

	if( !description->constantBufferName.IsEmpty() )
	{
		mParentConstantBuffer = mContext->GetConstantBuffer( description->constantBufferName );
	}

	//
	// Allocate and copy over our data.
	//

	TC_SAFE_DELETE( mDefaultValue );
	mDefaultValue = new unsigned char[ GetDataSize() ];
	TCMemoryUtils::MemCopySafe( description->defaultValue, mDefaultValue, GetDataSize() );

	SetValue( mDefaultValue, GetDataSize() );

	//
	// Create Shader Uniform.
	//

	TCResult result = mContext->CreateShaderUniform( (void*)description, this );
	if( TC_FAILED( result ) )
	{
		TC_SHADER_UNIFORM_LOG_ERROR( "Failed to initialize TCShaderUniform: " + result );
		return result;
	}

	return result;
}

//
// SetValue
//		- This function will set the value of the shader uniform.
// Inputs:
//		- void* data: The data to set.
//		- unsigned int dataSize: The size of the data to set.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderUniform::SetValue( void* data, unsigned int dataSize )
{
	if( mContext == NULL )
	{
		return Failure_InvalidState;
	}

	if( dataSize != GetDataSize() )
	{
		TC_SHADER_UNIFORM_LOG_ERROR( "Failed to set value: Provided size was invalid" );
	}

	//
	// If they are providing NULL data, assume that they want the default value.
	//

	if( data == NULL )
	{
		data = mDefaultValue;
	}

	if( data == NULL )
	{
		return Failure_InvalidParameter;
	}

	//
	// Copy the data over.
	//

	TC_SAFE_DELETE( mData );
	mData = new unsigned char[ GetDataSize() ];
	TCMemoryUtils::MemCopySafe( data, mData, GetDataSize() );

	return Success;
}

//
// BindValue
//		- This method will flush the uniform for use.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderUniform::BindValue()
{
	return mContext->SetShaderUniform( this );
}

//
// Release
//		- This method will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderUniform::Release()
{
	if( mContext == NULL )
		return Failure_InvalidState;

	TCResult result = mContext->ReleaseShaderUniform( this );
	if( TC_FAILED( result ) )
	{
		return result;
	}

	TC_SAFE_DELETE( mDefaultValue );

	return IGraphicsResource::Release();
}

//
// GetDataSize
//		- This method will return the size of a TCShaderUniform::Type variable.
// Inputs:
//		- TCShaderUniform::Type type: The type of uniform.
// Outputs:
//		- unsigned int: The size of the uniform.
//

unsigned int TCShaderUniform::GetDataSize( TCShaderUniform::Type type )
{
	switch( type )
	{
		case kShaderUniformTypeFloat:		return sizeof( float );
		case kShaderUniformTypeFloat2:		return sizeof( float ) * 2;
		case kShaderUniformTypeFloat3:		return sizeof( float ) * 3;
		case kShaderUniformTypeFloat4:		return sizeof( float ) * 4;
		case kShaderUniformTypeInt:			return sizeof( int );
		case kShaderUniformTypeInt2:		return sizeof( int ) * 2;
		case kShaderUniformTypeInt3:		return sizeof( int ) * 3;
		case kShaderUniformTypeInt4:		return sizeof( int ) * 4;
		case kShaderUniformTypeHalf:		return sizeof( float );
		case kShaderUniformTypeHalf2:		return sizeof( float ) * 2;
		case kShaderUniformTypeHalf3:		return sizeof( float ) * 3;
		case kShaderUniformTypeHalf4:		return sizeof( float ) * 4;
		case kShaderUniformTypeMatrix3x3:	return sizeof( float ) * 9;
		case kShaderUniformTypeMatrix4x4:	return sizeof( float ) * 16;
		case kShaderUniformTypeBool:		return sizeof( bool );
		case kShaderUniformTypeBool2:		return sizeof( bool ) * 2;
		case kShaderUniformTypeBool3:		return sizeof( bool ) * 3;
		case kShaderUniformTypeBool4:		return sizeof( bool ) * 4;
		default:						return 0;
	}
}

//
// TypeToString
//		- Will convert a type to a string.
// Inputs:
//		- TCShaderUniform::Type type: The type to convert.
// Outputs:
//		- TCString: The type string.
//

TCString TCShaderUniform::TypeToString( TCShaderUniform::Type type )
{
	switch( type )
	{
		case kShaderUniformTypeBool:		return TCString( "kShaderUniformTypeBool" );
		case kShaderUniformTypeBool2:		return TCString( "kShaderUniformTypeBool2" );
		case kShaderUniformTypeBool3:		return TCString( "kShaderUniformTypeBool3" );
		case kShaderUniformTypeBool4:		return TCString( "kShaderUniformTypeBool4" );
		case kShaderUniformTypeFloat:		return TCString( "kShaderUniformTypeFloat" );
		case kShaderUniformTypeFloat2:		return TCString( "kShaderUniformTypeFloat2" );
		case kShaderUniformTypeFloat3:		return TCString( "kShaderUniformTypeFloat3" );
		case kShaderUniformTypeFloat4:		return TCString( "kShaderUniformTypeFloat4" );
		case kShaderUniformTypeHalf:		return TCString( "kShaderUniformTypeHalf" );
		case kShaderUniformTypeHalf2:		return TCString( "kShaderUniformTypeHalf2" );
		case kShaderUniformTypeHalf3:		return TCString( "kShaderUniformTypeHalf3" );
		case kShaderUniformTypeHalf4:		return TCString( "kShaderUniformTypeHalf4" );
		case kShaderUniformTypeInt:			return TCString( "kShaderUniformTypeInt" );
		case kShaderUniformTypeInt2:		return TCString( "kShaderUniformTypeInt2" );
		case kShaderUniformTypeInt3:		return TCString( "kShaderUniformTypeInt3" );
		case kShaderUniformTypeInt4:		return TCString( "kShaderUniformTypeInt4" );
		case kShaderUniformTypeMatrix3x3:	return TCString( "kShaderUniformTypeMatrix3x3" );
		case kShaderUniformTypeMatrix4x4:	return TCString( "kShaderUniformTypeMatrix4x4" );
		default:							return TCString( "kShaderUniformTypeUnknown" );
	}
}

//
// StringToType
//		- Will convert a string to a type.
// Inputs:
//		- TCString string: The string to convert.
// Outputs:
//		- TCShaderUniform::Type: The type.
//

TCShaderUniform::Type TCShaderUniform::StringToType( TCString& string )
{
	for( int currentUniformType = 0; currentUniformType < (int)kNumShaderUniformTypes; ++currentUniformType )
	{
		if( TypeToString( (TCShaderUniform::Type) currentUniformType ) == string )
		{
			return (TCShaderUniform::Type)currentUniformType;
		}
	}

	return kShaderUniformTypeUnknown;
}

//
// UsageToString
//		- Will convert a usage to a string.
// Inputs:
//		- TCShaderUniform::Usage usage: The usage to convert.
// Outputs:
//		- TCString: The usage string.
//

TCString TCShaderUniform::UsageToString( TCShaderUniform::Usage usage )
{
	switch( usage )
	{
		case kShaderUniformUsageEyePosition:		return TCString( "kShaderUniformUsageEyePosition" );
		case kShaderUniformUsageWorldPosition:		return TCString( "kShaderUniformUsageWorldPosition" );
		case kShaderUniformUsageProperty:			return TCString( "kShaderUniformUsageProperty" );
		default:									return TCString( "kShaderUniformUsageUnknown" );
	}
}

//
// StringToUsage
//		- Will convert a string to a usage.
// Inputs:
//		- TCString string: The string to convert.
// Outputs:
//		- TCShaderUniform::Usage: The usage.
//

TCShaderUniform::Usage TCShaderUniform::StringToUsage( TCString& string )
{
	for( int currentUniformUsage = 0; currentUniformUsage < (int)kNumShaderUniformUsages; ++currentUniformUsage )
	{
		if( UsageToString( (TCShaderUniform::Usage) currentUniformUsage ) == string )
		{
			return (TCShaderUniform::Usage)currentUniformUsage;
		}
	}

	return kShaderUniformUsageUnknown;
}

//
// Clone
//		- This function will make this instance a copy of another instance.
// Inputs:
//		- const TCShaderUniform& uniformToCopy: The uniform to copy.
// Outputs:
//		- None.
//

void TCShaderUniform::Clone( const TCShaderUniform& uniformToCopy )
{
	IGraphicsResource::Clone( uniformToCopy );

	mType					= uniformToCopy.mType;
	mName					= uniformToCopy.mName;
	mContext				= uniformToCopy.mContext;
	mUsage					= uniformToCopy.mUsage;
	mRegister				= uniformToCopy.mRegister;
	mParentConstantBuffer	= uniformToCopy.mParentConstantBuffer;
	
	//
	// Safely copy over the uniform data.
	//

	TC_SAFE_DELETE( mDefaultValue );
	mDefaultValue = new unsigned char[ GetDataSize() ];
	TCMemoryUtils::MemCopySafe( uniformToCopy.mDefaultValue, mDefaultValue, GetDataSize() );

	SetValue( uniformToCopy.mData, GetDataSize() );
}