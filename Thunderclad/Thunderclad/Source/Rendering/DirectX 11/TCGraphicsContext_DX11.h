//
// TCGraphicsContext_DX11.h
// This object will define all the interactions between the application and the graphics API.
//

#ifndef __TC_GRAPHICS_CONTEXT_DX11_H__
#define __TC_GRAPHICS_CONTEXT_DX11_H__

//
// Includes
//

#include "TCGraphicsContext.h"
#include "TCList.h"
//#include "TCShaderImporter.h"

#if TC_GRAPHICS_PLATFORM_DX11

#include "D3D11.h"

//
// Defines
//

#define TC_MAX_SIMULTANEOUS_VERTEX_BUFFERS	(8)
#define TC_MAX_SHADER_DEFINES				(100)
#define TC_MAX_BOUND_SHADERS				(3)
#define TC_MAX_BOUND_CONSTANT_BUFFERS		(16)

//
// Forward Declarations
//

class IGraphicsResource;

//////////////////////////////////////////////////////////////////////////////////////////
// TCGraphicsContext_DX11 -- Defines the DX11 implementation of the graphics.
//////////////////////////////////////////////////////////////////////////////////////////

class TCGraphicsContext_DX11 :
	public TCGraphicsContext
{
	public:		// Members
	public:		// Methods
							TCGraphicsContext_DX11();
		virtual				~TCGraphicsContext_DX11();

		virtual	TCResult	Initialize( TCGraphicsContext::Description& desc );
		virtual TCResult	Destroy();
		virtual TCResult	BeginFrame();
		virtual TCResult	EndFrame();
		virtual TCResult	SwapBuffers();

	protected:	// Members
		ID3D11DeviceContext*		mDeviceContext;
		ID3D11Device*				mDevice;
		IDXGISwapChain*				mSwapChain;
		DXGI_SWAP_CHAIN_DESC		mSwapChainDesc;

		TCVertexBuffer*				mBoundVertexBuffers[ TC_MAX_SIMULTANEOUS_VERTEX_BUFFERS ];

		//
		// Temp Render Target Information -- Needs to be moved out.
		//

		ID3D11RenderTargetView*		mBackBufferView;
		ID3D11DepthStencilView*		mDepthStencilView;
		ID3D11DepthStencilState*	mDepthStencilState;
		ID3D11Texture2D*			mDepthStencilTexture;
		D3D11_VIEWPORT				mViewport;

		//
		// Temp Rasterizer State -- Needs to be moved out.
		//

		ID3D11RasterizerState*		mRasterizerState;

	protected:	// Methods
		virtual bool		IsValidDescription( Description& desc );
				TCResult	InitializeSwapChain( Description& desc );
				TCResult	InitializeDevice( Description& desc );
				TCResult	InitializeViewport( Description& desc );
				TCResult	InitializeBackBuffer( Description& desc );
				TCResult	InitializeSubContexts();

	protected:	// Component Interface Classes

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// TCBufferContext_DX11 -- The private interface of vertex/index buffers on DX11.
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class TCBufferContext_DX11 : public TCGraphicsContext::TCBufferContext
		{
			public:
									TCBufferContext_DX11( TCGraphicsContext_DX11* context );
			protected:
				struct TCBufferDX11Resources
				{
					ID3D11Buffer* mBuffer;
				};

				virtual TCResult	CreateVertexBuffer( void* description, TCVertexBuffer* bufferToCreate );
				virtual TCResult	CreateIndexBuffer( void* description, TCIndexBuffer* bufferToCreate );
				virtual TCResult	Release( IGraphicsBuffer* buffer );
				virtual TCResult	BindVertexBuffer( TCVertexBuffer* buffer, unsigned int index, unsigned int offset = 0 );
				virtual TCResult	UnbindVertexBuffer( TCVertexBuffer* buffer );
				virtual TCResult	BindIndexBuffer( TCIndexBuffer* buffer, unsigned int offset = 0 );
				virtual TCResult	UnbindIndexBuffer( TCIndexBuffer* buffer );
				virtual TCResult	Fill( IGraphicsBuffer* buffer, void* data, unsigned int dataSize );
				virtual TCResult	Lock( IGraphicsBuffer* buffer, void** data );
				virtual TCResult	Unlock( IGraphicsBuffer* buffer, void* data, unsigned int dataSize );
				virtual TCResult	Copy( IGraphicsBuffer* source, IGraphicsBuffer* dest );
				virtual TCResult	Release();

			protected: // Variables
				TCGraphicsContext_DX11*		mDX11GraphicsContext;

				friend class TCVertexBuffer;
				friend class TCIndexBuffer;

		};	friend class TCBufferContext_DX11;	// Make sure this class has access to the graphics context.

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// TCShaderContext_DX11 -- The private interface of shader/constant buffers on DX11
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class TCShaderContext_DX11 : public TCGraphicsContext::TCShaderContext
		{
			public:
											TCShaderContext_DX11( TCGraphicsContext_DX11* context );
			protected:
				virtual TCResult			CreateVertexShader( void* description, TCShader* shaderToCreate );
				virtual	TCResult			CreatePixelShader( void* description, TCShader* shaderToCreate );
				virtual TCResult			CreateShaderProgram( void* description, TCShaderProgram* programToCreate );
				virtual TCResult			CreateConstantBuffer( void* description, TCConstantBuffer* bufferToCreate );
				virtual TCResult			CreateShaderUniform( void* description, TCShaderUniform* shaderUniform );
				virtual TCResult			CreateShaderAttribute( void* description, TCShaderAttribute* shaderAttribute );
				virtual TCResult			ReleaseVertexShader( TCShader* shader );
				virtual TCResult			ReleasePixelShader( TCShader* shader );
				virtual TCResult			ReleaseShaderProgram( TCShaderProgram* program );
				virtual TCResult			ReleaseConstantBuffer( TCConstantBuffer* buffer );
				virtual TCResult			ReleaseShaderUniform( TCShaderUniform* uniform );
				virtual TCResult			ReleaseShaderAttribute( TCShaderAttribute* attribute );
				virtual TCResult			BindShaderProgram( TCShaderProgram* program );
				virtual TCResult			BindVertexShader( TCShader* shader );
				virtual TCResult			BindPixelShader( TCShader* shader );
				virtual TCResult			BindConstantBuffer( TCConstantBuffer* buffer );
				virtual TCResult			UnbindVertexShader( TCShader* shader );
				virtual TCResult			UnbindPixelShader( TCShader* shader );
				virtual TCResult			UnbindShaderProgram( TCShaderProgram* program );
				virtual TCResult			UnbindConstantBuffer( TCConstantBuffer* buffer );
				virtual TCResult			SetShaderUniform( TCShaderUniform* shaderUniform );
				virtual TCResult			SetShaderAttribute( TCShaderAttribute* shaderAttribute );

				virtual TCShader*			GetShader( TCString& shaderName );
				virtual TCConstantBuffer*	GetConstantBuffer( TCString& constantBufferName );
				virtual TCShaderProgram*	GetShaderProgram( TCString& shaderProgramName );

				virtual TCResult			AddReference( IGraphicsResource& toReference, void* platformResources );
				virtual	TCResult			RemoveReference( IGraphicsResource& source );
				virtual TCResult			Release();
				virtual	TCResult			ReleaseConstantBuffers();
				virtual TCResult			ReleaseShaderPrograms();
				virtual TCResult			ReleaseShaders();
						
			private:
						D3D_SHADER_MACRO*	CreateMacroArray( void* defineData );
						TCResult			CompileShader( ID3D10Blob** shaderBlob, void* shaderImportOutput, void* shaderDescription );

			protected: // Members
					TCGraphicsContext_DX11*			mDX11Context;
					D3D_SHADER_MACRO				mMacros[ TC_MAX_SHADER_DEFINES ];
					TCList< TCShader* >				mShaders;
					TCList< TCConstantBuffer* >		mConstantBuffers;
					TCList< TCShaderProgram* >		mShaderPrograms;

					TCShader*						mBoundShaders[ TC_MAX_BOUND_SHADERS ];
					TCConstantBuffer*				mBoundConstantBuffers[ TC_MAX_BOUND_CONSTANT_BUFFERS ];

			protected: // Hidden Classes.
				//
				// ShaderIncludeFinder
				//		- This class is a helper class used to find include files in my directories.
				//

				class ShaderIncludeFinder
					: public ID3DInclude
				{
					public:
						ShaderIncludeFinder( TCString systemDirectory, TCString shaderDirectory, TCFileManager* fileManager );
						HRESULT __stdcall Open( D3D_INCLUDE_TYPE  includeType,
												LPCSTR			pFileName,
												LPCVOID			pParentData,
												LPCVOID*			ppData,
												UINT*				pBytes );

						HRESULT __stdcall Close( LPCVOID pData );

					protected:
						TCString mSystemDirectory;
						TCString mShaderDirectory;
						TCFileManager* mFileManager;
				};

				//
				// TCVertexShaderPlatformData
				//		- This class will encapsulate platform specific information for shaders.
				//

				struct TCVertexShaderPlatformData
					: public TCPlatformData
				{
					ID3D11VertexShader* mVertexShader;

					TCVertexShaderPlatformData( TCGraphicsContext* context );
					~TCVertexShaderPlatformData();
				};

				//
				// TCPixelShaderPlatformData
				//		- This class will encapsulate platform specific information for pixel shaders.
				//

				struct TCPixelShaderPlatformData
					: public TCPlatformData
				{
					ID3D11PixelShader* mPixelShader;

					TCPixelShaderPlatformData( TCGraphicsContext* context );
					~TCPixelShaderPlatformData();
				};

				//
				// TCConstantBufferPlatformData
				//		- This class will encapsulate platform specific information for constant buffers.
				//

				struct TCConstantBufferPlatformData
					: public TCPlatformData
				{
					ID3D11Buffer* mConstantBuffer;

					TCConstantBufferPlatformData( TCGraphicsContext* context );
					~TCConstantBufferPlatformData();
				};

				friend class TCShader;
				friend class TCShaderProgram;
				friend class TCConstantBuffer;
				friend class TCShaderUniform;
		}; friend class TCShaderContext_DX11; // Give this context access to the graphics context.

}; // TCGraphicsContext_DX11

#endif // TC_GRAPHICS_PLATFORM_DX11

#endif // __TC_GRAPHICS_CONTEXT_DX11_H__