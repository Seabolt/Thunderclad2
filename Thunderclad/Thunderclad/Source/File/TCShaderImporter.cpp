//
// TCShaderImporter.cpp
// THis file will define how to import a shader.
//

//
// Includes
//

#include "TCShaderImporter.h"
#include "TCLogger.h"
#include "TCFileManager.h"
#include "TCFile.h"
#include "yaml.h"

//
// Defines
//

#define kEntryPointNodeId					("EntryPoint")
#define kProfileNodeId						("Profile")
#define kModelNodeId						("Model")
#define kUniformNodeId						("Uniforms")
#define kUniformNameNodeId					("name")
#define kUniformTypeNodeId					("type")
#define kUniformGlobalRegister				("globalRegister")
#define kUniformDefaultValueNodeId			("defaultValue")
#define kUniformConstantBufferNameNodeId	("constantBuffer")
#define kUniformConstantBufferOffsetNodeId	("constantBufferOffset")
#define kUniformUsageNodeId					("usage")
#define kSamplersNodeId						("Samplers")
#define kConstantBuffersNodeId				("ConstantBuffers")
#define kConstantBufferNameNodeId			("name")
#define kConstantBufferRegisterNodeId		("register")
#define kConstantBufferSizeNodeId			("size")
#define kConstantBufferShaderType			("shaderType")
#define kAttributesNodeId					("Attributes")
#define kAttributeNameNodeId				("name")
#define kAttributeTypeNodeId				("type")
#define kAttributeTypeUsageId				("usage")
#define kRawShaderTextId					("ShaderText")
#define kShaderPropertiesId					("ShaderProperties")

#define TC_SHADER_IMPORTER_LOG_ERROR( error )	(TCLogger::GetInstance()->LogError( TCString( "[TCShaderImporter" ) + error ))

//
// TCShaderImporter::Output::Output()
//		- This will initialize the output to a safe state.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCShaderImporter::Output::Output()
{
	mProfile = TCShader::kShaderProfileUnknown;
	mModel = TCShader::kShaderModelUnknown;
}

//
// TCShaderImporter::TCShaderImporter()
//		- This constructor will initialize this importer to a safe state.
// Inputs:
//		- TCFileManager* fileManager: The file manager for this instance.
// Outputs:
//		- None.
//

TCShaderImporter::TCShaderImporter( TCFileManager* fileManager )
{
	mFileManager	= fileManager;
	mShaderFile		= NULL;
}

//
// TCShaderImporter:: Copy Constructor
//		- This constructor will set this instance equal to another.
// Inputs:
//		- const TCShaderImporter& toCopy: The shader importer to copy.
// Outputs:
//		- None.
//

TCShaderImporter::TCShaderImporter( const TCShaderImporter& toCopy )
{
	Clone( toCopy );
}

//
// TCShaderImporter:: Destructor
//		- This will release all resources associated with this importer.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCShaderImporter::~TCShaderImporter()
{
	Release();
}

//
// TCShaderImporter::Initialize
//		- Will import the shader specified.
// Inputs:
//		- TCString filepath: The filepath for the shader to import.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderImporter::Import( TCString filepath, TCShaderImporter::Output** output )
{
	//
	// First we need to see if we have a file manager that is valid.
	//

	if( mFileManager == NULL )
	{
		return Failure_InvalidState;
	}

	//
	// Make sure our output is valid.
	//

	*output = new Output();

	//
	// Create our shader name.
	//

	mCurrentName = GetShaderName( filepath );

	//
	// Next we need to verify that the file exists.
	//

	if( !mFileManager->FileExists( filepath ) )
	{
		return Failure_FileNotFound;
	}

	//
	// Start processing the file.
	//

	TCResult result = ParseYAMLFile( filepath, *output );
	if( TC_FAILED( result ) )
	{
		return result;
	}

	return Success;
}

//
// TCShaderImporter::Release
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderImporter::Release()
{
	TCResult result = Success;
	if( mFileManager != NULL && mShaderFile != NULL )
	{
		result = mFileManager->CloseFile( mShaderFile );
		if( TC_FAILED( result ) )
		{
			return result;
		}
	}

	mShaderFile = NULL;
	return Success;
}

