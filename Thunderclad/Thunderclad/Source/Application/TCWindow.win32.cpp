//
// TCWindow.win32.cpp
// This file will define the functionality for a window in the Win32 platform
//

//
// Includes
//

#include "TCWindow.win32.h"
#include "TCLogger.h"
#include "TCSystemEvents.h"

#if TC_PLATFORM_WIN32

//
// Defines
//

#define DEFAULT_APPLICATION_ICON_RESOURCE_ID (32512)

//
// Static Initializers
//

TCList< TCWindow_Win32* > TCWindow_Win32::gWindowList;	// This a list of all windows created so we can look them up based on hWND.
TCWindow_Win32* gWindowBeingInitialized = NULL;			// This is a bit hacky, but we don't actually get our hWND until after the message proc has recieved message.
														// So this is used as a shitty way to catch that edge case.

//
// Default Window Procedure
// Inputs:
//		- HWND hWnd: The handle to the window
//		- UINT msg: the message ID to handle.
//		- WPARAM wParam: Additional information about the message.
//		- LPARAM lParam: Additional information about the message.
// Outputs:
//		- LRESULT: The result of the message.
//

LRESULT CALLBACK DefaultWindowMessagePump( HWND hWnd,
										   UINT message,
										   WPARAM wParam, 
										   LPARAM lParam )
{
	//
	// Get the window that this is for.
	//

	TCWindow_Win32* window = TCWindow_Win32::GetWindowFromHWND( hWnd );
	if( window == NULL )
	{
		//
		// If there is no window being initialized, then this is an error.
		//

		if( gWindowBeingInitialized == NULL )
		{
			gLogger->LogFailure("Recieving a window message for a window that hasn't been added to the window list!");
			return DefWindowProc( hWnd, message, wParam, lParam );
		}
		else
		{
			//
			// There is a bug where the message pump will get messages before they actually return our HWND, so we have no way
			// of catching those first few messages.
			//

			window = gWindowBeingInitialized;
		}
	}

	//
	// Now we can handle the message.
	//
	
	TCResult result = window->HandleWindowMessage( message, wParam, lParam );

	if( result == Success_Unhandled )
	{
		return DefWindowProc( hWnd, message, wParam, lParam );	
	}
	else if( TC_FAILED( result ) )
	{
		gLogger->LogError("Failed to process a window message!");
		return 0;
	}
	else
	{
		return 0;
	}
}

//
// Win32Description Constructor
//		- This function is responsible for initializing a window description
// Inputs:
//		- None
// Outputs:
//		- None
//

TCWindow_Win32::Win32Description::Win32Description()
{
	instance			= NULL;	// No default window instance, this must be provided.
	parentWindow		= NULL;	// No default parent window instance, this can be NULL.
	windowTitle			= _T("DEFAULT_WINDOW_TITLE_REPLACE_ME");
	windowClassName		= _T("DEFAULT_WINDOW_CLASS_NAME");
	showCommand			= 0;
	backgroundBrush		= BLACK_BRUSH;
	winProc				= NULL;
}

//
// TCWindow_Win32 Destructor
//		- This function is responsible for cleaning up all resources used by this object.
// Inputs:
//		- None
// Outputs:
//		- None
//

TCWindow_Win32::~TCWindow_Win32()
{
	Destroy();

	// Remove ourselves from the global list.
	gWindowList.Remove( this );
}

//
// TCWindow_Win32 Constructor
//		This funciton is responsible for initializing it to a safe state.
// Inputs:
//		- None
// Outputs:
//		- None
//

TCWindow_Win32::TCWindow_Win32()
{
	mInstance			= NULL;
	mParentWindow		= NULL;
	mWindow				= NULL;
	mWindowProcedure	= NULL;
	mBackgroundBrush	= NULL;
	mCursor				= NULL;
	mShowCommand		= 0;
	mIcon				= NULL;
	mSmallIcon			= NULL;
	mWindowTitle		= "Unknown Window Name";
	mWindowClassName	= "Unknown Window Class Name";

	// Make sure we add ourselves to the global list.
	gWindowList.Append( this );
}

