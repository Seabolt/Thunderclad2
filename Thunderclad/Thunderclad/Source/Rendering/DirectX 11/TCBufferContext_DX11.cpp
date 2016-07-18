////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TCBufferContext_DX11 -- Implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Includes
//

#include "TCGraphicsContext_DX11.h"
#include "TCWindow.Win32.h"
#include "TCLogger.h"
#include "TCVertexBuffer.h"
#include "TCIndexBuffer.h"

//
// Defines
//

#define TC_SAFE_RELEASE( resource )					{ if( resource != NULL ) { resource->Release(); resource = NULL; } }

//
// DetermineBufferFlag
//		- Will determine the DX11 buffer type from the engine flag.
// Inputs:
//		- IGraphicsBuffer::Type type: The engine type.
// Outputs:
//		- D3D11_BIND_FLAG flag: The flag
//

D3D11_BIND_FLAG	DetermineBufferFlag( IGraphicsBuffer::Type type )
{
	switch( type )
	{
		case IGraphicsBuffer::Index:
			return D3D11_BIND_INDEX_BUFFER;

		case IGraphicsBuffer::Vertex:
		default:
			return D3D11_BIND_VERTEX_BUFFER;
	}
}

//
// DetermineAccessFlag
//		- Will determine the DX11 access flag from the engine flag.
// Inputs:
//		- IGraphicsBuffer::Usage usage: The engine usage.
// Outputs:
//		- D3D11_CPU_ACCESS_FLAG: The CPU access flag.
//

D3D11_CPU_ACCESS_FLAG DetermineAccessFlag( IGraphicsBuffer::Usage usage )
{
	switch( usage )
	{
		case IGraphicsBuffer::Static:		return D3D11_CPU_ACCESS_WRITE;
		case IGraphicsBuffer::Dynamic:		return D3D11_CPU_ACCESS_READ;
		case IGraphicsBuffer::Immutable:	return (D3D11_CPU_ACCESS_FLAG)0;
		default:							return (D3D11_CPU_ACCESS_FLAG)0;
	}
}

//
// DetermineUsage
//		- Will determine the DX11 usage.
// Inputs:
//		- IGraphicsBuffer::Usage: The engine usage.
// Outputs:
//		- D3D11_USAGE: The usage of this resource by DX11.
//

D3D11_USAGE DetermineUsage( IGraphicsBuffer::Usage usage )
{
	switch( usage )
	{
		case IGraphicsBuffer::Static:		return D3D11_USAGE_DEFAULT;
		case IGraphicsBuffer::Dynamic:		return D3D11_USAGE_DYNAMIC;
		case IGraphicsBuffer::Immutable:	return D3D11_USAGE_IMMUTABLE;
		default:							return D3D11_USAGE_DEFAULT;
	}
}


//
// TCVertexBufferContext_DX11::Constructor
//		- Will initialize this object to a default state.
// Inputs:
//		- TCGraphicsContext_DX11* context: The context to initialize this object with.
// Outputs:
//		- None.
//

TCGraphicsContext_DX11::TCBufferContext_DX11::TCBufferContext_DX11( TCGraphicsContext_DX11* context )
	: TCGraphicsContext::TCBufferContext( context )
{
	mDX11GraphicsContext = context;
}

