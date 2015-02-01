//
// TCFile_Win32.win32.cpp
// This file will define all functionality for a file in windows.
//

//
// Includes
//

#include "TCFile.win32.h"
#include "TCLogger.h"
#include <sys/stat.h>

//
// Defines
//

//
// Default Constructor
//		- This will initialize the object to safe values.
// Inputs:
//		- TCFileManager* fileManager: The manager for this file.
// Outputs:
//		- None.
//

TCFile_Win32::TCFile_Win32( TCFileManager* fileManager ) :
	TCFile( fileManager )
{
	mFile = NULL;
}

//
// Copy Constructor
//		- This will copy another file.
// Inputs:
//		- const TCFile_Win32& inRef: The file to copy.
// Outputs:
//		- None.
//

TCFile_Win32::TCFile_Win32( const TCFile_Win32& inRef ) :
	TCFile( inRef )
{
	Clone( inRef );
}

//
// Assignment Operator
//		- This will set this instance equal to another and return a reference to the new file.
// Inputs:
//		- const TCFile_Win32& inRef: The file to copy.
// Outputs:
//		- TCFile_Win32&: A reference to this file with the new data.
//

TCFile_Win32& TCFile_Win32::operator=( const TCFile_Win32& inRef )
{
	Close();
	Clone( inRef );

	return (*this);
}

//
// Destructor
//		- This will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCFile_Win32::~TCFile_Win32()
{
	Close();
}

//
// Write
//		- This will allow the user to write to the file, if the file is open.
// Inputs:
//		- void* data: The information to write.
//		- unsigned int dataLength: The size of the data to write.
// Outputs:
//		- TCResult: The result of the operation:
//			- Success: We wrote to the file successfully.
//			- Failure_InvalidAccess: The file is not write-able.
//			- Failure_InvalidOperation: The file is not open.
//

TCResult TCFile_Win32::Write( void* data, unsigned int dataLength )
{
	//
	// Make sure we're in a good state.
	//

	if( mFile == NULL )
		return Failure_InvalidOperation;

	if( mAccessType == TCFileManager::Access_ReadOnly )
		return Success_Cached;

	//
	// Write to the file.
	//

	unsigned int count = fwrite( data, 1, dataLength, mFile );
	if( count != dataLength )
	{
		int error = ferror( mFile );
		gLogger->LogError( TCString( "[TCFile] Failed to write to file: " ) + mFilename + TCString( ". Error number: " ) + error );
		return Failure_InvalidOperation;
	}

	return Success;
}

//
// Write
//		- This will allow the user to write to the file, if the file is open.
// Inputs:
//		- TCString& text: The text to write to the file.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: We wrote to the file successfully.
//			- Failure_InvalidAccess: The file is not write-able.
//			- Failure_InvalidOperation: The file is not open.
//

TCResult TCFile_Win32::Write( TCString& text )
{
	return Write( text.Data(), text.Length() );
}

//
// Read
//		- This will allow the user to read from the file, if the file is open.
// Inputs:
//		- void** data: The data buffer to write to.
//		- unsigned int dataLength: The amount to read from the file.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: We read from the file successfully.
//			- Failure_InvalidAccess: The file is not read-able.
//			- Failure_InvalidOperation: The file is not open.
//

TCResult TCFile_Win32::Read( void** data, unsigned int dataLength )
{
	//
	// Make sure we're in a good state.
	//

	if( mFile == NULL )
		return Failure_InvalidOperation;

	if( mAccessType == TCFileManager::Access_WriteOnly )
		return Success_Handled;

	//
	// Read from the file.
	//

	unsigned int count = fread( *data, 1, dataLength, mFile );
	if( count != dataLength )
	{
		int error = ferror( mFile );
		gLogger->LogError( TCString( "[TCFile] Failed to write to file: " ) + mFilename + TCString( ". Error number: " ) + error );
		return Failure_InvalidOperation;
	}
}

//
// ReadLine
//		- This will allow the user to read a line from the file.
// Inputs:
//		- TCString& text: The string to place the line.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: We read from the file successfully.
//			- Failure_InvalidAccess: The file is not read-able.
//			- Failure_InvalidOperation: The file is not open or the file is not treated as text.
//