//
// Initialize()
//		This function is responsible for creating the window and making it safe to use.
// Inputs:
//		- TCWindow_Win32::Win32Description& description: The descriptions structure that contains all valid/necessary information to initialize.
// Outputs:
//		- TCResult: The result of the operation. Possible returns:
//			- TCResult::Failure_InvalidParameter: The description has some invalid parameter. Check the logs for more information.
//			- TCResult::Success: The operation succeeded.
//

TCResult TCWindow_Win32::Initialize( TCWindow_Win32::Win32Description& description )
{
	//
	// First lets make sure that the information is valid.
	//
	
	TCResult result = IsValidDescription( description );
	if( TC_FAILED( result ) )
	{
		gLogger->LogError("[TCWindow] Tried to create a window with a invalid description.");
		return result;
	}

	//
	// Do our base initialize
	//

	result = TCWindow::Initialize( description );
	if( TC_FAILED( result ) )
	{
		gLogger->LogError( "[TCWindow] Failed base initialization." );
		return result;
	}

	//
	// Copy our variables from the description.
	//

	mParentWindow		= description.parentWindow;
	mInstance			= description.instance;
	mShowCommand		= description.showCommand;
	mWindowProcedure	= description.winProc;
	mWindowTitle		= description.windowTitle;
	mWindowClassName	= description.windowClassName;

	//
	// Initialize our icons
	//

	result = InitializeIcons( description.instance, description.iconResource, description.smallIconResource, mIcon, mSmallIcon );
	if( TC_FAILED( result ) )
	{
		gLogger->LogError("[TCWindow] Failed to create icons.");
		return result;
	}

	//
	// Initialize our cursor.
	//

	result = InitializeCursor( description.instance, description.cursorResource, mCursor );
	if( TC_FAILED( result ) )
	{
		gLogger->LogError("[TCWindow] Failed to create cursor icons.");
		return result;
	}

	//
	// Initialize our background brush.
	//

	result = InitializeBrush( description.instance, description.backgroundBrush, mBackgroundBrush );
	if( TC_FAILED( result ) )
	{
		gLogger->LogError("[TCWindow] Failed to create background brush");
		return result;
	}

	//
	// Next we initialize our window class.
	//

	result = InitializeWindowClass( description );
	if( TC_FAILED( result ) )
	{
		gLogger->LogError("[TCWindow] Failed to create the window class");
		return result;
	}

	//
	// Then we initialize our window.
	//

	result = InitializeWindow( description );
	if( TC_FAILED( result ) )
	{
		gLogger->LogError("[TCWindow] Failed to initialize the window.");
		return result;
	}

	//
	// Show the window 
	//

	ShowWindow( mWindow, SW_SHOWNORMAL );

	//
	// Update the window flags.
	//

	UpdateWindow( mWindow );

	//
	// We initialized successfully!
	//

	return TCResult::Success;
}

//
// TCWindow_Win32::Destroy()
//		This function will free all resources used by this window.
// Inputs:
//		- None
// Outputs:
//		- None
//

void TCWindow_Win32::Destroy()
{
	if( mWindow != NULL )
	{
		DestroyWindow( mWindow );
		UnregisterClass( mWindowClassName.Data(), mInstance );
		mWindow = NULL;
	}
}

//
// TCWindow_Win32::IsValidDesription()
//		This function will determine if the description provided is valid
// Inputs:
//		Win32Description& description: The description to check.
// Outputs:
//		TCResult: the result of the operation.
//

