//
// TCMouseInput.win32.cpp
// This file will define the functionality for a mouse in Windows.
//

//
// Includes
//

#include "TCMouseInput.win32.h"
#include "TCLogger.h"
#include "TCSystemEvents.h"

//
// Defines
//

//
// Default Constructor
//		- Will intialize this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCMouseInput_Win32::TCMouseInput_Win32()
{

}

//
// Copy Constructor
//		- Will initialize this object as a copy of another mouse.
// Inputs:
//		- const TCMouseInput_Win32& inRef: The reference to copy.
// Outptus:
//		- None.
//

TCMouseInput_Win32::TCMouseInput_Win32( const TCMouseInput_Win32& inRef )
{
	Clone( inRef );
}

//
// Assignment Operator
//		- Will set this object to be the same as another TCMouseInput_Win32&
// Inputs:
//		- const TCMouseInput_Win32& inRef: The reference to set ourselves to.
// Outputs:
//		- TCMouseInput_Win32&: A reference to this object.
//

TCMouseInput_Win32& TCMouseInput_Win32::operator=( const TCMouseInput_Win32& inRef )
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

TCMouseInput_Win32::~TCMouseInput_Win32()
{
	Destroy();
}

//
// Initialize
//		- Will create all resources associated with the object.
// Inputs:
//		- TCMouseInput_Win32::Description& desc: The information necessary to initialize this object.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The object initialized successfully.
//			- Failure_InvalidParameter: The window provided was NULL.
//

TCResult TCMouseInput_Win32::Initialize( TCMouseInput_Win32::Description& desc )
{
	//
	// Test if the description is valid.
	//

	TCMouseInput_Win32::Description_Win32& platformDesc = (TCMouseInput_Win32::Description_Win32&)desc;
	if( IsValidDescription( platformDesc ) == false )
	{
		gLogger->LogError( "[TCMouseInput_Win32] Failed to initialize the mouse! Invalid description!" );
		return Failure_InvalidParameter;
	}

	//
	// Register this mouse as a listener to the window.
	//

	SubscribeTo( platformDesc.mWindow );

	//
	// Perform base initialization.
	//

	return TCMouseInput::Initialize( desc );
}

//
// Update
//		- This method will perform the frame to frame update of this object.
// Inputs:
//		- float deltaTime: The time since the last frame.
// Outputs:
//		- None.
//

void TCMouseInput_Win32::Update( float deltaTime )
{
	TCMouseInput::Update( deltaTime );
}

//
// OnEventFired
//		- Will be called whenever an event is fired.
// Inputs:
//		- TCEventID eventID: The event that was fired.
//		- void* eventData: The data regarding the event that was fired.
// Outputs:
//		- None.
//

void TCMouseInput_Win32::OnEventFired( TCEventID eventID, void* eventData )
{
	switch( eventID )
	{
		case SysEvent_WindowEvent_MouseButtonDown:
		{
			HandleMouseButtonDownEvent( (TCWindow_Win32::MouseButtonDown*)eventData );
			break;
		}

		case SysEvent_WindowEvent_MouseButtonUp:
		{
			HandleMouseButtonUpEvent( (TCWindow_Win32::MouseButtonUp*)eventData );
			break;
		}

		case SysEvent_WindowEvent_MouseMovement:
		{
			HandleMouseMoveEvent( (TCWindow_Win32::MouseMove*)eventData );
			break;
		}

		case SysEvent_WindowEvent_MouseWheelMovement:
		{
			HandleMouseWheelMoveEvent( (TCWindow_Win32::MouseWheelMove*)eventData );
			break;
		}
	}
}

//
// Destroy
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCMouseInput_Win32::Destroy()
{
	for( int currentWindow = 0; currentWindow < mRegisteredWindows.Count(); ++currentWindow )
	{
		if( mRegisteredWindows[ currentWindow ] != NULL )
		{
			mRegisteredWindows[ currentWindow ]->RemoveListener( this );
		}
	}
	mRegisteredWindows.Clear();

	//
	// Call the base destroy.
	//

	TCMouseInput::Destroy();
}

//
// AddWindow
//		- Will add a new window that this mouse can affect.
// Inputs:
//		- TCWindow_Win32* window: The window to add.
// Outputs:
//		- None.
//

void TCMouseInput_Win32::AddWindow( TCWindow_Win32* window )
{
	if( window == NULL || mRegisteredWindows.Contains( window ) )
		return;

	SubscribeTo( window );
	mRegisteredWindows.Append( window );
}

//
// RemoveWindow
//		- Will remove a window that this mouse can affect.
// Inputs:
//		- TCWindow_Win32* window: The window to remove.
// Outputs:
//		- None.
//

