////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TCShaderContext_DX11 -- Implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Includes
//

#include "TCGraphicsContext_DX11.h"
#include "TCShader.h"
#include "TCShaderProgram.h"
#include "TCShaderUniform.h"
#include "TCShaderAttribute.h"
#include "TCConstantBuffer.h"
#include "TCShaderImporter.h"
#include "TCFileManager.h"
#include "TCFile.h"
#include "TCLogger.h"

#include <D3DCompiler.h>
//
// Defines
//

#define TC_SAFE_RELEASE( resource )					{ if( resource != NULL ) { resource->Release(); resource = NULL; } }

//
// TCShaderContext_DX11
//		- Will create a TCShaderContext_DX11.
// Inputs:
//		- TCGraphicsContext_DX11* context: The graphics context to use.
// Outputs:
//		- None.
//

TCGraphicsContext_DX11::TCShaderContext_DX11::TCShaderContext_DX11( TCGraphicsContext_DX11* context )
	: TCGraphicsContext::TCShaderContext( context )
{
	mDX11Context = context;
	mContext = (TCGraphicsContext*)context;

	for( int currentShader = 0; currentShader < TC_MAX_BOUND_SHADERS; ++currentShader )
	{
		mBoundShaders[ currentShader ] = NULL;
	}

	for( int currentConstantBuffer = 0; currentConstantBuffer < TC_MAX_BOUND_CONSTANT_BUFFERS; ++currentConstantBuffer )
	{
		mBoundConstantBuffers[ currentConstantBuffer ] = NULL;
	}
}

//
// TCShaderContext_DX11::ShaderIncludeFinder::ShaderIncludeFinder()
//		- Will initialize this finder to a safe state.
// Inputs:
//		- TCString systemDirectory: The absolute path to the resource directory.
//		- TCString shaderDirectory: The absolute path to the shader's current directory.
//		- TCFileManager* fileManager: The filemanager to open/close files.
// Outputs:
//		- None.
//

TCGraphicsContext_DX11::TCShaderContext_DX11::ShaderIncludeFinder::ShaderIncludeFinder( TCString systemDirectory,
																						TCString shaderDirectory,
																						TCFileManager* fileManager )
{
	mSystemDirectory = systemDirectory;
	mShaderDirectory = shaderDirectory;
	mFileManager = fileManager;
}

//
// TCShaderContext_DX11::ShaderIncludeFinder::Open
//		- Will search and open up any files that are included by other shaders.
// Inputs:
//		- D3D_INCLUDE_TYPE includeType: The type of include.
//		- LPCSTR pFileName: The path to the file.
//		- LPCVOID pParentData: The pointer to the container that includes the #include file. But this can be NULL and we should just search our own directories.
//		- LPCVOID* ppData: The buffer that contains the text from the include file.
//		- UINT*	pBytes: THe buffer that contains the size of the text from teh include file.
// Outputs:
//		- HRESULT: The result of the operation.
//

HRESULT TCGraphicsContext_DX11::TCShaderContext_DX11::ShaderIncludeFinder::Open( D3D_INCLUDE_TYPE includeType,
																				 LPCSTR pFileName,
																				 LPCVOID pParentData,
																				 LPCVOID* ppData,
																				 UINT* pBytes )
{
	//
	// Determine the filepath location.
	//

	TCString filepath;
	switch( includeType )
	{
		case D3D_INCLUDE_LOCAL:
			filepath = mShaderDirectory + pFileName;
			break;

		case D3D_INCLUDE_SYSTEM:
			filepath = mSystemDirectory + pFileName;
			break;

		default:
			TCLogger::GetInstance()->LogError( TCString( "[TCShader] Failed to include file: " ) + TCString( pFileName ) + TCString( ", unsupported include type." ) );
			return ERROR_FILE_NOT_FOUND;
	}

	//
	// Open the file!
	//

	TCFile* includeFile = NULL;
	TCResult result = mFileManager->OpenFile( filepath, &includeFile, TCFileManager::Access_ReadOnly );
	if( TC_FAILED( result ) || includeFile == NULL)
	{
		TCLogger::GetInstance()->LogError( TCString( "[TCShader] Failed to include file: " ) + TCString( pFileName ) + TCString( ", failed to open include file." ) );
		return ERROR_FILE_NOT_FOUND;
	}

	//
	// Read the text.
	//

	TCString includeText;
	result = includeFile->ReadFile( includeText );
	if( TC_FAILED( result ) || includeText.IsEmpty() )
	{
		TCLogger::GetInstance()->LogError( TCString( "[TCShader] Failed to include file: " ) + TCString( pFileName ) + TCString( ", failed to read from include file." ) );
		return ERROR_FILE_CORRUPT;
	}

	//
	// Store the text/size in teh output buffers.
	//

	char* includeTextOutput = new char[ includeText.Length() ];
	TCStringUtils::Copy( includeTextOutput, includeText.Data() );

	*ppData = includeTextOutput;
	*pBytes = includeText.Length();

	return S_OK;
}

