//
// TCInputManager.cpp
// This file will define the platform agnostic layer for input.
//

//
// Includes
//

#include "TCInputManager.h"
#include "TCMouseInput.h"
#include "TCKeyboardInput.h"

//
// Defines
//

//
// Default Constructor
//		- Will initialize the members of this class.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCInputManager::TCInputManager()
{
	mMouse = NULL;
}

//
// Copy Constructor
//		- Will copy another TCInputManager
// Inputs:
//		- const TCInputManager& inRef: The reference to copy.
// Outputs:
//		- None.
//

TCInputManager::TCInputManager( const TCInputManager& inRef )
{
	Clone( inRef );
}

//
// Assignment Operator
//		- Will set this instance equal to another and return the reference to this instance.
// Inputs:
//		- const TCInputManager& inRef: The reference to copy.
// Outputs:
//		- TCInputManager&: The reference to this object.
//

TCInputManager& TCInputManager::operator=( const TCInputManager& inRef )
{
	Destroy();
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

TCInputManager::~TCInputManager()
{
	Destroy();
}

//
// Initialize
//		- Will initialize and create all resources associated with this object.
// Inputs:
//		- TCInputManager::Description& desc: The description for how to initialize this object.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: This level will always return success.
//

TCResult TCInputManager::Initialize( TCInputManager::Description& desc )
{
	return Success;
}

//
// Update
//		- Will perform the frame to frame update of this object.
// Inputs:
//		- float deltaTime: The time since the last frame.
// Outputs:
//		- None.
//

void TCInputManager::Update( float deltaTime )
{
	UpdateMouse( deltaTime );
	UpdateKeyboard( deltaTime );
}

//
// Destroy
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCInputManager::Destroy()
{
	TC_SAFE_DELETE( mMouse );
	TC_SAFE_DELETE( mKeyboard );
}

//
// Clone
//		- Will copy from another TCInputManager
// Inputs:
//		- const TCInputManager& inRef: The reference to copy.
// Outputs:
//		- None.
//

void TCInputManager::Clone( const TCInputManager& inRef )
{
	mMouse = inRef.mMouse;
	mKeyboard = inRef.mKeyboard;
}

//
// UpdateMouse
//		- Will perform the per-frame update of the mouse.
// Inputs:
//		- float deltaTime: The time since the last frame.
// Outputs:
//		- None.
//

void TCInputManager::UpdateMouse( float deltaTime )
{
	if( mMouse != NULL )
	{
		mMouse->Update( deltaTime );
	}
}

//
// UpdateKeyboard
//		- Will perform the per-frame update of the keyboard.
// Inputs:
//		- float deltaTime: The time since the last frame.
// Outputs:
//		- None.
//

void TCInputManager::UpdateKeyboard( float deltaTime )
{
	if( mKeyboard != NULL )
	{
		mKeyboard->Update( deltaTime );
	}
}