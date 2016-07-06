//
// TCGraphicsContext_DX11.cpp
// Will define the functionality for a TCGraphicsContext_DX11.
//

//
// Includes
//

#include "TCGraphicsContext_DX11.h"
#include "TCWindow.Win32.h"
#include "TCLogger.h"
#include "TCVertexBuffer.h"
#include "TCIndexBuffer.h"

#if TC_GRAPHICS_PLATFORM_DX11

//
// Defines
//

#define TC_SAFE_RELEASE( resource )					{ if( resource != NULL ) { resource->Release(); resource = NULL; } }
#define TC_GRAPHICS_CONTEXT_LOG_ERROR( string )		{ TCLogger::GetInstance()->LogError( TCString("[TCGraphicsContext] ") + string ); }

//
// Default Constructor
//		- Will initialize the object to a safe state.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCGraphicsContext_DX11::TCGraphicsContext_DX11()
	: TCGraphicsContext()
{
	mDevice					= NULL;
	mDeviceContext			= NULL;
	mSwapChain				= NULL;
	mBackBufferView			= NULL;
	mDepthStencilView		= NULL;
	mDepthStencilState		= NULL;
	mDepthStencilTexture	= NULL;

	mBufferContext			= new TCBufferContext_DX11( this );
}

//
// Destructor
//		- Will release all resources for this object
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCGraphicsContext_DX11::~TCGraphicsContext_DX11()
{
	Destroy();
}

//
// Initialize
//		- Will allocate all resources associated with this object.
// Inputs:
//		- Description& desc: The structure that will define how to initialize this object.
// Outputs:
//		- TCResult: The result of the method
//			- Success: The object initialized successfully.
//			- Failure_InvalidParameter: The description contains an invalid parameter.
//

TCResult TCGraphicsContext_DX11::Initialize( TCGraphicsContext::Description& desc )
{
	//
	// Call the base initialize.
	//

	TCResult result = TCGraphicsContext::Initialize( desc );
	if( TC_FAILED( result ) )
	{
		TC_GRAPHICS_CONTEXT_LOG_ERROR( "Failed initialize the base class!" );
		return result;
	}

	//
	// Initialize the swap chain.
	//

	result = InitializeSwapChain( desc );
	if( TC_FAILED( result ) )
	{
		TC_GRAPHICS_CONTEXT_LOG_ERROR( "Failed to initialize the swap chain!" );
		return result;
	}

	//
	// Initialize the device.
	//

	result = InitializeDevice( desc );
	if( TC_FAILED( result ) )
	{
		TC_GRAPHICS_CONTEXT_LOG_ERROR( "Failed initialize the device!" );
		return result;
	}

	//
	// Initialize the back buffer
	//

	result = InitializeBackBuffer( desc );
	if( TC_FAILED( result ) )
	{
		TC_GRAPHICS_CONTEXT_LOG_ERROR( "Failed initialize the back buffer!" );
		return result;
	}

	//
	// Initialize the view port
	//

	result = InitializeViewport( desc );
	if( TC_FAILED( result ) )
	{
		TC_GRAPHICS_CONTEXT_LOG_ERROR( "Failed initialize the viewport!" );
		return result;
	}

	//
	// Initialize all our sub modules.
	//

	result = InitializeSubContexts();

	return Success;
}

//
// Destroy
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the method.
//			- Success: The object cleaned up successfully.
//

TCResult TCGraphicsContext_DX11::Destroy()
{
	TC_SAFE_RELEASE( mBackBufferView );
	TC_SAFE_RELEASE( mDepthStencilView );
	TC_SAFE_RELEASE( mDepthStencilTexture );
	TC_SAFE_RELEASE( mDepthStencilState );
	TC_SAFE_RELEASE( mRasterizerState );
	TC_SAFE_RELEASE( mSwapChain );
	TC_SAFE_RELEASE( mDeviceContext );
	TC_SAFE_RELEASE( mDevice );
	TC_SAFE_DELETE( mBufferContext );

	return Success;
}