//
// TCShaderContext_DX11::ShaderIncludeFinder::Close()
//		- Will close any opened files that are included by other shaders.
// Inputs:
//		- LPCVOID pData: The buffer that contains the text from the include file.
// Outputs:
//		- HRESULT: THe result of the operation.
//

HRESULT TCGraphicsContext_DX11::TCShaderContext_DX11::ShaderIncludeFinder::Close( LPCVOID pData )
{
	TC_SAFE_DELETE_ARRAY( pData );
	return S_OK;
}

//
// CreateVertexShader
//		- Will initialize the resources for a vertex shader.
// Inputs:
//		- void* description: The description for the vertex shader.
//		- TCShader* shaderToCreate: The shader to store the resources in.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::CreateVertexShader( void* description, TCShader* shaderToCreate )
{
	TCShader::Description* shaderDesc = (TCShader::Description*)description;
	TCFileManager* fileManager = mContext->GetFileManager();

	//
	// Verify that this a unique instance.
	//

	TCShader* shader = GetShader( shaderDesc->shaderName );
	if( shader != NULL )
	{
		TCResult result = AddReference( *shaderToCreate, shader->GetGraphicsResources() );
		if( TC_FAILED( result ) )
		{
			TCLogger::GetInstance()->LogError( TCString( "[TCShader] Failed to add a reference to the shader: ") + TCResultUtils::ResultToString( result ) );
			return result;
		}
		return Success_Cached;
	}
	
	//
	// Now we need to create the platform shader data.
	//

	TCVertexShaderPlatformData* platformData = new TCVertexShaderPlatformData( mContext );

	//
	// Next we need to compile the shader.
	//

	ID3D10Blob* shaderBlob = NULL;
	TCResult result = CompileShader( &shaderBlob, shaderDesc->shaderImportData, description );
	if( TC_FAILED( result ) )
	{
		return result;
	}

	//
	// Now we need to create the specific shader.
	//

	HRESULT hResult = mDX11Context->mDevice->CreateVertexShader( shaderBlob->GetBufferPointer(),
																 shaderBlob->GetBufferSize(),
																 NULL,
																 &platformData->mVertexShader );
	if( FAILED( hResult ) )
	{
		TCLogger::GetInstance()->LogError( TCString("[TCShader] Failed to create vertex shader: ") + TCString( shader->GetName() ) + TCString( ", failed to create platform shader" ) );
		return Failure_InvalidOperation;
	}
	else
	{
		TC_SAFE_RELEASE(shaderBlob);
	}

	//
	// Set the graphics resources.
	//

	shaderToCreate->SetGraphicsResources( platformData );
	mShaders.Append( shaderToCreate );
	return Success;
}

//
// CreateMacroArray
//		- Will create the platform define information from shader output.
// Inputs:
//		- TCList< TCShaderImporter::DefineDescription >& defineData: The information to build the defines from.
// Outputs:
//		- D3D_SHADER_MACRO*: The NULL terminated array of defines.
//

D3D_SHADER_MACRO* TCGraphicsContext_DX11::TCShaderContext_DX11::CreateMacroArray( void* descriptions )
{
	TCList< TCShaderImporter::DefineDescription >& defineList = *(TCList< TCShaderImporter::DefineDescription >*) descriptions;
	TC_ASSERT( defineList.Count() < TC_MAX_SHADER_DEFINES && "Exceeded max shader defines!" );

	for( int currentDefineDescription = 0; currentDefineDescription < defineList.Count(); ++currentDefineDescription )
	{
		mMacros[ currentDefineDescription ].Name = defineList[ currentDefineDescription ].defineName.Data();
		mMacros[ currentDefineDescription ].Definition = defineList[ currentDefineDescription ].definition.Data();
	}

	mMacros[ defineList.Count() ].Definition = NULL;
	mMacros[ defineList.Count() ].Name = NULL;
	return mMacros;
}

