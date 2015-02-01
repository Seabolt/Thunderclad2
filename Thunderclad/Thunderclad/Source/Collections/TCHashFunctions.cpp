//
// TCHashFunctions.cpp
// This file will define the various hash functions.
// NOTE: The majority of the functions here were taken from: http://www.eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
//

//
// Includes
//

#include "TCHashFunctions.h"
#include "TCPlatformPrecompilerSymbols.h"

//
// Defines
//

namespace TCHashFunctions
{
	//
	// AdditiveHashFunction
	//		- This is a naive hash function that works by adding each subsequent byte of a key.
	// Inputs:
	//		- void* data: The key.
	//		- unsigned int len: The length of the key in bytes.
	// Outputs:
	//		- unsigned int: The hash.
	//

	unsigned int AdditiveHash( void* data, unsigned int len )
	{
		TC_ASSERT( data != NULL && len > 0);

		unsigned char* pointer = (unsigned char*)data;
		unsigned int hash = 0;

		for( unsigned int currentByte = 0; currentByte < len; ++currentByte )
		{
			hash += pointer[ currentByte ];	// Accumulate the value of the bytes.
		}

		return hash;
	}

	//
	// XorHash
	//		- This is a hash function that works by XOR-ing each subsequent byte of a key.
	// Inputs:
	//		- void* data: The key.
	//		- unsigned int len: The length of the key in bytes.
	// Outputs:
	//		- unsigned int: The hash.
	//

	unsigned int XorHash( void* data, unsigned int len )
	{
		TC_ASSERT( data != NULL && len > 0);

		unsigned char* pointer = (unsigned char*)data;
		unsigned int hash = 0;

		for( unsigned int currentByte = 0; currentByte < len; ++currentByte )
		{
			hash ^= pointer[ currentByte ];
		}

		return hash;
	}

	//
	// RotatingHash
	//		- This is a naive hash function that works by XOR-ing then shifting the bytes.
	// Inputs:
	//		- void* data: The key.
	//		- unsigned int len: The length of the key in bytes.
	// Outputs:
	//		- unsigned int: The hash.
	//

	unsigned int RotatingHash( void* data, unsigned int len )
	{
		TC_ASSERT( data != NULL && len > 0);
		
		unsigned char* pointer = (unsigned char*)data;
		unsigned int hash = 0;

		for( unsigned int currentByte = 0; currentByte < len; ++currentByte )
		{
			hash = ( hash << 4 ) ^ ( hash >> 28 ) ^ pointer[ currentByte ];
		}

		return hash;
	}

	//
	// BernsteinHash
	//		- This is a strange hash that works well, but no one really knows why.
	// Inputs:
	//		- void* data: The key.
	//		- unsigned int len: The length of the key in bytes.
	// Outputs:
	//		- unsigned int: The hash.
	//

	unsigned int BernsteinHash( void* data, unsigned int len )
	{
		TC_ASSERT( data != NULL && len > 0);

		unsigned char* pointer = (unsigned char*)data;
		unsigned int hash = 0;

		for( unsigned int currentByte = 0; currentByte < len; ++currentByte )
		{
			hash = 33 * hash + pointer[ currentByte ];
		}

		return hash;
	}

	//
	// ShiftAddXorHash
	//		- This is a naive hash function that works by adding, shifting, then xor-ing each subsequent byte of a key.
	// Inputs:
	//		- void* data: The key.
	//		- unsigned int len: The length of the key in bytes.
	// Outputs:
	//		- unsigned int: The hash.
	//

	unsigned int ShiftAddXorHash( void* data, unsigned int len )
	{
		TC_ASSERT( data != NULL && len > 0);
		
		unsigned char* pointer = (unsigned char*)data;
		unsigned int hash = 0;

		for( unsigned int currentByte = 0; currentByte < len; ++currentByte )
		{
			hash ^= ( hash << 5 ) + ( hash >> 2 ) + pointer[ currentByte ];
		}

		return hash;
	}

	//
	// FNV
	//		- This is an excellent hash function created by Fowler/Noll/Vo
	// Inputs:
	//		- void* data: The key.
	//		- unsigned int len: The length of the key in bytes.
	// Outputs:
	//		- unsigned int: The hash.
	//

	unsigned int FNVHash( void* data, unsigned int len )
	{
		TC_ASSERT( data != NULL && len > 0);

		unsigned char* pointer = (unsigned char*) data;
		unsigned int hash = 2166136261;

		for( unsigned int currentByte = 0; currentByte < len; ++currentByte )
		{
			hash = ( hash * 16777619 ) ^ pointer[ currentByte ];
		}

		return hash;
	}

	//
	// OneAtATimeHash
	//		- This is a very excellent hash written by a leader in hashes: Bob Jenkins.
	// Inputs:
	//		- void* data: The key.
	//		- unsigned int len: The length of the key in bytes.
	// Outputs:
	//		- unsigned int: The hash.
	//

	unsigned int OneAtATimeHash( void* data, unsigned int len )
	{
		TC_ASSERT( data != NULL && len > 0 );

		unsigned char* pointer = (unsigned char*)data;
		unsigned int hash = 0;

		for( unsigned int currentByte = 0; currentByte < len; ++currentByte )
		{
			hash += pointer[ currentByte ];
			hash += ( hash << 10 );
			hash ^= ( hash >> 6 );
		}

		hash += ( hash << 3 );
		hash ^= ( hash >> 11 );
		hash += ( hash >> 15 );

		return hash;
	}
}