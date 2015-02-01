//
// TCList.h
// This file will define an object list that will safely resize and move objects.
//

#ifndef __TCLIST_H__
#define __TCLIST_H__

//
// Includes
//

#include "TCPlatformPrecompilerSymbols.h"

//
// Class Declaration
//

template< typename Object >
class TCList
{	
	public:		// Members
	public:		// Methods
						TCList();
						TCList( float growthRate );
						TCList( const TCList& list );
						~TCList();

		TCList&			operator=( const TCList& list );
		inline Object&	operator[]( int index )			{ TC_ASSERT( index >= 0 && index < mNumObjects && "Accessing a list out of bounds"); return mList[ index ]; }

		void			SetGrowthRate( float rate )		{ mGrowthRate = rate; }
		float			GetGrowthRate()					{ return rate; }

		int				Count()							{ return mNumObjects; }
		int				Capacity()						{ return mCapacity; }

		void			Append( Object object );
		void			Remove( Object object );
		void			RemoveAt( int index );
		void			Insert( Object object, int index );
		void			Clear();
		bool			Contains( Object object );
		int				Find( Object object );	
		void			Reserve( int amount );
		void			Resize( int size );
		void			Copy( const TCList& list );

	private:	// Members
		Object*		mList;
		int			mNumObjects;
		int			mCapacity;
		float		mGrowthRate;

	private:	// Methods
		void			IncreaseCapacity();
};

#include "TCList.hpp"	// Include the declaration.

#endif // __TCLIST_H__