//
// CreatePixelShader
//		- Will initialize the resources for a pixel shader.
// Inputs:
//		- void* description: The description for the pixel shader.
//		- TCShader* shaderToCreate: The shader to store the resources in.
// Outputs:
//		- TCresult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::CreatePixelShader( void* description, TCShader* shaderToCreate )
{
	TCShader::Description* shaderDesc = (TCShader::Description*)description;
	TCFileManager* fileManager = mContext->GetFileManager();

	//
	// Verify that this a unique instance.
	//

	TCShader* shader = GetShader( shaderDesc->shaderName );
	if( shader != NULL )
	{
		TCResult result = AddReference( *shaderToCreate, shader->GetGraphicsResources() );
		if( TC_FAILED( result ) )
		{
			TCLogger::GetInstance()->LogError( TCString( "[TCShader] Failed to add a reference to the shader: ") + TCResultUtils::ResultToString( result ) );
			return result;
		}
		
		return Success_Cached;
	}
	
	//
	// Now we need to create the platform shader data.
	//

	TCPixelShaderPlatformData* platformData = new TCPixelShaderPlatformData( mContext );

	//
	// Next we need to compile the shader.
	//

	ID3D10Blob* shaderBlob = NULL;
	TCResult result = CompileShader( &shaderBlob, shaderDesc->shaderImportData, description );
	if( TC_FAILED( result ) )
	{
		return result;
	}

	//
	// Now we need to create the specific shader.
	//

	HRESULT hResult = mDX11Context->mDevice->CreatePixelShader( shaderBlob->GetBufferPointer(),
																shaderBlob->GetBufferSize(),
																NULL,
																&platformData->mPixelShader );
	if( FAILED( hResult ) )
	{
		TCLogger::GetInstance()->LogError( TCString("[TCShader] Failed to create pixel shader: ") + TCString( shader->GetName() ) + TCString( ", failed to create platform shader" ) );
		return Failure_InvalidOperation;
	}
	else
	{
		TC_SAFE_RELEASE(shaderBlob);
	}

	//
	// Set the graphics resources.
	//

	shaderToCreate->SetGraphicsResources( platformData );

	mShaders.Append( shaderToCreate );
	return Success;
}

//
// CompileShader
//		- Will compile a shader based on the output of a shader import.
// Inputs:
//		- ID3D10Blob* shaderBlob: The compiled shader buffer.
//		- void* output: The output from a shader import.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::CompileShader( ID3D10Blob** shaderBlob, void* output, void* shaderDescription )
{
	TCShaderImporter::Output* importOutput = (TCShaderImporter::Output*)output;
	TCShader::Description* description = (TCShader::Description*)shaderDescription;

	UINT flags = 0;
	HRESULT hResult = S_OK;
	ID3D10Blob* errorBuffer = NULL;

#ifdef _DEBUG
	flags |= D3D10_SHADER_DEBUG|D3D10_SHADER_SKIP_OPTIMIZATION|D3D10_SHADER_WARNINGS_ARE_ERRORS;
#endif

	//
	// Create our platform define data.
	//

	D3D_SHADER_MACRO* macros = CreateMacroArray( (void*)&importOutput->mDefineData );

	ShaderIncludeFinder finder( mDX11Context->mFileManager->GetResourceDirectory(), description->shaderFilepath, mDX11Context->mFileManager );
	hResult = D3DCompile( importOutput->mShaderText.Data(),
						  importOutput->mShaderText.Length() + 1,
						  description->shaderName.Data(),
						  macros,
						  &finder,
						  importOutput->mEntryPointName.Data(),
						  TCShader::ProfileEnumToString( importOutput->mProfile ).Data(),
						  flags,
						  0,
						  shaderBlob,
						  &errorBuffer );
	if( FAILED( hResult ) )
	{
		TCLogger::GetInstance()->LogError( TCString( "[TCShader] Failed to compile shader: ") + description->shaderName + TCString( " with error: " ) + (char*)errorBuffer->GetBufferPointer() );
		return Failure_Unknown;
	}

	return Success;
}

//
// CreateShaderProgram
//		- Will initialize the resources for a shader program.
// Inputs:
//		- void* description: The description for the shader program.
//		- TCShaderProgram* shaderProgramToCreate: The shader program to store the resources in.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::CreateShaderProgram( void* description, TCShaderProgram* programToCreate )
{
	return Success;
}

