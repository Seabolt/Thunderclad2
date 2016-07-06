//
// TCShaderImporter.h
// This class will define the information contained inside a TCShader file (.tc*s).
//

#ifndef __TC_SHADER_IMPORTER_H__
#define __TC_SHADER_IMPORTER_H__

//
// Includes
//

#include "TCShader.h"
#include "TCShaderUniform.h"
#include "TCShaderAttribute.h"
#include "TCConstantBuffer.h"
#include "node.h"

//
// Defines
//

//
// Forward Declarations
//

class TCFile;
class TCFileManager;

//
// Class Declaration
//

class TCShaderImporter
{
	public:		// Members

		struct IncludeDescription
		{
			TCString includeName;
		};

		struct DefineDescription
		{
			TCString defineName;
			TCString definition;
		};

		struct Description
		{
			TCString mShaderFilepath;
		};

		struct Output
		{
			TCList< TCShaderUniform::Description >		mUniformData;
			TCList< TCConstantBuffer::Description >		mConstantBufferData;
			TCList< TCShaderAttribute::Description >	mAttributeData;
			TCList< IncludeDescription >				mIncludeData;
			TCList< DefineDescription >					mDefineData;
			TCString									mEntryPointName;
			TCString									mShaderText;
			TCShader::Profile							mProfile;
			TCShader::Model								mModel;

			Output();
		};

	public:		// Methods
							TCShaderImporter( TCFileManager* fileManager );
							TCShaderImporter( const TCShaderImporter& toCopy );
							~TCShaderImporter();
		TCShaderImporter&	operator=( const TCShaderImporter& toCopy );

		TCResult			Import( TCString filepath, Output** output );
		TCResult			Release();

	private:	// Members
		TCFile*			mShaderFile;
		TCFileManager*	mFileManager;
		TCString		mCurrentName;
	
	private:	// Methods
		void			Clone( const TCShaderImporter& toCopy );
		TCString		GetShaderName( TCString& shaderFilepath );
		TCResult		ParseYAMLFile( TCString& filepath, Output* output );
		TCResult		ParseShaderProperties( YAML::Node node, Output* output );
		TCResult		ParseShaderUniforms( YAML::Node& node, Output* output );
		TCResult		ParseShaderUniformDefaultValue( YAML::Node& defaultNode, TCShaderUniform::Description* uniform, TCShaderUniform::Type type );
		TCResult		ParseShaderConstantBuffers( YAML::Node& node, Output* output );
		TCResult		ParseShaderAttributes( YAML::Node& node, Output* output );
		TCResult		ParseShaderSamplers( YAML::Node& node, Output* output );
		TCResult		ParseShaderText( YAML::Node& node, Output* output );

		TCResult		ReadBool( bool*& boolArray, TCString& valueString, TCString& uniformName, unsigned int numElements );
		TCResult		ReadInt( int*& intArray, TCString& valueString, TCString& uniformName, unsigned int numElements );
		TCResult		ReadHalf( float*& shortArray, TCString& valueString, TCString& uniformName, unsigned int numElements );
		TCResult		ReadFloat( float*& floatArray, TCString& valueString, TCString& uniformName, unsigned int numElements );
};

#endif // __TC_SHADER_IMPORTER_H__