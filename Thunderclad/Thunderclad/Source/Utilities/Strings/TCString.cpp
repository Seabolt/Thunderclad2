//
// TCString.cpp
// This file will define the functionality of a string in 
//

//
// Includes
//

#include "TCString.h"
#include <malloc.h>

//
// Defines
//

//
// TCString Default Constructor
//		Will initialize a string to a safe state.
// Inputs:
//		- None
// Outputs:
//		- None
//

TCString::TCString()
{
	mString = NULL;
	Copy( EMPTY_STRING );
}

//
// TCString Memberwise Constructor
//		- Will intiialize this string to the string provided.
// Inputs:
//		- char8* string: The string to initialize to.
// Outputs:
//		- None.
//

TCString::TCString( const char8* string )
{
	mString = NULL;
	Copy( string );
}

//
// TCString Memberwise Constructor
//		- Will initialize this string to the string provided.
// Inputs:
//		- TCString& string: The string to initialize to.
// Outputs:
//		- None.
//

TCString::TCString( const TCString& string )
{
	mString = NULL;
	Copy( string );
	mLength = TCStringUtils::Length( Data() );
}

//
// TCString Destructor
//		- Will release all resources associated with this object.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCString::~TCString()
{
	ReleaseString( mString );
	mLength = 0;
}

//
// AllocateString
//		- Will allocate a new string at a specific size.
// Inputs:
//		- int length: The length of the new string.
// Outputs:
//		- char8*: The string that was allocated. NULL if there is a failure.
//

char8* TCString::AllocateString( int length )
{
	char8* string = (char8*)malloc( length );
	if( string == NULL )
	{
		// TODO: Log that there was an allocation failure. 
	}

	return string;
}

//
// ReleaseString
//		- Will release the memory of a specified string
// Inputs:
//		- char8* string: the string to release.
// Outputs:
//		- None.
//

void TCString::ReleaseString( char8*& string )
{
	if( string == NULL )
	{
		return;
	}

	free( string );
	string = NULL;
}

//
// ResizeString
//		- Will resize a string to a new size
// Inputs:
//		- int newSize: The size to resize the string to.
// Outputs:
//		- None.
//

void TCString::ResizeString( int newSize )
{
	//
	// Allocate a new string.
	//

	mString = AllocateString( newSize );
	if( mString == NULL )
	{
		return;
	}
}

//
// Length
//		- Will return the length of the string.
// Inputs:
//		- None.
// Outputs:
//		- int: The length of the string.
//

int TCString::Length()
{
	return mLength;
}

//
// IsEmpty
//		- Will return whether or not this string is empty
// Inputs:
//		- None.
// Outputs:
//		- bool: Is this string empty?
//

bool TCString::IsEmpty()
{
	return Equal( EMPTY_STRING );
}

//
// Clear
//		- Empty out this string.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCString::Clear()
{
	Copy( EMPTY_STRING );
}

//
// Copy
//		- Will copy from the provided string to the internal string.
// Inputs:
//		- const char8* string: The string to copy from.
// Outputs:
//		- None.
//

void TCString::Copy( const char8* string )
{
	//
	// Reallocate a string of the right size.
	//

	int inputStringLength = TCStringUtils::Length( string );
	Copy( string, inputStringLength );
}

//
// Copy
//		- Will copy from the provided string to the internal string.
// Inputs:
//		- const char8* string: The string to copy from.
//		- int length: The length of the string to copy from.
// Outputs:
//		- None.
//

void TCString::Copy( const char8* string, unsigned int length )
{
	if( string == NULL )
		return;

	//
	// Reallocate a string of the right size.
	//

	ReleaseString( mString );
	ResizeString( length + 1 );

	//
	// Copy in the new string.
	//

	TCStringUtils::Copy( mString, string, length );
	mString[ length ] = NULL_TERMINATOR;

	//
	// Update our length 
	//

	mLength = length;
}

//
// Copy
//		- Will copy from a TCString into our internal string.
// Inputs:
//		- TCString& string: The string to copy from.
// Outputs:
//		- None.
//

void TCString::Copy( const TCString& string )
{
	Copy( string.Data() );
}