//
// TCBufferContext_DX11::CreateVertexBuffer
//		- Will allocate a vertex buffer's GPU resources and return an allocated vertex buffer.
// Inputs:
//		- void* description: The information necessary to allocate a vertex buffer.
//		- IGraphicsBuffer* bufferToCreate: The buffer pointer to allocate.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCBufferContext_DX11::CreateVertexBuffer( void* description, TCVertexBuffer* bufferToCreate )
{
	if( mDX11GraphicsContext == NULL )			return Failure_InvalidState;
	if( mDX11GraphicsContext->mDevice == NULL )	return Failure_InvalidState;

	TCVertexBuffer::Description* bufferDesc = (TCVertexBuffer::Description*)description;

	//
	// Create the description
	//

	D3D11_BUFFER_DESC desc;
	desc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags			= DetermineAccessFlag( bufferDesc->mUsage );
	desc.Usage					= DetermineUsage( bufferDesc->mUsage );
	desc.ByteWidth				= bufferDesc->mDataSize;
	desc.MiscFlags				= 0;
	desc.StructureByteStride	= 0;

	//
	// Setup the subresource mapping.
	//

	D3D11_SUBRESOURCE_DATA subresourceData;
	subresourceData.pSysMem				= bufferDesc->mData;
	subresourceData.SysMemPitch			= 0;
	subresourceData.SysMemSlicePitch	= 0;

	//
	// Allocate the buffer.
	//

	ID3D11Buffer* buffer = NULL;
	HRESULT result = mDX11GraphicsContext->mDevice->CreateBuffer( &desc,
																  &subresourceData,
																  &buffer );
	if( FAILED( result ) )
	{
		TCLogger::GetInstance()->LogError( TCString( "[TCGraphicsContextDX11] Failed to create vertex buffer: " ) + result );
		return Failure_InvalidParameter;
	}

	//
	// Set the resources for that object.
	//

	TCBufferDX11Resources* resources = new TCBufferDX11Resources;
	resources->mBuffer = buffer; 
	bufferToCreate->SetGraphicsResources( resources );

	return Success;
}

//
// TCBufferContext_DX11::CreateIndexBuffer
//		- Will create an index buffer
// Inputs:
//		- void* description: The description for how to initialize this index buffer.
//		- TCIndexBuffer* indexBuffer: The index buffer to create.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCBufferContext_DX11::CreateIndexBuffer( void* description, TCIndexBuffer* indexBuffer )
{
	if( mDX11GraphicsContext == NULL )			return Failure_InvalidState;
	if( mDX11GraphicsContext->mDevice == NULL )	return Failure_InvalidState;

	TCIndexBuffer::Description* bufferDesc = (TCIndexBuffer::Description*)description;
	
	//
	// Create the description
	//

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth				= bufferDesc->mDataSize;
	desc.Usage					= DetermineUsage( bufferDesc->mUsage );
	desc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags			= DetermineAccessFlag( bufferDesc->mUsage );
	desc.MiscFlags				= 0;
	desc.StructureByteStride	= 0;

	//
	// Setup the subresource mapping.
	//

	D3D11_SUBRESOURCE_DATA subresourceData;
	subresourceData.pSysMem				= bufferDesc->mData;
	subresourceData.SysMemPitch			= 0;
	subresourceData.SysMemSlicePitch	= 0;

	//
	// Allocate the buffer.
	//

	ID3D11Buffer* buffer = NULL;
	HRESULT result = mDX11GraphicsContext->mDevice->CreateBuffer( &desc,
																  &subresourceData,
																  &buffer );
	if( FAILED( result ) )
	{
		TCLogger::GetInstance()->LogError( TCString( "[TCGraphicsContextDX11] Failed to create index buffer: " ) + result );
		return Failure_InvalidParameter;
	}

	//
	// Set the resources for that object.
	//

	TCBufferDX11Resources* resources = new TCBufferDX11Resources;
	resources->mBuffer = buffer; 
	indexBuffer->SetGraphicsResources( resources );

	return Success;
}

//
// TCBufferContext_DX11::Release
//		- Will unallocate a vertex buffer's GPU resources.
// Inputs:
//		- TCVertexBuffer* buffer: The buffer point to release.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCBufferContext_DX11::Release( IGraphicsBuffer* buffer )
{
	//
	// Get the resources for the vertex buffer.
	//

	TCBufferDX11Resources* resources = (TCBufferDX11Resources*)buffer->GetGraphicsResources();
	if( resources == NULL ) return Success_Handled;

	//
	// Release the buffer.
	//

	TC_SAFE_RELEASE( resources->mBuffer );
	return Success;
}

