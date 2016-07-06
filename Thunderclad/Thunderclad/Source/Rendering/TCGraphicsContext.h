//
// TCGraphicsContext.h
// This object is responsible for the initialize and management of a platform's graphics context.
//

#ifndef __TC_GRAPHICS_CONTEXT_H__
#define __TC_GRAPHICS_CONTEXT_H__

//
// Includes
//

#include "TCResultCode.h"
#include "TCPlatformPrecompilerSymbols.h"
#include "TCString.h"
#include "TCList.h"

//
// Defines
//

//
// Forward Declarations
//

class TCWindow;
class IGraphicsBuffer;
class TCVertexBuffer;
class TCIndexBuffer;
class TCShader;
class TCShaderProgram;
class TCConstantBuffer;
class TCShaderUniform;
class TCShaderAttribute;
class TCFileManager;
class IGraphicsResource;

//
// Class Declaration
//

class TCGraphicsContext
{
	public:			// Variables
		struct Description
		{
			int				mNumBuffers;
			TCWindow*		mWindow;
			TCFileManager*	mFileManager;
		};

	protected:		// Component Interfaces

		//
		// TCPlatformData
		//		- This is the base class for all platform data.
		//

		struct TCPlatformData
		{
			unsigned int mNumReferences;
			TCGraphicsContext* mContext;