//
// BeginFrame
//		- Will prepare the frame for rendering.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The frame was prepared correctly.
//

TCResult TCGraphicsContext_DX11::BeginFrame()
{
	FLOAT color[ 4 ] = { 0.2f, 0.8f, 0.3f, 1.0f };
	mDeviceContext->ClearRenderTargetView( mBackBufferView, color );

	return Success;
}

//
// EndFrame
//		- Will finish a frame of rendering.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The frame finished correctly.
//

TCResult TCGraphicsContext_DX11::EndFrame()
{
	TCResult result = SwapBuffers();
	if( TC_FAILED( result ) )
	{
		return result;
	}

	return Success;
}

//
// SwapBuffers
//		- Will swap buffers from the back buffer to the front buffer.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The buffers were swapped correctly.
//

TCResult TCGraphicsContext_DX11::SwapBuffers()
{
	HRESULT result = mSwapChain->Present( 0, 0 );
	if( FAILED( result ) )
	{
		TC_GRAPHICS_CONTEXT_LOG_ERROR( "Failed to swap buffers!" );
		return Failure_InvalidOperation;
	}

	return Success;
}

//
// IsValidDescription
//		- Will determine if a description is valid.
// Inputs:
//		- Description& desc: The description to test if all it's variables are valid to initialize the context.
// Outputs:
//		- bool: Was the description valid?
//

bool TCGraphicsContext_DX11::IsValidDescription( TCGraphicsContext_DX11::Description& desc )
{
	return TCGraphicsContext::IsValidDescription( desc );
}

//
// InitializeSwapChain
//		- Will initialize the swap chain for this context.
// Inputs:
//		- Description& desc: The description to initialize this from.
// Outputs:
//		- TCResult: The result of this operation.
//			- Success: The operation succeeded.
//			- Failure_InvalidParameter: A parameter in the description isn't valid.
//

TCResult TCGraphicsContext_DX11::InitializeSwapChain( TCGraphicsContext_DX11::Description& desc )
{
	if( desc.mWindow == NULL )	return Failure_InvalidParameter;
	TCWindow_Win32* window = (TCWindow_Win32*)desc.mWindow;

	mSwapChainDesc.BufferCount			= 1;									// How many back buffers
	mSwapChainDesc.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;			// RGBA 32 bit color buffer
	mSwapChainDesc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;		// Allow the back buffer to be a render target
	mSwapChainDesc.OutputWindow			= window->GetHWindow();					// The window to render to
	mSwapChainDesc.Windowed				= !window->IsFullScreen();				// Are we using windowed?
	mSwapChainDesc.BufferDesc.Width		= (UINT)window->GetBounds().GetWidth();	// Back buffer width
	mSwapChainDesc.BufferDesc.Height	= (UINT)window->GetBounds().GetHeight();// Back buffer height
	mSwapChainDesc.SampleDesc.Count		= 1;									// MSAA sample count
	mSwapChainDesc.SampleDesc.Quality	= 0;									// MSAA quality

	return Success;
}

//
// InitializeDevice
//		- Will initialize the DirectX 11 device context.
// Inputs:
//		- Description& desc: The description to use to initialize the device from.
// Outputs:
//		- TCResult: The result of this operation.
//			- Success: The operation succeeded.
//			- Failure_InvalidOperation: The device failed to be created.
//			- Failure_InvalidParameter: A parameter in the description isn't valid.
//

