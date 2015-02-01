#ifndef __TCRESULTCODE_H__
#define __TCRESULTCODE_H__

//
// TCResult
// These values are used to better inform the application of the results of operations.
//

enum TCResult
{
	Success_Unhandled = -3,
	Success_Handled = - 2,
	Success_Cached = -1,
	Success = 0,
	Failure_InvalidParameter,
	Failure_InvalidState,
	Failure_OutOfMemory,
	Failure_Cached,
	Failure_NotImplemented,
	Failure_InvalidOperation,
	Failure_OutOfBounds,
	Failure_InvalidAccess,
	Failure_InvalidPath,
	Failure_AlreadyExists,
	Failure_Unknown,
};

#define TC_SUCCEEDED( x )	(x <= TCResult::Success)
#define TC_FAILED( x )		(!TC_SUCCEEDED( x ))

#endif