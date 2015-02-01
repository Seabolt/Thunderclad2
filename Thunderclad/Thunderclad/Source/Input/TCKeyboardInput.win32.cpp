//
// TCKeyboardInput.win32.cpp
// This file will define the window specific functionality for a keyboard.
//

//
// Includes
//

#include "TCKeyboardInput.win32.h"
#include "TCLogger.h"
#include "TCWindow.win32.h"
#include "TCSystemEvents.h"

//
// Defines
//

//
// Default Constructor
//		- Will initialize all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCKeyboardInput_Win32::TCKeyboardInput_Win32()
{
	
}

//
// Copy Constructor
//		- Will copy another instance.
// Inputs:
//		- const TCKeyboardInput_Win32& inRef: The reference to copy.
// Outputs:
//		- None.
//

TCKeyboardInput_Win32::TCKeyboardInput_Win32( const TCKeyboardInput_Win32& inRef )
{
	Clone( inRef );
}

//
// Assignment Operator
//		- Will set this reference equal to this another.
// Inputs:
//		- const TCKeyboardInput_Win32& inRef: The reference to set this equal to.
// Outputs:
//		- TCKeyboardInput_Win32&: The reference to this object.
//

TCKeyboardInput_Win32& TCKeyboardInput_Win32::operator=( const TCKeyboardInput_Win32& inRef )
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

TCKeyboardInput_Win32::~TCKeyboardInput_Win32()
{
	Destroy();
}

//
// Initialize
//		- Will initialize all resources associated with this object.
// Inputs:
//		- Description& desc: The description on how to initialize this object.
// Outputs:
//		- TCResult: The result of this operation.
//			- Success: The operation completed successfully.
//			- Failure_InvalidParameter: The description has some invalid parameters.
//

TCResult TCKeyboardInput_Win32::Initialize( TCKeyboardInput::Description& desc )
{
	//
	// Test to see if the description is valid.
	//

	Description_Win32& platformDesc = (Description_Win32&)desc;
	if( IsValidDescription( platformDesc ) == false )
	{
		gLogger->LogError( "[TCKeyboardInput] Failed to initialize the keyboard." );
		return Failure_InvalidParameter;
	}

	//
	// Subscribe to the window.
	//

	SubscribeTo( platformDesc.window );
	return Success;
}

//
// Update
//		- Will perform the per-frame update of this object.
// Inputs:
//		- float deltaTime: The time since the last frame.
// Outputs:
//		- None.
//

void TCKeyboardInput_Win32::Update( float deltaTime )
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

void TCKeyboardInput_Win32::Destroy()
{
	TCKeyboardInput::Destroy();
}

//
// OnEventFired
//		- Will be triggered when a dispatcher that this listener is subscribe to send an event.
// Inputs:
//		- TCEventID eventID: The event that was triggered.
//		- void* eventData: Information regarding the event.
// Outputs:
//		- None.
//

void TCKeyboardInput_Win32::OnEventFired( TCEventID eventID, void* eventData )
{
	switch( eventID )
	{
		case SysEvent_WindowEvent_KeyDown:
		{
			TCWindow_Win32::KeyDown* keyDown = (TCWindow_Win32::KeyDown*)eventData;
			HandlePlatformKeyDown( keyDown->keyCode, keyDown->lParam );
			break;
		}

		case SysEvent_WindowEvent_KeyUp:
		{
			TCWindow_Win32::KeyUp* keyUp = (TCWindow_Win32::KeyUp*)eventData;
			HandlePlatformKeyUp( keyUp->keyCode, keyUp->lParam );
			break;
		}
	}
}

//
// IsValidDescription
//		- Will determine if the description provided is valid
// Inputs:
//		- Description_Win32& description: The description to check.
// Outputs:
//		- bool: Was the description valid.
//

bool TCKeyboardInput_Win32::IsValidDescription( TCKeyboardInput_Win32::Description_Win32& description )
{
	if( description.window == NULL )
		return false;

	return true;
}

