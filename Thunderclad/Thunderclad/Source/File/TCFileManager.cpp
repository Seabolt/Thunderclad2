//
// TCFileManager.cpp
// This file will define the platform agnostic functionality of a file manager.
//

//
// Includes
//

#include "TCFileManager.h"
#include "TCFile.h"
#include "TCLogger.h"

//
// Defines
//

//
// Default Constructor
//		- Will initialize this object to a safe state.
// Inputs:
//		- None.
// Outputs:
//		- None.
//
TCFileManager::TCFileManager()
{
	mResourceDirectory = "";
	mEngineResourceDirectory = "";
	mProgramDirectory = "";
}

//
// Copy Constructor
//		- Will initialize this object as a copy of another TCFileManager.
// Inputs:
//		- const TCFileManager& inRef: The file manager to copy.
// Outputs:
//		- None.
//

TCFileManager::TCFileManager( const TCFileManager& inRef )
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

TCFileManager& TCFileManager::operator=( const TCFileManager& inRef )
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

TCFileManager::~TCFileManager()
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

TCResult TCFileManager::Initialize()
{
	TCResult result = Success;

	//
	// Initialize our engine directory.
	//

	mEngineResourceDirectory = GetProgramDirectory() + "/Thunderclad Resources/";
	if( !DirectoryExists( mEngineResourceDirectory ) )
	{
		result = CreateDirectory( mEngineResourceDirectory );
		if( TC_FAILED( result ) )
		{
			gLogger->LogFailure( "[TCFile] Failed to create engine resource directory!" );
			return result;
		}
	}

	return Success;
}

//
// Destroy
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCFileManager::Destroy()
{
	for( int currentOpenedFile = 0; currentOpenedFile < mOpenedFiles.Count(); ++currentOpenedFile )
	{
		if( mOpenedFiles[ currentOpenedFile ] != NULL )
		{
			CloseFile( mOpenedFiles[ currentOpenedFile ] );
			delete mOpenedFiles[ currentOpenedFile ];
		}
	}

	mOpenedFiles.Clear();
}

//
// CreateFile
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