TCResult TCWindow_Win32::IsValidDescription( TCWindow_Win32::Win32Description& description )
{
	//
	// Check to see if the description fails our parent checks.
	//

	TCResult result = TCWindow::IsValidDescription( description );
	if( TC_FAILED( result ) )
	{
		return result;
	}

	//
	// Verify that we were given a HINSTANCE handle
	//

	if( description.instance == NULL )
	{
		return TCResult::Failure_InvalidParameter;
	}

	//
	// Return success.
	//

	return TCResult::Success;
}

//
// InitializeIcons
//		This function will initialize the our icon resources
// Inputs:
//		- HINSTANCE instance: The instance of the window.
//		- TCString& resourceName: The name of the primary icon resource
//		- TCString& smallResourceName: The name of the small icon resource
//		- HICON& outIcon: The icon handle to be filled out for the large icon
//		- HICON& outSmallIcon: The icon handle to be filled out the small icon
// Outputs:
//		- HICON& outIcon: The handle of the icon initialize
//		- HICON& outSmallIcon: The handle of the small icon initialize
//		- TCResult: The result of the operation.
//

TCResult TCWindow_Win32::InitializeIcons( HINSTANCE instance, TCString& resourceName, TCString& smallResourceName, HICON& outIcon, HICON& outSmallIcon )
{
	//
	// Initialize the main resource icon
	//

	if( !resourceName.IsEmpty() )
	{
		outIcon = LoadIcon( instance, resourceName.Data() );
	}
	else
	{
		outIcon = LoadIcon( NULL, IDI_APPLICATION);
	}

	//
	// Initialize the small resource icon
	//

	if( !smallResourceName.IsEmpty() )
	{
		outSmallIcon = LoadIcon( instance, smallResourceName.Data() );
	}
	else
	{
		outSmallIcon = LoadIcon( NULL, IDI_APPLICATION );
	}

	//
	// Return our result.
	//

	if( outIcon == NULL || outSmallIcon == NULL )
	{
		return TCResult::Failure_InvalidParameter;
	}
	else
	{
		return TCResult::Success;
	}
}

//
// InitializeCursor
//		This function will initialize a cursor resource
// Inputs:
//		- HINSTANCE instance: The instance of the window.
//		- TCString resourceID: The id of the cursor resource.
//		- HCURSOR& outCursor: The cursor handle to be filled.
// Outputs:
//		- HCURSOR& outCursor: The cursor handle that will be filled with the result of the initialize.
//		- TCResult: The result of the operation.
//

TCResult TCWindow_Win32::InitializeCursor( HINSTANCE instance, TCString& resourceID, HCURSOR& outCursor )
{
	if( !resourceID.IsEmpty() )
	{
		outCursor = LoadCursor( instance, resourceID.Data() );
	}
	else
	{
		outCursor = LoadCursor( NULL, IDC_ARROW );
	}

	//
	// Determine the result.
	//

	if( outCursor == NULL )
	{
		return TCResult::Failure_InvalidParameter;
	}
	else
	{
		return TCResult::Success;
	}
}

//
// InitializeBrush
//		This function will load a new brush from a resource
// Inputs:
//		- HINSTANCE instance: THe instance of the window.
//		- int brushID: The id of the brush to create.
//		- HBRUSH& outBrush: The handle to fill out with the result.
// Outputs:
//		- HBRUSH& outBrush: The handle to the brush that was created.
//		- TCResult: The result of the operation.
//

TCResult TCWindow_Win32::InitializeBrush( HINSTANCE instance, int brushID, HBRUSH& outBrush )
{
	if( brushID != -1 )
	{
		outBrush = (HBRUSH)GetStockObject( brushID );
	}
	else
	{
		outBrush = (HBRUSH)GetStockObject( BLACK_BRUSH );
	}

	//
	// Determine the result
	//

	if( outBrush == NULL )
	{
		return TCResult::Failure_InvalidParameter;
	}
	else 
	{
		return TCResult::Success;
	}
}

