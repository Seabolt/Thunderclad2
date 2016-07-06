//
// TCThreadWin32.h
// This object will define the win32 variant of a thread.
//

#ifndef __TC_THREAD_WIN32_H__
#define __TC_THREAD_WIN32_H__

//
// Includes
//

#include "TCThread.h"

//
// Defines
//

//
// Forward Declarations
//

//
// Class Declaration
//

class TCThreadWin32 : public TCThread
{
	public:		// Members
	public:		// Methods
								TCThreadWin32();
		virtual					~TCThreadWin32();
								TCThreadWin32( const TCThreadWin32& inRef );
		virtual	TCThreadWin32&	operator=( const TCThreadWin32& inRef );

		virtual TCResult		Initialize( TCThreadFunc, void* data ) = 0;
		virtual ExecutionState	GetState();
		virtual TCResult		Join();
		virtual TCResult		Suspend();
		virtual TCResult		Destroy();

	protected:	// Members
	protected:	// Methods
		virtual void			Copy( const TCThread& inRef );
};

#endif // __TC_THREAD_WIN32_H__