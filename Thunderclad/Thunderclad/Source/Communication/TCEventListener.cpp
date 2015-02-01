//
// TCEventListener.cpp
// This file will define the methods to listen to an event.
//

//
// Includes
//

#include "TCEventListener.h"
#include "TCEventDispatcher.h"
#include "TCLogger.h"
#include "TCResultCode.h"

//
// Defines
//

//
// Default Constructor
//		- Will create the event listener and provide the event dispatcher to listen to.
// Inputs:
//		- None
// Outputs:
//		- None.
//

TCEventListener::TCEventListener()
{
}

//
// Copy Constructor
//		- Will create a fresh instance of a TCEventListener copied from another reference.
// Inputs:
//		- const TCEventListener& inRef: The reference to copy from.
// Outputs:
//		- None.
//

TCEventListener::TCEventListener( const TCEventListener& inRef )
{
	Clone( inRef );
}

//
// Assignment Operator
//		- Will set this instance equal to another instance.
// Inputs:
//		- const TCEventListener& inRef: The reference to set to.
// Outputs:
//		- TCEventListener& The reference to this object.
//

TCEventListener& TCEventListener::operator=( const TCEventListener& inRef )
{
	UnsubscribeFromAll();
	Clone( inRef );

	return *this;
}

//
// Destructor
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCEventListener::~TCEventListener()
{
	UnsubscribeFromAll();
}


//
// SubscribeTo
//		- Will subscribe this object to listen to a dispatcher.
// Inputs:
//		- TCEventDispatcher*: The dispatcher to subscribe to.
// Outputs:
//		- None.
//

void TCEventListener::SubscribeTo( TCEventDispatcher* dispatcher )
{
	if( dispatcher != NULL )
	{
		//
		// Check for duplication.
		//

		if( mEventDispatchers.Contains( dispatcher) )
		{
			gLogger->LogWarning( "Tried to subscribe to the same event dispatcher twice:" );
			return;
		}

		//
		// Subscribe to the dispatcher.
		//

		TCResult result = dispatcher->AddListener( this );
		if( TC_FAILED( result ) )
		{
			gLogger->LogError( "Failed to subscribe to an event." );
			return;
		}
		
		mEventDispatchers.Append( dispatcher );
	}
}

//
// UnsubscribeFrom
//		- Will unsubscribe this object from any further events from a specific dispatcher.
// Inputs:
//		- TCEventDispatcher* dispatcher: The event dispatcher to unsubscribe from.
// Outputs:
//		- None.
//

void TCEventListener::UnsubscribeFrom( TCEventDispatcher* dispatcher )
{
	if( dispatcher != NULL && mEventDispatchers.Contains( dispatcher ) )
	{
		TCResult result = dispatcher->RemoveListener( this );
		if( TC_FAILED( result ) )
		{
			gLogger->LogError("Failed to unsubscribe from event");
			return;
		}

		mEventDispatchers.Remove( dispatcher );
	}
}

//
// UnsubscribeFromAll
//		- Will unsubscribe this object from all event dispatchers.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCEventListener::UnsubscribeFromAll()
{
	for( int currentDispatcher = 0; currentDispatcher < mEventDispatchers.Count(); ++currentDispatcher )
	{
		mEventDispatchers[ currentDispatcher ]->RemoveListener( this );
	}
	mEventDispatchers.Clear();
}

//
// Clone
//		- Will copy from another TCEventListener into this one.
// Inputs:
//		- const TCEventListener& inRef.
// Outputs:
//		- None.
//

void TCEventListener::Clone( const TCEventListener& inRef )
{
	mEventDispatchers = inRef.mEventDispatchers;
}