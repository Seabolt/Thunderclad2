//
// TCShader.cpp
// This file will provide the definition for a TCShader
//

//
// Includes
//

#include "TCShader.h"
#include "TCLogger.h"
#include "TCShaderImporter.h"

//
// Defines
//

//
// TCShader::Description::Constructor
//		- Will initialize this description to a safe default state.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCShader::Description::Description()
{
	shaderType = kShaderTypeUnknown;
	shaderName = "Unnamed Shader";
	shaderImportData = NULL;
}

//
// TCShader Default Constructor
//		- This will initialize the shader to a safe default state.
// Inputs:
//		- TCGraphicsContext*: The parent context for this shader.
// Outputs:
//		- None
//

TCShader::TCShader( TCGraphicsContext* context )
	: IGraphicsResource( context )
{
	mShaderContext	= context->GetShaderContext();
	mIsBound		= false;
	mType			= TCShader::Type::kShaderTypeUnknown;
	mProfile		= kShaderProfileUnknown;
	mModel			= kShaderModelUnknown;
}

//
// TCShader Copy Constructor
//		- This will initialize a shader to a copy of another shader.
// Inputs:
//		- TCShader& toCopy: The shader to copy.
// Outputs:
//		- None.
//

TCShader::TCShader( TCShader& toCopy )
	: IGraphicsResource( toCopy )
{
	Clone( toCopy );
}

//
// TCShader Destructor
//		- This will release all resources associated with this shader.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCShader::~TCShader()
{
	Release();
}

//
// Assignment Operator
//		- This will set this shader equal to another shader.
// Inputs:
//		- const TCShader& toCopy: The shader to copy.
// Outputs:
//		- TCShader&: A reference to this object after being changed.
//

TCShader& TCShader::operator=( const TCShader& toCopy )
{
	Release();
	Clone( toCopy );

	return *this;
}

//
// Initialize
//		- This will initialize the underlying resources provided in the description.
// Inputs:
//		- TCShaderDescription& desc: The information needed to initialize this shader.
// Outputs:
//		- TCResult: The result of the operation.
//			- Failure_InvalidParameter: One of the parameters in the description is invalid.
//			- Failure_InvalidPath: The path provided was malformed.
//			- Failure_FileNotFound: The path provided didn't point to a file.
//			- Failure_InvalidState: The shader has an invalid shader context.
//

TCResult TCShader::Initialize( TCShader::Description& desc )
{
	mType = desc.shaderType;
	mName = desc.shaderName;

	TCResult result = Success;

	//
	// Check for a valid shader context.
	//

	if( mShaderContext == NULL )
		return Failure_InvalidState;

	//
	// Check to see if this shader is cached.
	//

	TCShader* prevShader = mShaderContext->GetShader( desc.shaderName );
	if( prevShader != NULL )
	{
		result = mShaderContext->AddReference( *this, prevShader->GetGraphicsResources() );
	}

	//
	// Now we need to import this shader.
	//

	TCShaderImporter* output;
	result = Import( desc.shaderFilepath, (void**)&output );
	if( TC_FAILED( result ) || output == NULL )
	{
		TCLogger::GetInstance()->LogError( TCString("Failed to import shader: ") + TCResultUtils::ResultToString( result ) );
		return result;
	}

	desc.shaderImportData = output;

	//
	// Initialize the shader now.
	//

	result = Success;
	switch( mType )
	{
		case kShaderTypeVertex:
			result = mShaderContext->CreateVertexShader( (void*)(&desc), this );
			break;

		case kShaderTypePixel:
			result = mShaderContext->CreatePixelShader( (void*)(&desc), this );
			break;

		default:
			TCLogger::GetInstance()->LogError(TCString("[TCShader] Failed to initialize shader: Unknown shader type provided."));
			return Failure_InvalidParameter;
	}

	//
	// Check the result of the operation.
	//

	if( TC_FAILED( result ) )
	{
		TCLogger::GetInstance()->LogError((TCString("[TCShader] Failed to initialize shader: ") + result ));
		return result;
	}

	//
	// Now we need to initialize all the sub components of the shader.
	//

	result = PopulateSubComponents( output );
	if( TC_FAILED( result ) )
	{
		TCLogger::GetInstance()->LogError( TCString("[TCShader] Failed to initialize shader sub components: ") + result );
		return result;
	}

	return Success;
}

