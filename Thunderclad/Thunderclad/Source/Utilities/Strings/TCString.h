//
// TCString.h
// This class is used to encapsulate strings and string functionality.
//

#ifndef __TC_STRING_H__
#define __TC_STRING_H__

//
// Includes
//

#include "TCPlatformPrecompilerSymbols.h"
#include "TCStringUtils.h"

//
// Defines
//


class TCString
{
	public: // Members
	public: // Methods

		TCString();
		TCString( const char8* string );
		TCString( const TCString& string );

		virtual ~TCString();

		int Length();

		bool IsEmpty();
		void Clear();

		void Copy( const char8* string );
		void Copy( const char8* string, unsigned int length );
		void Copy( const TCString& string );
		void Copy( const TCString& string, unsigned int length );
			
		void Append( const char8* string );
		void Append( const char8* string, unsigned int length );
		void Append( const TCString& string );
		void Append( const TCString& string, unsigned int length );

		int Compare( const char8* string );
		int Compare( const TCString& string );

		int CompareInsensitive( const char8* string );
		int CompareInsensitive( const TCString& string );

		bool Equal( const char8* string );
		bool Equal( const TCString& string );

		bool Contains( const char8* string );
		bool Contains( const TCString& string );

		int FindFirstIndexOf( const char8 character );
		int FindLastIndexOf( const char8 character );

		void Reverse();

		void ToLower();
		void ToUpper();

		void Substring( unsigned int startIndex, TCString& outString );
		void Substring( unsigned int startIndex, unsigned int endIndex, TCString& outString );

		void TrimWhitespace();

		bool StartsWith( const char8* string );
		bool StartsWith( const TCString& string );

		TCString& operator=( const TCString& string );
		TCString& operator=( const char8* string );
		TCString& operator=( const char8 character );
		TCString& operator=( const int value );

		TCString operator+( const TCString& string );
		TCString operator+( const char8* string );
		TCString operator+( const char8 character );
		TCString operator+( const int value );

		void operator+=( const TCString& string );
		void operator+=( const char8* string );
		void operator+=( const char character );
		void operator+=( const int value );

		bool operator==( const TCString& string );
		bool operator==( const char8* string );
		bool operator==( const char character );
		bool operator==( const int value );

		bool operator!=( const TCString& string );
		bool operator!=( const char8* string );
		bool operator!=( const char character );
		bool operator!=( const int value );

		char8 operator[]( unsigned int index ) { TC_ASSERT( mString != NULL && index > 0 && index < mLength ); return mString[ index ]; }

		char8* Data() const { return mString; } 

	private: // Members
		char8* mString;
		unsigned int mLength;

	protected: // Methods

		char8* AllocateString( int length );
		void ReleaseString( char8*& string );
		void ResizeString( int newSize );
};

#endif // __TC_STRING_H__