//
// TCEventDispatcher
// This file is responsible for listening to a triggered event and dispatch it to any object listeners.
//

#ifndef __TCEVENT_DISPATCHER_H__
#define __TCEVENT_DISPATCHER_H__

//
// Includes
//

#include "TCList.h"
#include "TCResultCode.h"

//
// Defines
//

//
// Forward Declaration
//

class TCEventListener;

//
// Typedef
//

typedef unsigned int TCEventID;

//
// Class Declaration
//

class TCEventDispatcher
{
	public:		// Members
	public:		// Methods
									TCEventDispatcher();
									TCEventDispatcher( const TCEventDispatcher& inRef );
		virtual TCEventDispatcher&	operator=( const TCEventDispatcher& inRef );
		virtual						~TCEventDispatcher();

		virtual TCResult			FireEvent( TCEventID eventID, void* eventData = NULL );
		virtual TCResult			AddListener( TCEventListener* listener );
		virtual TCResult			RemoveListener( TCEventListener* listener );
		virtual void				CleanUp();

	private:	// Members
		TCList< TCEventListener* > mEventListeners;	
		
	protected:	// Methods
		virtual void Clone( const TCEventDispatcher& inRef );
};

#endif // __TCEVENT_DISPATCHER_H__