//
// Release
//		- Release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- TCResult:
//			- Failure_InvalidState: The context has already been released
//

TCResult TCShader::Release()
{
	if( mShaderContext == NULL )
		return Failure_InvalidState;

	TCResult result = Success;
	switch( mType )
	{
		case kShaderTypeVertex:
			result = mShaderContext->ReleaseVertexShader( this );

		case kShaderTypePixel:
			result = mShaderContext->ReleasePixelShader( this );

		default: 
		{
			return Failure_InvalidState;	
		}
	}

	if( TC_FAILED( result ) )
	{
		return result;
	}
 
	return IGraphicsResource::Release();
}

//
// Bind
//		- Bind this shader to be used.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShader::Bind()
{
	if( mIsBound )
		return Success_Cached;

	TCResult result = Success;
	switch( mType )
	{
		case kShaderTypeVertex:
			result = mShaderContext->BindVertexShader( this );
			break;
		case kShaderTypePixel:
			result = mShaderContext->BindPixelShader( this );
			break;
		default: 
			return Failure_InvalidState;
	}

	if( TC_SUCCEEDED( result ) )
		mIsBound = true;

	return result;
}

//
// Unbind
//		- Unbind this shader to be used.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShader::Unbind()
{
	if( !mIsBound )
		return Failure_InvalidState;

	TCResult result = Success;
	switch( mType )
	{
		case kShaderTypeVertex:
			result = mShaderContext->UnbindVertexShader( this );
			break;

		case kShaderTypePixel:
			result = mShaderContext->UnbindPixelShader( this );
			break;

		default: 
			result = Failure_InvalidState;
	}

	mIsBound = false;

	return result;
}

//
// SetShaderUniform
//		- Set a uniform for a shader.
// Inputs:
//		- char* name: The name of the uniform.
//		- void* data: The data to set.
//		- unsigned int dataSize: The size of the data.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShader::SetShaderUniform( char* name, void* data, unsigned int dataSize )
{
	if( mShaderContext == NULL ||
		data == NULL ||
		name == NULL || 
		dataSize == 0 )
	{
		return Failure_InvalidParameter;
	}

	return mShaderContext->SetShaderUniform( name, data, dataSize );
}

//
// TypeEnumToString
//		- Will convert an enum to a string.
// Inputs:
//		- TCShader::Type type: The type to convert.
// Outputs:
//		- TCString: The string
//

TCString TCShader::TypeEnumToString( TCShader::Type type )
{
	switch( type )
	{
		case kShaderTypeVertex:			return TCString( "kShaderTypeVertex" );
		case kShaderTypePixel:			return TCString( "kShaderTypePixel" );
		case kShaderTypeGeometry:		return TCString( "kShaderTypeGeometry" );
		default:						return TCString( "kShaderTypeUnknown" );
	}
}

//
// TypeStringToEnum
//		- Will convert a string to an enum
// Inputs:
//		- TCString& string: The string to convert.
// Outputs:
//		- TCShader::Type: The type.
//

TCShader::Type TCShader::TypeStringToEnum( TCString& typeString )
{
	for( int currentType = (int)kShaderProfileUnknown + 1; currentType < (int)kNumShaderTypes; ++currentType )
	{
		if( typeString == TypeEnumToString( (TCShader::Type)currentType ) )
		{
			return (TCShader::Type)currentType;
		}
	}

	return kShaderTypeUnknown;
}

