//
// TCMemUtils.h
// This file will define various helper methods for dealing with memory.
//

#ifndef __TCMEM_UTILS_H__
#define __TCMEM_UTILS_H__

//
// Includes
//

#include "TCPlatformPrecompilerSymbols.h"

//
// Defines
//

#define TC_SAFE_DELETE( x )				if( x != NULL ){ delete x; x = NULL; }
#define TC_SAFE_DELETE_ARRAY( x )		if( x != NULL ){ delete[] x; x = NULL; }

#endif // __TCMEM_UTILS_H__