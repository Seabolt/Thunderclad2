//
// TCFileManager.win32.h
// This file will declare the interface for a Win32 version of the file manager.
//

#ifndef __TC_FILE_MANAGER_WIN_32_H__
#define __TC_FILE_MANAGER_WIN_32_H__

//
// Includes
//

#include "TCFileManager.h"

//
// Defines
//

//
// Forward Declarations
//

class TCFile_Win32;

//
// Class Declaration
//

class TCFileManager_Win32 :
	public TCFileManager
{
	public:		// Members
	public:		// Methods
									TCFileManager_Win32();
									TCFileManager_Win32( const TCFileManager_Win32& inRef );
		virtual TCFileManager&		operator=( const TCFileManager& inRef );
		virtual						~TCFileManager_Win32();
		
		virtual TCResult			Initialize();
		virtual void				Destroy();

		virtual TCResult			CreateFile( const TCString& path,	
												TCFile** filePointer,	
												AccessType accessType = Access_ReadWrite,	
												DataType dataType = FileDataType_Binary );

		virtual TCResult			OpenFile( const TCString& path,		
											  TCFile** filePointer,		
											  AccessType accessType = Access_ReadWrite,	
											  DataType dataType = FileDataType_Binary,		
											  OpenMode openMode = OpenMode_Append );		

		virtual TCResult			CloseFile( TCFile* file );
		virtual TCResult			DeleteFile( const TCString& path );
		virtual TCResult			CreateDirectory( const TCString& path );
		virtual TCResult			DeleteDirectory( const TCString& path );
		virtual TCResult			CopyFile( const TCString& pathToFile, const TCString& pathToDestination );
		virtual TCResult			EnumerateDirectory( const TCString& pathToDirectory, TCList< TCString >& files, TCList< TCString >& directories );
		virtual TCFileAttributeFlag	GetFileAccessType( const TCString& path );
		virtual bool				FileExists( const TCString& path );
		virtual bool				DirectoryExists( const TCString& path );
		virtual TCString			GetRootDirectory();
		virtual TCString			GetProgramDirectory();

	protected:	// Members
	protected:	// Methods
		virtual void		Clone( const TCFileManager& inRef );

		friend class TCFile_Win32;
};

#endif // __TC_FILE_MANAGER_WIN_32_H__