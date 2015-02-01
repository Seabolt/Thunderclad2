//
// TCString_UnitTest.cpp
// This will define the testing for a string.
//

//
// Includes
//

#include "TCString_UnitTest.h"
#include "TCLogger.h"

#include <string>

//
// Defines
//

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

TCUnitTest::Result TCString_UnitTest::StartTest()
{
	const char* test_helloWorldString = "Hello World!";
	const char* test_whatsUpString = "What's up?!";
	const char* test_longString = "How much wood could a wood chuck chuck if a wood chuck could chuck wood? Well a wood chuck can chuck wood, so we may never know how much wood could a wood chuck chuck if a wood chuck could chuck wood.";

	int helloWorldStringLength = strlen( test_helloWorldString );
	int whatsUpStringLength = strlen( test_whatsUpString );
	int longStringLength = strlen( test_longString );

	//
	// First Test - Constructors.
	//

	gLogger->LogInfo( "[TCString] Starting string test!" );
	gLogger->LogInfo( "[TCString] Testing constructors!" );

	TCString helloWorldString = test_helloWorldString;	// Assignment operator.
	TCString whatsUpString( test_whatsUpString );	// Default constructor.
	TCString copyString( helloWorldString );	// Copy constructor.
	TCString crashString = "jasdjfjasdf";

	gLogger->LogInfo( "[TCString] Constructor test success!" );

	//
	// Second Test - Length.
	//

	gLogger->LogInfo( "[TCString] Testing Length()!" );

	if( helloWorldString.Length() != helloWorldStringLength )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Length did not return the correct value!" );
	}

	if( whatsUpString.Length() != whatsUpStringLength )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] What's up string did not return the correct value!" );
	}

	if( copyString.Length() != helloWorldString.Length() )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Copy string length does not equal the hello world string!");
	}

	gLogger->LogInfo( "[TCString] Length() test success!" );

	//
	// IsEmpty Test
	//

	gLogger->LogInfo( "[TCString] Starting IsEmpty() test!");

	TCString emptyString;
	const char* emptyArray = "";
	const char* nullArray = NULL;

	if( !emptyString.IsEmpty() )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Empty string did not register as empty." );
	}

	emptyString += "Not Empty";
	if( emptyString.IsEmpty() )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Not empty string registered as empty" );
	}

	gLogger->LogInfo( "[TCString] IsEmpty() test success!" );

	//
	// Clear Test.
	//

	gLogger->LogInfo( "[TCString] Starting Clear() test!" );

	emptyString.Clear();
	if( !emptyString.IsEmpty() )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Cleared string did not register as empty!" );
	}

	gLogger->LogInfo( "[TCString] Clear() test success!" );

	//
	// Copy Test.
	//

	gLogger->LogInfo( "[TCString] Starting Copy() test!" );

	helloWorldString.Copy( whatsUpString );
	if( strcmp( helloWorldString.Data(), "What's up?!" ) != 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Copy of a TCString failed!" );
	}

	whatsUpString.Copy( "Hello World!" );
	if( strcmp( whatsUpString.Data(), "Hello World!" ) != 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Copy of a char* failed!" );
	}

	copyString.Copy( helloWorldString, 5 );
	if( strcmp( copyString.Data(), "What'" ) != 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Copy of a TCString with a length argument failed!" );
	}

	TCString copyArrayString;
	copyArrayString.Copy( "Hello World!", 11 );
	if( strcmp( copyArrayString.Data(), "Hello World" ) != 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Copy of a char* with a length argument failed!" );
	}

	gLogger->LogInfo( "[TCString] Testing crash copy!" );

	crashString.Copy(NULL);
	crashString.Copy(NULL, 100);
	crashString.Copy("Hello", 100);

	gLogger->LogInfo( "[TCString] Crash copy succeeded!" );
	gLogger->LogInfo( "[TCString] Copy test succeeded!" );

	//
	// Append Test
	//

	gLogger->LogInfo( "[TCString] Starting Append() test!" );

	helloWorldString.Append( " Oh, not much." );
	if( strcmp( helloWorldString.Data(), "What's up?! Oh, not much." ) != 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Append of a char* failed!" );
	}

	whatsUpString.Append( copyString );
	if( strcmp( whatsUpString.Data(), "Hello World!What'" ) != 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Append of a TCString failed!" );
	}

	copyString.Append( " World!9", 7 );
	if( strcmp( copyString.Data(), "What' World!" ) != 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Append of a char* with a length failed!" );
	}

	emptyString.Copy( "" );
	copyArrayString.Append( emptyString, 1 );
	if( strcmp( copyArrayString.Data(), "Hello World" ) != 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Append of a empty string with a length failed!" );		
	}

	gLogger->LogInfo( "[TCString] Testing Append() crash cases!" );

	crashString.Clear();
	crashString.Append( NULL );
	crashString.Append( "", 100 );
	crashString.Append( NULL, 100 );
	crashString.Append( copyString, 10 );

	gLogger->LogInfo( "[TCString] Append() crash cases success!" );
	gLogger->LogInfo( "[TCString] Append() test success!" );

	//
	// Compare Test
	//

	gLogger->LogInfo( "[TCString] Starting compare tests!" );

	helloWorldString.Copy( test_helloWorldString );
	if( helloWorldString.Compare( test_helloWorldString ) != 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Compare test with equal string failed!" );
	}
	
	helloWorldString.Append( "!" );
	if( helloWorldString.Compare( test_helloWorldString ) <= 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Compare test with an smaller string failed!" );
	}

	if( helloWorldString.Compare( "Hello World!! Dude!" ) >= 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Compare test with a larger string failed!" );
	}

	copyString.Copy( helloWorldString );
	if( helloWorldString.Compare( copyString ) != 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Compare test with a equal TCString failed!" );
	}

	copyString.Copy( "Hello" );
	if( helloWorldString.Compare( copyString ) <= 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Compare test with a smaller TCString failed!" );
	}

	copyString.Copy( "Hello World, what's up dude!");
	if( helloWorldString.Compare( copyString ) >= 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Compare test with a larger TCString failed!" );
	}

	helloWorldString.Copy( test_helloWorldString );
	if( helloWorldString.CompareInsensitive( "HELLO WORLD!" ) != 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Insensitive Compare test with an equal upper case string failed!");
	}

	if( helloWorldString.CompareInsensitive( "hello world!" ) != 0 ) 
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Insensitive Compare test with an equal lower case string failed!" );
	}

	if( helloWorldString.CompareInsensitive( "HELLO WORLD" ) <= 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Insensitive Compare test with a small string failed!");
	}

	if( helloWorldString.CompareInsensitive( "HELLO WORLD!!!") >= 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Insensitive Compare test with a larger string failed!" );
	}

	copyString.Copy( "HELLO WORLD!" );
	if( helloWorldString.CompareInsensitive( copyString ) != 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Insensitive compare to an equal upper case TCString failed!" );
	}

	copyString.Copy( "hello world!" );
	if( helloWorldString.CompareInsensitive( copyString ) != 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Insensitive compare to an equal lower case TCString failed!" );
	}

	copyString.Copy( "HELLO WORLD" );
	if( helloWorldString.CompareInsensitive( copyString ) <= 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Insensitive compare to a smaller TCString failed!" );
	}

	copyString.Copy( "hello world!!!");
	if( helloWorldString.CompareInsensitive( copyString ) >= 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Insensitive compare to a larger TCString failed!" );
	}

	gLogger->LogInfo( "[TCString] Starting Compare() crash cases!" );
	crashString.Compare( NULL );
	crashString.Compare( emptyString );
	crashString.CompareInsensitive( NULL );
	crashString.CompareInsensitive( emptyString );
	gLogger->LogInfo( "[TCString] Compare() crash cases success!" );
	gLogger->LogInfo( "[TCString] Compare() test success!" );

	//
	// Equal Test.
	//

	gLogger->LogInfo( "[TCString] Starting Equal() tests!" );
	helloWorldString.Copy( test_helloWorldString );

	if( !helloWorldString.Equal( test_helloWorldString ) )
	{
		RETURN_UNIT_TEST_FAILURE( "Equal() with an equal string failed!" );
	}

	TCString equalString( test_helloWorldString );
	if( !helloWorldString.Equal( equalString ) )
	{
		RETURN_UNIT_TEST_FAILURE( "Equal() with an equal TCString failed!" );
	}

	if( helloWorldString.Equal( "kajsdfjasdf" ) )
	{
		RETURN_UNIT_TEST_FAILURE( "Equal() registered true with an inequal string!" );
	}

	equalString.Copy( "asdfasdf" );
	if( helloWorldString.Equal( equalString ) )
	{
		RETURN_UNIT_TEST_FAILURE( "Equal() registered true with an inequal TCString!" );
	}

	gLogger->LogInfo( "[TCString] Testing Equal() crash cases!" );
	emptyString.Clear();
	crashString.Equal( NULL );
	crashString.Equal( emptyString );
	gLogger->LogInfo( "[TCString] Equal() crash cases succeeded!" );
	gLogger->LogInfo( "[TCString] Equal() tests succeeded!" );

	//
	// Contains Tests.
	//

	gLogger->LogInfo( "[TCString] Testing Contains()!" );

	if( !helloWorldString.Contains( "Hello" ) )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Contains test failed to find a sub string" );
	}

	TCString containsString( "World" );
	if( !helloWorldString.Contains( containsString ) )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Contains test failed to find a sub string" );
	}

	if( helloWorldString.Contains( "Hello!" ) )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Contains test found an invalid sub string!" );
	}

	gLogger->LogInfo( "[TCString] Testing Contains() crash cases" );
	crashString.Clear();
	crashString.Contains( "Hello" );
	crashString.Contains( NULL );
	gLogger->LogInfo( "[TCString] Contains() crash cases succeeded!" );
	gLogger->LogInfo( "[TCString] Contains() tests succeeded!" );

	//
	// Find functions
	//

	gLogger->LogInfo( "[TCString] Starting FindFirstIndexOf() and FindLastIndexOf() test!");

	TCString findString( "-_-_-" );
	TCString revFindString( "_-_-_" );

	if( findString.FindFirstIndexOf( '_' ) != 1 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] FindFirstIndexOf returned invalid index" );
	}

	if( findString.FindLastIndexOf( '_' ) != 3 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] FindLastIndexOf returned invalid index" );
	}

	if( findString.FindFirstIndexOf( '-' ) != 0 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] FindFirstIndexOf returned invalid index!" );
	}

	if( findString.FindLastIndexOf( '-' ) != findString.Length() - 1)
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] FindLastIndexOf returned invalid index!" );
	}

	if( findString.FindFirstIndexOf( 'a' ) != -1 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] FindFirstIndexOf found an invalid character!" );
	}

	if( findString.FindLastIndexOf( 'b' ) != -1 )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] FindLastIndexOf found an invalid character!" );
	}

	gLogger->LogInfo( "[TCString] Testing FindFirstIndexOf() and FindLastIndexOf() crash cases!" );
	crashString.Clear();
	crashString.FindFirstIndexOf( '\0' );
	crashString.FindLastIndexOf( '\0' );
	gLogger->LogInfo( "[TCString] FindFirstIndexOf() and FindLastIndexOf() crash cases succeeded!" );
	gLogger->LogInfo( "[TCString] FindFirstIndexOf() and FindLastIndexOf() tests succeeded!" );

	//
	// Reverse Test.
	//

	gLogger->LogInfo( "[TCString] Testing Reverse()!" );
	helloWorldString.Copy( "!dlroW olleH" );
	helloWorldString.Reverse();

	if( helloWorldString.Compare( test_helloWorldString ) )
	{
		RETURN_UNIT_TEST_FAILURE( "Reverse() test failed!" );
	}

	gLogger->LogInfo( "[TCString] Testing Reverse() crash cases!" );
	crashString.Clear();
	crashString.Reverse();
	gLogger->LogInfo( "[TCString] Reverse() crash cases succeeded!" );
	gLogger->LogInfo( "[TCString] Reverse() tests succeeded!" );

	//
	// ToLower/ToUpper Tests.
	//

	gLogger->LogInfo( "[TCString] Starting ToUpper()/ToLower() tests!" );

	helloWorldString.Copy( test_helloWorldString );
	helloWorldString.ToUpper();

	if( !helloWorldString.Equal( "HELLO WORLD!" ) )
	{
		RETURN_UNIT_TEST_FAILURE( "ToUpper() test failed to register a match!" );
	}
	
	helloWorldString.ToLower();
	if( !helloWorldString.Equal( "hello world!" ) )
	{
		RETURN_UNIT_TEST_FAILURE( "ToLower() test failed to register a match!" );
	}

	gLogger->LogInfo( "[TCString] Starting ToUpper()/ToLower() crash cases!" );
	crashString.Clear();
	crashString.ToUpper();
	crashString.ToLower();
	gLogger->LogInfo( "[TCString] ToUpper()/ToLower() crash cases succeeded!" );
	gLogger->LogInfo( "[TCString] ToUpper()/ToLower() tests succeeded!" );

	//
	// Substring Tests.
	//

	gLogger->LogInfo( "[TCString] Starting Substring() tests!" );
	helloWorldString.Copy( test_helloWorldString );
	TCString outputString;

	helloWorldString.Substring( 6, outputString );
	if( !outputString.Equal( "World!" ) )
	{
		RETURN_UNIT_TEST_FAILURE( "Substring with just a start index failed!" );
	}

	helloWorldString.Substring( 1, 4, outputString );
	if( !outputString.Equal( "ello") )
	{
		RETURN_UNIT_TEST_FAILURE( "Substring with a start and end index failed!" );
	}

	gLogger->LogInfo( "[TCString] Starting Substring() crash cases!" );
	crashString.Clear();
	crashString.Substring( 0, outputString );
	crashString.Substring( 100, 1, outputString );
	crashString.Copy( "Hello Mate" );
	crashString.Substring( -1, outputString );
	crashString.Substring( -1, -1, outputString );
	crashString.Substring( 10000, 0, outputString );
	gLogger->LogInfo( "[TCString] Substring() crash cases succeeded!" );
	gLogger->LogInfo( "[TCString] Substring() tests succeeded!" );

	//
	// TrimWhitespace Test
	//

	gLogger->LogInfo( "[TCString] Starting TrimWhitespace() tests!" );

	copyString.Copy( "  Hello\tWorld\n\n\t\r \t Dude!\n  ");
	copyString.TrimWhitespace();
	if( !copyString.Equal( "HelloWorldDude!") )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] TrimWhitespace failed to remove all whitespace.");
	}

	gLogger->LogInfo( "[TCString] Starting TrimWhitespace() crash cases!" );
	crashString.Copy( "HelloWorldDude!" );
	crashString.TrimWhitespace();
	crashString.Clear();
	crashString.TrimWhitespace();
	gLogger->LogInfo( "[TCString] TrimWhitespace() crash cases succeeded!" );
	gLogger->LogInfo( "[TCString] TrimWhitespace() tests succeeded!" );

	//
	// StartsWith Tests.
	//

	gLogger->LogInfo( "[TCString] Starting StartsWith() tests!" );

	copyString.Copy( "PRE_HelloWorld!" );
	if( !copyString.StartsWith( "PRE_" ) )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] StartsWith() test didn't tess accurately what the string starts with.");
	}

	if( copyString.StartsWith( TCString("PRE__") ) )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] StartsWith() reported that is started with an invalid prefix." );
	}

	gLogger->LogInfo( "[TCString] Starting StartsWith() crash cases." );
	crashString.Clear();
	crashString.StartsWith( "PRE" );
	crashString.Copy( "Hello" );
	crashString.StartsWith( NULL );
	crashString.StartsWith( "HELLLLOOOOO" );
	gLogger->LogInfo( "[TCString] StartsWith() crash cases succeeded!" );
	gLogger->LogInfo( "[TCString] StartsWith() tests succeeded!" );

	//
	// Assignment Operator Tests.
	//

	gLogger->LogInfo( "[TCString] Starting Assignment Operator tests" );
	
	helloWorldString = "Hello World!";
	if( helloWorldString.Equal( "Hello World!" ) == false )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Assignment Operator failed to assign to a char*" );
	}

	helloWorldString = TCString( "Hello World Dude!" );
	if( helloWorldString.Equal( "Hello World Dude!" ) == false )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Assignment Operator failed to assign to a TCString!");
	}

	helloWorldString = 'a';
	if( helloWorldString.Equal( "a" ) == false )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Assignment Operator failed to assign to a character!" );
	}

	helloWorldString = 5;
	if( helloWorldString.Equal( "5" ) == false )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Assignment Operator failed to assign to a number!" );
	}

	helloWorldString = 1234567890;
	if( helloWorldString.Equal( "1234567890") == false )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Assignment operator failed to assign to a large number!" );
	}

	gLogger->LogInfo( "[TCString] Testing Assignment Operator crash cases!" );
	crashString = NULL;
	gLogger->LogInfo( "[TCString] Assignment Operator crash cases succeeded!" );
	gLogger->LogInfo( "[TCString] Assignment Operator tests succeeded!" );

	//
	// Concatenation Operator Tests.
	//

	gLogger->LogInfo( "[TCString] Starting Concatenation Operator tests" );
	helloWorldString = "Hello World!";
	copyString = helloWorldString + " Thanks!";
	if( copyString.Equal( "Hello World! Thanks!" ) == false )
	{
		RETURN_UNIT_TEST_FAILURE( "Failed to concatenate a TCString!" );
	}

	copyString = copyString + " No problem!";
	if( copyString.Equal( "Hello World! Thanks! No problem!" ) == false )
	{
		RETURN_UNIT_TEST_FAILURE( "Failed to concatenate a TCString!" );
	}

	copyString = copyString + TCString( " You're very polite!" ) + TCString( " You too!" ) + " Get a room!";
	if( copyString.Equal( "Hello World! Thanks! No problem! You're very polite! You too! Get a room!" ) == false )
	{
		RETURN_UNIT_TEST_FAILURE( "Failed to concatenate a daisy chained string!");
	}

	gLogger->LogInfo( "[TCString] Starting Concatenation Operator crash cases." );
	crashString.Clear();
	crashString = crashString + NULL;
	crashString = crashString + TCString( NULL ) + NULL;
	gLogger->LogInfo( "[TCString] Concatenation Operator crash cases succeeded!" );
	gLogger->LogInfo( "[TCString] Concatenation Operator tests succeeded!" );

	//
	// Append Operator Tests.
	//

	gLogger->LogInfo( "[TCString] Starting Append Operator tests!" );

	copyString = "";
	copyString += "Hello World!";

	if( copyString.Equal( "Hello World!" ) == false )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Append operator failed!" );
	}

	copyString += TCString( " Oh god, not again!" ) + " Yep.";
	if( copyString.Equal( "Hello World! Oh god, not again! Yep." ) == false )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Append operator failed with daisy chained strings." );
	}

	copyString += '\n';
	if( copyString.Equal( "Hello World! Oh god, not again! Yep.\n" ) == false )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Append operator failed with append test for a character." );
	}

	copyString += 1;
	if( copyString.Equal( "Hello World! Oh god, not again! Yep.\n1" ) == false )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Append operator failed with append test for a number." );
	}

	gLogger->LogInfo( "[TCString] Testing Append operator crash cases!" );
	crashString.Clear();
	crashString += NULL;
	crashString += TCString( NULL ) + NULL;
	gLogger->LogInfo( "[TCString] Append operator crash cases succeeded!" );
	gLogger->LogInfo( "[TCString] Append operator tests succeeded!" );

	//
	// Equivalence Tests.
	//

	gLogger->LogInfo( "[TCString] Testing equivalence operator!" );
	copyString = "Hello Stephanie Seabolt!";
	if( copyString == "Hello Stephanie" )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Equivalence test failed" );
	}

	if( copyString != "Hello Stephanie Seabolt!" )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Non-equivalence test failed!" );
	}

	copyString = TCString( "Stephanie Seabolt is the greatest!" );
	if( copyString == TCString("Stephanie Seabolt is okay!") )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Equivalence test failed!" );
	}

	if( copyString != TCString( "Stephanie Seabolt is the greatest!" ) )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Non-equivalence test failed!" );
	}

	copyString = 'a';
	if( copyString != 'a' )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Equivalence test failed for a character!" );
	}

	if( copyString == 'b' )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Non-equivalence test failed for a character!" );
	}

	copyString = 11;
	if( copyString != "11" )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Equivalence test failed for a number!" );
	}

	if( copyString == "12" )
	{
		RETURN_UNIT_TEST_FAILURE( "[TCString] Non-equivalence test failed for a number!" );
	}

	//
	// Return success!
	//

	return Result::TestResult_Success;

} 