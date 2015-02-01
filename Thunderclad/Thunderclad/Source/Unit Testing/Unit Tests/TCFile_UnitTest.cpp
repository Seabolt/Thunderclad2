//
// TCFile_UnitTest.cpp
// This will define the testing for a file.
//

//
// Includes
//

#include "TCFile_UnitTest.h"
#include "TCLogger.h"


//
// Defines
//

//
// Defines
//

#define RETURN_UNIT_TEST_FAILURE( x ) { TCLogger::GetInstance()->LogError( TCString("[TCFile] ") + x ); return TCUnitTest::TestResult_Failed; }

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

	
	
}