//
// TCShaderImporter::Clone
//		- This will set this instance to be equal to another.
// Inputs:
//		- const TCShaderImporter& toCopy: The instance to copy.
// Outputs:
//		- None.
//

void TCShaderImporter::Clone( const TCShaderImporter& toCopy )
{
	mFileManager = toCopy.mFileManager;
	mShaderFile = toCopy.mShaderFile;
	mCurrentName = toCopy.mCurrentName;
}

//
// GetShaderName
//		- This will determine the shader name from the filepath.
// Inputs:
//		- TCString& filepath: The filepath of the shader.
// Outputs:
//		- TCString name: The name of the shader
//

TCString TCShaderImporter::GetShaderName( TCString& filepath )
{
	TCString name;
	int lastIndexOf = filepath.FindLastIndexOf( '\\' ) + 1;
	if( lastIndexOf > 0 )
	{
		filepath.Substring( lastIndexOf, name );
	}

	return name;
}

//
// ParseYAMLFile
//		- This is the entry point for the YAML processor.
// Inputs:
//		- TCString& filepath: The path to the file.
//		- TCShaderImporter::Output* output: The output.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderImporter::ParseYAMLFile( TCString& filepath, TCShaderImporter::Output* output )
{
	//
	// First we need to see if we can open up the file.
	//

	std::string filepathString = filepath.Data();
	try
	{
		YAML::Node root = YAML::LoadFile( filepathString );

		if( root.IsNull() )
		{
			return Failure_FileNotFound;
		}

		//
		// Next we want to parse out the shader properties.
		//

		YAML::Node shaderProperties = root[ kShaderPropertiesId ];
		if( shaderProperties.IsNull() )
		{
			return Failure_MalformedData;
		}

		ParseShaderProperties( shaderProperties, output );

		//
		// Now we need to get the raw shader text.
		//

		YAML::Node shaderText = root[ kRawShaderTextId ];
		if( shaderText.IsNull() )
		{
			return Failure_MalformedData;
		}

		ParseShaderText( shaderText, output );

	}
	catch( YAML::Exception exception )
	{
		TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to load file: " ) + filepath + TCString( ", parser threw: " ) + exception.msg.c_str() );
		return Failure_Unknown;
	}

	//
	// Success!
	//

	return Success;
}

