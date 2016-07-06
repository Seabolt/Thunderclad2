//
// TCStringUtils.cpp
// This file will define the various string utility functions
//

//
// Includes
//

#include "TCStringUtils.h"
#include "TCMathUtils.h"
#include "TCString.h"

//
// Defines
//

#define TC_ASCII_LOWERCASE_RANGE_START	(97)
#define TC_ASCII_LOWERCASE_RANGE_END	(122)
#define TC_ASCII_UPPERCASE_RANGE_START	(65)
#define TC_ASCII_UPPERCASE_RANGE_END	(90)
#define TC_ASCII_CASE_CONVERSION_AMOUNT	(TC_ASCII_LOWERCASE_RANGE_START - TC_ASCII_UPPERCASE_RANGE_START)
#define TC_ASCII_NUMERIC_RANGE_START	(48)
#define TC_ASCII_NUMERIC_RANGE_END		(57)

namespace TCStringUtils
{
	//
	// Length
	//		- Will determine the length of a string
	// Inputs:
	//		- char8* string: The string to check.
	// Outputs:
	//		- int: The length of the string.
	//

	int Length( const char8* string )
	{
		if( string == NULL )
			return 0;

		int size = 0;
		while( string[ size ] != NULL_TERMINATOR )
		{
			size++;
		}

		return size;
	}

	//
	// Copy
	//		- Will copy from one string to the other string up to length
	// Inputs:
	//		- char8* copyTo: The string to copy to.
	//		- char8* copyFrom: The string to copy from.
	//		- int length: The maximum number of characters to copy.
	// Outputs:
	//		- None.
	//

	void Copy( char8* copyTo, const char8* copyFrom, int length )
	{
#ifdef _DEBUG
		if( copyTo == NULL || copyFrom == NULL )
		{
			TC_ASSERT( 0 && "String Copy failed! One of the strings is NULL");
			return;
		}
#endif

		int currentChar = 0;
		for( currentChar; currentChar < length; ++currentChar )
		{			
			copyTo[ currentChar ] = copyFrom[ currentChar ];

			if( copyFrom[ currentChar ] == NULL_TERMINATOR )
			{
				break;
			}
		}

		copyTo[ currentChar ] = NULL_TERMINATOR;
	}

	//
	// Copy
	//		- Will copy from one string to the other until the end of copyFrom
	// Inputs:
	//		- char8* copyTo: the string to copy into.
	//		- char8* copyFrom: The string to copy from.
	// Outputs:
	//		- None.
	//

	void Copy( char8* copyTo, const char8* copyFrom )
	{
#if _DEBUG
		if( copyTo == NULL || copyFrom == NULL )
		{
			TC_ASSERT( 0 && "String copy failed! One of the strings is NULL" );
			return;
		}
#endif

		int currentChar = 0;
		while( true )
		{
			copyTo[ currentChar ] = copyFrom[ currentChar ];
			if( copyTo[ currentChar ] == NULL_TERMINATOR )
				break;

			currentChar++;
		}
	}

	//
	// Compare
	//		- Will compare two strings and will determine if they are the same string.
	// Inputs:
	//		- char8* lhs: The left hand string to compare
	//		- char8* rhs: The right hand string to compare.
	// Outputs:
	//		- int: The result of the compare. If 0, the strings are the same, otherwise it will be the larger of the two characters where the string differs.
	//

	int Compare( const char8* lhs, const char8* rhs )
	{
		if( lhs == rhs )
			return 0;

		if( lhs == NULL )
			return rhs[0];

		if( rhs == NULL )
			return lhs[0];

		int currentChar = 0;
		while(true)
		{
			if( lhs[ currentChar ] == NULL_TERMINATOR &&
				rhs[ currentChar ] == NULL_TERMINATOR )
			{
				return 0;
			}

			if( lhs[ currentChar ] != rhs[ currentChar ] )
			{
				if( lhs[ currentChar ] < rhs[ currentChar ] )
				{
					return -1;
				}
				else
				{
					return 1;
				}
			}

			currentChar++;

		}
	}

