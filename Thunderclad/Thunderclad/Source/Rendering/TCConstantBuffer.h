//
// TCConstantBuffer.h
// This file will declare the functionality of a constant buffer, which groups several shader uniforms together to bind them all at once.
//

#ifndef __TC_CONSTANT_BUFFER_H__
#define __TC_CONSTANT_BUFFER_H__

//
// Includes
//

#include "TCGraphicsContext.h"
#include "TCString.h"
#include "TCList.h"
#include "IGraphicsResource.h"

//
// Defines
//

//
// Forward Declarations
//

class TCShader;

//
// Class Declaration
//

class TCConstantBuffer
	: public IGraphicsResource
{
	public:		// Members
		struct Description
		{
			TCString name;
			int bufferRegister;
			int bufferSize;
			unsigned int shaderType;

			Description();
		};

		static const int kInvalidRegister = -1;

	public:		// Methods
							TCConstantBuffer( TCGraphicsContext* context );
							TCConstantBuffer( const TCConstantBuffer& toCopy );
							~TCConstantBuffer();
		TCConstantBuffer&	operator=( const TCConstantBuffer& toCopy );

		TCResult			Initialize( Description* description );
		TCResult			Release();
		TCResult			Bind();
		TCResult			Unbind();
		TCResult			UpdateShaderUniform( TCShaderUniform* uniform );
		TCResult			UpdateShaderUniform( const char8* uniformName, void* uniformData, unsigned int uniformSize );
		
		TCShaderUniform*	GetShaderUniform( const char8* uniformName );
		TCShaderUniform*	GetShaderUniform( unsigned int uniformIndex );
		unsigned int		GetNumShaderUniforms()							{ return mShaderUniforms.Count(); }
		const TCString&		GetName()										{ return mName; }

	protected:	// Members
		TCList< TCShaderUniform* >			mShaderUniforms;
		TCString							mName;
		unsigned int						mRegister;
		TCGraphicsContext::TCShaderContext* mContext;
		bool								mIsBound;
		unsigned int						mShaderType;

	protected:	// Methods
		void				Clone( const TCConstantBuffer& toCopy );

		friend class TCGraphicsContext;
		friend class TCGraphicsContext_DX11;
};

#endif //__TC_CONSTANT_BUFFER_H__