//
// CreateConstantBuffer
//		- Will create a constant buffer to be used. 
// Inputs:
//		- void* description: The description for the constant buffer to be used.
//		- TCConstantBuffer* bufferToCreate: The constant buffer to store the resources in.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::CreateConstantBuffer( void* description, TCConstantBuffer* constantBufferToCreate )
{
	TCConstantBuffer::Description* cbDescription = (TCConstantBuffer::Description*)description;

	//
	// Check to see if this is a unique buffer.
	//

	TCConstantBuffer* firstBuffer = GetConstantBuffer( cbDescription->name );
	if( firstBuffer != NULL )
	{
		TCResult result = AddReference( *constantBufferToCreate, firstBuffer->GetGraphicsResources() );
		if( TC_FAILED( result ) )
		{
			TCLogger::GetInstance()->LogError( TCString("[TCConstantBuffer] Failed to add a reference to a constant buffer: ") + TCResultUtils::ResultToString( result ) );
			return result;
		}

		return Success_Cached;
	}
	
	//
	// Create a platform constant buffer.
	//

	ID3D11Buffer* buffer = NULL;
	D3D11_BUFFER_DESC bufferDescription;

	bufferDescription.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.ByteWidth				= cbDescription->bufferSize;
	bufferDescription.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	bufferDescription.Usage					= D3D11_USAGE_DYNAMIC;
	bufferDescription.MiscFlags				= 0;
	bufferDescription.StructureByteStride	= 0;

	HRESULT hResult = mDX11Context->mDevice->CreateBuffer( &bufferDescription, NULL, &buffer );
	if( FAILED( hResult ) )
	{
		TCLogger::GetInstance()->LogError( TCString( "[TCConstantBuffer] Failed to create constant buffer: " ) + hResult );
		return Failure_Unknown;
	}

	//
	// Create our container
	//

	TCConstantBufferPlatformData* platformData = new TCConstantBufferPlatformData( mContext );
	platformData->mConstantBuffer = buffer;

	//
	// Increase our reference.
	//

	TCResult result = AddReference( *constantBufferToCreate, platformData );
	if( TC_FAILED( result ) )
	{
		TCLogger::GetInstance()->LogError( TCString( "[TCConstantBuffer] Failed to add a reference: ") + TCResultUtils::ResultToString( result ) );
		return result;
	}

	return Success;
}

//
// CreateShaderUniform
//		- Will create a shader uniform to be used.
// Inputs:
//		- void* description: The description for the shader uniform to be created.
//		- TCShaderUniform* shaderUniformToCreate: The shader uniform to store the resources in.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::CreateShaderUniform( void* description, TCShaderUniform* uniformToCreate )
{
	TCShaderUniform::Description* uniformDescription = (TCShaderUniform::Description*)description;
	if( uniformDescription == NULL )
	{
		TCLogger::GetInstance()->LogError( TCString("[TCShaderUniform] Failed to create shader uniform: ") + TCResultUtils::ResultToString( Failure_InvalidParameter ) );
	}

	//
	// Nothing to do here.
	//

	return Success;
}

//
// CreateShaderAttribute
//		- Will create a shader attribute to be used.
// Inputs:
//		- void* description: The description for the shader attribute to be created.
//		- TCShaderAttribute* shaderAttributeToCreate: The shader attribute to store the resources in.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::CreateShaderAttribute( void* description, TCShaderAttribute* attributeToCreate )
{
	//
	// Nothing to do here.
	//

	return Success;
}

//
// ReleasePixelShader
//		- Will release resources used by a shader.
// Inputs:
//		- TCShader* shader: The shader to release resources for.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::ReleasePixelShader( TCShader* shader )
{
	TCPixelShaderPlatformData* platformData = (TCPixelShaderPlatformData*)shader->GetGraphicsResources();
	if( platformData == NULL )
	{
		return Failure_Cached;
	}

	platformData->mNumReferences--;
	if( platformData->mNumReferences <= 0 )
	{
		TC_SAFE_RELEASE(platformData->mPixelShader);
		delete platformData;
	}

	mShaders.Remove(shader);
	return Success;
}

//
// ReleaseVertexShader
//		- Will release resources used by a shader.
// Inputs:
//		- TCShader* shader: The shader to release resources for.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::ReleaseVertexShader( TCShader* shader )
{
	TCVertexShaderPlatformData* platformData = (TCVertexShaderPlatformData*)shader->GetGraphicsResources();
	if( platformData == NULL )
	{
		return Failure_Cached;
	}

	platformData->mNumReferences--;
	if( platformData->mNumReferences <= 0 )
	{
		TC_SAFE_RELEASE(platformData->mVertexShader);
		delete platformData;
	}

	mShaders.Remove(shader);
	return Success;
}

//
// ReleaseShaderProgram
//		- Will release resources used by a shader program.
// Inputs:
//		- TCShaderProgram* shaderProgram: The shader program to release the resources for.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::ReleaseShaderProgram( TCShaderProgram* program )
{
	if( program == NULL )
		return Failure_InvalidParameter;

	TCResult result = Success;
	if( program->mVertexShader != NULL )
	{
		result = program->mVertexShader->Release();
		if( TC_FAILED( result ) )
		{
			TCLogger::GetInstance()->LogError( TCString( "[TCShaderProgram] Failed to release vertex shader: ") + TCResultUtils::ResultToString( result ) );
			return result;
		}
	}

	if( program->mPixelShader != NULL )
	{
		result = program->mPixelShader->Release();
		if( TC_FAILED( result ) )
		{
			TCLogger::GetInstance()->LogError( TCString( "[TCShaderProgram] Failed to release pixel shader: ") + TCResultUtils::ResultToString( result ) );
			return result;
		}
	}

	return Success;
}

