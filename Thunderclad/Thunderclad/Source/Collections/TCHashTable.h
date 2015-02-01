//
// TCHashTable.h
// This file will declare a hash table that provides fast access at the cost of higher memory.
//

#ifndef __TC_HASH_TABLE_H__
#define __TC_HASH_TABLE_H__

//
// Includes
//

#include "TCPlatformPrecompilerSymbols.h"
#include "TCHashFunctions.h"
#include "TCList.h"

//
// Defines
//

//
// Class Declaration
//

template< typename Key, typename Value >
class TCHashTable
{
	public:		// Members
		class Entry
		{
			public:
				Value value;
				int keyIndex;
		};

	public:		// Methods

						TCHashTable( unsigned int capacity = 0, TCHashFunction func = TCHashFunctions::OneAtATimeHash );
						TCHashTable( const TCHashTable& table );
		TCHashTable&	operator=( const TCHashTable& table );
						~TCHashTable();
						
		void			Clone( const TCHashTable& table );
		void			Clear();
		inline int		Count()										{ return mCount; }

		void			AddValue( Key key, Value value );
		void			RemoveValue( Key key );

		bool			ContainsValue( Value value );
		bool			ContainsKey( Key key );

		int				GetNumKeys()								{ return mKeys.Count(); }
		Key				GetKey( int keyIndex )						{ return mKeys[ keyIndex ]; }
		bool			GetValue( Key key, Value** value );

	private:	// Members
		TCList< TCList< Entry > >	mTable;
		TCList< Key >				mKeys;
		TCList< int >				mBucketListIndices;
		TCHashFunction				mHashFunction;
		unsigned int				mCapacity;
		unsigned int				mCount;

	private:	// Methods

		void IncreaseCapacity( unsigned int capacity );
		unsigned int GetBucketIndex( Key key );
		unsigned int GeneratePrime( unsigned int capacity );
};

#include "TCHashTable.hpp"

#endif // __TC_HASH_TABLE_H__