TCResult TCGraphicsContext_DX11::InitializeDevice( TCGraphicsContext_DX11::Description& desc )
{
	//
	// Determine the type of device to create.
	//

	DWORD deviceFlags = 0;
#if TC_BUILD_CONFIGURATION_DEBUG
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//
	// Create the device and device context.
	//

	HRESULT result = D3D11CreateDeviceAndSwapChain( NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&mSwapChainDesc,
		&mSwapChain,
		&mDevice,
		NULL,
		&mDeviceContext );

	//
	// Determine if the creation was successful.
	//

	if( FAILED( result ) )
	{
		TC_GRAPHICS_CONTEXT_LOG_ERROR( TCString( "Failed to intialize the device and swap chain with error: " ) + TCString( (int)result ) );
		return Failure_InvalidOperation;	// The device failed to create
	}

	return Success; // All good!
}

//
// InitializeViewport
//		- Will initialize the viewport for the device context.
// Inputs:
//		- Description& desc: The description to use to initialize the viewport from.
// Outputs:
//		- TCResult: The result of this operation.
//			- Success: The operation succeeded.
//			- Failure_InvalidParameter: A parameter in the description isn't valid.
//

TCResult TCGraphicsContext_DX11::InitializeViewport( TCGraphicsContext_DX11::Description& desc )
{
	//
	// Create the viewport for the program
	//

	memset( &mViewport, 0, sizeof( D3D11_VIEWPORT ) );
	mViewport.TopLeftX	= (FLOAT)mWindow->GetPosition().x;
	mViewport.TopLeftY	= (FLOAT)mWindow->GetPosition().y;
	mViewport.Width		= (FLOAT)mWindow->GetBounds().GetWidth();
	mViewport.Height	= (FLOAT)mWindow->GetBounds().GetHeight();

	//
	// Set the viewport for the program
	//

	mDeviceContext->RSSetViewports( 1, &mViewport );

	//
	// Set up the correct rasterizer state for us
	//

	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.AntialiasedLineEnable	= FALSE;			// Should we smooth lines?
	rasterizerDesc.DepthBias				= 0;				// Should we apply a bias to our depth valids
	rasterizerDesc.DepthBiasClamp			= 0.0f;				// Should we a bias to the clamp.
	rasterizerDesc.SlopeScaledDepthBias		= 0.0f;				// Should we apply a bias to projective aliasing
	rasterizerDesc.DepthClipEnable			= FALSE;			// Should we enable depth clipping?
	rasterizerDesc.FillMode					= D3D11_FILL_SOLID;	// How should we fill the geometry
	rasterizerDesc.FrontCounterClockwise	= FALSE;			// Cull order.
	rasterizerDesc.MultisampleEnable		= FALSE;			// Should we enable MSAA.
	rasterizerDesc.ScissorEnable			= FALSE;			// Should we enable scissoring.
	rasterizerDesc.CullMode					= D3D11_CULL_NONE;	// Cull mode.
	
	HRESULT result = mDevice->CreateRasterizerState( &rasterizerDesc, &mRasterizerState );
	if( FAILED( result ) )
	{
		TC_GRAPHICS_CONTEXT_LOG_ERROR( "Failed to create the default rasterizer state!" );
		return Failure_InvalidParameter;
	}

	mDeviceContext->RSSetState( mRasterizerState );
	return Success;
}

//
// InitializeBackBuffer
//		- Will initialize the back buffer for this context.
// Inputs:
//		- Description& desc: The description to use to initialize the back buffer from.
// Outputs:
//		- TCResult: The result of this operation.
//			- Success: The operation succeeded.
//			- Failure_InvalidParameter: A parameter in the description isn't valid.
//			- Failure_InvalidOperation: The operation failed.
//