//
// ParseShaderProperties
//		- Will parse all the shader properties in the file.
// Inputs:
//		- YAML::Node& node: The node read from the shader file.
//		- Output* output: The output of the import process.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderImporter::ParseShaderProperties( YAML::Node node, TCShaderImporter::Output* output )
{
	if( node.IsNull() )
		return Failure_InvalidParameter;

	//
	// Read the entry point information.
	//

	YAML::Node entryPointNode = node[ kEntryPointNodeId ];
	if( entryPointNode.IsNull() || !entryPointNode.IsDefined() )
	{
		TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to parse shader: ") + mCurrentName + TCString( ", no entry point was found" ) );
		return Failure_MalformedData;
	}
	output->mEntryPointName = entryPointNode.as<std::string>().c_str();

	//
	// Read the profile information.
	//

	YAML::Node profileNode = node[ kProfileNodeId ];
	if( profileNode.IsNull() || !profileNode.IsScalar() )
	{
		TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to parse shader: ") + mCurrentName + TCString( ", no profile was found" ) );
		return Failure_MalformedData;
	}
	output->mProfile = TCShader::ProfileStringToEnum( TCString(node[ kProfileNodeId ].Scalar().c_str()) );
	if( output->mProfile == TCShader::kShaderProfileUnknown )
	{
		TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to parse shader: ") + mCurrentName + TCString( ", invalid profile was found." ) );
		return Failure_InvalidParameter;
	}

	//
	// Read the model information
	//

	YAML::Node modelNode = node[ kModelNodeId ];
	if( modelNode.IsNull() || !modelNode.IsScalar() )
	{
		TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to parse shader: ") + mCurrentName + TCString( ", no model was found." ) );
		return Failure_MalformedData;
	}
	output->mModel = TCShader::ModelStringToEnum( TCString(node[ kModelNodeId ].Scalar().c_str()) );
	if( output->mModel == TCShader::kShaderModelUnknown )
	{
		TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to parse shader: ") + mCurrentName + TCString( ", invalid model was found." ) );
		return Failure_InvalidParameter;
	}

	//
	// Read the constant buffer information.
	//

	YAML::Node constantBufferNode = node[ kConstantBuffersNodeId ];
	TCResult result = ParseShaderConstantBuffers( constantBufferNode, output );
	if( TC_FAILED( result ) )
	{
		TCLogger::GetInstance()->LogError( TCString( "[TCShaderImporter] Failed to parse shader constant buffers: " ) + mCurrentName + TCResultUtils::ResultToString( result ) );
		return result;
	}

	//
	// Read the uniform information.
	//

	YAML::Node uniformsNode = node[ kUniformNodeId ];
	
	result = ParseShaderUniforms( uniformsNode, output );
	if( TC_FAILED( result ) )
	{
		TCLogger::GetInstance()->LogError( TCString( "[TCShaderImporter] Failed to parse shader uniforms: " ) + mCurrentName + TCResultUtils::ResultToString( result ) );
		return result;
	}

	//
	// Read the shader attribute information.
	//

	YAML::Node attributeNode = node[ kAttributesNodeId ];
	result = ParseShaderAttributes( attributeNode, output );
	if( TC_FAILED( result ) )
	{
		TCLogger::GetInstance()->LogError( TCString( "[TCShaderImporter] Failed to parse shader attribute: " ) + mCurrentName + TCResultUtils::ResultToString( result ) );
		return result;
	}

	//
	// Read the shader samplers information
	//

	YAML::Node samplersNode = node[ kSamplersNodeId ];
	result = ParseShaderSamplers( samplersNode, output );
	if( TC_FAILED( result ) )
	{
		TCLogger::GetInstance()->LogError( TCString( "[TCShaderImporter] Failed to parse shader samplers: " ) + mCurrentName + TCResultUtils::ResultToString( result ) );
		return result;
	}


	return Success;
}

