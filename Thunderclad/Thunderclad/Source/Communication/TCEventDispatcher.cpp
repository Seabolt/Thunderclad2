//
// TCEventDispatcher.cpp
// This file will define the functionality for a event dispatcher.
//

//
// Includes
//

#include "TCEventDispatcher.h"
#include "TCEventListener.h"
#include "TCLogger.h"

//
// Defines
//

//
// Default Constructor
//		- Initializes this object to a safe default state.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCEventDispatcher::TCEventDispatcher()
{
	mEventListeners.SetGrowthRate( 2.0f );	// This will exand quickly when adding a bunch of listeners.
}

//
// Copy Constructor
//		- Will initialize this object to be a copy of another object.
// Inputs:
//		- const TCEventDispatcher& inRef: The reference to copy from.
// Outputs:
//		- None.
//

TCEventDispatcher::TCEventDispatcher( const TCEventDispatcher& inRef )
{
	Clone( inRef );
}

//
// Assignment Operator
//		- Will set this instance equal to another instance.
// Inputs:
//		- const TCEventDispatcher& inRef: The reference to make equal to.
// Outputs:
//		- TCEventDispatcher&: The newly assigned reference.
//

TCEventDispatcher& TCEventDispatcher::operator=( const TCEventDispatcher& inRef )
{
	CleanUp();
	Clone( inRef );

	return *this;
}

//
// Destructor
//		- Will release all resources associated with object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCEventDispatcher::~TCEventDispatcher()
{
	CleanUp();
}

//
// FireEvent
//		- Will trigger an event and notify the listeners.
// Inputs:
//		- TCEventID eventID: The event that has triggered.
//		- void* eventData: The data that goes along with the event.
// Outputs:
//		- TCResult result: The result of the operation.
//			- Failure_InvalidParameter: The event is not contained or is less than zero.
//			- Success: The operation finished successfully.
//

TCResult TCEventDispatcher::FireEvent( TCEventID eventID, void* eventData )
{
	//
	// Make sure the event is > 0.
	//

	if( eventID < 0 )
	{
		gLogger->LogWarning("Tried to fire an event that was less than zero.");
		return Failure_InvalidParameter;
	}

	//
	// Notify all listeners.
	//

	for( int currentListener = 0; currentListener < mEventListeners.Count(); ++currentListener )
	{
		if( mEventListeners[ currentListener ] != NULL )
		{
			mEventListeners[ currentListener ]->OnEventFired( eventID, eventData );
		}
	}

	return Success;
}

//
// AddListener
//		- Will add a new listener as a subscriber to a particular event.
// Inputs:
//		- TCEventListener* listener: The listener that will be the subscriber.
// Outputs:
//		- TCResult: The result of the operation.
//			- Failure_InvalidParameter: The event is not managed by this dispatcher, or the listener is NULL.
//			- Success: The operation succeeded.
//

TCResult TCEventDispatcher::AddListener( TCEventListener* listener )
{
	//
	// Make sure the listener is valid.
	//

	if( listener == NULL )
	{
		gLogger->LogWarning("Tried to subscribe to an event with a NULL listener.");
		return Failure_InvalidParameter;
	}

	mEventListeners.Append( listener );
	return Success;
}

//
// RemoveListener
//		- Will remove a listener from being subscribed to a particular event.
// Inputs:
//		- TCEventListener* listener: The listener to unsubscribe.
// Outputs:
//		- TCResult: The result of the operation.
//			- Failure_InvalidParameter: Returned if the eventID <0, or is not managed by this dispatcher. Or the listener was NULL.
//			- Success: The operation completed successfully.
//

TCResult TCEventDispatcher::RemoveListener( TCEventListener* listener )
{
	//
	// Make sure the listener is valid.
	//

	if( listener == NULL )
	{
		gLogger->LogWarning("Tried to subscribe to an event with a NULL listener.");
		return Failure_InvalidParameter;
	}

	//
	// Remove the listener.
	//

	mEventListeners.Remove( listener );
	return Success;
}

//
// CleanUp
//		- Will clean up all listeners/events.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCEventDispatcher::CleanUp()
{
	TCList< TCEventListener* > listenersToRemove = mEventListeners; 
	for( int currentListener = 0; currentListener < listenersToRemove.Count(); ++currentListener )
	{
		if( listenersToRemove[ currentListener ] != NULL )
		{
			listenersToRemove[ currentListener ]->UnsubscribeFrom( this );
		}
	}

	listenersToRemove.Clear();
	mEventListeners.Clear();
}

//
// Clone
//		- Will copy another TCEventDispatcher&
// Inputs:
//		- TCEventDispatcher& inRef: The dispatcher to copy.
// Outputs:
//		- None.
//

void TCEventDispatcher::Clone( const TCEventDispatcher& inRef )
{
	mEventListeners = inRef.mEventListeners;
}