TCResult TCGraphicsContext_DX11::InitializeBackBuffer( TCGraphicsContext_DX11::Description& desc )
{
	HRESULT result = 0;

	//
	// Store the back buffer into a texture.
	//

	ID3D11Texture2D* pBackBuffer = NULL;												// Use a temporary texture to store the buffer
	mSwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer );		// The the back buffer
	
	//
	// Get the render target view for the back buffer.
	//
	
	result = mDevice->CreateRenderTargetView( pBackBuffer, NULL, &mBackBufferView );	// Create the RenderTargetView
	if( FAILED( result ) )
	{
		TC_GRAPHICS_CONTEXT_LOG_ERROR( "Failed to create the back buffer!" );
		return Failure_InvalidOperation;
	}
	
	//
	// Release the previous back buffer texture.
	//

	pBackBuffer->Release();	

	//
	// Create a texture to store the depth stencil buffer.
	//

	D3D11_TEXTURE2D_DESC depthDesc;
	memset( &depthDesc, 0, sizeof( depthDesc ) );
	depthDesc.Width					= (UINT)mWindow->GetBounds().GetWidth();	// The width of the depth buffer.
	depthDesc.Height				= (UINT)mWindow->GetBounds().GetHeight();	// The height of the depth buffer.
	depthDesc.ArraySize				= 1;										// The number of textures in the texture array. A cube map would have 6 textures.
	depthDesc.CPUAccessFlags		= 0;										// How should the CPU be able to access this texture. It should be read only.
	depthDesc.MipLevels				= 1;										// The number of mip levels. Only one for the back buffer.
	depthDesc.MiscFlags				= 0;										// Any new flags.
	depthDesc.Usage					= D3D11_USAGE_DEFAULT;						// The usage for the depth buffer.
	depthDesc.SampleDesc.Count		= 1;										// The number of multi-samples in MSAA.
	depthDesc.SampleDesc.Quality	= 0;										// The quality of the MSAA.
	depthDesc.BindFlags				= D3D11_BIND_DEPTH_STENCIL;					// The type of texture.

	// Note: DX11 doesn't have a D24 format
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;							// The format for the depth texture.

	//
	// Create the depth stencil texture
	//

	result = mDevice->CreateTexture2D( &depthDesc,
									   NULL,
									   &mDepthStencilTexture );
	if( FAILED( result ) )
	{
		TC_GRAPHICS_CONTEXT_LOG_ERROR( "Failed to create the depth stencil texture!" );
		return Failure_InvalidOperation;	// Failed to create the depth stencil texture.
	}

	//
	// Set up the depth stencil state
	//

	D3D11_DEPTH_STENCIL_DESC depthStencilDescription;
	memset( &desc, 0, sizeof( desc ) );
	depthStencilDescription.DepthEnable = 1;
	depthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDescription.StencilEnable = 0;

	result = mDevice->CreateDepthStencilState( &depthStencilDescription, &mDepthStencilState );
	if( FAILED( result ) )
	{
		TC_GRAPHICS_CONTEXT_LOG_ERROR( "Failed to create the depth stencil state!" );
		return Failure_InvalidOperation;
	}
	
	//
	// Set the depth stencil state
	//

	mDeviceContext->OMSetDepthStencilState( mDepthStencilState, 0 );

	//
	// Set up depth stencil view
	//

	D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc;
	viewDesc.Format = depthDesc.Format;
	viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = 0;
	viewDesc.Flags = 0;

	//
	// Create depth stencil view
	//

	result = mDevice->CreateDepthStencilView( mDepthStencilTexture,
											  &viewDesc,
											  &mDepthStencilView );
	if( FAILED( result ) )
	{
		TC_GRAPHICS_CONTEXT_LOG_ERROR( "Failed to create depth stencil view!" );
		return Failure_InvalidOperation;
	}

	mDeviceContext->OMSetRenderTargets( 1, &mBackBufferView, mDepthStencilView );
	return Success;
}

TCResult TCGraphicsContext_DX11::InitializeSubContexts()
{
	//
	// Create our buffer context.
	//

	mBufferContext = new TCBufferContext_DX11( this );
	if( mBufferContext == NULL )
	{
		return Failure_OutOfMemory;
	}

	//
	// Create our shader context.
	//

	mShaderContext = new TCShaderContext_DX11( this );
	if( mShaderContext == NULL )
	{
		return Failure_OutOfMemory;
	}

	return Success;
}

#endif //TC_GRAPHICS_PLATFORM_DX11