//
// ParseShaderUniforms
//		- Will parse all the shader uniforms in the text file.
// Inputs:
//		- YAML::Node& node: The parent node to read from the file.
//		- Output* output: The output of the import process.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderImporter::ParseShaderUniforms( YAML::Node& node, Output* output )
{
	//
	// Make sure the node is valid.
	//

	if( node.IsNull() || !node.IsDefined() )
	{
		return Success_Handled;
	}

	if( !node.IsSequence() )
	{
		TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to parse shader: ") + mCurrentName + TCString(", uniforms node was not formatted correctly."));
		return Failure_MalformedData;
	}

	//
	// Walk all the sub nodes.
	//

	std::size_t numUniforms = node.size();
	output->mUniformData.Resize( (int)numUniforms );
	for( std::size_t currentUniform = 0; currentUniform < node.size(); ++currentUniform )
	{
		YAML::Node uniformNode = node[ currentUniform ];

		//
		// Get the uniform name.
		//

		if( !uniformNode[ kUniformNameNodeId ] )
		{
			TC_SHADER_IMPORTER_LOG_ERROR( TCString("Failed to import uniform for shader: ") + mCurrentName + ", name node is missing." );
			return Failure_MalformedData;
		}
		else
		{
			output->mUniformData[ currentUniform ].name = uniformNode[ kUniformNameNodeId ].Scalar().c_str();
		}

		//
		// Get the uniform type.
		//

		if( !uniformNode[ kUniformTypeNodeId ] )
		{
			TC_SHADER_IMPORTER_LOG_ERROR( TCString("Failed to import uniform for shader: ") + mCurrentName + ", type node is missing." );
			return Failure_MalformedData;
		}
		else
		{
			output->mUniformData[ currentUniform ].type = TCShaderUniform::StringToType( TCString(uniformNode[ kUniformTypeNodeId ].Scalar().c_str()));
		}
		
		//
		// Get the uniform register.
		//

		if( uniformNode[ kUniformGlobalRegister ] )
		{
			output->mUniformData[ currentUniform ].globalRegister = uniformNode[ kUniformGlobalRegister ].as<int>();
		}
		else
		{
			output->mUniformData[ currentUniform ].globalRegister = -1;
		}

		//
		// Get the uniform default value.
		//

		if( !uniformNode[ kUniformDefaultValueNodeId ] )
		{
			TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to import shader: " ) + mCurrentName + TCString( ", default value wasn't found for a uniform!" ) );
			return Failure_MalformedData;
		}
		else
		{
			TCResult result = ParseShaderUniformDefaultValue( uniformNode[ kUniformDefaultValueNodeId ], &output->mUniformData[ currentUniform ], output->mUniformData[ currentUniform ].type );
			if( TC_FAILED( result ) )
			{
				return result;
			}
		}

		//
		// Get the uniform usage.
		//

		if( !uniformNode[ kUniformUsageNodeId ] )
		{
			TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to import shader: " ) + mCurrentName + TCString( "Uniform: " ) + output->mUniformData[ currentUniform ].name + TCString( " doesn't have a usage attribute!" ) );
		}
		else
		{
			output->mUniformData[ currentUniform ].usage = TCShaderUniform::StringToUsage( TCString( uniformNode[ kUniformUsageNodeId ].Scalar().c_str() ) );
		}

		//
		// Get the uniform constant buffer parent.
		//

		if( uniformNode[ kUniformConstantBufferNameNodeId ] )
		{
			output->mUniformData[ currentUniform ].constantBufferName = TCString( uniformNode[ kUniformConstantBufferNameNodeId ].Scalar().c_str() );
		}

		//
		// Get the uniform constant buffer offset.
		//

		if( uniformNode[ kUniformConstantBufferOffsetNodeId ] )
		{
			output->mUniformData[ currentUniform ].constantBufferOffset = TCStringUtils::AtoI( TCString( uniformNode[ kUniformConstantBufferOffsetNodeId ].Scalar().c_str() ) );
		}

	}

	return Success;
}

//
// ReadBool
//		- Will read boolean value from a uniform string.
// Inputs:
//		- bool* output: The array of bools in the uniform.
//		- TCString& uniformString: The string representing the uniform.
//		- TCString& uniformName: The name of the uniform.
//		- unsigned int numElements: The number of elements that should be in the string.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderImporter::ReadBool( bool*& output, TCString& uniformString, TCString& uniformName, unsigned int numElements )
{
	TCList< TCString > boolStrings;
	uniformString.Split( ',', boolStrings );

	if( boolStrings.Count() != numElements )
	{
		TC_SHADER_IMPORTER_LOG_ERROR( TCString("Failed to import shader: ") + mCurrentName + TCString(", uniform: ") + uniformName + TCString(", uniform default value element count was mismatched.") );
		return Failure_MalformedData;
	}

	for( int currentBool = 0; currentBool < boolStrings.Count(); ++currentBool )
	{
		if( boolStrings[ currentBool ] == "true" )
		{
			output[ currentBool ] = true;
		}
		else if( boolStrings[ currentBool ] == "false" )
		{
			output[ currentBool ] = false;
		}
		else
		{
			TC_SHADER_IMPORTER_LOG_ERROR( TCString("Failed to import shader: ") + mCurrentName + TCString(", uniform: ") + uniformName + TCString(", uniform default value element count was mismatched.") );	
			return Failure_MalformedData;
		}
	}
	return Success;
}

//
// ReadFloat
//		- Will read float value from a uniform string.
// Inputs:
//		- float* output: The array of floats in the uniform.
//		- TCString& uniformString: The string representing the uniform.
//		- TCString& uniformName: The name of the uniform.
//		- unsigned int numElements: The number of elements that should be in the string.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderImporter::ReadFloat( float*& output, TCString& uniformString, TCString& uniformName, unsigned int numElements )
{
	TCList< TCString > floatStrings;
	uniformString.Split( ',', floatStrings );

	if( floatStrings.Count() != numElements )
	{
		TC_SHADER_IMPORTER_LOG_ERROR( TCString("Failed to import shader: ") + mCurrentName + TCString(", uniform: ") + uniformName + TCString(", uniform default value element count was mismatched.") );
		return Failure_MalformedData;
	}

	for( int currentFloat = 0; currentFloat < floatStrings.Count(); ++currentFloat )
	{
		output[ currentFloat ] = TCStringUtils::AtoF( floatStrings[ currentFloat ] );
	}
	return Success;
}

