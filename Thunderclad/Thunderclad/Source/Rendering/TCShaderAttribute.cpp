//
// TCShaderAttribute.cpp
// This file will define the functionality for a shader attribute.
//

//
// Includes
//

#include "TCShaderAttribute.h"
#include "TCLogger.h"

//
// Defines
//

#define TC_SHADER_ATTRIBUTE_LOG_ERROR(x) (TCLogger::GetInstance()->LogError( TCString("[TCShaderAttribute] ") + x))

//
// TCShaderAttribute -- Default Constructor
//		- Will initialize this attribute to a safe state.
// Inputs:
//		- TCGraphicsContext* context: The context that this attribute belongs to.
// Outputs:
//		- None
//

TCShaderAttribute::TCShaderAttribute( TCGraphicsContext* context )
	: IGraphicsResource( context )
{
	mType = kShaderAttributeTypeUnknown;
	mUsage = kShaderAttributeUsageUnknown;
	mContext = context->GetShaderContext();
}

//
// TCShaderAttribute -- Copy Constructor
//		- Will initialize this attribute to be a copy of another.
// Inputs:
//		- const TCShaderAttribute& toCopy
// Outputs:
//		- None
//

TCShaderAttribute::TCShaderAttribute( const TCShaderAttribute& toCopy )
	: IGraphicsResource( toCopy )
{
	mName = toCopy.mName;
	mType = toCopy.mType;
	mUsage = toCopy.mUsage;

	mContext = toCopy.mContext;
}

//
// TCShaderAttribute -- Destructor
//		- Will Release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//- None.
//

TCShaderAttribute::~TCShaderAttribute()
{
	Release();
}

//
// Assignment Operator
//		- Will make this instance a copy of another instance.
// Inputs:
//		- const TCShaderAttribute& toCopy
// Outputs:
//		- TCShaderAttribute&: A reference to this object after it's been changed.
//

TCShaderAttribute& TCShaderAttribute::operator=( const TCShaderAttribute& toCopy )
{
	Release();
	Clone( toCopy );

	return *this;
}

//
// Initialize
//		- Will initialize this instance so it can be used.
// Inputs:
//		- Description* description: The information needed to initialize this instance.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderAttribute::Initialize( TCShaderAttribute::Description* description )
{
	if( mContext == NULL )
	{
		TC_SHADER_ATTRIBUTE_LOG_ERROR( "Failed to initialize shader attribute, context was invalid.");
		return Failure_InvalidState;
	}

	if( description == NULL )
	{
		TC_SHADER_ATTRIBUTE_LOG_ERROR( "Failed to initialize shader attribute, no description was provided." );
		return Failure_InvalidParameter;
	}

	if( description->name.IsEmpty() )
	{
		TC_SHADER_ATTRIBUTE_LOG_ERROR( "Failed to initialize shader attribute, no name provided." );
		return Failure_InvalidParameter;
	}

	if( description->type == kShaderAttributeTypeUnknown )
	{
		TC_SHADER_ATTRIBUTE_LOG_ERROR( "Failed to initialize shader attribute, no type provided." );
		return Failure_InvalidParameter;
	}

	if( description->usage == kShaderAttributeUsageUnknown )
	{
		TC_SHADER_ATTRIBUTE_LOG_ERROR( "Failed to initialize shader attribute, no usage provided." );
		return Failure_InvalidParameter;
	}

	return Success;
}

//
// Release
//		- This function will release all associated resources.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderAttribute::Release()
{
	if( mContext == NULL )
	{
		return Failure_InvalidState;
	}

	TCResult result = mContext->ReleaseShaderAttribute( this );
	if( TC_FAILED( result ) )
	{
		return result;
	}

	return IGraphicsResource::Release();
}

//
// TypeToString
//		- Will convert a type to a string.
// Inputs:
//		- TCShaderUniform::Type type: The type to convert.
// Outputs:
//		- TCString: The type string.
//

