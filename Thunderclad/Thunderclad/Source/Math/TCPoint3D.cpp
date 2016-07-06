//
// TCPoint3D.cpp
//		- This file will define the functionality of a Three Dimensional Point in space.
//

//
// Includes
//

#include "TCPoint3D.h"

//
// Defines
//

//
// Default constructor
//

TCPoint3D::TCPoint3D()
{
	x = 0.0f;
	y = 0.0f;
}

//
// Memberwise constructor
//

TCPoint3D::TCPoint3D( float inX, float inY, float inZ )
{
	x = inX;
	y = inY;
	z = inZ;
}

//
// Copy Constructor
//

TCPoint3D::TCPoint3D( const TCPoint3D& ref )
{
	x = ref.x;
	y = ref.y;
	z = ref.z;
}

//
// Assignment Operator
//

TCPoint3D& TCPoint3D::operator=( const TCPoint3D& ref )
{
	x = ref.x;
	y = ref.y;
	z = ref.z;

	return *this;
}

//
// Addition Operator
//

TCPoint3D TCPoint3D::operator+( const TCPoint3D& ref )
{
	return TCPoint3D( x + ref.x, y + ref.y, z + ref.z );
}

//
// Subtraction Operator
//

TCPoint3D TCPoint3D::operator-( const TCPoint3D& ref )
{
	return TCPoint3D( x - ref.x, y - ref.y, z - ref.z );
}

//
// Scalar Multiply Operator
//

TCPoint3D TCPoint3D::operator*( float scalar )
{
	return TCPoint3D( x * scalar, y * scalar, z * scalar );
}

//
// Scalar Divide Operator
//

TCPoint3D TCPoint3D::operator/( float scalar )
{
	return TCPoint3D( x / scalar, y / scalar, z / scalar );
}

//
// Addition Assignment Operator
//

TCPoint3D& TCPoint3D::operator+=( const TCPoint3D& ref )
{
	x += ref.x;
	y += ref.y;
	z += ref.z;

	return *this;
}

//
// Subtraction Assignment Operator
//

TCPoint3D& TCPoint3D::operator-=( const TCPoint3D& ref )
{
	x -= ref.x;
	y -= ref.y;
	z -= ref.z;

	return *this;
}

//
// Scalar Multiply Assignment Operator
//

TCPoint3D& TCPoint3D::operator*=( float scalar )
{
	x *= scalar;
	y *= scalar;
	z *= scalar;

	return *this;
}

//
// Scalar Divide Assignment Operator
//

TCPoint3D& TCPoint3D::operator/=( float scalar )
{
	x /= scalar;
	y /= scalar;
	z /= scalar;

	return *this;
}

void TCPoint3D::Set( float inX, float inY, float inZ )
{
	x = inX;
	y = inY;
	z = inZ;
}