//
// TCBufferContext_DX11::BindVertexBuffer
//		- Will bind a vertex buffer to the GPU at a specified stream index.
// Inputs:
//		- IGraphicsBuffer* buffer: The buffer to bind.
//		- unsigned int index: The index to bind at.
//		- unsigned int offset: The offset into the vertex buffer.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCBufferContext_DX11::BindVertexBuffer( TCVertexBuffer* buffer, unsigned int index, unsigned int offset )
{
	//
	// Get the resources for the vertex buffer.
	//

	TCBufferDX11Resources* resources = (TCBufferDX11Resources*)buffer->GetGraphicsResources();
	if( resources == NULL ) return Success_Handled;

	//
	// Verify that the vertex bufer is valid.
	//

	if( buffer->GetVertexFormat() == NULL ) return Failure_InvalidParameter;

	//
	// Bind the vertex buffer.
	//

	ID3D11Buffer* buffers[] = { resources->mBuffer };
	UINT strides[]			= { buffer->mVertexFormat->Size() };
	UINT offsets[]			= { offset };
	mDX11GraphicsContext->mDeviceContext->IASetVertexBuffers( index, 1, buffers, strides, offsets );

	return Success;
}

//
// TCBufferContext_DX11::UnbindVertexBuffer
//		- Will unbind a vertex buffer from the GPU.
// Inputs:
//		- TCVertexBuffer* buffer: The buffer to unbind.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCBufferContext_DX11::UnbindVertexBuffer( TCVertexBuffer* buffer )
{
	mDX11GraphicsContext->mDeviceContext->IASetVertexBuffers( buffer->GetBoundIndex(), 1, NULL, NULL, NULL );
	return Success;
}

//
// TCBufferContext_DX11::BindIndexBuffer
//		- Will bind an index buffer to the GPU
// Inputs:
//		- TCIndexBuffer* buffer: The buffer to bind
//		- unsigned int offset: The offset into the index buffer to use.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCBufferContext_DX11::BindIndexBuffer( TCIndexBuffer* buffer, unsigned int offset )
{
	TCBufferDX11Resources* resources = (TCBufferDX11Resources*)buffer->GetGraphicsResources();
	if( resources == NULL ) return Success_Handled;

	//
	// Determine the index format
	//

	DXGI_FORMAT indexBufferFormat = buffer->GetIndexSize() == sizeof( unsigned int ) ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;

	//
	// Bind the index buffer.	
	//

	mDX11GraphicsContext->mDeviceContext->IASetIndexBuffer( resources->mBuffer, indexBufferFormat, offset );

	return Success;
}

//
// TCBufferContext_DX11::UnbindIndexBuffer
//		- Will unbind an index buffer from the GPU
// Inputs:
//		- TCIndexBuffer* buffer: The buffer to unbind.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCBufferContext_DX11::UnbindIndexBuffer( TCIndexBuffer* buffer )
{
	TCBufferDX11Resources* resources = (TCBufferDX11Resources*)buffer->GetGraphicsResources();
	if( resources == NULL ) return Success_Handled;

	//
	// Determine the index format
	//

	DXGI_FORMAT indexBufferFormat = buffer->GetIndexSize() == sizeof( unsigned int ) ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;

	//
	// Unbind the index buffer.
	//

	mDX11GraphicsContext->mDeviceContext->IASetIndexBuffer( NULL, indexBufferFormat, 0 );
	return Success;
}

//
// TCBufferContext_DX11::Lock
//		- Will lock this asset and return the vertex memory.
// Inputs:
//		- IGraphicsBuffer* buffer: The buffer to fill.
//		- void** vertices: The pointer to the vertex buffer's memory.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCBufferContext_DX11::Lock( IGraphicsBuffer* buffer, void** vertices )
{
	if( buffer == NULL )				return Failure_InvalidParameter;
	if( mDX11GraphicsContext == NULL )	return Failure_InvalidParameter;
	if( vertices == NULL )				return Failure_InvalidParameter;

	//
	// Make sure we have valid graphics resources.
	//

	TCBufferDX11Resources* resources = (TCBufferDX11Resources*)buffer->GetGraphicsResources();
	if( resources == NULL || resources->mBuffer == NULL ) return Failure_InvalidParameter;

	//
	// Lock the buffer and return the memory.
	//

	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	HRESULT result = mDX11GraphicsContext->mDeviceContext->Map( resources->mBuffer,
																0,
																D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
																0,
																&mappedSubResource );
	if( FAILED( result ) )
	{
		TCLogger::GetInstance()->LogError( TCString("[TCVertexBufferContext_DX11] Failed to map a vertex buffer: ") + result );
		return Failure_InvalidOperation;
	}

	//
	// Signal that the buffer is locked and set the output vertices.
	//

	*vertices = mappedSubResource.pData; 
	buffer->SetLockState( IGraphicsBuffer::Locked );
	return Success;
}