//
// ReleaseConstantBuffer
//		- Will release the resources used by a constant buffer.
// Inputs:
//		- TCConstantBuffer* buffer: The buffer to release the resources for.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::ReleaseConstantBuffer( TCConstantBuffer* buffer )
{
	TCConstantBufferPlatformData* platformData = (TCConstantBufferPlatformData*)buffer->GetGraphicsResources();
	if( platformData == NULL )
	{
		return Failure_Cached;
	}

	platformData->mNumReferences--;
	if( platformData->mNumReferences <= 0 )
	{
		TC_SAFE_RELEASE(platformData->mConstantBuffer);
		delete platformData;
	}

	mConstantBuffers.Remove(buffer);
	return Success;
}

//
// ReleaseShaderUniform
//		- Will release the resources used by a shader uniform.
// Inputs:
//		- TCShaderUniform* uniform: The shader uniform to release the resources for.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::ReleaseShaderUniform( TCShaderUniform* uniform )
{
	//
	// Nothing to do here.
	//

	return Success;
}

//
// ReleaseShaderAttribute
//		- Will release the resources used by a shader attribute.
// Inputs:
//		- TCShaderAttribute* attribute: The shader attribute to release the resources for.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::ReleaseShaderAttribute( TCShaderAttribute* attribute )
{
	//
	// Nothing to do here.
	//

	return Success;
}

//
// BindShaderProgram
//		- Will bind a shader program for use.
// Inputs:
//		- TCShaderProgram* program: The program to bind.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::BindShaderProgram( TCShaderProgram* program )
{
	if( program == NULL )
	{
		TCLogger::GetInstance()->LogError( TCString( "[TCShaderProgram] Failed to bind shader program; program is NULL" ) );
		return Failure_InvalidParameter;
	}

	//
	// Bind the vertex shader.
	//

	if( program->mVertexShader != NULL )
	{
		TCResult result = program->mVertexShader->Bind();
		if( TC_FAILED( result ) )
		{
			TCLogger::GetInstance()->LogError( TCString( "[TCShaderProgram] Failed to bind shader program; vertex shader failed with: " ) + TCResultUtils::ResultToString( result ) );
			return result;
		}
	}

	//
	// Bind the pixel shader.
	//

	if( program->mPixelShader != NULL )
	{
		TCResult result = program->mPixelShader->Bind();
		if( TC_FAILED( result ) )
		{
			TCLogger::GetInstance()->LogError( TCString( "[TCShaderProgram] Failed to bind shader program; pixel shader failed with: " ) + TCResultUtils::ResultToString( result ) );
			return result;
		}
 	}

	return Success;
}

//
// UnbindShaderProgram
//		- Will unbind a shader program for use.
// Inputs:
//		- TCShaderProgram* program: The program to unbind.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::UnbindShaderProgram( TCShaderProgram* program )
{
	if( program == NULL )
	{
		TCLogger::GetInstance()->LogError( TCString( "[TCShaderProgram] Failed to unbind shader program; program is NULL" ) );
		return Failure_InvalidParameter;
	}

	//
	// Unbind the vertex shader.
	//

	if( program->mVertexShader != NULL )
	{
		TCResult result = program->mVertexShader->Unbind();
		if( TC_FAILED( result ) )
		{
			TCLogger::GetInstance()->LogError( TCString( "[TCShaderProgram] Failed to unbind shader program; vertex shader failed with: " ) + TCResultUtils::ResultToString( result ) );
			return result;
		}
	}

	//
	// Unbind the pixel shader.
	//

	if( program->mPixelShader != NULL )
	{
		TCResult result = program->mPixelShader->Unbind();
		if( TC_FAILED( result ) )
		{
			TCLogger::GetInstance()->LogError( TCString( "[TCShaderProgram] Failed to unbind shader program; pixel shader failed with: " ) + TCResultUtils::ResultToString( result ) );
			return result;
		}
 	}

	return Success;
}

//
// BindVertexShader
//		- Will bind a shader to be used.
// Inputs:
//		- TCShader* shader: The shader to be bound.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::BindVertexShader( TCShader* shader )
{
	TCVertexShaderPlatformData* platformShader = (TCVertexShaderPlatformData*)shader->GetGraphicsResources();
	if( platformShader == NULL )
	{
		TCLogger::GetInstance()->LogError( TCString("Failed to bind vertex shader, shader is NULL" ) );
		return Failure_InvalidParameter;
	}

	mDX11Context->mDeviceContext->VSSetShader( platformShader->mVertexShader, NULL, 0 );
	mBoundShaders[ TCShader::kShaderTypeVertex ] = shader;

	return Success;
}

