//
// TCStringUtils
// This file will define various string manipulation methods and utility functions.
//

#ifndef __TCSTRING_UTILS_H__
#define __TCSTRING_UTILS_H__

//
// Includes
//

//
// Defines
//

#define NULL_TERMINATOR ('\0')
#define EMPTY_STRING ("\0")

//
// Typedefs
//

typedef short char16;
typedef char char8;

//
// Forward Declaration
//

class TCString;

namespace TCStringUtils
{
	int Length( const char8* string );
	void Copy( char8* copyTo, const char8* copyFrom, int length );
	void Copy( char8* copyTo, const char8* copyFrom );
	int Compare( const char8* lhs, const char8* rhs );
	int CompareInsensitive( const char8* lhs, const char8* rhs );
	char8 ConvertToLowercase( char8 character );
	char8 ConvertToUppercase( char8 character );
	bool IsWhitespaceCharacter( char8 character );
	bool IsNumericCharacter( char8 character );

	TCString ItoA( int value, int base = 10 );
	int AtoI( TCString string );
}

#endif // __TCSTRING_UTILS_H__