//
// ProfileEnumToString
//		- Will convert an enum to a string.
// Inputs:
//		- TCShader::Profile profile: The profile to convert.
// Outputs:
//		- TCString: The string
//

TCString TCShader::ProfileEnumToString( TCShader::Profile profile )
{
	switch( profile )
	{
		case kShaderProfile_compatability:		return TCString( "compatability" );
		case kShaderProfile_core:				return TCString( "core" );
		case kShaderProfile_vs_2_x:				return TCString( "vs_2_x" );
		case kShaderProfile_ps_2_x:				return TCString( "ps_2_x" );
		case kShaderProfile_cs_4_0:				return TCString( "cs_4_0" );
		case kShaderProfile_cs_4_1:				return TCString( "cs_4_1" );
		case kShaderProfile_cs_5_0:				return TCString( "cs_5_0" );
		case kShaderProfile_ds_5_0:				return TCString( "ds_5_0" );
		case kShaderProfile_gs_4_0:				return TCString( "gs_4_0" );
		case kShaderProfile_gs_4_1:				return TCString( "gs_4_1" );
		case kShaderProfile_gs_5_0:				return TCString( "gs_5_0" );
		case kShaderProfile_hs_5_0:				return TCString( "hs_5_0" );
		case kShaderProfile_lib_4_0:			return TCString( "lib_4_0" );
		case kShaderProfile_lib_4_0_level_9_0:	return TCString( "lib_4_0_level_9_0" );
		case kShaderProfile_lib_4_0_level_9_1:	return TCString( "lib_4_0_level_9_1" );
		case kShaderProfile_lib_4_0_level_9_3:	return TCString( "lib_4_0_level_9_3" );
		case kShaderProfile_lib_4_1:			return TCString( "lib_4_1" );
		case kShaderProfile_lib_5_0:			return TCString( "lib_5_0" );
		case kShaderProfile_ps_2_0:				return TCString( "ps_2_0" );
		case kShaderProfile_ps_3_0:				return TCString( "ps_3_0" );
		case kShaderProfile_ps_4_0:				return TCString( "ps_4_0" );
		case kShaderProfile_ps_4_0_level_9_0:	return TCString( "ps_4_0_level_9_0" );
		case kShaderProfile_ps_4_0_level_9_1:	return TCString( "ps_4_0_level_9_1" );
		case kShaderProfile_ps_4_0_level_9_3:	return TCString( "ps_4_0_level_9_3" );
		case kShaderProfile_ps_4_1:				return TCString( "ps_4_1" );
		case kShaderProfile_ps_5_0:				return TCString( "ps_5_0" );
		case kShaderProfile_vs_1_1:				return TCString( "vs_1_1" );
		case kShaderProfile_vs_2_0:				return TCString( "vs_2_0" );
		case kShaderProfile_vs_3_0:				return TCString( "vs_3_0" );
		case kShaderProfile_vs_4_0:				return TCString( "vs_4_0" );
		case kShaderProfile_vs_4_0_level_9_0:	return TCString( "vs_4_0_level_9_0" );
		case kShaderProfile_vs_4_0_level_9_1:	return TCString( "vs_4_0_level_9_1" );
		case kShaderProfile_vs_4_0_level_9_3:	return TCString( "vs_4_0_level_9_3" );
		case kShaderProfile_vs_4_1:				return TCString( "vs_4_1" );
		case kShaderProfile_vs_5_0:				return TCString( "vs_5_0" );
		default:								{TC_ASSERT( 0 && "Unknown Profile!" ); return TCString( "Unknown Profile!"); }
	}
}

//
// ProfileStringToEnum
//		- Will convert a string to an enum
// Inputs:
//		- TCString& string: The string to convert.
// Outputs:
//		- TCShader::Profile: The profile.
//

