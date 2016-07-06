//
// TCFileManager.win32.cpp
// This file will define the Win32 functionality of a file manager.
//

//
// Includes
//

#include "TCFileManager.win32.h"
#include "TCFile.win32.h"
#include "TCLogger.h"
#include <Windows.h>

//
// Defines
//

#undef CreateFile
#undef DeleteFile
#undef CreateDirectory
#undef CopyFile

//
// Default Constructor
//		- Will initialize this object to a safe state.
// Inputs:
//		- None.
// Outputs:
//		- None.
//
TCFileManager_Win32::TCFileManager_Win32()
{

}

//
// Copy Constructor
//		- Will initialize this object as a copy of another TCFileManager.
// Inputs:
//		- const TCFileManager& inRef: The file manager to copy.
// Outputs:
//		- None.
//

TCFileManager_Win32::TCFileManager_Win32( const TCFileManager_Win32& inRef )
{
	Clone(inRef);
}

//
// Assignment Operator
//		- Will set this instance equal to another instance
// Inputs:
//		- const TCFileManager& inRef: The file manager to copy.
// Outputs:
//		- None.
//

TCFileManager& TCFileManager_Win32::operator=( const TCFileManager& inRef )
{
	Destroy();
	Clone( inRef );

	return (*this);
}

//
// Destructor
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCFileManager_Win32::~TCFileManager_Win32()
{
	Destroy();
}

//
// Initialize
//		- Will initialize all members to be ready for use.
// Inputs:
//		- None.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The file manager initialized successfully.
//

TCResult TCFileManager_Win32::Initialize()
{
	return TCFileManager::Initialize();
}

//
// Destroy
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCFileManager_Win32::Destroy()
{
	for( int currentOpenedFile = 0; currentOpenedFile < mOpenedFiles.Count(); ++currentOpenedFile )
	{
		if( mOpenedFiles[ currentOpenedFile ] != NULL )
		{
			CloseFile( mOpenedFiles[ currentOpenedFile ] );
			delete (TCFile_Win32*)mOpenedFiles[ currentOpenedFile ];
		}
	}

	mOpenedFiles.Clear();
}

//
// CreateNewFile
//		- Will create a new file on disk.
// Inputs:
//		- const TCString& path: The path for the new file to be created at.
//		- TCFile** filePointer: The pointer to set the created file pointer at.
//		- TCFileManager::AccessType: The read/write access desired for the file.
//		- TCFileManager::DataType: How should the file be treated.
// Outputs:
//		- TCFile** filePointer: The pointer to the newly created file.
//		- TCResult: The result of the operation.
//			- Success: The file was created successfully.
//			- Failure_InvalidPath: The path was malformed or didn't point to an existing directory.
//			- Failure_AlreadyExists: There already is a file at the path specified.
//

TCResult TCFileManager_Win32::CreateFile( const TCString& path,	
										  TCFile** filePointer,	
										  TCFileManager::AccessType accessType,	
										  TCFileManager::DataType dataType )
{
	//
	// Make sure we have a valid file pointer.
	//

	if( filePointer == NULL )
	{
		return Failure_InvalidParameter;	
	}

	//
	// Verify that the file doesn't already exist.
	//

	if( FileExists( path ) )
	{
		return Failure_AlreadyExists; 
	}

	//
	// Determine our access flags.
	//
	
	DWORD sysAccessFlags = 0;
	switch( accessType )
	{
		case TCFileManager::Access_ReadOnly:
			sysAccessFlags = GENERIC_READ;
			break;

		case TCFileManager::Access_ReadWrite:
			sysAccessFlags = GENERIC_READ | GENERIC_WRITE;
			break;

		case TCFileManager::Access_WriteOnly:
			sysAccessFlags = GENERIC_WRITE;
			break;
	}

	//
	// Determine our share flags.
	//

	DWORD sysShareFlags = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
	
	//
	// Determine our creation flags
	//

	DWORD sysCreationFlags = CREATE_NEW;

	//
	// Create the file!
	//

	HANDLE hFile = CreateFileA( path.Data(),
							    sysAccessFlags,
							    sysShareFlags,
							    NULL,
							    sysCreationFlags,
							    FILE_ATTRIBUTE_NORMAL,
							    NULL );

	//
	// Check for error!
	//

	if( hFile == INVALID_HANDLE_VALUE )
	{
		TCString errorString = "[TCFile] Failed to create a file named: ";
		errorString += path;
		errorString += ". GetLastError() returned: ";

		DWORD lastError = GetLastError();
		errorString += (int)lastError;

		gLogger->LogError( errorString );
		return Failure_InvalidPath;
	}

	//
	// Create a new file!
	//

	TCFile_Win32* file = new TCFile_Win32( this );
	
	FileDescription description;
	description.accessType	= accessType;
	description.dataType	= dataType;
	description.openMode	= TCFileManager::OpenMode_Truncate;
	description.path		= path;

	//
	// Open the file.
	//

	TCResult result = file->Open( description );
	if( TC_FAILED( result ) )
	{
		gLogger->LogError( "[TCFile] Failed to open the newly created file." );
		return result;
	}

	//
	// Store the file, and return success.
	//

	*filePointer = file;
	mOpenedFiles.Append( file );

	return Success;
}

