//
// TCInputManager.h
// This object will encapsulate all methods of input and perform any platform agnostic functionality.
//

#ifndef __TC_INPUT_MANAGER_H__
#define __TC_INPUT_MANAGER_H__

//
// Includes
//

#include "TCResultCode.h"
#include "TCPlatformPrecompilerSymbols.h"

//
// Defines
//

//
// Forward Declarations
//

class TCMouseInput;
class TCKeyboardInput;

//
// Class Declaration
//

class TCInputManager
{
	public:		// Members

		struct Description
		{
		};

	public:		// Methods
									TCInputManager();
									TCInputManager( const TCInputManager& inRef );
		virtual TCInputManager&		operator=( const TCInputManager& inRef );
		virtual						~TCInputManager();

		virtual TCResult			Initialize( Description& desc );
		virtual void				Update( float deltaTime );
		virtual void				Destroy();

				TCMouseInput*		GetMouse()										{ return mMouse; }
				TCKeyboardInput*	GetKeyboard()									{ return mKeyboard; }

	protected:	// Members
		TCMouseInput*		mMouse;
		TCKeyboardInput*	mKeyboard;

	protected:	// Methods

		virtual void			Clone( const TCInputManager& inRef );
		virtual void			UpdateMouse( float deltaTime );
		virtual void			UpdateKeyboard( float deltaTime );
};

#endif // __TC_INPUT_H__