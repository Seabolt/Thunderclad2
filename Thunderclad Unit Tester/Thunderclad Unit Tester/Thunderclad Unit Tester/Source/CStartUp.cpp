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
#include "TCUnitTestManager.h"
#include "TCList_UnitTest.h"
#include "TCHashTable_UnitTest.h"
#include "TCString_UnitTest.h"

//
// Defines
//

#define NULL 0

//
// Globals
//

static TCString gWindowClassName		= _T("thundercladWindow");				// The name of the window class
static TCString gWindowTitle			= _T("Thunderclad UnitTesting");		// The name of the window.
static HINSTANCE gInstance				= NULL;									// The current instance of this process.

static TCWindow_Win32 gDefaultWindow;

LRESULT CALLBACK WindowMessagePump(HWND hWnd,
							  UINT Msg,
						      WPARAM wParam, 
							  LPARAM lParam)
{
    switch(Msg)
    {
		// If the user wants to close the application
		case WM_DESTROY:
			// then close it
			gDefaultWindow.Destroy();
			PostQuitMessage(WM_QUIT);
			break;

		default:
			// Process the left-over messages
			return DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    // If something was not done, let it go
    return 0;
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
	windowDesc.windowTitle			= gWindowTitle;
	windowDesc.windowClassName		= gWindowClassName;
	windowDesc.winProc				= WindowMessagePump;

	TCResult result = gDefaultWindow.Initialize( windowDesc );
	if( TC_FAILED( result ) )
	{
		TC_ASSERT(0 && "Failed to create default window!");
	}

	TCLogger::GetInstance()->SetLogName("Thunderclad");
	TCLogger::GetInstance()->LogInfo("Successfully created the main window!");

	//
	// Run Unit Test
	//

	TCUnitTestManager::GetInstance()->Initialize();
	TCUnitTestManager::GetInstance()->AddUnitTest( new TCString_UnitTest() );
	TCUnitTestManager::GetInstance()->AddUnitTest( new TCList_UnitTest() );
	TCUnitTestManager::GetInstance()->AddUnitTest( new TCHashTable_UnitTest() );
	TCUnitTestManager::GetInstance()->StartTests();

	//
	// Spin up our main loop.
	//

	MSG msg;
	while( GetMessage( &msg, NULL, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );

		TCUnitTestManager::GetInstance()->Update();
	}

	TCUnitTestManager::GetInstance()->Destroy();
}