//
// Copy
//		- Will copy from a TCString of a specified length into our internal string.
// Inputs:
//		- TCString& string: The string to copy from.
//		- unsigned int length: The length of the string to copy.
// Outputs:
//		- None.
//

void TCString::Copy( const TCString& string, unsigned int length )
{
	Copy( string.Data(), length );
}

//
// Append
//		- Will append the string provided to the internal string.
// Inputs:
//		- char8* string: the string to append.
// Outputs:
//		- None.
//

void TCString::Append( const char8* string )
{
	if( string == NULL )
		return;

	Append( string, TCStringUtils::Length( string ) );
}

//
// Append
//		- Will append a string to the internal string, reading only a specific length.
// Inputs:
//		- char8* string: The string to copy.
//		- unsigned int length: The length to read from the string.
// Outputs:
//		- None
//

void TCString::Append( const char8* string, unsigned int length )
{
	if( string == NULL )
		return;

	//
	// Calculate the size of the string we'll need.
	//

	int oldLength = Length();
	int newSize = Length() + length + 1;

	//
	// Resize the string.
	//

	char8* prevString = mString;
	ResizeString( newSize );

	//
	// Copy in our old string.
	//

	if( prevString != NULL )
	{
		TCStringUtils::Copy( mString, prevString );	
	}

	//
	// Release the previous string.
	//

	ReleaseString( prevString );

	//
	// Now we need to copy in the string to append.
	//

	TCStringUtils::Copy( &mString[oldLength], string, length );

	//
	// Update our length
	//

	mLength = newSize - 1;
}

//
// Append
//		- Will append a string to the end of the internal string.
// Inputs:
//		- const TCString& string: The string to append.
// Outputs:
//		- None.
//

void TCString::Append( const TCString& string )
{
	Append( string.Data() );
}

//
// Append
//		- Will append a string to the end of the internal string.
// Inputs:
//		- const TCString& string: The string to append.
//		- unsigned int length: The length of the string to append.
// Outputs:
//		- None.
//

void TCString::Append( const TCString& string, unsigned int length )
{
	Append( string.Data(), length );
}

//
// Compare
//		- Will compare the internal string with a string provided.
// Inputs:
//		- const char8* string: The string to compare.
// Outputs:
//		- int: 
//			- 0 if the strings are equal
//			- <0 if this string is less than the input.
//			- >0 if this string is greater than the input.
//

int TCString::Compare( const char8* string )
{
	return TCStringUtils::Compare( Data(), string );
}

//
// Compare
//		- Will compare the internal string with a string provided.
// Inputs:
//		- const TCString& string: the string to compare.
// Outputs:
//		- int: 
//			- 0 if the strings are equal
//			- <0 if this string is less than the input.
//			- >0 if this string is greater than the input.
//

int TCString::Compare( const TCString& string )
{
	return Compare( string.Data() );
}

//
// CompareInsensitive
//		- Will compare the internal string with a string provided with no regards to capitalization.
// Inputs:
//		- const char8* string: The string to compare.
// Outputs:
//		- int: 
//			- 0 if the strings are equal
//			- <0 if this string is less than the input.
//			- >0 if this string is greater than the input.
//

int TCString::CompareInsensitive( const char8* string )
{
	return TCStringUtils::CompareInsensitive( Data(), string );
}

//
// CompareInsensitive
//		- Will compare the internal string with a string provided with no regards to capitalization.
// Inputs:
//		- const TCString& string: The string to compare.
// Outputs:
//		- int: 
//			- 0 if the strings are equal
//			- <0 if this string is less than the input.
//			- >0 if this string is greater than the input.
//

int TCString::CompareInsensitive( const TCString& string )
{
	return CompareInsensitive( string.Data() );
}

//
// Equal
//		- Will determine if two strings are equivalent.
// Inputs:
//		- const char8* string: The string to compare.
// Outputs:
//		- None.
//

bool TCString::Equal( const char8* string )
{
	return Compare( string ) == 0;
}

//
// Equal
//		- Will determine if two strings are equivalent.
// Inputs:
//		- const char8* string: The string to compare.
// Outputs:
//		- None.
//

bool TCString::Equal( const TCString& string )
{
	return Equal( string.Data() );
}

//
// Contains
//		- Will determine if the provided string is in the internal string.
// Inputs:
//		- const char8* string: The string to check.
// Outputs:
//		- bool: Was the string found.
//

