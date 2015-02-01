//
// TCApplication
// This class is responsible for interfacing with the engine and abstracting game logic from engine code.
//

#ifndef __TC_APPLICATION_H__
#define __TC_APPLICATION_H__

//
// Includes
//

#include "TCEventDispatcher.h"
#include "TCEventListener.h"

//
// Forward Declarations
//

class TCWindow;
class TCInputManager;
class TCFileManager;

//
// Class Declaration
//

class TCApplication : 
	public TCEventDispatcher,
	public TCEventListener
{
	public:		// Members		
	public:		// Methods
								TCApplication();
								TCApplication( const TCApplication& inRef );
		virtual TCApplication&	operator=( const TCApplication& inRef );
		virtual					~TCApplication();
		
		virtual void			Initialize( TCWindow* mainWindow );
		virtual bool			Update( float deltaTime );
		virtual void			CleanUp();

		virtual void			Quit();
		virtual void			OnEventFired( TCEventID event, void* eventData = NULL );

				TCInputManager*	GetInputManager()			{ return mInputManager; }

	protected:	// Members
		TCWindow*			mMainWindow;
		TCInputManager*		mInputManager;
		TCFileManager*		mFileManager;

		bool		mExitGame;

	protected:	// Methods
		virtual void			Clone( const TCApplication& inRef );
		virtual TCResult		InitializeInput();
		virtual TCResult		InitializeFileManager();

		// Event Handlers.
		virtual void			OnWindowDestroyed( void* eventData );
};

#endif // __TC_APPLICATION_H__