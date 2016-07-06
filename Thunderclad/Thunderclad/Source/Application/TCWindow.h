#ifndef __TCWINDOW_H__
#define __TCWINDOW_H__

//
// Includes
//

#include "TCPoint2D.h"
#include "TCBoundingBox2D.h"
#include "TCResultCode.h"
#include "TCEventDispatcher.h"

//
// Defines
//

// Window flags
#define WINDOW_FLAG_RESIZABLE	0x00000001
#define WINDOW_FLAG_MOVABLE		0x00000010

// Window button flags
#define WINDOW_BUTTON_MAXIMIZE	0x00000001
#define WINDOW_BUTTON_MINIMIZE	0x00000010
#define WINDOW_BUTTON_CLOSE		0x00000100

class TCWindow : public TCEventDispatcher
{
	public:		// Members	

		enum Visibility
		{
			WinVis_Hidden,
			WinVis_Maximized,
			WinVis_Minimized,
			WinVis_Shown,
		};

		//
		//- This structure is used to provide all initialization information needed
		//  to create a window.
		//

		struct Description
		{
			float width;
			float height;
			float xPosition;
			float yPosition;
			bool fullscreen;
			unsigned int windowFlags;
			unsigned int windowButtonFlags;
		};

	public:		// Methods

		virtual void				Update( float deltaTime );
		virtual void				Draw();
		virtual void				Destroy();

				TCPoint2D			GetPosition()	{ return mPosition; }
				TCBoundingBox2D		GetBounds()		{ return mBounds; }
				bool				IsFullScreen()	{ return mIsFullScreen; }

	protected:	// Methods
		virtual TCResult			Initialize( Description& desc );
		virtual TCResult			IsValidDescription( Description& desc );

	public:		// Members
		//
		//- This structure defines the information regarding a window creation event.
		//

		struct CreateEventData
		{
			TCWindow* window;
		};

		//
		//- This structure defines the information regarding a window resized event.
		//

		struct ResizeEventData
		{
			TCWindow* window;
			int newWidth;
			int newHeight;
		};

		//
		//- This structure defines the information regarding a window destroy event.
		//

		struct DestroyEventData
		{
			TCWindow* window;
		};

		//
		//- This structure defines the window visibility change
		//

		struct VisibilityChangeEventData
		{
			TCWindow* window;
			Visibility visibility;
		};

	protected:	// Members

		TCPoint2D		mPosition;
		TCBoundingBox2D mBounds;
		unsigned int	mWindowFlags;
		unsigned int	mWindowButtonFlags;
		bool			mIsFullScreen;
};

#endif