bool TCString::Contains( const char8* string )
{
	int checkLength = TCStringUtils::Length( string );
	for( unsigned int currentChar = 0; currentChar < mLength; ++currentChar )
	{
		if( mString[ currentChar ] == string[ 0 ] )
		{
			//
			// Check if this string is proceeded.
			//

			int currentCheckChar = 1;
			bool matched = true;
			for( currentCheckChar; currentCheckChar < checkLength; ++currentCheckChar )
			{
				if( mString[ currentChar + currentCheckChar ] == NULL_TERMINATOR )
				{
					// We've reached the end of our string.
					return false;
				}

				if( mString[ currentChar + currentCheckChar ] != string[ currentCheckChar ] )
				{
					// The strings don't match.
					currentChar += currentCheckChar;
					matched = false;
				}
			}

			//
			// We've reached the end of the search string with no misses, return success!
			//

			if( matched )
			{
				return true;
			}
		}
	}

	return false;
}

//
// Contains
//		- Will determine if a string provided is found inside our internal string.
// Inputs:
//		- const TCString& string: The string to search for.
// Outputs:
//		- bool: Was the string found.
//

bool TCString::Contains( const TCString& string )
{
	return Contains( string.Data() );
}

//
// FindFirstIndexOf
//		- Will return the first index of the specified character.
// Inputs:
//		- char8 character.
// Outputs:
//		- int: The index found at. -1 if not found.
//

int TCString::FindFirstIndexOf( char8 character )
{
	for( unsigned int currentChar = 0; currentChar < mLength; ++currentChar )
	{
		if( mString[ currentChar ] == character )
		{
			return currentChar;
		}
	}

	return -1;
}

//
// FindLastIndexOf
//		- Will return the last index of the specified character
// Inputs:
//		- char8 character: The character to find.
// Outputs:
//		- int: the index found at. -1 if not found.
//

int TCString::FindLastIndexOf( char8 character )
{
	for( int currentChar = mLength - 1; currentChar >= 0; --currentChar )
	{
		if( mString[ currentChar ] == character )
		{
			return currentChar;
		}
	}

	return -1;
}

//
// Reverse
//		- Will reverse this string.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCString::Reverse()
{
	int end = Length() - 1;
	int start = 0;

	while( start < end )
	{
		char8 c = mString[ start ];
		mString[ start ] = mString[ end ];
		mString[ end ] = c;

		start++;
		end--;
	}
}

//
// ToLower
//		- Will change all uppercase character to lower case.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCString::ToLower()
{
	for( unsigned int currentChar = 0; currentChar < mLength; ++ currentChar )
	{
		mString[ currentChar ] = TCStringUtils::ConvertToLowercase( mString[ currentChar ] );
	}
}

//
// ToUpper
//		- Will change all lowercase characters to uppercase.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCString::ToUpper()
{
	for( unsigned int currentChar = 0; currentChar < mLength; ++currentChar )
	{
		mString[ currentChar ] = TCStringUtils::ConvertToUppercase( mString[ currentChar ] );
	}
}

//
// Substring
//		- Will fill in the provided string with the substring specified.
// Inputs:
//		- int startIndex: The index where the substring should start.
//		- TCString& stringToFill: The string to fill.
// Outputs:
//		- None.
//

void TCString::Substring( unsigned int startIndex, TCString& stringToFill )
{
	Substring( startIndex, mLength - 1, stringToFill );
}

//
// Substring
//		- Will fill in the provided string with the substring specified.
// Inputs:
//		- int startIndex: The index where the substring should start.
//		- int endIndex: The index where the substring should end.
//		- TCString& stringToFill: The string to fill.
// Outputs:
//		- None.
//

void TCString::Substring( unsigned int startIndex, unsigned int endIndex, TCString& stringToFill )
{
	if( startIndex < 0 || startIndex >= mLength )
		return;

	if( endIndex < 0 || endIndex >= mLength )
		return;

	char8* substring = AllocateString( (endIndex - startIndex) + 2 ); // Include the last index + NULL_TERMINATOR
	TCStringUtils::Copy( substring, &mString[ startIndex ], endIndex - startIndex + 1);
	stringToFill.Copy( substring );
}

