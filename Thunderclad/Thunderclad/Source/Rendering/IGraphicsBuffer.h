//
// IGraphicsBuffer.h
// Will encapsulate all information regarding a buffer.
//

#ifndef __IGRAPHICS_BUFFER_H__
#define __IGRAPHICS_BUFFER_H__

//
// Includes
//

#include "IGraphicsResource.h"

//
// Defines
//

//
// Class Declaration
//

class IGraphicsBuffer
	: public IGraphicsResource
{
	public:		// Members
		enum Usage
		{
			Static = 0,
			Dynamic,
			Immutable,
		};

		enum Type
		{
			Vertex,
			Index,
		};

		enum LockState
		{
			Unlocked,
			Locked
		};

		struct Description
		{
			void*			mData;
			unsigned int	mDataSize;
			Usage			mUsage;
		};

	public:		// Methods
										IGraphicsBuffer( TCGraphicsContext* graphicsContext );
										IGraphicsBuffer( const IGraphicsBuffer& inRef );
		virtual	IGraphicsBuffer&		operator=( const IGraphicsBuffer& inRef );
		virtual							~IGraphicsBuffer();

		virtual TCResult				Initialize( Description& desc )			= 0;
		virtual TCResult				Release()								= 0;
		virtual TCResult				Fill( void* data, unsigned int size )	= 0;
		virtual TCResult				Bind( int index = 0 )					= 0;
		virtual TCResult				Unbind()								= 0;

				LockState				GetLockState()					const	{ return mLockState; }
				void					SetLockState( LockState state )			{ mLockState = state; }

				Usage					GetUsage()						const	{ return mUsage; }
				unsigned int			GetDataSize()					const	{ return mDataSize; }
				unsigned int			GetBoundIndex()					const	{ return mBindIndex; }
				Type					GetBufferType()					const	{ return mType; }

		virtual TCResult				GetBufferData( void** outData, unsigned int* outSize ) = 0;			

	protected:	// Members
		Usage			mUsage;
		Type			mType;
		unsigned int	mDataSize;
		unsigned int	mBindIndex;
		LockState		mLockState;

	protected:	// Methods
		virtual	void					Clone( const IGraphicsBuffer& inRef );
		virtual bool					IsValidDescription( Description& desc );
};

#endif