void TCMouseInput_Win32::RemoveWindow( TCWindow_Win32* window )
{
	if( window == NULL || !mRegisteredWindows.Contains( window ) )
		return;

	UnsubscribeFrom( window );
	mRegisteredWindows.Remove( window );
}

//
// IsValidDescription
//		- Will determine if the description provided is valid.
// Inputs:
//		- TCMouseInput_Win32::Description_Win32& desc: The description.
// Outputs:
//		- bool: Was the description valid.
//

bool TCMouseInput_Win32::IsValidDescription( TCMouseInput_Win32::Description_Win32& desc )
{
	if( desc.mWindow == NULL )
		return false;

	return true;
}

//
// Clone
//		- Will copy another TCMouseInput_Win32 into this reference.
// Inputs:
//		- const TCMouseInput_Win32& inRef: The mouse to copy.
// Outputs:
//		- None.
//

void TCMouseInput_Win32::Clone( const TCMouseInput_Win32& inRef )
{
	mRegisteredWindows = inRef.mRegisteredWindows;
	TCMouseInput::Clone( inRef );
}

//
// HandleMouseButtonDownEvent
//		- Will process a system event of when a mouse down event has happened.
// Inputs:
//		- TCWindow_Win32::MouseButtonDown* downEvent: The event data for a mouse button down event.
// Outputs:
//		- None.
//

void TCMouseInput_Win32::HandleMouseButtonDownEvent( TCWindow_Win32::MouseButtonDown* downEvent )
{
	if( downEvent == NULL )
		return;

	//
	// Capture the mouse event so that we make sure we don't miss any events if they leave the window.
	//

	TCWindow_Win32* window = (TCWindow_Win32*)downEvent->window;
	if( window )
	{
		SetCapture( window->GetHWindow() );	
	}

	//
	// Determine what button and then send then process it at the platform agnostic layer.
	//

	switch( downEvent->button )
	{
		case TCWindow_Win32::LeftButton:
		{
			OnButtonDown( LeftButton );
			break;
		}

		case TCWindow_Win32::RightButton:
		{
			OnButtonDown( RightButton );
			break;
		}

		case TCWindow_Win32::MiddleButton:
		{
			OnButtonDown( MiddleButton );
			break;
		}

		default:
		{
			gLogger->LogWarning( "[TCMouseInput] Handling an unknown mouse button down event.");
			break;
		}
	}
}

//
// HandleMouseButtonUpEvent
//		- This function will handle the translation from the window event to the input event.
// Inputs:
//		- TCWindow_Win32::MouseButtonUpEvent* upEvent: The information about the button up event.
// Outputs:
//		- None.
//

void TCMouseInput_Win32::HandleMouseButtonUpEvent( TCWindow_Win32::MouseButtonUp* upEvent )
{
	if( upEvent == NULL )
		return;

	//
	// Release the mouse.
	//

	SetCapture( NULL );

	//
	// Translate the window event to the input event.
	//

	switch( upEvent->button )
	{
		case TCWindow_Win32::LeftButton:
		{
			OnButtonUp( LeftButton );
			break;
		}

		case TCWindow_Win32::MiddleButton:
		{
			OnButtonUp( MiddleButton );
			break;
		}

		case TCWindow_Win32::RightButton:
		{
			OnButtonUp( RightButton );
			break;
		}

		default:
		{
			gLogger->LogWarning( "[TCMouseInput] Processing an unknown mouse up event type.");
			break;
		}
	}
}

//
// HandleMouseMoveEvent
//		- Will handle the translation from the platform event to a system input event.
// Inputs:
//		- TCWindow_Win32::MouseMoveEvent* moveEvent: The information about the movement.
// Outputs:
//		- None.
//

void TCMouseInput_Win32::HandleMouseMoveEvent( TCWindow_Win32::MouseMove* moveEvent )
{
	if( moveEvent == NULL )
		return;

	OnMove( moveEvent->relativePosition, moveEvent->absolutePosition );
}

//
// HandleMouseWheelMoveEvent
//		- Will handle the platform event translating it to a system input event.
// Inputs:
//		- TCWindow_Win32::MouseWheelMoveEvent* wheelMoveEvent: The information about the movement.
// Outputs:
//		- None.
//

void TCMouseInput_Win32::HandleMouseWheelMoveEvent( TCWindow_Win32::MouseWheelMove* wheelMoveEvent )
{
	if( wheelMoveEvent == NULL )
		return;

	OnWheelScroll( wheelMoveEvent->delta );
}