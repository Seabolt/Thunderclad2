//
// TCFileManager.h
// This file will define an object that will manage the lifetime of all files.
//

#ifndef __TC_FILE_MANAGER_H__
#define __TC_FILE_MANAGER_H__

//
// Includes
//

#include "TCResultCode.h"
#include "TCList.h"
#include "TCString.h"

//
// Defines
//

//
// Forward Declaration
//

class TCFile;

typedef unsigned int TCFileAttributeFlag;

//
// Class Declaration
//

class TCFileManager
{
	public:		// Members
		enum AccessType
		{
			Access_Unknown,
			Access_ReadOnly,
			Access_ReadWrite,
			Access_WriteOnly
		};

	
		static const int FileAttribute_Unknown			= 0x00000000;
		static const int FileAttribute_Normal			= 0x00000001;
		static const int FileAttribute_ReadOnly			= 0x00000010;
		static const int FileAttribute_Compressed		= 0x00000100;
		static const int FileAttribute_IsDirectory		= 0x00001000;
		static const int FileAttribute_Hidden			= 0x00010000;
		static const int FileAttribute_Archived			= 0x00100000;

		enum DataType
		{
			FileDataType_Unknown,
			FileDataType_Binary,
			FileDataType_Text
		};

		enum OpenMode
		{
			OpenMode_Unknown,
			OpenMode_Append,
			OpenMode_Truncate
		};

	public:		// Methods
									TCFileManager();
									TCFileManager( const TCFileManager& inRef );
		virtual TCFileManager&		operator=( const TCFileManager& inRef );
		virtual						~TCFileManager();
		
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
				void				SetResourceDirectory( TCString& path );
				TCString			GetResourceDirectory();
				void				SetEngineResourceDirectory( TCString& path );
				TCString			GetEngineResourceDirectory();

	protected:	// Members
		struct FileDescription
		{
			TCString	path;
			AccessType	accessType;
			DataType	dataType;
			OpenMode	openMode;
		};

		TCList< TCFile* >	mOpenedFiles;
		TCString			mResourceDirectory;
		TCString			mEngineResourceDirectory;
		TCString			mProgramDirectory;

	protected:	// Methods
		virtual void		Clone( const TCFileManager& inRef );

		friend class TCFile;
};

#endif // __TC_FILE_MANAGER_H__