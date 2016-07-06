//
// TCFile_UnitTest.cpp
// This will define the testing for a file.
//

//
// Includes
//

#include "TCFile_UnitTest.h"
#include "TCLogger.h"
#include "TCFile.h"

//
// Defines
//

//
// Defines
//

#define RETURN_UNIT_TEST_FAILURE( x ) { TCLogger::GetInstance()->LogError( TCString("[TCFile_UnitTest] ") + x ); return TCUnitTest::TestResult_Failed; }

//
// Default Constructor
//

TCFile_UnitTest::TCFile_UnitTest( TCFileManager* managerToTest )
{
	mManagerToTest = managerToTest;
	TC_ASSERT( managerToTest );
}

//
// StartTest
//		- This function will run the unit test for this module.
// Inputs:
//		- None.
// Outputs:
//		- TCUnitTest::Result: The result of the operation.
//

TCUnitTest::Result TCFile_UnitTest::StartTest()
{
	if( mManagerToTest == NULL )
	{
		RETURN_UNIT_TEST_FAILURE( "Can't test without a file manager." );
	}

	//
	// First we want to create a directory for our test.
	//
	
	gLogger->LogInfo( "[TCFile_UnitTest] Creating test directory." );

	TCString testDirectoryPath = mManagerToTest->GetEngineResourceDirectory() + TCString( "Test Directory" );

	//
	// Delete the previous directory, just in case.
	//

	mManagerToTest->DeleteDirectory( testDirectoryPath );
	
	TCResult result = mManagerToTest->CreateDirectory( testDirectoryPath );	
	if( TC_FAILED( result ) )
	{
		RETURN_UNIT_TEST_FAILURE( "Failed to create test directory!" );
	}

	//
	// Now we need to populate the directory with a bunch of files.
	//

	TCString testFilePath = testDirectoryPath + "/TestFile";
	TCList< TCFile* > testFiles;
	for( int currentFile = 0; currentFile < 100; ++currentFile )
	{
		TCString currentFilePath = testFilePath + currentFile;
		currentFilePath += ".txt";
		TCFile* fileCreated = NULL;

		gLogger->LogInfo( TCString( "[TCFile_UnitTest] Creating test file: " ) + currentFile );

		TCResult result = mManagerToTest->CreateFile( currentFilePath,
													  &fileCreated,
													  TCFileManager::Access_ReadWrite,
													  TCFileManager::FileDataType_Text );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to create a file!" );
		}

		testFiles.Append( fileCreated );
	}

	//
	// Now let's write some text to them.
	//

	for( int currentFile = 0; currentFile < testFiles.Count(); ++currentFile )
	{
		gLogger->LogInfo( TCString( "[TCFile_UnitTest] Testing file write on file: " ) + currentFile );

		if( testFiles[ currentFile ] != NULL )
		{
			TCResult result;
			result = testFiles[ currentFile ]->Write( TCString("Hello world! ") + currentFile + TCString( "\n" ) );					if( TC_FAILED( result ) ) { RETURN_UNIT_TEST_FAILURE( "Failed to write to a text file!" ); }
			result = testFiles[ currentFile ]->Write( "How much wood could a woodchuck chuck," );									if( TC_FAILED( result ) ) { RETURN_UNIT_TEST_FAILURE( "Failed to write to a text file!" ); }
			result = testFiles[ currentFile ]->Write( "if a woodchuck could chuck wood?\n" );										if( TC_FAILED( result ) ) { RETURN_UNIT_TEST_FAILURE( "Failed to write to a text file!" ); }
			result = testFiles[ currentFile ]->Write( "Well a woodchuck can chuck wood, so we'll never know " );					if( TC_FAILED( result ) ) { RETURN_UNIT_TEST_FAILURE( "Failed to write to a text file!" ); }
			result = testFiles[ currentFile ]->Write( "how much wood could a woodchuck chuck if a woodchuck could chuck wood.\n" );	if( TC_FAILED( result ) ) { RETURN_UNIT_TEST_FAILURE( "Failed to write to a text file!" ); }
			result = testFiles[ currentFile ]->Flush(); if( TC_FAILED( result ) ) { RETURN_UNIT_TEST_FAILURE( "Failed to flush writes to the file." ); }

			//
			// Close each file as well.
			//

			gLogger->LogInfo( TCString( "[TCFile_UnitTest] Testing file close on file: " ) + currentFile );
			result = mManagerToTest->CloseFile( testFiles[ currentFile ] ); if( TC_FAILED( result ) ) { RETURN_UNIT_TEST_FAILURE( "Failed to close the file." ); }
			testFiles[ currentFile ] = NULL;
		}
	}

	//
	// Now re-open the files and see if we can read from it.
	//

	for( int currentFile = 0; currentFile < testFiles.Count(); ++currentFile )
	{
		TCFile* file = NULL;
		TCString currentFilePath = testFilePath + currentFile;
		currentFilePath += ".txt";
		
		//
		// Open the file.
		//

		gLogger->LogInfo( TCString( "[TCFile_UnitTest] Testing file open on file: " ) + currentFile );

		result = mManagerToTest->OpenFile( currentFilePath,
										   &file,
										   TCFileManager::Access_ReadOnly,
										   TCFileManager::FileDataType_Text,
										   TCFileManager::OpenMode_Append );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to reopen a file after having written to it." );
		}

		//
		// Store the reopened file.
		//

		testFiles[ currentFile ] = file;

		//
		// Now try to write the the file, make sure that it fails.
		//

		gLogger->LogInfo( TCString( "[TCFile_UnitTest] Testing file write on read only file: " ) + currentFile );
		result = testFiles[ currentFile ]->Write( "Fuck you." );
		if( TC_SUCCEEDED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed the read only test on a file." );
		}

		//
		// Now we read from the file and make sure that we are reading the correct data.
		//

		gLogger->LogInfo( TCString( "[TCFile_UnitTest] Testing file read on file: " ) + currentFile );

		TCString line;
		result = testFiles[ currentFile ]->ReadLine( line );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to read line from the file." );
		}

		TCString compareLineTo = TCString("Hello world! ") + currentFile;
		if( line != compareLineTo )
		{
			RETURN_UNIT_TEST_FAILURE( "Line read was not correct." );
		}

		//
		// Test read text.
		//

		compareLineTo = "How much wood could a woodchuck chuck";
		result = testFiles[ currentFile ]->ReadText( line, ',' );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to read the text." );
		}

		if( line != compareLineTo )
		{
			RETURN_UNIT_TEST_FAILURE( "Text read was not correct." );
		}

		//
		// Test read word.
		//

		compareLineTo = "if";
		result = testFiles[ currentFile ]->ReadWord( line );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to read the word.");
		}

		if( line != compareLineTo )
		{
			RETURN_UNIT_TEST_FAILURE( "Word read was not correct." );
		}

		//
		// Test seek backwards.
		//

		testFiles[ currentFile ]->SeekRead( testFiles[ currentFile ]->GetReadPosition() - line.Length() - 1 );

		//
		// Read the next line.
		//

		compareLineTo = "if a woodchuck could chuck wood?";
		result = testFiles[ currentFile ]->ReadLine( line );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to read the next line." );
		}

		if( line != compareLineTo )
		{
			RETURN_UNIT_TEST_FAILURE( "Line read was not correct." );
		}

		//
		// Read the last line.
		//

		compareLineTo = "Well a woodchuck can chuck wood, so we'll never know how much wood could a woodchuck chuck if a woodchuck could chuck wood.";
		result = testFiles[ currentFile ]->ReadText( line, '\n' );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to read the line." );
		}

		if( line != compareLineTo )
		{
			RETURN_UNIT_TEST_FAILURE( "The line read was incorrect." );
		}
	}

	//
	// Now we need to test copying the files over to a new directory.
	//

	gLogger->LogInfo( TCString( "[TCFile_UnitTest] Creating new test directory: " ) );

	TCString newDirectoryName = mManagerToTest->GetEngineResourceDirectory() + TCString( "Test Directory 2" );

	//
	// Delete the previous directory, just in case.
	//

	mManagerToTest->DeleteDirectory( newDirectoryName );

	//
	// Create the directory
	//

	mManagerToTest->CreateDirectory( newDirectoryName );
	if( !mManagerToTest->DirectoryExists( newDirectoryName ) )
	{
		RETURN_UNIT_TEST_FAILURE( "Failed to create second directory." );
	}

	for( int currentFile = 0; currentFile < testFiles.Count(); ++currentFile )
	{
		gLogger->LogInfo( TCString( "[TCFile_UnitTest] Testing file copy on file: " ) + currentFile );

		TCString newFilepath = newDirectoryName + "/" + testFiles[ currentFile ]->GetFilename() + ".txt";
		TCString currentFilepath = testFiles[ currentFile ]->GetFilepath();

		//
		// Let's close the file.
		//

		TCResult result = mManagerToTest->CloseFile( testFiles[ currentFile ] );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to close the file." );
		}
		testFiles[ currentFile ] = NULL;

		//
		// Now let's copy the file over.
		//

		result = mManagerToTest->CopyFile( currentFilepath, newFilepath );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to copy the file!" );
		}
	}

	//
	// Delete the old directory
	//

	gLogger->LogInfo( TCString( "[TCFile_UnitTest] Testing directory delete on previous test directory" ) );

	result = mManagerToTest->DeleteDirectory( testDirectoryPath );
	if( TC_FAILED( result ) )
	{
		RETURN_UNIT_TEST_FAILURE( "Failed to delete directory" );
	}

	//
	// Make sure we can reopen the new files.
	//

	TCString testFileName = "TestFile";
	for( int currentFile = 0; currentFile < testFiles.Count(); ++currentFile )
	{
		gLogger->LogInfo( TCString( "[TCFile_UnitTest] Testing file reopen on file: " ) + currentFile );

		TCString newFilepath = newDirectoryName + "/" + testFileName + currentFile + ".txt";

		result = mManagerToTest->OpenFile( newFilepath,
										   &testFiles[ currentFile ],
										   TCFileManager::Access_ReadOnly,
										   TCFileManager::FileDataType_Text,
										   TCFileManager::OpenMode_Append );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to open the copied file." );
		}

		//
		// Close them again.
		//

		result = mManagerToTest->CloseFile( testFiles[ currentFile ] );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to close the copied file." );
		}
	}

	//
	// Create binary files and test them real quick.
	//

	struct dataTest
	{
		char testName[ 128 ];
		unsigned int count;
		float squareRootOfSix;
	};

	TCList< TCFile* > testBinaryFiles;
	TCList< dataTest* > testData;
	TCString testBinName = "Test Bin File";

	gLogger->LogInfo( TCString( "[TCFile_UnitTest] Testing binary files" ) );

	for( int currentFile = 0; currentFile < 50; ++currentFile )
	{
		gLogger->LogInfo( TCString( "[TCFile_UnitTest] Creating binary file: " ) + currentFile );

		TCFile* binFile = NULL;
		TCString binFilepath = newDirectoryName + "/" + testBinName + currentFile + ".bin";
		result = mManagerToTest->CreateFile( binFilepath,
											 &binFile,
										     TCFileManager::Access_ReadWrite,
											 TCFileManager::FileDataType_Binary );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to create the test binary file.");
		}

		//
		// Write to the file.
		//

		dataTest* dataToWrite = new dataTest;
		dataToWrite->count = currentFile;
		TCStringUtils::Copy( dataToWrite->testName, "David R Seabolt" );
		dataToWrite->squareRootOfSix = (float)TCMathUtils::Sqrt( 6 );

		gLogger->LogInfo( TCString( "[TCFile_UnitTest] Testing binary file write: " ) + currentFile );

		result = binFile->Write( dataToWrite, sizeof( dataTest ) );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to write to the binary file.");
		}

		testData.Append( dataToWrite );

		//
		// Flush to the file.
		//

		result = binFile->Flush();
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to flush to the binary file." );
		}

		//
		// Close the file.
		//

		gLogger->LogInfo( TCString( "[TCFile_UnitTest] Testing binary file close: " ) + currentFile );

		result = mManagerToTest->CloseFile( binFile );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to close the binary file." );
		}

		//
		// Reopen the file.
		//

		gLogger->LogInfo( TCString( "[TCFile_UnitTest] Testing binary file open: " ) + currentFile );
		result = mManagerToTest->OpenFile( binFilepath,
										   &binFile,
										   TCFileManager::Access_ReadOnly,
										   TCFileManager::FileDataType_Binary,
										   TCFileManager::OpenMode_Append );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to reopen the binary file." );
		}

		//
		// Read the file data.
		//

		gLogger->LogInfo( TCString( "[TCFile_UnitTest] Testing binary file read: " ) + currentFile );

		dataTest* readTest = new dataTest;
		result = binFile->Read( (void**)&readTest, sizeof( dataTest ) );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to read from the binary file." );
		}

		if( TCStringUtils::Compare( readTest->testName, dataToWrite->testName ) != 0 )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to read from the binary file, invalid test name." );
		}

		if( readTest->count != dataToWrite->count )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to read from the binary file, invalid count." );
		}

		if( readTest->squareRootOfSix != dataToWrite->squareRootOfSix )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to read from the binary file, invalid square root." );
		}

		//
		// Close the file again.
		//

		result = mManagerToTest->CloseFile( binFile );
		if( TC_FAILED( result ) )
		{
			RETURN_UNIT_TEST_FAILURE( "Failed to close the binary file." );
		}
	}

	gLogger->LogInfo( TCString( "[TCFile_UnitTest] Deleting current directory."));
	result = mManagerToTest->DeleteDirectory( newDirectoryName );
	if( TC_FAILED( result ) )
	{
		RETURN_UNIT_TEST_FAILURE( "failed to clean up the new directory.");
	}

	return TCUnitTest::TestResult_Success;
}