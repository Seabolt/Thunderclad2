//
// TCResultCode.cpp
//		This file will define all result utility functions.
//

//
// Includes
//

#include "TCResultCode.h"

//
// Defines
//

namespace TCResultUtils
{
	TCString ResultToString( TCResult result )
	{
		switch( result )
		{
			case Success_EndOfFile:			return TCString( "Success_EndOfFile" );
			case Success_Unhandled:			return TCString( "Success_Unhandled" );
			case Success_Handled:			return TCString( "Success_Handled" );
			case Success_Cached:			return TCString( "Success_Cached" );
			case Success:					return TCString( "Success" );
			case Failure_InvalidParameter:	return TCString( "Failure_InvalidParameter" );
			case Failure_InvalidState:		return TCString( "Failure_InvalidState" );
			case Failure_OutOfMemory:		return TCString( "Failure_OutOfMemory" );
			case Failure_Cached:			return TCString( "Failure_Cached" );
			case Failure_NotImplemented:	return TCString( "Failure_NotImplemented" );
			case Failure_InvalidOperation:	return TCString( "Failure_InvalidOperation" );
			case Failure_OutOfBounds:		return TCString( "Failure_OutOfBounds" );
			case Failure_InvalidAccess:		return TCString( "Failure_InvalidAccess" );
			case Failure_InvalidPath:		return TCString( "Failure_InvalidPath" );
			case Failure_FileNotFound:		return TCString( "Failure_FileNotFound" );
			case Failure_MalformedData:		return TCString( "Failure_MalformedData" );
			case Failure_AlreadyExists:		return TCString( "Failure_AlreadyExists" );
			default:						return TCString( "Failure_Unknown" );
		}
	}
}