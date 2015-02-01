//
// TCWindowsStartUp
// This file's purpose is to provide a space to intialize an application on Windows
//

//
// Includes
//

#include <windows.h>
#include <tchar.h>

#include "TCWindow.win32.h"
#include "TCLogger.h"
#include "TCMemUtils.h"

#include "CApplication.h"

//
// Defines
//

#define NULL 0

//
// Globals
//

static TCString gWindowClassName		= _T("thundercladWindow");				// The name of the window class
static TCString gWindowTitle			= _T("Thunderclad Graphics Testbed");	// The name of the window.
static HINSTANCE gInstance				= NULL;									// The current instance of this process.

static TCWindow_Win32* gDefaultWindow	= NULL;
static CApplication* gApplication		= NULL;

//
// CreateMainWindow
//		- Will instantiate the main window for this application.
// Inputs:
//		- TCString title: The name of the application.
//		- HINSTANCE hInstance: The instance of the program.
// Outputs:
//		- TCWindow_Win32*: The default window.
//

TCWindow_Win32* CreateMainWindow( TCString title, HINSTANCE hInstance )
{
	TCWindow_Win32* window = new TCWindow_Win32();

	TCWindow_Win32::Win32Description windowDesc;
	windowDesc.fullscreen			= false;
	windowDesc.xPosition			= 0.0f;
	windowDesc.yPosition			= 0.0f;
	windowDesc.width				= 1280.0f;
	windowDesc.height				= 720.0f;
	windowDesc.windowButtonFlags	= 0;
	windowDesc.windowFlags			= 0;
	windowDesc.instance				= hInstance;
	windowDesc.parentWindow			= NULL;
	windowDesc.windowTitle			= title;
	windowDesc.windowClassName		= gWindowClassName;

	TCResult result = window->Initialize( windowDesc );
	if( TC_FAILED( result ) )
	{
		TC_ASSERT(0 && "Failed to create default window!");
	}

	return window;
}

//
// WinMain
// This is the entry point for any windows application.
//

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	//
	// Create our window
	//

	gDefaultWindow = CreateMainWindow( gWindowTitle, hInstance );

	//
	// Set up logging.
	//

	gLogger->SetLogName("Thunderclad");
	gLogger->LogInfo("[Lifecycle] Successfully created the main window!");

	//
	// Create our application.
	//

	gApplication = new CApplication();
	gApplication->Initialize( gDefaultWindow );

	//
	// Spin up our main loop.
	//

	MSG msg;
	while( true )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}	
		else if( gApplication->Update( 0.0f ) )
		{
			gLogger->LogInfo("[Lifecycle] Application has flagged for quit.");
			break;
		}
	}

	//
	// Clean up.
	//

	TC_SAFE_DELETE( gApplication );
	TC_SAFE_DELETE( gDefaultWindow );
}