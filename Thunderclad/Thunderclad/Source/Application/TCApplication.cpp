//
// TCApplication.cpp
// This file will define the engine interface for an application.
//

//
// Includes
//

#include "TCApplication.h"
#include "TCSystemEvents.h"
#include "TCWindow.h"
#include "TCInputManager.h"
#include "TCMouseInput.h"
#include "TCKeyboardInput.h"
#include "TCLogger.h"

#if TC_PLATFORM_WIN32
	#include "TCInputManager.win32.h"
	#include "TCFileManager.win32.h"
#endif

//
// Defines
//

//
// Default Constructor
//		- Will initialize this object.
// Inputs:
//		- None.
// Outputs:
//		- None.

TCApplication::TCApplication()
{
	mMainWindow		= NULL;
	mInputManager	= NULL;
	mFileManager	= NULL;
	mExitGame		= false;
}

//
// Copy Constructor
//		- Will copy another application into this object.
// Inputs:
//		- const TCApplication& inRef
// Outputs:
//		- None
//

TCApplication::TCApplication( const TCApplication& inRef )
{
	Clone( inRef );
}

//
// Assignment Operator
//		- Will set this instance equal to another instance.
// Inputs:
//		- const TCApplication& inRef: The reference to set this instance equal to.
// Outputs:
//		- TCApplication&: This reference after being assigned to.
//

TCApplication& TCApplication::operator=( const TCApplication& inRef )
{
	CleanUp();
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

TCApplication::~TCApplication()
{
	CleanUp();
}

//
// Initialize
//		- Will set up this instance to be ready for use.
// Inputs:
//		- TCWindow* mainWindow.
// Outputs:
//		- None.
//

void TCApplication::Initialize( TCWindow* mainWindow )
{
	//
	// Register ourselves as a listener to the main window.
	//

	mMainWindow = mainWindow;
	if( mMainWindow != NULL )
	{
		SubscribeTo( mMainWindow );
	}

	//
	// Create Input.
	//

	InitializeInput();
}

//
// Update
//		- Will update this application.
// Inputs:
//		- float deltaTime: The timestep since the last frame.
// Outputs:
//		- bool: Should we exit the game loop.
//

bool TCApplication::Update( float deltaTime )
{
	if( mExitGame )
	{
		CleanUp();
		return true;	// Kill the game.
	}

	return false;		// Continue running the game.
}


//
// CleanUp
//		- Will release all resources associated with this object.
// Inputs:
//		- None
// Outputs:
//		- None
//

void TCApplication::CleanUp()
{
	TC_SAFE_DELETE( mInputManager );
	TC_SAFE_DELETE( mFileManager );
}

//
// Quit
//		- Will end the game loop on the next frame.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCApplication::Quit()
{
	mExitGame = true;
}

//
// OnEventFired
//		- Will handle any events that this object listens to.
// Inputs:
//		- TCEventID eventID: The event that was fired.
//		- void* eventData: The event data
// Outputs:
//		- None.
//

void TCApplication::OnEventFired( TCEventID eventID, void* eventData )
{
	switch( eventID )
	{
		case SysEvent_WindowEvent_Destroy:
			OnWindowDestroyed( eventData );
	}
}

//
// Clone
//		- Will copy another instance
// Inputs:
//		- const TCApplication& inRef.
// Outputs:
//		- None.
//

void TCApplication::Clone( const TCApplication& inRef )
{
	mMainWindow			= inRef.mMainWindow;
	mExitGame			= inRef.mExitGame;
	mInputManager		= inRef.mInputManager;
	mFileManager		= inRef.mFileManager;

	TCEventDispatcher::Clone( inRef );
	TCEventListener::Clone( inRef );
}

//
// InitializeInput
//		- Will initialize the correct form of input for this class.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The input was initialized successfully.
//			- Failure_InvalidParameter: The window was invalid.
//

TCResult TCApplication::InitializeInput()
{
#if TC_PLATFORM_WIN32
	TCWindow_Win32* window = (TCWindow_Win32*)mMainWindow;
	TCInputManager_Win32::Description_Win32 inputDesc;

	inputDesc.window = window;
	mInputManager = new TCInputManager_Win32();
	
	//
	// Initialize the input.
	//

	TCResult result = mInputManager->Initialize( inputDesc );
	if( TC_FAILED( result ) )
	{
		gLogger->LogFailure( "[TCApplication] Failed to create the input manager!" );
		return result;
	}

	//
	// Subscribe to the input.
	//

	if( mInputManager->GetMouse() != NULL )
	{
		SubscribeTo( mInputManager->GetMouse() );
	}

	if( mInputManager->GetKeyboard() != NULL )
	{
		SubscribeTo( mInputManager->GetKeyboard() );
	}

	return Success;

#endif
}

//
// InitializeFileManager
//		- Will create and initialize the file manager.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//

TCResult TCApplication::InitializeFileManager()
{
#if TC_PLATFORM_WIN32
	mFileManager = new TCFileManager_Win32();
	
	TCResult result =  mFileManager->Initialize();
	if( TC_FAILED( result ) )
		return result;

#endif

	mFileManager->SetEngineResourceDirectory( TCString("C:/Thunderclad/Thunderclad/Thunderclad/Resources/") );
}

//
// OnWindowDestroyed
//		- Will handle when a window is destroyed.
// Inputs:
//		- void* eventData: The information about the event.
// Outputs:
//		- None.
//

void TCApplication::OnWindowDestroyed( void* eventData )
{
	TCWindow::DestroyEventData* destroyEvent = (TCWindow::DestroyEventData*)eventData;
	if( destroyEvent != NULL )
	{
		if( destroyEvent->window == mMainWindow )
		{
			//
			// Flag for exit. Tell everyone that their destruction is emminent.
			//

			FireEvent( SysEvent_DestructionImminent );
			Quit();
		}
	}
}