//
// TrimWhitespace
//		- Will remove all tabs, spaces, and line feeds from a string.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCString::TrimWhitespace()
{
	//
	// Determine how many whitespace characters are in the string.
	//

	int numWhitespaceCharacters = 0;
	for( unsigned int currentChar = 0; currentChar < mLength; ++currentChar )
	{
		if( TCStringUtils::IsWhitespaceCharacter( mString[ currentChar ] ) )
		{
			numWhitespaceCharacters++;
		}
	}

	//
	// Early out if possible.
	//

	if( numWhitespaceCharacters == 0 )
		return;

	//
	// Resize the string.
	//

	int newLength = mLength - numWhitespaceCharacters;
	int prevLength = mLength;
	char* prevString = mString;
	ResizeString( newLength + 1);
	mLength = newLength;

	//
	// Copy over all the characters except whitespace characters.
	//

	int currentTrimChar = 0;
	for( int currentChar = 0; currentChar < prevLength; ++currentChar )
	{
		if( !TCStringUtils::IsWhitespaceCharacter( prevString[ currentChar ] ) )
		{
			mString[ currentTrimChar++ ] = prevString[ currentChar ];
		}
	}

	//
	// Release the previous string.
	//

	ReleaseString( prevString );

	//
	// Update our length again.
	//

	mLength = newLength;
	mString[ mLength ] = NULL_TERMINATOR;
}

//
// StartsWith
//		- Will return if the string has the specified prefix.
// Inputs:
//		- char8* string: The string to check for.
// Outputs:
//		- bool: Does the string start with the prefix provided.
//

bool TCString::StartsWith( const char8* string )
{
	//
	// Check some early outs.
	//

	if( string == NULL )
	{
		return false;
	}

	if( mString[ 0 ] != string[ 0 ] )
	{
		return false;
	}

	//
	// Check to see if the prefix matches our strings start.
	//

	int prefixLength = TCStringUtils::Length( string );
	for( int currentChar = 0; currentChar < prefixLength; ++currentChar )
	{
		if( mString[ currentChar ] != string[ currentChar ] )
			return false;
	}

	return true;
}

//
// StartsWith
//		- Will determine if the string starts with the specified prefix
// Inputs:
//		- TCString& string: The string to search for.
// Outputs:
//		- bool: Did the string start with the prefix.
//

bool TCString::StartsWith( const TCString& string )
{
	return StartsWith( string.Data() );
}

//
// AssignmentOperator
//		- Will set the value of this string to the string specified.
// Inputs:
//		- char8* string: The string to set this string to.
// Outputs:
//		- const TCString&: The resultant string.
//

TCString& TCString::operator=( const char8* string )
{
	Copy( string );
	return *this;
}

//
// Assignment Operator
// Inputs:
//		- TCString& string: The string to set this string to.
// Outputs:
//		- const TCString&: The resultant string.
//

TCString& TCString::operator=( const TCString& string )
{
	return (*this) = string.Data();
}

//
// Assignment Operator
// Inputs:
//		- char8 character: The character to set this string to.
// Outputs:
//		- const TCString&: The resultant string.
//

TCString& TCString::operator=( const char8 character )
{
	char8 string[2];
	string[ 0 ] = character;
	string[ 1 ] = NULL_TERMINATOR;

	return (*this) = string;
}

//
// Assignment Operator
// Inputs:
//		- int value: The value to set this string to.
// Outputs:
//		- const TCString&: The resultant string.
//

TCString& TCString::operator=( const int value )
{
	TCString valueStr = TCStringUtils::ItoA( value );
	return (*this) = valueStr;
}

//
// Concatentation Operator
//		- Will return a string that is the result of this string appended to the string provided.
// Inputs:
//		- char8* string: The string to append this string to.
// Outputs:
//		- const TCString&: The resultant string.
//

TCString TCString::operator+( const char8* string )
{
	TCString toReturn = *this;
	toReturn.Append( string );
	return toReturn;
}

//
// Concatentation Operator
//		- Will return a string that is the result of this string appended to the string provided.
// Inputs:
//		- TCString& string: The string to append this string to.
// Outputs:
//		- const TCString&: The resultant string.
//

TCString TCString::operator+( const TCString& string )
{
	return (*this) + string.Data();
}