//
// ReadHalf
//		- Will read half value from a uniform string.
// Inputs:
//		- float* output: The array of floats in the uniform.
//		- TCString& uniformString: The string representing the uniform.
//		- TCString& uniformName: The name of the uniform.
//		- unsigned int numElements: The number of elements that should be in the string.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderImporter::ReadHalf( float*& output, TCString& uniformString, TCString& uniformName, unsigned int numElements )
{
	TCList< TCString > floatStrings;
	uniformString.Split( ',', floatStrings );

	if( floatStrings.Count() != numElements )
	{
		TC_SHADER_IMPORTER_LOG_ERROR( TCString("Failed to import shader: ") + mCurrentName + TCString(", uniform: ") + uniformName + TCString(", uniform default value element count was mismatched.") );
		return Failure_MalformedData;
	}

	for( int currentFloat = 0; currentFloat < floatStrings.Count(); ++currentFloat )
	{
		output[ currentFloat ] = TCStringUtils::AtoF( floatStrings[ currentFloat ] );
	}
	return Success;
}

//
// ReadInt
//		- Will read int value from a uniform string.
// Inputs:
//		- int* output: The array of ints in the uniform.
//		- TCString& uniformString: The string representing the uniform.
//		- TCString& uniformName: The name of the uniform.
//		- unsigned int numElements: The number of elements that should be in the string.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderImporter::ReadInt( int*& output, TCString& uniformString, TCString& uniformName, unsigned int numElements )
{
	TCList< TCString > intStrings;
	uniformString.Split( ',', intStrings );

	if( intStrings.Count() != numElements )
	{
		TC_SHADER_IMPORTER_LOG_ERROR( TCString("Failed to import shader: ") + mCurrentName + TCString(", uniform: ") + uniformName + TCString(", uniform default value element count was mismatched.") );
		return Failure_MalformedData;
	}

	for( int currentFloat = 0; currentFloat < intStrings.Count(); ++currentFloat )
	{
		output[ currentFloat ] = TCStringUtils::AtoI( intStrings[ currentFloat ] );
	}
	return Success;
}

