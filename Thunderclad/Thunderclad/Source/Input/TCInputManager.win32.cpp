//
// TCInputManager.win32.cpp
// This file will define the windows specific functionality of an input manager.
//

//
// Includes
//

#include "TCInputManager.win32.h"
#include "TCMemUtils.h"
#include "TCMouseInput.win32.h"
#include "TCKeyboardInput.win32.h"
#include "TCLogger.h"

//
// Defines
//

//
// Default Constructor
//		- Will initialize all components of this object to a safe state.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCInputManager_Win32::TCInputManager_Win32()
{

}

//
// Copy Constructor
//		- Will copy another TCInputManager_Win32 reference.
// Inputs:
//		- const TCInputManager_Win32& inRef: The reference to copy.
// Outputs:
//		- None.
//

TCInputManager_Win32::TCInputManager_Win32( const TCInputManager_Win32& inRef )
{
	Clone( inRef );
}

//
// Assignment Operator
//		- Will set this instance equal to another instance and return the reference to this object.
// Inputs:
//		- const TCInputManager_Win32& inRef: The reference to set this instance equal to.
// Outputs:
//		- TCInputManager_Win32&: The reference to this object.
//

TCInputManager_Win32& TCInputManager_Win32::operator=( const TCInputManager_Win32& inRef )
{
	Destroy();
	Clone( inRef );

	return (*this);
}

//
// Destructor
//		- Will free all resources associated with this instance.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCInputManager_Win32::~TCInputManager_Win32()
{
	Destroy();
}

//
// Initialize
//		- Will initialize all resources associated with this object.
// Inputs:
//		- TCInputManager_Win32::Description& desc: The description for how to initialize this object.
// Outputs:
//		- TCResult: The result of this operation.
//			- Success: The initialization succeeded.
//			- Failure_InvalidParameter: The description is invalid.
//

TCResult TCInputManager_Win32::Initialize( TCInputManager::Description& desc )
{
	//
	// Check to see if the description is valid.
	//

	Description_Win32& platformDesc = (Description_Win32&)desc;
	if( IsValidDescription( platformDesc ) == false )
	{
		return Failure_InvalidParameter;
	}

	//
	// Initialize our sub-modules.
	//

	TCResult result = InitializeMouse( platformDesc );
	if( TC_FAILED( result ) )
	{
		return result;
	}
	
	//
	// Initialize our keyboard
	//

	result = InitializeKeyboard( platformDesc );
	if( TC_FAILED( result ) )
	{
		return result;
	}

	//
	// Return base Initialize
	//

	return TCInputManager::Initialize( desc );
}

//
// Update
//		- Will perform the frame to frame update of this object
// Inputs:
//		- float deltaTime: The time since the last frame.
// Outputs:
//		- None.
//

void TCInputManager_Win32::Update( float deltaTime )
{
	TCInputManager::Update( deltaTime );
}

//
// Destroy
//		- Will release all resources associated with this object.
// Inputs:
//		- None
// Outputs:
//		- None
//

void TCInputManager_Win32::Destroy()
{
	TCInputManager::Destroy();
}

//
// Clone
//		- Will copy another instance into this one.
// Inputs:
//		- const TCInputManager_Win32& inRef: The reference to copy.
// Outputs:
//		- None.
//

void TCInputManager_Win32::Clone( const TCInputManager_Win32& inRef )
{
	TCInputManager::Clone( inRef );
}

//
// IsValidDescription
//		- Will return if the description provided is valid
// Inputs:
//		- TCInputManager_Win32::Description_Win32& desc: The description to check.
// Outputs:
//		- bool: Was the description valid.
//

bool TCInputManager_Win32::IsValidDescription( TCInputManager_Win32::Description_Win32& desc )
{
	if( desc.window == NULL )
	{
		return false;
	}

	return true;
}

//
// InitializeMouse
//		- Will create the mouse for this manager.
// Inputs:
//		- TCInputManager_Win32::Description_Win32& desc: The description to create the mouse with.
// Outputs:
//		- TCResult: The result of this operation.
//			- Success: We successfully initialize the mouse.
//			- Failure_InvalidParameter: The description was invalid when provided to the mouse.
//

TCResult TCInputManager_Win32::InitializeMouse( TCInputManager_Win32::Description_Win32& desc )
{
	//
	// Free any old copies of the mouse.
	//

	TC_SAFE_DELETE( mMouse );

	//
	// Create and Initialize the mouse.
	//

	mMouse = new TCMouseInput_Win32();

	TCMouseInput_Win32::Description_Win32 mouseDesc;
	mouseDesc.mWindow = desc.window;

	TCResult result = mMouse->Initialize( mouseDesc );

	//
	// Check for failure.
	//

	if( TC_FAILED( result ) )
	{
		gLogger->LogError( "[TCInputManager] Failed to initialize the mouse!");
		return result;
	}

	return Success;
}

//
// InitializeKeyboard
//		- Will create the keyboard for this manager.
// Inputs:
//		- TCInputManager_Win32::Description_Win32& desc: The description to create the keyboard with.
// Outputs:
//		- TCResult: The result of this operation.
//			- Success: We successfully initialize the mouse.
//			- Failure_InvalidParameter: The description was invalid when provided to the mouse.
//

TCResult TCInputManager_Win32::InitializeKeyboard( TCInputManager_Win32::Description_Win32& desc )
{
	//
	// Free any old copies of the mouse.
	//

	TC_SAFE_DELETE( mKeyboard );

	//
	// Create and Initialize the mouse.
	//

	mKeyboard = new TCKeyboardInput_Win32();

	TCKeyboardInput_Win32::Description_Win32 keyboardDesc;
	keyboardDesc.window = desc.window;

	TCResult result = mKeyboard->Initialize( keyboardDesc );

	//
	// Check for failure.
	//

	if( TC_FAILED( result ) )
	{
		gLogger->LogError( "[TCInputManager] Failed to initialize the keyboard!");
		return result;
	}

	return Success;
}