#ifndef __TCBOUNDINGBOX2D_H__
#define __TCBOUNDINGBOX2D_H__

//
// Include
//

#include "TCPoint2D.h"

//
// Defines
//

//
// TCBoundingBox2D
// This class will define a two dimensional box that encapsulates an object. It is defined by a center and it's extents.
//

class TCBoundingBox2D
{
	public:		// Methods
		TCBoundingBox2D();
		TCBoundingBox2D( TCPoint2D& center, TCPoint2D& extents );
		TCBoundingBox2D( const TCBoundingBox2D& ref );
		TCBoundingBox2D& operator=(const TCBoundingBox2D& ref );

		bool Contains( TCPoint2D& point ) const;
		bool Contains( TCBoundingBox2D& box ) const;
		bool Intersects( TCBoundingBox2D& box ) const;

		float GetWidth()	{ return extents.x * 2.0f; }
		float GetHeight()	{ return extents.y * 2.0f; }

		TCPoint2D GetMin()	{ return center - extents; }
		TCPoint2D GetMax()	{ return center + extents; }

	public:		// Members
		TCPoint2D center;
		TCPoint2D extents;
};



#endif