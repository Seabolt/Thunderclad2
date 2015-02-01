//
// TCUnitTestManager.cpp
// This file will define all functionality for the unit test manager.
//

//
// Includes
//

#include "TCUnitTestManager.h"
#include "TCLogger.h"

//
// Defines
//

//
// Default Constructor
//		- Will initialize the class to a safe state.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCUnitTestManager::TCUnitTestManager()
{

}

//
// Copy Constructor
//		- Will create a new instance of the class copying the provided class.
// Inputs:
//		- const TCUnitTestManager& manager: The manager to copy.
// Outputs:
//		- None.
//

TCUnitTestManager::TCUnitTestManager( const TCUnitTestManager& manager )
{
	mUnitTests = manager.mUnitTests;
}

//
// Assignment Operator
//		- Will set this manager to the same as the unit test manager.
// Inputs:
//		- const TCUnitTestManager& manager: The manager to set us to.
// Outputs:
//		- TCUnitTestManager&
//

TCUnitTestManager& TCUnitTestManager::operator=( const TCUnitTestManager& manager )
{
	mUnitTests = manager.mUnitTests;
	return (*this);
}

//
// Initialize
//		- Will initialize all resources associated to this class.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCUnitTestManager::Initialize()
{
	RegisterTests();
}

//
// StartTests
//		- Will begin all unit tests.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCUnitTestManager::StartTests()
{
	for( int currentTest = 0; currentTest < mUnitTests.Count(); ++currentTest )
	{
		if( mUnitTests[ currentTest ] == NULL )
			continue;

		TCUnitTest::Result result = mUnitTests[ currentTest ]->StartTest();
		if( result < TCUnitTest::TestResult_Success )
		{
			TCLogger::GetInstance()->LogError( mUnitTests[ currentTest ]->GetName() + " unit test failed!" );
		}
	}
}

//
// Update
//		- Will update all the test necessary
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCUnitTestManager::Update()
{

}

//
// Destroy
//		- Will release all resources associated with this manager.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCUnitTestManager::Destroy()
{
	mUnitTests.Clear();
}

//
// AddUnitTest
//		- Will add a new unit test to start.
// Inputs:
//		- TCUnitTest* test: The test to add.
// Outputs:
//		- None.
//

void TCUnitTestManager::AddUnitTest( TCUnitTest* test )
{
	mUnitTests.Append( test );
}

//
// RegisterTests
//		- Will register tests to run.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCUnitTestManager::RegisterTests()
{

}