TCString TCShaderAttribute::TypeToString( TCShaderAttribute::Type type )
{
	switch( type )
	{
		case kShaderAttributeTypeFloat:		return TCString( "kShaderAttributeTypeFloat" );
		case kShaderAttributeTypeFloat2:	return TCString( "kShaderAttributeTypeFloat2" );
		case kShaderAttributeTypeFloat3:	return TCString( "kShaderAttributeTypeFloat3" );
		case kShaderAttributeTypeFloat4:	return TCString( "kShaderAttributeTypeFloat4" );
		case kShaderAttributeTypeMatrix3x3:	return TCString( "kShaderAttributeTypeMatrix3x3" );
		case kShaderAttributeTypeMatrix4x4:	return TCString( "kShaderAttributeTypeMatrix4x4" );
		default:							return TCString( "kShaderAttributeTypeUnknown" );
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

TCShaderAttribute::Type TCShaderAttribute::StringToType( TCString& string )
{
	for( int currentAttributeType = 0; currentAttributeType < (int)kNumShaderAttributeTypes; ++currentAttributeType )
	{
		if( TypeToString( (TCShaderAttribute::Type) currentAttributeType ) == string )
		{
			return (TCShaderAttribute::Type)currentAttributeType;
		}
	}

	return kShaderAttributeTypeUnknown;
}

//
// UsageToString
//		- Will convert a usage to a string.
// Inputs:
//		- TCShaderUniform::Usage usage: The usage to convert.
// Outputs:
//		- TCString: The usage string.
//

TCString TCShaderAttribute::UsageToString( TCShaderAttribute::Usage usage )
{
	switch( usage )
	{
		case kShaderAttributeUsageBoneIndex:		return TCString( "kShaderAttributeUsageBoneIndex" );
		case kShaderAttributeUsageBoneWeight:		return TCString( "kShaderAttributeUsageBoneWeight" );
		case kShaderAttributeUsageColor0:			return TCString( "kShaderAttributeUsageColor0" );
		case kShaderAttributeUsageColor1:			return TCString( "kShaderAttributeUsageColor1" );
		case kShaderAttributeUsageColor2:			return TCString( "kShaderAttributeUsageColor2" );
		case kShaderAttributeUsageNormal:			return TCString( "kShaderAttributeUsageNormal" );
		case kShaderAttributeUsagePosition:			return TCString( "kShaderAttributeUsagePosition" );
		case kShaderAttributeUsageTexCoord0:		return TCString( "kShaderAttributeUsageTexCoord0" );
		case kShaderAttributeUsageTexCoord1:		return TCString( "kShaderAttributeUsageTexCoord1" );
		case kShaderAttributeUsageTexCoord2:		return TCString( "kShaderAttributeUsageTexCoord2" );
		case kShaderAttributeUsageTexCoord3:		return TCString( "kShaderAttributeUsageTexCoord3" );
		default:									return TCString( "kShaderAttributeUsageUnknown" );
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

TCShaderAttribute::Usage TCShaderAttribute::StringToUsage( TCString& string )
{
	for( int currentUniformUsage = 0; currentUniformUsage < (int)kNumShaderAttributeUsages; ++currentUniformUsage )
	{
		if( UsageToString( (TCShaderAttribute::Usage) currentUniformUsage ) == string )
		{
			return (TCShaderAttribute::Usage)currentUniformUsage;
		}
	}

	return kShaderAttributeUsageUnknown;
}

//
// Clone
//		- This function will make this instance a copy of another instance.
// Inputs:
//		- const TCShaderAttribute& toCopy: The attribute to copy.
// Outputs:
//		- None.
//

void TCShaderAttribute::Clone( const TCShaderAttribute& toCopy )
{
	IGraphicsResource::Clone( toCopy );

	mName		= toCopy.mName;
	mContext	= toCopy.mContext;
	mType		= toCopy.mType;
	mUsage		= toCopy.mUsage;
}