//
// TCFile.win32.h
// This file will define all windows specific functionality for a reading and writing to a file.
//

#ifndef __TC_FILE_WIN32_H__
#define __TC_FILE_WIN32_H__

//
// Includes
//

#include "TCFile.h"
#include "TCFileManager.win32.h"
#include <stdio.h>

//
// Defines
//

//
// Forward Declarations
//

//
// Class Declaration
//

class TCFile_Win32
	: public TCFile
{		
		virtual TCResult		Write( void* data, unsigned int dataLength );
		virtual TCResult		Write( TCString text );

		virtual TCResult		Read( void** data, unsigned int dataLength );
		virtual TCResult		ReadLine( TCString& line );
		virtual TCResult		ReadWord( TCString& word );
		virtual TCResult		ReadChar( char& character );
		virtual TCResult		ReadText( TCString& text, char8 delimiter );
		virtual TCResult		ReadFile( TCString& text );

		virtual TCResult		Flush();

		inline	bool			IsOpen()										{ return mIsOpen; }
		virtual	unsigned int	GetReadPosition();
		virtual	unsigned int	GetWritePosition();
		inline  TCString&		GetFilepath()									{ return mFilepath; }
		inline	TCString&		GetFilename()									{ return mFilename; }

		virtual TCResult		SeekRead( unsigned int position );
		virtual TCResult		SeekWrite( unsigned int position );

		virtual char			GetEofCharacter();

	protected:		// Members
		FILE* mFile;

	protected:		// Methods
								TCFile_Win32( TCFileManager* manager );
								TCFile_Win32( const TCFile_Win32& inRef );
		virtual TCFile_Win32&	operator=( const TCFile_Win32& inRef );
		virtual					~TCFile_Win32();

		virtual	void			Clone( const TCFile_Win32& inRef );
		
		virtual TCResult		Open( TCFileManager::FileDescription& description );
		virtual TCResult		Close();

				bool			IsValidDescription( TCFileManager::FileDescription& description );
				TCString		GetModeString( TCFileManager::FileDescription& description );

		friend TCFileManager_Win32;
};

#endif // __TC_FILE_WIN32_H__