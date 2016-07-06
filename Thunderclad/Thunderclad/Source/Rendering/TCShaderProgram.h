//
// TCShaderProgram.h
// This file will define how a multiple shaders will bind/interact when being used.
//

#ifndef __TC_SHADER_PROGRAM_H__
#define __TC_SHADER_PROGRAM_H__

//
// Includes
//

#include "TCGraphicsContext.h"
#include "TCShader.h"
#include "IGraphicsResource.h"

//
// Forward Declarations
//

class TCVertexFormat;

//
// Defines
//

//
// Class Declaration
//

class TCShaderProgram
	: IGraphicsResource
{
	public:		// Members
		struct TCShaderProgramDescription
		{
			TCShader*		mVertexShader;
			TCShader*		mPixelShader;
			TCVertexFormat* mVertexFormat;
			TCString		mProgramName;
		};

	public:		// Methods
									TCShaderProgram( TCGraphicsContext* context );
									TCShaderProgram( const TCShaderProgram& shaderProgram );
		virtual						~TCShaderProgram();
		virtual TCShaderProgram&	operator=( const TCShaderProgram& shaderProgram );

		virtual TCResult			Initialize( TCShaderProgramDescription* desc );
		virtual TCResult			Release();
		virtual TCResult			Bind();
		virtual TCResult			Unbind();

		const	TCString&			GetName() const		{ return mProgramName; }

	protected:	// Members
		TCShader*							mVertexShader;
		TCShader*							mPixelShader;
		TCVertexFormat*						mVertexFormat;
		bool								mIsBound;
		TCString							mProgramName;
		TCGraphicsContext::TCShaderContext*	mContext;

	protected:	// Methods
		virtual void				Clone( const TCShaderProgram& toCopy );

		friend class TCGraphicsContext;
		friend class TCGraphicsContext_DX11;
}; // TCShaderProgram

#endif //__TC_SHADER_PROGRAM_H__