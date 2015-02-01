//
// TCSystemEvents.h
// This file will define all the possible events that can be fired from an event dispatcher from a system level.
//

#ifndef __TC_SYSTEM_EVENTS_H__
#define __TC_SYSTEM_EVENTS_H__

//
// Includes
//

//
// Defines
//

#define MAX_SYSTEM_EVENTS 5000

//
// Typedefs
//

enum TCSystemEvent
{
	//
	// Lifecycle Events.
	//

	SysEvent_ApplicationCreated = 0,
	SysEvent_DestructionImminent,
	SysEvent_Destroy,
	SysEvent_Suspended,
	SysEvent_Resumed,

	//
	// Window Events
	//

	SysEvent_WindowEvents_Start = 50,
	SysEvent_WindowEvent_Destroy,
	SysEvent_WindowEvent_Created,
	SysEvent_WindowEvent_Resize,
	SysEvent_WindowEvent_Minimized,
	SysEvent_WindowEvent_Maximized,
	SysEvent_WindowEvent_MouseButtonDown,
	SysEvent_WindowEvent_MouseButtonUp,
	SysEvent_WindowEvent_MouseMovement,
	SysEvent_WindowEvent_MouseWheelMovement,
	SysEvent_WindowEvent_KeyDown,
	SysEvent_WindowEvent_KeyUp,
	SysEvent_WindowEvents_End,

	//
	// Input Events
	//
	 
	SysEvent_MouseEvents_Start = 100,
	SysEvent_MouseEvent_Movement,
	SysEvent_MouseEvent_ButtonDown,
	SysEvent_MouseEvent_ButtonUp,
	SysEvent_MouseEvent_ButtonClicked,
	SysEvent_MouseEvent_ButtonDoubleClicked,
	SysEvent_MouseEvent_WheelMovement,
	SysEvent_MouseEvents_End,

	SysEvent_KeyboardEvents_Start = 120,
	SysEvent_KeyboardEvent_OnKeyDown,
	SysEvent_KeyboardEvent_OnKeyUp,
	SysEvent_KeyboardEvent_OnKeyClicked,
	SysEvent_KeyboardEvents_End,

	SysEvent_MaxCount = MAX_SYSTEM_EVENTS
};


#endif