TCResult TCFileManager::CreateFile( const TCString& path,	
									TCFile** filePointer,	
									TCFileManager::AccessType accessType,	
									TCFileManager::DataType dataType )
{
	TC_ASSERT( "This is the platform agnostic layer, this should be overwritten." && 0 );
	return Failure_NotImplemented;
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

TCResult TCFileManager::OpenFile( const TCString& path,		
								  TCFile** filePointer,		
								  TCFileManager::AccessType accessType,	
								  TCFileManager::DataType dataType,		
								  TCFileManager::OpenMode openMode )
{
	TC_ASSERT( "This is the platform agnostic layer, this should be overwritten." && 0 );
	return Failure_NotImplemented;
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

TCResult TCFileManager::CloseFile( TCFile* file )
{
	//
	// Ensure that the file is not NULL.
	//

	if( file == NULL )
		return Failure_InvalidParameter;

	//
	// Call the file's close.
	// 

	TCResult result =  file->Close();
	
	//
	// Release the file and return the result.
	//

	delete file;
	return result;
}

//
// DeleteFile
//		- This will remove a file from disk.
// Inputs:
//		- TCString& path: The path to the file to delete.
// Outputs:
//		- TCResult: The result of the operation.
//			- Success: The file was deleted successfully.
//			- Failure_InvalidPath: The path was malformed.
//

TCResult TCFileManager::DeleteFile( const TCString& path )
{
	TC_ASSERT( "This is the platform agnostic layer, this should be overwritten." && 0 );
	return Failure_NotImplemented;
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

TCResult TCFileManager::CreateDirectory( const TCString& path )
{
	TC_ASSERT( "This is the platform agnostic layer, this should be overwritten." && 0 );
	return Failure_NotImplemented;
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

TCResult TCFileManager::DeleteDirectory( const TCString& path )
{
	TC_ASSERT( "This is the platform agnostic layer, this should be overwritten." && 0 );
	return Failure_NotImplemented;
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

TCResult TCFileManager::CopyFile( const TCString& pathToFile, const TCString& pathToDestination )
{
	TC_ASSERT( "This is the platform agnostic layer, this should be overwritten." && 0 );
	return Failure_NotImplemented;
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

TCResult TCFileManager::EnumerateDirectory( const TCString& pathToDirectory, TCList< TCString >& files, TCList< TCString >& directories )
{
	TC_ASSERT( "This is the platform agnostic layer, this should be overwritten." && 0 );
	return Failure_NotImplemented;
}

//
// GetFileAccessType
//		- Will determine the access type of a file on disk.
// Inputs:
//		- TCString& path: The path to the file to check.
// Outputs:
//		- TCFileAttributeFlag: The access type for the specified file.
//

TCFileAttributeFlag TCFileManager::GetFileAccessType( const TCString& path )
{
	TC_ASSERT( "This is the platform agnostic layer, this should be overwritten." && 0 );
	return Access_Unknown;
}

//
// FileExists
//		- Will return if a file exists at the path specified.
// Inputs:
//		- TCString& path: The path to a file to check.
// Outputs:
//		- bool: Does the file exist.
//

bool TCFileManager::FileExists( const TCString& path )
{
	TC_ASSERT( "This is the platform agnostic layer, this should be overwritten." && 0 );
	return false;
}

//
// DirectoryExists
//		- Will return if a directory exists at the path specified.
// Inputs:
//		- TCString& path: The path to a directory to check.
// Outputs:
//		- bool: Does the directory exists.
//

bool TCFileManager::DirectoryExists( const TCString& path )
{
	TC_ASSERT( "This is the platform agnostic layer, this should be overwritten." && 0 );
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

TCString TCFileManager::GetRootDirectory()
{
	TC_ASSERT( "This is the platform agnostic layer, this should be overwritten." && 0 );
	return "";
}

//
// GetProgramDirectory
//		- Will return the current directory where the application is.
// Inputs:
//		- None.
// Outputs:
//		- TCString: The current directory.
//

TCString TCFileManager::GetProgramDirectory()
{
	TC_ASSERT( "This is the platform agnostic layer, this should be overwritten." && 0 );
	return "";
}

//
// SetResourceDirectory
//		- Will set the resource directory for an application.
// Inputs:
//		- TCString& path: The path to the resource directory for the application.
// Outputs:
//		- None.
//

void TCFileManager::SetResourceDirectory( TCString& path )
{
	if( DirectoryExists( path ) )
	{
		mResourceDirectory = path;
	}
}

//
// GetResourceDirectory
//		- Will return the resource directory for an application.
// Inputs:
//		- None.
// Outputs:
//		- TCString: the resource directory.
//

TCString TCFileManager::GetResourceDirectory()
{
	return mResourceDirectory;
}

//
// SetEngineResourceDirectory
//		- Will set the resource directory for the engine.
// Inputs:
//		- TCString& path: The path to the directory for the engine.
// Outputs:
//		- None.
//

void TCFileManager::SetEngineResourceDirectory( TCString& path )
{
	if( DirectoryExists( path ) )
	{
		mEngineResourceDirectory = path;
	}
}

//
// GetEngineResourceDirectory
//		- Will return the resource directory for the engine.
// Inputs:
//		- None.
// Outputs:
//		- TCString: The resource directory.
//

TCString TCFileManager::GetEngineResourceDirectory()
{
	return mEngineResourceDirectory;
}

//
// Clone
//		- Will copy another TCFileManager&
// Inputs:
//		- const TCFileManager& inRef: The reference to copy.
// Outputs:
//		- None.
//

void TCFileManager::Clone( const TCFileManager& inRef )
{
	mEngineResourceDirectory = inRef.mEngineResourceDirectory;
	mResourceDirectory = inRef.mResourceDirectory;
	mOpenedFiles = inRef.mOpenedFiles;
}