	//
	// CompareInsensitive
	//		- Will compare two strings and will determine if they are the same string with no regard to capitalization.
	// Inputs:
	//		- char8* lhs: The left hand string to compare
	//		- char8* rhs: The right hand string to compare.
	// Outputs:
	//		- int: The result of the compare. If 0, the strings are the same, otherwise it will be the larger of the two characters where the string differs.
	//

	int CompareInsensitive( const char8* lhs, const char8* rhs )
	{
		if( lhs == rhs )
			return 0;

		if( lhs == NULL )
			return rhs[0];

		if( rhs == NULL )
			return lhs[0];

		int currentChar = 0;
		char8 lhsChar = 0;
		char8 rhsChar = 0;
		while(true)
		{
			lhsChar = ConvertToLowercase( lhs[ currentChar ] );
			rhsChar = ConvertToLowercase( rhs[ currentChar ] );

			if( lhsChar == NULL_TERMINATOR &&
				rhsChar == NULL_TERMINATOR )
			{
				return 0;
			}

			if( lhsChar != rhsChar )
			{
				if( lhsChar < rhsChar )
				{
					return -1;
				}
				else
				{
					return 1;
				}
			}

			currentChar++;
		}
	}

	//
	// ConvertToLowercase
	//		- Will return the lower case variant of the character provided.
	// Inputs:
	//		- char8 character: The character to convert.
	// Outputs:
	//		- char8: The converted character.
	//

	char8 ConvertToLowercase( char8 character )
	{
		if( character > TC_ASCII_UPPERCASE_RANGE_END ||
			character < TC_ASCII_UPPERCASE_RANGE_START )
		{
			return character;
		}
		else
		{
			return character + TC_ASCII_CASE_CONVERSION_AMOUNT;
		}
	}

	//
	// ConvertToUppercase
	//		- Will return the upper case variant of the character.
	// Inputs:
	//		- char8 character: The character to convert.
	// Outputs:
	//		- char8: The converted character.
	//

	char8 ConvertToUppercase( char8 character )
	{
		if( character > TC_ASCII_LOWERCASE_RANGE_END||
			character < TC_ASCII_LOWERCASE_RANGE_START )
		{
			return character;
		}
		else
		{
			return character - TC_ASCII_CASE_CONVERSION_AMOUNT;
		}
	}

	//
	// IsWhitespaceCharacter
	//		- Will return whether the character provided is a whitespace character.
	// Inputs:
	//		- char8 character: The character to test.
	// Outputs:
	//		- bool: Will return if the character is whitespace or not.
	//

