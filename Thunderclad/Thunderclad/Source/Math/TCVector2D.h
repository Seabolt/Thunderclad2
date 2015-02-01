//
// TCVector2D.h
// This file will define a two dimensional vector.
//

#ifndef __TC_VECTOR_2D_H__
#define __TC_VECTOR_2D_H__

//
// Includes
//

#include "TCPoint2D.h"

//
// Defines
//

//
// Class Declaration
//

class TCVector2D : public TCPoint2D
{
public:	// Members
public: // Methods
	float		Dot( const TCVector2D& in );
	float		Cross( const TCVector2D& in );
	
	bool		IsPerpendicular( const TCVector2D& in );
	bool		IsFacingSameDirectionAs( const TCVector2D& in );

	float		Length();
	float		LengthSquared();

	void		Normalize();
	float		Angle( const TCVector2D& in );
};

#endif // __TC_VECTOR_2D_H__