//
// TCVector3D.h
// This file will define a three dimensional vector.
//

#ifndef __TC_VECTOR_3D_H__
#define __TC_VECTOR_3D_H__

//
// Includes
//

#include "TCPoint3D.h"

//
// Defines
//

//
// Class Declaration
//

class TCVector3D : public TCPoint3D
{
public:	// Members
public: // Methods
				TCVector3D();
				TCVector3D( const TCVector3D& vec );
				TCVector3D( float x, float y, float z );

	float		Dot( const TCVector3D& in );
	TCVector3D	Cross( const TCVector3D& in );
	
	bool		IsPerpendicular( const TCVector3D& in );
	bool		IsFacingSameDirectionAs( const TCVector3D& in );

	float		Length();
	float		LengthSquared();

	void		Normalize();
	float		Angle( const TCVector3D& in );
};

#endif // __TC_VECTOR_2D_H__