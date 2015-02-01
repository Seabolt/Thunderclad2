//
// TCHashTable.cpp
// This file will implement a hash table.
//

#ifndef __TC_HASH_TABLE_HPP__
#define __TC_HASH_TABLE_HPP__

//
// Includes
//

#include "TCHashTable.h"
#include "TCLogger.h"
#include "TCMathUtils.h"

#include <limits.h>

//
// Defines
//

#define MAX_COLLISIONS	5
#define HASH_PRIME		101

//
// Default Constructor
//		- Will initialize the class to a safe state.
// Inputs:
//		- unsigned int capacity: The capacity for this table.
//		- TCHashFunction function: The hash function to use.
// Outputs:
//		- None.
//

template< class Key, class Value >
TCHashTable< Key, Value >::TCHashTable( unsigned int capacity, TCHashFunction function )
{
	if( function == NULL )
	{
		TCLogger::GetInstance()->LogError("Provided a NULL hash funciton to a hash table. Falling back to default.");
		function = TCHashFunctions::OneAtATimeHash;
	}

	mCount = 0; 
	mHashFunction = function;

	IncreaseCapacity( capacity );
}

//
// Copy Constructor
//		- Will initialize this class to a copy of another hash table.
// Inputs:
//		- const TCHashTable& inRef: The table to initialize us to.
// Outputs:
//		- None.
//

template< class Key, class Value >
TCHashTable< Key, Value >::TCHashTable( const TCHashTable& inRef )
{
	Clone( inRef );
}

//
// Assignment Operator
//		- Will set this table equal to another table.
// Inputs:
//		- const TCHashTable& inRef: The hash table to set us equal to.
// Outputs:
//		- TCHashTable&: The reference to this table.
//

template< class Key, class Value >
TCHashTable< Key, Value >& TCHashTable< Key, Value >::operator=( const TCHashTable& inRef )
{
	Clear();
	Clone( inRef );
	return *this;
}

//
// Destructor
//		- This will free all resources associated with this table.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

template< class Key, class Value >
TCHashTable< Key, Value >::~TCHashTable()
{
	Clear();
}

//
// Clone
//		- This will copy another hash table to this table.
// Inputs:
//		- const TCHashTable& inRef: The table to copy.
// Outputs:
//		- None.
//

template< class Key, class Value >
void TCHashTable< Key, Value >::Clone( const TCHashTable< Key, Value >& inRef )
{
	mCount			= inRef.mCount;
	mCapacity		= inRef.mCapacity;
	mTable			= inRef.mTable;
	mKeys			= inRef.mKeys;
	mBucketIndices	= inRef.mBucketIndices;
	mHashFunction	= inRef.mHashFunction;
}

//
// Clear
//		- This will free all resources associated with this table.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

template< class Key, class Value >
void TCHashTable< Key, Value >::Clear()
{
	for( int currentBucket = 0; currentBucket < mTable.Count(); ++currentBucket )
	{
		mTable[ currentBucket ].Clear();
	}
	mTable.Clear();
	mKeys.Clear();
	mBucketListIndices.Clear();

	mCount = 0;
}

//
// AddValue
//		- This will add a value to the hash table.
// Inputs:
//		- Key key: The key for this value, it must not be a duplicate.
//		- Value value: The value for this key.
// Outputs:
//		- None.
//

template< class Key, class Value >
void TCHashTable< Key, Value >::AddValue( Key key, Value value )
{
	//
	// First we need to generate the hash for this key.
	//

	unsigned int hash = GetBucketIndex( key );

	//
	// Check to see if this key already exists.
	//

	if( mKeys.Contains( key ) )
	{
		TCLogger::GetInstance()->LogError("Hash Table already contains this key");
		return;
	}

	//
	// Now we create a new KVP and add it to the bucket.
	//

	mKeys.Append( key );
	int keyIndex = mKeys.Count() - 1;

	Entry entry;
	entry.value = value;
	entry.keyIndex = keyIndex;
	mTable[ hash ].Append( entry );

	int bucketIndex = mTable[ hash ].Count() - 1;
	mBucketListIndices.Append( bucketIndex );

	//
	// Check to see if we should increase capacity.
	//

	if( mTable[ hash ].Count() >= MAX_COLLISIONS )
	{
		IncreaseCapacity( GeneratePrime( mCapacity ) );
	}

	//
	// Increase our count.
	//

	mCount++;
}

//
// RemoveValue
//		- Will remove a value from the table.
// Inputs:
//		- Key key: The key for the value to remove.
// Outputs:
//		- None.
//

