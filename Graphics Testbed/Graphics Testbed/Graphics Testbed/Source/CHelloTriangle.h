#ifndef __C_HELLO_TRIANGLE_H__
#define __C_HELLO_TRIANGLE_H__

//
// Includes
//

#include "TCApplication.h"
#include "CApplication.h"

//
// Defines
//

//
// Forward Declarations
//

class TCShader;

//
// Class Declaration
//

class CHelloTriangle : public CApplication
{
	public:		// Members
	public:		// Methods
		virtual void Initialize(TCWindow* mainWindow);
		virtual bool Update(float deltaTime);
		virtual void CleanUp();

		virtual void OnEventFired(TCEventID eventID, void* eventData);

	protected:	// Members
		TCShader* vertexShader;
		TCShader* pixelShader;

	protected:	// Methods

};

#endif
