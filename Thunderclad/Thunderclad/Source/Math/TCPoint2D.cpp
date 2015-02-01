//
// TCPoint2D.cpp
// This file will define the methods for TCPoint2D
//

//
// Includes
//

#include "TCPoint2D.h"

//
// Default constructor
//

TCPoint2D::TCPoint2D()
{
	x = 0.0f;
	y = 0.0f;
}

//
// Memberwise constructor
//

TCPoint2D::TCPoint2D( float inX, float inY )
{
	x = inX;
	y = inY;
}

//
// Copy Constructor
//

TCPoint2D::TCPoint2D( const TCPoint2D& ref )
{
	x = ref.x;
	y = ref.y;
}

//
// Assignment Operator
//

TCPoint2D& TCPoint2D::operator=( const TCPoint2D& ref )
{
	x = ref.x;
	y = ref.y;

	return *this;
}

//
// Addition Operator
//

TCPoint2D TCPoint2D::operator+( const TCPoint2D& ref )
{
	return TCPoint2D( x + ref.x, y + ref.y );
}

//
// Subtraction Operator
//

TCPoint2D TCPoint2D::operator-( const TCPoint2D& ref )
{
	return TCPoint2D( x - ref.x, y - ref.y );
}

//
// Scalar Multiply Operator
//

TCPoint2D TCPoint2D::operator*( float scalar )
{
	return TCPoint2D( x * scalar, y * scalar );
}

//
// Scalar Divide Operator
//

TCPoint2D TCPoint2D::operator/( float scalar )
{
	return TCPoint2D( x / scalar, y / scalar );
}

//
// Addition Assignment Operator
//

TCPoint2D& TCPoint2D::operator+=( const TCPoint2D& ref )
{
	x += ref.x;
	y += ref.y;

	return *this;
}

//
// Subtraction Assignment Operator
//

TCPoint2D& TCPoint2D::operator-=( const TCPoint2D& ref )
{
	x -= ref.x;
	y -= ref.y;

	return *this;
}

//
// Scalar Multiply Assignment Operator
//

TCPoint2D& TCPoint2D::operator*=( float scalar )
{
	x *= scalar;
	y *= scalar;

	return *this;
}

//
// Scalar Divide Assignment Operator
//

TCPoint2D& TCPoint2D::operator/=( float scalar )
{
	x /= scalar;
	y /= scalar;

	return *this;
}

void TCPoint2D::Set( float inX, float inY )
{
	x = inX;
	y = inY;
}