//
// OpenFile
//		- This function will return a TCFile* to an opened file.
// Inputs:
//		- const TCString& path: The path to the file to open.
//		- TCFile** filePointer: The pointer to a file pointer to fill.
//		- TCFileManager::AccessType accessType: The read/write permissions requested.
//		- TCFileManager::DataType dataType: How should the file data be treated?
//		- TCFileManager::OpenMode openMode: How should we open the file.
// Outputs:
//		- TCFile** filePointer: A pointer to an allocated TCFile that is opened.
//		- TCResult: The result of the operation.
//			- Success: The file was created successfully.
//			- Failure_InvalidPath: The path was malformed or didn't point to an existing directory.
//			- Failure_InvalidAccess: We tried to open the file with invalid permissions.
//

TCResult TCFileManager_Win32::OpenFile( const TCString& path,		
									    TCFile** filePointer,		
										TCFileManager::AccessType accessType,	
										TCFileManager::DataType dataType,		
										TCFileManager::OpenMode openMode )
{
	if( filePointer == NULL )
		return Failure_InvalidParameter;

	TCFile_Win32* file = new TCFile_Win32( this );
	
	FileDescription fileDesc;
	fileDesc.accessType = accessType;
	fileDesc.dataType = dataType;
	fileDesc.openMode = openMode;
	fileDesc.path = path;

	*filePointer = file;

	return file->Open( fileDesc );
}

//
// CloseFile
//		- Will close the access to the file and de-allocate it.
// Inputs:
//		- TCFile* file: The file to close.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The file was closed successfully.
//			- Failure_InvalidParam: The file was NULL.
//

TCResult TCFileManager_Win32::CloseFile( TCFile* file )
{
	//
	// Ensure that the file is not NULL.
	//

	if( file == NULL )
		return Failure_InvalidParameter;

	//
	// Call the file's close.
	// 

	TCFile_Win32* winFile = (TCFile_Win32*)file;
	return winFile->Close();
}

//
// DeleteFile
//		- This will delete a file from disk.
// Inputs:
//		- TCString& path: The path to the file to delete.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The file was deleted successfully.
//			- Failure_InvalidPath: The path was malformed.
//

TCResult TCFileManager_Win32::DeleteFile( const TCString& path )
{
	if( !FileExists( path ) )
	{
		return Failure_InvalidPath;
	}

	BOOL result = DeleteFileA( path.Data() );
	if( result == FALSE )
	{
		gLogger->LogError( TCString( "[TCFile] Failed to remove a file: " ) + path.Data() );
		return Failure_InvalidParameter;
	}

	return Success;
}

//
// CreateDirectory
//		- Will create a new directory on disk.
// Inputs:
//		- TCString& path: The path to where the directory should be created.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The directory was created successfully.
//			- Failure_InvalidPath: The path pointed to an invalid location.
//			- Failure_AlreadyExists: There is already a directory at the path specified.
//

TCResult TCFileManager_Win32::CreateDirectory( const TCString& path )
{
	if( DirectoryExists( path ) )
	{
		return Failure_AlreadyExists;
	}

	BOOL result = CreateDirectoryA( path.Data(), NULL );
	if( result == FALSE )
	{
		DWORD lastError = GetLastError();
		gLogger->LogError( TCString( "[TCFile] Failed to create a directory at: " ) + path.Data() + TCString( ". Last error: " ) + (int)lastError );
		return Failure_InvalidPath;
	}

	return Success;
}

