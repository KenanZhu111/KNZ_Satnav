#ifndef _DEG2DMS_H_
#define _DEG2DMS_H_

#include<stdlib.h>
#include<math.h>

typedef struct
{
    double D;
    double M;
    double S;    
}deg2dms;
/* -------------------------------------------------------------------------- */
/// @brief Converts decimal degrees to degree minutes and seconds
/// @param deg2dms Parameter transfer structure
/// @param D The number of degrees to convert
/// @return return structure contain degree minute second                      
/* -------------------------------------------------------------------------- */
deg2dms DEG2DMS(deg2dms deg2dms,
                double D)
{
    int sig = D / fabs(D);//sign judgment
    deg2dms.D = (int)(D);//to degree
    deg2dms.M = (int)((D - deg2dms.D) * 60);//to minute
    deg2dms.S = (int)(((D - deg2dms.D) * 60 - deg2dms.M) * 60);//to second
    deg2dms.M  = deg2dms.M * sig;
    deg2dms.S  = deg2dms.S * sig;

    return deg2dms;
}

#endif