TCResult TCFile_Win32::ReadLine( TCString& text )
{
	return ReadText( text, '\r' );
}

//
// ReadWord
//		- This will allow the user to read a word from a file.
// Inputs:
//		- TCString& word: The string to place the word.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: We read from the file successfully.
//			- Failure_InvalidAccess: The file is not read-able.
//			- Failure_InvalidOperation: The file is not open or the file is not treated as text.
//

TCResult TCFile_Win32::ReadWord( TCString& word )
{
	return ReadText( word, ' ' );
}

//
// ReadChar
//		- This will read the next character out of the file.
// Inputs:
//		- char8& character: The character to fill.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: We read from the file successfully.
//			- Failure_InvalidAccess: The file is not read-able.
//			- Failure_InvalidOperation: The file is not open or the file is not treated as text.
//

TCResult TCFile_Win32::ReadChar( char8& character )
{
	void* cPointer = &character;
	return Read( &cPointer, 1 );
}

//
// ReadText
//		- This will allow the user to read until a specified delimiter.
// Inputs:
//		- TCString& text: The string to place the text.
//		- char8 delimiter: The character that signifies when to stop.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: We read from the file successfully.
//			- Failure_InvalidAccess: The file is not read-able.
//			- Failure_InvalidOperation: The file is not open.
//

TCResult TCFile_Win32::ReadText( TCString& text, char8 delimiter )
{
	if( mFile == NULL )
		return Failure_InvalidOperation;

	if( mAccessType == TCFileManager::Access_WriteOnly )
		return Failure_InvalidAccess;

	//
	// Walk until we find our delimiter.
	//

	int character = 0;
	while( character != delimiter && character != EOF )
	{
		character = fgetc( mFile );
		char8 c = (char8)character;

		text += c;
	}

	return Success;
}

//
// Flush
//		- This will submit all changes to the file.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: We successfully flushed the changes.
//			- Failure_InvalidOperation: The file is not open.
//

TCResult TCFile_Win32::Flush()
{
	if( mFile == NULL )
		return Failure_InvalidOperation;

	int result = fflush( mFile );
	if( result != 0 )
	{
		gLogger->LogError( TCString( "[TCFile] Failed to flush file, error returned: " ) + ferror( mFile ) );
		return Failure_Unknown;
	}

	return Success;
}

//
// SeekRead
//		- Will move the read position.
// Inputs:
//		- unsigned int position: The position in bytes where to move to in the file.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: We seeked successfully.
//			- Failure_InvalidOperation: The file is not open.
//			- Failure_OutOfBounds: The specified position is not within the bounds of the file.
//

TCResult TCFile_Win32::SeekRead( unsigned int position )
{
	if( mFile == NULL )
		return Failure_InvalidOperation;

	if( position < 0 || position > mFileLength )
		return Failure_OutOfBounds;

	int result = fseek( mFile, position, SEEK_CUR );
	if( result != 0 )
	{
		gLogger->LogError( TCString( "[TCFile] Failed to seek in file, error returned" ) + ferror( mFile ) );
		return Failure_Unknown;
	}

	mReadPosition = position;
	mWritePosition = position;

	return Success;
}

//
// SeekWrite
//		- Will move the write position.
// Inputs:
//		- unsigned int position: The position in bytes where to move to in the file.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: We seeked successfully.
//			- Failure_InvalidOperation: The file is not open.
//			- Failure_OutOfBounds: The specified position is not within the bounds of the file.
//

TCResult TCFile_Win32::SeekWrite( unsigned int position )
{
	return SeekRead( position );
}

//
// Clone
//		- This function will copy another file.
// Inputs:
//		- const TCFile_Win32& inRef: The file to copy.
// Outputs:
//		- None.
//

void TCFile_Win32::Clone( const TCFile_Win32& inRef )
{
	mFile = inRef.mFile;
	TCFile::Clone( inRef );
}

//
// Open
//		- This function will open a file for use.
// Inputs:
//		- TCFileManager::FileDescription& desc: The information on how to open the file.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: We successfully opened the file.
//			- Failure_InvalidAccess: The file at the path specified is read only and we are trying to write at it.
//			- Failure_InvalidPath: The path was malformed or does not exist.
//