//
// TCBufferContext_DX11::Unlock
//		- Will unlock a vertex buffer, and copy the memory back in.
// Inputs:
//		- IGraphicsBuffer* buffer: The buffer to fill.
//		- void* vertices: The vertices to copy into the memory.
//		- unsigned int dataSize: The size of the data to copy.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCBufferContext_DX11::Unlock( IGraphicsBuffer* buffer, void* vertices, unsigned int dataSize )
{
	if( buffer == NULL )				return Failure_InvalidParameter;
	if( vertices == NULL )				return Failure_InvalidParameter;
	if( dataSize == 0 )					return Failure_InvalidParameter;
	if( mDX11GraphicsContext == NULL )	return Failure_InvalidParameter;

	//
	// Make sure we have valid graphics resources.
	//

	TCBufferDX11Resources* resources = (TCBufferDX11Resources*)buffer->GetGraphicsResources();
	if( resources == NULL || resources->mBuffer == NULL ) return Failure_InvalidParameter;

	//
	// Unmap the resource
	//

	mDX11GraphicsContext->mDeviceContext->Unmap( resources->mBuffer, 0 );
	return Success;
}

//
// TCBufferContext_DX11::Fill
//		- Will fill a buffer.
// Inputs:
//		- IGraphicsBuffer* buffer: The buffer to fill.
//		- void* data: The data.
//		- unsigned int: The data size
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCBufferContext_DX11::Fill( IGraphicsBuffer* buffer, void* data, unsigned int dataSize )
{
	//
	// Lock the vertex buffer first.
	//

	void* outputData = NULL;
	TCResult result = Lock( buffer, &outputData );
	if( TC_FAILED( result ) )
	{
		TCLogger::GetInstance()->LogError( "[TCBufferContext_DX11] Failed to lock the buffer when filling it." );
		return result;
	}

	//
	// Copy the memory into the resource
	//

	TCMemoryUtils::MemCopy( data, outputData, dataSize );

	//
	// Unlock the vertex buffer.
	//

	result = Unlock( buffer, outputData, dataSize );
	if( TC_FAILED( result ) )
	{
		TCLogger::GetInstance()->LogError( "[TCBufferContext_DX11] Failed to unlock the buffer when filling it." );
		return result;
	}
	
	return Success;
}

//
// TCBufferContext_DX11::Copy
//		- Will copy one set of GPU resources to another's buffer's resources.
// Inputs:
//		- const IGraphicsBuffer* source: The source buffer to copy from.
//		- IGraphicsBuffer* dest: The destination buffer to copy to.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCGraphicsContext_DX11::TCBufferContext_DX11::Copy( IGraphicsBuffer* source, IGraphicsBuffer* dest )
{
	if( source == NULL || source->GetGraphicsResources() == NULL )	return Failure_InvalidParameter;
	if( dest == NULL || dest->GetGraphicsResources() == NULL )		return Failure_InvalidParameter;

	TCBufferDX11Resources* sourceResources = (TCBufferDX11Resources*)source->GetGraphicsResources();
	TCBufferDX11Resources* destResources = (TCBufferDX11Resources*)dest->GetGraphicsResources();

	//
	// Lock the source buffer
	//

	void* sourceData = NULL;
	TCResult result = Lock( source, &sourceData );
	if( TC_FAILED( result ) )
	{
		gLogger->LogError( "Failed to lock source buffer when copying." );
		return result;
	}

	//
	// Fill the destination vertex buffer.
	//

	result = Fill( dest, sourceData, source->GetDataSize() );
	if( TC_FAILED( result ) )
	{
		gLogger->LogError( "Failed to fill the destination buffer" );
		return result;
	}

	//
	// Unlock the vertex buffer.
	//

	result = Unlock( source, sourceData, source->GetDataSize() );
	if( TC_FAILED( result ) )
	{
		gLogger->LogError( "Failed to unlock the source buffer" );
		return result;
	}

	return Success;
}

//
// Release
//		- Will release all resources associated with this context.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCResult TCGraphicsContext_DX11::TCBufferContext_DX11::Release()
{
	return Success;
}