//
// ParseShaderUniformDefaultValue
//		- Will parse the default value of a uniform.
// Inputs:
//		- YAML::Node& node: The parent node to read from the file.
//		- TCShaderUniform::Description& description: The uniform description to store the value in.
//		- TCShaderUniform::Type type: The type of the uniform.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderImporter::ParseShaderUniformDefaultValue( YAML::Node& node, TCShaderUniform::Description* description, TCShaderUniform::Type type )
{
	if( node.IsNull() || !node.IsScalar() )
	{
		TC_SHADER_IMPORTER_LOG_ERROR( TCString("Failed to import shader: ") + mCurrentName + TCString(", default value was malformed for uniform: ") + description->name );
		return Failure_MalformedData;
	}

	unsigned int dataSize = TCShaderUniform::GetDataSize( type );
	unsigned char* defaultValueData = new unsigned char[ dataSize ];
	TCString defaultValueString = node.Scalar().c_str();

	switch( type )
	{
		case TCShaderUniform::kShaderUniformTypeBool:
		case TCShaderUniform::kShaderUniformTypeBool2:
		case TCShaderUniform::kShaderUniformTypeBool3:
		case TCShaderUniform::kShaderUniformTypeBool4:
		{
			ReadBool( (bool*&)defaultValueData, defaultValueString, description->name, TCShaderUniform::GetDataSize( type ) / sizeof( bool ) );
			break;
		}
		case TCShaderUniform::kShaderUniformTypeFloat:
		case TCShaderUniform::kShaderUniformTypeFloat2:
		case TCShaderUniform::kShaderUniformTypeFloat3:
		case TCShaderUniform::kShaderUniformTypeFloat4:
		{
			ReadFloat( (float*&)defaultValueData, defaultValueString, description->name, TCShaderUniform::GetDataSize( type ) / sizeof( float ) );
			break;
		}
		case TCShaderUniform::kShaderUniformTypeHalf:
		case TCShaderUniform::kShaderUniformTypeHalf2:
		case TCShaderUniform::kShaderUniformTypeHalf3:
		case TCShaderUniform::kShaderUniformTypeHalf4:
		{
			ReadHalf( (float*&)defaultValueData, defaultValueString, description->name, TCShaderUniform::GetDataSize( type ) / sizeof( float ) );
			break;
		}
		case TCShaderUniform::kShaderUniformTypeInt:
		case TCShaderUniform::kShaderUniformTypeInt2:
		case TCShaderUniform::kShaderUniformTypeInt3:
		case TCShaderUniform::kShaderUniformTypeInt4:
		{
			ReadInt( (int*&)defaultValueData, defaultValueString, description->name, TCShaderUniform::GetDataSize( type ) / sizeof( int ) );
			break;
		}
		case TCShaderUniform::kShaderUniformTypeMatrix3x3:
		{
			ReadFloat( (float*&)defaultValueData, defaultValueString, description->name, TCShaderUniform::GetDataSize( type ) / sizeof( float ) );
			break;
		}
		case TCShaderUniform::kShaderUniformTypeMatrix4x4:
		{
			ReadFloat( (float*&)defaultValueData, defaultValueString, description->name, TCShaderUniform::GetDataSize( type ) / sizeof( float ) );
			break;
		}
		default:
		{
			delete defaultValueData;
			return Failure_InvalidParameter;
		}
	}

	description->defaultValue = defaultValueData;
	return Success;
}

//
// ParseShaderConstantBuffers
//		- Will parse all the shader constant buffers in the text file.
// Inputs:
//		- YAML::Node& node: The parent node to read from the file.
//		- Output* output: The output of the import process.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderImporter::ParseShaderConstantBuffers( YAML::Node& node, Output* output )
{
	if( node.IsNull() || !node.IsDefined() )
	{
		return Success_Handled;
	}

	if( !node.IsSequence() )
	{
		TC_SHADER_IMPORTER_LOG_ERROR( TCString("Failed to import shader: ") + mCurrentName + TCString(", constant buffer node is malformed" ) );
		return Failure_MalformedData;
	}

	//
	// Walk all the sub nodes.
	//

	std::size_t numConstantBuffers = node.size();
	output->mConstantBufferData.Resize( numConstantBuffers );
	for( unsigned int currentConstantBuffer = 0; currentConstantBuffer < node.size(); ++currentConstantBuffer )
	{
		YAML::Node constantBufferNode = node[ currentConstantBuffer ];

		//
		// Get the uniform name.
		//

		if( !constantBufferNode[ kConstantBufferNameNodeId ] )
		{
			TC_SHADER_IMPORTER_LOG_ERROR( TCString("Failed to import constant buffer for shader: ") + mCurrentName + ", name node is missing." );
			return Failure_MalformedData;
		}
		else
		{
			output->mConstantBufferData[ currentConstantBuffer ].name = constantBufferNode[ kConstantBufferNameNodeId ].Scalar().c_str();
		}

		//
		// Read the constant buffer's register.
		//

		if( !constantBufferNode[ kConstantBufferRegisterNodeId ] )
		{
			TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to import shader: " ) + mCurrentName + TCString( ", a constant buffer is missing it's buffer register!" ) );
			return Failure_MalformedData;
		}
		else
		{
			output->mConstantBufferData[ currentConstantBuffer ].bufferRegister = TCStringUtils::AtoI( TCString( constantBufferNode[ kConstantBufferRegisterNodeId ].Scalar().c_str() ) );
		}

		//
		// Read the constant buffer's size.
		//

		if( constantBufferNode[ kConstantBufferSizeNodeId ] )
		{
			output->mConstantBufferData[ currentConstantBuffer ].bufferSize = TCStringUtils::AtoI( TCString( constantBufferNode[ kConstantBufferNameNodeId ].Scalar().c_str() ) );
		}
		else
		{
			TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to import shader: " ) + mCurrentName + TCString( ", a constant buffer is missing it's buffer size!" ) );
			return Failure_MalformedData;
		}

		//
		// Set the constant buffer's type.
		//

		if( constantBufferNode[ kConstantBufferShaderType ] )
		{
			output->mConstantBufferData[ currentConstantBuffer ].shaderType = (unsigned int)TCShader::TypeStringToEnum( TCString( constantBufferNode[ kConstantBufferShaderType ].Scalar().c_str() ) );	
		}
		else
		{
			TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to import shader: " ) + mCurrentName + TCString( ", a constant buffer is missing it's shader type!" ) );
			return Failure_MalformedData;
		}
	}
	return Success;
}

