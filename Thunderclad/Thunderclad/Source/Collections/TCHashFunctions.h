//
// TCHashFunctions.h
// This file will define some basic hash functions.
//

#ifndef __TC_HASH_FUNCTIONS_H__
#define __TC_HASH_FUNCTIONS_H__

//
// Typedefs
//

typedef unsigned int (*TCHashFunction)( void* data, unsigned int len );	// TCHashFunction will serve as our reference to a hash function.

namespace TCHashFunctions
{
	unsigned int AdditiveHash( void* data, unsigned int len );
	unsigned int XorHash( void* data, unsigned int len );
	unsigned int RotatingHash( void* data, unsigned int len );
	unsigned int BernsteinHash( void* data, unsigned int len );
	unsigned int ShiftAddXorHash( void* data, unsigned int len );
	unsigned int FNVHash( void* data, unsigned int len );
	unsigned int OneAtATimeHash( void* data, unsigned int len );
}

#endif