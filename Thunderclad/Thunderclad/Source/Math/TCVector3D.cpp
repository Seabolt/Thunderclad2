//
// TCVector3D.cpp
// This file will define the functionality of a three dimensional vector.
//

//
// Includes
//

#include "TCVector3D.h"
#include "TCMathUtils.h"

//
// Defines
//

//
// Default Constructor
//		- Will initialize this vector to a default state.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCVector3D::TCVector3D()
{
	x = y = z = 0.0f;
}

//
// Copy Constructor
//		- Will intiialize this vector to match another.
// Inputs:
//		- TCVector3D& inRef: The instance to copy.
// Outputs:
//		- None
//

TCVector3D::TCVector3D( const TCVector3D& vec )
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

//
// Memberwise Constructor
//		- Will initialize this vectors components.
// Inputs:
//		- float x: The x component.
//		- float y: The y component.
//		- float z: The z component.
// Outputs:
//		- None.
//

TCVector3D::TCVector3D( float inX, float inY, float inZ )
{
	x = inX;
	y = inY;
	z = inZ;
}

//
// Dot
//		- This function will return the dot or inner product of two vectors.
// Inputs:
//		- const TCVector3D& in: The vector to dot with.
// Outputs:
//		- float: Dot products yield a scalar equal to the cosine between the two vectors,
//				 multiplied by the lengths of the two vectors.
//

float TCVector3D::Dot( const TCVector3D& in )
{
	return (x * in.x) + (y * in.y) + (z * in.z);
}

//
// Cross 
//		- This function will return an orthogonal vector to the input and this vector.
// Inputs:
//		- const TCVector3D& in: The vector to cross with.
// Outputs:
//		- float: The orthogonal vector to the input and this vector.
//

TCVector3D TCVector3D::Cross( const TCVector3D& in )
{
	return TCVector3D( (y * in.z) - (z * in.y),
					   (z * in.x) - (x * in.z),
					   (x * in.y) - (y * in.x) );
}

//
// IsPerpendicular
//		- This function will return if this vector and the input vector are perpendicular
// Inputs:
//		- const TCVector3D& in: The vector to check against.
// Outputs:
//		- bool: True if the two vectors are perpendicular. False if not.
//

bool TCVector3D::IsPerpendicular( const TCVector3D& in )
{
	return Dot( in ) == 0.0f;
}

//
// IsFacingSameDirectionAs
//		- This function will return if this vector and the input vector are facing the same direction.
// Inputs:
//		- const TCVector3D& in: The vector to check against.
// Outputs:
//		- bool: Will return if the two vectors are facing the same direction.
//

bool TCVector3D::IsFacingSameDirectionAs( const TCVector3D& in )
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

float TCVector3D::Length()
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

float TCVector3D::LengthSquared()
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

void TCVector3D::Normalize()
{
	float magnitude = Length();
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
}

//
// Angle
//		- Will return the angle in radians between the two vectors.
// Inputs:
//		- const TCVector2D& in: the vector to determine the angle between.
// Outputs:
//		- float: The angle between the two vectors.
//

float TCVector3D::Angle( const TCVector3D& in )
{
	TCVector3D thisNormalized = *this;
	thisNormalized.Normalize();

	TCVector3D inNormalized = in;
	inNormalized.Normalize();

	return TCMathUtils::Acos( thisNormalized.Dot( inNormalized ) );
}