template< class Key, class Value >
void TCHashTable< Key, Value >::RemoveValue( Key key )
{
	//
	// Check to see if we have the key specified.
	//

	unsigned int keyIndex = mKeys.Find( key );
	if( keyIndex == -1 )
		return;

	//
	// Get the hash for the key
	//

	unsigned int hash = GetBucketIndex( key );
	unsigned int bucketIndex = mBucketListIndices[ keyIndex ];

	//
	// Remove the value from the table.
	//

	if( hash < (unsigned int)mTable.Count() )
	{
		for( int currentEntry = bucketIndex + 1; currentEntry < mTable[ hash ].Count(); ++currentEntry )
		{
			mBucketListIndices[ mTable[ hash ][ currentEntry ].keyIndex ]--;	// Shift back the bucket indices.
			mTable[ hash ][ currentEntry ].keyIndex--;
		}
		mTable[ hash ].RemoveAt( bucketIndex );
	}

	//
	// Remove the key.
	//

	mKeys.Remove( key );
	mBucketListIndices.RemoveAt( keyIndex );

	mCount--;


}

//
// ContainsValue
//		- Will return if the table contains a KVP with the value provided.
// Inputs:
//		- Value value: The value to search for.
// Outputs:
//		- bool: Does the table contain the value.
//

template< class Key, class Value >
bool TCHashTable< Key, Value >::ContainsValue( Value value )
{
	// Go through each bucket.
	for( int currentBucket = 0; currentBucket < mTable.Count(); ++currentBucket )
	{
		for( int currentBucketIndex = 0; currentBucketIndex < mTable[ currentBucket ].Count(); ++currentBucketIndex )
		{
			if( mTable[ currentBucket ][ currentBucketIndex ].value == value )
			{
				return true;
			}
		}
	}

	return false;
}

//
// ContainsKey
//		- Will return if the table contains the specified key.
// Inputs:
//		- Key key: The key to search for.
// Outputs:
//		- bool: Does the table contain the key.
//

template< class Key, class Value >
bool TCHashTable< Key, Value >::ContainsKey( Key key )
{
	return mKeys.Contains( key );
}

//
// GetValue
//		- Will return the value associated with a specified key.
// Inputs:
//		- Key key: The key associated with the value.
//		- Value* value: The pointer to set the value to.
// Outputs:
//		- Value: The value in the table.
//		- bool: Whether the operation succeeded or not.
//

template< class Key, class Value >
bool TCHashTable< Key, Value >::GetValue( Key key, Value** value )
{
	//
	// Make sure we have a valid value pointer.
	//

	if( value == NULL )
		return false;

	//
	// Make sure we actually have this key.
	//

	int keyIndex = mKeys.Find( key );
	if( keyIndex == -1 )
	{
		value = NULL;
		return false;	
	}

	//
	// Get our hash.
	//

	unsigned int hash = GetBucketIndex( key );

	//
	// Return the value
	//

	if( hash < (unsigned int)mTable.Count() )
	{
		*value = &mTable[ hash ][ mBucketListIndices[ keyIndex ] ].value;
	}

	return true;
}

//
// GetBucketIndex
//		- This function will take a key, generate a hash, and return the bucket index.
// Inputs:
//		- Key key: the key to use.
// Outputs:
//		- unsigned int: The bucket index to use.
//

template< class Key, class Value >
unsigned int TCHashTable< Key, Value >::GetBucketIndex( Key key )
{
	unsigned int hash = mHashFunction( (void*)&key, sizeof( Key ) );
	hash &= 0x7FFFFFFF;
	return hash % mTable.Count();
} 

//
// GeneratePrime
//		- This function will take in a capacity and return the next prime greater than it.
// Inputs:
//		- unsigned int capacity
// Outputs:
//		- unsigned int prime.
//

template< class Key, class Value >
unsigned int TCHashTable< Key, Value >::GeneratePrime( unsigned int capacity )
{
	for (int i = (capacity | 1); i < INT_MAX; i+=2) 
    {
        if (TCMathUtils::IsPrime(i))
            return i;
    }

	return -1;
}

//
// IncreaseCapacity
//		- This will expand the width of our table to better avoid collisions.
// Inputs:
//		- unsigned int capacity
// Outputs:
//		- None.
//

template< class Key, class Value >
void TCHashTable< Key, Value >::IncreaseCapacity( unsigned int capacity )
{
	if( capacity < 0 )
	{
		TCLogger::GetInstance()->LogFailure("Provided an invalid capacity to the hash table.");
		TC_ASSERT(0 && "Failed to increase capacity for hash table.");
	}

	if( capacity == 0 )
		capacity = 1;

	mCapacity = capacity;
	mTable.Resize( capacity );
}

#endif // __TC_HASH_TABLE_HPP__