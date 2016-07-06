//
// TCMathUtils.cpp
// This file will define various math related utility functions.
//

//
// Includes
//

#include "TCMathUtils.h"
#include <math.h>

//
// Defines
//

namespace TCMathUtils
{

	//
	// IsPrime
	//		- Will return if the value provided is a prime number or not.
	// Inputs:
	//		- int candidate: The number to check.
	// Outputs:
	//		- bool: Is the number a prime value or not.
	//

	bool IsPrime( int candidate )
	{
		if ((candidate & 1) != 0) 
        {
            int limit = (int)sqrt((double)candidate);
            for (int divisor = 3; divisor <= limit; divisor += 2)
            {
                if ((candidate % divisor) == 0)
                    return false;
            }
            return true;
        }
        return (candidate == 2);
	}

	//
	// Sqrt
	//		- Will return the square root of a value
	// Inputs:
	//		- float/int/double in
	// Outputs:
	//		- float/int/double: The square root of the value.
	//

	float Sqrt( float in )
	{
		return sqrt( in );
	}

	int Sqrt( int in )
	{
		return (int)sqrt( (double)in );
	}

	double Sqrt( double in )
	{
		return sqrt( in );
	}

	//
	// Acos
	//		- Will return the arc cosine of a value theta.
	// Inputs:
	//		- float/int/double theta.
	// Outputs:
	//		- float/int/double: The angle in radians.
	//

	float Acos( float theta )
	{
		return acos( theta );
	}

	int Acos( int theta )
	{
		return (int)acos( (double)theta );
	}

	double Acos( double theta )
	{
		return acos( theta );	
	}

	//
	// Log10
	//		- Will return the base ten logarith of a value.
	// Inputs:
	//		- float/int/double n: The number to get the logarithm of.
	// Outputs:
	//		- float/int/double n: The logaraith of the number.
	//

	float Log10( float n )
	{
		if( n <= 0 )
			return NAN;

		return log10( n );	
	}

	double Log10( double n )
	{
		if( n <= 0 )
			return NAN;

		return log10( n );	
	}

	int Log10( int n )
	{
		if( n <= 0 )
			return (int)NAN;

		return (int)log10( (double)n );	
	}

	//
	// Pow
	//		- Will raise a value to a specific exponent.
	// Inputs
	//		- float/int/double: the value to raise.
	// Outputs:
	//		- float/int/double: The raised value.
	//

	float Pow( float value, int exponent )
	{
		return pow( value, exponent );
	}

	int Pow( int value, int exponent )
	{
		return (int)pow( (double)value, exponent );
	}

	double Pow( double value, int exponent )
	{
		return pow( value, exponent );
	}

	//
	// Floor
	//		- Round a value down.
	// Inputs:
	//		- float/double: The value to round down.
	// Outputs:
	//		- float/double: The rounded value.
	//

	float Floor( float value )
	{
		return floor( value );
	}

	double Floor( double value )
	{
		return floor( value );
	}

	//
	// Ceil
	//		- Round a value up.
	// Inputs:
	//		- float/double: The value to round up.
	// Outputs:
	//		- float/double: The rounded value.
	//

	float Ceil( float value )
	{
		return ceil( value );
	}

	double Ceil( double value )
	{
		return ceil( value );
	}
}	
