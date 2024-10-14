#ifndef _DEGRRAD_H_
#define _DEGRRAD_H_

#include"public.h"
/* -------------------------------------------------------------------------- */
/// @brief degree to radian
/// @param deg 
/// @return return radian
/* -------------------------------------------------------------------------- */
double deg2rad(double deg)
{
    double rad = (PI / 180) * deg;
    return rad;
}

/* -------------------------------------------------------------------------- */
/// @brief radian to degree                      
/// @param rad 
/// @return return degree
/* -------------------------------------------------------------------------- */
double rad2deg(double rad)
{
    double deg = (180 / PI) * rad;
    return deg;
}

#endif