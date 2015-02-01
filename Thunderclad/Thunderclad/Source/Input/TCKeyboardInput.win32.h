//
// TCKeyboardInput.win32.h
// This file will define the windows implementation of a keyboard.
//

#ifndef __TC_KEYBOARD_INPUT_WIN32_H__
#define __TC_KEYBOARD_INPUT_WIN32_H__

//
// Inputs
//

#include "TCKeyboardInput.h"
#include "TCResultCode.h"

//
// Defines
//

//
// Forward Declaration
//

class TCWindow_Win32;

//
// Class Declaration
//

class TCKeyboardInput_Win32
	: public TCKeyboardInput
{
	public:		// Members
		struct Description_Win32 : Description
		{
			TCWindow_Win32* window;
		};

	public:		// Methods
										TCKeyboardInput_Win32();
										TCKeyboardInput_Win32( const TCKeyboardInput_Win32& inRef );
		virtual TCKeyboardInput_Win32&	operator=( const TCKeyboardInput_Win32& inRef );
		virtual							~TCKeyboardInput_Win32();

		virtual TCResult				Initialize( Description& description );
		virtual void					Update( float deltaTime );
		virtual void					Destroy();

		virtual void					OnEventFired( TCEventID eventID, void* eventData );

	private:	// Members
	private:	// Methods

		virtual	bool					IsValidDescription( Description_Win32& desc );
		virtual void					Clone( const TCKeyboardInput_Win32& inRef );
				TCKeyCode				ConvertPlatformKeyCode( unsigned int keyCode, unsigned int lParam );
				void					HandlePlatformKeyDown( unsigned int keyCode, unsigned int lParam );
				void					HandlePlatformKeyUp( unsigned int keyCode, unsigned int lParam );
};

#endif // __TC_KEYBOARD_INPUT_WIN32_H__