TCShader::Profile TCShader::ProfileStringToEnum( TCString& profileString )
{
	for( int currentProfile = (int)kShaderProfileUnknown + 1; currentProfile < (int)kNumShaderProfiles; ++currentProfile )
	{
		if( profileString == ProfileEnumToString( (TCShader::Profile)currentProfile ) )
		{
			return (TCShader::Profile)currentProfile;
		}
	}

	return kShaderProfileUnknown;
}

//
// ModelEnumToString
//		- Will convert a model enum to a string
// Inputs:
//		- TCShader::Model model: The model for the shader.
// Outputs:
//		- TCString: The string for the model.
//

TCString TCShader::ModelEnumToString( TCShader::Model model )
{
	switch( model )
	{
		case kShaderModel_GLSL_1_20:		return TCString( "GLSL_1_20" );
		case kShaderModel_GLSL_1_30:		return TCString( "GLSL_1_30" );
		case kShaderModel_GLSL_1_40:		return TCString( "GLSL_1_40" );
		case kShaderModel_GLSL_1_50:		return TCString( "GLSL_1_50" );
		case kShaderModel_HLSL_3:			return TCString( "HLSL_3" );
		case kShaderModel_HLSL_4:			return TCString( "HLSL_4" );
		case kShaderModel_HLSL_5:			return TCString( "HLSL_5" );
		default:							{ TC_ASSERT( 0 && "Unknown Model!" ); return TCString( "Unknown Model!" ); }
	}
}

//
// ModelStringToEnum
//		- Will convert a string to a model enum
// Inputs:
//		- TCString& modelString: The string to convert.
// Outputs:
//		- TCShader::Model: The model result..
//

TCShader::Model TCShader::ModelStringToEnum( TCString& modelString )
{
	for( int currentModel = 0; currentModel < (int)kNumShaderModel; ++currentModel )
	{
		if( ModelEnumToString( (TCShader::Model) currentModel ) == modelString )
		{
			return (TCShader::Model)currentModel;
		}
	}

	return kShaderModelUnknown;
}

//
// Clone
//		- Will set this instance to be a copy of another.
// Inputs:
//		- TCShader& toCopy: The shader to copy.
// Outputs:
//		- None.
//

void TCShader::Clone( const TCShader& toCopy )
{
	IGraphicsResource::Clone( toCopy );

	mShaderContext	= toCopy.mShaderContext;
	mIsBound		= toCopy.mIsBound;
	mType			= toCopy.mType;
	mProfile		= toCopy.mProfile;
	mModel			= toCopy.mModel;
	mName			= toCopy.mName;

	mConstantBuffers = toCopy.mConstantBuffers;
	mInputAttributes = toCopy.mInputAttributes;
	mUniforms		 = toCopy.mUniforms;
}

//
// Import
//		- Will read a file that represents the shader and return the information therein.
// Inputs:
//		- const TCString& filepath: The filepath where the shader file exists.
//		- void** output: The output from the import process.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShader::Import( const TCString& filepath, void** output )
{
	TCShaderImporter importer( mGraphicsContext->GetFileManager() );
	TCResult result = importer.Import( filepath, (TCShaderImporter::Output**)output );
	if( TC_FAILED( result ) )
	{
		TCLogger::GetInstance()->LogError( TCString( "[TCShader] Failed to import shader at location: ") + filepath + ", with result: " + TCResultUtils::ResultToString( result ) );
		TC_SAFE_DELETE( *output );
		return result;
	}

	return result;
}

//
// PopulateSubComponents
//		- Will populate all the subcomponents of this shader.
// Inputs:
//		- void* output: The output of the shader's import process.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShader::PopulateSubComponents( void* output )
{
	TCResult result = Success;

	//
	// Create our constant buffers
	//

	result = PopulateConstantBuffers( output );
	if( TC_FAILED( result ) )
	{
		return result;
	}

	//
	// Create our shader uniforms
	//

	result = PopulateShaderUniforms( output );
	if( TC_FAILED( result ) )
	{
		return result;
	}

	//
	// Create our shader attributes
	//

	result = PopulateShaderAttributes( output );
	if( TC_FAILED( result ) )
	{
		return result;
	}

	return result;
}

