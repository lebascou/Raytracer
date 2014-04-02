/*!
    Copyright 1994, 2002 by Steven Worley
    This software may be modified and redistributed without restriction
    provided this comment header remains intact in the source code.
    This code is provided with no warrantee, express or implied, for
    any purpose.
    
    A detailed description and application examples can be found in the
    1996 SIGGRAPH paper "A Cellular Texture Basis Function" and
    especially in the 2002 book "Texturing and Modeling, a Procedural
    Approach, 3rd edition." There is also extra information on the web
    site http://www.worley.com/cellular.html .
    
    If you do find interesting uses for this tool, and especially if
    you enhance it, please drop me an email at steve@worley.com.
*/
#ifndef CSE168_WORLEY_H_INCLUDED
#define CSE168_WORLEY_H_INCLUDED

#include "Vector3.h"

//! Worley noise class
/*!
    An implementation of the key cellular texturing basis
    function. This function is hardwired to return an average F_1 value
    of 1.0. It returns the <n> closest feature point distances
    F_1, F_2, .. F_n the vector delta to those points, and a 32 bit
    seed for each of the feature points.  This function is not
    difficult to extend to compute alternative information such as
    higher order F values, to use the Manhattan distance metric, or
    other fun perversions.
    
    <at>    The input sample location. 
    <maxOrder>  Smaller values compute faster. < 5, read the book to extend it.
    <F>     The output values of F_1, F_2, ..F[n] in F[0], F[1], F[n-1]
    <delta> The output vector difference between the sample point and the n-th
                closest feature point. Thus, the feature point's location is the
                hit point minus this value. The DERIVATIVE of F is the unit
                normalized version of this vector.
    <ID>    The output 32 bit ID number which labels the feature point. This
                is useful for domain partitions, especially for coloring flagstone
                patterns.
    
    This implementation is tuned for speed in a way that any order > 5
    will likely have discontinuous artifacts in its computation of F5+.
    This can be fixed by increasing the internal points-per-cube
    density in the source code, at the expense of slower
    computation. The book lists the details of this tuning.
*/
class WorleyNoise
{
public:
	static float EuclidianDistanceFunc(Vector3 p1, Vector3 p2)
	{
		return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z);
	}
    // 1D
    static void noise1D(float at, int maxOrder,
                float *F, float (*delta), unsigned int *ID);
    
    // 2D
    static void noise2D(float at[2], int maxOrder,
                float *F, float (*delta)[2], unsigned int *ID);
    
    // 3D
    static void noise3D(float at[3], int maxOrder,
                float *F, float (*delta)[3], unsigned int *ID); 
};

#endif // CSE168_WORLEY_H_INCLUDED
