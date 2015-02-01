//
// TCKeyboardInput.h
// This file will handle the platform agnostic layer for a keyboard.
//

#ifndef __TC_KEYBOARD_INPUT_H__
#define __TC_KEYBOARD_INPUT_H__

//
// Includes
//

#include "TCKeyCodes.h"
#include "TCResultCode.h"
#include "TCEventDispatcher.h"
#include "TCEventListener.h"

//
// Defines
//

#define TC_MAX_KEYCODES (256)

//
// Forward Declarations
//

//
// Class Declaration
//

class TCKeyboardInput :
	public TCEventDispatcher,
	public TCEventListener
{
	public:		// Members
		struct Description
		{
		};

		struct KeyDownEvent
		{
			TCKeyCode keyCode;
			TCKeyboardInput* keyboard;
		};

		struct KeyClickedEvent
		{
			TCKeyCode keyCode;
			TCKeyboardInput* keyboard;
		};

		struct KeyUpEvent
		{
			TCKeyCode keyCode;
			TCKeyboardInput* keyboard;
		};

	public:		// Methods
									TCKeyboardInput();
									TCKeyboardInput( const TCKeyboardInput& inRef );
		virtual TCKeyboardInput&	operator=( const TCKeyboardInput& inRef );
									~TCKeyboardInput();

		virtual TCResult			Initialize( Description& desc );
		virtual void				Update( float deltaTime );
		virtual void				Destroy();

		virtual void				OnEventFired( TCEventID eventID, void* eventData );

		inline bool					IsKeyDown( TCKeyCode keyCode )						{ return mKeyStates[ keyCode ]; }
		inline bool					IsKeyUp( TCKeyCode keyCode )						{ return !mKeyStates[ keyCode ]; }

	protected:	// Members
		bool mKeyStates[ TC_MAX_KEYCODES ];

	protected:	// Methods
		virtual void				Clone( const TCKeyboardInput& inRef );

		virtual void				OnKeyDown( TCKeyCode keyCode );
		virtual void				OnKeyClicked( TCKeyCode keyCode );
		virtual void				OnKeyUp( TCKeyCode keyCode );
};

#endif // __TC_KEYBOARD_INPUT_H__