//
// InitializeWindowClass
//		This function will initialize the information regarding the type of window we are going to create.
// Inputs:
//		- TCWindow_Win32::Win32Description& description: The description to initialize the class from.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCWindow_Win32::InitializeWindowClass( TCWindow_Win32::Win32Description& description )
{
	//
	// Make sure we have valid icons/brushes/cursors. 
	//

	if( mIcon == NULL ||
		mSmallIcon == NULL ||
		mBackgroundBrush == NULL ||
		mCursor == NULL )
	{
		return TCResult::Failure_InvalidState;
	}

	//
	// Check to see if we need to use the default window proc
	//

	if( mWindowProcedure == NULL )
	{
		mWindowProcedure = DefaultWindowMessagePump;
	}

	//
	// Initialize our class variable. 
	//

	mWindowClass.cbSize			= sizeof( WNDCLASSEX );			// The size of the class we are passing in.
	mWindowClass.style			= CS_HREDRAW | CS_VREDRAW;		// We want to redraw if we resize horizontally and vertically.
	mWindowClass.lpfnWndProc	= mWindowProcedure;				// The window procedure we are going to use for this object.
	mWindowClass.cbClsExtra		= 0;
	mWindowClass.cbWndExtra		= 0;
	mWindowClass.hIcon			= mIcon;						// This is the icon if installed.
	mWindowClass.hIconSm		= mSmallIcon;					// This is the icon shown in corners / pins.
	mWindowClass.hCursor		= mCursor;						// This is the cursor to be shown when within our window's borders.
	mWindowClass.hbrBackground	= mBackgroundBrush;				// This is the clear color for the window.
	mWindowClass.lpszMenuName	= NULL;
	mWindowClass.lpszClassName	= mWindowClassName.Data();		// The name of the class that will be registered.

	//
	// Create the window class.
	//

	RegisterClassEx( &mWindowClass );	// TODO: Determine how to check for success/failure on this operation.
	return TCResult::Success;
}

//
// TCWindow_Win32::InitializeWindow
//		This function will actually create the window.
// Inputs:
//		TCWindow_Win32::Win32Description& description: The description to create the window from.
// Outputs:
//		TCResult: The result of the operation.
//

TCResult TCWindow_Win32::InitializeWindow( TCWindow_Win32::Win32Description& description )
{
	//
	// Copy our relevant information from the description.
	//

	mParentWindow = description.parentWindow;
	mShowCommand = description.showCommand;

	//
	// Create our window.
	//

	gWindowBeingInitialized = this; // This is a hacky way to fix the message pump from getting messages from a window that can't register yet.
	mWindow = CreateWindow( mWindowClassName.Data(),		// The name of the window class we just registered.
							mWindowTitle.Data(),			// The title of the window.
							WS_OVERLAPPEDWINDOW,			// The style for the window.
							(int)description.xPosition,		// The x position of the window.
							(int)description.yPosition,		// The y position of the window.
							(int)description.width,			// The width of the window.
							(int)description.height,		// The height of the window.
							mParentWindow,					// The handle to the parent window, (OK to be NULL)
							NULL,							// The menu handle.
							mInstance,						// The instance of the program.
							NULL);							// The LPARAM, unused.
	gWindowBeingInitialized = NULL;
	//
	// Check success.
	//

	if( mWindow == NULL )
	{
		return TCResult::Failure_InvalidParameter;
	}
	else
	{
		return TCResult::Success;
	}
}

//
// GetWindowFromHWND
//		- Will return the TCWindow_Win32* that has the same HWND as provided, NULL if not found.
// Inputs:
//		- HWND hWnd: The handle to the window to find.
// Outputs:
//		- TCWindow_Win32* window: The window found, NULL if not found.
//

TCWindow_Win32* TCWindow_Win32::GetWindowFromHWND( HWND hWnd )
{
	for( int currentWindow = 0; currentWindow < gWindowList.Count(); ++currentWindow )
	{
		if( gWindowList[ currentWindow ]->GetHWindow() == hWnd )
		{
			return gWindowList[ currentWindow ];
		}
	}

	return NULL;
}

