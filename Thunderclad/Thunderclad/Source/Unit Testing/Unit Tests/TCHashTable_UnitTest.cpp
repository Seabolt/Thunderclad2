//
// TCHashTable_UnitTest.cpp
// This file will define the unit test for a hash table.
//

//
// Includes
//

#include "TCHashTable_UnitTest.h"

//
// Defines
//

#define RETURN_UNIT_TEST_FAILURE( x ) { TCLogger::GetInstance()->LogError( x ); return TCUnitTest::TestResult_Failed; }

//
// StartTest
//		- This function will run the unit test for this module.
// Inputs:
//		- None.
// Outputs:
//		- TCUnitTest::Result: The result of the operation.
//

TCUnitTest::Result TCHashTable_UnitTest::StartTest()
{
	//
	// Create our test hash tables.
	//

	TCHashTable< int, int > intTable;
	TCHashTable< int, int > intTable2;
	TCHashTable< float, TCString > stringTable;
	TCHashTable< TCString, TCHashTable< int, int >* > tableOfTables;

	//
	// Test basic addition.
	//

	intTable.AddValue( -1, 0 );
	intTable.AddValue( 0, 1 );
	intTable.AddValue( 1, 2 );
	intTable.AddValue( 2, 3 );
	intTable.AddValue( 3, 3 );
	intTable.AddValue( 4, 3 );
	intTable.AddValue( 5, 3 );
	intTable.AddValue( 6, 3 );
	intTable.AddValue( 7, 3 );
	intTable.AddValue( 8, 3 );
	intTable.AddValue( 9, 3 );
	intTable.AddValue( 10, 3 );
	intTable.AddValue( 11, 3 );
	intTable.AddValue( 12, 3 );


	intTable2.AddValue( 0, -1 );
	intTable2.AddValue( 1, 0 );
	intTable2.AddValue( 2, 1 );
	intTable2.AddValue( 3, 2 );

	stringTable.AddValue( 0.0f, "0.0f");
	stringTable.AddValue( 1.0f, "1.0f");
	stringTable.AddValue( 2.0f, "2.0f");
	stringTable.AddValue( 3.0f, "3.0f");

	tableOfTables.AddValue( "intTable", &intTable );
	tableOfTables.AddValue( "intTable2", &intTable2 );

	//
	// Test basic removal.
	//

	intTable.RemoveValue( 0 );
	intTable.RemoveValue( 1 );

	intTable2.RemoveValue( 3 );
	intTable2.RemoveValue( 0 );

	stringTable.RemoveValue( 1.0f );
	stringTable.RemoveValue( 2.5f );

	//
	// Test count
	//

	if( intTable.Count() != 12 )
	{
		RETURN_UNIT_TEST_FAILURE("TCHashTable_UnitTest failed! intTable count is not accurate.");
	}

	if( intTable2.Count() != 2 )
	{
		RETURN_UNIT_TEST_FAILURE("TCHashTable_UnitTest failed! intTable2 count is not accurate.");
	}

	if( stringTable.Count() != 3 )
	{
		RETURN_UNIT_TEST_FAILURE("TCHashTable_UnitTest failed! stringTable count is not accurate.");
	}

	if( tableOfTables.Count() != 2 )
	{
		RETURN_UNIT_TEST_FAILURE("TCHashTable_UnitTest failed! tableOfTables count is not accurate.");
	}

	//
	// Test retrieval
	//

	// Test the int table.
	int* value = NULL;
	if( intTable.GetValue( -1, &value ) == false || *value != 0 )
	{
		RETURN_UNIT_TEST_FAILURE("TCHashTable_UnitTest failed! intTable::GetValue provided a wrong output.");
	}

	if( intTable.GetValue( 2, &value ) == false || *value != 3 )
	{
		RETURN_UNIT_TEST_FAILURE("TCHashTable_UnitTest failed! intTable::GetValue provided a wrong output.");
	}

	// Test the second int table.
	if( intTable2.GetValue( 1, &value ) == false || *value != 0 )
	{
		RETURN_UNIT_TEST_FAILURE("TCHashTable_UnitTest failed! intTable2::GetValue provided a wrong output.");
	}

	if( intTable2.GetValue( 2, &value ) == false || *value != 1 )
	{
		RETURN_UNIT_TEST_FAILURE("TCHashTable_UnitTest failed! intTable2::GetValue provided a wrong output.");
	}

	// Test the string table.
	TCString* stringValue = NULL;
	if( stringTable.GetValue( 0.0f, &stringValue ) == false || *stringValue != "0.0f" )
	{
		RETURN_UNIT_TEST_FAILURE("TCHashTable_UnitTest failed! Wrong value was found");
	}

	//
	// Test contains
	//

	if( stringTable.ContainsKey( 3.0f ) != true )
	{
		RETURN_UNIT_TEST_FAILURE("TCHashTable_UnitTest failed! Contain test failed." );
	}

	if( stringTable.ContainsValue( "3.0f" ) != true )
	{
		RETURN_UNIT_TEST_FAILURE("TCHashTable_UnitTest failed! Contain test failed.");
	}

	if( intTable.ContainsKey( -1000 ) )
	{
		RETURN_UNIT_TEST_FAILURE("TCHashTable_UnitTest failed, contain test returned false positive.");
	}
	
	//
	// Test clears.
	//

	intTable.Clear();
	intTable2.Clear();
	stringTable.Clear();
	tableOfTables.Clear();

	return Result::TestResult_Success;
}