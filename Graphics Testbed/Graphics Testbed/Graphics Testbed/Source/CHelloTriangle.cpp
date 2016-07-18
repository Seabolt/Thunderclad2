//
// CApplication.cpp
// This file will define the functionality of this application.
//

//
// Includes
//

#include "CHelloTriangle.h"
#include "TCSystemEvents.h"
#include "TCLogger.h"
#include "TCKeyboardInput.h"
#include "TCShader.h"
#include "TCFileManager.h"

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

void CHelloTriangle::Initialize(TCWindow* mainWindow)
{
	CApplication::Initialize(mainWindow);

	//
	// Create the vertex shader.
	//

	vertexShader = new TCShader(mGraphicsContext);

	TCShader::Description description;
	description.shaderFilepath = mFileManager->GetResourceDirectory() + "Shaders\\HLSL\\UnlitHomogenous.rvs";
	description.shaderType = TCShader::kShaderTypeVertex;
	description.shaderName = "UnlitHomogenous.rvs";
	if (TC_FAILED(vertexShader->Initialize(description)))
	{
		TCLogger::GetInstance()->LogError("Failed to create vertex shader");
	}

	// 
	// Create the pixel shader.
	//

	pixelShader = new TCShader(mGraphicsContext);

	description.shaderFilepath = mFileManager->GetResourceDirectory() + "Shaders\\HLSL\\UnlitHomogenous.rps";
	description.shaderType = TCShader::kShaderTypePixel;
	description.shaderName = "UnlitHomogenous.rps";
	if (TC_FAILED(pixelShader->Initialize(description)))
	{
		TCLogger::GetInstance()->LogError("Failed to create pixel shader");
	}
}

//
// Update
//		- Will perform the frame to frame update of this application.
// Inputs:
//		- float deltaTime: The time step since the last frame.
// Outputs:
//		- bool: Should the we leave the main loop?
//

bool CHelloTriangle::Update(float deltaTime)
{
	return CApplication::Update(deltaTime);
}

//
// CleanUp
//		- Will free all resources associated with this object
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void CHelloTriangle::CleanUp()
{
	if( vertexShader != NULL )
	{
		vertexShader->Release();
		TC_SAFE_DELETE( vertexShader );
	}

	if( pixelShader != NULL )
	{
		pixelShader->Release();
		TC_SAFE_DELETE( pixelShader );
	}

	CApplication::CleanUp();
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

void CHelloTriangle::OnEventFired(TCEventID eventID, void* eventData)
{
	CApplication::OnEventFired(eventID, eventData);
}