//
// HandleWindowMessage
//		- Will handle a message sent from Windows.
// Inputs:
//		- UINT message: The message ID from Windows.
//		- WPARAM wParam: Information about the message.
//		- LPARAM lParam: Information about the message
// Outputs:
//		- TCResult: The result of the operation:
//			- Success_Handled: The message was handled by the window
//			- Success_Unhandled: The message was processed, but not handled.
//			- Failure_InvalidParameter: The wParam or lParam was malformed.
//

TCResult TCWindow_Win32::HandleWindowMessage( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
    {
		// If the user wants to close the application
		case WM_DESTROY:
			HandleDestroyMessage( wParam, lParam );
			break;

		case WM_LBUTTONDOWN:
			HandleLeftButtonDownMessage( wParam, lParam );
			break;

		case WM_LBUTTONUP:
			HandleLeftButtonUpMessage( wParam, lParam );
			break;

		case WM_RBUTTONDOWN:
			HandleRightButtonDownMessage( wParam, lParam );
			break;

		case WM_RBUTTONUP:
			HandleRightButtonUpMessage( wParam, lParam );
			break;

		case WM_MBUTTONDOWN:
			HandleMiddleButtonDownMessage( wParam, lParam );
			break;

		case WM_MBUTTONUP:
			HandleMiddleButtonUpMessage( wParam, lParam );
			break;

		case WM_MOUSEMOVE:
			HandleMouseMoveMessage( wParam, lParam );
			break;

		case WM_MOUSEWHEEL:
			HandleMouseWheelMoveMessage( wParam, lParam );
			break;

		case WM_KEYDOWN:
			HandleKeyDown( wParam, lParam );
			break;

		case WM_KEYUP:
			HandleKeyUp( wParam, lParam );
			break;

		default:
			// Process the left-over messages
			return Success_Unhandled;
    }

	return Success_Handled;
}

//
// HandleDestroyMessage
//		- Will handle a Windows destroy message and dispatch as necessary.
// Inputs:
//		- WPARAM wParam: Information about the message.
//		- LPARAM lParam: Information about the message.
// Outputs:
//		- TCResult: How did we handle this message:
//			- Success_UnHandled: The message was handled, but should be passed on.
//			- Success_Handled: The message was handled, eat it.
//			- Failure_InvalidParameter: The params are malformed.
//

TCResult TCWindow_Win32::HandleDestroyMessage( WPARAM wParam, LPARAM lParam )
{
	gLogger->LogInfo("[TCWindow] Window destroy message handled!");

	//
	// Fire the destroyed event.
	//

	DestroyEventData eventData;
	eventData.window = this;
	FireEvent( SysEvent_WindowEvent_Destroy, &eventData );

	return Success_Handled;
}

//
// HandleLeftButtonDownMessage
//		- Will handle a left button down Windows message 
// Inputs:
//		- WPARAM wParam: Information about the message.
//		- LPARAM lParam: Information about the message.
// Outputs:
//		- TCResult: How did we handle this message.
//			- Success_Unhandled: The message was handled but should be passed on.
//			- Success_Handled: The message was handled, eat it.
//			- Failure_InvalidParameter: The params are malformed.
//

TCResult TCWindow_Win32::HandleLeftButtonDownMessage( WPARAM wParam, LPARAM lParam )
{
	MouseButtonDown downEvent;
	downEvent.button = LeftButton;
	downEvent.window = this;

	FireEvent( SysEvent_WindowEvent_MouseButtonDown, &downEvent );

	return Success_Handled;
}

//
// HandleMiddleButtonDownMessage
//		- Will handle a middle button down Windows message 
// Inputs:
//		- WPARAM wParam: Information about the message.
//		- LPARAM lParam: Information about the message.
// Outputs:
//		- TCResult: How did we handle this message.
//			- Success_Unhandled: The message was handled but should be passed on.
//			- Success_Handled: The message was handled, eat it.
//			- Failure_InvalidParameter: The params are malformed.
//

