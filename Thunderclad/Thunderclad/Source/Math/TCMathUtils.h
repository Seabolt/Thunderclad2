//
// TCMathUtils.h
// This file will define various helpful math functions.
//

#ifndef __TCMATH_UTILITIES_H__
#define __TCMATH_UTILITIES_H__

#include <math.h>

#define TC_MIN( x, y )						(x < y ? x : y)
#define TC_MAX( x, y )						(x > y ? x : y)
#define TC_CLAMP( min, max, value )			( value < min ? (value > max ? max : value) : min)

namespace TCMathUtils
{
	bool	IsPrime( int candidate );
	
	float	Sqrt( float in );
	int		Sqrt( int in );
	double	Sqrt( double in );

	float	Acos( float in );
	double	Acos( double in );
	int		Acos( int in );

	float	Log10( float in );
	int		Log10( int in );
	double	Log10( double in );

	float	Pow( float in, int exp );
	int		Pow( int pow, int exp );
	double	Pow( double pow, int exp );

	float	Floor( float in );
	double	Floor( double in );

	float	Ceil( float in );
	double	Ceil( double in );
}

#endif