//
// DeleteDirectory
//		- Will remove a directory and all it's contents from disk.
// Inputs:
//		- TCString& path: The path to where the directory should be created.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The directory was deleted successfully.
//			- Failure_InvalidPath: The path pointed to an invalid location.
//			- Failure_InvalidAccess: The application didn't have the permission to delete the directory.
//

TCResult TCFileManager_Win32::DeleteDirectory( const TCString& path )
{
	if( !DirectoryExists( path ) )
	{
		gLogger->LogError( TCString( "[TCFile] Failed to delete directory named: " ) + path );
		return Failure_InvalidPath;
	}
	
	//
	// Make sure you double terminate the filepath.
	//

	char pathToUse[ MAX_PATH + 1 ];
	TCStringUtils::Copy( pathToUse, path.Data() );
	pathToUse[ path.Length() + 1 ] = NULL_TERMINATOR;

	//
	// Launch a shell operation to remove the directory silently and without UI.
	//

	SHFILEOPSTRUCT shellOp = {
        NULL,
        FO_DELETE,
        pathToUse,
        NULL,
        FOF_NOCONFIRMATION |
        FOF_NOERRORUI |
        FOF_SILENT,
        false,
        0,
        "" };

    int result = SHFileOperation(&shellOp);
	if( result != 0 )
	{
		if( result == 2 )	// FILE_NOT_FOUND_ERROR
		{
			return Failure_FileNotFound;
		}
		else
		{
			return Failure_Unknown;		
		}
	}

	return Success;
}

//
// CopyFile
//		- Will copy a file from one location to another location.
// Inputs:
//		- TCString& pathToFile: The path to the file to be copied.
//		- TCString& pathToDestination: The path to where the file should be copied.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The file was copied successfully.
//			- Failure_InvalidParameter: The pathToFile and pathToDestination were the same.
//			- Failure_InvalidPath: Either the pathToFile or pathToDestination were malformed.
//			- Failure_InvalidAccess: The application didn't have permission to move the file.
//

TCResult TCFileManager_Win32::CopyFile( const TCString& pathToFile, const TCString& pathToDestination )
{
	//
	// Ensure the target and destination are not the same.
	//

	if( pathToFile.Data() == pathToDestination.Data() )
	{
		gLogger->LogError( "[TCFile] Failed to copy file, target and destination path are the same.");
		return Failure_InvalidParameter;
	}

	//
	// Ensure the target file exists.
	//

	if( !FileExists( pathToFile.Data() ) )
	{
		gLogger->LogError( TCString("[TCFile] Failed to copy file, target file doesn't exist! ") + pathToFile.Data() );
		return Failure_InvalidParameter;
	}

	//
	// Copy the file!
	//

	BOOL result = CopyFileA( pathToFile.Data(), pathToDestination.Data(), FALSE );
	if( result == FALSE )
	{
		gLogger->LogError( "[TCFile] Failed to copy the file!" );
		return Failure_InvalidAccess;
	}

	return Success;
}

//
// EnumerateDirectory
//		- Will build a list of all the files in a specified directory.
// Inputs:
//		- TCString& pathToDirectory: The path to the directory to enumerate.
//		- TCList< TCString >& files: The files in the directory.
//		- TCList< TCString >& directories: The directories in the directory.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: Succesfully enumerated the directory.
//			- Failure_InvalidPath: The path to the directory was malformed.
//

TCResult TCFileManager_Win32::EnumerateDirectory( const TCString& pathToDirectory, TCList< TCString >& files, TCList< TCString >& directories )
{
	files.Clear();
	directories.Clear();

	//
	// Make sure the directory exists.
	//

	if( !DirectoryExists( pathToDirectory.Data() ) )
	{
		gLogger->LogError( TCString( "[TCFile] Failed to enumerate directory, invalid path provided to: " ) + pathToDirectory.Data() );
		return Failure_InvalidPath;
	}

	//
	// Get the files in the directory.
	//

	WIN32_FIND_DATA findData;
	HANDLE hFoundFile = FindFirstFileA( pathToDirectory.Data(), & findData );
	if( hFoundFile == INVALID_HANDLE_VALUE )
	{
		DWORD error = GetLastError();
		gLogger->LogError( TCString( "[TCFile] Failed to enumerate directory, last error: " ) + (int)error );
		return Failure_Unknown;
	}

	//
	// Walk all the files in the directory.
	//

	do
	{
		if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			directories.Append( findData.cFileName );
		}
		else if( findData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL )
		{
			files.Append( findData.cFileName );
		}
	}
	while( FindNextFile( hFoundFile, &findData ) != 0 );

	//
	// Ensure that there were no errors.
	//

	DWORD error = GetLastError();
	if( error != ERROR_NO_MORE_FILES )
	{
		gLogger->LogError( TCString( "[TCFile] Failed when enumerating the directory, last error: " ) + (int)error );
		return Failure_Unknown;
	}

	FindClose( hFoundFile );
	return Success;
}

