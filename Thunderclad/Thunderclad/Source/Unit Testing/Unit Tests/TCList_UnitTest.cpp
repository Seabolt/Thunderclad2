//
// TCList_UnitTest.cpp
// Will define all functionality for the TCList unit test.
//

//
// Include
//

#include "TCList_UnitTest.h"
#include "TCLogger.h"

//
// Define
//

//
// StartTest
//		- Will run the test 
// Inputs:
//		- None.
// Outputs:
//		- Result: the result of the test.
//

TCUnitTest::Result TCList_UnitTest::StartTest()
{
	//
	// Test some easy things first, make sure we initialize with the right values.
	//

	TCList< int > integerList;
	TCList< float > floatList;
	TCList< TCString > stringList;

	TCLogger::GetInstance()->LogInfo( "Starting Append Test" );

	integerList.Append( 1 );
	integerList.Append( 2 );
	integerList.Append( 3 );
	integerList.Append( 4 );
	integerList.Append( 5 );

	floatList.Append( 1.0f );
	floatList.Append( 2.0f );
	floatList.Append( 3.0f );
	floatList.Append( 4.0f );
	floatList.Append( 5.0f );

	stringList.Append( "1" );
	stringList.Append( "2" );
	stringList.Append( "3" );
	stringList.Append( "4" );
	stringList.Append( "5" );

	TCLogger::GetInstance()->LogInfo( "Appended Test: Testing values" );
	for( int currentCheck = 0; currentCheck < 5; ++currentCheck )
	{
		if( integerList[ currentCheck ] != currentCheck + 1 )
		{
			TCLogger::GetInstance()->LogError( "Append Test Failed: integerList doesn't match!" );
			return Result::TestResult_Failed;
		}

		if( floatList[ currentCheck ] != (float)(currentCheck + 1) )
		{
			TCLogger::GetInstance()->LogError( "Append Test Failed: floatList doesn't match!" );
		}

		switch( currentCheck )
		{
		case 0:
			if( stringList[ currentCheck ] != "1" )
			{
				TCLogger::GetInstance()->LogError( "Append Test Failed: stringList doesn't match!" );
				return TCUnitTest::TestResult_Failed;
			}
			else
			{
				break;
			}
		case 1:
			if( stringList[ currentCheck ] != "2" )
			{
				TCLogger::GetInstance()->LogError( "Append Test Failed: stringList doesn't match!" );
				return TCUnitTest::TestResult_Failed;
			}
			else
			{
				break;
			}
		case 2:
			if( stringList[ currentCheck ] != "3" )
			{
				TCLogger::GetInstance()->LogError( "Append Test Failed: stringList doesn't match!" );
				return TCUnitTest::TestResult_Failed;
			}
			else
			{
				break;
			}
		case 3:
			if( stringList[ currentCheck ] != "4" )
			{
				TCLogger::GetInstance()->LogError( "Append Test Failed: stringList doesn't match!" );
				return TCUnitTest::TestResult_Failed;
			}
			else
			{
				break;
			}
		case 4:
			if( stringList[ currentCheck ] != "5" )
			{
				TCLogger::GetInstance()->LogError( "Append Test Failed: stringList doesn't match!" );
				return TCUnitTest::TestResult_Failed;
			}
			else
			{
				break;
			}
		}
	}

	TCLogger::GetInstance()->LogInfo( "Append Test success!" );

	//
	// Now let's remove a value.
	//

	TCLogger::GetInstance()->LogInfo( "Remove Test start" );

	integerList.Remove( 4 );
	floatList.Remove( 4.0f );
	stringList.RemoveAt( 3 );

	if( integerList[ 3 ] == 4 )
	{
		TCLogger::GetInstance()->LogError( "Remove Test failed" );
		return TCUnitTest::TestResult_Failed;
	}

	if( floatList[ 3 ] == 4.0f )
	{
		TCLogger::GetInstance()->LogError( "Remove Test failed" );
		return TCUnitTest::TestResult_Failed;
	}

	if( stringList[ 3 ] == "4" )
	{
		TCLogger::GetInstance()->LogError( "Remove Test failed" );
		return TCUnitTest::TestResult_Failed;
	}

	//
	// Now let's test some inserts.
	//

	integerList.Insert( -1, 0 );

	int floatIndex = floatList.Count() - 1;
	floatList.Insert( 6, floatIndex );
	stringList.Insert( "Hello!", 2 );

	if( integerList[ 0 ] != -1 || integerList[ 1 ] != 1 )
	{
		TCLogger::GetInstance()->LogError( "Insert Test Failed" );
		return TCUnitTest::TestResult_Failed;
	}

	if( floatList[ floatIndex ] != 6.0f )
	{
		TCLogger::GetInstance()->LogError( "Insert Test Failed" );
		return TCUnitTest::TestResult_Failed;
	}

	if( stringList[ 2 ] != "Hello!" )
	{
		TCLogger::GetInstance()->LogError("Insert Test Failed" );
		return TCUnitTest::TestResult_Failed;
	}

	//
	// Next let's check finds/contains
	//

	if( !integerList.Contains( 2 ) )
	{
		TCLogger::GetInstance()->LogError("Contains Test Failed" );
		return TCUnitTest::TestResult_Failed;
	}

	if( floatList.Find(6.0f) != floatIndex )
	{
		TCLogger::GetInstance()->LogError("Find Test Failed");
		return TCUnitTest::TestResult_Failed;
	}

	if( stringList.Contains("BlahBlah") )
	{
		TCLogger::GetInstance()->LogError("Contains Test Failed");
		return TCUnitTest::TestResult_Failed;
	}

	//
	// Next let's check capacity
	//

	integerList.Reserve( 100 );
	if( integerList.Capacity() != 100 )
	{
		TCLogger::GetInstance()->LogError("Reserve Test Failed");
		return TCUnitTest::TestResult_Failed;
	}

	if( integerList.Contains(-1) == false )
	{
		TCLogger::GetInstance()->LogError("Reserve Test Failed");
		return TCUnitTest::TestResult_Failed;
	}

	//
	// Now let's test clear.
	//

	floatList.Clear();
	if( floatList.Count() > 0 || floatList.Capacity() > 0 )
	{
		TCLogger::GetInstance()->LogError("Clear Test Failed");
		return TCUnitTest::TestResult_Failed;
	}

	if( floatList.Contains( 0 ) )
	{
		TCLogger::GetInstance()->LogError("Clear Test Failed");
		return TCUnitTest::TestResult_Failed;
	}

	floatList.Append( 1.0f );
	if( !floatList.Contains( 1.0f ) )
	{
		TCLogger::GetInstance()->LogError("Clear Test Failed");
		return TCUnitTest::TestResult_Failed;
	}

	return TCUnitTest::TestResult_Success;
}