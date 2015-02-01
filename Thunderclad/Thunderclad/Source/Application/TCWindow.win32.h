#ifndef __TCWINDOW_WIN32_H__
#define __TCWINDOW_WIN32_H__

#include "Globals\TCPlatformPrecompilerSymbols.h"

#if TC_PLATFORM_WIN32

//
// Includes
//

#include "TCWindow.h"

#include <tchar.h>
#include <Windows.h>
#include "TCString.h"

//
// Typedefs
//

typedef LRESULT (CALLBACK *WndProcedure)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

//
// Defines
//

class TCWindow_Win32 : public TCWindow
{
	public:

		// Win32Description is a helper structure to store all initialization information.
		struct Win32Description : public TCWindow::Description
		{
			HINSTANCE instance;
			TCString windowTitle;
			TCString windowClassName;
			HWND parentWindow;

			int showCommand;
			TCString iconResource;
			TCString smallIconResource;
			int backgroundBrush;
			TCString cursorResource;
			WndProcedure winProc;

			Win32Description();
		};

	public:		// Methods

		virtual ~TCWindow_Win32();
		TCWindow_Win32();

		TCResult Initialize( Win32Description& description );
		TCResult HandleWindowMessage( UINT message, WPARAM wParam, LPARAM lParam );
		void Destroy();

		HWND GetHWindow()			{ return mWindow; }
		HWND GetParentHWindow()		{ return mParentWindow; }
		HINSTANCE GetHInstance()	{ return mInstance; }
		HBRUSH GetBackgroundBrush()	{ return mBackgroundBrush; }
		HCURSOR GetCursor()			{ return mCursor; }

		static TCWindow_Win32* GetWindowFromHWND( HWND windowHandle );

	private:	// Methods
		
		TCResult IsValidDescription( Win32Description& description );
		TCResult InitializeWindowClass( Win32Description& description );
		TCResult InitializeWindow( Win32Description& description );

		static TCResult InitializeIcons( HINSTANCE instance, TCString& iconResourceName, TCString& smallIconResourceName, HICON& outIcon, HICON& outSmallIcon );
		static TCResult InitializeCursor( HINSTANCE instance, TCString& cursorResource, HCURSOR& outCursor );
		static TCResult InitializeBrush( HINSTANCE instance, int brush, HBRUSH& outBrush );

		TCResult HandleDestroyMessage( WPARAM wParam, LPARAM lParam );
		TCResult HandleLeftButtonDownMessage( WPARAM wParam, LPARAM lParam );
		TCResult HandleMiddleButtonDownMessage( WPARAM wParam, LPARAM lParam );
		TCResult HandleRightButtonDownMessage( WPARAM wParam, LPARAM lParam );
		TCResult HandleLeftButtonUpMessage( WPARAM wParam, LPARAM lParam );
		TCResult HandleMiddleButtonUpMessage( WPARAM wParam, LPARAM lParam );
		TCResult HandleRightButtonUpMessage( WPARAM wParam, LPARAM lParam );
		TCResult HandleMouseMoveMessage( WPARAM wParam, LPARAM lParam );
		TCResult HandleMouseWheelMoveMessage( WPARAM wParam, LPARAM lParam );
		TCResult HandleKeyDown( WPARAM wParam, LPARAM lParam );
		TCResult HandleKeyUp( WPARAM wParam, LPARAM lParam );

	public:		// Members
				// Event Info
		enum MouseButton
		{
			LeftButton,
			MiddleButton,
			RightButton,
		};

		struct MouseButtonDown
		{
			MouseButton button;
			TCWindow* window;
		};

		struct MouseButtonUp
		{
			MouseButton button;
			TCWindow* window;
		};

		struct MouseWheelMove
		{
			int delta;
			TCWindow* window;
		};

		struct MouseMove
		{
			TCPoint2D relativePosition;
			TCPoint2D absolutePosition;
			TCWindow* window;
		};

		struct KeyDown
		{
			unsigned int keyCode;
			unsigned int lParam;
			TCWindow* window;
		};

		struct KeyUp
		{
			unsigned int keyCode;
			unsigned int lParam;
			TCWindow* window;
		};

	private:	// Members

		HINSTANCE		mInstance;
		HWND			mWindow;
		HWND			mParentWindow;
		WNDCLASSEX		mWindowClass;
		HBRUSH			mBackgroundBrush;
		HCURSOR			mCursor;
		WndProcedure	mWindowProcedure;
		int				mShowCommand;
		HICON			mIcon;
		HICON			mSmallIcon;
		TCString		mWindowTitle;
		TCString		mWindowClassName;

		static TCList< TCWindow_Win32* > gWindowList;
};

#endif	// #if TC_PLATFORM_WIN32

#endif	// #ifndef __TCWINDOW_WIN32_H__