//
// BindPixelShader
//		- Will bind a shader to be used.
// Inputs:
//		- TCShader* shader: The shader to be bound.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::BindPixelShader( TCShader* shader )
{
	TCPixelShaderPlatformData* platformShader = (TCPixelShaderPlatformData*)shader->GetGraphicsResources();
	if( platformShader == NULL )
	{
		TCLogger::GetInstance()->LogError( TCString("Failed to bind pixel shader, shader is NULL" ) );
		return Failure_InvalidParameter;
	}

	mDX11Context->mDeviceContext->PSSetShader( platformShader->mPixelShader, NULL, 0 );
	mBoundShaders[ TCShader::kShaderTypePixel ] = shader;

	return Success;
}

//
// UnbindVertexShader
//		- Will unbind a shader from use.
// Inputs:
//		- TCShader* shader: The shader to be unbound.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::UnbindVertexShader( TCShader* shader )
{
	if( shader == mBoundShaders[ TCShader::kShaderTypeVertex ] )
	{
		mDX11Context->mDeviceContext->VSSetShader( NULL, NULL, 0 );
		mBoundShaders[ TCShader::kShaderTypeVertex ] = NULL;
	}
	return Success;
}

//
// UnbindPixelShader
//		- Will unbind a shader from use.
// Inputs:
//		- TCShader* shader: The shader to be unbound.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::UnbindPixelShader( TCShader* shader )
{
	if( shader == mBoundShaders[ TCShader::kShaderTypePixel ] )
	{
		mDX11Context->mDeviceContext->PSSetShader( NULL, NULL, 0 );
		mBoundShaders[ TCShader::kShaderTypePixel ] = NULL;
	}
	return Success;
}

//
// BindConstantBuffer
//		- Will bind a constant buffer from use.
// Inputs:
//		- TCConstantBuffer* constantBuffer: The constant buffer to be bound.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::BindConstantBuffer( TCConstantBuffer* buffer )
{
	TCConstantBufferPlatformData* platformData = (TCConstantBufferPlatformData*)buffer->GetGraphicsResources();
	if( platformData == NULL )
	{
		return Failure_InvalidParameter;
	}

	//
	// Now we need to map the constant buffer so we can write to it.
	//

	D3D11_MAPPED_SUBRESOURCE subResource;
	HRESULT hResult = mDX11Context->mDeviceContext->Map( platformData->mConstantBuffer,
														 0,
														 D3D11_MAP_WRITE,
														 0,
														 &subResource );
	if( FAILED( hResult ) )
	{
		TCLogger::GetInstance()->LogError( TCString("[TCConstantBuffer] Failed to map constant buffer: " ) + hResult );
		return Failure_Unknown;
	}

	//
	// Walk our uniforms and write the data to the correct offsets in the buffer.
	//

	unsigned int numUniforms = buffer->GetNumShaderUniforms();
	for( unsigned int currentUniform = 0; currentUniform < numUniforms; ++currentUniform )
	{
		TCShaderUniform* uniform = buffer->GetShaderUniform( currentUniform );
		if( uniform == NULL )
			continue;

		const void* data = uniform->GetData();
		unsigned int dataSize = uniform->GetDataSize();
		unsigned int offset = uniform->mRegister;

		bool didCopy = TCMemoryUtils::MemCopy( data, (void*)((unsigned char*)subResource.pData + offset), dataSize );
		if( !didCopy )
		{
			TCLogger::GetInstance()->LogError( TCString("[TCConstantBuffer] Failed to update constant buffer: Failed to copy uniform data") );
		}
	}

	//
	// Unmap the resource
	//

	mDX11Context->mDeviceContext->Unmap( platformData->mConstantBuffer, 0 );

	//
	// Now we need to bind the constant buffer.
	//

	switch( buffer->mShaderType )
	{
		case TCShader::kShaderTypeVertex:
		{
			mDX11Context->mDeviceContext->VSSetConstantBuffers( buffer->mRegister, 1, &platformData->mConstantBuffer ); 
			break;
		}

		case TCShader::kShaderTypePixel:
		{
			mDX11Context->mDeviceContext->PSSetConstantBuffers( buffer->mRegister, 1, &platformData->mConstantBuffer );
			break;
		}

		default:
		{
			TCLogger::GetInstance()->LogError( TCString( "[TCConstantBuffer] Failed to bind constant buffer: Unknown parent shader type." ) );
			return Failure_InvalidParameter;
		}
	}

	return Success;
}

