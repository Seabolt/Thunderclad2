//
// TCFile.h
// This file will encapsulate the accessing, reading, and lifetime of a file.
//

#ifndef __TC_FILE_H__
#define __TC_FILE_H__

//
// Includes
//

#include "TCResultCode.h"
#include "TCString.h"
#include "TCFileManager.h"

//
// Defines
//

//
// Forward Declaration
//

class TCFileManager;

//
// Class Declaration
//

class TCFile
{
	public:			// Members
	public:			// Methods
		
		virtual TCResult		Write( void* data, unsigned int dataLength );
		virtual TCResult		Write( TCString& text );

		virtual TCResult		Read( void** data, unsigned int dataLength );
		virtual TCResult		ReadLine( TCString& line );
		virtual TCResult		ReadWord( TCString& word );
		virtual TCResult		ReadChar( char& character );
		virtual TCResult		ReadText( TCString& text, char8 delimiter );

		virtual TCResult		Flush();

		inline	bool			IsOpen()										{ return mIsOpen; }
		inline	unsigned int	GetReadPosition()								{ return mReadPosition; }
		inline	unsigned int	GetWritePosition()								{ return mWritePosition; }
		inline  TCString&		GetFilepath()									{ return mFilepath; }
		inline	TCString&		GetFilename()									{ return mFilename; }

		virtual TCResult		SeekRead( unsigned int position );
		virtual TCResult		SeekWrite( unsigned int position );

	protected:		// Members
		TCFileManager*				mFileManager;
		unsigned int				mFileLength;

		TCFileManager::AccessType	mAccessType;
		TCFileManager::DataType		mDataType;
		TCFileManager::OpenMode		mOpenMode;

		unsigned int				mReadPosition;
		unsigned int				mWritePosition;
		bool						mIsOpen;

		TCString					mFilepath;
		TCString					mFilename;

	protected:		// Methods
								TCFile( TCFileManager* manager );
								TCFile( const TCFile& inRef );
		virtual TCFile&			operator=( const TCFile& inRef );
		virtual					~TCFile();

		virtual	void Clone( const TCFile& inRef );
		
		virtual TCResult Open( TCFileManager::FileDescription& description );
		virtual TCResult Close();

		friend TCFileManager;
};

#endif // __TC_FILE_H__