//
// ParseShaderAttributes
//		- Will parse shader attributes in the text file.
// Inputs:
//		- YAML::Node& node: The parent node to read from the file.
//		- Output* output: The output of the import process.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderImporter::ParseShaderAttributes( YAML::Node& node, Output* output )
{
	if( node.IsNull() || !node.IsDefined() )
	{
		return Success_Handled;
	}

	if( !node.IsSequence() )
	{
		TC_SHADER_IMPORTER_LOG_ERROR( TCString("Failed to import shader: ") + mCurrentName + TCString(", attribute node is malformed" ) );
		return Failure_MalformedData;
	}

	//
	// Lets figure out how many shader attributes there are.
	//

	std::size_t numShaderAttributes = node.size();
	output->mAttributeData.Resize( numShaderAttributes );
	for( unsigned int currentShaderAttribute = 0; currentShaderAttribute < numShaderAttributes; ++currentShaderAttribute )
	{
		YAML::Node attributeNode = node[ currentShaderAttribute ];
		
		if( attributeNode[ kAttributeNameNodeId ] )
		{
			output->mAttributeData[ currentShaderAttribute ].name = attributeNode[ kAttributeNameNodeId ].Scalar().c_str();
		}
		else
		{
			TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to import shader: ") + mCurrentName + TCString(", attribute doesn't have a name!" ) );
			return Failure_MalformedData;
		}

		if( attributeNode[ kAttributeTypeNodeId ] )
		{
			output->mAttributeData[ currentShaderAttribute ].type = TCShaderAttribute::StringToType( TCString( attributeNode[ kAttributeNameNodeId ].Scalar().c_str() ) );
		}
		else
		{
			TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to import shader: " ) + mCurrentName + TCString(", attribute doesn't have a type!" ) );
		}

		if( attributeNode[ kAttributeTypeUsageId ] )
		{
			output->mAttributeData[ currentShaderAttribute ].usage = TCShaderAttribute::StringToUsage( TCString( attributeNode[ kAttributeTypeUsageId ].Scalar().c_str() ) );
		}
		else
		{
			TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to import shader: " ) + mCurrentName + TCString(", attribute doesn't have a usage!" ) );
		}
	}
	return Success;
}

//
// ParseShaderSamplers
//		- Will parse shader samplers in the text file.
// Inputs:
//		- YAML::Node& node: The parent node to read from the file.
//		- Output* output: The output of the import process.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderImporter::ParseShaderSamplers( YAML::Node& node, Output* output )
{
	return Success;
}

//
// ParseShaderText
//		- Will parse all the shader text in the file.
// Inputs:
//		- YAML::Node& node: The parent node read from teh file.
//		- Output* output: The output of the import process.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCShaderImporter::ParseShaderText( YAML::Node& node, TCShaderImporter::Output* output )
{
	if( node.IsNull() )
	{
		TC_SHADER_IMPORTER_LOG_ERROR( TCString( "Failed to import shader: " ) + mCurrentName + TCString( ", no shader text was found!" ) );
		return Failure_MalformedData;
	}

	output->mShaderText = node.Scalar().c_str();
	return Success;
}