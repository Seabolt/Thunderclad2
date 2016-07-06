//
// TCShader.h
// This object will encapsulate the public interactions between the application and the shader subsystem.
//

#ifndef __TC_SHADER_H__
#define __TC_SHADER_H__

//
// Includes
//

#include "TCGraphicsContext.h"
#include "TCString.h"
#include "TCVertexFormat.h"
#include "IGraphicsResource.h"

//
// Defines
//

//
// Forward Declarations
//

//
// Class Declaration
//

class TCShader
	: public IGraphicsResource
{
	public: // Members
		
		enum Type
		{
			kShaderTypeUnknown = -1,
			kShaderTypeVertex,
			kShaderTypePixel,
			kShaderTypeGeometry,
			kNumShaderTypes,
		};

		enum Model
		{
			kShaderModelUnknown = -1,
			kShaderModel_HLSL_3,
			kShaderModel_HLSL_4,
			kShaderModel_HLSL_5,
			kShaderModel_GLSL_1_20,
			kShaderModel_GLSL_1_30,
			kShaderModel_GLSL_1_40,
			kShaderModel_GLSL_1_50,
			kNumShaderModel
		};

		enum Profile
		{
			kShaderProfileUnknown = -1,
			kShaderProfile_vs_1_1,
			kShaderProfile_ps_2_0,
			kShaderProfile_ps_2_x,
			kShaderProfile_vs_2_0,
			kShaderProfile_vs_2_x,
			kShaderProfile_ps_4_0_level_9_0,
			kShaderProfile_ps_4_0_level_9_1,
			kShaderProfile_ps_4_0_level_9_3,
			kShaderProfile_vs_4_0_level_9_0,
			kShaderProfile_vs_4_0_level_9_1,
			kShaderProfile_vs_4_0_level_9_3,
			kShaderProfile_lib_4_0_level_9_0,
			kShaderProfile_lib_4_0_level_9_1,
			kShaderProfile_lib_4_0_level_9_3,
			kShaderProfile_ps_3_0,
			kShaderProfile_vs_3_0,
			kShaderProfile_cs_4_0,
			kShaderProfile_gs_4_0,
			kShaderProfile_vs_4_0,
			kShaderProfile_ps_4_0,
			kShaderProfile_lib_4_0,
			kShaderProfile_cs_4_1,
			kShaderProfile_gs_4_1,
			kShaderProfile_vs_4_1,
			kShaderProfile_ps_4_1,
			kShaderProfile_lib_4_1,
			kShaderProfile_cs_5_0,
			kShaderProfile_ds_5_0,
			kShaderProfile_gs_5_0,
			kShaderProfile_ps_5_0,
			kShaderProfile_hs_5_0,
			kShaderProfile_vs_5_0,
			kShaderProfile_lib_5_0,
			kShaderProfile_core,
			kShaderProfile_compatability,
			kNumShaderProfiles
		};

		struct Description
		{
			Type		shaderType;
			TCString	shaderName;
			TCString	shaderFilepath;

			Description();

			private:
				void*		shaderImportData;
				friend class TCGraphicsContext;
				friend class TCGraphicsContext_DX11;
				friend class TCShader;
		};

	public: // Methods

							TCShader( TCGraphicsContext* context );
							TCShader( TCShader& toCopy );
							~TCShader();
		virtual TCShader&	operator=( const TCShader& toCopy );

		virtual TCResult	Initialize( Description& shaderDescription );
		virtual TCResult	Release();
		virtual TCResult	Bind();
		virtual TCResult	Unbind();

		virtual TCResult	SetShaderUniform( char* uniformName, void* data, unsigned int dataSize );

				Profile		GetProfile()		{ return mProfile; }
				Type		GetType()			{ return mType; }
				Model		GetModel()			{ return mModel; }
		const   TCString&	GetName()			{ return mName; }

		static	Type		TypeStringToEnum( TCString& string );
		static	TCString	TypeEnumToString( Type type );
		static	TCString	ProfileEnumToString( Profile profile );
		static	Profile		ProfileStringToEnum( TCString& string );
		static	TCString	ModelEnumToString( Model model );
		static	Model		ModelStringToEnum( TCString& string );
			
	protected: // Members
		Type								mType;
		Profile								mProfile;
		Model								mModel;
		TCGraphicsContext::TCShaderContext*	mShaderContext;
		TCList< TCShaderUniform* >			mUniforms;
		TCList< TCConstantBuffer* >			mConstantBuffers;
		TCList< TCShaderAttribute* >		mInputAttributes;
		TCString							mName;
		bool								mIsBound;

	protected: // Methods
		virtual void		Clone( const TCShader& toCopy );
		virtual TCResult	Import( const TCString& filepath, void** output );
		virtual TCResult	PopulateSubComponents( void* importData );
		virtual TCResult	PopulateConstantBuffers( void* importData );
		virtual TCResult	PopulateShaderUniforms( void* importData );
		virtual TCResult	PopulateShaderAttributes( void* importData );
};

#endif // __TC_SHADER_H__