//
// TCInputManager.win32.h
// This file will perform the win32 platform specific functionality.
//

#ifndef __TC_INPUT_MANAGER_WIN32_H__
#define __TC_INPUT_MANAGER_WIN32_H__

//
// Includes
//

#include "TCInputManager.h"

//
// Defines
//

//
// Forward Declaration
//

class TCWindow_Win32;

//
// Class Declaration
//

class TCInputManager_Win32 :
	public TCInputManager
{
	public:		// Members
		struct Description_Win32
			: public Description
		{
			TCWindow_Win32* window;
		};

	public:		// Methods
										TCInputManager_Win32();
										TCInputManager_Win32( const TCInputManager_Win32& inRef );	
		virtual TCInputManager_Win32&	operator=( const TCInputManager_Win32& inRef );
		virtual							~TCInputManager_Win32();

		virtual	TCResult				Initialize( Description& desc );
		virtual void					Update( float deltaTime );
		virtual void					Destroy();

	private:	// Members
	private:	// Methods

		virtual void					Clone( const TCInputManager_Win32& inRef );
				bool					IsValidDescription( Description_Win32& inDesc );
				TCResult				InitializeMouse( Description_Win32& inDesc );
				TCResult				InitializeKeyboard( Description_Win32& inDesc );
};

#endif // __TC_INPUT_MANAGER_WIN32_H__