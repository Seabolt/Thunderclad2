//
// CApplication.h
// This file will define the application class for the graphics test bed.
//

#ifndef __C_APPLICATION_H__
#define __C_APPLICATION_H__

//
// Includes
//

#include "TCApplication.h"

//
// Defines
//

//
// Forward Declaration
//

class TCShader;

//
// Class Declaration
//

class CApplication : public TCApplication
{
	public:		// Members
	public:		// Methods

		virtual void Initialize( TCWindow* mainWindow );
		virtual bool Update( float deltaTime );
		virtual void CleanUp();

		virtual void OnEventFired( TCEventID eventID, void* eventData );

	private:	// Members
		TCShader* vertexShader;
		TCShader* pixelShader;

	private:	// Methods
};

#endif