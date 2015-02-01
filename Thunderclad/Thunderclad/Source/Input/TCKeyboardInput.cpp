//
// TCKeyboardInput.cpp
// This file will define the functionality for platform agnostic keyboard input.
//

//
// Includes
//

#include "TCKeyboardInput.h"
#include "TCSystemEvents.h"

//
// Defines
//

//
// Default Constructor
//		- Will initialize this object's members to a safe state.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCKeyboardInput::TCKeyboardInput()
{
	for( int currentKey = 0; currentKey < TC_MAX_KEYCODES; ++currentKey )
	{
		mKeyStates[ currentKey ] = false;
	}
}

//
// Copy Constructor
//		- Will copy another instance
// Inputs:
//		- const TCKeyboardInput& inRef: The reference to copy.
// Outputs:
//		- None.
//

TCKeyboardInput::TCKeyboardInput( const TCKeyboardInput& inRef )
{
	Clone( inRef );
}

//
// Assignment Operator
//		- Will set this instance equal to another.
// Inputs:
//		- const TCKeyboardInput& inRef: The reference to copy.
// Outputs:
//		- TCKeyboardInput&: The reference to this object.
//

TCKeyboardInput& TCKeyboardInput::operator=( const TCKeyboardInput& inRef )
{
	Destroy();
	Clone( inRef );

	return (*this);
}

//
// Destructor
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCKeyboardInput::~TCKeyboardInput()
{
	Destroy();
}

//
// Initialize
//		- Will initialize all resources associated with this object.
// Inputs:
//		- Description& desc: The information needed to initialize this object.
// Outputs:
//		- TCResult: The result of the operator.
//			- Success: The operation succeeded.
//

TCResult TCKeyboardInput::Initialize( TCKeyboardInput::Description& desc )
{
	return Success;
}

//
// Update
//		- Will perform the per-frame updates necessary for this object.
// Inputs:
//		- float deltaTime: The time since the last frame.
// Outputs:
//		- None.
//

void TCKeyboardInput::Update( float deltaTime )
{
	
}

//
// Destroy
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCKeyboardInput::Destroy()
{
	TCEventDispatcher::CleanUp();
	TCEventListener::UnsubscribeFromAll();
}

//
// OnEventFired
//		- Will be called whenever a dispatcher this object is subscribed to fires an event.
// Inputs:
//		- TCEventID eventID: The event that was fired.
//		- void* eventData: The data associated with the event.
// Outputs:
//		- None.
//

void TCKeyboardInput::OnEventFired( TCEventID eventID, void* eventData )
{
	
}

//
// Clone
//		- Will be called to copy another instance.
// Inputs:
//		- const TCKeyboardInput& inRef
// Outputs:
//		- None.
//

void TCKeyboardInput::Clone( const TCKeyboardInput& inRef )
{
	TCEventDispatcher::Clone( inRef );
	TCEventListener::Clone( inRef );

	for( int currentKey = 0; currentKey < TC_MAX_KEYCODES; ++currentKey )
	{
		mKeyStates[ currentKey ] = inRef.mKeyStates[ currentKey ];
	}
}

//
// OnKeyDown
//		- Will be triggered when a key is down.
// Inputs:
//		- TCKeyCode keyCode: The keyCode that was pressed.
// Outputs:
//		- None.
//

void TCKeyboardInput::OnKeyDown( TCKeyCode keyCode )
{
	//
	// Update the key state.
	//

	if( mKeyStates[ keyCode ] == false )
	{
		OnKeyClicked( keyCode );
	}
	mKeyStates[ keyCode ] = true;

	//
	// Send off the event.
	//

	KeyDownEvent downEvent;
	downEvent.keyCode = keyCode;
	downEvent.keyboard = this;

	FireEvent( SysEvent_KeyboardEvent_OnKeyDown, &downEvent );
}

//
// OnKeyClicked
//		- Will be triggered when a key is clicked.
// Inputs:
//		- TCKeyCode keyCode: The key that was clicked.
// Outputs:
//		- None.
//

void TCKeyboardInput::OnKeyClicked( TCKeyCode keyCode )
{
	KeyClickedEvent clickEvent;
	clickEvent.keyCode = keyCode;
	clickEvent.keyboard = this;

	FireEvent( SysEvent_KeyboardEvent_OnKeyClicked, &clickEvent );
}

//
// OnKeyUp
//		- Will be triggered when a key is released.
// Inputs:
//		- TCKeyCode keyCode: The key that was released.
// Outputs:
//		- None.
//

void TCKeyboardInput::OnKeyUp( TCKeyCode keyCode )
{
	mKeyStates[ keyCode ] = false;

	KeyUpEvent upEvent;
	upEvent.keyCode = keyCode;
	upEvent.keyboard = this;

	FireEvent( SysEvent_KeyboardEvent_OnKeyUp, &upEvent );
}