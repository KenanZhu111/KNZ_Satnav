#ifndef _XYZ2BLH_H_
#define _XYZ2BLH_H_

#include<stdlib.h>
#include<math.h>

#include"public.h"

typedef struct 
{   
  double B;
  double L;
  double H;  
}xyz2blh;
/* -------------------------------------------------------------------------- */
/// @brief Convert ECEF geostationary coordinates to BLH coordinates
/// @param xyz2blh parameter transfer structure
/// @param X X coordinates of ECEF
/// @param Y Y coordinates of ECEF
/// @param Z Z coordinates of ECEF
/// @param a major semiaxis of WGS-84 ellipsoid
/// @param e2 the square of the first eccentricity of WGS-84 ellipsoid
/// @return return type of structure contain B, L, H members
/* -------------------------------------------------------------------------- */
xyz2blh XYZ2BLH(xyz2blh xyz2blh, 
                  double X, double Y, double Z)

{
    double B = 0.0, N = 0.0, H = 0.0, R0, R1, deltaH, deltaB;
    R0 = sqrt(pow(X, 2) + pow(Y, 2));
    R1 = sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
    xyz2blh.L = atan2(Y, X);
    N = a;
    H = R1 - a;
    B = atan2(Z * (N + H), R0 * (N * (1 - e2) + H));
    do
    {
        deltaH = N;
        deltaB = B;
		N = a / sqrt(1 - e2 * pow(sin(B), 2));
		H = R0 / cos(B) - N;
		B = atan2(Z * (N + H), R0 * (N * (1 - e2) + H));
	} while (fabs(deltaH - H) > 1.0e-3 && fabs(deltaB - B) > 1.0e-9);
	xyz2blh.B = B;
	xyz2blh.H = H;

    return xyz2blh;
}
    
#endif