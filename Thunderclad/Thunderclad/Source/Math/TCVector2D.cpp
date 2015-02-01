//
// TCVector2D.cpp
// This file will define the functionality of a two dimensional vector.
//

//
// Includes
//

#include "TCVector2D.h"
#include "TCMathUtils.h"

//
// Defines
//

//
// Dot
//		- This function will return the dot or inner product of two vectors.
// Inputs:
//		- const TCVector2D& in: The vector to dot with.
// Outputs:
//		- float: Dot products yield a scalar equal to the cosine between the two vectors,
//				 multiplied by the lengths of the two vectors.
//

float TCVector2D::Dot( const TCVector2D& in )
{
	return (x * in.x) + (y * in.y);
}

//
// Cross 
//		- This function will return a determinant of a 2x2 matrix made of the two vectors.
// Inputs:
//		- const TCVector2D& in: The vector to cross with.
// Outputs:
//		- float: The determinant of a 2x2 matrix made of the two vectors.
//

float TCVector2D::Cross( const TCVector2D& in )
{
	return (x * in.y) - (y * in.x);
}

//
// IsPerpendicular
//		- This function will return if this vector and the input vector are perpendicular
// Inputs:
//		- const TCVector2D& in: The vector to check against.
// Outputs:
//		- bool: True if the two vectors are perpendicular. False if not.
//

bool TCVector2D::IsPerpendicular( const TCVector2D& in )
{
	return Dot( in ) == 0.0f;
}

//
// IsFacingSameDirectionAs
//		- This function will return if this vector and the input vector are facing the same direction.
// Inputs:
//		- const TCVector2D& in: The vector to check against.
// Outputs:
//		- bool: Will return if the two vectors are facing the same direction.
//

bool TCVector2D::IsFacingSameDirectionAs( const TCVector2D& in )
{
	return Dot( in ) > 0;
}

//
// Length
//		- This function will return the length or magnitude of the vector.
// Inputs:
//		- None.
// Outputs:
//		- float: The length or magnitude of the vector.
//

float TCVector2D::Length()
{
	return TCMathUtils::Sqrt( x*x + y*y );
}

//
// LengthSquared
//		- This function will return the length squared of the vector, not accurate but is useful for performant comparisons.
// Inputs:
//		- None.
// Outputs:
//		- float: The length or magnitude of the vector squared.
//

float TCVector2D::LengthSquared()
{
	return Dot( *this );
}

//
// Normalize
//		- This function will normalize this vector so it is a length of one.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

void TCVector2D::Normalize()
{
	float magnitude = Length();
	x /= magnitude;
	y /= magnitude;
}

//
// Angle
//		- Will return the angle in radians between the two vectors.
// Inputs:
//		- const TCVector2D& in: the vector to determine the angle between.
// Outputs:
//		- float: The angle between the two vectors.
//

float TCVector2D::Angle( const TCVector2D& in )
{
	TCVector2D thisNormalized = *this;
	thisNormalized.Normalize();

	TCVector2D inNormalized = in;
	inNormalized.Normalize();

	return TCMathUtils::Acos( thisNormalized.Dot( inNormalized ) );
}