TCResult TCWindow_Win32::HandleMiddleButtonDownMessage( WPARAM wParam, LPARAM lParam )
{
	MouseButtonDown downEvent;
	downEvent.button = MiddleButton;
	downEvent.window = this;

	FireEvent( SysEvent_WindowEvent_MouseButtonDown, &downEvent );

	return Success_Handled;
}

//
// HandleRightButtonDownMessage
//		- Will handle a right button down Windows message 
// Inputs:
//		- WPARAM wParam: Information about the message.
//		- LPARAM lParam: Information about the message.
// Outputs:
//		- TCResult: How did we handle this message.
//			- Success_Unhandled: The message was handled but should be passed on.
//			- Success_Handled: The message was handled, eat it.
//			- Failure_InvalidParameter: The params are malformed.
//

TCResult TCWindow_Win32::HandleRightButtonDownMessage( WPARAM wParam, LPARAM lParam )
{
	MouseButtonDown downEvent;
	downEvent.button = RightButton;
	downEvent.window = this;

	FireEvent( SysEvent_WindowEvent_MouseButtonDown, &downEvent );

	return Success_Handled;
}

//
// HandleLeftButtonUpMessage
//		- Will handle a left button up Windows message 
// Inputs:
//		- WPARAM wParam: Information about the message.
//		- LPARAM lParam: Information about the message.
// Outputs:
//		- TCResult: How did we handle this message.
//			- Success_Unhandled: The message was handled but should be passed on.
//			- Success_Handled: The message was handled, eat it.
//			- Failure_InvalidParameter: The params are malformed.
//

TCResult TCWindow_Win32::HandleLeftButtonUpMessage( WPARAM wParam, LPARAM lParam )
{
	MouseButtonUp upEvent;
	upEvent.button = LeftButton;
	upEvent.window = this;

	FireEvent( SysEvent_WindowEvent_MouseButtonUp, &upEvent );

	return Success_Handled;
}

//
// HandleMiddleButtonUpMessage
//		- Will handle a middle button up Windows message 
// Inputs:
//		- WPARAM wParam: Information about the message.
//		- LPARAM lParam: Information about the message.
// Outputs:
//		- TCResult: How did we handle this message.
//			- Success_Unhandled: The message was handled but should be passed on.
//			- Success_Handled: The message was handled, eat it.
//			- Failure_InvalidParameter: The params are malformed.
//

TCResult TCWindow_Win32::HandleMiddleButtonUpMessage( WPARAM wParam, LPARAM lParam )
{
	MouseButtonUp upEvent;
	upEvent.button = MiddleButton;
	upEvent.window = this;

	FireEvent( SysEvent_WindowEvent_MouseButtonUp, &upEvent );

	return Success_Handled;
}

//
// HandleRightButtonUpMessage
//		- Will handle a right button up Windows message 
// Inputs:
//		- WPARAM wParam: Information about the message.
//		- LPARAM lParam: Information about the message.
// Outputs:
//		- TCResult: How did we handle this message.
//			- Success_Unhandled: The message was handled but should be passed on.
//			- Success_Handled: The message was handled, eat it.
//			- Failure_InvalidParameter: The params are malformed.
//

TCResult TCWindow_Win32::HandleRightButtonUpMessage( WPARAM wParam, LPARAM lParam )
{
	MouseButtonUp upEvent;
	upEvent.button = RightButton;
	upEvent.window = this;

	FireEvent( SysEvent_WindowEvent_MouseButtonUp, &upEvent );

	return Success_Handled;
}