//
// GetFileAccessType
//		- Will determine the access type of a file on disk.
// Inputs:
//		- TCString& path: The path to the file to check.
// Outputs:
//		- TCFileManager::FileAttribute: The access type for the specified file.
//

TCFileAttributeFlag TCFileManager_Win32::GetFileAccessType( const TCString& path )
{
	DWORD fileAttributes = GetFileAttributes( path.Data() );
	TCFileAttributeFlag outFileAttributes = FileAttribute_Unknown;

	if( fileAttributes & FILE_ATTRIBUTE_READONLY )
	{
		outFileAttributes |= FileAttribute_ReadOnly;
	}
	else if( fileAttributes & FILE_ATTRIBUTE_ARCHIVE )
	{
		outFileAttributes |= FileAttribute_Archived;
	}
	else if( fileAttributes & FILE_ATTRIBUTE_COMPRESSED )
	{
		outFileAttributes |= FileAttribute_Compressed;
	}
	else if( fileAttributes & FILE_ATTRIBUTE_DIRECTORY )
	{
		outFileAttributes |= FileAttribute_IsDirectory;
	}
	else if( fileAttributes |= FILE_ATTRIBUTE_HIDDEN )
	{
		outFileAttributes |= FileAttribute_Hidden;
	}
	else
	{
		outFileAttributes |= FileAttribute_Normal;
	}

	return outFileAttributes;
}

//
// FileExists
//		- Will return if a file exists at the path specified.
// Inputs:
//		- TCString& path: The path to a file to check.
// Outputs:
//		- bool: Does the file exist.
//

bool TCFileManager_Win32::FileExists( const TCString& path )
{
	DWORD fileAttributes = GetFileAttributes( path.Data() );
	if( fileAttributes == INVALID_FILE_ATTRIBUTES || (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		return false;

	return true;
}

//
// DirectoryExists
//		- Will return if a directory exists at the path specified.
// Inputs:
//		- TCString& path: The path to a directory to check.
// Outputs:
//		- bool: Does the directory exists.
//

bool TCFileManager_Win32::DirectoryExists( const TCString& path )
{
	DWORD fileAttributes = GetFileAttributes( path.Data() );
	if( fileAttributes == INVALID_FILE_ATTRIBUTES )
		return false;

	if( (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
		return true;

	return false;
}

//
// GetRootDirectory
//		- Will return a string representing the root directory of a platform.
// Inputs:
//		- None.
// Outputs:
//		- TCString: the root directory.
//

TCString TCFileManager_Win32::GetRootDirectory()
{
	return TCString( "C:/" );
}

//
// GetProgramDirectory
//		- Will return a string representing the program directory for the application.
// Inputs:
//		- None.
// Outputs:
//		- TCString: The program directory
//

TCString TCFileManager_Win32::GetProgramDirectory()
{
	if( mProgramDirectory.IsEmpty() )
	{
		char buffer[ _MAX_PATH ];
		char* bufferPtr = buffer;
		if( _get_pgmptr( &bufferPtr ) == 0 )
		{
			mProgramDirectory = bufferPtr;	// This will get us a path to the executable.
			int directoryStartIndex = mProgramDirectory.FindLastIndexOf( '\\' );
			mProgramDirectory.Substring( 0, directoryStartIndex - 1, mProgramDirectory );
			mProgramDirectory.ReplaceAllInstances( '\\', '/' );
		}
	}

	return mProgramDirectory;
}

//
// Clone
//		- Will copy another TCFileManager&
// Inputs:
//		- const TCFileManager& inRef: The reference to copy.
// Outputs:
//		- None.
//

void TCFileManager_Win32::Clone( const TCFileManager& inRef )
{
	TCFileManager::Clone( inRef );
}