//
// TCMouseInput.win32.h
// This file will define the window implementation of a mouse.
//

#ifndef __TC_MOUSE_INPUT_WIN32_H__
#define __TC_MOUSE_INPUT_WIN32_H__

//
// Includes
//

#include "TCMouseInput.h"
#include "TCWindow.win32.h"
#include "TCList.h"

//
// Defines
//

//
// Forward Declaration
//


//
// Class Declaration
//

class TCMouseInput_Win32 : 
	public TCMouseInput
{
	public:		// Members

		struct Description_Win32 : 
			public TCMouseInput::Description
		{
			TCWindow_Win32* mWindow;
		};

	public:		// Methods
									TCMouseInput_Win32();
									TCMouseInput_Win32( const TCMouseInput_Win32& inRef );
		virtual TCMouseInput_Win32& operator=( const TCMouseInput_Win32& inRef );
		virtual						~TCMouseInput_Win32();

		virtual TCResult			Initialize( Description& desc );
		virtual void				Update( float deltaTime );
		virtual void				OnEventFired( TCEventID eventID, void* eventData );
		virtual void				Destroy();

		virtual void				AddWindow( TCWindow_Win32* window );
		virtual void				RemoveWindow( TCWindow_Win32* window );

	private:	// Members

		TCList< TCWindow_Win32* > mRegisteredWindows;

	private:	// Methods
		bool IsValidDescription( TCMouseInput_Win32::Description_Win32& desc );
		void Clone( const TCMouseInput_Win32& inRef );

		void HandleMouseButtonDownEvent( TCWindow_Win32::MouseButtonDown* downEvent );
		void HandleMouseButtonUpEvent( TCWindow_Win32::MouseButtonUp* upEvent );
		void HandleMouseMoveEvent( TCWindow_Win32::MouseMove* moveEvent );
		void HandleMouseWheelMoveEvent( TCWindow_Win32::MouseWheelMove* wheelMoveEvent );
};

#endif