//
// Concatentation Operator
//		- Will return a string that is the result of this string appended to the string provided.
// Inputs:
//		- char8 character: The character to append to the string.
// Outputs:
//		- const TCString&: The resultant string.
//

TCString TCString::operator+( const char8 character )
{
	char8 string[2];
	string[ 0 ] = character;
	string[ 1 ] = NULL_TERMINATOR;

	return (*this) + string;
}

//
// Concatenation Operator
//		- Will return a string that is the result of this string appended to the string provided.
// Inputs:
//		- int value: The value to append to the string.
// Outputs:
//		- TCString: The resultant string.
//

TCString TCString::operator+( const int value )
{
	TCString valueStr = TCStringUtils::ItoA( value );
	return (*this) + valueStr;
}

//
// Append Operator
//		- Will append the specified string to this string.
// Inputs:
//		- char8* string: the string to append.
// Outputs:
//		None.
//

void TCString::operator+=( const char8* string )
{
	Append( string );
}

//
// Append Operator
//		- Will append the specified string to this string.
// Inputs:
//		- TCString string: the string to append.
// Outputs:
//		None.
//

void TCString::operator+=( const TCString& string )
{
	(*this) += string.Data();
}

//
// Append Operator
//		- Will append the specified string to this string.
// Inputs:
//		- char8 character: the character to append.
// Outputs:
//		None.
//

void TCString::operator+=( const char8 character )
{
	char8 string[2];
	string[ 0 ] = character;
	string[ 1 ] = NULL_TERMINATOR;

	(*this) += string;
}

//
// Append Operator
//		- Will append the specified value to this string.
// Inputs:
//		- int value: The value to append.
// Outputs:
//		- None.
//

void TCString::operator+=( const int value )
{
	TCString valStr = TCStringUtils::ItoA( value );
	(*this) += valStr;
}

//
// Equivalence Operator
//		- Will determine if this string is equal to another.
// Inputs:
//		- char8* string: The string to check.
// Outputs:
//		- bool: Is this string the same as the string provided.
//

bool TCString::operator==( const char8* string )
{
	return Equal( string );
}

//
// Equivalence Operator
//		- Will determine if this string is equal to another.
// Inputs:
//		- TCString& string: The string to check.
// Outputs:
//		- bool: Is this string the same as the string provided.
//

bool TCString::operator==( const TCString& string )
{
	return (*this) == string.Data();
}

//
// Equivalence Operator
//		- Will determine if this string is equal to another.
// Inputs:
//		- char8 character: The character to check.
// Outputs:
//		- bool: Is this string the same as the string provided.
//

bool TCString::operator==( const char8 character )
{
	char8 string[2];
	string[ 0 ] = character;
	string[ 1 ] = NULL_TERMINATOR;

	return (*this) == string;
}

//
// Equivalence Operator
//		- Will determine if this string is equal to the value.
// Inputs:
//		- int value: The value to check.
// Outputs:
//		- bool: Is this string the same as the value provided.
//

bool TCString::operator==( const int value )
{
	TCString valStr = TCStringUtils::ItoA( value );
	return (*this) == valStr;
}

//
// Unequivalence Operator
//		- Will determine if this string is not equal to another.
// Inputs:
//		- const char8* string: The string to check.
// Outputs:
//		- None.
//

bool TCString::operator!=( const char8* string )
{
	return !Equal( string );
}

//
// Unequivalence Operator
//		- Will determine if this string is not equal to another
// Inputs:
//		- const TCString& string: The string to check.
// Outputs:
//		- None.
//

bool TCString::operator!=( const TCString& string )
{
	return (*this) != string.Data();
}

//
// Unequivalence Operator
//		- Will determine if this string is not equal to another
// Inputs:
//		- const char8 character: The character to check.
// Outputs:
//		- None.
//

bool TCString::operator!=( const char8 character )
{
	char8 string[2];
	string[ 0 ] = character;
	string[ 1 ] = NULL_TERMINATOR;

	return (*this) != string;
}

//
// Unequivalence Operator
//		- Will determine if this string is not equal to the value.
// Inputs:
//		- const int value: The value to check against.
// Outputs:
//		- bool: Does this character not equal this value.
//

bool TCString::operator!=( const int value )
{
	return !( (*this) == value );
}