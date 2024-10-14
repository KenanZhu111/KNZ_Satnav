#ifndef _RAHCAL_H_
#define _RAHCAL_H_

#include<stdlib.h>
#include<math.h>

#include"public.h"

typedef struct 
{
    double R;
    double A;
    double H;
}rahcal;
/* -------------------------------------------------------------------------- */
/// @brief satellite azimuth angle calculation
/// @param rahcal parameter transfer structure
/// @param E east
/// @param N north
/// @param U up
/// @return return structure contain R, A, H members                      
/* -------------------------------------------------------------------------- */
rahcal RAHCAL(rahcal rahcal, 
              double E, double N, double U)
{
    rahcal.H = atan2(U, sqrt(E * E + N * N));
    rahcal.A = atan2(E, U);
    if (rahcal.A < 0)
        rahcal.A += 2 * PI;
    if (rahcal.A > 2 * PI)
        rahcal.A -= 2 * PI;
    rahcal.R = sqrt(E * E + N * N + U * U);

    return rahcal;
}

#endif