//
// HandleMouseMoveMessage
//		- Will handle a mouse movement Windows message
// Inputs:
//		- WPARAM wParam: Information about the message.
//		- LPARAM lParam: Information about the message.
// Outputs:
//		- TCResult: How did we handle this message.
//			- Success_Unhandled: the message was handled but should be passed on.
//			- Success_Handled: The message was handled, eat it.
//			- Failure_InvalidParameter: The params are malformed.
//

TCResult TCWindow_Win32::HandleMouseMoveMessage( WPARAM wParam, LPARAM lParam )
{
	MouseMove moveEvent;
	moveEvent.window = this;

	//
	// Get the position of the cursor.
	//

	POINT pos;
	BOOL result = GetCursorPos( &pos );
	if( result == FALSE )
	{
		gLogger->LogError("[TCWindow] Failed to get mouse position");
		return Failure_Unknown;
	}

	moveEvent.relativePosition.x = (float)pos.x;
	moveEvent.relativePosition.y = (float)pos.y;

	//
	// Now we need to get the absolute position of the mouse.
	//

	result = ScreenToClient( mWindow, &pos );
	if( result == FALSE )
	{
		gLogger->LogError("[TCWindow] Failed to get the mouse position in absolute space.");
		return Failure_Unknown;
	}

	moveEvent.absolutePosition.x = (float)pos.x;
	moveEvent.absolutePosition.y = (float)pos.y;

	//
	// Send off the event.
	//

	FireEvent( SysEvent_WindowEvent_MouseMovement, &moveEvent );

	return Success_Handled;
}

//
// HandleMouseWheelMessage
//		- Will handle a mouse wheel movement Windows message.
// Inputs:
//		- WPARAM wParam: Information about the message.
//		- LPARAM lParam: Information about the message.
// Outputs:
//		- TCResult: How did we handle this message.
//			- Success_Unhandled: The message was handled but should be passed on.
//			- Success_Handled: The message was handled, eat it.
//			- Failure_InvalidParameter: The params are malformed.
//

TCResult TCWindow_Win32::HandleMouseWheelMoveMessage( WPARAM wParam, LPARAM lParam )
{
	MouseWheelMove wheelMoveEvent;
	wheelMoveEvent.window = this;
	wheelMoveEvent.delta = GET_WHEEL_DELTA_WPARAM( wParam );

	FireEvent( SysEvent_WindowEvent_MouseWheelMovement, &wheelMoveEvent );

	return Success_Handled;
}

//
// HandleKeyDown
//		- Will handle a key down message.
// Inputs:
//		- WPARAM wParam: Information about the message.
//		- LPARAM lParam: Information about the message.
// Outputs:
//		- TCResult: How did we handle this message.
//			- Success_Unhandled: The message was handled but should be passed on.
//			- Success_Handled: The message was handled, eat it.
//			- Failure_InvalidParameter: The params are malformed.
//

TCResult TCWindow_Win32::HandleKeyDown( WPARAM wParam, LPARAM lParam )
{
	KeyDown keyDownEvent;
	keyDownEvent.window = this;
	keyDownEvent.lParam = lParam;
	keyDownEvent.keyCode = wParam;

	FireEvent( SysEvent_WindowEvent_KeyDown, &keyDownEvent );
	return Success_Handled;
}

//
// HandleKeyUp
//		- Will handle a key up message.
// Inputs:
//		- WPARAM wParam: Information about the message.
//		- LPARAM lParam: Information about the message.
// Outputs:
//		- TCResult: How did we handle this message.
//			- Success_Unhandled: The message was handled but should be passed on.
//			- Success_Handled: The message was handled, eat it.
//			- Failure_InvalidParameter: The params are malformed.
//

TCResult TCWindow_Win32::HandleKeyUp( WPARAM wParam, LPARAM lParam )
{
	KeyUp keyUpEvent;
	keyUpEvent.window = this;
	keyUpEvent.lParam = lParam;
	keyUpEvent.keyCode = wParam;

	FireEvent( SysEvent_WindowEvent_KeyUp, &keyUpEvent );
	return Success_Handled;
}

#endif