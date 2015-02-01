//
// CApplication.cpp
// This file will define the functionality of this application.
//

//
// Includes
//

#include "CApplication.h"
#include "TCSystemEvents.h"
#include "TCLogger.h"
#include "TCKeyboardInput.h"

//
// Defines
//

//
// Initialize
//		- Will setup this application, making it ready for use.
// Inputs
//		- TCWindow* mainWindow: The main window for this application.
// Outputs:
//		- None.
//

void CApplication::Initialize( TCWindow* mainWindow )
{
	TCApplication::Initialize( mainWindow );
}

//
// Update
//		- Will perform the frame to frame update of this application.
// Inputs:
//		- float deltaTime: The time step since the last frame.
// Outputs:
//		- bool: Should the we leave the main loop?
//

bool CApplication::Update( float deltaTime )
{
	return TCApplication::Update( deltaTime );
}

//
// CleanUp
//		- Will free all resources associated with this object
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void CApplication::CleanUp()
{
	TCApplication::CleanUp();
}

//
// OnEventFired
//		- Will handle any events fired by any subscribed dispatchers.
// Inputs:
//		- TCEventID eventID: The event that was fired.
//		- void* eventData: Any information about the event.
// Outputs:
//		- None.
//

void CApplication::OnEventFired( TCEventID eventID, void* eventData )
{
	if( eventID == SysEvent_KeyboardEvent_OnKeyClicked )
	{
		TCKeyboardInput::KeyClickedEvent* clickedEvent = (TCKeyboardInput::KeyClickedEvent*)eventData;
		if( clickedEvent != NULL && clickedEvent->keyCode == TCKeyCode_Escape )
		{
			Quit();
		}
	}


	TCApplication::OnEventFired( eventID, eventData );
}