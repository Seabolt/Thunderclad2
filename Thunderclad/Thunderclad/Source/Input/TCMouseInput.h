//
// TCMouseInput.h
// This file will define the platform agnoistic interface for a mouse.
//

#ifndef __TC_MOUSE_INPUT_H__
#define __TC_MOUSE_INPUT_H__

//
// Includes
//

#include "TCVector2D.h"
#include "TCBoundingBox2D.h"
#include "TCResultCode.h"
#include "TCEventDispatcher.h"
#include "TCEventListener.h"

//
// Defines
//

//
// Forward Declaration
//

class TCWindow;

//
// Class Declaration
//

class TCMouseInput : 
	public TCEventDispatcher,
	public TCEventListener
{
	public:		// Members
		enum Button
		{
			LeftButton = 0,
			MiddleButton,
			RightButton,
			NumButtons
		};

		enum ButtonState
		{
			Down,
			Up,
			NumStates
		};

		struct Description
		{
		};

		struct ButtonDownEvent
		{
			Button button;
		};

		struct ButtonUpEvent
		{
			Button button;
		};

		struct ButtonClickEvent
		{
			Button button;
		};

		struct ButtonDoubleClickEvent
		{
			Button button;
		};

		struct MovementEvent
		{
			TCVector2D delta;
			TCPoint2D relativePos;
			TCPoint2D absolutePos;
		};

		struct WheelMovementEvent
		{
			int delta;
		};

	public:		// Methods

								TCMouseInput();
								TCMouseInput( const TCMouseInput& inRef );
		virtual TCMouseInput&	operator=( const TCMouseInput& inRef );

		virtual					~TCMouseInput();

		virtual TCResult		Initialize( Description& desc );
		virtual void			Update( float deltaTime );
		virtual void			OnEventFired( TCEventID eventID, void* eventData );

				bool			IsButtonDown( Button button )				{ return mButtonStates[ button ] == Down; }
				bool			IsButtonUp( Button button )					{ return mButtonStates[ button ] == Up; }
				ButtonState		GetButtonState( Button button )				{ return mButtonStates[ button ]; }

				float			GetMousePositionX()							{ return mPosition.x; }
				float			GetMousePositionY()							{ return mPosition.y; }
				void			GetMousePosition( TCPoint2D& in )			{ in = mPosition; }

				float			GetAbsoluteMousePositionX()					{ return mAbsolutePosition.x; }
				float			GetAbsoluteMousePositionY()					{ return mAbsolutePosition.y; }
				void			GetAbsoluteMousePosition( TCPoint2D& in )	{ in = mAbsolutePosition; }

				float			GetMouseMoveDeltaX()						{ return mMoveDelta.x; }
				float			GetMouseMoveDeltaY()						{ return mMoveDelta.y; }
				void			GetMouseMoveDelta( TCVector2D& in )			{ in = mMoveDelta; }

		virtual	void			SetMousePosition( TCPoint2D& in );

				bool			IsInsideBounds( const TCBoundingBox2D& in );

		virtual void			Destroy();

	protected:	// Members
		TCPoint2D	mPosition;
		TCPoint2D	mAbsolutePosition;
		TCVector2D	mMoveDelta;
		float		mMouseWheelDelta;
		ButtonState mButtonStates[ NumButtons ];
		float		mTimeSinceLastClick[ NumButtons ];

	protected:	// Methods
		void Clone( const TCMouseInput& inRef );
		void UpdateClickTimers( float deltaTime );

		void OnButtonDown( Button button );
		void OnButtonUp( Button button );
		void OnButtonClick( Button button );
		void OnButtonDoubleClick( Button button );
		void OnMove( TCPoint2D& newPosition, TCPoint2D& newAbsolutePosition );
		void OnWheelScroll( int scrollDelta );

		bool IsDoubleClick( Button button );
};

#endif //__TC_MOUSE_INPUT_H__