//
// PopulateConstantBuffer
//		- Create all our constant buffers 
// Inputs:
//		- void* output: The output of the shader import process.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShader::PopulateConstantBuffers( void* output )
{
	TCResult result = Success;

	//
	// Make sure the import data is valid.
	//

	TCShaderImporter::Output* importData = (TCShaderImporter::Output*)output;
	if( importData == NULL )
		return Failure_InvalidParameter;

	//
	// Next we need to create any constant buffers.
	//

	for( int currentConstantBuffer = 0; currentConstantBuffer < importData->mConstantBufferData.Count(); ++currentConstantBuffer )
	{
		TCConstantBuffer* constantBuffer = new TCConstantBuffer( mGraphicsContext );
		if( constantBuffer == NULL )
		{
			return Failure_OutOfMemory;
		}

		//
		// Initialize the constant buffer.
		//

		result = constantBuffer->Initialize( &importData->mConstantBufferData[ currentConstantBuffer ] );

		if( TC_FAILED( result ) )
		{
			TCLogger::GetInstance()->LogError( TCString( "[TCShader] Failed to create constant buffer: " ) + TCResultUtils::ResultToString( result ) );
			return result;
		}

		mConstantBuffers.Append( constantBuffer );
	}

	return result;
}

//
// PopulateShaderUniforms
//		- Will initialize all the shader uniforms that are dependent on this shader.
// Inputs:
//		- void* output: The output from the shader import.
// Outputs:
//		- TCResult result: The result of the operation.
//

TCResult TCShader::PopulateShaderUniforms( void* output )
{
	TCResult result = Success;

	//
	// Make sure the import data is valid.
	//

	TCShaderImporter::Output* importData = (TCShaderImporter::Output*)output;
	if( importData == NULL )
		return Failure_InvalidParameter;

	//
	// Now create all the shader uniforms.
	//

	for( int currentUniform = 0; currentUniform < importData->mUniformData.Count(); ++currentUniform )
	{
		TCShaderUniform* uniform = new TCShaderUniform( mGraphicsContext );
		if( uniform == NULL )
		{
			return Failure_OutOfMemory;
		}

		//
		// Initialize the uniform.
		//

		result = uniform->Initialize( &importData->mUniformData[ currentUniform ] );
		if( TC_FAILED( result ) )
		{
			TCLogger::GetInstance()->LogError( TCString( "[TCShader] Failed to initialize shader uniform: ") + TCResultUtils::ResultToString( result ) );
			return result;
		}

		mUniforms.Append( uniform );
	}

	return Success;
}

//
// PopulateShaderAttributes
//		- Will initialize all dependent shader attributes
// Inputs:
//		- void* output: The output from the shader import process.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShader::PopulateShaderAttributes( void* output )
{
	TCResult result = Success;

	//
	// Make sure the import data is valid.
	//

	TCShaderImporter::Output* importData = (TCShaderImporter::Output*)output;
	if( importData == NULL )
		return Failure_InvalidParameter;

	//
	// Now we need to create our shader attributes.
	//

	for( int currentShaderAttribute = 0; currentShaderAttribute < importData->mAttributeData.Count(); ++currentShaderAttribute )
	{
		TCShaderAttribute* attribute = new TCShaderAttribute( mGraphicsContext );
		if( attribute == NULL )
		{
			return Failure_OutOfMemory;
		}

		//
		// Initialize the attribute
		//

		result = attribute->Initialize( &importData->mAttributeData[ currentShaderAttribute ] );
		if( TC_FAILED( result ) )
		{
			TCLogger::GetInstance()->LogError( TCString("[TCShader] Failed to initialize shader attribute: ") + TCResultUtils::ResultToString( result ) );
			return result;
		}

		mInputAttributes.Append( attribute );
	}

	return Success;
}