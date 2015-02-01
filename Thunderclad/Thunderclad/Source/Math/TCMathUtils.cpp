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
}	
