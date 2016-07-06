//
// TCPoint3D.h
//		- This file will define a 3 dimensional point in space.
//

#ifndef __TC_POINT_3D_H__
#define __TC_POINT_3D_H__

//
// Includes
//

//
// Defines
//

//
// Forward Declarations
//

//
// Class Declaration
//

class TCPoint3D
{
	public:		// Members
		
		//
		// Constructors
		//

		TCPoint3D();
		TCPoint3D( float x, float y, float z );
		TCPoint3D( const TCPoint3D& point );

		//
		// Operators
		//

		TCPoint3D& operator=( const TCPoint3D& ref );
		TCPoint3D  operator+( const TCPoint3D& ref );
		TCPoint3D  operator-( const TCPoint3D& ref );
		TCPoint3D  operator*( float scalar );
		TCPoint3D  operator/( float scalar );
		TCPoint3D& operator+=( const TCPoint3D& ref );
		TCPoint3D& operator-=( const TCPoint3D& ref );
		TCPoint3D& operator*=( float scalar );
		TCPoint3D& operator/=( float scalar );

		void Set( float x, float y, float z );

	public:		// Methods
		union 
		{
			struct{ float x; float y; float z; };
			float components[ 3 ];
		};

	protected:	// Members
	protected:	// Methods
};

#endif // __TC_POINT_3D_H__