//
// UnbindConstantBuffer
//		- Will unbind a constant buffer from use.
// Inputs:
//		- TCConstantBuffer* constantBuffer: The constant buffer to be unbound.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::UnbindConstantBuffer( TCConstantBuffer* buffer )
{
	if( buffer == NULL )
		return Failure_InvalidParameter;

	TCConstantBufferPlatformData* platformData = (TCConstantBufferPlatformData*)buffer->GetGraphicsResources();
	if( platformData == NULL )
		return Failure_InvalidParameter;

	ID3D11Buffer* nullBuffer = NULL;
	switch( buffer->mShaderType )
	{
		case TCShader::kShaderTypeVertex:
		{
			mDX11Context->mDeviceContext->VSSetConstantBuffers( buffer->mRegister, 1, &nullBuffer ); 
			break;
		}

		case TCShader::kShaderTypePixel:
		{
			mDX11Context->mDeviceContext->PSSetConstantBuffers( buffer->mRegister, 1, &nullBuffer );
			break;
		}

		default:
		{
			TCLogger::GetInstance()->LogError( TCString( "[TCConstantBuffer] Failed to bind constant buffer: Unknown parent shader type." ) );
			return Failure_InvalidParameter;
		}
	}
	return Success;
}

//
// SetShaderUniform
//		- Will update a shader uniform
// Inputs:
//		- TCShaderUniform* shaderUniform: The uniform to set.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::SetShaderUniform( TCShaderUniform* shaderUniform )
{
	//
	// Nothing to do here; it should be handled by the constant buffer.
	//

	return Success;
}

//
// SetShaderAttribute
//		- Will update a shader attribute.
// Inputs:
//		- TCShaderAttribute* shaderAttribute: The attribute to set.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::SetShaderAttribute( TCShaderAttribute* shaderAttribute )
{
	return Success;
}

//
// GetShader
//		- Will get a shader that is managed by this context.
// Inputs:
//		- TCString& shaderName: The name of the shader to find.
// Outputs:
//		- TCShader*: The shader found, NULL if not found.
//

TCShader* TCGraphicsContext_DX11::TCShaderContext_DX11::GetShader( TCString& shaderName )
{
	for( int currentShader = 0; currentShader < mShaders.Count(); ++currentShader )
	{
		if( mShaders[ currentShader ]->GetName() == shaderName )
		{
			return mShaders[ currentShader ];
		}
	}

	return NULL;
}

//
// GetConstantBuffer
//		- Will get a constant buffer that is managed by this context.
// Inputs:
//		- TCString& constantBufferName: The name of the shader to find.
// Outputs:
//		- TCConstantBuffer*: The constant buffer found, NULL if not found.
//

TCConstantBuffer* TCGraphicsContext_DX11::TCShaderContext_DX11::GetConstantBuffer( TCString& constantBufferName )
{
	for( int currentConstantBuffer = 0; currentConstantBuffer < mConstantBuffers.Count(); ++currentConstantBuffer )
	{
		if( mConstantBuffers[ currentConstantBuffer ]->GetName() == constantBufferName )
		{
			return mConstantBuffers[ currentConstantBuffer ];
		}
	}

	return NULL;
}

//
// GetShaderProgram
//		- Will get a shader program that is managed by this context.
// Inputs:
//		- TCString& shaderProgramName: The of the shader program to find.
// Outputs:
//		- TCShaderProgram*: The shader program found, NULL if not found.
//

TCShaderProgram* TCGraphicsContext_DX11::TCShaderContext_DX11::GetShaderProgram( TCString& shaderProgramName )
{
	for( int currentShaderProgram = 0; currentShaderProgram < mShaderPrograms.Count(); ++currentShaderProgram )
	{
		if(mShaderPrograms[ currentShaderProgram ]->GetName() == shaderProgramName )
		{
			return mShaderPrograms[ currentShaderProgram ];
		}
	}

	return NULL;
}

//
// AddReference
//		- Will copy the reference to a resource's platform data.
// Inputs:
//		- IGraphicsResource& inRef: The graphics resource to reference.
//		- void* platformResources: The resources to add the reference to.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::AddReference( IGraphicsResource& toReference, void* resources )
{
	TCPlatformData* platformData = (TCPlatformData*)resources;
	if( platformData == NULL )
	{
		return Failure_InvalidParameter;
	}

	platformData->mNumReferences++;
	toReference.SetGraphicsResources( platformData );

	return Success;
}

//
// RemoveReference
//		- Will remove a reference to a resource's platform data.
// Inputs:
//		- IGraphcisResource& inRef: The graphics resource to reference.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::RemoveReference( IGraphicsResource& inRef )
{
	TCPlatformData* platformData = (TCPlatformData*)inRef.GetGraphicsResources();
	if( platformData == NULL )
	{
		return Success_Handled;
	}

	platformData->mNumReferences--;
	return Success;
}