			TCPlatformData( TCGraphicsContext* context );
		};

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// TCBufferContext -- The private interface for all buffers
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class TCBufferContext
		{
			public:
									TCBufferContext( TCGraphicsContext* context )											{ mContext = NULL; }
			protected:
				virtual TCResult	CreateVertexBuffer( void* description, TCVertexBuffer* bufferToCreate )					{ return Failure_NotImplemented; }
				virtual TCResult	CreateIndexBuffer( void* description, TCIndexBuffer* bufferToCreate )					{ return Failure_NotImplemented; }
				virtual TCResult	Release( IGraphicsBuffer* buffer )														{ return Failure_NotImplemented; }
				virtual TCResult	BindVertexBuffer( TCVertexBuffer* buffer, unsigned int index, unsigned int offset = 0 )	{ return Failure_NotImplemented; }
				virtual TCResult	UnbindVertexBuffer( TCVertexBuffer* buffer )											{ return Failure_NotImplemented; }
				virtual TCResult	BindIndexBuffer( TCIndexBuffer* buffer, unsigned int offset = 0 )						{ return Failure_NotImplemented; }
				virtual TCResult	UnbindIndexBuffer( TCIndexBuffer* buffer )												{ return Failure_NotImplemented; }
				virtual TCResult	Lock( IGraphicsBuffer* buffer, void** vertices )										{ return Failure_NotImplemented; }
				virtual TCResult	Unlock( IGraphicsBuffer* buffer, void* vertices, unsigned int dataSize )				{ return Failure_NotImplemented; }
				virtual TCResult	Fill( IGraphicsBuffer* buffer, void* vertices, unsigned int dataSize )					{ return Failure_NotImplemented; }
				virtual TCResult	Copy( IGraphicsBuffer* source, IGraphicsBuffer* dest )									{ return Failure_NotImplemented; }

				TCGraphicsContext*	mContext;

				friend class TCVertexBuffer;
				friend class TCIndexBuffer;
		}; friend class TCBufferContext; // Give this context access to the graphics context.

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// TCShaderContext -- The private interface for all shaders
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class TCShaderContext
		{
			public:
									TCShaderContext( TCGraphicsContext* context )											{ mContext = NULL; }
			protected:
				virtual TCResult	CreateVertexShader( void* description, TCShader* shaderToCreate )						{ return Failure_NotImplemented; }
				virtual	TCResult	CreatePixelShader( void* description, TCShader* shaderToCreate )						{ return Failure_NotImplemented; }
				virtual TCResult	CreateShaderProgram( void* description, TCShaderProgram* programToCreate )				{ return Failure_NotImplemented; }
				virtual TCResult	CreateConstantBuffer( void* description, TCConstantBuffer* bufferToCreate )				{ return Failure_NotImplemented; }
				virtual TCResult	CreateShaderUniform( void* description, TCShaderUniform* uniformToCreate )				{ return Failure_NotImplemented; }
				virtual TCResult	CreateShaderAttribute( void* description, TCShaderAttribute* attributeToCreate )		{ return Failure_NotImplemented; }
				virtual TCResult	ReleasePixelShader( TCShader* shader )													{ return Failure_NotImplemented; }
				virtual TCResult	ReleaseVertexShader( TCShader* shader )													{ return Failure_NotImplemented; }
				virtual TCResult	ReleaseShaderProgram( TCShaderProgram* program )										{ return Failure_NotImplemented; }
				virtual TCResult	ReleaseConstantBuffer( TCConstantBuffer* buffer )										{ return Failure_NotImplemented; }
				virtual TCResult	ReleaseShaderUniform( TCShaderUniform* uniform )										{ return Failure_NotImplemented; }
				virtual TCResult	ReleaseShaderAttribute( TCShaderAttribute* attribute )									{ return Failure_NotImplemented; }
				virtual TCResult	BindShaderProgram( TCShaderProgram* program )											{ return Failure_NotImplemented; }
				virtual TCResult	BindVertexShader( TCShader* shader )													{ return Failure_NotImplemented; }
				virtual TCResult	BindPixelShader( TCShader* shader )														{ return Failure_NotImplemented; }
				virtual TCResult	BindConstantBuffer( TCConstantBuffer* buffer )											{ return Failure_NotImplemented; }
				virtual TCResult	UnbindVertexShader( TCShader* shader )													{ return Failure_NotImplemented; }
				virtual TCResult	UnbindPixelShader( TCShader* shader	)													{ return Failure_NotImplemented; }
				virtual TCResult	UnbindShaderProgram( TCShaderProgram* program )											{ return Failure_NotImplemented; }
				virtual TCResult	UnbindConstantBuffer( TCConstantBuffer* buffer )										{ return Failure_NotImplemented; }
				virtual TCResult	SetShaderUniform( const char* uniformName, void* data, unsigned int dataSize )			{ return Failure_NotImplemented; }
				virtual TCResult	SetShaderUniform( TCShaderUniform* uniform )											{ return Failure_NotImplemented; }
				virtual TCResult	SetShaderAttribute( TCShaderAttribute* attribute )										{ return Failure_NotImplemented; }
				virtual TCResult	SetConstantBufferUniform( TCConstantBuffer* buffer, TCShaderUniform* uniform )			{ return Failure_NotImplemented; }
				
				virtual TCShader*			GetShader( TCString& shaderName )												{ return NULL; }
				virtual TCConstantBuffer*	GetConstantBuffer( TCString& constantBufferName )								{ return NULL; }
				virtual TCShaderUniform*	GetShaderUniform( TCString& shaderUniformName )									{ return NULL; }
				virtual TCShaderProgram*	GetShaderProgram( TCString& shaderProgramName )									{ return NULL; }

				virtual TCResult			AddReference( IGraphicsResource& inRef, void* resourceData )					{ return Failure_NotImplemented; }
				virtual TCResult			RemoveReference( IGraphicsResource& inRef )										{ return Failure_NotImplemented; }

				TCGraphicsContext*				mContext;
				TCList< TCShader* >				mShaders;
				TCList< TCConstantBuffer* >		mConstantBuffers;
				TCList< TCShaderUniform* >		mShaderUniforms;
				TCList< TCShaderProgram* >		mShaderProgram;

				friend class TCShader;
				friend class TCShaderProgram;
				friend class TCConstantBuffer;
				friend class TCShaderUniform;
				friend class TCShaderAttribute;
		}; friend class TCShaderContext; // Give this context access to the graphics context.
		
	public:			// Functions
										TCGraphicsContext();
		virtual							~TCGraphicsContext();

		virtual	TCResult				Initialize( Description& desc ) = 0;
		virtual TCResult				Destroy();
		virtual TCResult				BeginFrame();
		virtual TCResult				EndFrame();
		virtual TCResult				SwapBuffers();

				TCBufferContext*		GetBufferContext()	{ return mBufferContext; }
				TCShaderContext*		GetShaderContext()	{ return mShaderContext; }
				TCFileManager*			GetFileManager()	{ return mFileManager; }

	protected:		// Variables
		TCWindow*				mWindow;
		TCBufferContext*		mBufferContext;
		TCShaderContext*		mShaderContext;
		TCFileManager*			mFileManager;


	protected:		// Functions
		virtual bool					IsValidDescription( Description& desc );

		// Friends
		friend class TCVertexBuffer;
		friend class TCIndexBuffer;
		friend class TCShader;
		friend class TCShaderProgram;
		friend class TCShaderUniform;
		friend class TCConstantBuffer;
		friend class TCShaderAttribute;
};

#endif // __TC_GRAPHICS_CONTEXT_H__