//
// TCEventListener
// This file will define the interface for a object to listen to events.
//

#ifndef __TC_EVENT_LISTENER_H__
#define __TC_EVENT_LISTENER_H__

//
// Includes
//

#include "TCList.h"

//
// Defines
//

//
// Typedef
//

typedef unsigned int TCEventID;

//
// Forward Declarations
//

class TCEventDispatcher;

//
// Class Declaration
//

class TCEventListener
{
	public:		// Members
	public:		// Methods	
									TCEventListener();
									TCEventListener( const TCEventListener& inRef );
		virtual						~TCEventListener();
		virtual	TCEventListener&	operator=( const TCEventListener& inRef );

		virtual void				SubscribeTo( TCEventDispatcher* dispatcher );
		virtual void				UnsubscribeFrom( TCEventDispatcher* dispatcher );
		virtual void				UnsubscribeFromAll();

		virtual void				OnEventFired( TCEventID eventId, void* eventData ) = 0;

	private:	// Members
		TCList< TCEventDispatcher* >	mEventDispatchers;

	protected:	// Methods
		virtual void				Clone( const TCEventListener& inRef );
};

#endif // __TC_EVENT_LISTENER_H__