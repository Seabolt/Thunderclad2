#ifndef __TCPOINT2D_H__
#define __TCPOINT2D_H__

//
// Includes
//

//
// Defines
//

//
// TCPoint2D
// This object is responsible for representing the two components (x,y) 
// that make a point in 2 dimensional space
//

class TCPoint2D
{
	public:		// Methods
		
		//
		// Constructors
		//

		TCPoint2D();
		TCPoint2D( float x, float y );
		TCPoint2D( const TCPoint2D& point );

		//
		// Operators
		//

		TCPoint2D& operator=( const TCPoint2D& ref );
		TCPoint2D  operator+( const TCPoint2D& ref );
		TCPoint2D  operator-( const TCPoint2D& ref );
		TCPoint2D  operator*( float scalar );
		TCPoint2D  operator/( float scalar );
		TCPoint2D& operator+=( const TCPoint2D& ref );
		TCPoint2D& operator-=( const TCPoint2D& ref );
		TCPoint2D& operator*=( float scalar );
		TCPoint2D& operator/=( float scalar );

		void Set( float x, float y );

	public:		// Members

		float x;
		float y;
};


#endif