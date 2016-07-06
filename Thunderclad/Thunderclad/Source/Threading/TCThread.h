//
// TCThread.h
// This file will define the platform agnostic interface for a thread.
//

#ifndef __TC_THREAD_H__
#define __TC_THREAD_H__

//
// Includes
//

#include "TCResultCode.h"

//
// Defines
//

//
// Forward Declarations
//

typedef TCResult (*TCThreadFunc)( void* data );

//
// Class Declaration
//

class TCThread
{
	public:		// Members
		enum ExecutionState
		{
			Uninitialized,		// The thread hasn't been allocated yet.
			Idle,				// The thread is allocated, but it suspended or not running yet.
			Running,			// The thread is currently executing.
			Joining,			// The thread is joining, (waiting to finish)
			Dead				// The thread is not executing and is deallocated.
		};


	public:		// Methods
								TCThread();
		virtual					~TCThread();
								TCThread( const TCThread& inRef );
		virtual	TCThread&		operator=( const TCThread& inRef );

		virtual TCResult		Initialize( TCThreadFunc, void* data ) = 0;
		virtual ExecutionState	GetState();
		virtual TCResult		Join();
		virtual TCResult		Suspend();
		virtual TCResult		Destroy();

	protected:	// Members
	protected:	// Methods
		virtual void			Copy( const TCThread& inRef );
};

#endif // __TC_THREAD_H__