TCResult TCFile_Win32::Open( TCFileManager::FileDescription& desc )
{
	//
	// Make sure the description is valid.
	//

	if( !IsValidDescription( desc ) )
	{
		return Failure_InvalidParameter;
	}

	//
	// Make sure the file exists.
	//

	if( mFileManager->FileExists( desc.path ) == false )
	{
		return Failure_InvalidPath;
	}

	//
	// Make sure we have access to the file.
	//

	if( mFileManager->GetFileAccessType( desc.path ) & TCFileManager::FileAttribute_ReadOnly )
	{
		return Failure_InvalidAccess;
	}

	//
	// Determine the open mode.
	//

	TCString mode = GetModeString( desc );

	//
	//	Try and open the file.
	//

	errno_t error = fopen_s( &mFile, desc.path.Data(), mode.Data() );
	if( mFile == NULL )
	{
		return Failure_InvalidPath;
	}

	//
	// Get our file length.
	//

	struct stat statBuffer;
    int result = stat( desc.path.Data(), &statBuffer );
    mFileLength =  result == 0 ? statBuffer.st_size : -1;

	//
	// Store our description variables.
	//

	mIsOpen = true;
	mAccessType = desc.accessType;
	mOpenMode = desc.openMode;
	mDataType = desc.dataType;
	mFilepath = desc.path;

	//
	// Get our seek positions.
	//

	fpos_t position;
	fgetpos( mFile, &position );

	mReadPosition = (unsigned int)position;
	mWritePosition = (unsigned int)position;

	//
	// Store our name
	//
	
	int fileNameStartIndex = mFilepath.FindLastIndexOf( '/' ) + 1;
	if( fileNameStartIndex == 0 )
	{
		mFilename = mFilepath;
	}

	int fileExtensionStartIndex = mFilepath.FindFirstIndexOf( '.' ) - 1;
	if( fileExtensionStartIndex <= 0 )
	{
		mFilename = mFilepath;
	}

	mFilepath.Substring( fileNameStartIndex, fileExtensionStartIndex, mFilename );
}

//
// Close
//		- This function will close the file.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: the result of the operation.
//			- Success: We successfully closed the file.
//

TCResult TCFile_Win32::Close()
{
	if( mFile == NULL )
		return Success;

	fclose( mFile );
	mFile = NULL;

	return Success;
}

//
// IsValidDescription
//		- Will determine if the input description is valid.
// Inputs:
//		- TCFileManager::FileDescription& description: The description to check.
// Outputs:
//		- bool: Was the file description valid?
//

bool TCFile_Win32::IsValidDescription( TCFileManager::FileDescription& description )
{
	if( description.accessType == TCFileManager::Access_Unknown )
		return false;

	if( description.dataType == TCFileManager::FileDataType_Unknown )
		return false;

	if( description.openMode == TCFileManager::OpenMode_Unknown )
		return false;

	return true;
}

//
// GetModeString
//		- Will return the correct open mode for a POSIX file
// Inputs:
//		- TCFileManager::FileDescription& description: The description to create the mode from.
// Outputs:
//		- TCString: The read mode.
//

TCString TCFile_Win32::GetModeString( TCFileManager::FileDescription& description )
{
	TCString mode;
	if( description.accessType == TCFileManager::Access_ReadOnly )
	{
		mode = "r";		// Read only mode.
	}
	else if( description.accessType == TCFileManager::Access_ReadWrite )
	{
		if( description.openMode == TCFileManager::OpenMode_Append )
		{
			mode = "a";	// This append mode doesn't allow file seeking, is that good behavior to enforce?
		}
		else if( description.openMode == TCFileManager::OpenMode_Truncate )
		{
			mode = "w+";
		}
		else
		{
			mode = "r+";
		}
	}
	else if( description.accessType == TCFileManager::Access_WriteOnly )
	{
		if( description.openMode == TCFileManager::OpenMode_Truncate )
		{
			mode = "w+";
		}
		else
		{
			mode = "r+";	// Allow read write, just early out on our side.		
		}
	}

	//
	// Check if we are doing a binary mode.
	//

	if( description.dataType == TCFileManager::FileDataType_Binary )
	{
		mode += 'b';
	}

	return mode;
}