//
// TCMouseInput.cpp
// This file will define the functionality for a mouse at a platform agnostic level.
//

//
// Includes
//

#include "TCMouseInput.h"
#include "TCSystemEvents.h"

//
// Defines
//

#define TC_DOUBLE_CLICK_TIMER (0.25f)
#define TC_MOUSE_MOVE_THRESHOLD (0.5f)

//
// Default Constructor
//		- This function will set up all information needed to use this mouse.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCMouseInput::TCMouseInput()
{
	mPosition.Set( 0.0f, 0.0f );
	mAbsolutePosition.Set( 0.0f, 0.0f );
	mMoveDelta.Set( 0.0f, 0.0f );
	
	for( int currentButton = 0; currentButton < NumButtons; ++currentButton )
	{
		mButtonStates[ currentButton ] = TCMouseInput::Up;
		mTimeSinceLastClick[ currentButton ] = 0.0f;
	}
}

//
// Copy Constructor
//		- This function will copy from another TCMouseInput
// Inputs:
//		- const TCMouseInput& inRef: The TCMouseInput to copy from.
// Outputs:
//		- None.
//

TCMouseInput::TCMouseInput( const TCMouseInput& inRef )
{
	Clone( inRef );
}

//
// Assignment Operator
//		- This function will copy from another TCMouseInput and return a reference to this object.
// Inputs:
//		- const TCMouseINput& inRef: The TCMouseInput to copy from.
// Outputs:
//		- None.
//