//
// Release
//		- Will release all resources associated with the shader context.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::Release()
{
	//
	// Release all the sub components.
	//
	
	TCResult result = ReleaseConstantBuffers();
	if (TC_FAILED(result))
	{
		TCLogger::GetInstance()->LogError("[TCShaderContext_DX11] Failed to release all constant buffers!");
		return result;
	}

	result = ReleaseShaderPrograms();
	if (TC_FAILED(result))
	{
		TCLogger::GetInstance()->LogError("[TCShaderContext_DX11] Failed to release all the shader programs!");
		return result;
	}

	result = ReleaseShaders();
	if( TC_FAILED( result ) )
	{
		TCLogger::GetInstance()->LogError("[TCShaderContext_DX11] Failed to release all shaders!");
		return result;
	}

	return Success;
}

//
// ReleaseConstantBuffers
//		- Will release all constant buffers
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::ReleaseConstantBuffers()
{
	for (int currentConstantBuffer = mConstantBuffers.Count() - 1; currentConstantBuffer >= 0; --currentConstantBuffer)
	{
		ReleaseConstantBuffer(mConstantBuffers[currentConstantBuffer]);
	}
	mConstantBuffers.Clear();

	return Success;
}

//
// ReleaseShaderPrograms
//		- Will release all shader programs
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::ReleaseShaderPrograms()
{
	for (int currentShaderProgram = mConstantBuffers.Count() - 1; currentShaderProgram >= 0; --currentShaderProgram)
	{
		ReleaseShaderProgram(mShaderPrograms[currentShaderProgram]);
	}
	mShaderPrograms.Clear();

	return Success;
}

//
// ReleaseShaders
//		- Will release all shaders
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCShaderContext_DX11::ReleaseShaders()
{
	for (int currentShader = mShaders.Count() - 1; currentShader >= 0; --currentShader)
	{
		switch (mShaders[currentShader]->GetType())
		{
			case TCShader::kShaderTypeVertex:
			{
				ReleaseVertexShader(mShaders[currentShader]);
				break;
			}

			case TCShader::kShaderTypePixel:
			{
				ReleasePixelShader(mShaders[ currentShader ] );
				break;
			}

			default:
			{
				TC_ASSERT("Failed to release unknown shader type!" && 0);
				break;
			}

		}
	}
	mShaders.Clear();

	return Success;
}

//
// TCVertexShaderPlatformData::Constructor
//		- Will initialize this platform data to a safe starting state
// Inputs:
//		- TCGraphicsContext* context: The context this platform data belongs to.
// Outputs:
//		- None. 
//

TCGraphicsContext_DX11::TCShaderContext_DX11::TCVertexShaderPlatformData::TCVertexShaderPlatformData( TCGraphicsContext* context )
	: TCPlatformData( context )
{
	mVertexShader = NULL;
}

//
// TCVertexShaderPlatformData::Destructor
//		- Will release all resources associated with this platform data.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCGraphicsContext_DX11::TCShaderContext_DX11::TCVertexShaderPlatformData::~TCVertexShaderPlatformData()
{
	TC_GRAPHICS_RESOURCE_SAFE_RELEASE( mVertexShader );
}

//
// TCPixelShaderPlatformData::Constructor
//		- Will initialize this platform data to a safe starting state
// Inputs:
//		- TCGraphicsContext* context: The context this platform data belongs to.
// Outputs:
//		- None. 
//

TCGraphicsContext_DX11::TCShaderContext_DX11::TCPixelShaderPlatformData::TCPixelShaderPlatformData( TCGraphicsContext* context )
	: TCPlatformData( context )
{
	mPixelShader = NULL;
}

//
// TCPixelShaderPlatformData::Destructor
//		- Will release all resources associated with this platform data.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCGraphicsContext_DX11::TCShaderContext_DX11::TCPixelShaderPlatformData::~TCPixelShaderPlatformData()
{
	TC_GRAPHICS_RESOURCE_SAFE_RELEASE( mPixelShader );
}

//
// TCConstantBufferPlatformData::Constructor
//		- Will initialize this platform data to a safe starting state
// Inputs:
//		- TCGraphicsContext* context: The context this platform data belongs to.
// Outputs:
//		- None. 
//

TCGraphicsContext_DX11::TCShaderContext_DX11::TCConstantBufferPlatformData::TCConstantBufferPlatformData( TCGraphicsContext* context )
	: TCPlatformData( context )
{
	mConstantBuffer = NULL;
}

//
// TCPixelShaderPlatformData::Destructor
//		- Will release all resources associated with this platform data.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCGraphicsContext_DX11::TCShaderContext_DX11::TCConstantBufferPlatformData::~TCConstantBufferPlatformData()
{
	TC_GRAPHICS_RESOURCE_SAFE_RELEASE( mConstantBuffer );
}