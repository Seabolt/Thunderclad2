//
// TCFile.cpp
// This file will define the platform agnostic functionality for a file.
//

//
// Includes
//

#include "TCFile.h"

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

TCFile::TCFile( TCFileManager* fileManager )
{
	mFileManager = fileManager;
	mFileLength = 0;

	mAccessType = TCFileManager::Access_ReadOnly;
	mDataType = TCFileManager::FileDataType_Binary;
	mOpenMode = TCFileManager::OpenMode_Append;

	mIsOpen = false;
	mReadPosition = 0;
	mWritePosition = 0;

	mFilename = "";
	mFilepath = "";
}

//
// Copy Constructor
//		- This will copy another file.
// Inputs:
//		- const TCFile& inRef: The file to copy.
// Outputs:
//		- None.
//

TCFile::TCFile( const TCFile& inRef )
{
	Clone( inRef );
}

//
// Assignment Operator
//		- This will set this instance equal to another and return a reference to the new file.
// Inputs:
//		- const TCFile& inRef: The file to copy.
// Outputs:
//		- TCFile&: A reference to this file with the new data.
//

TCFile& TCFile::operator=( const TCFile& inRef )
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

TCFile::~TCFile()
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

TCResult TCFile::Write( void* data, unsigned int dataLength )
{
	TC_ASSERT( "This is the platform agnostic write, this should be overloaded." && 0 );
	return Failure_NotImplemented;
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

TCResult TCFile::Write( TCString text )
{
	TC_ASSERT( "This is the platform agnostic write, this should be overloaded." && 0 );
	return Failure_NotImplemented;
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

TCResult TCFile::Read( void** data, unsigned int dataLength )
{
	TC_ASSERT( "This is the platform agnostic read, this should be overloaded." && 0 );
	return Failure_NotImplemented;
}

//
// ReadLine
//		- This will allow the user to read a line from the file.
// Inputs:
//		- TCString text: The string to place the line.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: We read from the file successfully.
//			- Failure_InvalidAccess: The file is not read-able.
//			- Failure_InvalidOperation: The file is not open or the file is not treated as text.
//

TCResult TCFile::ReadLine( TCString& text )
{
	TC_ASSERT( "This is the platform agnostic read line, this should be overloaded." && 0 );
	return Failure_NotImplemented;
}

//
// ReadWord
//		- This will allow the user to read a word from a file.
// Inputs:
//		- TCString word: The string to place the word.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: We read from the file successfully.
//			- Failure_InvalidAccess: The file is not read-able.
//			- Failure_InvalidOperation: The file is not open or the file is not treated as text.
//

TCResult TCFile::ReadWord( TCString& word )
{
	TC_ASSERT( "This is the platform agnostic read word, this should be overloaded." && 0 );
	return Failure_NotImplemented;
}

//
// ReadChar
//		- This will read the next character out of the file.
// Inputs:
//		- char8 character: The character to fill.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: We read from the file successfully.
//			- Failure_InvalidAccess: The file is not read-able.
//			- Failure_InvalidOperation: The file is not open or the file is not treated as text.
//

TCResult TCFile::ReadChar( char8& character )
{
	TC_ASSERT( "This is the platform agnostic read, this should be overloaded." && 0 );
	return Failure_NotImplemented;
}

//
// ReadText
//		- This will allow the user to read until a specified delimiter.
// Inputs:
//		- TCString text: The string to place the text.
//		- char8 delimiter: The character that signifies when to stop.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: We read from the file successfully.
//			- Failure_InvalidAccess: The file is not read-able.
//			- Failure_InvalidOperation: The file is not open.
//

TCResult TCFile::ReadText( TCString& text, char8 delimiter )
{
	TC_ASSERT( "This is the platform agnostic read text, this should be overloaded." && 0 );
	return Failure_NotImplemented;
}

//
// ReadFile
//		- This will allow the user to read all the text from the file.
// Inputs:
//		- TCString& text: The string to place the text.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: We read from the file successfully.
//			- Failure_InvalidAccess: The file is not read-able.
//			- Failure_InvalidOperation: The file is not open.
//

TCResult TCFile::ReadFile( TCString& text )
{
	TC_ASSERT( "This is the platform agnostic read file, this should be overloaded." && 0 );
	return Failure_NotImplemented;
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

TCResult TCFile::Flush()
{
	TC_ASSERT( "This is the platform agnostic flush, this should be overloaded." && 0 );
	return Failure_NotImplemented;
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

TCResult TCFile::SeekRead( unsigned int position )
{
	TC_ASSERT( "This is the platform agnostic seek read, this should be overloaded." && 0 );
	return Failure_NotImplemented;
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

TCResult TCFile::SeekWrite( unsigned int position )
{
	TC_ASSERT( "This is the platform agnostic seek write, this should be overloaded." && 0 );
	return Failure_NotImplemented;
}

//
// GetReadPosition
//		- Will return the current read position in the file.
// Inputs:
//		- None
// Outputs:
//		- unsigned int: the current position.
//

unsigned int TCFile::GetReadPosition()
{
	return 0;
}

//
// GetWritePosition
//		- Will return the current write position in the file.
// Inputs:
//		- None.
// Outputs:
//		- unsigned int: The current position.
//

unsigned int TCFile::GetWritePosition()
{
	return 0;
}

//
// Clone
//		- This function will copy another file.
// Inputs:
//		- const TCFile& inRef: The file to copy.
// Outputs:
//		- None.
//

void TCFile::Clone( const TCFile& inRef )
{
	mFilepath = inRef.mFilepath;
	mFilename = inRef.mFilename;

	mAccessType = inRef.mAccessType;
	mDataType = inRef.mDataType;
	mOpenMode = inRef.mOpenMode;

	mFileLength = inRef.mFileLength;
	mFileManager = inRef.mFileManager;

	mReadPosition = inRef.mReadPosition;
	mWritePosition = inRef.mWritePosition;

	mIsOpen = inRef.mIsOpen;
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

TCResult TCFile::Open( TCFileManager::FileDescription& desc )
{
	TC_ASSERT( "This is the platform agnostic file open, this should be overloaded." && 0 );
	return Failure_NotImplemented;
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

TCResult TCFile::Close()
{
	TC_ASSERT( "This is the platform agnostic file close, this should be overloaded." && 0 );
	return Failure_NotImplemented;
}

//
// GetEndOfFileCharacter
//		- This function will determine the platform End-of-File character
// Inputs:
//		- None.
// Outputs:
//		- None.
//

char TCFile::GetEofCharacter()
{
	return (char)-1;
}