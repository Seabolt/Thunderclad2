//
// TCWindow.cpp
// This will define functionality for the platform agnostic window
//

#include "TCWindow.h"

//
// Initialize
// Will setup a window for use.
// Inputs:
//		- Description desc: The description for how to initialize this window.
// Outputs:
//		- TCResult result: The result of this operation.
//

TCResult TCWindow::Initialize( TCWindow::Description& desc )
{
	//
	// Set the bounds and position of the window.
	//

	m_Bounds.center.Set( desc.xPosition + (desc.width / 2.0f), desc.yPosition + (desc.height / 2.0f) );
	m_Position.Set( desc.xPosition, desc.yPosition );

	//
	// Copy over our flags
	//

	m_WindowButtonFlags = desc.windowButtonFlags;
	m_WindowFlags = desc.windowFlags;

	return TCResult::Success;
}

//
// Update
// Will perform the per-frame update for this object
// Inputs: 
//		- float deltaTime: The time step for this frame
// Outputs:
//		- None.
//

void TCWindow::Update( float deltaTime )
{

}

//
// Draw
// Will draw all children of this object 
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCWindow::Draw()
{

}

//
// Destroy
// Will clean up all resources of this object
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCWindow::Destroy()
{

}

//
// IsValidDescription
//		Will return if the description provided is valid
// Inputs:
//		- Description& description: The description to check
// Outputs:
//		- TCResult: The result of the operation.
//		- Possible returns are:
//			- TCResult::Failure_InvalidParameter
//			- TCResult::Success
//

TCResult TCWindow::IsValidDescription( Description& description )
{
	if( description.width <= 0 || description.height <= 0 )
		return TCResult::Failure_InvalidParameter;

	return TCResult::Success;
}