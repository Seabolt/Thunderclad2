//
// TCBoundingBox2D.cpp
// This file will define the functionality of a 2D bounding box.
//

//
// Includes
//

#include "TCBoundingBox2D.h"

//
// Default Constructor
//

TCBoundingBox2D::TCBoundingBox2D()
{
	center.Set( 0.0f, 0.0f );
	extents.Set( 0.1f, 0.1f );
}

//
// Memberwise Constructor
//

TCBoundingBox2D::TCBoundingBox2D( TCPoint2D& inCenter, TCPoint2D& inExtents )
{
	center = inCenter;
	extents = inExtents;
}

//
// Copy Constructor
//

TCBoundingBox2D::TCBoundingBox2D( const TCBoundingBox2D& ref )
{
	center = ref.center;
	extents = ref.extents;
}

//
// Assignment Operator
//

TCBoundingBox2D& TCBoundingBox2D::operator=( const TCBoundingBox2D& ref )
{
	center = ref.center;
	extents = ref.extents;

	return *this;
}

//
// Contains
// This method will determine if a point provided is inside this bounding box.
//

bool TCBoundingBox2D::Contains( TCPoint2D& point ) const
{
	if( point.x > center.x + extents.x )
		return false;

	if( point.x < center.x - extents.x )
		return false;

	if( point.y > center.y + extents.y )
		return false;

	if( point.y < center.y - extents.y )
		return false;

	return true;
}

//
// Contains
// This method will determine if a bounding box is within this bounding box.
//

bool TCBoundingBox2D::Contains( TCBoundingBox2D& box ) const
{
	//
	// If we contain the box's min and max, then we contain the box.
	//

	if( !Contains( box.GetMin() ) )
		return false;

	if( !Contains( box.GetMax() ) )
		return false;

	return true;
}

//
// Intersects
// This method will determine if a bounding box is intersected by another bounding box.
//

bool TCBoundingBox2D::Intersects( TCBoundingBox2D& box ) const
{
	// If it's completely contained then it doesn't intersect.
	if( Contains( box ) )
		return false;

	// If we contain any corner then we must intersect.
	if( Contains( box.GetMin() ) || Contains( box.GetMax() ) )
		return true;

	// Otherwise we don't contain the box.
	return false;
}