//
// Clone
//		- Will copy another instance into this object.
// Inputs:
//		- const TCKeyboardInput_Win32& inRef: The reference to copy.
// Outputs:
//		- None.
//

void TCKeyboardInput_Win32::Clone( const TCKeyboardInput_Win32& inRef )
{
	TCKeyboardInput::Clone( inRef );
}

//
// ConvertPlatformKeyCode
//		- Will convert from a windows code to the engine key code.
// Inputs:
//		- unsigned int keyCode: The key code to convert.
// Outputs:
//		- TCKeyCode: The engine key code.
//

TCKeyCode TCKeyboardInput_Win32::ConvertPlatformKeyCode( unsigned int keyCode, unsigned int lParam )
{
	switch( keyCode )
	{
		case 'A':			return TCKeyCode_A;
		case 'B':			return TCKeyCode_B;
		case 'C':			return TCKeyCode_C;
		case 'D':			return TCKeyCode_D;
		case 'E':			return TCKeyCode_E;
		case 'F':			return TCKeyCode_F;
		case 'G':			return TCKeyCode_G;
		case 'H':			return TCKeyCode_H;
		case 'I':			return TCKeyCode_I;
		case 'J':			return TCKeyCode_J;
		case 'K':			return TCKeyCode_K;
		case 'L':			return TCKeyCode_L;
		case 'M':			return TCKeyCode_M;
		case 'N':			return TCKeyCode_N;
		case 'O':			return TCKeyCode_O;
		case 'P':			return TCKeyCode_P;
		case 'Q':			return TCKeyCode_Q;
		case 'R':			return TCKeyCode_R;
		case 'S':			return TCKeyCode_S;
		case 'T':			return TCKeyCode_T;
		case 'U':			return TCKeyCode_U;
		case 'V':			return TCKeyCode_V;
		case 'W':			return TCKeyCode_W;
		case 'X':			return TCKeyCode_X;
		case 'Y':			return TCKeyCode_Y;
		case 'Z':			return TCKeyCode_Z;
		case '0':			return TCKeyCode_0;
		case '1':			return TCKeyCode_1;
		case '2':			return TCKeyCode_2;
		case '3':			return TCKeyCode_3;
		case '4':			return TCKeyCode_4;
		case '5':			return TCKeyCode_5;
		case '6':			return TCKeyCode_6;
		case '7':			return TCKeyCode_7;
		case '8':			return TCKeyCode_8;
		case '9':			return TCKeyCode_9;
		case VK_NUMPAD0:	return TCKeyCode_Numeric_0;
		case VK_NUMPAD1:	return TCKeyCode_Numeric_1;
		case VK_NUMPAD2:	return TCKeyCode_Numeric_2;
		case VK_NUMPAD3:	return TCKeyCode_Numeric_3;
		case VK_NUMPAD4:	return TCKeyCode_Numeric_4;
		case VK_NUMPAD5:	return TCKeyCode_Numeric_5;
		case VK_NUMPAD6:	return TCKeyCode_Numeric_6;
		case VK_NUMPAD7:	return TCKeyCode_Numeric_7;
		case VK_NUMPAD8:	return TCKeyCode_Numeric_8;
		case VK_NUMPAD9:	return TCKeyCode_Numeric_9;
		case VK_MULTIPLY:	return TCKeyCode_Numeric_Multiply;
		case VK_ADD:		return TCKeyCode_Numeric_Add;
		case VK_DECIMAL:	return TCKeyCode_Numeric_Decimal;
		case VK_DIVIDE:		return TCKeyCode_Numeric_Divide;
		case VK_SUBTRACT:	return TCKeyCode_Numeric_Subtract;
		case VK_F1:			return TCKeyCode_F1;
		case VK_F2:			return TCKeyCode_F2;
		case VK_F3:			return TCKeyCode_F3;
		case VK_F4:			return TCKeyCode_F4;
		case VK_F5:			return TCKeyCode_F5;
		case VK_F6:			return TCKeyCode_F6;
		case VK_F7:			return TCKeyCode_F7;
		case VK_F8:			return TCKeyCode_F8;
		case VK_F9:			return TCKeyCode_F9;
		case VK_F10:		return TCKeyCode_F10;
		case VK_F11:		return TCKeyCode_F11;
		case VK_F12:		return TCKeyCode_F12;
		case VK_F13:		return TCKeyCode_F13;
		case VK_F14:		return TCKeyCode_F14;
		case VK_F15:		return TCKeyCode_F15;
		case VK_BACK:		return TCKeyCode_Backspace;
		case VK_TAB:		return TCKeyCode_Tab;
		case VK_LSHIFT:
		case VK_RSHIFT:		return TCKeyCode_Shift;
		case VK_RCONTROL:
		case VK_LCONTROL:	return TCKeyCode_Control;
		case VK_CAPITAL:	return TCKeyCode_CapsLock;
		case VK_ESCAPE:		return TCKeyCode_Escape;
		case VK_SPACE:		return TCKeyCode_Spacebar;
		case VK_PRIOR:		return TCKeyCode_PageUp;
		case VK_NEXT:		return TCKeyCode_PageDown;
		case VK_END:		return TCKeyCode_End;
		case VK_HOME:		return TCKeyCode_Home;
		case VK_LEFT:		return TCKeyCode_LeftArrow;
		case VK_UP:			return TCKeyCode_UpArrow;
		case VK_RIGHT:		return TCKeyCode_RightArrow;
		case VK_DOWN:		return TCKeyCode_DownArrow;
		case VK_INSERT:		return TCKeyCode_Insert;
		case VK_DELETE:		return TCKeyCode_Delete;
		case VK_NUMLOCK:	return TCKeyCode_NumLock;
		case VK_PAUSE:		return TCKeyCode_Pause_Break;
		case VK_OEM_1:		return TCKeyCode_Semicolon;
		case VK_OEM_PLUS:	return TCKeyCode_Equal; 
		case VK_OEM_MINUS:	return TCKeyCode_Minus;
		case VK_OEM_5:		return TCKeyCode_Backslash;
		case VK_OEM_2:		return TCKeyCode_ForwardSlash;
		case VK_OEM_3:		return TCKeyCode_BackQuote;
		case VK_OEM_7:		return TCKeyCode_QuotationMark;
		case VK_OEM_COMMA:	return TCKeyCode_Comma;
		case VK_OEM_PERIOD:	return TCKeyCode_Period;
		case VK_RETURN:
		{
			if( lParam & 0x10000000 )
			{
				return TCKeyCode_Numeric_Enter;
			}
			else
			{
				return TCKeyCode_Enter;
			}
		}

		default:
			return TCKeyCode_Spacebar;
	}
}

//
// HandlePlatformKeyDown
//		- This function will handle the windows event of when a key goes down.
// Inputs:
//		- unsigned int keyCode: The platform key code.
//		- unsigned int lParam: The information about the key code.
// Outputs:
//		- None.
//

void TCKeyboardInput_Win32::HandlePlatformKeyDown( unsigned int keyCode, unsigned int lParam )
{
	TCKeyCode engineKeyCode = ConvertPlatformKeyCode( keyCode, lParam );
	OnKeyDown( engineKeyCode );
}

//
// HandlePlatformKeyUp
//		- This function will handle the windows event of when a key goes up.
// Inputs:
//		- unsiged int keyCode: The platform key code.
//		- unsigned int lParam: Information about the key code.
// Outputs:
//		- None.
//

void TCKeyboardInput_Win32::HandlePlatformKeyUp( unsigned int keyCode, unsigned int lParam )
{
	TCKeyCode engineKeyCode = ConvertPlatformKeyCode( keyCode, lParam );
	OnKeyUp( engineKeyCode );
}