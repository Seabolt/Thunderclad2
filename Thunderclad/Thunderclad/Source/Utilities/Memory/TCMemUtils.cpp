//
// TCMemUtils.cpp
// This file will define various utility functions for memory
//

//
// Includes
//

#include "TCMemUtils.h"
#include <memory.h>

//
// Defines
//

bool TCMemoryUtils::MemCopy( const void* source, void* dest, unsigned int dataSize )
{
	if( source == NULL || dest == NULL || dataSize <= 0 )
		return false;

	//
	// Check to make sure the memory doesn't overlap.
	//

	if( dest >= source && dest <= ((unsigned char*)source + dataSize) )
		return false;

	memcpy( dest, source, dataSize );
	return true;
}

bool TCMemoryUtils::MemCopySafe( const void* source, void* dest, unsigned int dataSize )
{
	if( source == NULL || dest == NULL || dataSize <= 0 )
		return false;

	//
	// Check to make sure the memory doesn't overlap.
	//

	if( dest >= source && dest <= ((unsigned char*)source + dataSize) )
		return false;

	memcpy_s( dest, dataSize, source, dataSize );
	return true;
}