	bool IsWhitespaceCharacter( char8 character )
	{
		if( character == ' ' ||
			character == '\t' ||
			character == '\r' ||
			character == '\n' )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//
	// IsNumericCharacter
	//		- Will return whether the character provided is a numeric character.
	// Inputs:
	//		- char8 character: The character to test.
	// Outputs:
	//		- bool: Will return if the character is numeric or not.
	//

	bool IsNumericCharacter( char8 character )
	{
		if( character >= '0' && character <= '9' )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//
	// ItoA
	//		- Will convert from an integer value to a string.
	// Inputs:
	//		- int value: The value to convert to a string.
	//		- int base: The base for the integer.
	// Outputs:
	//		- TCString: The resultant string.
	//

	TCString ItoA( int value, int base )
	{
		//
		// Early out if the value is zero.
		//

		if( value == 0 )
		{
			return TCString( "0" );
		}

		//
		// The standard itoa only will negate numbers if the base is 10.
		//

		bool isNegative = false;
		if( value < 0 && base == 10 )
		{
			isNegative = true;
			value = -value;
		}

		//
		// Walk the list and calculate the string.
		//

		TCString retString;
		while( value != 0 )
		{
			int remainder = value % base;
			retString += (char8)(( remainder > 9 ) ? ( remainder - 10 ) + 'a' : remainder + '0');
			value /= base;
		}

		//
		// Append negative sign if applicable
		//

		if( isNegative )
		{
			retString += '-';
		}

		//
		// Reverse the string.
		//

		retString.Reverse();

		return retString;
	}

	//
	// AtoI
	//		- Will take a string and convert it to an integer.
	// Inputs:
	//		- TCString string: The string to convert.
	// Outputs:
	//		- int: The value represented in the string.
	//

	int AtoI( TCString& string )
	{
		//
		// Check to see if the string is empty.
		//

		if( string.IsEmpty() )
			return 0;

		//
		// Determine the sign 
		//

		int sign = 1;
		int offset = 0;
		if( string[ 0 ] == '-' )
		{
			sign = -1;
			offset++;
		}

		//
		// Calculate the value.
		//

		int result = 0;
		for( int currentChar = offset; currentChar < string.Length(); ++currentChar )
		{
			if( !IsNumericCharacter( string[ currentChar ] ) )
			{
				return 0;
			}

			result = result * 10 + string[ currentChar ] - TC_ASCII_NUMERIC_RANGE_START;
		}

		return result * sign;
	}

	//
	// FtoA
	//		- Will convert a float to a string.
	// Inputs:
	//		- float value: The float to convert.
	//		- int precision: The precision of the string.
	// Outputs:
	//		- TCString: The string representing the float.
	//

	TCString TCStringUtils::FtoA( float value, int precision )
	{
		//
		// Handle our special cases.
		//

		if( isnan( value ) )
		{
			return TCString( "NaN" );
		}
		else if( isinf( value ) )
		{
			return TCString( "Inf" );
		}
		else if( value == 0.0f )
		{
			return TCString( "0" );
		}

		TCString result;
		char text[ 32 ];
		char* textBuffer = text;

		int magnitude = (int)TCMathUtils::Log10( value );
		int sciNotationMagnitude = magnitude;
		int digit = -1;

		bool isNegative = value < 0.0f;
		bool printExponent = magnitude >= 14 || ( isNegative && magnitude >= 9 || magnitude <= -9 );
		
		if( isNegative ) *(textBuffer++) = '-';

		//
		// Setup for scientific notation if necessary.
		//

		if( printExponent )
		{
			if( magnitude < 0 )
			{
				magnitude -= 1;
			}

			value = value / TCMathUtils::Pow( 10.0f, magnitude );	// Shear off the exponent and get the mantissa.
			sciNotationMagnitude = magnitude;
			magnitude = 0;
		}

		//
		// Clamp our magnitude
		//

		if( magnitude < 1 )
		{
			magnitude = 0;
		}

		//
		// Generate the digits.
		//

		while( value > precision || magnitude >= 0 )
		{
			//
			// Determine the current digit.
			//

			float weight = TCMathUtils::Pow( 10.0f, magnitude );
			if( weight > 0 && !isinf( weight ) )
			{
				digit = (int)TCMathUtils::Floor( value / weight );		// Get the current digit.
				value -= digit * weight;								// Reduce value by the digit we just added, eg, move from hundreds to tens.
				*(textBuffer++) = TC_ASCII_NUMERIC_RANGE_START + digit;	// Append the numeric character, eg: '0', '1'
			}

			//
			// Check to see if we need to add the decimal.
			//

			if( magnitude == 0 && value > 0 )
			{
				*(textBuffer++) = '.'; 
			}

			//
			// Reduce our magnitude
			//

			magnitude--;
		}

		//
		// Append the scientific notation, if necessary.
		//

		if( printExponent )
		{
			//
			// Determine the prefix's sign.
			//

			*(textBuffer++) = 'e';
			if( sciNotationMagnitude > 0 )
			{
				*(textBuffer++) = '+';
			}
			else
			{
				*(textBuffer++) = '-';
				sciNotationMagnitude = -sciNotationMagnitude;
			}

			magnitude = 0;
			while( sciNotationMagnitude > 0 )
			{
				*(textBuffer++) = TC_ASCII_NUMERIC_RANGE_START + (sciNotationMagnitude % 10); // Determine the current notation digit.
				sciNotationMagnitude /= 10;
				magnitude++; // Use magnitude as a counter for how many digits are in the notation.
			}

			//
			// Swap the digits order, since they are outputted backwards
			//

			textBuffer -= magnitude; // Move back to the start of the notation.
			int startOfMagnitude = 0;
			int endOfMagnitude = 0;
			for( startOfMagnitude = 0, endOfMagnitude = magnitude - 1; startOfMagnitude < endOfMagnitude; ++startOfMagnitude, --endOfMagnitude )
			{
				textBuffer[ startOfMagnitude ] ^= textBuffer[ endOfMagnitude ];
				textBuffer[ endOfMagnitude ] ^= textBuffer[ startOfMagnitude ];
				textBuffer[ startOfMagnitude ] ^= textBuffer[ endOfMagnitude ];
			}
			textBuffer += magnitude; // Move back to the end of the string.
		}

		//
		// Terminate the string and return the result.
		//

		*(textBuffer) = '\0';
		result = textBuffer;
		return result;
	}

	//
	// AtoF
	//		- Will convert a string to a float.
	// Inputs:
	//		- TCString text: The text of the float to convert.
	// Outputs:
	//		- float: The float result.
	//

	float TCStringUtils::AtoF( TCString& text )
	{
		char8* string				= text.Data();
		bool isNegative				= string[ 0 ] == '-';
		bool negativeExponent		= false;
		int decimalIndex			= -1;
		int scientificNotationIndex = -1;
		int scientificNotationExp	= 0;
		int mantissa				= 0;
		int sciNotationExponent		= 0;
		float exponent				= 0.0f;

		//
		// First we need to find out the mantissa.
		//

		for( int currentChar = 0; currentChar < text.Length(); ++currentChar )
		{
			if( string[ currentChar ] == '.' )
			{
				decimalIndex = currentChar;
				break;
			}

			if( string[ currentChar ] == 'f' )
				continue;

			mantissa = mantissa * 10 + (string[ currentChar ] - TC_ASCII_NUMERIC_RANGE_START);
		}

		//
		// Next we need to determine the exponent.
		//

		if( decimalIndex > 0 )
		{
			int numExponentDigits = 0;
			for( int currentChar = decimalIndex + 1; currentChar < text.Length(); ++currentChar )
			{
				if( string[ currentChar ] == 'e' )
				{
					scientificNotationIndex = currentChar + 2; // Move past the 'e' and the sign ('-','+')
					negativeExponent = string[ currentChar + 1 ] == '-';
					break;
				}

				if( string[ currentChar ] == 'f' )
				{
					continue;
				}

				exponent = exponent * 10.0f + (float)(string[ currentChar ] - TC_ASCII_NUMERIC_RANGE_START);
			}
		}


		//
		// Next we need to read the scientific notation exponent.
		//

		if( scientificNotationIndex > 0 )
		{
			for( int currentChar = scientificNotationIndex; currentChar < text.Length(); ++currentChar )
			{
				if( string[ currentChar ] == 'f' )
					continue;

				scientificNotationExp = scientificNotationExp * 10 + (string[ currentChar ] - TC_ASCII_NUMERIC_RANGE_START);
			}
		}

		//
		// Now we need to combine everything to make the actual value.
		//

		float result = (float)mantissa;
		
		if( exponent > 0.0f )
		{
			while( exponent > 1.0f ) exponent /= 10.0f;
			result += exponent;
		}

		if( scientificNotationExp > 0.0f )
		{
			result *= TCMathUtils::Pow( 10, scientificNotationExp );
		}

		return result;
	}
}