TCMouseInput& TCMouseInput::operator=( const TCMouseInput& inRef )
{
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

TCMouseInput::~TCMouseInput()
{
	Destroy();
}

//
// Initialize
//		- This function will create all resources associated with this object.
// Inputs:
//		- TCWindow::Description& desc: The information necessary to create this mouse.
// Outputs:
//		- TCResult: The result of this operation.
//			- Success: This method at this inheritance level always returns success.
//

TCResult TCMouseInput::Initialize( TCMouseInput::Description& desc )
{
	return Success;
}

//
// Update
//		- This function will perform the frame to frame update for this object.
// Inputs:
//		- float deltaTime: The time between this frame and the last.
// Outputs:
//		- None.
//

void TCMouseInput::Update( float deltaTime )
{
	//
	// Update our click timers.
	//

	UpdateClickTimers( deltaTime );
}

//
// OnEventFired
//		- This function will be called when an event was fired that we are listening to.
// Inputs:
//		- TCEventID eventID: The event that was fired.
//		- void* eventData: The information regarding to the event.
// Outputs:
//		- None.
//

void TCMouseInput::OnEventFired( TCEventID eventID, void* eventData )
{
	
}

//
// SetMousePosition
//		- Will update the mouse position to the new position provided.
// Inputs:
//		- TCPoint2D& newPosition.
// Outputs:
//		- None.
//

void TCMouseInput::SetMousePosition( TCPoint2D& newPosition )
{
	TC_ASSERT( "Not implemented! This should be implemented in the platform layer." && 0 );
}

//
// IsInsideBounds
//		- Will return if this mouse is inside the bounds provided.
// Inputs:
//		- const TCBoundingBox2D& inRef: The bounds to test against.
// Outputs:
//		- bool: Is the mouse inside the bounds.
//

bool TCMouseInput::IsInsideBounds( const TCBoundingBox2D& in )
{
	return in.Contains( mPosition );
}

//
// Destroy
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCMouseInput::Destroy()
{
	TCEventDispatcher::CleanUp();
	TCEventListener::UnsubscribeFromAll();
}

//
// Clone
//		- Will copy from another TCMouseInput.
// Inputs:
//		- const TCMouseInput& inRef: The mouse to copy from.
// Outputs:
//		- None.
//

void TCMouseInput::Clone( const TCMouseInput& inRef )
{
	mPosition = inRef.mPosition;
	mMoveDelta = inRef.mMoveDelta;
	mAbsolutePosition = inRef.mAbsolutePosition;

	for( int currentButton = 0; currentButton < NumButtons; ++currentButton )
	{
		mButtonStates[ currentButton ] = inRef.mButtonStates[ currentButton ];
		mTimeSinceLastClick[ currentButton ] = inRef.mTimeSinceLastClick[ currentButton ];
	}

	TCEventDispatcher::Clone( inRef );
	TCEventListener::Clone( inRef );
}

//
// UpdateClickTimers
//		- Will update all the click timers
// Inputs:
//		- float deltaTime: the time since the last frame.
// Outputs:
//		- None.
//

void TCMouseInput::UpdateClickTimers( float deltaTime )
{
	for( int currentButton = 0; currentButton < NumButtons; ++currentButton )
	{
		mTimeSinceLastClick[ currentButton ] += deltaTime;
	}
}

//
// OnButtonDown
//		- Will trigger when a mouse button goes down.
// Inputs:
//		- TCMouseInput::Button button: the button that went down.
// Outputs:
//		- None.
//

void TCMouseInput::OnButtonDown( TCMouseInput::Button button )
{
	//
	// We consider the first frame that the mouse is down as the "click" event.
	//

	if( mButtonStates[ button ] != Down )
	{
		OnButtonClick( button );
	}

	mButtonStates[ button ] = Down;

	//
	// Fire the event.
	//

	ButtonDownEvent downEvent;
	downEvent.button = button;

	FireEvent( SysEvent_MouseEvent_ButtonDown, &downEvent );
}

//
// OnButtonUp
//		- Will trigger when a mouse button goes up.
// Inputs:
//		- TCMouseInput::Button button: The button that went up.
// Outputs:
//		- None.
//

void TCMouseInput::OnButtonUp( TCMouseInput::Button button )
{
	mButtonStates[ button ] = Up;

	//
	// Fire the event.
	//

	ButtonUpEvent upEvent;
	upEvent.button = button;

	FireEvent( SysEvent_MouseEvent_ButtonUp, &upEvent );
}

//
// OnButtonClick
//		- Will trigger when a mouse button is clicked
// Inputs:
//		- TCMouseInput::Button button: The button that was clicked.
// Outputs:
//		- None.
// 

void TCMouseInput::OnButtonClick( TCMouseInput::Button button )
{
	//
	// Check to see if this is a double click.
	//

	if( IsDoubleClick( button ) )
	{
		OnButtonDoubleClick( button );
	}
	else // We only send the click event if it's not the double click event.
	{
		ButtonClickEvent clickEvent;
		clickEvent.button = button;

		FireEvent( SysEvent_MouseEvent_ButtonClicked, &clickEvent );
	}

	//
	// Clear the last click timer.
	//

	mTimeSinceLastClick[ button ] = 0.0f;
}

//
// OnButtonDoubleClick
//		- Will trigger when a mouse button is double clicked
// Inputs:
//		- TCMouseInput::Button: The button that was double clicked.
// Outputs:
//		- None.
//

void TCMouseInput::OnButtonDoubleClick( TCMouseInput::Button button )
{
	ButtonDoubleClickEvent doubleClickEvent;
	doubleClickEvent.button = button;

	FireEvent( SysEvent_MouseEvent_ButtonDoubleClicked );
}

//
// OnMove
//		- Will trigger when the mouse has been detected as having moved.
// Inputs:
//		- TCPoint2D& newPosition: The position to set the mouse to.
//		- TCPoint2D& newAbsolutePosition: The new absolute position of the mouse.
// Outputs:
//		- None.
//

void TCMouseInput::OnMove( TCPoint2D& newPosition, TCPoint2D& newAbsolutePosition )
{
	mMoveDelta.x = newPosition.x - mPosition.x;
	mMoveDelta.y = newPosition.y - mMoveDelta.y;

	mPosition = newPosition;
	mAbsolutePosition = newAbsolutePosition;

	//
	// If the mouse has moved enough, fire the event.
	//

	if( mMoveDelta.x >= TC_MOUSE_MOVE_THRESHOLD &&
		mMoveDelta.y >= TC_MOUSE_MOVE_THRESHOLD )
	{
		MovementEvent moveEvent;
		moveEvent.absolutePos = mAbsolutePosition;
		moveEvent.relativePos = mPosition;
		moveEvent.delta = mMoveDelta;

		FireEvent( SysEvent_MouseEvent_Movement, &moveEvent );
	}
}

//
// OnWheelScroll
//		- Will fire whenever a mouse wheel has scrolled.
// Inputs:
//		- int scrollDelta: The amount scrolled in this frame.
// Outputs:
//		- None.
//

void TCMouseInput::OnWheelScroll( int scrollDelta )
{
	mMouseWheelDelta = (float)scrollDelta;

	WheelMovementEvent wheelMovementEvent;
	wheelMovementEvent.delta = (int)mMouseWheelDelta;

	FireEvent( SysEvent_MouseEvent_WheelMovement, &wheelMovementEvent );
}

//
// IsDoubleClick
//		- Will determine if the time since the last click was small enough to be considered a double click.
// Inputs:
//		- TCMouseInput::Button: The button to check.
// Outputs:
//		- bool: Was the time between clicks small enough to be considered a double click.
//

bool TCMouseInput::IsDoubleClick( TCMouseInput::Button button )
{
	return mTimeSinceLastClick[ button ] <= TC_DOUBLE_CLICK_TIMER;
}